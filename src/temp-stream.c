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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ "stdlib--.h"  /* For mkstemp that returns safer FDs.  */
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "tmpdir.h"

#ಅನ್ತರ್ಭಾವ "temp-stream.h"


#ಆದರೆ defined __MSDOS__ || defined _WIN32
/* Define this to non-zero on systems for which the regular mechanism
   (of unlinking an open file and expecting to be able to write, seek
   back to the beginning, then reread it) doesn't work.  E.g., on Windows
   and DOS systems.  */
# define DONT_UNLINK_WHILE_OPEN 1
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ DONT_UNLINK_WHILE_OPEN

/* FIXME-someday: remove all of this DONT_UNLINK_WHILE_OPEN junk.
   Using atexit like this is wrong, since it can fail
   when called e.g. 32 or more times.
   But this isn't a big deal, since the code is used only on WOE/DOS
   systems, and few people invoke tac on that many nonseekable files.  */

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_ಗಮ್ಯ_remove;
ಸ್ಥಿರ FILE *fp_ಗಮ್ಯ_close;

ಸ್ಥಿರ ಶೂನ್ಯ
unlink_tempfile (ಶೂನ್ಯ)
{
  fclose (fp_ಗಮ್ಯ_close);
  unlink (file_ಗಮ್ಯ_remove);
}

ಸ್ಥಿರ ಶೂನ್ಯ
ದಾಖಲೆ_or_unlink_tempfile (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fn, FILE *fp)
{
  ಆದರೆ (!file_ಗಮ್ಯ_remove)
    {
      file_ಗಮ್ಯ_remove = fn;
      fp_ಗಮ್ಯ_close = fp;
      atexit (unlink_tempfile);
    }
}

#ಇಲ್ಲದಿದ್ದರೆ

ಸ್ಥಿರ ಶೂನ್ಯ
ದಾಖಲೆ_or_unlink_tempfile (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fn, MAYBE_ಬಳಸದ FILE *fp)
{
  unlink (fn);
}

#ಆದರೆ_ಅಂತ್ಯ

/* A wrapper around mkstemp that gives us both an open stream pointer,
   FP, and the corresponding FILE_NAME.  Always return the same FP/name
   pair, rewinding/truncating it upon each reuse.

   Note this honors $TMPDIR, unlike the standard defined tmpfile().

   Returns TRUE on success.  */
bool
ತಾತ್ಕಾಲಿಕ_ಹರಿವು (FILE **fp, ಅಕ್ಷರ **file_name)
{
  ಸ್ಥಿರ ಅಕ್ಷರ *tempfile = ಖಾಲಿ;
  ಸ್ಥಿರ FILE *tmp_fp;
  ಆದರೆ (tempfile == ಖಾಲಿ)
    {
      ಅಕ್ಷರ *tempbuf = ಖಾಲಿ;
      size_t tempbuf_len = 128;

      ಆಗ (true)
        {
          ಆದರೆ (! (tempbuf = realloc (tempbuf, tempbuf_len)))
            {
              error (0, errno, _("failed to make temporary file name"));
              ಹಿಂತಿರುಗಿ false;
            }

          ಆದರೆ (ಮಾರ್ಗ_ಹುಡುಕು (tempbuf, tempbuf_len, ಖಾಲಿ, "cutmp", true) == 0)
            ನಿಲ್ಲಿಸು;

          ಆದರೆ (errno != EINVAL || PATH_MAX / 2 < tempbuf_len)
            {
              error (0, errno == EINVAL ? ENAMETOOLONG : errno,
                     _("failed to make temporary file name"));
              ಹಿಂತಿರುಗಿ false;
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

      ಪೂರ್ಣಾಂಕ fd = mkstemp (tempfile);
      ಆದರೆ (fd < 0)
        {
          error (0, errno, _("failed to create temporary file %s"),
                 quoteaf (tempfile));
          ಹೋಗು Reset;
        }

      tmp_fp = fdopen (fd, (O_ದ್ವಿಮಾನ ? "w+b" : "w+"));
      ಆದರೆ (! tmp_fp)
        {
          error (0, errno, _("failed to open %s for writing"),
                 quoteaf (tempfile));
          close (fd);
          unlink (tempfile);
        Reset:
          ಬಿಡುಗಡೆ (tempfile);
          tempfile = ಖಾಲಿ;
          ಹಿಂತಿರುಗಿ false;
        }

      ದಾಖಲೆ_or_unlink_tempfile (tempfile, tmp_fp);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      clearerr (tmp_fp);
      ಆದರೆ (fseeko (tmp_fp, 0, SEEK_SET) < 0
          || ftruncate (fileno (tmp_fp), 0) < 0)
        {
          error (0, errno, _("failed to rewind stream for %s"),
                 quoteaf (tempfile));
          ಹಿಂತಿರುಗಿ false;
        }
    }

  *fp = tmp_fp;
  ಆದರೆ (file_name)
    *file_name = tempfile;
  ಹಿಂತಿರುಗಿ true;
}
