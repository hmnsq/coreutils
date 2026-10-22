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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>

#അന്തര്ഭാവ "stdlib--.h"  /* For mkstemp that returns safer FDs.  */
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "tmpdir.h"

#അന്തര്ഭാവ "temp-stream.h"


#എങ്കിൽ defined __MSDOS__ || defined _WIN32
/* Define this to non-zero on systems for which the regular mechanism
   (of unlinking an open file and expecting to be able to write, seek
   back to the beginning, then reread it) doesn't work.  E.g., on Windows
   and DOS systems.  */
# define DONT_UNLINK_WHILE_OPEN 1
#endif

#എങ്കിൽ DONT_UNLINK_WHILE_OPEN

/* FIXME-someday: remove all of this DONT_UNLINK_WHILE_OPEN junk.
   Using atexit like this is wrong, since it can fail
   when called e.g. 32 or more times.
   But this isn't a big deal, since the code is used only on WOE/DOS
   systems, and few people invoke tac on that many nonseekable files.  */

സ്ഥിതം അക്ഷരം സ്ഥിരം *file_to_remove;
സ്ഥിതം FILE *fp_to_close;

സ്ഥിതം ശൂന്യം
unlink_tempfile (ശൂന്യം)
{
  fclose (fp_to_close);
  unlink (file_to_remove);
}

സ്ഥിതം ശൂന്യം
record_or_unlink_tempfile (അക്ഷരം സ്ഥിരം *fn, FILE *fp)
{
  എങ്കിൽ (!file_to_remove)
    {
      file_to_remove = fn;
      fp_to_close = fp;
      atexit (unlink_tempfile);
    }
}

#അല്ലെങ്കിൽ

സ്ഥിതം ശൂന്യം
record_or_unlink_tempfile (അക്ഷരം സ്ഥിരം *fn, MAYBE_UNUSED FILE *fp)
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
temp_stream (FILE **fp, അക്ഷരം **file_name)
{
  സ്ഥിതം അക്ഷരം *tempfile = ശൂന്യ;
  സ്ഥിതം FILE *tmp_fp;
  എങ്കിൽ (tempfile == ശൂന്യ)
    {
      അക്ഷരം *tempbuf = ശൂന്യ;
      size_t tempbuf_len = 128;

      ആകുമ്പോൾ (true)
        {
          എങ്കിൽ (! (tempbuf = realloc (tempbuf, tempbuf_len)))
            {
              error (0, errno, _("failed to make temporary file name"));
              തിരികെ false;
            }

          എങ്കിൽ (path_തിരയൽ (tempbuf, tempbuf_len, ശൂന്യ, "cutmp", true) == 0)
            നിർത്തുക;

          എങ്കിൽ (errno != EINVAL || PATH_MAX / 2 < tempbuf_len)
            {
              error (0, errno == EINVAL ? ENAMETOOLONG : errno,
                     _("failed to make temporary file name"));
              തിരികെ false;
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

      പൂർണ്ണസംഖ്യ fd = mkstemp (tempfile);
      എങ്കിൽ (fd < 0)
        {
          error (0, errno, _("failed to create temporary file %s"),
                 quoteaf (tempfile));
          പോകുക Reset;
        }

      tmp_fp = fdopen (fd, (O_ദ്വിമാന ? "w+b" : "w+"));
      എങ്കിൽ (! tmp_fp)
        {
          error (0, errno, _("failed to open %s for writing"),
                 quoteaf (tempfile));
          close (fd);
          unlink (tempfile);
        Reset:
          സ്വതന്ത്രം (tempfile);
          tempfile = ശൂന്യ;
          തിരികെ false;
        }

      record_or_unlink_tempfile (tempfile, tmp_fp);
    }
  അല്ലെങ്കിൽ
    {
      clearerr (tmp_fp);
      എങ്കിൽ (fseeko (tmp_fp, 0, SEEK_SET) < 0
          || ftruncate (fileno (tmp_fp), 0) < 0)
        {
          error (0, errno, _("failed to rewind stream for %s"),
                 quoteaf (tempfile));
          തിരികെ false;
        }
    }

  *fp = tmp_fp;
  എങ്കിൽ (file_name)
    *file_name = tempfile;
  തിരികെ true;
}
