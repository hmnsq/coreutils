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

#అన్తర్భావ <config.h>
#అన్తర్భావ <selinux/label.h>
#అన్తర్భావ <selinux/context.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "canonicalize.h"
#అన్తర్భావ "xfts.h"
#అన్తర్భావ "selinux.h"

#ఒకవేళ HAVE_SELINUX_LABEL_H

# ఒకవేళ ! HAVE_బహుళకం_గమ్యం_SECURITY_CLASS
/*
  This function has been added to libselinux-2.1.12-5, but is here
  for support with older versions of SELinux

  Translates a mode into an Internal SELinux security_class definition.
  Returns 0 on failure, with errno set to EINVAL.
*/
స్థిర security_class_t
బహుళకం_గమ్యం_security_class (mode_t m)
{

  ఒకవేళ (S_ISREG (m))
    తిరిగి string_గమ్యం_security_class ("file");
  ఒకవేళ (S_ISDIR (m))
    తిరిగి string_గమ్యం_security_class ("dir");
  ఒకవేళ (S_ISCHR (m))
    తిరిగి string_గమ్యం_security_class ("chr_file");
  ఒకవేళ (S_ISBLK (m))
    తిరిగి string_గమ్యం_security_class ("blk_file");
  ఒకవేళ (S_ISFIFO (m))
    తిరిగి string_గమ్యం_security_class ("fifo_file");
  ఒకవేళ (S_ISLNK (m))
    తిరిగి string_గమ్యం_security_class ("lnk_file");
  ఒకవేళ (S_ISSOCK (m))
    తిరిగి string_గమ్యం_security_class ("sock_file");

  errno = EINVAL;
  తిరిగి 0;
}
# endif

/*
  This function takes a PATH and a MODE and then asks SELinux what the label
  of the path object would be if the current process label created it.
  It then returns the label.

  Returns -1 on failure.  errno will be set appropriately.
*/

స్థిర పూర్ణాంకం
computecon_raw (అక్షరం స్థిరరాశి *మార్గం, mode_t బహుళకం_2, అక్షరం **con)
{
  అక్షరం *scon_raw = ఖాళీ;
  అక్షరం *tcon_raw = ఖాళీ;
  security_class_t tclass;
  పూర్ణాంకం rc = -1;

  అక్షరం *dir = dir_name (మార్గం);
  ఒకవేళ (!dir)
    వెళ్ళు quit;
  ఒకవేళ (getcon_raw (&scon_raw) < 0)
    వెళ్ళు quit;
  ఒకవేళ (getfilecon_raw (dir, &tcon_raw) < 0)
    వెళ్ళు quit;
  tclass = బహుళకం_గమ్యం_security_class (బహుళకం_2);
  ఒకవేళ (!tclass)
    వెళ్ళు quit;
  rc = security_compute_create_raw (scon_raw, tcon_raw, tclass, con);

 quit:;
  పూర్ణాంకం err = errno;
  విడుదల (dir);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  తిరిగి rc;
}

/*
  This function takes a handle, path and mode, it calls computecon_raw to get
  the label of the path object if the current process created it, then it calls
  selabel_lookup to get the default type for the object.  It substitutes the
  default type into label.  It tells the SELinux Kernel to label all new file
  system objects created by the current process with this label.

  Returns -1 on failure.  errno will be set appropriately.
*/
పూర్ణాంకం
defaultcon (నిర్మాణం selabel_handle *selabel_handle,
            అక్షరం స్థిరరాశి *మార్గం, mode_t బహుళకం_2)
{
  పూర్ణాంకం rc = -1;
  అక్షరం *scon_raw = ఖాళీ;
  అక్షరం *tcon_raw = ఖాళీ;
  context_t scontext = ఖాళీ, tcontext = ఖాళీ;
  అక్షరం స్థిరరాశి *contype;
  అక్షరం స్థిరరాశి *constr;
  అక్షరం *newpath = ఖాళీ;

  ఒకవేళ (! IS_ABSOLUTE_FILE_NAME (మార్గం))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.  */
      newpath = canonicalize_filename_బహుళకం (మార్గం, CAN_MISSING);
      ఒకవేళ (! newpath)
        వెళ్ళు quit;
      మార్గం = newpath;
    }

  ఒకవేళ (selabel_lookup_raw (selabel_handle, &scon_raw, మార్గం, బహుళకం_2) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup().  */
      ఒకవేళ (errno == ENOENT)
        errno = ENODATA;
      వెళ్ళు quit;
    }
  ఒకవేళ (computecon_raw (మార్గం, బహుళకం_2, &tcon_raw) < 0)
    వెళ్ళు quit;
  ఒకవేళ (!(scontext = context_new (scon_raw)))
    వెళ్ళు quit;
  ఒకవేళ (!(tcontext = context_new (tcon_raw)))
    వెళ్ళు quit;

  ఒకవేళ (!(contype = context_type_get (scontext)))
    వెళ్ళు quit;
  ఒకవేళ (context_type_సమితి (tcontext, contype))
    వెళ్ళు quit;
  ఒకవేళ (!(constr = context_str (tcontext)))
    వెళ్ళు quit;

  rc = setfscreatecon_raw (constr);

 quit:;
  పూర్ణాంకం err = errno;
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  విడుదల (newpath);
  errno = err;
  తిరిగి rc;
}

