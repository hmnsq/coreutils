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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <selinux/label.h>
#অন্তর্ভাৱ <selinux/context.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "canonicalize.h"
#অন্তর্ভাৱ "xfts.h"
#অন্তর্ভাৱ "selinux.h"

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
    ঘূৰাই string_to_security_class ("file");
  যদি (S_ISDIR (m))
    ঘূৰাই string_to_security_class ("dir");
  যদি (S_ISCHR (m))
    ঘূৰাই string_to_security_class ("chr_file");
  যদি (S_ISBLK (m))
    ঘূৰাই string_to_security_class ("blk_file");
  যদি (S_ISFIFO (m))
    ঘূৰাই string_to_security_class ("fifo_file");
  যদি (S_ISLNK (m))
    ঘূৰাই string_to_security_class ("lnk_file");
  যদি (S_ISSOCK (m))
    ঘূৰাই string_to_security_class ("sock_file");

  errno = EINVAL;
  ঘূৰাই 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

স্থির পূর্ণাঙ্ক
computecon_raw (ৱর্ণ ধ্রুৱ *path, mode_t mode, ৱর্ণ **con)
{
  ৱর্ণ *scon_raw = রিক্ত;
  ৱর্ণ *tcon_raw = রিক্ত;
  security_class_t tclass;
  পূর্ণাঙ্ক rc = -1;

  ৱর্ণ *dir = dir_name (path);
  যদি (!dir)
    গচ্ছ quit;
  যদি (getcon_raw (&scon_raw) < 0)
    গচ্ছ quit;
  যদি (getfilecon_raw (dir, &tcon_raw) < 0)
    গচ্ছ quit;
  tclass = mode_to_security_class (mode);
  যদি (!tclass)
    গচ্ছ quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  পূর্ণাঙ্ক err = errno;
  মুক্ত (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ঘূৰাই rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
পূর্ণাঙ্ক
defaultcon (সংরচনা selabel_handle *selabel_handle,
            ৱর্ণ ধ্রুৱ *path, mode_t mode)
{
  পূর্ণাঙ্ক rc = -1;
  ৱর্ণ *scon_raw = রিক্ত;
  ৱর্ণ *tcon_raw = রিক্ত;
  context_t scontext = রিক্ত, tcontext = রিক্ত;
  ৱর্ণ ধ্রুৱ *contype;
  ৱর্ণ ধ্রুৱ *constr;
  ৱর্ণ *newpath = রিক্ত;

  যদি (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      যদি (! newpath)
        গচ্ছ quit;
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
      গচ্ছ quit;
    }
  যদি (computecon_raw (path, mode, &tcon_raw) < 0)
    গচ্ছ quit;
  যদি (!(scontext = context_new (scon_raw)))
    গচ্ছ quit;
  যদি (!(tcontext = context_new (tcon_raw)))
    গচ্ছ quit;

  যদি (!(contype = context_type_get (scontext)))
    গচ্ছ quit;
  যদি (context_type_set (tcontext, contype))
    গচ্ছ quit;
  যদি (!(constr = context_str (tcontext)))
    গচ্ছ quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  পূর্ণাঙ্ক err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  মুক্ত (newpath);
  errno = err;
  ঘূৰাই rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
স্থির পূর্ণাঙ্ক
restorecon_private (সংরচনা selabel_handle *selabel_handle, ৱর্ণ ধ্রুৱ *path)
{
  পূর্ণাঙ্ক rc = -1;
  সংরচনা stat sb;
  ৱর্ণ *scon_raw = রিক্ত;
  ৱর্ণ *tcon_raw = রিক্ত;
  context_t scontext = রিক্ত, tcontext = রিক্ত;
  ৱর্ণ ধ্রুৱ *contype;
  ৱর্ণ ধ্রুৱ *constr;
  পূর্ণাঙ্ক fd;

  যদি (!selabel_handle)
    {
      যদি (getfscreatecon_raw (&tcon_raw) < 0)
        ঘূৰাই rc;
      যদি (!tcon_raw)
        {
          errno = ENODATA;
          ঘূৰাই rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      পূর্ণাঙ্ক err = errno;
      freecon (tcon_raw);
      errno = err;
      ঘূৰাই rc;
    }

  fd = open (path, O_RDONLY | O_NOFOLLOW);
  যদি (fd == -1 && (errno != ELOOP))
    গচ্ছ quit;

  যদি (fd != -1)
    {
      যদি (fstat (fd, &sb) < 0)
        গচ্ছ quit;
    }
  অন্যথা
    {
      যদি (lstat (path, &sb) < 0)
        গচ্ছ quit;
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
      গচ্ছ quit;
    }
  যদি (!(scontext = context_new (scon_raw)))
    গচ্ছ quit;

  যদি (fd != -1)
    {
      যদি (fgetfilecon_raw (fd, &tcon_raw) < 0)
        গচ্ছ quit;
    }
  অন্যথা
    {
      যদি (lgetfilecon_raw (path, &tcon_raw) < 0)
        গচ্ছ quit;
    }

  যদি (!(tcontext = context_new (tcon_raw)))
    গচ্ছ quit;

  যদি (!(contype = context_type_get (scontext)))
    গচ্ছ quit;
  যদি (context_type_set (tcontext, contype))
    গচ্ছ quit;
  যদি (!(constr = context_str (tcontext)))
    গচ্ছ quit;

  যদি (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  অন্যথা
    rc = lsetfilecon_raw (path, constr);

 quit:;
  পূর্ণাঙ্ক err = errno;
  যদি (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ঘূৰাই rc;
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
restorecon (সংরচনা selabel_handle *selabel_handle,
            ৱর্ণ ধ্রুৱ *path, bool recurse)
{
  ৱর্ণ *newpath = রিক্ত;

  যদি (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      যদি (! newpath)
        ঘূৰাই false;
      path = newpath;
    }

  যদি (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      পূর্ণাঙ্ক err = errno;
      মুক্ত (newpath);
      errno = err;
      ঘূৰাই ok;
    }

  ৱর্ণ ধ্রুৱ *ftspath[2] = { path, রিক্ত };
  FTS *fts = xfts_open ((ৱর্ণ *ধ্রুৱ *) ftspath, FTS_PHYSICAL, রিক্ত);

  পূর্ণাঙ্ক err = 0;
  বাবে (FTSENT *ent; (ent = fts_read (fts)); )
    যদি (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = errno;

  যদি (errno != 0)
    err = errno;

  যদি (fts_close (fts) != 0)
    err = errno;

  মুক্ত (newpath);
  ঘূৰাই !err;
}
#endif
