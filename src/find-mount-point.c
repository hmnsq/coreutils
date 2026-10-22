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

#అన్తర్భావ <config.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "save-cwd.h"
#అన్తర్భావ "xgetcwd.h"
#అన్తర్భావ "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
బాహ్య అక్షరం *
కనుగొను_mount_బిందువు (అక్షరం స్థిరరాశి *file, నిర్మాణం stat స్థిరరాశి *file_stat)
{
  నిర్మాణం saved_cwd cwd;
  నిర్మాణం stat చివరి_stat;
  అక్షరం *mp = ఖాళీ;		/* The malloc'd mount point.  */

  ఒకవేళ (save_cwd (&cwd) != 0)
    {
      error (0, errno, _("cannot get current directory"));
      తిరిగి ఖాళీ;
    }

  ఒకవేళ (S_ISDIR (file_stat->st_బహుళకం))
    /* FILE is a directory, so just chdir there directly.  */
    {
      చివరి_stat = *file_stat;
      ఒకవేళ (chdir (file) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (file));
          తిరిగి ఖాళీ;
        }
    }
  లేకపోతే
    /* FILE is some other kind of file; use its directory.  */
    {
      అక్షరం *dir = dir_name (file);

      ఒకవేళ (chdir (dir) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (dir));
          విడుదల (dir);
          తిరిగి ఖాళీ;
        }

      ఒకవేళ (stat (".", &చివరి_stat) < 0)
        {
          error (0, errno, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          విడుదల (dir);
          వెళ్ళు పూర్తి;
        }

      విడుదల (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  అయితే (true)
    {
      నిర్మాణం stat st;
      ఒకవేళ (stat ("..", &st) < 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (".."));
          వెళ్ళు పూర్తి;
        }
      ఒకవేళ (st.st_dev != చివరి_stat.st_dev || st.st_ino == చివరి_stat.st_ino)
        /* cwd is the mount point.  */
        ఆపు;
      ఒకవేళ (chdir ("..") < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (".."));
          వెళ్ళు పూర్తి;
        }
      చివరి_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

పూర్తి:
  /* Restore the original cwd.  */
  {
    పూర్ణాంకం save_errno = errno;
    ఒకవేళ (restore_cwd (&cwd) != 0)
      error (EXIT_FAILURE, errno,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    errno = save_errno;
  }

  తిరిగి mp;
}
