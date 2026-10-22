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

static char const simple_pattern[] = "CuXXXXXX";
enum { x_suffix_len = sizeof "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

enum { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

static char *
samedir_template (char const *dstname, char buf[smallsize])
{
  ptrdiff_t dstdirlen = last_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + sizeof simple_pattern;
  char *dsttmp;
  만약 (dsttmpsize <= smallsize)
    dsttmp = buf;
  아니면
    {
      dsttmp = malloc (dsttmpsize);
      만약 (!dsttmp)
        반환 dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_pattern);
  반환 dsttmp;
}


/* Auxiliaries for force_linkat.  */

struct link_arg
{
  정수 srcdir;
  char const *srcname;
  정수 dstdir;
  정수 flags;
};

static 정수
try_link (char *dest, void *arg)
{
  struct link_arg *a = arg;
  반환 linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
extern 정수
force_linkat (정수 srcdir, char const *srcname,
              정수 dstdir, char const *dstname, 정수 flags, bool force,
              정수 linkat_errno)
{
  만약 (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  만약 (!force || linkat_errno != EEXIST)
    반환 linkat_errno;

  char buf[smallsize];
  char *dsttmp = samedir_template (dstname, buf);
  만약 (! dsttmp)
    반환 errno;
  struct link_arg arg = { srcdir, srcname, dstdir, flags };
  정수 err;

  만약 (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = errno;
  아니면
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : errno;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  만약 (dsttmp != buf)
    free (dsttmp);
  반환 err;
}


/* Auxiliaries for force_symlinkat.  */

struct symlink_arg
{
  char const *srcname;
  정수 dstdir;
};

static 정수
try_symlink (char *dest, void *arg)
{
  struct symlink_arg *a = arg;
  반환 symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
extern 정수
force_symlinkat (char const *srcname, 정수 dstdir, char const *dstname,
                 bool force, 정수 symlinkat_errno)
{
  만약 (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  만약 (!force || symlinkat_errno != EEXIST)
    반환 symlinkat_errno;

  char buf[smallsize];
  char *dsttmp = samedir_template (dstname, buf);
  만약 (!dsttmp)
    반환 errno;
  struct symlink_arg arg = { srcname, dstdir };
  정수 err;

  만약 (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = errno;
  아니면 만약 (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = errno;
      unlinkat (dstdir, dsttmp, 0);
    }
  아니면
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  만약 (dsttmp != buf)
    free (dsttmp);
  반환 err;
}
