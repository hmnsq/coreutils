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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "force-link.h"

#அந்தர்பாவ <tempname.h>

/* A basename pattern suitable for a temporary file.  It should work
   even on file systems like FAT that support only short names.
   "Cu" is short for "Coreutils" or for "Changeable unstable",
   take your pick....  */

நிலையான எழுத்து மாறிலி simple_pattern[] = "CuXXXXXX";
எண்ணிக்கை { x_suffix_len = அளவு "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

எண்ணிக்கை { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

நிலையான எழுத்து *
samedir_template (எழுத்து மாறிலி *dstname, எழுத்து buf[smallsize])
{
  ptrdiff_t dstdirlen = last_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + அளவு simple_pattern;
  எழுத்து *dsttmp;
  எனில் (dsttmpsize <= smallsize)
    dsttmp = buf;
  இல்லையெனில்
    {
      dsttmp = malloc (dsttmpsize);
      எனில் (!dsttmp)
        திரும்பு dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_pattern);
  திரும்பு dsttmp;
}


/* Auxiliaries for force_linkat.  */

கட்டமைப்பு link_arg
{
  முழு srcdir;
  எழுத்து மாறிலி *srcname;
  முழு dstdir;
  முழு flags;
};

நிலையான முழு
try_link (எழுத்து *dest, வெறுமை *arg)
{
  கட்டமைப்பு link_arg *a = arg;
  திரும்பு linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
வெளி முழு
force_linkat (முழு srcdir, எழுத்து மாறிலி *srcname,
              முழு dstdir, எழுத்து மாறிலி *dstname, முழு flags, bool force,
              முழு linkat_errno)
{
  எனில் (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  எனில் (!force || linkat_errno != EEXIST)
    திரும்பு linkat_errno;

  எழுத்து buf[smallsize];
  எழுத்து *dsttmp = samedir_template (dstname, buf);
  எனில் (! dsttmp)
    திரும்பு errno;
  கட்டமைப்பு link_arg arg = { srcdir, srcname, dstdir, flags };
  முழு err;

  எனில் (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = errno;
  இல்லையெனில்
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : errno;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  எனில் (dsttmp != buf)
    விடுவி (dsttmp);
  திரும்பு err;
}


/* Auxiliaries for force_symlinkat.  */

கட்டமைப்பு symlink_arg
{
  எழுத்து மாறிலி *srcname;
  முழு dstdir;
};

நிலையான முழு
try_symlink (எழுத்து *dest, வெறுமை *arg)
{
  கட்டமைப்பு symlink_arg *a = arg;
  திரும்பு symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
வெளி முழு
force_symlinkat (எழுத்து மாறிலி *srcname, முழு dstdir, எழுத்து மாறிலி *dstname,
                 bool force, முழு symlinkat_errno)
{
  எனில் (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  எனில் (!force || symlinkat_errno != EEXIST)
    திரும்பு symlinkat_errno;

  எழுத்து buf[smallsize];
  எழுத்து *dsttmp = samedir_template (dstname, buf);
  எனில் (!dsttmp)
    திரும்பு errno;
  கட்டமைப்பு symlink_arg arg = { srcname, dstdir };
  முழு err;

  எனில் (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = errno;
  இல்லையெனில் எனில் (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = errno;
      unlinkat (dstdir, dsttmp, 0);
    }
  இல்லையெனில்
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  எனில் (dsttmp != buf)
    விடுவி (dsttmp);
  திரும்பு err;
}
