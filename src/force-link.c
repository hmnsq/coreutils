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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "force-link.h"

#ଅନ୍ତର୍ଭାଵ <tempname.h>

/* A basename pattern suitable for a temporary file.  It should work
   even on file systems like FAT that support only short names.
   "Cu" is short for "Coreutils" or for "Changeable unstable",
   take your pick....  */

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ simple_pattern[] = "CuXXXXXX";
ଗଣନା { x_suffix_len = ଆକାର "XXXXXX" - 1 };

/* A size for smallish buffers containing file names.  Longer file
   names can use malloc.  */

ଗଣନା { smallsize = 256 };

/* Return a template for a file in the same directory as DSTNAME.
   Use BUF if the template fits, otherwise use malloc and return NULL
   (setting errno) if unsuccessful.  */

ସ୍ଥିର ଅକ୍ଷର *
samedir_template (ଅକ୍ଷର ଅଚଳ *dstname, ଅକ୍ଷର buf[smallsize])
{
  ptrdiff_t dstdirlen = last_component (dstname) - dstname;
  size_t dsttmpsize = dstdirlen + ଆକାର simple_pattern;
  ଅକ୍ଷର *dsttmp;
  ଯଦି (dsttmpsize <= smallsize)
    dsttmp = buf;
  ଅନ୍ୟଥା
    {
      dsttmp = malloc (dsttmpsize);
      ଯଦି (!dsttmp)
        ଫେରନ୍ତୁ dsttmp;
    }
  strcpy (mempcpy (dsttmp, dstname, dstdirlen), simple_pattern);
  ଫେରନ୍ତୁ dsttmp;
}


/* Auxiliaries for force_linkat.  */

ସଂରଚନା link_arg
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcdir;
  ଅକ୍ଷର ଅଚଳ *srcname;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dstdir;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
try_link (ଅକ୍ଷର *dest, ଶୂନ୍ୟ *arg)
{
  ସଂରଚନା link_arg *a = arg;
  ଫେରନ୍ତୁ linkat (a->srcdir, a->srcname, a->dstdir, dest, a->flags);
}

/* Hard-link directory SRCDIR's file SRCNAME to directory DSTDIR's
   file DSTNAME, using linkat-style FLAGS to control the linking.
   If FORCE and DSTNAME already exists, replace it atomically.
   If LINKAT_ERRNO is 0, the hard link is already done; if positive,
   the hard link was tried and failed with errno == LINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
force_linkat (ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcdir, ଅକ୍ଷର ଅଚଳ *srcname,
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା dstdir, ଅକ୍ଷର ଅଚଳ *dstname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, bool force,
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା linkat_errno)
{
  ଯଦି (linkat_errno < 0)
    linkat_errno = (linkat (srcdir, srcname, dstdir, dstname, flags) == 0
                    ? 0 : errno);
  ଯଦି (!force || linkat_errno != EEXIST)
    ଫେରନ୍ତୁ linkat_errno;

  ଅକ୍ଷର buf[smallsize];
  ଅକ୍ଷର *dsttmp = samedir_template (dstname, buf);
  ଯଦି (! dsttmp)
    ଫେରନ୍ତୁ errno;
  ସଂରଚନା link_arg arg = { srcdir, srcname, dstdir, flags };
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

  ଯଦି (try_tempname_len (dsttmp, 0, &arg, try_link, x_suffix_len) != 0)
    err = errno;
  ଅନ୍ୟଥା
    {
      err = renameat (dstdir, dsttmp, dstdir, dstname) == 0 ? -1 : errno;
      /* Unlink DSTTMP even if renameat succeeded, in case DSTTMP
         and DSTNAME were already the same hard link and renameat
         was a no-op.  */
      unlinkat (dstdir, dsttmp, 0);
    }

  ଯଦି (dsttmp != buf)
    ମୁକ୍ତ (dsttmp);
  ଫେରନ୍ତୁ err;
}


/* Auxiliaries for force_symlinkat.  */

ସଂରଚନା symlink_arg
{
  ଅକ୍ଷର ଅଚଳ *srcname;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା dstdir;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
try_symlink (ଅକ୍ଷର *dest, ଶୂନ୍ୟ *arg)
{
  ସଂରଚନା symlink_arg *a = arg;
  ଫେରନ୍ତୁ symlinkat (a->srcname, a->dstdir, dest);
}

/* Create a symlink containing SRCNAME in directory DSTDIR's file DSTNAME.
   If FORCE and DSTNAME already exists, replace it atomically.
   If SYMLINKAT_ERRNO is 0, the symlink is already done; if positive,
   the symlink was tried and failed with errno == SYMLINKAT_ERRNO.  Return
   -1 if successful and DSTNAME already existed,
   0 if successful and DSTNAME did not already exist, and
   a positive errno value on failure.  */
ବାହ୍ୟ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
force_symlinkat (ଅକ୍ଷର ଅଚଳ *srcname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dstdir, ଅକ୍ଷର ଅଚଳ *dstname,
                 bool force, ପୂର୍ଣ୍ଣସଂଖ୍ୟା symlinkat_errno)
{
  ଯଦି (symlinkat_errno < 0)
    symlinkat_errno = symlinkat (srcname, dstdir, dstname) == 0 ? 0 : errno;
  ଯଦି (!force || symlinkat_errno != EEXIST)
    ଫେରନ୍ତୁ symlinkat_errno;

  ଅକ୍ଷର buf[smallsize];
  ଅକ୍ଷର *dsttmp = samedir_template (dstname, buf);
  ଯଦି (!dsttmp)
    ଫେରନ୍ତୁ errno;
  ସଂରଚନା symlink_arg arg = { srcname, dstdir };
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା err;

  ଯଦି (try_tempname_len (dsttmp, 0, &arg, try_symlink, x_suffix_len) != 0)
    err = errno;
  ଅନ୍ୟଥା ଯଦି (renameat (dstdir, dsttmp, dstdir, dstname) != 0)
    {
      err = errno;
      unlinkat (dstdir, dsttmp, 0);
    }
  ଅନ୍ୟଥା
    {
      /* Don't worry about renameat being a no-op, since DSTTMP is
         newly created.  */
      err = -1;
    }

  ଯଦି (dsttmp != buf)
    ମୁକ୍ତ (dsttmp);
  ଫେରନ୍ତୁ err;
}
