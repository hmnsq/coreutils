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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "save-cwd.h"
#অন্তর্ভাব "xgetcwd.h"
#অন্তর্ভাব "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
বাহ্য বর্ণ *
find_mount_point (বর্ণ ধ্রুব *file, সংরচনা stat ধ্রুব *file_stat)
{
  সংরচনা saved_cwd cwd;
  সংরচনা stat last_stat;
  বর্ণ *mp = রিক্ত;		/* The malloc'd mount point.  */

  যদি (save_cwd (&cwd) != 0)
    {
      error (0, errno, _("cannot get current directory"));
      প্রত্যাগম রিক্ত;
    }

  যদি (S_ISDIR (file_stat->st_mode))
    /* FILE is a directory, so just chdir there directly.  */
    {
      last_stat = *file_stat;
      যদি (chdir (file) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (file));
          প্রত্যাগম রিক্ত;
        }
    }
  অন্যথা
    /* FILE is some other kind of file; use its directory.  */
    {
      বর্ণ *dir = dir_name (file);

      যদি (chdir (dir) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (dir));
          মুক্ত (dir);
          প্রত্যাগম রিক্ত;
        }

      যদি (stat (".", &last_stat) < 0)
        {
          error (0, errno, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          মুক্ত (dir);
          গচ্ছ done;
        }

      মুক্ত (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  যাবত্ (true)
    {
      সংরচনা stat st;
      যদি (stat ("..", &st) < 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (".."));
          গচ্ছ done;
        }
      যদি (st.st_dev != last_stat.st_dev || st.st_ino == last_stat.st_ino)
        /* cwd is the mount point.  */
        ভঙ্গ;
      যদি (chdir ("..") < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (".."));
          গচ্ছ done;
        }
      last_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

done:
  /* Restore the original cwd.  */
  {
    পূর্ণাঙ্ক save_errno = errno;
    যদি (restore_cwd (&cwd) != 0)
      error (EXIT_FAILURE, errno,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    errno = save_errno;
  }

  প্রত্যাগম mp;
}
