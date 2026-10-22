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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/label.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/context.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "canonicalize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xfts.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "selinux.h"

#ਜੇ HAVE_SELINUX_LABEL_H

# ਜੇ ! HAVE_ਬਹੁਲਕ_ਮੰਜ਼ਿਲ_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
ਸਥਿਰ security_class_t
ਬਹੁਲਕ_ਮੰਜ਼ਿਲ_security_class (mode_t m)
{

  ਜੇ (S_ISREG (m))
    ਵਾਪਸ string_ਮੰਜ਼ਿਲ_security_class ("file");
  ਜੇ (S_ISDIR (m))
    ਵਾਪਸ string_ਮੰਜ਼ਿਲ_security_class ("dir");
  ਜੇ (S_ISCHR (m))
    ਵਾਪਸ string_ਮੰਜ਼ਿਲ_security_class ("chr_file");
  ਜੇ (S_ISBLK (m))
    ਵਾਪਸ string_ਮੰਜ਼ਿਲ_security_class ("blk_file");
  ਜੇ (S_ISFIFO (m))
    ਵਾਪਸ string_ਮੰਜ਼ਿਲ_security_class ("fifo_file");
  ਜੇ (S_ISLNK (m))
    ਵਾਪਸ string_ਮੰਜ਼ਿਲ_security_class ("lnk_file");
  ਜੇ (S_ISSOCK (m))
    ਵਾਪਸ string_ਮੰਜ਼ਿਲ_security_class ("sock_file");

  errno = EINVAL;
  ਵਾਪਸ 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

ਸਥਿਰ ਪੂਰਨ
computecon_raw (ਅੱਖਰ ਅਚਲ *ਰਸਤਾ, mode_t ਬਹੁਲਕ_2, ਅੱਖਰ **con)
{
  ਅੱਖਰ *scon_raw = ਨਿਰਾ;
  ਅੱਖਰ *tcon_raw = ਨਿਰਾ;
  security_class_t tclass;
  ਪੂਰਨ rc = -1;

  ਅੱਖਰ *dir = dir_name (ਰਸਤਾ);
  ਜੇ (!dir)
    ਜਾਓ quit;
  ਜੇ (getcon_raw (&scon_raw) < 0)
    ਜਾਓ quit;
  ਜੇ (getfilecon_raw (dir, &tcon_raw) < 0)
    ਜਾਓ quit;
  tclass = ਬਹੁਲਕ_ਮੰਜ਼ਿਲ_security_class (ਬਹੁਲਕ_2);
  ਜੇ (!tclass)
    ਜਾਓ quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  ਪੂਰਨ err = errno;
  ਮੁਕਤ (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ਵਾਪਸ rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
ਪੂਰਨ
defaultcon (ਬਣਤਰ selabel_handle *selabel_handle,
            ਅੱਖਰ ਅਚਲ *ਰਸਤਾ, mode_t ਬਹੁਲਕ_2)
{
  ਪੂਰਨ rc = -1;
  ਅੱਖਰ *scon_raw = ਨਿਰਾ;
  ਅੱਖਰ *tcon_raw = ਨਿਰਾ;
  context_t scontext = ਨਿਰਾ, tcontext = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *contype;
  ਅੱਖਰ ਅਚਲ *constr;
  ਅੱਖਰ *newpath = ਨਿਰਾ;

  ਜੇ (! IS_ABSOLUTE_FILE_NAME (ਰਸਤਾ))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_ਬਹੁਲਕ (ਰਸਤਾ, CAN_MISSING);
      ਜੇ (! newpath)
        ਜਾਓ quit;
      ਰਸਤਾ = newpath;
    }

  ਜੇ (selabel_lookup_raw (selabel_handle, &scon_raw, ਰਸਤਾ, ਬਹੁਲਕ_2) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      ਜੇ (errno == ENOENT)
        errno = ENODATA;
      ਜਾਓ quit;
    }
  ਜੇ (computecon_raw (ਰਸਤਾ, ਬਹੁਲਕ_2, &tcon_raw) < 0)
    ਜਾਓ quit;
  ਜੇ (!(scontext = context_new (scon_raw)))
    ਜਾਓ quit;
  ਜੇ (!(tcontext = context_new (tcon_raw)))
    ਜਾਓ quit;

  ਜੇ (!(contype = context_type_get (scontext)))
    ਜਾਓ quit;
  ਜੇ (context_type_ਸੈੱਟ (tcontext, contype))
    ਜਾਓ quit;
  ਜੇ (!(constr = context_str (tcontext)))
    ਜਾਓ quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  ਪੂਰਨ err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  ਮੁਕਤ (newpath);
  errno = err;
  ਵਾਪਸ rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
ਸਥਿਰ ਪੂਰਨ
restorecon_ਨਿੱਜੀ (ਬਣਤਰ selabel_handle *selabel_handle, ਅੱਖਰ ਅਚਲ *ਰਸਤਾ)
{
  ਪੂਰਨ rc = -1;
  ਬਣਤਰ stat sb;
  ਅੱਖਰ *scon_raw = ਨਿਰਾ;
  ਅੱਖਰ *tcon_raw = ਨਿਰਾ;
  context_t scontext = ਨਿਰਾ, tcontext = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *contype;
  ਅੱਖਰ ਅਚਲ *constr;
  ਪੂਰਨ fd;

  ਜੇ (!selabel_handle)
    {
      ਜੇ (getfscreatecon_raw (&tcon_raw) < 0)
        ਵਾਪਸ rc;
      ਜੇ (!tcon_raw)
        {
          errno = ENODATA;
          ਵਾਪਸ rc;
        }
      rc = lsetfilecon_raw (ਰਸਤਾ, tcon_raw);
      ਪੂਰਨ err = errno;
      freecon (tcon_raw);
      errno = err;
      ਵਾਪਸ rc;
    }

  fd = open (ਰਸਤਾ, O_RDONLY | O_NOFOLLOW);
  ਜੇ (fd == -1 && (errno != ELOOP))
    ਜਾਓ quit;

  ਜੇ (fd != -1)
    {
      ਜੇ (fstat (fd, &sb) < 0)
        ਜਾਓ quit;
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (lstat (ਰਸਤਾ, &sb) < 0)
        ਜਾਓ quit;
    }

  ਜੇ (selabel_lookup_raw (selabel_handle, &scon_raw, ਰਸਤਾ, sb.st_ਬਹੁਲਕ) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      ਜੇ (errno == ENOENT)
        errno = ENODATA;
      ਜਾਓ quit;
    }
  ਜੇ (!(scontext = context_new (scon_raw)))
    ਜਾਓ quit;

  ਜੇ (fd != -1)
    {
      ਜੇ (fgetfilecon_raw (fd, &tcon_raw) < 0)
        ਜਾਓ quit;
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (lgetfilecon_raw (ਰਸਤਾ, &tcon_raw) < 0)
        ਜਾਓ quit;
    }

  ਜੇ (!(tcontext = context_new (tcon_raw)))
    ਜਾਓ quit;

  ਜੇ (!(contype = context_type_get (scontext)))
    ਜਾਓ quit;
  ਜੇ (context_type_ਸੈੱਟ (tcontext, contype))
    ਜਾਓ quit;
  ਜੇ (!(constr = context_str (tcontext)))
    ਜਾਓ quit;

  ਜੇ (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  ਨਹੀਂਤਾਂ
    rc = lsetfilecon_raw (ਰਸਤਾ, constr);

 quit:;
  ਪੂਰਨ err = errno;
  ਜੇ (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ਵਾਪਸ rc;
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
restorecon (ਬਣਤਰ selabel_handle *selabel_handle,
            ਅੱਖਰ ਅਚਲ *ਰਸਤਾ, bool recurse)
{
  ਅੱਖਰ *newpath = ਨਿਰਾ;

  ਜੇ (! IS_ABSOLUTE_FILE_NAME (ਰਸਤਾ))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_ਬਹੁਲਕ (ਰਸਤਾ, CAN_MISSING);
      ਜੇ (! newpath)
        ਵਾਪਸ false;
      ਰਸਤਾ = newpath;
    }

  ਜੇ (! recurse)
    {
      bool ok = restorecon_ਨਿੱਜੀ (selabel_handle, ਰਸਤਾ) != -1;
      ਪੂਰਨ err = errno;
      ਮੁਕਤ (newpath);
      errno = err;
      ਵਾਪਸ ok;
    }

  ਅੱਖਰ ਅਚਲ *ftspath[2] = { ਰਸਤਾ, ਨਿਰਾ };
  FTS *fts = xfts_open ((ਅੱਖਰ *ਅਚਲ *) ftspath, FTS_PHYSICAL, ਨਿਰਾ);

  ਪੂਰਨ err = 0;
  ਲਈ (FTSENT *ent; (ent = fts_read (fts)); )
    ਜੇ (restorecon_ਨਿੱਜੀ (selabel_handle, fts->fts_ਰਸਤਾ) < 0)
      err = errno;

  ਜੇ (errno != 0)
    err = errno;

  ਜੇ (fts_close (fts) != 0)
    err = errno;

  ਮੁਕਤ (newpath);
  ਵਾਪਸ !err;
}
#ਜੇ_ਅੰਤ
