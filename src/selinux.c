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

#nếu HAVE_SELINUX_LABEL_H

# nếu ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
static security_class_t
mode_to_security_class (mode_t m)
{

  nếu (S_ISREG (m))
    trả_về string_to_security_class ("file");
  nếu (S_ISDIR (m))
    trả_về string_to_security_class ("dir");
  nếu (S_ISCHR (m))
    trả_về string_to_security_class ("chr_file");
  nếu (S_ISBLK (m))
    trả_về string_to_security_class ("blk_file");
  nếu (S_ISFIFO (m))
    trả_về string_to_security_class ("fifo_file");
  nếu (S_ISLNK (m))
    trả_về string_to_security_class ("lnk_file");
  nếu (S_ISSOCK (m))
    trả_về string_to_security_class ("sock_file");

  errno = EINVAL;
  trả_về 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

static số_nguyên
computecon_raw (char const *path, mode_t mode, char **con)
{
  char *scon_raw = NULL;
  char *tcon_raw = NULL;
  security_class_t tclass;
  số_nguyên rc = -1;

  char *dir = dir_name (path);
  nếu (!dir)
    goto quit;
  nếu (getcon_raw (&scon_raw) < 0)
    goto quit;
  nếu (getfilecon_raw (dir, &tcon_raw) < 0)
    goto quit;
  tclass = mode_to_security_class (mode);
  nếu (!tclass)
    goto quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  số_nguyên err = errno;
  free (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  trả_về rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
số_nguyên
defaultcon (struct selabel_handle *selabel_handle,
            char const *path, mode_t mode)
{
  số_nguyên rc = -1;
  char *scon_raw = NULL;
  char *tcon_raw = NULL;
  context_t scontext = NULL, tcontext = NULL;
  char const *contype;
  char const *constr;
  char *newpath = NULL;

  nếu (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      nếu (! newpath)
        goto quit;
      path = newpath;
    }

  nếu (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      nếu (errno == ENOENT)
        errno = ENODATA;
      goto quit;
    }
  nếu (computecon_raw (path, mode, &tcon_raw) < 0)
    goto quit;
  nếu (!(scontext = context_new (scon_raw)))
    goto quit;
  nếu (!(tcontext = context_new (tcon_raw)))
    goto quit;

  nếu (!(contype = context_type_get (scontext)))
    goto quit;
  nếu (context_type_set (tcontext, contype))
    goto quit;
  nếu (!(constr = context_str (tcontext)))
    goto quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  số_nguyên err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  free (newpath);
  errno = err;
  trả_về rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
static số_nguyên
restorecon_private (struct selabel_handle *selabel_handle, char const *path)
{
  số_nguyên rc = -1;
  struct stat sb;
  char *scon_raw = NULL;
  char *tcon_raw = NULL;
  context_t scontext = NULL, tcontext = NULL;
  char const *contype;
  char const *constr;
  số_nguyên fd;

  nếu (!selabel_handle)
    {
      nếu (getfscreatecon_raw (&tcon_raw) < 0)
        trả_về rc;
      nếu (!tcon_raw)
        {
          errno = ENODATA;
          trả_về rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      số_nguyên err = errno;
      freecon (tcon_raw);
      errno = err;
      trả_về rc;
    }

  fd = open (path, O_RDONLY | O_NOFOLLOW);
  nếu (fd == -1 && (errno != ELOOP))
    goto quit;

  nếu (fd != -1)
    {
      nếu (fstat (fd, &sb) < 0)
        goto quit;
    }
  không_thì
    {
      nếu (lstat (path, &sb) < 0)
        goto quit;
    }

  nếu (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      nếu (errno == ENOENT)
        errno = ENODATA;
      goto quit;
    }
  nếu (!(scontext = context_new (scon_raw)))
    goto quit;

  nếu (fd != -1)
    {
      nếu (fgetfilecon_raw (fd, &tcon_raw) < 0)
        goto quit;
    }
  không_thì
    {
      nếu (lgetfilecon_raw (path, &tcon_raw) < 0)
        goto quit;
    }

  nếu (!(tcontext = context_new (tcon_raw)))
    goto quit;

  nếu (!(contype = context_type_get (scontext)))
    goto quit;
  nếu (context_type_set (tcontext, contype))
    goto quit;
  nếu (!(constr = context_str (tcontext)))
    goto quit;

  nếu (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  không_thì
    rc = lsetfilecon_raw (path, constr);

 quit:;
  số_nguyên err = errno;
  nếu (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  trả_về rc;
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
restorecon (struct selabel_handle *selabel_handle,
            char const *path, bool recurse)
{
  char *newpath = NULL;

  nếu (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      nếu (! newpath)
        trả_về false;
      path = newpath;
    }

  nếu (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      số_nguyên err = errno;
      free (newpath);
      errno = err;
      trả_về ok;
    }

  char const *ftspath[2] = { path, NULL };
  FTS *fts = xfts_open ((char *const *) ftspath, FTS_PHYSICAL, NULL);

  số_nguyên err = 0;
  cho (FTSENT *ent; (ent = fts_read (fts)); )
    nếu (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = errno;

  nếu (errno != 0)
    err = errno;

  nếu (fts_close (fts) != 0)
    err = errno;

  free (newpath);
  trả_về !err;
}
#endif
