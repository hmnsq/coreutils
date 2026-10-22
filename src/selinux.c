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

#شامل <config.h>
#شامل <selinux/label.h>
#شامل <selinux/context.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "canonicalize.h"
#شامل "xfts.h"
#شامل "selinux.h"

#اگر HAVE_SELINUX_LABEL_H

# اگر ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
ایستا security_class_t
mode_to_security_class (mode_t m)
{

  اگر (S_ISREG (m))
    بازگشت string_to_security_class ("file");
  اگر (S_ISDIR (m))
    بازگشت string_to_security_class ("dir");
  اگر (S_ISCHR (m))
    بازگشت string_to_security_class ("chr_file");
  اگر (S_ISBLK (m))
    بازگشت string_to_security_class ("blk_file");
  اگر (S_ISFIFO (m))
    بازگشت string_to_security_class ("fifo_file");
  اگر (S_ISLNK (m))
    بازگشت string_to_security_class ("lnk_file");
  اگر (S_ISSOCK (m))
    بازگشت string_to_security_class ("sock_file");

  errno = EINVAL;
  بازگشت 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

ایستا صحیح
computecon_raw (نویسه ثابت *path, mode_t mode, نویسه **con)
{
  نویسه *scon_raw = پوچ;
  نویسه *tcon_raw = پوچ;
  security_class_t tclass;
  صحیح rc = -1;

  نویسه *dir = dir_name (path);
  اگر (!dir)
    برو quit;
  اگر (getcon_raw (&scon_raw) < 0)
    برو quit;
  اگر (getfilecon_raw (dir, &tcon_raw) < 0)
    برو quit;
  tclass = mode_to_security_class (mode);
  اگر (!tclass)
    برو quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  صحیح err = errno;
  آزاد (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  بازگشت rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
صحیح
defaultcon (ساختار selabel_handle *selabel_handle,
            نویسه ثابت *path, mode_t mode)
{
  صحیح rc = -1;
  نویسه *scon_raw = پوچ;
  نویسه *tcon_raw = پوچ;
  context_t scontext = پوچ, tcontext = پوچ;
  نویسه ثابت *contype;
  نویسه ثابت *constr;
  نویسه *newpath = پوچ;

  اگر (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      اگر (! newpath)
        برو quit;
      path = newpath;
    }

  اگر (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      اگر (errno == ENOENT)
        errno = ENODATA;
      برو quit;
    }
  اگر (computecon_raw (path, mode, &tcon_raw) < 0)
    برو quit;
  اگر (!(scontext = context_new (scon_raw)))
    برو quit;
  اگر (!(tcontext = context_new (tcon_raw)))
    برو quit;

  اگر (!(contype = context_type_get (scontext)))
    برو quit;
  اگر (context_type_set (tcontext, contype))
    برو quit;
  اگر (!(constr = context_str (tcontext)))
    برو quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  صحیح err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  آزاد (newpath);
  errno = err;
  بازگشت rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
ایستا صحیح
restorecon_private (ساختار selabel_handle *selabel_handle, نویسه ثابت *path)
{
  صحیح rc = -1;
  ساختار stat sb;
  نویسه *scon_raw = پوچ;
  نویسه *tcon_raw = پوچ;
  context_t scontext = پوچ, tcontext = پوچ;
  نویسه ثابت *contype;
  نویسه ثابت *constr;
  صحیح fd;

  اگر (!selabel_handle)
    {
      اگر (getfscreatecon_raw (&tcon_raw) < 0)
        بازگشت rc;
      اگر (!tcon_raw)
        {
          errno = ENODATA;
          بازگشت rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      صحیح err = errno;
      freecon (tcon_raw);
      errno = err;
      بازگشت rc;
    }

  fd = open (path, O_RDONLY | O_NOFOLLOW);
  اگر (fd == -1 && (errno != ELOOP))
    برو quit;

  اگر (fd != -1)
    {
      اگر (fstat (fd, &sb) < 0)
        برو quit;
    }
  وگرنه
    {
      اگر (lstat (path, &sb) < 0)
        برو quit;
    }

  اگر (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      اگر (errno == ENOENT)
        errno = ENODATA;
      برو quit;
    }
  اگر (!(scontext = context_new (scon_raw)))
    برو quit;

  اگر (fd != -1)
    {
      اگر (fgetfilecon_raw (fd, &tcon_raw) < 0)
        برو quit;
    }
  وگرنه
    {
      اگر (lgetfilecon_raw (path, &tcon_raw) < 0)
        برو quit;
    }

  اگر (!(tcontext = context_new (tcon_raw)))
    برو quit;

  اگر (!(contype = context_type_get (scontext)))
    برو quit;
  اگر (context_type_set (tcontext, contype))
    برو quit;
  اگر (!(constr = context_str (tcontext)))
    برو quit;

  اگر (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  وگرنه
    rc = lsetfilecon_raw (path, constr);

 quit:;
  صحیح err = errno;
  اگر (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  بازگشت rc;
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
restorecon (ساختار selabel_handle *selabel_handle,
            نویسه ثابت *path, bool recurse)
{
  نویسه *newpath = پوچ;

  اگر (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      اگر (! newpath)
        بازگشت false;
      path = newpath;
    }

  اگر (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      صحیح err = errno;
      آزاد (newpath);
      errno = err;
      بازگشت ok;
    }

  نویسه ثابت *ftspath[2] = { path, پوچ };
  FTS *fts = xfts_open ((نویسه *ثابت *) ftspath, FTS_PHYSICAL, پوچ);

  صحیح err = 0;
  برای (FTSENT *ent; (ent = fts_read (fts)); )
    اگر (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = errno;

  اگر (errno != 0)
    err = errno;

  اگر (fts_close (fts) != 0)
    err = errno;

  آزاد (newpath);
  بازگشت !err;
}
#endif