/*
  If SELABEL_HANDLE is null, set PATH's label to the default to the
  local process.  Otherwise use selabel_lookup to determine the
  default label, extract the type field and then modify the file
  system object.  Note only the type field is updated, thus preserving MLS
  levels and user identity etc. of the PATH.

  Returns -1 on failure.  errno will be set appropriately.
*/
స్థిర పూర్ణాంకం
restorecon_వ్యక్తిగత (నిర్మాణం selabel_handle *selabel_handle, అక్షరం స్థిరరాశి *మార్గం)
{
  పూర్ణాంకం rc = -1;
  నిర్మాణం stat sb;
  అక్షరం *scon_raw = ఖాళీ;
  అక్షరం *tcon_raw = ఖాళీ;
  context_t scontext = ఖాళీ, tcontext = ఖాళీ;
  అక్షరం స్థిరరాశి *contype;
  అక్షరం స్థిరరాశి *constr;
  పూర్ణాంకం fd;

  ఒకవేళ (!selabel_handle)
    {
      ఒకవేళ (getfscreatecon_raw (&tcon_raw) < 0)
        తిరిగి rc;
      ఒకవేళ (!tcon_raw)
        {
          errno = ENODATA;
          తిరిగి rc;
        }
      rc = lsetfilecon_raw (మార్గం, tcon_raw);
      పూర్ణాంకం err = errno;
      freecon (tcon_raw);
      errno = err;
      తిరిగి rc;
    }

  fd = open (మార్గం, O_RDONLY | O_NOFOLLOW);
  ఒకవేళ (fd == -1 && (errno != ELOOP))
    వెళ్ళు quit;

  ఒకవేళ (fd != -1)
    {
      ఒకవేళ (fstat (fd, &sb) < 0)
        వెళ్ళు quit;
    }
  లేకపోతే
    {
      ఒకవేళ (lstat (మార్గం, &sb) < 0)
        వెళ్ళు quit;
    }

  ఒకవేళ (selabel_lookup_raw (selabel_handle, &scon_raw, మార్గం, sb.st_బహుళకం) < 0)
    {
      /* "No such file or directory" is a confusing error,
         when processing files, when in fact it was the
         associated default context that was not found.
         Therefore map the error to something more appropriate
         to the context in which we're using selabel_lookup.  */
      ఒకవేళ (errno == ENOENT)
        errno = ENODATA;
      వెళ్ళు quit;
    }
  ఒకవేళ (!(scontext = context_new (scon_raw)))
    వెళ్ళు quit;

  ఒకవేళ (fd != -1)
    {
      ఒకవేళ (fgetfilecon_raw (fd, &tcon_raw) < 0)
        వెళ్ళు quit;
    }
  లేకపోతే
    {
      ఒకవేళ (lgetfilecon_raw (మార్గం, &tcon_raw) < 0)
        వెళ్ళు quit;
    }

  ఒకవేళ (!(tcontext = context_new (tcon_raw)))
    వెళ్ళు quit;

  ఒకవేళ (!(contype = context_type_get (scontext)))
    వెళ్ళు quit;
  ఒకవేళ (context_type_సమితి (tcontext, contype))
    వెళ్ళు quit;
  ఒకవేళ (!(constr = context_str (tcontext)))
    వెళ్ళు quit;

  ఒకవేళ (fd != -1)
    rc = fsetfilecon_raw (fd, constr);
  లేకపోతే
    rc = lsetfilecon_raw (మార్గం, constr);

 quit:;
  పూర్ణాంకం err = errno;
  ఒకవేళ (fd != -1)
    close (fd);
  context_free (scontext);
  context_free (tcontext);
  freecon (scon_raw);
  freecon (tcon_raw);
  errno = err;
  తిరిగి rc;
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
restorecon (నిర్మాణం selabel_handle *selabel_handle,
            అక్షరం స్థిరరాశి *మార్గం, bool recurse)
{
  అక్షరం *newpath = ఖాళీ;

  ఒకవేళ (! IS_ABSOLUTE_FILE_NAME (మార్గం))
    {
      /* Generate absolute name as required by subsequent selabel_lookup.
         When RECURSE, this also generates absolute names in the
         fts entries, which may be quicker to process in any case.  */
      newpath = canonicalize_filename_బహుళకం (మార్గం, CAN_MISSING);
      ఒకవేళ (! newpath)
        తిరిగి false;
      మార్గం = newpath;
    }

  ఒకవేళ (! recurse)
    {
      bool ok = restorecon_వ్యక్తిగత (selabel_handle, మార్గం) != -1;
      పూర్ణాంకం err = errno;
      విడుదల (newpath);
      errno = err;
      తిరిగి ok;
    }

  అక్షరం స్థిరరాశి *ftspath[2] = { మార్గం, ఖాళీ };
  FTS *fts = xfts_open ((అక్షరం *స్థిరరాశి *) ftspath, FTS_PHYSICAL, ఖాళీ);

  పూర్ణాంకం err = 0;
  కొరకు (FTSENT *ent; (ent = fts_read (fts)); )
    ఒకవేళ (restorecon_వ్యక్తిగత (selabel_handle, fts->fts_మార్గం) < 0)
      err = errno;

  ఒకవేళ (errno != 0)
    err = errno;

  ఒకవేళ (fts_close (fts) != 0)
    err = errno;

  విడుదల (newpath);
  తిరిగి !err;
}
#అయితే_ముగింపు
