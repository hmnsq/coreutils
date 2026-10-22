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

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ simple_pattern[] = "CuXXXXXX";
ಗಣನಾ { x_suffix_len = ಪರಿಮಾಣ "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

ಗಣನಾ { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

ಸ್ಥಿರ ವರ್ಣ *
samedir_template (ವರ್ಣ ಧ್ರುವ *dstname, ವರ್ಣ buf[smallsize])
{
  ptrdiff_t dstdirlen = last_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + ಪರಿಮಾಣ simple_pattern;
  ವರ್ಣ *dsttmp;
  ಯದಿ (dsttmpsize <= smallsize)
    dsttmp = buf;
  ಅನ್ಯಥಾ
    {
      dsttmp = malloc (dsttmpsize);
      ಯದಿ (!dsttmp)
        ಪ್ರತ್ಯಾಗಮ dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_pattern);
  ಪ್ರತ್ಯಾಗಮ dsttmp;
}


/* Auxiliaries for force_linkat.  */

ಸಂರಚನಾ link_arg
{
  ಪೂರ್ಣಾಙ್ಕ srcdir;
  ವರ್ಣ ಧ್ರುವ *srcname;
  ಪೂರ್ಣಾಙ್ಕ dstdir;
  ಪೂರ್ಣಾಙ್ಕ flags;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
try_link (ವರ್ಣ *dest, ಶೂನ್ಯ *arg)
{
  ಸಂರಚನಾ link_arg *a = arg;
  ಪ್ರತ್ಯಾಗಮ linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ
force_linkat (ಪೂರ್ಣಾಙ್ಕ srcdir, ವರ್ಣ ಧ್ರುವ *srcname,
              ಪೂರ್ಣಾಙ್ಕ dstdir, ವರ್ಣ ಧ್ರುವ *dstname, ಪೂರ್ಣಾಙ್ಕ flags, bool force,
              ಪೂರ್ಣಾಙ್ಕ linkat_errno)
{
  ಯದಿ (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  ಯದಿ (!force || linkat_errno != EEXIST)
    ಪ್ರತ್ಯಾಗಮ linkat_errno;

  ವರ್ಣ buf[smallsize];
  ವರ್ಣ *dsttmp = samedir_template (dstname, buf);
  ಯದಿ (! dsttmp)
    ಪ್ರತ್ಯಾಗಮ errno;
  ಸಂರಚನಾ link_arg arg = { srcdir, srcname, dstdir, flags };
  ಪೂರ್ಣಾಙ್ಕ err;

  ಯದಿ (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = errno;
  ಅನ್ಯಥಾ
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : errno;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  ಯದಿ (dsttmp != buf)
    ಮುಕ್ತ (dsttmp);
  ಪ್ರತ್ಯಾಗಮ err;
}


/* Auxiliaries for force_symlinkat.  */

ಸಂರಚನಾ symlink_arg
{
  ವರ್ಣ ಧ್ರುವ *srcname;
  ಪೂರ್ಣಾಙ್ಕ dstdir;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
try_symlink (ವರ್ಣ *dest, ಶೂನ್ಯ *arg)
{
  ಸಂರಚನಾ symlink_arg *a = arg;
  ಪ್ರತ್ಯಾಗಮ symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
ಬಾಹ್ಯ ಪೂರ್ಣಾಙ್ಕ
force_symlinkat (ವರ್ಣ ಧ್ರುವ *srcname, ಪೂರ್ಣಾಙ್ಕ dstdir, ವರ್ಣ ಧ್ರುವ *dstname,
                 bool force, ಪೂರ್ಣಾಙ್ಕ symlinkat_errno)
{
  ಯದಿ (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  ಯದಿ (!force || symlinkat_errno != EEXIST)
    ಪ್ರತ್ಯಾಗಮ symlinkat_errno;

  ವರ್ಣ buf[smallsize];
  ವರ್ಣ *dsttmp = samedir_template (dstname, buf);
  ಯದಿ (!dsttmp)
    ಪ್ರತ್ಯಾಗಮ errno;
  ಸಂರಚನಾ symlink_arg arg = { srcname, dstdir };
  ಪೂರ್ಣಾಙ್ಕ err;

  ಯದಿ (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = errno;
  ಅನ್ಯಥಾ ಯದಿ (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = errno;
      unlinkat (dstdir, dsttmp, 0);
    }
  ಅನ್ಯಥಾ
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  ಯದಿ (dsttmp != buf)
    ಮುಕ್ತ (dsttmp);
  ಪ್ರತ್ಯಾಗಮ err;
}
