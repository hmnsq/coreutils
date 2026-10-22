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

#include <config.h>
#include <sys/types.h>

#include "system.h"
#include "save-cwd.h"
#include "xgetcwd.h"
#include "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
extern var_na *
find_mount_point (var_na const *file, sa_nracanaa stat const *file_stat)
{
  sa_nracanaa saved_cwd cwd;
  sa_nracanaa stat last_stat;
  var_na *mp = NULL;		/* The malloc'd mount point.  */

  ប្រសិនបើ (save_cwd (&cwd) != 0)
    {
      error (0, errno, _("cannot get current directory"));
      ត្រឡប់ NULL;
    }

  ប្រសិនបើ (S_ISDIR (file_stat->st_mode))
    /* FILE is a directory, so just chdir there directly.  */
    {
      last_stat = *file_stat;
      ប្រសិនបើ (chdir (file) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (file));
          ត្រឡប់ NULL;
        }
    }
  បើមិនដូច្នេះ
    /* FILE is some other kind of file; use its directory.  */
    {
      var_na *dir = dir_name (file);

      ប្រសិនបើ (chdir (dir) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (dir));
          free (dir);
          ត្រឡប់ NULL;
        }

      ប្រសិនបើ (stat (".", &last_stat) < 0)
        {
          error (0, errno, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          free (dir);
          goto done;
        }

      free (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  ខណៈពេល (true)
    {
      sa_nracanaa stat st;
      ប្រសិនបើ (stat ("..", &st) < 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (".."));
          goto done;
        }
      ប្រសិនបើ (st.st_dev != last_stat.st_dev || st.st_ino == last_stat.st_ino)
        /* cwd is the mount point.  */
        bha_nga;
      ប្រសិនបើ (chdir ("..") < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (".."));
          goto done;
        }
      last_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

done:
  /* Restore the original cwd.  */
  {
    ចំនួនគត់ save_errno = errno;
    ប្រសិនបើ (restore_cwd (&cwd) != 0)
      error (EXIT_FAILURE, errno,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    errno = save_errno;
  }

  ត្រឡប់ mp;
}
