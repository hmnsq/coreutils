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

#समावेश <config.h>
#समावेश <selinux/label.h>
#समावेश <selinux/context.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "canonicalize.h"
#समावेश "xfts.h"
#समावेश "selinux.h"

#अगर HAVE_SELINUX_LABEL_H

# अगर ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
अटल security_class_t
mode_to_security_class (mode_t m)
{

  अगर (S_ISREG (m))
    वापस string_to_security_class ("file");
  अगर (S_ISDIR (m))
    वापस string_to_security_class ("dir");
  अगर (S_ISCHR (m))
    वापस string_to_security_class ("chr_file");
  अगर (S_ISBLK (m))
    वापस string_to_security_class ("blk_file");
  अगर (S_ISFIFO (m))
    वापस string_to_security_class ("fifo_file");
  अगर (S_ISLNK (m))
    वापस string_to_security_class ("lnk_file");
  अगर (S_ISSOCK (m))
    वापस string_to_security_class ("sock_file");

  त्रुटि_सं = EINVAL;
  वापस 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

अटल पूर्णांक
computecon_raw (अक्षर स्थिर *path, mode_t mode, अक्षर **con)
{
  अक्षर *scon_raw = शून्य;
  अक्षर *tcon_raw = शून्य;
  security_class_t tclass;
  पूर्णांक rc = -1;

  अक्षर *dir = dir_name (path);
  अगर (!dir)
    जाओ quit;
  अगर (getcon_raw (&scon_raw) < 0)
    जाओ quit;
  अगर (getfilecon_raw (dir, &tcon_raw) < 0)
    जाओ quit;
  tclass = mode_to_security_class (mode);
  अगर (!tclass)
    जाओ quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  पूर्णांक err = त्रुटि_सं;
  मुक्त (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  त्रुटि_सं = err;
  वापस rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
पूर्णांक
defaultcon (काष्ठा selabel_handle *selabel_handle,
            अक्षर स्थिर *path, mode_t mode)
{
  पूर्णांक rc = -1;
  अक्षर *scon_raw = शून्य;
  अक्षर *tcon_raw = शून्य;
  context_t scontext = शून्य, tcontext = शून्य;
  अक्षर स्थिर *contype;
  अक्षर स्थिर *constr;
  अक्षर *newpath = शून्य;

  अगर (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      अगर (! newpath)
        जाओ quit;
      path = newpath;
    }

  अगर (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      अगर (त्रुटि_सं == ENOENT)
        त्रुटि_सं = ENODATA;
      जाओ quit;
    }
  अगर (computecon_raw (path, mode, &tcon_raw) < 0)
    जाओ quit;
  अगर (!(scontext = context_new (scon_raw)))
    जाओ quit;
  अगर (!(tcontext = context_new (tcon_raw)))
    जाओ quit;

  अगर (!(contype = context_type_get (scontext)))
    जाओ quit;
  अगर (context_type_set (tcontext, contype))
    जाओ quit;
  अगर (!(constr = context_str (tcontext)))
    जाओ quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  पूर्णांक err = त्रुटि_सं;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  मुक्त (newpath);
  त्रुटि_सं = err;
  वापस rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
अटल पूर्णांक
restorecon_private (काष्ठा selabel_handle *selabel_handle, अक्षर स्थिर *path)
{
  पूर्णांक rc = -1;
  काष्ठा stat sb;
  अक्षर *scon_raw = शून्य;
  अक्षर *tcon_raw = शून्य;
  context_t scontext = शून्य, tcontext = शून्य;
  अक्षर स्थिर *contype;
  अक्षर स्थिर *constr;
  पूर्णांक fd;

  अगर (!selabel_handle)
    {
      अगर (getfscreatecon_raw (&tcon_raw) < 0)
        वापस rc;
      अगर (!tcon_raw)
        {
          त्रुटि_सं = ENODATA;
          वापस rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      पूर्णांक err = त्रुटि_सं;
      freecon (tcon_raw);
      त्रुटि_सं = err;
      वापस rc;
    }

  fd = खोलो (path, O_RDONLY | O_NOFOLLOW);
  अगर (fd == -1 && (त्रुटि_सं != ELOOP))
    जाओ quit;

  अगर (fd != -1)
    {
      अगर (ख_स्थिति (fd, &sb) < 0)
        जाओ quit;
    }
  अन्यथा
    {
      अगर (lstat (path, &sb) < 0)
        जाओ quit;
    }

  अगर (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      अगर (त्रुटि_सं == ENOENT)
        त्रुटि_सं = ENODATA;
      जाओ quit;
    }
  अगर (!(scontext = context_new (scon_raw)))
    जाओ quit;

  अगर (fd != -1)
    {
      अगर (fgetfilecon_raw (fd, &tcon_raw) < 0)
        जाओ quit;
    }
  अन्यथा
    {
      अगर (lgetfilecon_raw (path, &tcon_raw) < 0)
        जाओ quit;
    }

  अगर (!(tcontext = context_new (tcon_raw)))
    जाओ quit;

  अगर (!(contype = context_type_get (scontext)))
    जाओ quit;
  अगर (context_type_set (tcontext, contype))
    जाओ quit;
  अगर (!(constr = context_str (tcontext)))
    जाओ quit;

  अगर (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  अन्यथा
    rc = lsetfilecon_raw (path, constr);

 quit:;
  पूर्णांक err = त्रुटि_सं;
  अगर (fd != -1)
    बंद (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  त्रुटि_सं = err;
  वापस rc;
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
restorecon (काष्ठा selabel_handle *selabel_handle,
            अक्षर स्थिर *path, bool recurse)
{
  अक्षर *newpath = शून्य;

  अगर (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      अगर (! newpath)
        वापस false;
      path = newpath;
    }

  अगर (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      पूर्णांक err = त्रुटि_सं;
      मुक्त (newpath);
      त्रुटि_सं = err;
      वापस ok;
    }

  अक्षर स्थिर *ftspath[2] = { path, शून्य };
  FTS *fts = xfts_open ((अक्षर *स्थिर *) ftspath, FTS_PHYSICAL, शून्य);

  पूर्णांक err = 0;
  क्रम (FTSENT *ent; (ent = fts_read (fts)); )
    अगर (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = त्रुटि_सं;

  अगर (त्रुटि_सं != 0)
    err = त्रुटि_सं;

  अगर (fts_close (fts) != 0)
    err = त्रुटि_सं;

  मुक्त (newpath);
  वापस !err;
}
#पूर्ण_अगर
