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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "force-link.h"

#ಅನ್ತರ್ಭಾವ <tempname.h>

/* A basename pattern suitable for a temporary file.  It should work
   even on file systems like FAT that support only short names.
   "Cu" is short for "Coreutils" or for "Changeable unstable",
   take your pick....  */

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ simple_ನಮೂನೆ[] = "CuXXXXXX";
ಎಣಿಕೆ { x_suffix_len = ಗಾತ್ರ "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

ಎಣಿಕೆ { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
samedir_template (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dstname, ಅಕ್ಷರ buf[smallsize])
{
  ptrdiff_t dstdirlen = ಕೊನೆಯ_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + ಗಾತ್ರ simple_ನಮೂನೆ;
  ಅಕ್ಷರ *dsttmp;
  ಆದರೆ (dsttmpsize <= smallsize)
    dsttmp = buf;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      dsttmp = ಸ್ಮರಣೆ_ಕೊಡು (dsttmpsize);
      ಆದರೆ (!dsttmp)
        ಹಿಂತಿರುಗಿ dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_ನಮೂನೆ);
  ಹಿಂತಿರುಗಿ dsttmp;
}


/* Auxiliaries for force_linkat.  */

ರಚನೆ link_arg
{
  ಪೂರ್ಣಾಂಕ srcdir;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *srcname;
  ಪೂರ್ಣಾಂಕ dstdir;
  ಪೂರ್ಣಾಂಕ flags;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
try_link (ಅಕ್ಷರ *dest, ಶೂನ್ಯ *arg)
{
  ರಚನೆ link_arg *a = arg;
  ಹಿಂತಿರುಗಿ linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ
force_linkat (ಪೂರ್ಣಾಂಕ srcdir, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *srcname,
              ಪೂರ್ಣಾಂಕ dstdir, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dstname, ಪೂರ್ಣಾಂಕ flags, bool force,
              ಪೂರ್ಣಾಂಕ linkat_errno)
{
  ಆದರೆ (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  ಆದರೆ (!force || linkat_errno != EEXIST)
    ಹಿಂತಿರುಗಿ linkat_errno;

  ಅಕ್ಷರ buf[smallsize];
  ಅಕ್ಷರ *dsttmp = samedir_template (dstname, buf);
  ಆದರೆ (! dsttmp)
    ಹಿಂತಿರುಗಿ errno;
  ರಚನೆ link_arg arg = { srcdir, srcname, dstdir, flags };
  ಪೂರ್ಣಾಂಕ err;

  ಆದರೆ (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = errno;
  ಇಲ್ಲದಿದ್ದರೆ
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : errno;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  ಆದರೆ (dsttmp != buf)
    ಬಿಡುಗಡೆ (dsttmp);
  ಹಿಂತಿರುಗಿ err;
}


/* Auxiliaries for force_symlinkat.  */

ರಚನೆ symlink_arg
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *srcname;
  ಪೂರ್ಣಾಂಕ dstdir;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
try_symlink (ಅಕ್ಷರ *dest, ಶೂನ್ಯ *arg)
{
  ರಚನೆ symlink_arg *a = arg;
  ಹಿಂತಿರುಗಿ symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ
force_symlinkat (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *srcname, ಪೂರ್ಣಾಂಕ dstdir, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dstname,
                 bool force, ಪೂರ್ಣಾಂಕ symlinkat_errno)
{
  ಆದರೆ (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  ಆದರೆ (!force || symlinkat_errno != EEXIST)
    ಹಿಂತಿರುಗಿ symlinkat_errno;

  ಅಕ್ಷರ buf[smallsize];
  ಅಕ್ಷರ *dsttmp = samedir_template (dstname, buf);
  ಆದರೆ (!dsttmp)
    ಹಿಂತಿರುಗಿ errno;
  ರಚನೆ symlink_arg arg = { srcname, dstdir };
  ಪೂರ್ಣಾಂಕ err;

  ಆದರೆ (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = errno;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = errno;
      unlinkat (dstdir, dsttmp, 0);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  ಆದರೆ (dsttmp != buf)
    ಬಿಡುಗಡೆ (dsttmp);
  ಹಿಂತಿರುಗಿ err;
}
