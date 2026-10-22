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

#если HAVE_SELINUX_LABEL_H

# если ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
static security_class_t
mode_to_security_class (mode_t m)
{

  если (S_ISREG (m))
    вернуть string_to_security_class ("file");
  если (S_ISDIR (m))
    вернуть string_to_security_class ("dir");
  если (S_ISCHR (m))
    вернуть string_to_security_class ("chr_file");
  если (S_ISBLK (m))
    вернуть string_to_security_class ("blk_file");
  если (S_ISFIFO (m))
    вернуть string_to_security_class ("fifo_file");
  если (S_ISLNK (m))
    вернуть string_to_security_class ("lnk_file");
  если (S_ISSOCK (m))
    вернуть string_to_security_class ("sock_file");

  errno = EINVAL;
  вернуть 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

static целое
computecon_raw (char const *path, mode_t mode, char **con)
{
  char *scon_raw = NULL;
  char *tcon_raw = NULL;
  security_class_t tclass;
  целое rc = -1;

  char *dir = dir_name (path);
  если (!dir)
    goto quit;
  если (getcon_raw (&scon_raw) < 0)
    goto quit;
  если (getfilecon_raw (dir, &tcon_raw) < 0)
    goto quit;
  tclass = mode_to_security_class (mode);
  если (!tclass)
    goto quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  целое err = errno;
  free (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  вернуть rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
целое
defaultcon (struct selabel_handle *selabel_handle,
            char const *path, mode_t mode)
{
  целое rc = -1;
  char *scon_raw = NULL;
  char *tcon_raw = NULL;
  context_t scontext = NULL, tcontext = NULL;
  char const *contype;
  char const *constr;
  char *newpath = NULL;

  если (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      если (! newpath)
        goto quit;
      path = newpath;
    }

  если (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      если (errno == ENOENT)
        errno = ENODATA;
      goto quit;
    }
  если (computecon_raw (path, mode, &tcon_raw) < 0)
    goto quit;
  если (!(scontext = context_new (scon_raw)))
    goto quit;
  если (!(tcontext = context_new (tcon_raw)))
    goto quit;

  если (!(contype = context_type_get (scontext)))
    goto quit;
  если (context_type_set (tcontext, contype))
    goto quit;
  если (!(constr = context_str (tcontext)))
    goto quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  целое err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  free (newpath);
  errno = err;
  вернуть rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
static целое
restorecon_private (struct selabel_handle *selabel_handle, char const *path)
{
  целое rc = -1;
  struct stat sb;
  char *scon_raw = NULL;
  char *tcon_raw = NULL;
  context_t scontext = NULL, tcontext = NULL;
  char const *contype;
  char const *constr;
  целое fd;

  если (!selabel_handle)
    {
      если (getfscreatecon_raw (&tcon_raw) < 0)
        вернуть rc;
      если (!tcon_raw)
        {
          errno = ENODATA;
          вернуть rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      целое err = errno;
      freecon (tcon_raw);
      errno = err;
      вернуть rc;
    }

  fd = open (path, O_RDONLY | O_NOFOLLOW);
  если (fd == -1 && (errno != ELOOP))
    goto quit;

  если (fd != -1)
    {
      если (fstat (fd, &sb) < 0)
        goto quit;
    }
  иначе
    {
      если (lstat (path, &sb) < 0)
        goto quit;
    }

  если (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      если (errno == ENOENT)
        errno = ENODATA;
      goto quit;
    }
  если (!(scontext = context_new (scon_raw)))
    goto quit;

  если (fd != -1)
    {
      если (fgetfilecon_raw (fd, &tcon_raw) < 0)
        goto quit;
    }
  иначе
    {
      если (lgetfilecon_raw (path, &tcon_raw) < 0)
        goto quit;
    }

  если (!(tcontext = context_new (tcon_raw)))
    goto quit;

  если (!(contype = context_type_get (scontext)))
    goto quit;
  если (context_type_set (tcontext, contype))
    goto quit;
  если (!(constr = context_str (tcontext)))
    goto quit;

  если (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  иначе
    rc = lsetfilecon_raw (path, constr);

 quit:;
  целое err = errno;
  если (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  вернуть rc;
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

  если (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      если (! newpath)
        вернуть false;
      path = newpath;
    }

  если (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      целое err = errno;
      free (newpath);
      errno = err;
      вернуть ok;
    }

  char const *ftspath[2] = { path, NULL };
  FTS *fts = xfts_open ((char *const *) ftspath, FTS_PHYSICAL, NULL);

  целое err = 0;
  для (FTSENT *ent; (ent = fts_read (fts)); )
    если (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = errno;

  если (errno != 0)
    err = errno;

  если (fts_close (fts) != 0)
    err = errno;

  free (newpath);
  вернуть !err;
}
#endif
