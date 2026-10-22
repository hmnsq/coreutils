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

#समावेश <config.h>
#समावेश "system.h"

#समावेश "force-link.h"

#समावेश <tempname.h>

/* A basename pattern suitable for a temporary file.  It should work
   even on file systems like FAT that support only short names.
   "Cu" is short for "Coreutils" or for "Changeable unstable",
   take your pick....  */

अटल अक्षर स्थिर simple_pattern[] = "CuXXXXXX";
क्रमागत { x_suffix_len = माप "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

क्रमागत { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

अटल अक्षर *
samedir_template (अक्षर स्थिर *dstname, अक्षर buf[smallsize])
{
  सूचक_भेद_प्रकार dstdirlen = last_component (dstname) - dstname;
  माप_प्रकार dsttmpsize = dstdirlen + माप simple_pattern;
  अक्षर *dsttmp;
  अगर (dsttmpsize <= smallsize)
    dsttmp = buf;
  अन्यथा
    {
      dsttmp = दो_स्मृति (dsttmpsize);
      अगर (!dsttmp)
        वापस dsttmp;
    }
  म_नकल (mempcpy (dsttmp, dstname, dstdirlen), simple_pattern);
  वापस dsttmp;
}


/* Auxiliaries for force_linkat.  */

काष्ठा link_arg
{
  पूर्णांक srcdir;
  अक्षर स्थिर *srcname;
  पूर्णांक dstdir;
  पूर्णांक flags;
};

अटल पूर्णांक
try_link (अक्षर *dest, व्योम *arg)
{
  काष्ठा link_arg *a = arg;
  वापस linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
बाह्य पूर्णांक
force_linkat (पूर्णांक srcdir, अक्षर स्थिर *srcname,
              पूर्णांक dstdir, अक्षर स्थिर *dstname, पूर्णांक flags, bool force,
              पूर्णांक linkat_errno)
{
  अगर (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : त्रुटि_सं);
  अगर (!force || linkat_errno != EEXIST)
    वापस linkat_errno;

  अक्षर buf[smallsize];
  अक्षर *dsttmp = samedir_template (dstname, buf);
  अगर (! dsttmp)
    वापस त्रुटि_सं;
  काष्ठा link_arg arg = { srcdir, srcname, dstdir, flags };
  पूर्णांक err;

  अगर (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = त्रुटि_सं;
  अन्यथा
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : त्रुटि_सं;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  अगर (dsttmp != buf)
    मुक्त (dsttmp);
  वापस err;
}


/* Auxiliaries for force_symlinkat.  */

काष्ठा symlink_arg
{
  अक्षर स्थिर *srcname;
  पूर्णांक dstdir;
};

अटल पूर्णांक
try_symlink (अक्षर *dest, व्योम *arg)
{
  काष्ठा symlink_arg *a = arg;
  वापस symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
बाह्य पूर्णांक
force_symlinkat (अक्षर स्थिर *srcname, पूर्णांक dstdir, अक्षर स्थिर *dstname,
                 bool force, पूर्णांक symlinkat_errno)
{
  अगर (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : त्रुटि_सं;
  अगर (!force || symlinkat_errno != EEXIST)
    वापस symlinkat_errno;

  अक्षर buf[smallsize];
  अक्षर *dsttmp = samedir_template (dstname, buf);
  अगर (!dsttmp)
    वापस त्रुटि_सं;
  काष्ठा symlink_arg arg = { srcname, dstdir };
  पूर्णांक err;

  अगर (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = त्रुटि_सं;
  अन्यथा अगर (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = त्रुटि_सं;
      unlinkat (dstdir, dsttmp, 0);
    }
  अन्यथा
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  अगर (dsttmp != buf)
    मुक्त (dsttmp);
  वापस err;
}
