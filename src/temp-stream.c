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

#समावेश <config.h>

#समावेश <stdio.h>

#समावेश "stdlib--.h"  /* For mkstemp that returns safer FDs.  */
#समावेश "system.h"
#समावेश "tmpdir.h"

#समावेश "temp-stream.h"


#अगर defined __MSDOS__ || defined _WIN32
/* Define this to non-zero on systems for which the regular mechanism
   (of unlinking an open file and expecting to be able to write, seek
   back to the beginning, then reread it) doesn't work.  E.g., on Windows
   and DOS systems.  */
# define DONT_UNLINK_WHILE_OPEN 1
#पूर्ण_अगर

#अगर DONT_UNLINK_WHILE_OPEN

/* FIXME-someday: remove all of this DONT_UNLINK_WHILE_OPEN junk.
   Using atexit like this is wrong, since it can fail
   when called e.g. 32 or more times.
   But this isn't a big deal, since the code is used only on WOE/DOS
   systems, and few people invoke tac on that many nonseekable files.  */

अटल अक्षर स्थिर *file_to_remove;
अटल खाता *fp_to_close;

अटल व्योम
unlink_tempfile (व्योम)
{
  ख_बंद (fp_to_close);
  unlink (file_to_remove);
}

अटल व्योम
record_or_unlink_tempfile (अक्षर स्थिर *fn, खाता *fp)
{
  अगर (!file_to_remove)
    {
      file_to_remove = fn;
      fp_to_close = fp;
      निकास_पर (unlink_tempfile);
    }
}

#अन्यथा

अटल व्योम
record_or_unlink_tempfile (अक्षर स्थिर *fn, MAYBE_UNUSED खाता *fp)
{
  unlink (fn);
}

#पूर्ण_अगर

/* A wrapper around mkstemp that gives us both an open stream pointer,
   FP, and the corresponding FILE_NAME.  Always return the same FP/name
   pair, rewinding/truncating it upon each reuse.

   Note this honors $TMPDIR, unlike the standard defined tmpfile().

   Returns TRUE on success.  */
bool
अस्थायी_stream (खाता **fp, अक्षर **file_name)
{
  अटल अक्षर *tempfile = शून्य;
  अटल खाता *tmp_fp;
  अगर (tempfile == शून्य)
    {
      अक्षर *tempbuf = शून्य;
      माप_प्रकार tempbuf_len = 128;

      जबतक (true)
        {
          अगर (! (tempbuf = पुनः_स्मृति (tempbuf, tempbuf_len)))
            {
              error (0, त्रुटि_सं, _("failed to make temporary file name"));
              वापस false;
            }

          अगर (path_खोज (tempbuf, tempbuf_len, शून्य, "cutmp", true) == 0)
            अवरोध;

          अगर (त्रुटि_सं != EINVAL || PATH_MAX / 2 < tempbuf_len)
            {
              error (0, त्रुटि_सं == EINVAL ? ENAMETOOLONG : त्रुटि_सं,
                     _("failed to make temporary file name"));
              वापस false;
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

      पूर्णांक fd = mkstemp (tempfile);
      अगर (fd < 0)
        {
          error (0, त्रुटि_सं, _("failed to create temporary file %s"),
                 quoteaf (tempfile));
          जाओ Reset;
        }

      tmp_fp = fdopen (fd, (O_द्विआधारी ? "w+b" : "w+"));
      अगर (! tmp_fp)
        {
          error (0, त्रुटि_सं, _("failed to open %s for writing"),
                 quoteaf (tempfile));
          बंद (fd);
          unlink (tempfile);
        Reset:
          मुक्त (tempfile);
          tempfile = शून्य;
          वापस false;
        }

      record_or_unlink_tempfile (tempfile, tmp_fp);
    }
  अन्यथा
    {
      साफ_त्रुटि (tmp_fp);
      अगर (fseeko (tmp_fp, 0, शुरू_से) < 0
          || ftruncate (fileno (tmp_fp), 0) < 0)
        {
          error (0, त्रुटि_सं, _("failed to rewind stream for %s"),
                 quoteaf (tempfile));
          वापस false;
        }
    }

  *fp = tmp_fp;
  अगर (file_name)
    *file_name = tempfile;
  वापस true;
}
