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

স্থির বর্ণ ধ্রুব simple_pattern[] = "CuXXXXXX";
গণনা { x_suffix_len = পরিমাণ "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

গণনা { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

স্থির বর্ণ *
samedir_template (বর্ণ ধ্রুব *dstname, বর্ণ buf[smallsize])
{
  ptrdiff_t dstdirlen = last_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + পরিমাণ simple_pattern;
  বর্ণ *dsttmp;
  যদি (dsttmpsize <= smallsize)
    dsttmp = buf;
  অন্যথা
    {
      dsttmp = malloc (dsttmpsize);
      যদি (!dsttmp)
        প্রত্যাগম dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_pattern);
  প্রত্যাগম dsttmp;
}


/* Auxiliaries for force_linkat.  */

সংরচনা link_arg
{
  পূর্ণাঙ্ক srcdir;
  বর্ণ ধ্রুব *srcname;
  পূর্ণাঙ্ক dstdir;
  পূর্ণাঙ্ক flags;
};

স্থির পূর্ণাঙ্ক
try_link (বর্ণ *dest, শূন্য *arg)
{
  সংরচনা link_arg *a = arg;
  প্রত্যাগম linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
বাহ্য পূর্ণাঙ্ক
force_linkat (পূর্ণাঙ্ক srcdir, বর্ণ ধ্রুব *srcname,
              পূর্ণাঙ্ক dstdir, বর্ণ ধ্রুব *dstname, পূর্ণাঙ্ক flags, bool force,
              পূর্ণাঙ্ক linkat_errno)
{
  যদি (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  যদি (!force || linkat_errno != EEXIST)
    প্রত্যাগম linkat_errno;

  বর্ণ buf[smallsize];
  বর্ণ *dsttmp = samedir_template (dstname, buf);
  যদি (! dsttmp)
    প্রত্যাগম errno;
  সংরচনা link_arg arg = { srcdir, srcname, dstdir, flags };
  পূর্ণাঙ্ক err;

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
  প্রত্যাগম err;
}


/* Auxiliaries for force_symlinkat.  */

সংরচনা symlink_arg
{
  বর্ণ ধ্রুব *srcname;
  পূর্ণাঙ্ক dstdir;
};

স্থির পূর্ণাঙ্ক
try_symlink (বর্ণ *dest, শূন্য *arg)
{
  সংরচনা symlink_arg *a = arg;
  প্রত্যাগম symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
বাহ্য পূর্ণাঙ্ক
force_symlinkat (বর্ণ ধ্রুব *srcname, পূর্ণাঙ্ক dstdir, বর্ণ ধ্রুব *dstname,
                 bool force, পূর্ণাঙ্ক symlinkat_errno)
{
  যদি (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  যদি (!force || symlinkat_errno != EEXIST)
    প্রত্যাগম symlinkat_errno;

  বর্ণ buf[smallsize];
  বর্ণ *dsttmp = samedir_template (dstname, buf);
  যদি (!dsttmp)
    প্রত্যাগম errno;
  সংরচনা symlink_arg arg = { srcname, dstdir };
  পূর্ণাঙ্ক err;

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
  প্রত্যাগম err;
}
