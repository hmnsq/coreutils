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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "save-cwd.h"
#അന്തര്ഭാവ "xgetcwd.h"
#അന്തര്ഭാവ "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
ബാഹ്യ അക്ഷരം *
find_mount_point (അക്ഷരം സ്ഥിരം *file, ഘടന stat സ്ഥിരം *file_stat)
{
  ഘടന saved_cwd cwd;
  ഘടന stat last_stat;
  അക്ഷരം *mp = ശൂന്യ;		/* The malloc'd mount point.  */

  എങ്കിൽ (save_cwd (&cwd) != 0)
    {
      error (0, errno, _("cannot get current directory"));
      തിരികെ ശൂന്യ;
    }

  എങ്കിൽ (S_ISDIR (file_stat->st_mode))
    /* FILE is a directory, so just chdir there directly.  */
    {
      last_stat = *file_stat;
      എങ്കിൽ (chdir (file) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (file));
          തിരികെ ശൂന്യ;
        }
    }
  അല്ലെങ്കിൽ
    /* FILE is some other kind of file; use its directory.  */
    {
      അക്ഷരം *dir = dir_name (file);

      എങ്കിൽ (chdir (dir) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (dir));
          സ്വതന്ത്രം (dir);
          തിരികെ ശൂന്യ;
        }

      എങ്കിൽ (stat (".", &last_stat) < 0)
        {
          error (0, errno, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          സ്വതന്ത്രം (dir);
          പോകുക done;
        }

      സ്വതന്ത്രം (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  ആകുമ്പോൾ (true)
    {
      ഘടന stat st;
      എങ്കിൽ (stat ("..", &st) < 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (".."));
          പോകുക done;
        }
      എങ്കിൽ (st.st_dev != last_stat.st_dev || st.st_ino == last_stat.st_ino)
        /* cwd is the mount point.  */
        നിർത്തുക;
      എങ്കിൽ (chdir ("..") < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (".."));
          പോകുക done;
        }
      last_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

done:
  /* Restore the original cwd.  */
  {
    പൂർണ്ണസംഖ്യ save_errno = errno;
    എങ്കിൽ (restore_cwd (&cwd) != 0)
      error (EXIT_FAILURE, errno,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    errno = save_errno;
  }

  തിരികെ mp;
}
