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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "save-cwd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xgetcwd.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
ਬਾਹਰੀ ਅੱਖਰ *
ਲੱਭੋ_mount_ਬਿੰਦੂ (ਅੱਖਰ ਅਚਲ *file, ਬਣਤਰ stat ਅਚਲ *file_stat)
{
  ਬਣਤਰ saved_cwd cwd;
  ਬਣਤਰ stat ਆਖਰੀ_stat;
  ਅੱਖਰ *mp = ਨਿਰਾ;		/* The malloc'd mount point.  */

  ਜੇ (save_cwd (&cwd) != 0)
    {
      error (0, errno, _("cannot get current directory"));
      ਵਾਪਸ ਨਿਰਾ;
    }

  ਜੇ (S_ISDIR (file_stat->st_ਬਹੁਲਕ))
    /* FILE is a directory, so just chdir there directly.  */
    {
      ਆਖਰੀ_stat = *file_stat;
      ਜੇ (chdir (file) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (file));
          ਵਾਪਸ ਨਿਰਾ;
        }
    }
  ਨਹੀਂਤਾਂ
    /* FILE is some other kind of file; use its directory.  */
    {
      ਅੱਖਰ *dir = dir_name (file);

      ਜੇ (chdir (dir) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (dir));
          ਮੁਕਤ (dir);
          ਵਾਪਸ ਨਿਰਾ;
        }

      ਜੇ (stat (".", &ਆਖਰੀ_stat) < 0)
        {
          error (0, errno, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          ਮੁਕਤ (dir);
          ਜਾਓ ਪੂਰਾ;
        }

      ਮੁਕਤ (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  ਜਦੋਂ (true)
    {
      ਬਣਤਰ stat st;
      ਜੇ (stat ("..", &st) < 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (".."));
          ਜਾਓ ਪੂਰਾ;
        }
      ਜੇ (st.st_dev != ਆਖਰੀ_stat.st_dev || st.st_ino == ਆਖਰੀ_stat.st_ino)
        /* cwd is the mount point.  */
        ਰੋਕੋ;
      ਜੇ (chdir ("..") < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (".."));
          ਜਾਓ ਪੂਰਾ;
        }
      ਆਖਰੀ_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

ਪੂਰਾ:
  /* Restore the original cwd.  */
  {
    ਪੂਰਨ save_errno = errno;
    ਜੇ (restore_cwd (&cwd) != 0)
      error (EXIT_FAILURE, errno,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    errno = save_errno;
  }

  ਵਾਪਸ mp;
}
