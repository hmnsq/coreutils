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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "stdlib--.h"  /* For mkstemp that returns safer FDs.  */
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "tmpdir.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "temp-stream.h"


#ਜੇ defined __MSDOS__ || defined _WIN32
/* Define this to non-zero on systems for which the regular mechanism
   (of unlinking an open file and expecting to be able to write, seek
   back to the beginning, then reread it) doesn't work.  E.g., on Windows
   and DOS systems.  */
# define DONT_UNLINK_WHILE_OPEN 1
#ਜੇ_ਅੰਤ

#ਜੇ DONT_UNLINK_WHILE_OPEN

/* FIXME-someday: remove all of this DONT_UNLINK_WHILE_OPEN junk.
   Using atexit like this is wrong, since it can fail
   when called e.g. 32 or more times.
   But this isn't a big deal, since the code is used only on WOE/DOS
   systems, and few people invoke tac on that many nonseekable files.  */

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *file_ਮੰਜ਼ਿਲ_remove;
ਸਥਿਰ FILE *fp_ਮੰਜ਼ਿਲ_close;

ਸਥਿਰ ਖਾਲੀ
unlink_tempfile (ਖਾਲੀ)
{
  fclose (fp_ਮੰਜ਼ਿਲ_close);
  unlink (file_ਮੰਜ਼ਿਲ_remove);
}

ਸਥਿਰ ਖਾਲੀ
ਰਿਕਾਰਡ_or_unlink_tempfile (ਅੱਖਰ ਅਚਲ *fn, FILE *fp)
{
  ਜੇ (!file_ਮੰਜ਼ਿਲ_remove)
    {
      file_ਮੰਜ਼ਿਲ_remove = fn;
      fp_ਮੰਜ਼ਿਲ_close = fp;
      atexit (unlink_tempfile);
    }
}

#ਨਹੀਂਤਾਂ

ਸਥਿਰ ਖਾਲੀ
ਰਿਕਾਰਡ_or_unlink_tempfile (ਅੱਖਰ ਅਚਲ *fn, MAYBE_ਅਣਵਰਤਿਆ FILE *fp)
{
  unlink (fn);
}

#ਜੇ_ਅੰਤ

/* A wrapper around mkstemp that gives us both an open stream pointer,
   FP, and the corresponding FILE_NAME.  Always return the same FP/name
   pair, rewinding/truncating it upon each reuse.

   Note this honors $TMPDIR, unlike the standard defined tmpfile().

   Returns TRUE on success.  */
bool
ਆਰਜ਼ੀ_ਧਾਰਾ (FILE **fp, ਅੱਖਰ **file_name)
{
  ਸਥਿਰ ਅੱਖਰ *tempfile = ਨਿਰਾ;
  ਸਥਿਰ FILE *tmp_fp;
  ਜੇ (tempfile == ਨਿਰਾ)
    {
      ਅੱਖਰ *tempbuf = ਨਿਰਾ;
      size_t tempbuf_len = 128;

      ਜਦੋਂ (true)
        {
          ਜੇ (! (tempbuf = realloc (tempbuf, tempbuf_len)))
            {
              error (0, errno, _("failed to make temporary file name"));
              ਵਾਪਸ false;
            }

          ਜੇ (ਰਸਤਾ_ਖੋਜ (tempbuf, tempbuf_len, ਨਿਰਾ, "cutmp", true) == 0)
            ਰੋਕੋ;

          ਜੇ (errno != EINVAL || PATH_MAX / 2 < tempbuf_len)
            {
              error (0, errno == EINVAL ? ENAMETOOLONG : errno,
                     _("failed to make temporary file name"));
              ਵਾਪਸ false;
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

      ਪੂਰਨ fd = mkstemp (tempfile);
      ਜੇ (fd < 0)
        {
          error (0, errno, _("failed to create temporary file %s"),
                 quoteaf (tempfile));
          ਜਾਓ Reset;
        }

      tmp_fp = fdopen (fd, (O_ਦੁਆਧਾਰੀ ? "w+b" : "w+"));
      ਜੇ (! tmp_fp)
        {
          error (0, errno, _("failed to open %s for writing"),
                 quoteaf (tempfile));
          close (fd);
          unlink (tempfile);
        Reset:
          ਮੁਕਤ (tempfile);
          tempfile = ਨਿਰਾ;
          ਵਾਪਸ false;
        }

      ਰਿਕਾਰਡ_or_unlink_tempfile (tempfile, tmp_fp);
    }
  ਨਹੀਂਤਾਂ
    {
      clearerr (tmp_fp);
      ਜੇ (fseeko (tmp_fp, 0, SEEK_SET) < 0
          || ftruncate (fileno (tmp_fp), 0) < 0)
        {
          error (0, errno, _("failed to rewind stream for %s"),
                 quoteaf (tempfile));
          ਵਾਪਸ false;
        }
    }

  *fp = tmp_fp;
  ਜੇ (file_name)
    *file_name = tempfile;
  ਵਾਪਸ true;
}
