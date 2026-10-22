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

#अन्तर्भाव <config.h>
#अन्तर्भाव <selinux/label.h>
#अन्तर्भाव <selinux/context.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "canonicalize.h"
#अन्तर्भाव "xfts.h"
#अन्तर्भाव "selinux.h"

#सचे HAVE_SELINUX_LABEL_H

# सचे ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
स्थिर security_class_t
mode_to_security_class (mode_t m)
{

  सचे (S_ISREG (m))
    पटिनिवत्त string_to_security_class ("file");
  सचे (S_ISDIR (m))
    पटिनिवत्त string_to_security_class ("dir");
  सचे (S_ISCHR (m))
    पटिनिवत्त string_to_security_class ("chr_file");
  सचे (S_ISBLK (m))
    पटिनिवत्त string_to_security_class ("blk_file");
  सचे (S_ISFIFO (m))
    पटिनिवत्त string_to_security_class ("fifo_file");
  सचे (S_ISLNK (m))
    पटिनिवत्त string_to_security_class ("lnk_file");
  सचे (S_ISSOCK (m))
    पटिनिवत्त string_to_security_class ("sock_file");

  errno = EINVAL;
  पटिनिवत्त 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

स्थिर पूर्णाङ्क
computecon_raw (वर्ण ध्रुव *path, mode_t mode, वर्ण **con)
{
  वर्ण *scon_raw = रिक्त;
  वर्ण *tcon_raw = रिक्त;
  security_class_t tclass;
  पूर्णाङ्क rc = -1;

  वर्ण *dir = dir_name (path);
  सचे (!dir)
    गच्छ quit;
  सचे (getcon_raw (&scon_raw) < 0)
    गच्छ quit;
  सचे (getfilecon_raw (dir, &tcon_raw) < 0)
    गच्छ quit;
  tclass = mode_to_security_class (mode);
  सचे (!tclass)
    गच्छ quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  पूर्णाङ्क err = errno;
  मुक्त (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  पटिनिवत्त rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
पूर्णाङ्क
defaultcon (संरचना selabel_handle *selabel_handle,
            वर्ण ध्रुव *path, mode_t mode)
{
  पूर्णाङ्क rc = -1;
  वर्ण *scon_raw = रिक्त;
  वर्ण *tcon_raw = रिक्त;
  context_t scontext = रिक्त, tcontext = रिक्त;
  वर्ण ध्रुव *contype;
  वर्ण ध्रुव *constr;
  वर्ण *newpath = रिक्त;

  सचे (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      सचे (! newpath)
        गच्छ quit;
      path = newpath;
    }

  सचे (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      सचे (errno == ENOENT)
        errno = ENODATA;
      गच्छ quit;
    }
  सचे (computecon_raw (path, mode, &tcon_raw) < 0)
    गच्छ quit;
  सचे (!(scontext = context_new (scon_raw)))
    गच्छ quit;
  सचे (!(tcontext = context_new (tcon_raw)))
    गच्छ quit;

  सचे (!(contype = context_type_get (scontext)))
    गच्छ quit;
  सचे (context_type_set (tcontext, contype))
    गच्छ quit;
  सचे (!(constr = context_str (tcontext)))
    गच्छ quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  पूर्णाङ्क err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  मुक्त (newpath);
  errno = err;
  पटिनिवत्त rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
स्थिर पूर्णाङ्क
restorecon_private (संरचना selabel_handle *selabel_handle, वर्ण ध्रुव *path)
{
  पूर्णाङ्क rc = -1;
  संरचना stat sb;
  वर्ण *scon_raw = रिक्त;
  वर्ण *tcon_raw = रिक्त;
  context_t scontext = रिक्त, tcontext = रिक्त;
  वर्ण ध्रुव *contype;
  वर्ण ध्रुव *constr;
  पूर्णाङ्क fd;

  सचे (!selabel_handle)
    {
      सचे (getfscreatecon_raw (&tcon_raw) < 0)
        पटिनिवत्त rc;
      सचे (!tcon_raw)
        {
          errno = ENODATA;
          पटिनिवत्त rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      पूर्णाङ्क err = errno;
      freecon (tcon_raw);
      errno = err;
      पटिनिवत्त rc;
    }

  fd = open (path, O_RDONLY | O_NOFOLLOW);
  सचे (fd == -1 && (errno != ELOOP))
    गच्छ quit;

  सचे (fd != -1)
    {
      सचे (fstat (fd, &sb) < 0)
        गच्छ quit;
    }
  अञ्ञथा
    {
      सचे (lstat (path, &sb) < 0)
        गच्छ quit;
    }

  सचे (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      सचे (errno == ENOENT)
        errno = ENODATA;
      गच्छ quit;
    }
  सचे (!(scontext = context_new (scon_raw)))
    गच्छ quit;

  सचे (fd != -1)
    {
      सचे (fgetfilecon_raw (fd, &tcon_raw) < 0)
        गच्छ quit;
    }
  अञ्ञथा
    {
      सचे (lgetfilecon_raw (path, &tcon_raw) < 0)
        गच्छ quit;
    }

  सचे (!(tcontext = context_new (tcon_raw)))
    गच्छ quit;

  सचे (!(contype = context_type_get (scontext)))
    गच्छ quit;
  सचे (context_type_set (tcontext, contype))
    गच्छ quit;
  सचे (!(constr = context_str (tcontext)))
    गच्छ quit;

  सचे (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  अञ्ञथा
    rc = lsetfilecon_raw (path, constr);

 quit:;
  पूर्णाङ्क err = errno;
  सचे (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  पटिनिवत्त rc;
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
restorecon (संरचना selabel_handle *selabel_handle,
            वर्ण ध्रुव *path, bool recurse)
{
  वर्ण *newpath = रिक्त;

  सचे (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      सचे (! newpath)
        पटिनिवत्त false;
      path = newpath;
    }

  सचे (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      पूर्णाङ्क err = errno;
      मुक्त (newpath);
      errno = err;
      पटिनिवत्त ok;
    }

  वर्ण ध्रुव *ftspath[2] = { path, रिक्त };
  FTS *fts = xfts_open ((वर्ण *ध्रुव *) ftspath, FTS_PHYSICAL, रिक्त);

  पूर्णाङ्क err = 0;
  अत्थाय (FTSENT *ent; (ent = fts_read (fts)); )
    सचे (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = errno;

  सचे (errno != 0)
    err = errno;

  सचे (fts_close (fts) != 0)
    err = errno;

  मुक्त (newpath);
  पटिनिवत्त !err;
}
#endif
