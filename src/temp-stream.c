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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>

#அந்தர்பாவ "stdlib--.h"  /* For mkstemp that returns safer FDs.  */
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "tmpdir.h"

#அந்தர்பாவ "temp-stream.h"


#எனில் defined __MSDOS__ || defined _WIN32
/* Define this to non-zero on systems for which the regular mechanism
   (of unlinking an open file and expecting to be able to write, seek
   back to the beginning, then reread it) doesn't work.  E.g., on Windows
   and DOS systems.  */
# define DONT_UNLINK_WHILE_OPEN 1
#endif

#எனில் DONT_UNLINK_WHILE_OPEN

/* FIXME-someday: remove all of this DONT_UNLINK_WHILE_OPEN junk.
   Using atexit like this is wrong, since it can fail
   when called e.g. 32 or more times.
   But this isn't a big deal, since the code is used only on WOE/DOS
   systems, and few people invoke tac on that many nonseekable files.  */

நிலையான எழுத்து மாறிலி *file_to_remove;
நிலையான FILE *fp_to_close;

நிலையான வெறுமை
unlink_tempfile (வெறுமை)
{
  fclose (fp_to_close);
  unlink (file_to_remove);
}

நிலையான வெறுமை
record_or_unlink_tempfile (எழுத்து மாறிலி *fn, FILE *fp)
{
  எனில் (!file_to_remove)
    {
      file_to_remove = fn;
      fp_to_close = fp;
      atexit (unlink_tempfile);
    }
}

#இல்லையெனில்

நிலையான வெறுமை
record_or_unlink_tempfile (எழுத்து மாறிலி *fn, MAYBE_UNUSED FILE *fp)
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
temp_stream (FILE **fp, எழுத்து **file_name)
{
  நிலையான எழுத்து *tempfile = வெற்று;
  நிலையான FILE *tmp_fp;
  எனில் (tempfile == வெற்று)
    {
      எழுத்து *tempbuf = வெற்று;
      size_t tempbuf_len = 128;

      போது (true)
        {
          எனில் (! (tempbuf = realloc (tempbuf, tempbuf_len)))
            {
              error (0, errno, _("failed to make temporary file name"));
              திரும்பு false;
            }

          எனில் (path_தேடல் (tempbuf, tempbuf_len, வெற்று, "cutmp", true) == 0)
            நிறுத்து;

          எனில் (errno != EINVAL || PATH_MAX / 2 < tempbuf_len)
            {
              error (0, errno == EINVAL ? ENAMETOOLONG : errno,
                     _("failed to make temporary file name"));
              திரும்பு false;
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

      முழு fd = mkstemp (tempfile);
      எனில் (fd < 0)
        {
          error (0, errno, _("failed to create temporary file %s"),
                 quoteaf (tempfile));
          செல் Reset;
        }

      tmp_fp = fdopen (fd, (O_இருமம் ? "w+b" : "w+"));
      எனில் (! tmp_fp)
        {
          error (0, errno, _("failed to open %s for writing"),
                 quoteaf (tempfile));
          close (fd);
          unlink (tempfile);
        Reset:
          விடுவி (tempfile);
          tempfile = வெற்று;
          திரும்பு false;
        }

      record_or_unlink_tempfile (tempfile, tmp_fp);
    }
  இல்லையெனில்
    {
      clearerr (tmp_fp);
      எனில் (fseeko (tmp_fp, 0, SEEK_SET) < 0
          || ftruncate (fileno (tmp_fp), 0) < 0)
        {
          error (0, errno, _("failed to rewind stream for %s"),
                 quoteaf (tempfile));
          திரும்பு false;
        }
    }

  *fp = tmp_fp;
  எனில் (file_name)
    *file_name = tempfile;
  திரும்பு true;
}
