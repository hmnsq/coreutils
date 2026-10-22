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

#ضمن <config.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "save-cwd.h"
#ضمن "xgetcwd.h"
#ضمن "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
خارجي حرف *
find_mount_point (حرف ثابت *file, بنية stat ثابت *file_stat)
{
  بنية saved_cwd cwd;
  بنية stat last_stat;
  حرف *mp = عدم;		/* The malloc'd mount point.  */

  𐡄𐡍 (save_cwd (&cwd) != 0)
    {
      error (0, errno, _("cannot get current directory"));
      أرجع عدم;
    }

  𐡄𐡍 (S_ISDIR (file_stat->st_mode))
    /* FILE is a directory, so just chdir there directly.  */
    {
      last_stat = *file_stat;
      𐡄𐡍 (chdir (file) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (file));
          أرجع عدم;
        }
    }
  وإلا
    /* FILE is some other kind of file; use its directory.  */
    {
      حرف *dir = dir_name (file);

      𐡄𐡍 (chdir (dir) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (dir));
          حرر (dir);
          أرجع عدم;
        }

      𐡄𐡍 (stat (".", &last_stat) < 0)
        {
          error (0, errno, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          حرر (dir);
          اذهب done;
        }

      حرر (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  طالما (true)
    {
      بنية stat st;
      𐡄𐡍 (stat ("..", &st) < 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (".."));
          اذهب done;
        }
      𐡄𐡍 (st.st_dev != last_stat.st_dev || st.st_ino == last_stat.st_ino)
        /* cwd is the mount point.  */
        اكسر;
      𐡄𐡍 (chdir ("..") < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (".."));
          اذهب done;
        }
      last_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

done:
  /* Restore the original cwd.  */
  {
    صحيح save_errno = errno;
    𐡄𐡍 (restore_cwd (&cwd) != 0)
      error (EXIT_FAILURE, errno,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    errno = save_errno;
  }

  أرجع mp;
}
