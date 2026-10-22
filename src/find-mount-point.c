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

#شامل <config.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "save-cwd.h"
#شامل "xgetcwd.h"
#شامل "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
خارجی حرف *
ڈھونڈو_mount_نقطہ (حرف ثابت *file, ساخت stat ثابت *file_stat)
{
  ساخت saved_cwd cwd;
  ساخت stat آخری_stat;
  حرف *mp = تہی;		/* The malloc'd mount point.  */

  اگر (save_cwd (&cwd) != 0)
    {
      error (0, errno, _("cannot get current directory"));
      واپس تہی;
    }

  اگر (S_ISDIR (file_stat->st_منوال))
    /* FILE is a directory, so just chdir there directly.  */
    {
      آخری_stat = *file_stat;
      اگر (chdir (file) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (file));
          واپس تہی;
        }
    }
  ورنہ
    /* FILE is some other kind of file; use its directory.  */
    {
      حرف *dir = dir_name (file);

      اگر (chdir (dir) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (dir));
          آزاد (dir);
          واپس تہی;
        }

      اگر (stat (".", &آخری_stat) < 0)
        {
          error (0, errno, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          آزاد (dir);
          جاؤ مکمل;
        }

      آزاد (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  جب (true)
    {
      ساخت stat st;
      اگر (stat ("..", &st) < 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (".."));
          جاؤ مکمل;
        }
      اگر (st.st_dev != آخری_stat.st_dev || st.st_ino == آخری_stat.st_ino)
        /* cwd is the mount point.  */
        توڑو;
      اگر (chdir ("..") < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (".."));
          جاؤ مکمل;
        }
      آخری_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

مکمل:
  /* Restore the original cwd.  */
  {
    صحیح save_errno = errno;
    اگر (restore_cwd (&cwd) != 0)
      error (EXIT_FAILURE, errno,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    errno = save_errno;
  }

  واپس mp;
}
