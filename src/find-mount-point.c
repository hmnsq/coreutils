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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "save-cwd.h"
#ଅନ୍ତର୍ଭାଵ "xgetcwd.h"
#ଅନ୍ତର୍ଭାଵ "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
ବାହ୍ୟ ଅକ୍ଷର *
find_mount_point (ଅକ୍ଷର ଅଚଳ *file, ସଂରଚନା stat ଅଚଳ *file_stat)
{
  ସଂରଚନା saved_cwd cwd;
  ସଂରଚନା stat last_stat;
  ଅକ୍ଷର *mp = ଖାଲି;		/* The malloc'd mount point.  */

  ଯଦି (save_cwd (&cwd) != 0)
    {
      error (0, errno, _("cannot get current directory"));
      ଫେରନ୍ତୁ ଖାଲି;
    }

  ଯଦି (S_ISDIR (file_stat->st_mode))
    /* FILE is a directory, so just chdir there directly.  */
    {
      last_stat = *file_stat;
      ଯଦି (chdir (file) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (file));
          ଫେରନ୍ତୁ ଖାଲି;
        }
    }
  ଅନ୍ୟଥା
    /* FILE is some other kind of file; use its directory.  */
    {
      ଅକ୍ଷର *dir = dir_name (file);

      ଯଦି (chdir (dir) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (dir));
          ମୁକ୍ତ (dir);
          ଫେରନ୍ତୁ ଖାଲି;
        }

      ଯଦି (stat (".", &last_stat) < 0)
        {
          error (0, errno, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          ମୁକ୍ତ (dir);
          ଯାଅ done;
        }

      ମୁକ୍ତ (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  ଯେତେବେଳେ (true)
    {
      ସଂରଚନା stat st;
      ଯଦି (stat ("..", &st) < 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (".."));
          ଯାଅ done;
        }
      ଯଦି (st.st_dev != last_stat.st_dev || st.st_ino == last_stat.st_ino)
        /* cwd is the mount point.  */
        ଭାଙ୍ଗ;
      ଯଦି (chdir ("..") < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (".."));
          ଯାଅ done;
        }
      last_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

done:
  /* Restore the original cwd.  */
  {
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା save_errno = errno;
    ଯଦି (restore_cwd (&cwd) != 0)
      error (EXIT_FAILURE, errno,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    errno = save_errno;
  }

  ଫେରନ୍ତୁ mp;
}
