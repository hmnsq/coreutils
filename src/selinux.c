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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <selinux/label.h>
#அந்தர்பாவ <selinux/context.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "canonicalize.h"
#அந்தர்பாவ "xfts.h"
#அந்தர்பாவ "selinux.h"

#எனில் HAVE_SELINUX_LABEL_H

# எனில் ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
நிலையான security_class_t
mode_to_security_class (mode_t m)
{

  எனில் (S_ISREG (m))
    திரும்பு string_to_security_class ("file");
  எனில் (S_ISDIR (m))
    திரும்பு string_to_security_class ("dir");
  எனில் (S_ISCHR (m))
    திரும்பு string_to_security_class ("chr_file");
  எனில் (S_ISBLK (m))
    திரும்பு string_to_security_class ("blk_file");
  எனில் (S_ISFIFO (m))
    திரும்பு string_to_security_class ("fifo_file");
  எனில் (S_ISLNK (m))
    திரும்பு string_to_security_class ("lnk_file");
  எனில் (S_ISSOCK (m))
    திரும்பு string_to_security_class ("sock_file");

  errno = EINVAL;
  திரும்பு 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

நிலையான முழு
computecon_raw (எழுத்து மாறிலி *path, mode_t mode, எழுத்து **con)
{
  எழுத்து *scon_raw = வெற்று;
  எழுத்து *tcon_raw = வெற்று;
  security_class_t tclass;
  முழு rc = -1;

  எழுத்து *dir = dir_name (path);
  எனில் (!dir)
    செல் quit;
  எனில் (getcon_raw (&scon_raw) < 0)
    செல் quit;
  எனில் (getfilecon_raw (dir, &tcon_raw) < 0)
    செல் quit;
  tclass = mode_to_security_class (mode);
  எனில் (!tclass)
    செல் quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  முழு err = errno;
  விடுவி (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  திரும்பு rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
முழு
defaultcon (கட்டமைப்பு selabel_handle *selabel_handle,
            எழுத்து மாறிலி *path, mode_t mode)
{
  முழு rc = -1;
  எழுத்து *scon_raw = வெற்று;
  எழுத்து *tcon_raw = வெற்று;
  context_t scontext = வெற்று, tcontext = வெற்று;
  எழுத்து மாறிலி *contype;
  எழுத்து மாறிலி *constr;
  எழுத்து *newpath = வெற்று;

  எனில் (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      எனில் (! newpath)
        செல் quit;
      path = newpath;
    }

  எனில் (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      எனில் (errno == ENOENT)
        errno = ENODATA;
      செல் quit;
    }
  எனில் (computecon_raw (path, mode, &tcon_raw) < 0)
    செல் quit;
  எனில் (!(scontext = context_new (scon_raw)))
    செல் quit;
  எனில் (!(tcontext = context_new (tcon_raw)))
    செல் quit;

  எனில் (!(contype = context_type_get (scontext)))
    செல் quit;
  எனில் (context_type_set (tcontext, contype))
    செல் quit;
  எனில் (!(constr = context_str (tcontext)))
    செல் quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  முழு err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  விடுவி (newpath);
  errno = err;
  திரும்பு rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
நிலையான முழு
restorecon_private (கட்டமைப்பு selabel_handle *selabel_handle, எழுத்து மாறிலி *path)
{
  முழு rc = -1;
  கட்டமைப்பு stat sb;
  எழுத்து *scon_raw = வெற்று;
  எழுத்து *tcon_raw = வெற்று;
  context_t scontext = வெற்று, tcontext = வெற்று;
  எழுத்து மாறிலி *contype;
  எழுத்து மாறிலி *constr;
  முழு fd;

  எனில் (!selabel_handle)
    {
      எனில் (getfscreatecon_raw (&tcon_raw) < 0)
        திரும்பு rc;
      எனில் (!tcon_raw)
        {
          errno = ENODATA;
          திரும்பு rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      முழு err = errno;
      freecon (tcon_raw);
      errno = err;
      திரும்பு rc;
    }

  fd = open (path, O_RDONLY | O_NOFOLLOW);
  எனில் (fd == -1 && (errno != ELOOP))
    செல் quit;

  எனில் (fd != -1)
    {
      எனில் (fstat (fd, &sb) < 0)
        செல் quit;
    }
  இல்லையெனில்
    {
      எனில் (lstat (path, &sb) < 0)
        செல் quit;
    }

  எனில் (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      எனில் (errno == ENOENT)
        errno = ENODATA;
      செல் quit;
    }
  எனில் (!(scontext = context_new (scon_raw)))
    செல் quit;

  எனில் (fd != -1)
    {
      எனில் (fgetfilecon_raw (fd, &tcon_raw) < 0)
        செல் quit;
    }
  இல்லையெனில்
    {
      எனில் (lgetfilecon_raw (path, &tcon_raw) < 0)
        செல் quit;
    }

  எனில் (!(tcontext = context_new (tcon_raw)))
    செல் quit;

  எனில் (!(contype = context_type_get (scontext)))
    செல் quit;
  எனில் (context_type_set (tcontext, contype))
    செல் quit;
  எனில் (!(constr = context_str (tcontext)))
    செல் quit;

  எனில் (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  இல்லையெனில்
    rc = lsetfilecon_raw (path, constr);

 quit:;
  முழு err = errno;
  எனில் (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  திரும்பு rc;
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
restorecon (கட்டமைப்பு selabel_handle *selabel_handle,
            எழுத்து மாறிலி *path, bool recurse)
{
  எழுத்து *newpath = வெற்று;

  எனில் (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      எனில் (! newpath)
        திரும்பு false;
      path = newpath;
    }

  எனில் (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      முழு err = errno;
      விடுவி (newpath);
      errno = err;
      திரும்பு ok;
    }

  எழுத்து மாறிலி *ftspath[2] = { path, வெற்று };
  FTS *fts = xfts_open ((எழுத்து *மாறிலி *) ftspath, FTS_PHYSICAL, வெற்று);

  முழு err = 0;
  க்கு (FTSENT *ent; (ent = fts_read (fts)); )
    எனில் (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = errno;

  எனில் (errno != 0)
    err = errno;

  எனில் (fts_close (fts) != 0)
    err = errno;

  விடுவி (newpath);
  திரும்பு !err;
}
#endif
