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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "force-link.h"

#അന്തര്ഭാവ <tempname.h>

/* A basename pattern suitable for a temporary file.  It should work
   even on file systems like FAT that support only short names.
   "Cu" is short for "Coreutils" or for "Changeable unstable",
   take your pick....  */

സ്ഥിതം അക്ഷരം സ്ഥിരം simple_pattern[] = "CuXXXXXX";
എണ്ണൽ { x_suffix_len = വലുപ്പം "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

എണ്ണൽ { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

സ്ഥിതം അക്ഷരം *
samedir_template (അക്ഷരം സ്ഥിരം *dstname, അക്ഷരം buf[smallsize])
{
  ptrdiff_t dstdirlen = last_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + വലുപ്പം simple_pattern;
  അക്ഷരം *dsttmp;
  എങ്കിൽ (dsttmpsize <= smallsize)
    dsttmp = buf;
  അല്ലെങ്കിൽ
    {
      dsttmp = malloc (dsttmpsize);
      എങ്കിൽ (!dsttmp)
        തിരികെ dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_pattern);
  തിരികെ dsttmp;
}


/* Auxiliaries for force_linkat.  */

ഘടന link_arg
{
  പൂർണ്ണസംഖ്യ srcdir;
  അക്ഷരം സ്ഥിരം *srcname;
  പൂർണ്ണസംഖ്യ dstdir;
  പൂർണ്ണസംഖ്യ flags;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
try_link (അക്ഷരം *dest, ശൂന്യം *arg)
{
  ഘടന link_arg *a = arg;
  തിരികെ linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
ബാഹ്യ പൂർണ്ണസംഖ്യ
force_linkat (പൂർണ്ണസംഖ്യ srcdir, അക്ഷരം സ്ഥിരം *srcname,
              പൂർണ്ണസംഖ്യ dstdir, അക്ഷരം സ്ഥിരം *dstname, പൂർണ്ണസംഖ്യ flags, bool force,
              പൂർണ്ണസംഖ്യ linkat_errno)
{
  എങ്കിൽ (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  എങ്കിൽ (!force || linkat_errno != EEXIST)
    തിരികെ linkat_errno;

  അക്ഷരം buf[smallsize];
  അക്ഷരം *dsttmp = samedir_template (dstname, buf);
  എങ്കിൽ (! dsttmp)
    തിരികെ errno;
  ഘടന link_arg arg = { srcdir, srcname, dstdir, flags };
  പൂർണ്ണസംഖ്യ err;

  എങ്കിൽ (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = errno;
  അല്ലെങ്കിൽ
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : errno;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  എങ്കിൽ (dsttmp != buf)
    സ്വതന്ത്രം (dsttmp);
  തിരികെ err;
}


/* Auxiliaries for force_symlinkat.  */

ഘടന symlink_arg
{
  അക്ഷരം സ്ഥിരം *srcname;
  പൂർണ്ണസംഖ്യ dstdir;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
try_symlink (അക്ഷരം *dest, ശൂന്യം *arg)
{
  ഘടന symlink_arg *a = arg;
  തിരികെ symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
ബാഹ്യ പൂർണ്ണസംഖ്യ
force_symlinkat (അക്ഷരം സ്ഥിരം *srcname, പൂർണ്ണസംഖ്യ dstdir, അക്ഷരം സ്ഥിരം *dstname,
                 bool force, പൂർണ്ണസംഖ്യ symlinkat_errno)
{
  എങ്കിൽ (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  എങ്കിൽ (!force || symlinkat_errno != EEXIST)
    തിരികെ symlinkat_errno;

  അക്ഷരം buf[smallsize];
  അക്ഷരം *dsttmp = samedir_template (dstname, buf);
  എങ്കിൽ (!dsttmp)
    തിരികെ errno;
  ഘടന symlink_arg arg = { srcname, dstdir };
  പൂർണ്ണസംഖ്യ err;

  എങ്കിൽ (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = errno;
  അല്ലെങ്കിൽ എങ്കിൽ (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = errno;
      unlinkat (dstdir, dsttmp, 0);
    }
  അല്ലെങ്കിൽ
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  എങ്കിൽ (dsttmp != buf)
    സ്വതന്ത്രം (dsttmp);
  തിരികെ err;
}
