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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "save-cwd.h"
#ಅನ್ತರ್ಭಾವ "xgetcwd.h"
#ಅನ್ತರ್ಭಾವ "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
ಬಾಹ್ಯ ಅಕ್ಷರ *
ಕಂಡುಹಿಡಿ_mount_ಬಿಂದು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, ರಚನೆ stat ಸ್ಥಿರಾಂಕ *file_stat)
{
  ರಚನೆ saved_cwd cwd;
  ರಚನೆ stat ಕೊನೆಯ_stat;
  ಅಕ್ಷರ *mp = ಖಾಲಿ;		/* The malloc'd mount point.  */

  ಆದರೆ (save_cwd (&cwd) != 0)
    {
      error (0, errno, _("cannot get current directory"));
      ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

  ಆದರೆ (S_ISDIR (file_stat->st_ಬಹುಲಕ))
    /* FILE is a directory, so just chdir there directly.  */
    {
      ಕೊನೆಯ_stat = *file_stat;
      ಆದರೆ (chdir (file) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (file));
          ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    /* FILE is some other kind of file; use its directory.  */
    {
      ಅಕ್ಷರ *dir = dir_name (file);

      ಆದರೆ (chdir (dir) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (dir));
          ಬಿಡುಗಡೆ (dir);
          ಹಿಂತಿರುಗಿ ಖಾಲಿ;
        }

      ಆದರೆ (stat (".", &ಕೊನೆಯ_stat) < 0)
        {
          error (0, errno, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          ಬಿಡುಗಡೆ (dir);
          ಹೋಗು ಮುಗಿದ;
        }

      ಬಿಡುಗಡೆ (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  ಆಗ (true)
    {
      ರಚನೆ stat st;
      ಆದರೆ (stat ("..", &st) < 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (".."));
          ಹೋಗು ಮುಗಿದ;
        }
      ಆದರೆ (st.st_dev != ಕೊನೆಯ_stat.st_dev || st.st_ino == ಕೊನೆಯ_stat.st_ino)
        /* cwd is the mount point.  */
        ನಿಲ್ಲಿಸು;
      ಆದರೆ (chdir ("..") < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (".."));
          ಹೋಗು ಮುಗಿದ;
        }
      ಕೊನೆಯ_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

ಮುಗಿದ:
  /* Restore the original cwd.  */
  {
    ಪೂರ್ಣಾಂಕ save_errno = errno;
    ಆದರೆ (restore_cwd (&cwd) != 0)
      error (EXIT_FAILURE, errno,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    errno = save_errno;
  }

  ಹಿಂತಿರುಗಿ mp;
}
