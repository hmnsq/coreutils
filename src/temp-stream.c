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

#شامل <config.h>

#شامل <stdio.h>

#شامل "stdlib--.h"  /* For mkstemp that returns safer FDs.  */
#شامل "system.h"
#شامل "tmpdir.h"

#شامل "temp-stream.h"


#اگر defined __MSDOS__ || defined _WIN32
/* Define this to non-zero on systems for which the regular mechanism
   (of unlinking an open file and expecting to be able to write, seek
   back to the beginning, then reread it) doesn't work.  E.g., on Windows
   and DOS systems.  */
# define DONT_UNLINK_WHILE_OPEN 1
#اگر_ختم

#اگر DONT_UNLINK_WHILE_OPEN

/* FIXME-someday: remove all of this DONT_UNLINK_WHILE_OPEN junk.
   Using atexit like this is wrong, since it can fail
   when called e.g. 32 or more times.
   But this isn't a big deal, since the code is used only on WOE/DOS
   systems, and few people invoke tac on that many nonseekable files.  */

ساکن حرف ثابت *file_مقصد_remove;
ساکن FILE *fp_مقصد_close;

ساکن خالی
unlink_tempfile (خالی)
{
  fclose (fp_مقصد_close);
  unlink (file_مقصد_remove);
}

ساکن خالی
ریکارڈ_or_unlink_tempfile (حرف ثابت *fn, FILE *fp)
{
  اگر (!file_مقصد_remove)
    {
      file_مقصد_remove = fn;
      fp_مقصد_close = fp;
      atexit (unlink_tempfile);
    }
}

#ورنہ

ساکن خالی
ریکارڈ_or_unlink_tempfile (حرف ثابت *fn, MAYBE_غیر_مستعمل FILE *fp)
{
  unlink (fn);
}

#اگر_ختم

/* A wrapper around mkstemp that gives us both an open stream pointer,
   FP, and the corresponding FILE_NAME.  Always return the same FP/name
   pair, rewinding/truncating it upon each reuse.

   Note this honors $TMPDIR, unlike the standard defined tmpfile().

   Returns TRUE on success.  */
bool
عارضی_دھارا (FILE **fp, حرف **file_name)
{
  ساکن حرف *tempfile = تہی;
  ساکن FILE *tmp_fp;
  اگر (tempfile == تہی)
    {
      حرف *tempbuf = تہی;
      size_t tempbuf_len = 128;

      جب (true)
        {
          اگر (! (tempbuf = realloc (tempbuf, tempbuf_len)))
            {
              error (0, errno, _("failed to make temporary file name"));
              واپس false;
            }

          اگر (راستہ_تلاش (tempbuf, tempbuf_len, تہی, "cutmp", true) == 0)
            توڑو;

          اگر (errno != EINVAL || PATH_MAX / 2 < tempbuf_len)
            {
              error (0, errno == EINVAL ? ENAMETOOLONG : errno,
                     _("failed to make temporary file name"));
              واپس false;
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

      صحیح fd = mkstemp (tempfile);
      اگر (fd < 0)
        {
          error (0, errno, _("failed to create temporary file %s"),
                 quoteaf (tempfile));
          جاؤ Reset;
        }

      tmp_fp = fdopen (fd, (O_ثنائی ? "w+b" : "w+"));
      اگر (! tmp_fp)
        {
          error (0, errno, _("failed to open %s for writing"),
                 quoteaf (tempfile));
          close (fd);
          unlink (tempfile);
        Reset:
          آزاد (tempfile);
          tempfile = تہی;
          واپس false;
        }

      ریکارڈ_or_unlink_tempfile (tempfile, tmp_fp);
    }
  ورنہ
    {
      clearerr (tmp_fp);
      اگر (fseeko (tmp_fp, 0, SEEK_SET) < 0
          || ftruncate (fileno (tmp_fp), 0) < 0)
        {
          error (0, errno, _("failed to rewind stream for %s"),
                 quoteaf (tempfile));
          واپس false;
        }
    }

  *fp = tmp_fp;
  اگر (file_name)
    *file_name = tempfile;
  واپس true;
}
