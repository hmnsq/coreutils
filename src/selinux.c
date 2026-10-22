/* selinux - core functions for maintaining SELinux labeling
   Copyright (C) 2012-2026 Free Software Foundation, Inc.

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

/* Written by Daniel Walsh <dwalsh@redhat.com> */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <selinux/label.h>
#ಅನ್ತರ್ಭಾವ <selinux/context.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "canonicalize.h"
#ಅನ್ತರ್ಭಾವ "xfts.h"
#ಅನ್ತರ್ಭಾವ "selinux.h"

#ಯದಿ HAVE_SELINUX_LABEL_H

# ಯದಿ ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
ಸ್ಥಿರ security_class_t
mode_to_security_class (mode_t m)
{

  ಯದಿ (S_ISREG (m))
    ಪ್ರತ್ಯಾಗಮ string_to_security_class ("file");
  ಯದಿ (S_ISDIR (m))
    ಪ್ರತ್ಯಾಗಮ string_to_security_class ("dir");
  ಯದಿ (S_ISCHR (m))
    ಪ್ರತ್ಯಾಗಮ string_to_security_class ("chr_file");
  ಯದಿ (S_ISBLK (m))
    ಪ್ರತ್ಯಾಗಮ string_to_security_class ("blk_file");
  ಯದಿ (S_ISFIFO (m))
    ಪ್ರತ್ಯಾಗಮ string_to_security_class ("fifo_file");
  ಯದಿ (S_ISLNK (m))
    ಪ್ರತ್ಯಾಗಮ string_to_security_class ("lnk_file");
  ಯದಿ (S_ISSOCK (m))
    ಪ್ರತ್ಯಾಗಮ string_to_security_class ("sock_file");

  errno = EINVAL;
  ಪ್ರತ್ಯಾಗಮ 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
computecon_raw (ವರ್ಣ ಧ್ರುವ *path, mode_t mode, ವರ್ಣ **con)
{
  ವರ್ಣ *scon_raw = ರಿಕ್ತ;
  ವರ್ಣ *tcon_raw = ರಿಕ್ತ;
  security_class_t tclass;
  ಪೂರ್ಣಾಙ್ಕ rc = -1;

  ವರ್ಣ *dir = dir_name (path);
  ಯದಿ (!dir)
    ಗಚ್ಛ quit;
  ಯದಿ (getcon_raw (&scon_raw) < 0)
    ಗಚ್ಛ quit;
  ಯದಿ (getfilecon_raw (dir, &tcon_raw) < 0)
    ಗಚ್ಛ quit;
  tclass = mode_to_security_class (mode);
  ಯದಿ (!tclass)
    ಗಚ್ಛ quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  ಪೂರ್ಣಾಙ್ಕ err = errno;
  ಮುಕ್ತ (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ಪ್ರತ್ಯಾಗಮ rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
ಪೂರ್ಣಾಙ್ಕ
defaultcon (ಸಂರಚನಾ selabel_handle *selabel_handle,
            ವರ್ಣ ಧ್ರುವ *path, mode_t mode)
{
  ಪೂರ್ಣಾಙ್ಕ rc = -1;
  ವರ್ಣ *scon_raw = ರಿಕ್ತ;
  ವರ್ಣ *tcon_raw = ರಿಕ್ತ;
  context_t scontext = ರಿಕ್ತ, tcontext = ರಿಕ್ತ;
  ವರ್ಣ ಧ್ರುವ *contype;
  ವರ್ಣ ಧ್ರುವ *constr;
  ವರ್ಣ *newpath = ರಿಕ್ತ;

  ಯದಿ (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      ಯದಿ (! newpath)
        ಗಚ್ಛ quit;
      path = newpath;
    }

  ಯದಿ (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      ಯದಿ (errno == ENOENT)
        errno = ENODATA;
      ಗಚ್ಛ quit;
    }
  ಯದಿ (computecon_raw (path, mode, &tcon_raw) < 0)
    ಗಚ್ಛ quit;
  ಯದಿ (!(scontext = context_new (scon_raw)))
    ಗಚ್ಛ quit;
  ಯದಿ (!(tcontext = context_new (tcon_raw)))
    ಗಚ್ಛ quit;

  ಯದಿ (!(contype = context_type_get (scontext)))
    ಗಚ್ಛ quit;
  ಯದಿ (context_type_set (tcontext, contype))
    ಗಚ್ಛ quit;
  ಯದಿ (!(constr = context_str (tcontext)))
    ಗಚ್ಛ quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  ಪೂರ್ಣಾಙ್ಕ err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  ಮುಕ್ತ (newpath);
  errno = err;
  ಪ್ರತ್ಯಾಗಮ rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
restorecon_private (ಸಂರಚನಾ selabel_handle *selabel_handle, ವರ್ಣ ಧ್ರುವ *path)
{
  ಪೂರ್ಣಾಙ್ಕ rc = -1;
  ಸಂರಚನಾ stat sb;
  ವರ್ಣ *scon_raw = ರಿಕ್ತ;
  ವರ್ಣ *tcon_raw = ರಿಕ್ತ;
  context_t scontext = ರಿಕ್ತ, tcontext = ರಿಕ್ತ;
  ವರ್ಣ ಧ್ರುವ *contype;
  ವರ್ಣ ಧ್ರುವ *constr;
  ಪೂರ್ಣಾಙ್ಕ fd;

  ಯದಿ (!selabel_handle)
    {
      ಯದಿ (getfscreatecon_raw (&tcon_raw) < 0)
        ಪ್ರತ್ಯಾಗಮ rc;
      ಯದಿ (!tcon_raw)
        {
          errno = ENODATA;
          ಪ್ರತ್ಯಾಗಮ rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      ಪೂರ್ಣಾಙ್ಕ err = errno;
      freecon (tcon_raw);
      errno = err;
      ಪ್ರತ್ಯಾಗಮ rc;
    }

  fd = open (path, O_RDONLY | O_NOFOLLOW);
  ಯದಿ (fd == -1 && (errno != ELOOP))
    ಗಚ್ಛ quit;

  ಯದಿ (fd != -1)
    {
      ಯದಿ (fstat (fd, &sb) < 0)
        ಗಚ್ಛ quit;
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (lstat (path, &sb) < 0)
        ಗಚ್ಛ quit;
    }

  ಯದಿ (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      ಯದಿ (errno == ENOENT)
        errno = ENODATA;
      ಗಚ್ಛ quit;
    }
  ಯದಿ (!(scontext = context_new (scon_raw)))
    ಗಚ್ಛ quit;

  ಯದಿ (fd != -1)
    {
      ಯದಿ (fgetfilecon_raw (fd, &tcon_raw) < 0)
        ಗಚ್ಛ quit;
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (lgetfilecon_raw (path, &tcon_raw) < 0)
        ಗಚ್ಛ quit;
    }

  ಯದಿ (!(tcontext = context_new (tcon_raw)))
    ಗಚ್ಛ quit;

  ಯದಿ (!(contype = context_type_get (scontext)))
    ಗಚ್ಛ quit;
  ಯದಿ (context_type_set (tcontext, contype))
    ಗಚ್ಛ quit;
  ಯದಿ (!(constr = context_str (tcontext)))
    ಗಚ್ಛ quit;

  ಯದಿ (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  ಅನ್ಯಥಾ
    rc = lsetfilecon_raw (path, constr);

 quit:;
  ಪೂರ್ಣಾಙ್ಕ err = errno;
  ಯದಿ (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ಪ್ರತ್ಯಾಗಮ rc;
}

/*
  This function takes three parameters:

  SELABEL_HANDLE for selabel_lookup, or null to preserve.

  PATH of an existing file system object.

  A RECURSE boolean which if the file system object is a directory, will
  call restorecon_private on every file system object in the directory.

  Return false on failure.  errno will be set appropriately.
*/
bool
restorecon (ಸಂರಚನಾ selabel_handle *selabel_handle,
            ವರ್ಣ ಧ್ರುವ *path, bool recurse)
{
  ವರ್ಣ *newpath = ರಿಕ್ತ;

  ಯದಿ (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      ಯದಿ (! newpath)
        ಪ್ರತ್ಯಾಗಮ false;
      path = newpath;
    }

  ಯದಿ (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      ಪೂರ್ಣಾಙ್ಕ err = errno;
      ಮುಕ್ತ (newpath);
      errno = err;
      ಪ್ರತ್ಯಾಗಮ ok;
    }

  ವರ್ಣ ಧ್ರುವ *ftspath[2] = { path, ರಿಕ್ತ };
  FTS *fts = xfts_open ((ವರ್ಣ *ಧ್ರುವ *) ftspath, FTS_PHYSICAL, ರಿಕ್ತ);

  ಪೂರ್ಣಾಙ್ಕ err = 0;
  ಪ್ರತಿ (FTSENT *ent; (ent = fts_read (fts)); )
    ಯದಿ (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = errno;

  ಯದಿ (errno != 0)
    err = errno;

  ಯದಿ (fts_close (fts) != 0)
    err = errno;

  ಮುಕ್ತ (newpath);
  ಪ್ರತ್ಯಾಗಮ !err;
}
#endif
