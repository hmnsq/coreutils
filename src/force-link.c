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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "force-link.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <tempname.h>

/* A basename pattern suitable for a temporary file.  It should work
   even on file systems like FAT that support only short names.
   "Cu" is short for "Coreutils" or for "Changeable unstable",
   take your pick....  */

ਸਥਿਰ ਅੱਖਰ ਅਚਲ simple_ਨਮੂਨਾ[] = "CuXXXXXX";
ਗਿਣਤੀ { x_suffix_len = ਆਕਾਰ "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

ਗਿਣਤੀ { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

ਸਥਿਰ ਅੱਖਰ *
samedir_template (ਅੱਖਰ ਅਚਲ *dstname, ਅੱਖਰ buf[smallsize])
{
  ptrdiff_t dstdirlen = ਆਖਰੀ_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + ਆਕਾਰ simple_ਨਮੂਨਾ;
  ਅੱਖਰ *dsttmp;
  ਜੇ (dsttmpsize <= smallsize)
    dsttmp = buf;
  ਨਹੀਂਤਾਂ
    {
      dsttmp = ਸਮ੍ਰਿਤੀ_ਦਿਓ (dsttmpsize);
      ਜੇ (!dsttmp)
        ਵਾਪਸ dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_ਨਮੂਨਾ);
  ਵਾਪਸ dsttmp;
}


/* Auxiliaries for force_linkat.  */

ਬਣਤਰ link_arg
{
  ਪੂਰਨ srcdir;
  ਅੱਖਰ ਅਚਲ *srcname;
  ਪੂਰਨ dstdir;
  ਪੂਰਨ flags;
};

ਸਥਿਰ ਪੂਰਨ
try_link (ਅੱਖਰ *dest, ਖਾਲੀ *arg)
{
  ਬਣਤਰ link_arg *a = arg;
  ਵਾਪਸ linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
ਬਾਹਰੀ ਪੂਰਨ
force_linkat (ਪੂਰਨ srcdir, ਅੱਖਰ ਅਚਲ *srcname,
              ਪੂਰਨ dstdir, ਅੱਖਰ ਅਚਲ *dstname, ਪੂਰਨ flags, bool force,
              ਪੂਰਨ linkat_errno)
{
  ਜੇ (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  ਜੇ (!force || linkat_errno != EEXIST)
    ਵਾਪਸ linkat_errno;

  ਅੱਖਰ buf[smallsize];
  ਅੱਖਰ *dsttmp = samedir_template (dstname, buf);
  ਜੇ (! dsttmp)
    ਵਾਪਸ errno;
  ਬਣਤਰ link_arg arg = { srcdir, srcname, dstdir, flags };
  ਪੂਰਨ err;

  ਜੇ (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = errno;
  ਨਹੀਂਤਾਂ
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : errno;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  ਜੇ (dsttmp != buf)
    ਮੁਕਤ (dsttmp);
  ਵਾਪਸ err;
}


/* Auxiliaries for force_symlinkat.  */

ਬਣਤਰ symlink_arg
{
  ਅੱਖਰ ਅਚਲ *srcname;
  ਪੂਰਨ dstdir;
};

ਸਥਿਰ ਪੂਰਨ
try_symlink (ਅੱਖਰ *dest, ਖਾਲੀ *arg)
{
  ਬਣਤਰ symlink_arg *a = arg;
  ਵਾਪਸ symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
ਬਾਹਰੀ ਪੂਰਨ
force_symlinkat (ਅੱਖਰ ਅਚਲ *srcname, ਪੂਰਨ dstdir, ਅੱਖਰ ਅਚਲ *dstname,
                 bool force, ਪੂਰਨ symlinkat_errno)
{
  ਜੇ (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  ਜੇ (!force || symlinkat_errno != EEXIST)
    ਵਾਪਸ symlinkat_errno;

  ਅੱਖਰ buf[smallsize];
  ਅੱਖਰ *dsttmp = samedir_template (dstname, buf);
  ਜੇ (!dsttmp)
    ਵਾਪਸ errno;
  ਬਣਤਰ symlink_arg arg = { srcname, dstdir };
  ਪੂਰਨ err;

  ਜੇ (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = errno;
  ਨਹੀਂਤਾਂ ਜੇ (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = errno;
      unlinkat (dstdir, dsttmp, 0);
    }
  ਨਹੀਂਤਾਂ
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  ਜੇ (dsttmp != buf)
    ਮੁਕਤ (dsttmp);
  ਵਾਪਸ err;
}
