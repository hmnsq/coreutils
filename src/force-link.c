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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "force-link.h"

#અન્તર્ભાવ <tempname.h>

/* A basename pattern suitable for a temporary file.  It should work
   even on file systems like FAT that support only short names.
   "Cu" is short for "Coreutils" or for "Changeable unstable",
   take your pick....  */

સ્થિર અક્ષર અચલ simple_pattern[] = "CuXXXXXX";
ગણના { x_suffix_len = કદ "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

ગણના { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

સ્થિર અક્ષર *
samedir_template (અક્ષર અચલ *dstname, અક્ષર buf[smallsize])
{
  ptrdiff_t dstdirlen = last_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + કદ simple_pattern;
  અક્ષર *dsttmp;
  જો (dsttmpsize <= smallsize)
    dsttmp = buf;
  નહીંતર
    {
      dsttmp = malloc (dsttmpsize);
      જો (!dsttmp)
        પાછા dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_pattern);
  પાછા dsttmp;
}


/* Auxiliaries for force_linkat.  */

સંરચના link_arg
{
  પૂર્ણાંક srcdir;
  અક્ષર અચલ *srcname;
  પૂર્ણાંક dstdir;
  પૂર્ણાંક flags;
};

સ્થિર પૂર્ણાંક
try_link (અક્ષર *dest, શૂન્ય *arg)
{
  સંરચના link_arg *a = arg;
  પાછા linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
બાહ્ય પૂર્ણાંક
force_linkat (પૂર્ણાંક srcdir, અક્ષર અચલ *srcname,
              પૂર્ણાંક dstdir, અક્ષર અચલ *dstname, પૂર્ણાંક flags, bool force,
              પૂર્ણાંક linkat_errno)
{
  જો (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  જો (!force || linkat_errno != EEXIST)
    પાછા linkat_errno;

  અક્ષર buf[smallsize];
  અક્ષર *dsttmp = samedir_template (dstname, buf);
  જો (! dsttmp)
    પાછા errno;
  સંરચના link_arg arg = { srcdir, srcname, dstdir, flags };
  પૂર્ણાંક err;

  જો (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = errno;
  નહીંતર
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : errno;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  જો (dsttmp != buf)
    મુક્ત (dsttmp);
  પાછા err;
}


/* Auxiliaries for force_symlinkat.  */

સંરચના symlink_arg
{
  અક્ષર અચલ *srcname;
  પૂર્ણાંક dstdir;
};

સ્થિર પૂર્ણાંક
try_symlink (અક્ષર *dest, શૂન્ય *arg)
{
  સંરચના symlink_arg *a = arg;
  પાછા symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
બાહ્ય પૂર્ણાંક
force_symlinkat (અક્ષર અચલ *srcname, પૂર્ણાંક dstdir, અક્ષર અચલ *dstname,
                 bool force, પૂર્ણાંક symlinkat_errno)
{
  જો (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  જો (!force || symlinkat_errno != EEXIST)
    પાછા symlinkat_errno;

  અક્ષર buf[smallsize];
  અક્ષર *dsttmp = samedir_template (dstname, buf);
  જો (!dsttmp)
    પાછા errno;
  સંરચના symlink_arg arg = { srcname, dstdir };
  પૂર્ણાંક err;

  જો (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = errno;
  નહીંતર જો (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = errno;
      unlinkat (dstdir, dsttmp, 0);
    }
  નહીંતર
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  જો (dsttmp != buf)
    મુક્ત (dsttmp);
  પાછા err;
}
