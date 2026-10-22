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

#यदि HAVE_SELINUX_LABEL_H

# यदि ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
स्थिर security_class_t
mode_to_security_class (mode_t m)
{

  यदि (S_ISREG (m))
    फर्काऊ string_to_security_class ("file");
  यदि (S_ISDIR (m))
    फर्काऊ string_to_security_class ("dir");
  यदि (S_ISCHR (m))
    फर्काऊ string_to_security_class ("chr_file");
  यदि (S_ISBLK (m))
    फर्काऊ string_to_security_class ("blk_file");
  यदि (S_ISFIFO (m))
    फर्काऊ string_to_security_class ("fifo_file");
  यदि (S_ISLNK (m))
    फर्काऊ string_to_security_class ("lnk_file");
  यदि (S_ISSOCK (m))
    फर्काऊ string_to_security_class ("sock_file");

  त्रुटि_सं = EINVAL;
  फर्काऊ 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

स्थिर पूर्णाङ्क
computecon_raw (वर्ण अचर *path, mode_t mode, वर्ण **con)
{
  वर्ण *scon_raw = रिक्त;
  वर्ण *tcon_raw = रिक्त;
  security_class_t tclass;
  पूर्णाङ्क rc = -1;

  वर्ण *dir = dir_name (path);
  यदि (!dir)
    गच्छ quit;
  यदि (getcon_raw (&scon_raw) < 0)
    गच्छ quit;
  यदि (getfilecon_raw (dir, &tcon_raw) < 0)
    गच्छ quit;
  tclass = mode_to_security_class (mode);
  यदि (!tclass)
    गच्छ quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  पूर्णाङ्क err = त्रुटि_सं;
  मुक्त (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  त्रुटि_सं = err;
  फर्काऊ rc;
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
            वर्ण अचर *path, mode_t mode)
{
  पूर्णाङ्क rc = -1;
  वर्ण *scon_raw = रिक्त;
  वर्ण *tcon_raw = रिक्त;
  context_t scontext = रिक्त, tcontext = रिक्त;
  वर्ण अचर *contype;
  वर्ण अचर *constr;
  वर्ण *newpath = रिक्त;

  यदि (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      यदि (! newpath)
        गच्छ quit;
      path = newpath;
    }

  यदि (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      यदि (त्रुटि_सं == ENOENT)
        त्रुटि_सं = ENODATA;
      गच्छ quit;
    }
  यदि (computecon_raw (path, mode, &tcon_raw) < 0)
    गच्छ quit;
  यदि (!(scontext = context_new (scon_raw)))
    गच्छ quit;
  यदि (!(tcontext = context_new (tcon_raw)))
    गच्छ quit;

  यदि (!(contype = context_type_get (scontext)))
    गच्छ quit;
  यदि (context_type_set (tcontext, contype))
    गच्छ quit;
  यदि (!(constr = context_str (tcontext)))
    गच्छ quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  पूर्णाङ्क err = त्रुटि_सं;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  मुक्त (newpath);
  त्रुटि_सं = err;
  फर्काऊ rc;
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
restorecon_private (संरचना selabel_handle *selabel_handle, वर्ण अचर *path)
{
  पूर्णाङ्क rc = -1;
  संरचना stat sb;
  वर्ण *scon_raw = रिक्त;
  वर्ण *tcon_raw = रिक्त;
  context_t scontext = रिक्त, tcontext = रिक्त;
  वर्ण अचर *contype;
  वर्ण अचर *constr;
  पूर्णाङ्क fd;

  यदि (!selabel_handle)
    {
      यदि (getfscreatecon_raw (&tcon_raw) < 0)
        फर्काऊ rc;
      यदि (!tcon_raw)
        {
          त्रुटि_सं = ENODATA;
          फर्काऊ rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      पूर्णाङ्क err = त्रुटि_सं;
      freecon (tcon_raw);
      त्रुटि_सं = err;
      फर्काऊ rc;
    }

  fd = खोलो (path, O_RDONLY | O_NOFOLLOW);
  यदि (fd == -1 && (त्रुटि_सं != ELOOP))
    गच्छ quit;

  यदि (fd != -1)
    {
      यदि (ख_स्थिति (fd, &sb) < 0)
        गच्छ quit;
    }
  अन्यथा
    {
      यदि (lstat (path, &sb) < 0)
        गच्छ quit;
    }

  यदि (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      यदि (त्रुटि_सं == ENOENT)
        त्रुटि_सं = ENODATA;
      गच्छ quit;
    }
  यदि (!(scontext = context_new (scon_raw)))
    गच्छ quit;

  यदि (fd != -1)
    {
      यदि (fgetfilecon_raw (fd, &tcon_raw) < 0)
        गच्छ quit;
    }
  अन्यथा
    {
      यदि (lgetfilecon_raw (path, &tcon_raw) < 0)
        गच्छ quit;
    }

  यदि (!(tcontext = context_new (tcon_raw)))
    गच्छ quit;

  यदि (!(contype = context_type_get (scontext)))
    गच्छ quit;
  यदि (context_type_set (tcontext, contype))
    गच्छ quit;
  यदि (!(constr = context_str (tcontext)))
    गच्छ quit;

  यदि (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  अन्यथा
    rc = lsetfilecon_raw (path, constr);

 quit:;
  पूर्णाङ्क err = त्रुटि_सं;
  यदि (fd != -1)
    बंद (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  त्रुटि_सं = err;
  फर्काऊ rc;
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
            वर्ण अचर *path, bool recurse)
{
  वर्ण *newpath = रिक्त;

  यदि (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      यदि (! newpath)
        फर्काऊ false;
      path = newpath;
    }

  यदि (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      पूर्णाङ्क err = त्रुटि_सं;
      मुक्त (newpath);
      त्रुटि_सं = err;
      फर्काऊ ok;
    }

  वर्ण अचर *ftspath[2] = { path, रिक्त };
  FTS *fts = xfts_open ((वर्ण *अचर *) ftspath, FTS_PHYSICAL, रिक्त);

  पूर्णाङ्क err = 0;
  लागि (FTSENT *ent; (ent = fts_read (fts)); )
    यदि (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = त्रुटि_सं;

  यदि (त्रुटि_सं != 0)
    err = त्रुटि_सं;

  यदि (fts_close (fts) != 0)
    err = त्रुटि_सं;

  मुक्त (newpath);
  फर्काऊ !err;
}
#endif
