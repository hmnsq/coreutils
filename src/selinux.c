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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <selinux/label.h>
#അന്തര്ഭാവ <selinux/context.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "canonicalize.h"
#അന്തര്ഭാവ "xfts.h"
#അന്തര്ഭാവ "selinux.h"

#എങ്കിൽ HAVE_SELINUX_LABEL_H

# എങ്കിൽ ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
സ്ഥിതം security_class_t
mode_to_security_class (mode_t m)
{

  എങ്കിൽ (S_ISREG (m))
    തിരികെ string_to_security_class ("file");
  എങ്കിൽ (S_ISDIR (m))
    തിരികെ string_to_security_class ("dir");
  എങ്കിൽ (S_ISCHR (m))
    തിരികെ string_to_security_class ("chr_file");
  എങ്കിൽ (S_ISBLK (m))
    തിരികെ string_to_security_class ("blk_file");
  എങ്കിൽ (S_ISFIFO (m))
    തിരികെ string_to_security_class ("fifo_file");
  എങ്കിൽ (S_ISLNK (m))
    തിരികെ string_to_security_class ("lnk_file");
  എങ്കിൽ (S_ISSOCK (m))
    തിരികെ string_to_security_class ("sock_file");

  errno = EINVAL;
  തിരികെ 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

സ്ഥിതം പൂർണ്ണസംഖ്യ
computecon_raw (അക്ഷരം സ്ഥിരം *path, mode_t mode, അക്ഷരം **con)
{
  അക്ഷരം *scon_raw = ശൂന്യ;
  അക്ഷരം *tcon_raw = ശൂന്യ;
  security_class_t tclass;
  പൂർണ്ണസംഖ്യ rc = -1;

  അക്ഷരം *dir = dir_name (path);
  എങ്കിൽ (!dir)
    പോകുക quit;
  എങ്കിൽ (getcon_raw (&scon_raw) < 0)
    പോകുക quit;
  എങ്കിൽ (getfilecon_raw (dir, &tcon_raw) < 0)
    പോകുക quit;
  tclass = mode_to_security_class (mode);
  എങ്കിൽ (!tclass)
    പോകുക quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  പൂർണ്ണസംഖ്യ err = errno;
  സ്വതന്ത്രം (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  തിരികെ rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
പൂർണ്ണസംഖ്യ
defaultcon (ഘടന selabel_handle *selabel_handle,
            അക്ഷരം സ്ഥിരം *path, mode_t mode)
{
  പൂർണ്ണസംഖ്യ rc = -1;
  അക്ഷരം *scon_raw = ശൂന്യ;
  അക്ഷരം *tcon_raw = ശൂന്യ;
  context_t scontext = ശൂന്യ, tcontext = ശൂന്യ;
  അക്ഷരം സ്ഥിരം *contype;
  അക്ഷരം സ്ഥിരം *constr;
  അക്ഷരം *newpath = ശൂന്യ;

  എങ്കിൽ (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      എങ്കിൽ (! newpath)
        പോകുക quit;
      path = newpath;
    }

  എങ്കിൽ (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      എങ്കിൽ (errno == ENOENT)
        errno = ENODATA;
      പോകുക quit;
    }
  എങ്കിൽ (computecon_raw (path, mode, &tcon_raw) < 0)
    പോകുക quit;
  എങ്കിൽ (!(scontext = context_new (scon_raw)))
    പോകുക quit;
  എങ്കിൽ (!(tcontext = context_new (tcon_raw)))
    പോകുക quit;

  എങ്കിൽ (!(contype = context_type_get (scontext)))
    പോകുക quit;
  എങ്കിൽ (context_type_set (tcontext, contype))
    പോകുക quit;
  എങ്കിൽ (!(constr = context_str (tcontext)))
    പോകുക quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  പൂർണ്ണസംഖ്യ err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  സ്വതന്ത്രം (newpath);
  errno = err;
  തിരികെ rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
സ്ഥിതം പൂർണ്ണസംഖ്യ
restorecon_private (ഘടന selabel_handle *selabel_handle, അക്ഷരം സ്ഥിരം *path)
{
  പൂർണ്ണസംഖ്യ rc = -1;
  ഘടന stat sb;
  അക്ഷരം *scon_raw = ശൂന്യ;
  അക്ഷരം *tcon_raw = ശൂന്യ;
  context_t scontext = ശൂന്യ, tcontext = ശൂന്യ;
  അക്ഷരം സ്ഥിരം *contype;
  അക്ഷരം സ്ഥിരം *constr;
  പൂർണ്ണസംഖ്യ fd;

  എങ്കിൽ (!selabel_handle)
    {
      എങ്കിൽ (getfscreatecon_raw (&tcon_raw) < 0)
        തിരികെ rc;
      എങ്കിൽ (!tcon_raw)
        {
          errno = ENODATA;
          തിരികെ rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      പൂർണ്ണസംഖ്യ err = errno;
      freecon (tcon_raw);
      errno = err;
      തിരികെ rc;
    }

  fd = open (path, O_RDONLY | O_NOFOLLOW);
  എങ്കിൽ (fd == -1 && (errno != ELOOP))
    പോകുക quit;

  എങ്കിൽ (fd != -1)
    {
      എങ്കിൽ (fstat (fd, &sb) < 0)
        പോകുക quit;
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (lstat (path, &sb) < 0)
        പോകുക quit;
    }

  എങ്കിൽ (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      എങ്കിൽ (errno == ENOENT)
        errno = ENODATA;
      പോകുക quit;
    }
  എങ്കിൽ (!(scontext = context_new (scon_raw)))
    പോകുക quit;

  എങ്കിൽ (fd != -1)
    {
      എങ്കിൽ (fgetfilecon_raw (fd, &tcon_raw) < 0)
        പോകുക quit;
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (lgetfilecon_raw (path, &tcon_raw) < 0)
        പോകുക quit;
    }

  എങ്കിൽ (!(tcontext = context_new (tcon_raw)))
    പോകുക quit;

  എങ്കിൽ (!(contype = context_type_get (scontext)))
    പോകുക quit;
  എങ്കിൽ (context_type_set (tcontext, contype))
    പോകുക quit;
  എങ്കിൽ (!(constr = context_str (tcontext)))
    പോകുക quit;

  എങ്കിൽ (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  അല്ലെങ്കിൽ
    rc = lsetfilecon_raw (path, constr);

 quit:;
  പൂർണ്ണസംഖ്യ err = errno;
  എങ്കിൽ (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  തിരികെ rc;
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
restorecon (ഘടന selabel_handle *selabel_handle,
            അക്ഷരം സ്ഥിരം *path, bool recurse)
{
  അക്ഷരം *newpath = ശൂന്യ;

  എങ്കിൽ (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      എങ്കിൽ (! newpath)
        തിരികെ false;
      path = newpath;
    }

  എങ്കിൽ (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      പൂർണ്ണസംഖ്യ err = errno;
      സ്വതന്ത്രം (newpath);
      errno = err;
      തിരികെ ok;
    }

  അക്ഷരം സ്ഥിരം *ftspath[2] = { path, ശൂന്യ };
  FTS *fts = xfts_open ((അക്ഷരം *സ്ഥിരം *) ftspath, FTS_PHYSICAL, ശൂന്യ);

  പൂർണ്ണസംഖ്യ err = 0;
  വേണ്ടി (FTSENT *ent; (ent = fts_read (fts)); )
    എങ്കിൽ (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = errno;

  എങ്കിൽ (errno != 0)
    err = errno;

  എങ്കിൽ (fts_close (fts) != 0)
    err = errno;

  സ്വതന്ത്രം (newpath);
  തിരികെ !err;
}
#endif
