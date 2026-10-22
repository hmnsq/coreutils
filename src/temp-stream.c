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

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>

#అన్తర్భావ "stdlib--.h"  /* For mkstemp that returns safer FDs.  */
#అన్తర్భావ "system.h"
#అన్తర్భావ "tmpdir.h"

#అన్తర్భావ "temp-stream.h"


#ఒకవేళ defined __MSDOS__ || defined _WIN32
/* Define this to non-zero on systems for which the regular mechanism
   (of unlinking an open file and expecting to be able to write, seek
   back to the beginning, then reread it) doesn't work.  E.g., on Windows
   and DOS systems.  */
# define DONT_UNLINK_WHILE_OPEN 1
#అయితే_ముగింపు

#ఒకవేళ DONT_UNLINK_WHILE_OPEN

/* FIXME-someday: remove all of this DONT_UNLINK_WHILE_OPEN junk.
   Using atexit like this is wrong, since it can fail
   when called e.g. 32 or more times.
   But this isn't a big deal, since the code is used only on WOE/DOS
   systems, and few people invoke tac on that many nonseekable files.  */

స్థిర అక్షరం స్థిరరాశి *file_గమ్యం_remove;
స్థిర FILE *fp_గమ్యం_close;

స్థిర శూన్య
unlink_tempfile (శూన్య)
{
  fclose (fp_గమ్యం_close);
  unlink (file_గమ్యం_remove);
}

స్థిర శూన్య
రికార్డు_or_unlink_tempfile (అక్షరం స్థిరరాశి *fn, FILE *fp)
{
  ఒకవేళ (!file_గమ్యం_remove)
    {
      file_గమ్యం_remove = fn;
      fp_గమ్యం_close = fp;
      atexit (unlink_tempfile);
    }
}

#లేకపోతే

స్థిర శూన్య
రికార్డు_or_unlink_tempfile (అక్షరం స్థిరరాశి *fn, MAYBE_వాడని FILE *fp)
{
  unlink (fn);
}

#అయితే_ముగింపు

/* A wrapper around mkstemp that gives us both an open stream pointer,
   FP, and the corresponding FILE_NAME.  Always return the same FP/name
   pair, rewinding/truncating it upon each reuse.

   Note this honors $TMPDIR, unlike the standard defined tmpfile().

   Returns TRUE on success.  */
bool
తాత్కాలిక_ప్రవాహం (FILE **fp, అక్షరం **file_name)
{
  స్థిర అక్షరం *tempfile = ఖాళీ;
  స్థిర FILE *tmp_fp;
  ఒకవేళ (tempfile == ఖాళీ)
    {
      అక్షరం *tempbuf = ఖాళీ;
      size_t tempbuf_len = 128;

      అయితే (true)
        {
          ఒకవేళ (! (tempbuf = realloc (tempbuf, tempbuf_len)))
            {
              error (0, errno, _("failed to make temporary file name"));
              తిరిగి false;
            }

          ఒకవేళ (మార్గం_శోధన (tempbuf, tempbuf_len, ఖాళీ, "cutmp", true) == 0)
            ఆపు;

          ఒకవేళ (errno != EINVAL || PATH_MAX / 2 < tempbuf_len)
            {
              error (0, errno == EINVAL ? ENAMETOOLONG : errno,
                     _("failed to make temporary file name"));
              తిరిగి false;
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

      పూర్ణాంకం fd = mkstemp (tempfile);
      ఒకవేళ (fd < 0)
        {
          error (0, errno, _("failed to create temporary file %s"),
                 quoteaf (tempfile));
          వెళ్ళు Reset;
        }

      tmp_fp = fdopen (fd, (O_ద్విమాన ? "w+b" : "w+"));
      ఒకవేళ (! tmp_fp)
        {
          error (0, errno, _("failed to open %s for writing"),
                 quoteaf (tempfile));
          close (fd);
          unlink (tempfile);
        Reset:
          విడుదల (tempfile);
          tempfile = ఖాళీ;
          తిరిగి false;
        }

      రికార్డు_or_unlink_tempfile (tempfile, tmp_fp);
    }
  లేకపోతే
    {
      clearerr (tmp_fp);
      ఒకవేళ (fseeko (tmp_fp, 0, SEEK_SET) < 0
          || ftruncate (fileno (tmp_fp), 0) < 0)
        {
          error (0, errno, _("failed to rewind stream for %s"),
                 quoteaf (tempfile));
          తిరిగి false;
        }
    }

  *fp = tmp_fp;
  ఒకవేళ (file_name)
    *file_name = tempfile;
  తిరిగి true;
}
