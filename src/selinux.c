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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <selinux/label.h>
#ଅନ୍ତର୍ଭାଵ <selinux/context.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "canonicalize.h"
#ଅନ୍ତର୍ଭାଵ "xfts.h"
#ଅନ୍ତର୍ଭାଵ "selinux.h"

#ଯଦି HAVE_SELINUX_LABEL_H

# ଯଦି ! HAVE_MODE_TO_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
ସ୍ଥିର security_class_t
mode_to_security_class (mode_t m)
{

  ଯଦି (S_ISREG (m))
    ଫେରନ୍ତୁ string_to_security_class ("file");
  ଯଦି (S_ISDIR (m))
    ଫେରନ୍ତୁ string_to_security_class ("dir");
  ଯଦି (S_ISCHR (m))
    ଫେରନ୍ତୁ string_to_security_class ("chr_file");
  ଯଦି (S_ISBLK (m))
    ଫେରନ୍ତୁ string_to_security_class ("blk_file");
  ଯଦି (S_ISFIFO (m))
    ଫେରନ୍ତୁ string_to_security_class ("fifo_file");
  ଯଦି (S_ISLNK (m))
    ଫେରନ୍ତୁ string_to_security_class ("lnk_file");
  ଯଦି (S_ISSOCK (m))
    ଫେରନ୍ତୁ string_to_security_class ("sock_file");

  errno = EINVAL;
  ଫେରନ୍ତୁ 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
computecon_raw (ଅକ୍ଷର ଅଚଳ *path, mode_t mode, ଅକ୍ଷର **con)
{
  ଅକ୍ଷର *scon_raw = ଖାଲି;
  ଅକ୍ଷର *tcon_raw = ଖାଲି;
  security_class_t tclass;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc = -1;

  ଅକ୍ଷର *dir = dir_name (path);
  ଯଦି (!dir)
    ଯାଅ quit;
  ଯଦି (getcon_raw (&scon_raw) < 0)
    ଯାଅ quit;
  ଯଦି (getfilecon_raw (dir, &tcon_raw) < 0)
    ଯାଅ quit;
  tclass = mode_to_security_class (mode);
  ଯଦି (!tclass)
    ଯାଅ quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = errno;
  ମୁକ୍ତ (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ଫେରନ୍ତୁ rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
ପୂର୍ଣ୍ଣସଂଖ୍ୟା
defaultcon (ସଂରଚନା selabel_handle *selabel_handle,
            ଅକ୍ଷର ଅଚଳ *path, mode_t mode)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc = -1;
  ଅକ୍ଷର *scon_raw = ଖାଲି;
  ଅକ୍ଷର *tcon_raw = ଖାଲି;
  context_t scontext = ଖାଲି, tcontext = ଖାଲି;
  ଅକ୍ଷର ଅଚଳ *contype;
  ଅକ୍ଷର ଅଚଳ *constr;
  ଅକ୍ଷର *newpath = ଖାଲି;

  ଯଦି (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      ଯଦି (! newpath)
        ଯାଅ quit;
      path = newpath;
    }

  ଯଦି (selabel_lookup_raw (selabel_handle, &scon_raw, path, mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      ଯଦି (errno == ENOENT)
        errno = ENODATA;
      ଯାଅ quit;
    }
  ଯଦି (computecon_raw (path, mode, &tcon_raw) < 0)
    ଯାଅ quit;
  ଯଦି (!(scontext = context_new (scon_raw)))
    ଯାଅ quit;
  ଯଦି (!(tcontext = context_new (tcon_raw)))
    ଯାଅ quit;

  ଯଦି (!(contype = context_type_get (scontext)))
    ଯାଅ quit;
  ଯଦି (context_type_set (tcontext, contype))
    ଯାଅ quit;
  ଯଦି (!(constr = context_str (tcontext)))
    ଯାଅ quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  ମୁକ୍ତ (newpath);
  errno = err;
  ଫେରନ୍ତୁ rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
restorecon_private (ସଂରଚନା selabel_handle *selabel_handle, ଅକ୍ଷର ଅଚଳ *path)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rc = -1;
  ସଂରଚନା stat sb;
  ଅକ୍ଷର *scon_raw = ଖାଲି;
  ଅକ୍ଷର *tcon_raw = ଖାଲି;
  context_t scontext = ଖାଲି, tcontext = ଖାଲି;
  ଅକ୍ଷର ଅଚଳ *contype;
  ଅକ୍ଷର ଅଚଳ *constr;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;

  ଯଦି (!selabel_handle)
    {
      ଯଦି (getfscreatecon_raw (&tcon_raw) < 0)
        ଫେରନ୍ତୁ rc;
      ଯଦି (!tcon_raw)
        {
          errno = ENODATA;
          ଫେରନ୍ତୁ rc;
        }
      rc = lsetfilecon_raw (path, tcon_raw);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = errno;
      freecon (tcon_raw);
      errno = err;
      ଫେରନ୍ତୁ rc;
    }

  fd = open (path, O_RDONLY | O_NOFOLLOW);
  ଯଦି (fd == -1 && (errno != ELOOP))
    ଯାଅ quit;

  ଯଦି (fd != -1)
    {
      ଯଦି (fstat (fd, &sb) < 0)
        ଯାଅ quit;
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (lstat (path, &sb) < 0)
        ଯାଅ quit;
    }

  ଯଦି (selabel_lookup_raw (selabel_handle, &scon_raw, path, sb.st_mode) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      ଯଦି (errno == ENOENT)
        errno = ENODATA;
      ଯାଅ quit;
    }
  ଯଦି (!(scontext = context_new (scon_raw)))
    ଯାଅ quit;

  ଯଦି (fd != -1)
    {
      ଯଦି (fgetfilecon_raw (fd, &tcon_raw) < 0)
        ଯାଅ quit;
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (lgetfilecon_raw (path, &tcon_raw) < 0)
        ଯାଅ quit;
    }

  ଯଦି (!(tcontext = context_new (tcon_raw)))
    ଯାଅ quit;

  ଯଦି (!(contype = context_type_get (scontext)))
    ଯାଅ quit;
  ଯଦି (context_type_set (tcontext, contype))
    ଯାଅ quit;
  ଯଦି (!(constr = context_str (tcontext)))
    ଯାଅ quit;

  ଯଦି (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  ଅନ୍ୟଥା
    rc = lsetfilecon_raw (path, constr);

 quit:;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = errno;
  ଯଦି (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  ଫେରନ୍ତୁ rc;
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
restorecon (ସଂରଚନା selabel_handle *selabel_handle,
            ଅକ୍ଷର ଅଚଳ *path, bool recurse)
{
  ଅକ୍ଷର *newpath = ଖାଲି;

  ଯଦି (! IS_ABSOLUTE_FILE_NAME (path))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_mode (path, CAN_MISSING);
      ଯଦି (! newpath)
        ଫେରନ୍ତୁ false;
      path = newpath;
    }

  ଯଦି (! recurse)
    {
      bool ok = restorecon_private (selabel_handle, path) != -1;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = errno;
      ମୁକ୍ତ (newpath);
      errno = err;
      ଫେରନ୍ତୁ ok;
    }

  ଅକ୍ଷର ଅଚଳ *ftspath[2] = { path, ଖାଲି };
  FTS *fts = xfts_open ((ଅକ୍ଷର *ଅଚଳ *) ftspath, FTS_PHYSICAL, ଖାଲି);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err = 0;
  ପାଇଁ (FTSENT *ent; (ent = fts_read (fts)); )
    ଯଦି (restorecon_private (selabel_handle, fts->fts_path) < 0)
      err = errno;

  ଯଦି (errno != 0)
    err = errno;

  ଯଦି (fts_close (fts) != 0)
    err = errno;

  ମୁକ୍ତ (newpath);
  ଫେରନ୍ତୁ !err;
}
#endif
