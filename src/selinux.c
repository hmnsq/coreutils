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

#ضمن <config.h>
#ضمن <selinux/label.h>
#ضمن <selinux/context.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "canonicalize.h"
#ضمن "xfts.h"
#ضمن "selinux.h"

#إذا HAVE_SELINUX_LABEL_H

# إذا ! HAVE_ډېرپېښ_منزل_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
ساكن security_class_t
ډېرپېښ_منزل_security_class (mode_t m)
{

  إذا (S_ISREG (m))
    أرجع string_منزل_security_class ("file");
  إذا (S_ISDIR (m))
    أرجع string_منزل_security_class ("dir");
  إذا (S_ISCHR (m))
    أرجع string_منزل_security_class ("chr_file");
  إذا (S_ISBLK (m))
    أرجع string_منزل_security_class ("blk_file");
  إذا (S_ISFIFO (m))
    أرجع string_منزل_security_class ("fifo_file");
  إذا (S_ISLNK (m))
    أرجع string_منزل_security_class ("lnk_file");
  إذا (S_ISSOCK (m))
    أرجع string_منزل_security_class ("sock_file");

  errno = EINVAL;
  أرجع 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

ساكن صحيح
computecon_raw (حرف ثابت *لار, mode_t ډېرپېښ_2, حرف **con)
{
  حرف *scon_raw = عدم;
  حرف *tcon_raw = عدم;
  security_class_t tclass;
  صحيح rc = -1;

  حرف *dir = dir_name (لار);
  إذا (!dir)
    اذهب quit;
  إذا (getcon_raw (&scon_raw) < 0)
    اذهب quit;
  إذا (getfilecon_raw (dir, &tcon_raw) < 0)
    اذهب quit;
  tclass = ډېرپېښ_منزل_security_class (ډېرپېښ_2);
  إذا (!tclass)
    اذهب quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  صحيح err = errno;
  حرر (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  أرجع rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
صحيح
defaultcon (بنية selabel_handle *selabel_handle,
            حرف ثابت *لار, mode_t ډېرپېښ_2)
{
  صحيح rc = -1;
  حرف *scon_raw = عدم;
  حرف *tcon_raw = عدم;
  context_t scontext = عدم, tcontext = عدم;
  حرف ثابت *contype;
  حرف ثابت *constr;
  حرف *newpath = عدم;

  إذا (! IS_ABSOLUTE_FILE_NAME (لار))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_ډېرپېښ (لار, CAN_MISSING);
      إذا (! newpath)
        اذهب quit;
      لار = newpath;
    }

  إذا (selabel_lookup_raw (selabel_handle, &scon_raw, لار, ډېرپېښ_2) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      إذا (errno == ENOENT)
        errno = ENODATA;
      اذهب quit;
    }
  إذا (computecon_raw (لار, ډېرپېښ_2, &tcon_raw) < 0)
    اذهب quit;
  إذا (!(scontext = context_new (scon_raw)))
    اذهب quit;
  إذا (!(tcontext = context_new (tcon_raw)))
    اذهب quit;

  إذا (!(contype = context_type_get (scontext)))
    اذهب quit;
  إذا (context_type_سېټ (tcontext, contype))
    اذهب quit;
  إذا (!(constr = context_str (tcontext)))
    اذهب quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  صحيح err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  حرر (newpath);
  errno = err;
  أرجع rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
ساكن صحيح
restorecon_شخصي (بنية selabel_handle *selabel_handle, حرف ثابت *لار)
{
  صحيح rc = -1;
  بنية stat sb;
  حرف *scon_raw = عدم;
  حرف *tcon_raw = عدم;
  context_t scontext = عدم, tcontext = عدم;
  حرف ثابت *contype;
  حرف ثابت *constr;
  صحيح fd;

  إذا (!selabel_handle)
    {
      إذا (getfscreatecon_raw (&tcon_raw) < 0)
        أرجع rc;
      إذا (!tcon_raw)
        {
          errno = ENODATA;
          أرجع rc;
        }
      rc = lsetfilecon_raw (لار, tcon_raw);
      صحيح err = errno;
      freecon (tcon_raw);
      errno = err;
      أرجع rc;
    }

  fd = open (لار, O_RDONLY | O_NOFOLLOW);
  إذا (fd == -1 && (errno != ELOOP))
    اذهب quit;

  إذا (fd != -1)
    {
      إذا (fstat (fd, &sb) < 0)
        اذهب quit;
    }
  وإلا
    {
      إذا (lstat (لار, &sb) < 0)
        اذهب quit;
    }

  إذا (selabel_lookup_raw (selabel_handle, &scon_raw, لار, sb.st_ډېرپېښ) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      إذا (errno == ENOENT)
        errno = ENODATA;
      اذهب quit;
    }
  إذا (!(scontext = context_new (scon_raw)))
    اذهب quit;

  إذا (fd != -1)
    {
      إذا (fgetfilecon_raw (fd, &tcon_raw) < 0)
        اذهب quit;
    }
  وإلا
    {
      إذا (lgetfilecon_raw (لار, &tcon_raw) < 0)
        اذهب quit;
    }

  إذا (!(tcontext = context_new (tcon_raw)))
    اذهب quit;

  إذا (!(contype = context_type_get (scontext)))
    اذهب quit;
  إذا (context_type_سېټ (tcontext, contype))
    اذهب quit;
  إذا (!(constr = context_str (tcontext)))
    اذهب quit;

  إذا (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  وإلا
    rc = lsetfilecon_raw (لار, constr);

 quit:;
  صحيح err = errno;
  إذا (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  أرجع rc;
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
restorecon (بنية selabel_handle *selabel_handle,
            حرف ثابت *لار, bool recurse)
{
  حرف *newpath = عدم;

  إذا (! IS_ABSOLUTE_FILE_NAME (لار))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_ډېرپېښ (لار, CAN_MISSING);
      إذا (! newpath)
        أرجع false;
      لار = newpath;
    }

  إذا (! recurse)
    {
      bool ok = restorecon_شخصي (selabel_handle, لار) != -1;
      صحيح err = errno;
      حرر (newpath);
      errno = err;
      أرجع ok;
    }

  حرف ثابت *ftspath[2] = { لار, عدم };
  FTS *fts = xfts_open ((حرف *ثابت *) ftspath, FTS_PHYSICAL, عدم);

  صحيح err = 0;
  لكل (FTSENT *ent; (ent = fts_read (fts)); )
    إذا (restorecon_شخصي (selabel_handle, fts->fts_لار) < 0)
      err = errno;

  إذا (errno != 0)
    err = errno;

  إذا (fts_close (fts) != 0)
    err = errno;

  حرر (newpath);
  أرجع !err;
}
#د_که_پای
