/* Implement ln -f "atomically"

   Copyright 2017-2026 Free Software Foundation, Inc.

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

/* Written by Paul Eggert.  */

/* A naive "ln -f A B" unlinks B and then links A to B.  This module
   instead links A to a randomly-named temporary T in B's directory,
   and then renames T to B.  This approach has a window with a
   randomly-named temporary, which is safer for many applications than
   a window where B does not exist.  */

#అన్తర్భావ <config.h>
#అన్తర్భావ "system.h"

#అన్తర్భావ "force-link.h"

#అన్తర్భావ <tempname.h>

/* A basename pattern suitable for a temporary file.  It should work
   even on file systems like FAT that support only short names.
   "Cu" is short for "Coreutils" or for "Changeable unstable",
   take your pick....  */

స్థిర అక్షరం స్థిరరాశి simple_నమూనా[] = "CuXXXXXX";
గణన { x_suffix_len = పరిమాణం "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

గణన { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

స్థిర అక్షరం *
samedir_template (అక్షరం స్థిరరాశి *dstname, అక్షరం buf[smallsize])
{
  ptrdiff_t dstdirlen = చివరి_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + పరిమాణం simple_నమూనా;
  అక్షరం *dsttmp;
  ఒకవేళ (dsttmpsize <= smallsize)
    dsttmp = buf;
  లేకపోతే
    {
      dsttmp = జ్ఞాపకం_ఇవ్వు (dsttmpsize);
      ఒకవేళ (!dsttmp)
        తిరిగి dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_నమూనా);
  తిరిగి dsttmp;
}


/* Auxiliaries for force_linkat.  */

నిర్మాణం link_arg
{
  పూర్ణాంకం srcdir;
  అక్షరం స్థిరరాశి *srcname;
  పూర్ణాంకం dstdir;
  పూర్ణాంకం flags;
};

స్థిర పూర్ణాంకం
try_link (అక్షరం *dest, శూన్య *arg)
{
  నిర్మాణం link_arg *a = arg;
  తిరిగి linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
బాహ్య పూర్ణాంకం
force_linkat (పూర్ణాంకం srcdir, అక్షరం స్థిరరాశి *srcname,
              పూర్ణాంకం dstdir, అక్షరం స్థిరరాశి *dstname, పూర్ణాంకం flags, bool force,
              పూర్ణాంకం linkat_errno)
{
  ఒకవేళ (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  ఒకవేళ (!force || linkat_errno != EEXIST)
    తిరిగి linkat_errno;

  అక్షరం buf[smallsize];
  అక్షరం *dsttmp = samedir_template (dstname, buf);
  ఒకవేళ (! dsttmp)
    తిరిగి errno;
  నిర్మాణం link_arg arg = { srcdir, srcname, dstdir, flags };
  పూర్ణాంకం err;

  ఒకవేళ (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = errno;
  లేకపోతే
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : errno;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  ఒకవేళ (dsttmp != buf)
    విడుదల (dsttmp);
  తిరిగి err;
}


/* Auxiliaries for force_symlinkat.  */

నిర్మాణం symlink_arg
{
  అక్షరం స్థిరరాశి *srcname;
  పూర్ణాంకం dstdir;
};

స్థిర పూర్ణాంకం
try_symlink (అక్షరం *dest, శూన్య *arg)
{
  నిర్మాణం symlink_arg *a = arg;
  తిరిగి symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
బాహ్య పూర్ణాంకం
force_symlinkat (అక్షరం స్థిరరాశి *srcname, పూర్ణాంకం dstdir, అక్షరం స్థిరరాశి *dstname,
                 bool force, పూర్ణాంకం symlinkat_errno)
{
  ఒకవేళ (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  ఒకవేళ (!force || symlinkat_errno != EEXIST)
    తిరిగి symlinkat_errno;

  అక్షరం buf[smallsize];
  అక్షరం *dsttmp = samedir_template (dstname, buf);
  ఒకవేళ (!dsttmp)
    తిరిగి errno;
  నిర్మాణం symlink_arg arg = { srcname, dstdir };
  పూర్ణాంకం err;

  ఒకవేళ (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = errno;
  లేకపోతే ఒకవేళ (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = errno;
      unlinkat (dstdir, dsttmp, 0);
    }
  లేకపోతే
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  ఒకవేళ (dsttmp != buf)
    విడుదల (dsttmp);
  తిరిగి err;
}
