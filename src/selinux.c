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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <selinux/label.h>
#અન્તર્ભાવ <selinux/context.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "canonicalize.h"
#અન્તર્ભાવ "xfts.h"
#અન્તર્ભાવ "selinux.h"

#જો HAVE_SELINUX_LABEL_H

# જો ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
સ્થિર security_class_t
mode_to_security_class (mode_t m)
{

  જો (S_ISREG (m))
    પાછા string_to_security_class ("file");
  જો (S_ISDIR (m))
    પાછા string_to_security_class ("dir");
  જો (S_ISCHR (m))
    પાછા string_to_security_class ("chr_file");
  જો (S_ISBLK (m))
    પાછા string_to_security_class ("blk_file");
  જો (S_ISFIFO (m))
    પાછા string_to_security_class ("fifo_file");
  જો (S_ISLNK (m))
    પાછા string_to_security_class ("lnk_file");
  જો (S_ISSOCK (m))
    પાછા string_to_security_class ("sock_file");

  errno = EINVAL;
  પાછા 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

સ્થિર પૂર્ણાંક
computecon_raw (અક્ષર અચલ *path, mode_t mode, અક્ષર **con)
{
  અક્ષર *scon_raw = ખાલી;
  અક્ષર *tcon_raw = ખાલી;
  security_class_t tclass;
  પૂર્ણાંક rc = -1;

  અક્ષર *dir = dir_name (path);
  જો (!dir)
    જાઓ quit;
  જો (getcon_raw (&scon_raw) < 0)
    જાઓ quit;
  જો (getfilecon_raw (dir, &tcon_raw) < 0)
    જાઓ quit;
  tclass = mode_to_security_class (mode);
  જો (!tclass)
    જાઓ quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  પૂર્ણાંક err = errno;
  મુક્ત (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  પાછા rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
પૂર્ણાંક
defaultcon (સંરચના selabel_handle *selabel_handle,
            અક્ષર અચલ *path, mode_t mode)
{
  પૂર્ણાંક rc = -1;
  અક્ષર *scon_raw = ખાલી;
  અક્ષર *tcon_raw = ખાલી;
  context_t scontext = ખાલી, tcontext = ખાલી;
  અક્ષર અચલ *contype;
  અક્ષર અચલ *constr;
  અક્ષર *newpath = ખાલી;

  જો (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      જો (! newpath)
        જાઓ quit;
      path = newpath;
    }

  જો (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      જો (errno == ENOENT)
        errno = ENODATA;
      જાઓ quit;
    }
  જો (computecon_raw (path, mode, &tcon_raw) < 0)
    જાઓ quit;
  જો (!(scontext = context_new (scon_raw)))
    જાઓ quit;
  જો (!(tcontext = context_new (tcon_raw)))
    જાઓ quit;

  જો (!(contype = context_type_get (scontext)))
    જાઓ quit;
  જો (context_type_set (tcontext, contype))
    જાઓ quit;
  જો (!(constr = context_str (tcontext)))
    જાઓ quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  પૂર્ણાંક err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  મુક્ત (newpath);
  errno = err;
  પાછા rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
સ્થિર પૂર્ણાંક
restorecon_private (સંરચના selabel_handle *selabel_handle, અક્ષર અચલ *path)
{
  પૂર્ણાંક rc = -1;
  સંરચના stat sb;
  અક્ષર *scon_raw = ખાલી;
  અક્ષર *tcon_raw = ખાલી;
  context_t scontext = ખાલી, tcontext = ખાલી;
  અક્ષર અચલ *contype;
  અક્ષર અચલ *constr;
  પૂર્ણાંક fd;

  જો (!selabel_handle)
    {
      જો (getfscreatecon_raw (&tcon_raw) < 0)
        પાછા rc;
      જો (!tcon_raw)
        {
          errno = ENODATA;
          પાછા rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      પૂર્ણાંક err = errno;
      freecon (tcon_raw);
      errno = err;
      પાછા rc;
    }

  fd = open (path, O_RDONLY | O_NOFOLLOW);
  જો (fd == -1 && (errno != ELOOP))
    જાઓ quit;

  જો (fd != -1)
    {
      જો (fstat (fd, &sb) < 0)
        જાઓ quit;
    }
  નહીંતર
    {
      જો (lstat (path, &sb) < 0)
        જાઓ quit;
    }

  જો (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      જો (errno == ENOENT)
        errno = ENODATA;
      જાઓ quit;
    }
  જો (!(scontext = context_new (scon_raw)))
    જાઓ quit;

  જો (fd != -1)
    {
      જો (fgetfilecon_raw (fd, &tcon_raw) < 0)
        જાઓ quit;
    }
  નહીંતર
    {
      જો (lgetfilecon_raw (path, &tcon_raw) < 0)
        જાઓ quit;
    }

  જો (!(tcontext = context_new (tcon_raw)))
    જાઓ quit;

  જો (!(contype = context_type_get (scontext)))
    જાઓ quit;
  જો (context_type_set (tcontext, contype))
    જાઓ quit;
  જો (!(constr = context_str (tcontext)))
    જાઓ quit;

  જો (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  નહીંતર
    rc = lsetfilecon_raw (path, constr);

 quit:;
  પૂર્ણાંક err = errno;
  જો (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  પાછા rc;
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
restorecon (સંરચના selabel_handle *selabel_handle,
            અક્ષર અચલ *path, bool recurse)
{
  અક્ષર *newpath = ખાલી;

  જો (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      જો (! newpath)
        પાછા false;
      path = newpath;
    }

  જો (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      પૂર્ણાંક err = errno;
      મુક્ત (newpath);
      errno = err;
      પાછા ok;
    }

  અક્ષર અચલ *ftspath[2] = { path, ખાલી };
  FTS *fts = xfts_open ((અક્ષર *અચલ *) ftspath, FTS_PHYSICAL, ખાલી);

  પૂર્ણાંક err = 0;
  માટે (FTSENT *ent; (ent = fts_read (fts)); )
    જો (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = errno;

  જો (errno != 0)
    err = errno;

  જો (fts_close (fts) != 0)
    err = errno;

  મુક્ત (newpath);
  પાછા !err;
}
#endif
