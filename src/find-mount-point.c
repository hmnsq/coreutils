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

#अन्तर्भाव <config.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "save-cwd.h"
#अन्तर्भाव "xgetcwd.h"
#अन्तर्भाव "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
बाह्य अक्षर *
find_mount_point (अक्षर स्थिरांक *file, संरचना stat स्थिरांक *file_stat)
{
  संरचना saved_cwd cwd;
  संरचना stat last_stat;
  अक्षर *mp = रिक्त;		/* The malloc'd mount point.  */

  जर (save_cwd (&cwd) != 0)
    {
      error (0, त्रुटि_सं, _("cannot get current directory"));
      परत रिक्त;
    }

  जर (S_ISDIR (file_stat->st_mode))
    /* FILE is a directory, so just chdir there directly.  */
    {
      last_stat = *file_stat;
      जर (स_बदलो (file) < 0)
        {
          error (0, त्रुटि_सं, _("cannot change to directory %s"), quoteaf (file));
          परत रिक्त;
        }
    }
  अन्यथा
    /* FILE is some other kind of file; use its directory.  */
    {
      अक्षर *dir = dir_name (file);

      जर (स_बदलो (dir) < 0)
        {
          error (0, त्रुटि_सं, _("cannot change to directory %s"), quoteaf (dir));
          मुक्त (dir);
          परत रिक्त;
        }

      जर (stat (".", &last_stat) < 0)
        {
          error (0, त्रुटि_सं, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          मुक्त (dir);
          जा done;
        }

      मुक्त (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  पर्यंत (true)
    {
      संरचना stat st;
      जर (stat ("..", &st) < 0)
        {
          error (0, त्रुटि_सं, _("cannot stat %s"), quoteaf (".."));
          जा done;
        }
      जर (st.st_dev != last_stat.st_dev || st.st_ino == last_stat.st_ino)
        /* cwd is the mount point.  */
        थांबा;
      जर (स_बदलो ("..") < 0)
        {
          error (0, त्रुटि_सं, _("cannot change to directory %s"), quoteaf (".."));
          जा done;
        }
      last_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

done:
  /* Restore the original cwd.  */
  {
    पूर्णांक save_errno = त्रुटि_सं;
    जर (restore_cwd (&cwd) != 0)
      error (निकास_त्रुटि, त्रुटि_सं,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    त्रुटि_सं = save_errno;
  }

  परत mp;
}
