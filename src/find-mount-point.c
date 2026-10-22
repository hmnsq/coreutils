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

#अन्तर्भाव <config.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "save-cwd.h"
#अन्तर्भाव "xgetcwd.h"
#अन्तर्भाव "find-mount-point.h"

/* Return the root mountpoint of the file system on which FILE exists, in
   malloced storage.  FILE_STAT should be the result of stating FILE.
   Give a diagnostic and return NULL if unable to determine the mount point.
   Exit if unable to restore current working directory.  */
बाह्य वर्ण *
find_mount_point (वर्ण ध्रुव *file, संरचना stat ध्रुव *file_stat)
{
  संरचना saved_cwd cwd;
  संरचना stat चरमः_stat;
  वर्ण *mp = रिक्त;		/* The malloc'd mount point.  */

  यदि (save_cwd (&cwd) != 0)
    {
      error (0, errno, _("cannot get current directory"));
      प्रत्यागम रिक्त;
    }

  यदि (S_ISDIR (file_stat->st_बहुलकम्))
    /* FILE is a directory, so just chdir there directly.  */
    {
      चरमः_stat = *file_stat;
      यदि (chdir (file) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (file));
          प्रत्यागम रिक्त;
        }
    }
  अन्यथा
    /* FILE is some other kind of file; use its directory.  */
    {
      वर्ण *dir = dir_name (file);

      यदि (chdir (dir) < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (dir));
          मुक्त (dir);
          प्रत्यागम रिक्त;
        }

      यदि (stat (".", &चरमः_stat) < 0)
        {
          error (0, errno, _("cannot stat current directory (now %s)"),
                 quoteaf (dir));
          मुक्त (dir);
          गच्छ समाप्तम्;
        }

      मुक्त (dir);
    }

  /* Now walk up FILE's parents until we find another file system or /,
     chdiring as we go.  LAST_STAT holds stat information for the last place
     we visited.  */
  यावत् (true)
    {
      संरचना stat st;
      यदि (stat ("..", &st) < 0)
        {
          error (0, errno, _("cannot stat %s"), quoteaf (".."));
          गच्छ समाप्तम्;
        }
      यदि (st.st_dev != चरमः_stat.st_dev || st.st_ino == चरमः_stat.st_ino)
        /* cwd is the mount point.  */
        भङ्ग;
      यदि (chdir ("..") < 0)
        {
          error (0, errno, _("cannot change to directory %s"), quoteaf (".."));
          गच्छ समाप्तम्;
        }
      चरमः_stat = st;
    }

  /* Finally reached a mount point, see what it's called.  */
  mp = xgetcwd ();

समाप्तम्:
  /* Restore the original cwd.  */
  {
    पूर्णाङ्क save_errno = errno;
    यदि (restore_cwd (&cwd) != 0)
      error (EXIT_FAILURE, errno,
             _("failed to return to initial working directory"));
    free_cwd (&cwd);
    errno = save_errno;
  }

  प्रत्यागम mp;
}
