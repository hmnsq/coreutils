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

#include <config.h>
#include <selinux/label.h>
#include <selinux/context.h>
#include <sys/types.h>

#include "system.h"
#include "canonicalize.h"
#include "xfts.h"
#include "selinux.h"

#གལ་སྲིད HAVE_SELINUX_LABEL_H

# གལ་སྲིད ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
static security_class_t
mode_to_security_class (mode_t m)
{

  གལ་སྲིད (S_ISREG (m))
    ལོག string_to_security_class ("file");
  གལ་སྲིད (S_ISDIR (m))
    ལོག string_to_security_class ("dir");
  གལ་སྲིད (S_ISCHR (m))
    ལོག string_to_security_class ("chr_file");
  གལ་སྲིད (S_ISBLK (m))
    ལོག string_to_security_class ("blk_file");
  གལ་སྲིད (S_ISFIFO (m))
    ལོག string_to_security_class ("fifo_file");
  གལ་སྲིད (S_ISLNK (m))
    ལོག string_to_security_class ("lnk_file");
  གལ་སྲིད (S_ISSOCK (m))
    ལོག string_to_security_class ("sock_file");

  errno = EINVAL;
  ལོག 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

static ཧྲིལ
computecon_raw (var_na const *path, mode_t mode, var_na **con)
{
  var_na *scon_raw = NULL;
  var_na *tcon_raw = NULL;
  security_class_t tclass;
  ཧྲིལ rc = -1;

  var_na *dir = dir_name (path);
  གལ་སྲིད (!dir)
    goto quit;
  གལ་སྲིད (getcon_raw (&scon_raw) < 0)
    goto quit;
  གལ་སྲིད (getfilecon_raw (dir, &tcon_raw) < 0)
    goto quit;
  tclass = mode_to_security_class (mode);
  གལ་སྲིད (!tclass)
    goto quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  ཧྲིལ err = errno;
  free (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ལོག rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
ཧྲིལ
defaultcon (sa_nracanaa selabel_handle *selabel_handle,
            var_na const *path, mode_t mode)
{
  ཧྲིལ rc = -1;
  var_na *scon_raw = NULL;
  var_na *tcon_raw = NULL;
  context_t scontext = NULL, tcontext = NULL;
  var_na const *contype;
  var_na const *constr;
  var_na *newpath = NULL;

  གལ་སྲིད (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      གལ་སྲིད (! newpath)
        goto quit;
      path = newpath;
    }

  གལ་སྲིད (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      གལ་སྲིད (errno == ENOENT)
        errno = ENODATA;
      goto quit;
    }
  གལ་སྲིད (computecon_raw (path, mode, &tcon_raw) < 0)
    goto quit;
  གལ་སྲིད (!(scontext = context_new (scon_raw)))
    goto quit;
  གལ་སྲིད (!(tcontext = context_new (tcon_raw)))
    goto quit;

  གལ་སྲིད (!(contype = context_type_get (scontext)))
    goto quit;
  གལ་སྲིད (context_type_set (tcontext, contype))
    goto quit;
  གལ་སྲིད (!(constr = context_str (tcontext)))
    goto quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  ཧྲིལ err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  free (newpath);
  errno = err;
  ལོག rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
static ཧྲིལ
restorecon_private (sa_nracanaa selabel_handle *selabel_handle, var_na const *path)
{
  ཧྲིལ rc = -1;
  sa_nracanaa stat sb;
  var_na *scon_raw = NULL;
  var_na *tcon_raw = NULL;
  context_t scontext = NULL, tcontext = NULL;
  var_na const *contype;
  var_na const *constr;
  ཧྲིལ fd;

  གལ་སྲིད (!selabel_handle)
    {
      གལ་སྲིད (getfscreatecon_raw (&tcon_raw) < 0)
        ལོག rc;
      གལ་སྲིད (!tcon_raw)
        {
          errno = ENODATA;
          ལོག rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      ཧྲིལ err = errno;
      freecon (tcon_raw);
      errno = err;
      ལོག rc;
    }

  fd = open (path, O_RDONLY | O_NOFOLLOW);
  གལ་སྲིད (fd == -1 && (errno != ELOOP))
    goto quit;

  གལ་སྲིད (fd != -1)
    {
      གལ་སྲིད (fstat (fd, &sb) < 0)
        goto quit;
    }
  དེ་མིན
    {
      གལ་སྲིད (lstat (path, &sb) < 0)
        goto quit;
    }

  གལ་སྲིད (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      གལ་སྲིད (errno == ENOENT)
        errno = ENODATA;
      goto quit;
    }
  གལ་སྲིད (!(scontext = context_new (scon_raw)))
    goto quit;

  གལ་སྲིད (fd != -1)
    {
      གལ་སྲིད (fgetfilecon_raw (fd, &tcon_raw) < 0)
        goto quit;
    }
  དེ་མིན
    {
      གལ་སྲིད (lgetfilecon_raw (path, &tcon_raw) < 0)
        goto quit;
    }

  གལ་སྲིད (!(tcontext = context_new (tcon_raw)))
    goto quit;

  གལ་སྲིད (!(contype = context_type_get (scontext)))
    goto quit;
  གལ་སྲིད (context_type_set (tcontext, contype))
    goto quit;
  གལ་སྲིད (!(constr = context_str (tcontext)))
    goto quit;

  གལ་སྲིད (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  དེ་མིན
    rc = lsetfilecon_raw (path, constr);

 quit:;
  ཧྲིལ err = errno;
  གལ་སྲིད (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ལོག rc;
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
restorecon (sa_nracanaa selabel_handle *selabel_handle,
            var_na const *path, bool recurse)
{
  var_na *newpath = NULL;

  གལ་སྲིད (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      གལ་སྲིད (! newpath)
        ལོག false;
      path = newpath;
    }

  གལ་སྲིད (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      ཧྲིལ err = errno;
      free (newpath);
      errno = err;
      ལོག ok;
    }

  var_na const *ftspath[2] = { path, NULL };
  FTS *fts = xfts_open ((var_na *const *) ftspath, FTS_PHYSICAL, NULL);

  ཧྲིལ err = 0;
  དོན་ལུ (FTSENT *ent; (ent = fts_read (fts)); )
    གལ་སྲིད (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = errno;

  གལ་སྲིད (errno != 0)
    err = errno;

  གལ་སྲིད (fts_close (fts) != 0)
    err = errno;

  free (newpath);
  ལོག !err;
}
#endif
