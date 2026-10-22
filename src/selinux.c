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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <selinux/label.h>
#অন্তর্ভাব <selinux/context.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "canonicalize.h"
#অন্তর্ভাব "xfts.h"
#অন্তর্ভাব "selinux.h"

#যদি HAVE_SELINUX_LABEL_H

# যদি ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
স্থির security_class_t
mode_to_security_class (mode_t m)
{

  যদি (S_ISREG (m))
    ফেরত string_to_security_class ("file");
  যদি (S_ISDIR (m))
    ফেরত string_to_security_class ("dir");
  যদি (S_ISCHR (m))
    ফেরত string_to_security_class ("chr_file");
  যদি (S_ISBLK (m))
    ফেরত string_to_security_class ("blk_file");
  যদি (S_ISFIFO (m))
    ফেরত string_to_security_class ("fifo_file");
  যদি (S_ISLNK (m))
    ফেরত string_to_security_class ("lnk_file");
  যদি (S_ISSOCK (m))
    ফেরত string_to_security_class ("sock_file");

  errno = EINVAL;
  ফেরত 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

স্থির পূর্ণসংখ্যা
computecon_raw (অক্ষর ধ্রুবক *path, mode_t mode, অক্ষর **con)
{
  অক্ষর *scon_raw = ফাঁকা;
  অক্ষর *tcon_raw = ফাঁকা;
  security_class_t tclass;
  পূর্ণসংখ্যা rc = -1;

  অক্ষর *dir = dir_name (path);
  যদি (!dir)
    যাও quit;
  যদি (getcon_raw (&scon_raw) < 0)
    যাও quit;
  যদি (getfilecon_raw (dir, &tcon_raw) < 0)
    যাও quit;
  tclass = mode_to_security_class (mode);
  যদি (!tclass)
    যাও quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  পূর্ণসংখ্যা err = errno;
  মুক্ত (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ফেরত rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
পূর্ণসংখ্যা
defaultcon (কাঠামো selabel_handle *selabel_handle,
            অক্ষর ধ্রুবক *path, mode_t mode)
{
  পূর্ণসংখ্যা rc = -1;
  অক্ষর *scon_raw = ফাঁকা;
  অক্ষর *tcon_raw = ফাঁকা;
  context_t scontext = ফাঁকা, tcontext = ফাঁকা;
  অক্ষর ধ্রুবক *contype;
  অক্ষর ধ্রুবক *constr;
  অক্ষর *newpath = ফাঁকা;

  যদি (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      যদি (! newpath)
        যাও quit;
      path = newpath;
    }

  যদি (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      যদি (errno == ENOENT)
        errno = ENODATA;
      যাও quit;
    }
  যদি (computecon_raw (path, mode, &tcon_raw) < 0)
    যাও quit;
  যদি (!(scontext = context_new (scon_raw)))
    যাও quit;
  যদি (!(tcontext = context_new (tcon_raw)))
    যাও quit;

  যদি (!(contype = context_type_get (scontext)))
    যাও quit;
  যদি (context_type_set (tcontext, contype))
    যাও quit;
  যদি (!(constr = context_str (tcontext)))
    যাও quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  পূর্ণসংখ্যা err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  মুক্ত (newpath);
  errno = err;
  ফেরত rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
স্থির পূর্ণসংখ্যা
restorecon_private (কাঠামো selabel_handle *selabel_handle, অক্ষর ধ্রুবক *path)
{
  পূর্ণসংখ্যা rc = -1;
  কাঠামো stat sb;
  অক্ষর *scon_raw = ফাঁকা;
  অক্ষর *tcon_raw = ফাঁকা;
  context_t scontext = ফাঁকা, tcontext = ফাঁকা;
  অক্ষর ধ্রুবক *contype;
  অক্ষর ধ্রুবক *constr;
  পূর্ণসংখ্যা fd;

  যদি (!selabel_handle)
    {
      যদি (getfscreatecon_raw (&tcon_raw) < 0)
        ফেরত rc;
      যদি (!tcon_raw)
        {
          errno = ENODATA;
          ফেরত rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      পূর্ণসংখ্যা err = errno;
      freecon (tcon_raw);
      errno = err;
      ফেরত rc;
    }

  fd = open (path, O_RDONLY | O_NOFOLLOW);
  যদি (fd == -1 && (errno != ELOOP))
    যাও quit;

  যদি (fd != -1)
    {
      যদি (fstat (fd, &sb) < 0)
        যাও quit;
    }
  অন্যথা
    {
      যদি (lstat (path, &sb) < 0)
        যাও quit;
    }

  যদি (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      যদি (errno == ENOENT)
        errno = ENODATA;
      যাও quit;
    }
  যদি (!(scontext = context_new (scon_raw)))
    যাও quit;

  যদি (fd != -1)
    {
      যদি (fgetfilecon_raw (fd, &tcon_raw) < 0)
        যাও quit;
    }
  অন্যথা
    {
      যদি (lgetfilecon_raw (path, &tcon_raw) < 0)
        যাও quit;
    }

  যদি (!(tcontext = context_new (tcon_raw)))
    যাও quit;

  যদি (!(contype = context_type_get (scontext)))
    যাও quit;
  যদি (context_type_set (tcontext, contype))
    যাও quit;
  যদি (!(constr = context_str (tcontext)))
    যাও quit;

  যদি (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  অন্যথা
    rc = lsetfilecon_raw (path, constr);

 quit:;
  পূর্ণসংখ্যা err = errno;
  যদি (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ফেরত rc;
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
restorecon (কাঠামো selabel_handle *selabel_handle,
            অক্ষর ধ্রুবক *path, bool recurse)
{
  অক্ষর *newpath = ফাঁকা;

  যদি (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      যদি (! newpath)
        ফেরত false;
      path = newpath;
    }

  যদি (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      পূর্ণসংখ্যা err = errno;
      মুক্ত (newpath);
      errno = err;
      ফেরত ok;
    }

  অক্ষর ধ্রুবক *ftspath[2] = { path, ফাঁকা };
  FTS *fts = xfts_open ((অক্ষর *ধ্রুবক *) ftspath, FTS_PHYSICAL, ফাঁকা);

  পূর্ণসংখ্যা err = 0;
  জন্য (FTSENT *ent; (ent = fts_read (fts)); )
    যদি (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = errno;

  যদি (errno != 0)
    err = errno;

  যদি (fts_close (fts) != 0)
    err = errno;

  মুক্ত (newpath);
  ফেরত !err;
}
#endif
