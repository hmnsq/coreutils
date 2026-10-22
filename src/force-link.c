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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "force-link.h"

#অন্তর্ভাব <tempname.h>

/* A basename pattern suitable for a temporary file.  It should work
   even on file systems like FAT that support only short names.
   "Cu" is short for "Coreutils" or for "Changeable unstable",
   take your pick....  */

স্থির অক্ষর ধ্রুবক simple_pattern[] = "CuXXXXXX";
গণনা { x_suffix_len = আকার "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

গণনা { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

স্থির অক্ষর *
samedir_template (অক্ষর ধ্রুবক *dstname, অক্ষর buf[smallsize])
{
  ptrdiff_t dstdirlen = last_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + আকার simple_pattern;
  অক্ষর *dsttmp;
  যদি (dsttmpsize <= smallsize)
    dsttmp = buf;
  অন্যথা
    {
      dsttmp = malloc (dsttmpsize);
      যদি (!dsttmp)
        ফেরত dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_pattern);
  ফেরত dsttmp;
}


/* Auxiliaries for force_linkat.  */

কাঠামো link_arg
{
  পূর্ণসংখ্যা srcdir;
  অক্ষর ধ্রুবক *srcname;
  পূর্ণসংখ্যা dstdir;
  পূর্ণসংখ্যা flags;
};

স্থির পূর্ণসংখ্যা
try_link (অক্ষর *dest, শূন্য *arg)
{
  কাঠামো link_arg *a = arg;
  ফেরত linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
বাহ্যিক পূর্ণসংখ্যা
force_linkat (পূর্ণসংখ্যা srcdir, অক্ষর ধ্রুবক *srcname,
              পূর্ণসংখ্যা dstdir, অক্ষর ধ্রুবক *dstname, পূর্ণসংখ্যা flags, bool force,
              পূর্ণসংখ্যা linkat_errno)
{
  যদি (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  যদি (!force || linkat_errno != EEXIST)
    ফেরত linkat_errno;

  অক্ষর buf[smallsize];
  অক্ষর *dsttmp = samedir_template (dstname, buf);
  যদি (! dsttmp)
    ফেরত errno;
  কাঠামো link_arg arg = { srcdir, srcname, dstdir, flags };
  পূর্ণসংখ্যা err;

  যদি (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = errno;
  অন্যথা
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : errno;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  যদি (dsttmp != buf)
    মুক্ত (dsttmp);
  ফেরত err;
}


/* Auxiliaries for force_symlinkat.  */

কাঠামো symlink_arg
{
  অক্ষর ধ্রুবক *srcname;
  পূর্ণসংখ্যা dstdir;
};

স্থির পূর্ণসংখ্যা
try_symlink (অক্ষর *dest, শূন্য *arg)
{
  কাঠামো symlink_arg *a = arg;
  ফেরত symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
বাহ্যিক পূর্ণসংখ্যা
force_symlinkat (অক্ষর ধ্রুবক *srcname, পূর্ণসংখ্যা dstdir, অক্ষর ধ্রুবক *dstname,
                 bool force, পূর্ণসংখ্যা symlinkat_errno)
{
  যদি (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  যদি (!force || symlinkat_errno != EEXIST)
    ফেরত symlinkat_errno;

  অক্ষর buf[smallsize];
  অক্ষর *dsttmp = samedir_template (dstname, buf);
  যদি (!dsttmp)
    ফেরত errno;
  কাঠামো symlink_arg arg = { srcname, dstdir };
  পূর্ণসংখ্যা err;

  যদি (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = errno;
  অন্যথা যদি (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = errno;
      unlinkat (dstdir, dsttmp, 0);
    }
  অন্যথা
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  যদি (dsttmp != buf)
    মুক্ত (dsttmp);
  ফেরত err;
}
