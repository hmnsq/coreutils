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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "save-cwd.h"
#અન્તર્ભાવ "xgetcwd.h"
#અન્તર્ભાવ "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
બાહ્ય અક્ષર *
find_mount_point (અક્ષર અચલ *file, સંરચના stat અચલ *file_stat)
{
  સંરચના saved_cwd cwd;
  સંરચના stat last_stat;
  અક્ષર *mp = ખાલી;		/* The malloc'd mount point.  */

  જો (save_cwd (&cwd) != 0)
    {
      error (0, errno, _("cannot get current directory"));
      પાછા ખાલી;
    }

  જો (S_ISDIR (file_stat->st_mode))
    /* FILE is a directory, so just chdir there directly.  */
    {
      last_stat = *file_stat;
      જો (chdir (file) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (file));
          પાછા ખાલી;
        }
    }
  નહીંતર
    /* FILE is some other kind of file; use its directory.  */
    {
      અક્ષર *dir = dir_name (file);

      જો (chdir (dir) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (dir));
          મુક્ત (dir);
          પાછા ખાલી;
        }

      જો (stat (".", &last_stat) < 0)
        {
          error (0, errno, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          મુક્ત (dir);
          જાઓ done;
        }

      મુક્ત (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  જ્યારે (true)
    {
      સંરચના stat st;
      જો (stat ("..", &st) < 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (".."));
          જાઓ done;
        }
      જો (st.st_dev != last_stat.st_dev || st.st_ino == last_stat.st_ino)
        /* cwd is the mount point.  */
        તોડો;
      જો (chdir ("..") < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (".."));
          જાઓ done;
        }
      last_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

done:
  /* Restore the original cwd.  */
  {
    પૂર્ણાંક save_errno = errno;
    જો (restore_cwd (&cwd) != 0)
      error (EXIT_FAILURE, errno,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    errno = save_errno;
  }

  પાછા mp;
}
