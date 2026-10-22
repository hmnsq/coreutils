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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "force-link.h"

#अन्तर्भाव <tempname.h>

/* A basename pattern suitable for a temporary file.  It should work
   even on file systems like FAT that support only short names.
   "Cu" is short for "Coreutils" or for "Changeable unstable",
   take your pick....  */

स्थिर वर्ण ध्रुव simple_pattern[] = "CuXXXXXX";
गणना { x_suffix_len = परिमाण "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

गणना { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

स्थिर वर्ण *
samedir_template (वर्ण ध्रुव *dstname, वर्ण buf[smallsize])
{
  ptrdiff_t dstdirlen = last_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + परिमाण simple_pattern;
  वर्ण *dsttmp;
  जइ (dsttmpsize <= smallsize)
    dsttmp = buf;
  अण्णहा
    {
      dsttmp = malloc (dsttmpsize);
      जइ (!dsttmp)
        प्रत्यागम dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_pattern);
  प्रत्यागम dsttmp;
}


/* Auxiliaries for force_linkat.  */

संरचना link_arg
{
  पूर्णाङ्क srcdir;
  वर्ण ध्रुव *srcname;
  पूर्णाङ्क dstdir;
  पूर्णाङ्क flags;
};

स्थिर पूर्णाङ्क
try_link (वर्ण *dest, शून्य *arg)
{
  संरचना link_arg *a = arg;
  प्रत्यागम linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
बाह्य पूर्णाङ्क
force_linkat (पूर्णाङ्क srcdir, वर्ण ध्रुव *srcname,
              पूर्णाङ्क dstdir, वर्ण ध्रुव *dstname, पूर्णाङ्क flags, bool force,
              पूर्णाङ्क linkat_errno)
{
  जइ (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  जइ (!force || linkat_errno != EEXIST)
    प्रत्यागम linkat_errno;

  वर्ण buf[smallsize];
  वर्ण *dsttmp = samedir_template (dstname, buf);
  जइ (! dsttmp)
    प्रत्यागम errno;
  संरचना link_arg arg = { srcdir, srcname, dstdir, flags };
  पूर्णाङ्क err;

  जइ (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = errno;
  अण्णहा
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : errno;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  जइ (dsttmp != buf)
    मुक्त (dsttmp);
  प्रत्यागम err;
}


/* Auxiliaries for force_symlinkat.  */

संरचना symlink_arg
{
  वर्ण ध्रुव *srcname;
  पूर्णाङ्क dstdir;
};

स्थिर पूर्णाङ्क
try_symlink (वर्ण *dest, शून्य *arg)
{
  संरचना symlink_arg *a = arg;
  प्रत्यागम symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
बाह्य पूर्णाङ्क
force_symlinkat (वर्ण ध्रुव *srcname, पूर्णाङ्क dstdir, वर्ण ध्रुव *dstname,
                 bool force, पूर्णाङ्क symlinkat_errno)
{
  जइ (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  जइ (!force || symlinkat_errno != EEXIST)
    प्रत्यागम symlinkat_errno;

  वर्ण buf[smallsize];
  वर्ण *dsttmp = samedir_template (dstname, buf);
  जइ (!dsttmp)
    प्रत्यागम errno;
  संरचना symlink_arg arg = { srcname, dstdir };
  पूर्णाङ्क err;

  जइ (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = errno;
  अण्णहा जइ (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = errno;
      unlinkat (dstdir, dsttmp, 0);
    }
  अण्णहा
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  जइ (dsttmp != buf)
    मुक्त (dsttmp);
  प्रत्यागम err;
}
