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

#ضمن <config.h>

#ضمن <stdio.h>

#ضمن "stdlib--.h"  /* For mkstemp that returns safer FDs.  */
#ضمن "system.h"
#ضمن "tmpdir.h"

#ضمن "temp-stream.h"


#إذا defined __MSDOS__ || defined _WIN32
/* Define this to non-zero on systems for which the regular mechanism
   (of unlinking an open file and expecting to be able to write, seek
   back to the beginning, then reread it) doesn't work.  E.g., on Windows
   and DOS systems.  */
# define DONT_UNLINK_WHILE_OPEN 1
#جيڪڏهن_پورو

#إذا DONT_UNLINK_WHILE_OPEN

/* FIXME-someday: remove all of this DONT_UNLINK_WHILE_OPEN junk.
   Using atexit like this is wrong, since it can fail
   when called e.g. 32 or more times.
   But this isn't a big deal, since the code is used only on WOE/DOS
   systems, and few people invoke tac on that many nonseekable files.  */

ساكن حرف ثابت *file_منزل_remove;
ساكن FILE *fp_منزل_close;

ساكن فراغ
unlink_tempfile (فراغ)
{
  fclose (fp_منزل_close);
  unlink (file_منزل_remove);
}

ساكن فراغ
رڪارڊ_or_unlink_tempfile (حرف ثابت *fn, FILE *fp)
{
  إذا (!file_منزل_remove)
    {
      file_منزل_remove = fn;
      fp_منزل_close = fp;
      atexit (unlink_tempfile);
    }
}

#وإلا

ساكن فراغ
رڪارڊ_or_unlink_tempfile (حرف ثابت *fn, MAYBE_اڻ_استعمال_ٿيل FILE *fp)
{
  unlink (fn);
}

#جيڪڏهن_پورو

/* A wrapper around mkstemp that gives us both an open stream pointer,
   FP, and the corresponding FILE_NAME.  Always return the same FP/name
   pair, rewinding/truncating it upon each reuse.

   Note this honors $TMPDIR, unlike the standard defined tmpfile().

   Returns TRUE on success.  */
bool
عارضي_وهڪرو (FILE **fp, حرف **file_name)
{
  ساكن حرف *tempfile = عدم;
  ساكن FILE *tmp_fp;
  إذا (tempfile == عدم)
    {
      حرف *tempbuf = عدم;
      size_t tempbuf_len = 128;

      طالما (true)
        {
          إذا (! (tempbuf = realloc (tempbuf, tempbuf_len)))
            {
              error (0, errno, _("failed to make temporary file name"));
              أرجع false;
            }

          إذا (رستو_ڳولا (tempbuf, tempbuf_len, عدم, "cutmp", true) == 0)
            اكسر;

          إذا (errno != EINVAL || PATH_MAX / 2 < tempbuf_len)
            {
              error (0, errno == EINVAL ? ENAMETOOLONG : errno,
                     _("failed to make temporary file name"));
              أرجع false;
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

      صحيح fd = mkstemp (tempfile);
      إذا (fd < 0)
        {
          error (0, errno, _("failed to create temporary file %s"),
                 quoteaf (tempfile));
          اذهب Reset;
        }

      tmp_fp = fdopen (fd, (O_ٻٽو ? "w+b" : "w+"));
      إذا (! tmp_fp)
        {
          error (0, errno, _("failed to open %s for writing"),
                 quoteaf (tempfile));
          close (fd);
          unlink (tempfile);
        Reset:
          حرر (tempfile);
          tempfile = عدم;
          أرجع false;
        }

      رڪارڊ_or_unlink_tempfile (tempfile, tmp_fp);
    }
  وإلا
    {
      clearerr (tmp_fp);
      إذا (fseeko (tmp_fp, 0, SEEK_SET) < 0
          || ftruncate (fileno (tmp_fp), 0) < 0)
        {
          error (0, errno, _("failed to rewind stream for %s"),
                 quoteaf (tempfile));
          أرجع false;
        }
    }

  *fp = tmp_fp;
  إذا (file_name)
    *file_name = tempfile;
  أرجع true;
}
