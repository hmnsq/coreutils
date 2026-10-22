/* find-mount-point.c -- find the root mount point for a file.
   Copyright (C) 2010-2026 Free Software Foundation, Inc.

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
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "save-cwd.h"
#समावेश "xgetcwd.h"
#समावेश "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
बाह्य अक्षर *
find_mount_point (अक्षर स्थिर *file, काष्ठा stat स्थिर *file_stat)
{
  काष्ठा saved_cwd cwd;
  काष्ठा stat last_stat;
  अक्षर *mp = शून्य;		/* The malloc'd mount point.  */

  अगर (save_cwd (&cwd) != 0)
    {
      error (0, त्रुटि_सं, _("cannot get current directory"));
      वापस शून्य;
    }

  अगर (S_ISDIR (file_stat->st_mode))
    /* FILE is a directory, so just chdir there directly.  */
    {
      last_stat = *file_stat;
      अगर (स_बदलो (file) < 0)
        {
          error (0, त्रुटि_सं, _("cannot change to directory %s"), quoteaf (file));
          वापस शून्य;
        }
    }
  अन्यथा
    /* FILE is some other kind of file; use its directory.  */
    {
      अक्षर *dir = dir_name (file);

      अगर (स_बदलो (dir) < 0)
        {
          error (0, त्रुटि_सं, _("cannot change to directory %s"), quoteaf (dir));
          मुक्त (dir);
          वापस शून्य;
        }

      अगर (stat (".", &last_stat) < 0)
        {
          error (0, त्रुटि_सं, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          मुक्त (dir);
          जाओ done;
        }

      मुक्त (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  जबतक (true)
    {
      काष्ठा stat st;
      अगर (stat ("..", &st) < 0)
        {
          error (0, त्रुटि_सं, _("cannot stat %s"), quoteaf (".."));
          जाओ done;
        }
      अगर (st.st_dev != last_stat.st_dev || st.st_ino == last_stat.st_ino)
        /* cwd is the mount point.  */
        अवरोध;
      अगर (स_बदलो ("..") < 0)
        {
          error (0, त्रुटि_सं, _("cannot change to directory %s"), quoteaf (".."));
          जाओ done;
        }
      last_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

done:
  /* Restore the original cwd.  */
  {
    पूर्णांक save_errno = त्रुटि_सं;
    अगर (restore_cwd (&cwd) != 0)
      error (निकास_त्रुटि, त्रुटि_सं,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    त्रुटि_सं = save_errno;
  }

  वापस mp;
}
