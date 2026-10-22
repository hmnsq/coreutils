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

#include <config.h>
#include "system.h"

#include "force-link.h"

#include <tempname.h>

/* A basename pattern suitable for a temporary file.  It should work
   even on file systems like FAT that support only short names.
   "Cu" is short for "Coreutils" or for "Changeable unstable",
   take your pick....  */

static var_na const simple_pattern[] = "CuXXXXXX";
ga_nanaa { x_suffix_len = parimaa_na "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

ga_nanaa { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

static var_na *
samedir_template (var_na const *dstname, var_na buf[smallsize])
{
  ptrdiff_t dstdirlen = last_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + parimaa_na simple_pattern;
  var_na *dsttmp;
  གལ་ཏེ (dsttmpsize <= smallsize)
    dsttmp = buf;
  དེ་མིན
    {
      dsttmp = malloc (dsttmpsize);
      གལ་ཏེ (!dsttmp)
        ཕྱིར dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_pattern);
  ཕྱིར dsttmp;
}


/* Auxiliaries for force_linkat.  */

sa_nracanaa link_arg
{
  ཧྲིལ srcdir;
  var_na const *srcname;
  ཧྲིལ dstdir;
  ཧྲིལ flags;
};

static ཧྲིལ
try_link (var_na *dest, void *arg)
{
  sa_nracanaa link_arg *a = arg;
  ཕྱིར linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
extern ཧྲིལ
force_linkat (ཧྲིལ srcdir, var_na const *srcname,
              ཧྲིལ dstdir, var_na const *dstname, ཧྲིལ flags, bool force,
              ཧྲིལ linkat_errno)
{
  གལ་ཏེ (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  གལ་ཏེ (!force || linkat_errno != EEXIST)
    ཕྱིར linkat_errno;

  var_na buf[smallsize];
  var_na *dsttmp = samedir_template (dstname, buf);
  གལ་ཏེ (! dsttmp)
    ཕྱིར errno;
  sa_nracanaa link_arg arg = { srcdir, srcname, dstdir, flags };
  ཧྲིལ err;

  གལ་ཏེ (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = errno;
  དེ་མིན
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : errno;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  གལ་ཏེ (dsttmp != buf)
    free (dsttmp);
  ཕྱིར err;
}


/* Auxiliaries for force_symlinkat.  */

sa_nracanaa symlink_arg
{
  var_na const *srcname;
  ཧྲིལ dstdir;
};

static ཧྲིལ
try_symlink (var_na *dest, void *arg)
{
  sa_nracanaa symlink_arg *a = arg;
  ཕྱིར symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
extern ཧྲིལ
force_symlinkat (var_na const *srcname, ཧྲིལ dstdir, var_na const *dstname,
                 bool force, ཧྲིལ symlinkat_errno)
{
  གལ་ཏེ (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  གལ་ཏེ (!force || symlinkat_errno != EEXIST)
    ཕྱིར symlinkat_errno;

  var_na buf[smallsize];
  var_na *dsttmp = samedir_template (dstname, buf);
  གལ་ཏེ (!dsttmp)
    ཕྱིར errno;
  sa_nracanaa symlink_arg arg = { srcname, dstdir };
  ཧྲིལ err;

  གལ་ཏེ (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = errno;
  དེ་མིན གལ་ཏེ (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = errno;
      unlinkat (dstdir, dsttmp, 0);
    }
  དེ་མིན
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  གལ་ཏེ (dsttmp != buf)
    free (dsttmp);
  ཕྱིར err;
}
