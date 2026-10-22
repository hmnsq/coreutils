/* temp-stream.c -- provide a stream to a per process temp file

   Copyright (C) 2023-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <stdio.h>

#ଅନ୍ତର୍ଭାଵ "stdlib--.h"  /* For mkstemp that returns safer FDs.  */
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "tmpdir.h"

#ଅନ୍ତର୍ଭାଵ "temp-stream.h"


#ଯଦି defined __MSDOS__ || defined _WIN32
/* Define this to non-zero on systems for which the regular mechanism
   (of unlinking an open file and expecting to be able to write, seek
   back to the beginning, then reread it) doesn't work.  E.g., on Windows
   and DOS systems.  */
# define DONT_UNLINK_WHILE_OPEN 1
#endif

#ଯଦି DONT_UNLINK_WHILE_OPEN

/* FIXME-someday: remove all of this DONT_UNLINK_WHILE_OPEN junk.
   Using atexit like this is wrong, since it can fail
   when called e.g. 32 or more times.
   But this isn't a big deal, since the code is used only on WOE/DOS
   systems, and few people invoke tac on that many nonseekable files.  */

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *file_to_remove;
ସ୍ଥିର FILE *fp_to_close;

ସ୍ଥିର ଶୂନ୍ୟ
unlink_tempfile (ଶୂନ୍ୟ)
{
  fclose (fp_to_close);
  unlink (file_to_remove);
}

ସ୍ଥିର ଶୂନ୍ୟ
record_or_unlink_tempfile (ଅକ୍ଷର ଅଚଳ *fn, FILE *fp)
{
  ଯଦି (!file_to_remove)
    {
      file_to_remove = fn;
      fp_to_close = fp;
      atexit (unlink_tempfile);
    }
}

#ଅନ୍ୟଥା

ସ୍ଥିର ଶୂନ୍ୟ
record_or_unlink_tempfile (ଅକ୍ଷର ଅଚଳ *fn, MAYBE_UNUSED FILE *fp)
{
  unlink (fn);
}

#endif

/* A wrapper around mkstemp that gives us both an open stream pointer,
   FP, and the corresponding FILE_NAME.  Always return the same FP/name
   pair, rewinding/truncating it upon each reuse.

   Note this honors $TMPDIR, unlike the standard defined tmpfile().

   Returns TRUE on success.  */
bool
temp_stream (FILE **fp, ଅକ୍ଷର **file_name)
{
  ସ୍ଥିର ଅକ୍ଷର *tempfile = ଖାଲି;
  ସ୍ଥିର FILE *tmp_fp;
  ଯଦି (tempfile == ଖାଲି)
    {
      ଅକ୍ଷର *tempbuf = ଖାଲି;
      size_t tempbuf_len = 128;

      ଯେତେବେଳେ (true)
        {
          ଯଦି (! (tempbuf = realloc (tempbuf, tempbuf_len)))
            {
              error (0, errno, _("failed to make temporary file name"));
              ଫେରନ୍ତୁ false;
            }

          ଯଦି (path_ସନ୍ଧାନ (tempbuf, tempbuf_len, ଖାଲି, "cutmp", true) == 0)
            ଭାଙ୍ଗ;

          ଯଦି (errno != EINVAL || PATH_MAX / 2 < tempbuf_len)
            {
              error (0, errno == EINVAL ? ENAMETOOLONG : errno,
                     _("failed to make temporary file name"));
              ଫେରନ୍ତୁ false;
            }

          tempbuf_len *= 2;
        }

      tempfile = tempbuf;

      /* FIXME: there's a small window between a successful mkstemp call
         and the unlink that's performed by record_or_unlink_tempfile.
         If we're interrupted in that interval, this code fails to remove
         the temporary file.  On systems that define DONT_UNLINK_WHILE_OPEN,
         the window is much larger -- it extends to the atexit-called
         unlink_tempfile.
         FIXME: clean up upon fatal signal.  Don't block them, in case
         $TMPDIR is a remote file system.  */

      ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = mkstemp (tempfile);
      ଯଦି (fd < 0)
        {
          error (0, errno, _("failed to create temporary file %s"),
                 quoteaf (tempfile));
          ଯାଅ Reset;
        }

      tmp_fp = fdopen (fd, (O_ଦ୍ୱିମିକ ? "w+b" : "w+"));
      ଯଦି (! tmp_fp)
        {
          error (0, errno, _("failed to open %s for writing"),
                 quoteaf (tempfile));
          close (fd);
          unlink (tempfile);
        Reset:
          ମୁକ୍ତ (tempfile);
          tempfile = ଖାଲି;
          ଫେରନ୍ତୁ false;
        }

      record_or_unlink_tempfile (tempfile, tmp_fp);
    }
  ଅନ୍ୟଥା
    {
      clearerr (tmp_fp);
      ଯଦି (fseeko (tmp_fp, 0, SEEK_SET) < 0
          || ftruncate (fileno (tmp_fp), 0) < 0)
        {
          error (0, errno, _("failed to rewind stream for %s"),
                 quoteaf (tempfile));
          ଫେରନ୍ତୁ false;
        }
    }

  *fp = tmp_fp;
  ଯଦି (file_name)
    *file_name = tempfile;
  ଫେରନ୍ତୁ true;
}
