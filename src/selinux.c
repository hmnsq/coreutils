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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <selinux/label.h>
#ಅನ್ತರ್ಭಾವ <selinux/context.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "canonicalize.h"
#ಅನ್ತರ್ಭಾವ "xfts.h"
#ಅನ್ತರ್ಭಾವ "selinux.h"

#ಆದರೆ HAVE_SELINUX_LABEL_H

# ಆದರೆ ! HAVE_ಬಹುಲಕ_ಗಮ್ಯ_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
ಸ್ಥಿರ security_class_t
ಬಹುಲಕ_ಗಮ್ಯ_security_class (mode_t m)
{

  ಆದರೆ (S_ISREG (m))
    ಹಿಂತಿರುಗಿ string_ಗಮ್ಯ_security_class ("file");
  ಆದರೆ (S_ISDIR (m))
    ಹಿಂತಿರುಗಿ string_ಗಮ್ಯ_security_class ("dir");
  ಆದರೆ (S_ISCHR (m))
    ಹಿಂತಿರುಗಿ string_ಗಮ್ಯ_security_class ("chr_file");
  ಆದರೆ (S_ISBLK (m))
    ಹಿಂತಿರುಗಿ string_ಗಮ್ಯ_security_class ("blk_file");
  ಆದರೆ (S_ISFIFO (m))
    ಹಿಂತಿರುಗಿ string_ಗಮ್ಯ_security_class ("fifo_file");
  ಆದರೆ (S_ISLNK (m))
    ಹಿಂತಿರುಗಿ string_ಗಮ್ಯ_security_class ("lnk_file");
  ಆದರೆ (S_ISSOCK (m))
    ಹಿಂತಿರುಗಿ string_ಗಮ್ಯ_security_class ("sock_file");

  errno = EINVAL;
  ಹಿಂತಿರುಗಿ 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
computecon_raw (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮಾರ್ಗ, mode_t ಬಹುಲಕ_2, ಅಕ್ಷರ **con)
{
  ಅಕ್ಷರ *scon_raw = ಖಾಲಿ;
  ಅಕ್ಷರ *tcon_raw = ಖಾಲಿ;
  security_class_t tclass;
  ಪೂರ್ಣಾಂಕ rc = -1;

  ಅಕ್ಷರ *dir = dir_name (ಮಾರ್ಗ);
  ಆದರೆ (!dir)
    ಹೋಗು quit;
  ಆದರೆ (getcon_raw (&scon_raw) < 0)
    ಹೋಗು quit;
  ಆದರೆ (getfilecon_raw (dir, &tcon_raw) < 0)
    ಹೋಗು quit;
  tclass = ಬಹುಲಕ_ಗಮ್ಯ_security_class (ಬಹುಲಕ_2);
  ಆದರೆ (!tclass)
    ಹೋಗು quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  ಪೂರ್ಣಾಂಕ err = errno;
  ಬಿಡುಗಡೆ (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ಹಿಂತಿರುಗಿ rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
ಪೂರ್ಣಾಂಕ
defaultcon (ರಚನೆ selabel_handle *selabel_handle,
            ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮಾರ್ಗ, mode_t ಬಹುಲಕ_2)
{
  ಪೂರ್ಣಾಂಕ rc = -1;
  ಅಕ್ಷರ *scon_raw = ಖಾಲಿ;
  ಅಕ್ಷರ *tcon_raw = ಖಾಲಿ;
  context_t scontext = ಖಾಲಿ, tcontext = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *contype;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *constr;
  ಅಕ್ಷರ *newpath = ಖಾಲಿ;

  ಆದರೆ (! IS_ABSOLUTE_FILE_NAME (ಮಾರ್ಗ))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_ಬಹುಲಕ (ಮಾರ್ಗ, CAN_MISSING);
      ಆದರೆ (! newpath)
        ಹೋಗು quit;
      ಮಾರ್ಗ = newpath;
    }

  ಆದರೆ (selabel_lookup_raw (selabel_handle, &scon_raw, ಮಾರ್ಗ, ಬಹುಲಕ_2) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      ಆದರೆ (errno == ENOENT)
        errno = ENODATA;
      ಹೋಗು quit;
    }
  ಆದರೆ (computecon_raw (ಮಾರ್ಗ, ಬಹುಲಕ_2, &tcon_raw) < 0)
    ಹೋಗು quit;
  ಆದರೆ (!(scontext = context_new (scon_raw)))
    ಹೋಗು quit;
  ಆದರೆ (!(tcontext = context_new (tcon_raw)))
    ಹೋಗು quit;

  ಆದರೆ (!(contype = context_type_get (scontext)))
    ಹೋಗು quit;
  ಆದರೆ (context_type_ಗಣ (tcontext, contype))
    ಹೋಗು quit;
  ಆದರೆ (!(constr = context_str (tcontext)))
    ಹೋಗು quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  ಪೂರ್ಣಾಂಕ err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  ಬಿಡುಗಡೆ (newpath);
  errno = err;
  ಹಿಂತಿರುಗಿ rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
restorecon_ಖಾಸಗಿ (ರಚನೆ selabel_handle *selabel_handle, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮಾರ್ಗ)
{
  ಪೂರ್ಣಾಂಕ rc = -1;
  ರಚನೆ stat sb;
  ಅಕ್ಷರ *scon_raw = ಖಾಲಿ;
  ಅಕ್ಷರ *tcon_raw = ಖಾಲಿ;
  context_t scontext = ಖಾಲಿ, tcontext = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *contype;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *constr;
  ಪೂರ್ಣಾಂಕ fd;

  ಆದರೆ (!selabel_handle)
    {
      ಆದರೆ (getfscreatecon_raw (&tcon_raw) < 0)
        ಹಿಂತಿರುಗಿ rc;
      ಆದರೆ (!tcon_raw)
        {
          errno = ENODATA;
          ಹಿಂತಿರುಗಿ rc;
        }
      rc = lsetfilecon_raw (ಮಾರ್ಗ, tcon_raw);
      ಪೂರ್ಣಾಂಕ err = errno;
      freecon (tcon_raw);
      errno = err;
      ಹಿಂತಿರುಗಿ rc;
    }

  fd = open (ಮಾರ್ಗ, O_RDONLY | O_NOFOLLOW);
  ಆದರೆ (fd == -1 && (errno != ELOOP))
    ಹೋಗು quit;

  ಆದರೆ (fd != -1)
    {
      ಆದರೆ (fstat (fd, &sb) < 0)
        ಹೋಗು quit;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (lstat (ಮಾರ್ಗ, &sb) < 0)
        ಹೋಗು quit;
    }

  ಆದರೆ (selabel_lookup_raw (selabel_handle, &scon_raw, ಮಾರ್ಗ, sb.st_ಬಹುಲಕ) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      ಆದರೆ (errno == ENOENT)
        errno = ENODATA;
      ಹೋಗು quit;
    }
  ಆದರೆ (!(scontext = context_new (scon_raw)))
    ಹೋಗು quit;

  ಆದರೆ (fd != -1)
    {
      ಆದರೆ (fgetfilecon_raw (fd, &tcon_raw) < 0)
        ಹೋಗು quit;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (lgetfilecon_raw (ಮಾರ್ಗ, &tcon_raw) < 0)
        ಹೋಗು quit;
    }

  ಆದರೆ (!(tcontext = context_new (tcon_raw)))
    ಹೋಗು quit;

  ಆದರೆ (!(contype = context_type_get (scontext)))
    ಹೋಗು quit;
  ಆದರೆ (context_type_ಗಣ (tcontext, contype))
    ಹೋಗು quit;
  ಆದರೆ (!(constr = context_str (tcontext)))
    ಹೋಗು quit;

  ಆದರೆ (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  ಇಲ್ಲದಿದ್ದರೆ
    rc = lsetfilecon_raw (ಮಾರ್ಗ, constr);

 quit:;
  ಪೂರ್ಣಾಂಕ err = errno;
  ಆದರೆ (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ಹಿಂತಿರುಗಿ rc;
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
restorecon (ರಚನೆ selabel_handle *selabel_handle,
            ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮಾರ್ಗ, bool recurse)
{
  ಅಕ್ಷರ *newpath = ಖಾಲಿ;

  ಆದರೆ (! IS_ABSOLUTE_FILE_NAME (ಮಾರ್ಗ))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_ಬಹುಲಕ (ಮಾರ್ಗ, CAN_MISSING);
      ಆದರೆ (! newpath)
        ಹಿಂತಿರುಗಿ false;
      ಮಾರ್ಗ = newpath;
    }

  ಆದರೆ (! recurse)
    {
      bool ok = restorecon_ಖಾಸಗಿ (selabel_handle, ಮಾರ್ಗ) != -1;
      ಪೂರ್ಣಾಂಕ err = errno;
      ಬಿಡುಗಡೆ (newpath);
      errno = err;
      ಹಿಂತಿರುಗಿ ok;
    }

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ftspath[2] = { ಮಾರ್ಗ, ಖಾಲಿ };
  FTS *fts = xfts_open ((ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *) ftspath, FTS_PHYSICAL, ಖಾಲಿ);

  ಪೂರ್ಣಾಂಕ err = 0;
  ಗಾಗಿ (FTSENT *ent; (ent = fts_read (fts)); )
    ಆದರೆ (restorecon_ಖಾಸಗಿ (selabel_handle, fts->fts_ಮಾರ್ಗ) < 0)
      err = errno;

  ಆದರೆ (errno != 0)
    err = errno;

  ಆದರೆ (fts_close (fts) != 0)
    err = errno;

  ಬಿಡುಗಡೆ (newpath);
  ಹಿಂತಿರುಗಿ !err;
}
#ಆದರೆ_ಅಂತ್ಯ
