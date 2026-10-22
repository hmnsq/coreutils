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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "save-cwd.h"
#அந்தர்பாவ "xgetcwd.h"
#அந்தர்பாவ "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
வெளி எழுத்து *
find_mount_point (எழுத்து மாறிலி *file, கட்டமைப்பு stat மாறிலி *file_stat)
{
  கட்டமைப்பு saved_cwd cwd;
  கட்டமைப்பு stat last_stat;
  எழுத்து *mp = வெற்று;		/* The malloc'd mount point.  */

  எனில் (save_cwd (&cwd) != 0)
    {
      error (0, errno, _("cannot get current directory"));
      திரும்பு வெற்று;
    }

  எனில் (S_ISDIR (file_stat->st_mode))
    /* FILE is a directory, so just chdir there directly.  */
    {
      last_stat = *file_stat;
      எனில் (chdir (file) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (file));
          திரும்பு வெற்று;
        }
    }
  இல்லையெனில்
    /* FILE is some other kind of file; use its directory.  */
    {
      எழுத்து *dir = dir_name (file);

      எனில் (chdir (dir) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (dir));
          விடுவி (dir);
          திரும்பு வெற்று;
        }

      எனில் (stat (".", &last_stat) < 0)
        {
          error (0, errno, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          விடுவி (dir);
          செல் done;
        }

      விடுவி (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  போது (true)
    {
      கட்டமைப்பு stat st;
      எனில் (stat ("..", &st) < 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (".."));
          செல் done;
        }
      எனில் (st.st_dev != last_stat.st_dev || st.st_ino == last_stat.st_ino)
        /* cwd is the mount point.  */
        நிறுத்து;
      எனில் (chdir ("..") < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (".."));
          செல் done;
        }
      last_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

done:
  /* Restore the original cwd.  */
  {
    முழு save_errno = errno;
    எனில் (restore_cwd (&cwd) != 0)
      error (EXIT_FAILURE, errno,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    errno = save_errno;
  }

  திரும்பு mp;
}
