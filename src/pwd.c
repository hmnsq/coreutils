/* pwd - print current directory
   Copyright (C) 1994-2026 Free Software Foundation, Inc.

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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "root-dev-ino.h"
#ଅନ୍ତର୍ଭାଵ "xgetcwd.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pwd"

#define AUTHORS proper_name ("Jim Meyering")

ସଂରଚନା file_name
{
  idx_t n_alloc;
  ଅକ୍ଷର *buf COUNTED_BY (n_alloc);
  ଅକ୍ଷର *start;
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"logical", no_argument, ଖାଲି, 'L'},
  {"physical", no_argument, ଖାଲି, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);
      fputs (_("\
Print the full filename of the current working directory.\n\
\n\
"), stdout);
      oputs (_("\
  -L, --logical\n\
         use PWD from environment, even ଯଦି it contains symlinks\n\
"));
      oputs (_("\
  -P, --physical\n\
         resolve all symlinks\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
If no option is specified, -P is assumed.\n\
"), stdout);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ସ୍ଥିର ଶୂନ୍ୟ
file_name_free (ସଂରଚନା file_name *p)
{
  ମୁକ୍ତ (p->buf);
  ମୁକ୍ତ (p);
}

ସ୍ଥିର ସଂରଚନା file_name *
file_name_init (ଶୂନ୍ୟ)
{
  ସଂରଚନା file_name *p = xmalloc (ଆକାର *p);

  /* Start with a buffer larger than likely file names, but beware of systems
     on which PATH_MAX is very large -- e.g., INT_MAX.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା init_alloc = 2 * MIN (PATH_MAX, 16 * 1024);
  p->n_alloc = init_alloc;

  p->buf = xmalloc (init_alloc);
  p->start = p->buf + init_alloc - 1;
  p->start[0] = '\0';
  ଫେରନ୍ତୁ p;
}

/* Prepend the name S of length S_LEN, to the growing file_name, P.  */
ସ୍ଥିର ଶୂନ୍ୟ
file_name_prepend (ସଂରଚନା file_name *p, ଅକ୍ଷର ଅଚଳ *s, size_t s_len)
{
  idx_t n_free = p->start - p->buf;
  ଯଦି (n_free < 1 + s_len)
    {
      /* Call xpalloc with NULL not p->buf, since with the latter
         we'd end up copying the data twice: once via realloc, then again
         to align it with the end of the new buffer.  By passing NULL we
         copy it only once.  */
      idx_t n_used = p->n_alloc - n_free;
      ଅକ୍ଷର *buf = xpalloc (ଖାଲି, &p->n_alloc, 1 + s_len - n_free, -1, 1);
      p->start = memcpy (buf + p->n_alloc - n_used, p->start, n_used);
      ମୁକ୍ତ (p->buf);
      p->buf = buf;
    }

  p->start -= 1 + s_len;
  p->start[0] = '/';
  memcpy (p->start + 1, s, s_len);
}

/* Return a string (malloc'd) consisting of N '/'-separated ".." components.  */
ସ୍ଥିର ଅକ୍ଷର *
nth_parent (size_t n)
{
  ଅକ୍ଷର *buf = xnmalloc (3, n);
  ଅକ୍ଷର *p = buf;

  ପାଇଁ (size_t i = 0; i < n; i++)
    {
      memcpy (p, "../", 3);
      p += 3;
    }
  p[-1] = '\0';
  ଫେରନ୍ତୁ buf;
}

/* Determine the basename of the current directory, where DOT_SB is the
   result of lstat'ing "." and prepend that to the file name in *FILE_NAME.
   Find the directory entry in '..' that matches the dev/i-node of DOT_SB.
   Upon success, update *DOT_SB with stat information of '..', chdir to '..',
   and prepend "/basename" to FILE_NAME.
   Otherwise, exit with a diagnostic.
   PARENT_HEIGHT is the number of levels '..' is above the starting directory.
   The first time this function is called (from the initial directory),
   PARENT_HEIGHT is 1.  This is solely for diagnostics.
   Exit nonzero upon error.  */

ସ୍ଥିର ଶୂନ୍ୟ
find_dir_entry (ସଂରଚନା stat *dot_sb, ସଂରଚନା file_name *file_name,
                size_t parent_height)
{
  DIR *dirp = opendir ("..");
  ଯଦି (dirp == ଖାଲି)
    error (EXIT_FAILURE, errno, _("cannot open directory %s"),
           quote (nth_parent (parent_height)));

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = dirfd (dirp);
  ଯଦି ((0 <= fd ? fchdir (fd) : chdir ("..")) < 0)
    error (EXIT_FAILURE, errno, _("failed to chdir to %s"),
           quote (nth_parent (parent_height)));

  ସଂରଚନା stat parent_sb;
  ଯଦି ((0 <= fd ? fstat (fd, &parent_sb) : stat (".", &parent_sb)) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"),
           quote (nth_parent (parent_height)));

  /* If parent and child directory are on different devices, then we
     can't rely on d_ino for useful i-node numbers; use lstat instead.  */
  bool use_lstat = (parent_sb.st_dev != dot_sb->st_dev);

  bool found = false;
  ଯେତେବେଳେ (true)
    {
      ସଂରଚନା dirent ଅଚଳ *dp;

      errno = 0;
      ଯଦି ((dp = readdir_ignoring_dot_and_dotdot (dirp)) == ଖାଲି)
        {
          ଯଦି (errno)
            {
              /* Save/restore errno across closedir call.  */
              ପୂର୍ଣ୍ଣସଂଖ୍ୟା e = errno;
              closedir (dirp);
              errno = e;

              /* Arrange to give a diagnostic after exiting this loop.  */
              dirp = ଖାଲି;
            }
          ଭାଙ୍ଗ;
        }

      ino_t ino = D_INO (dp);

      ସଂରଚନା stat ent_sb;
      ଯଦି (ino == NOT_AN_INODE_ସଂଖ୍ୟା || use_lstat)
        {
          ଯଦି (lstat (dp->d_name, &ent_sb) < 0)
            {
              /* Skip any entry we can't stat.  */
              ଜାରି;
            }
          ino = ent_sb.st_ino;
        }

      ଯଦି (ino != dot_sb->st_ino)
        ଜାରି;

      /* If we're not crossing a device boundary, then a simple i-node
         match is enough.  */
      ଯଦି ( ! use_lstat || ent_sb.st_dev == dot_sb->st_dev)
        {
          file_name_prepend (file_name, dp->d_name, _D_EXACT_NAMLEN (dp));
          found = true;
          ଭାଙ୍ଗ;
        }
    }

  ଯଦି (dirp == ଖାଲି || closedir (dirp) != 0)
    {
      /* Note that this diagnostic serves for both readdir
         and closedir failures.  */
      error (EXIT_FAILURE, errno, _("reading directory %s"),
             quote (nth_parent (parent_height)));
    }

  ଯଦି ( ! found)
    error (EXIT_FAILURE, 0,
           _("couldn't find directory entry in %s with matching i-node"),
           quote (nth_parent (parent_height)));

  *dot_sb = parent_sb;
}

/* Construct the full, absolute name of the current working
   directory and store it in *FILE_NAME.
   The getcwd function performs nearly the same task, but is typically
   unable to handle names longer than PATH_MAX.  This function has
   no such limitation.  However, this function *can* fail due to
   permission problems or a lack of memory, while GNU/Linux's getcwd
   function works regardless of restricted permissions on parent
   directories.  Upon failure, give a diagnostic and exit nonzero.

   Note: although this function is similar to getcwd, it has a fundamental
   difference in that it gives a diagnostic and exits upon failure.
   I would have liked a function that did not exit, and that could be
   used as a getcwd replacement.  Unfortunately, considering all of
   the information the caller would require in order to produce good
   diagnostics, it doesn't seem worth the added complexity.
   In any case, any getcwd replacement must *not* exceed the PATH_MAX
   limitation.  Otherwise, functions like 'chdir' would fail with
   ENAMETOOLONG.

   FIXME-maybe: if find_dir_entry fails due to permissions, try getcwd,
   in case the unreadable directory is close enough to the root that
   getcwd works from there.  */

ସ୍ଥିର ଶୂନ୍ୟ
robust_getcwd (ସଂରଚନା file_name *file_name)
{
  size_t height = 1;
  ସଂରଚନା dev_ino dev_ino_buf;
  ସଂରଚନା dev_ino *ମୂଳ_dev_ino = get_ମୂଳ_dev_ino (&dev_ino_buf);

  ଯଦି (ମୂଳ_dev_ino == ଖାଲି)
    error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
           quoteaf ("/"));

  ସଂରଚନା stat dot_sb;
  ଯଦି (stat (".", &dot_sb) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf ("."));

  ଯେତେବେଳେ (true)
    {
      /* If we've reached the root, we're done.  */
      ଯଦି (PSAME_INODE (&dot_sb, ମୂଳ_dev_ino))
        ଭାଙ୍ଗ;

      find_dir_entry (&dot_sb, file_name, height++);
    }

  /* See if a leading slash is needed; file_name_prepend adds one.  */
  ଯଦି (file_name->start[0] == '\0')
    file_name_prepend (file_name, "", 0);
}


/* Return PWD from the environment if it is acceptable for 'pwd -L'
   output, otherwise NULL.  */
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
logical_getcwd (ଶୂନ୍ୟ)
{
  ଅକ୍ଷର ଅଚଳ *wd = getenv ("PWD");

  /* Textual validation first.  */
  ଯଦି (!wd || wd[0] != '/')
    ଫେରନ୍ତୁ ଖାଲି;
  ଅକ୍ଷର ଅଚଳ *p = wd;
  ଯେତେବେଳେ ((p = strstr (p, "/.")))
    {
      ଯଦି (!p[2] || p[2] == '/'
          || (p[2] == '.' && (!p[3] || p[3] == '/')))
        ଫେରନ୍ତୁ ଖାଲି;
      p++;
    }

  /* System call validation.  */
  ସଂରଚନା stat st1;
  ସଂରଚନା stat st2;
  ଯଦି (stat (wd, &st1) == 0 && stat (".", &st2) == 0 && psame_inode (&st1, &st2))
    ଫେରନ୍ତୁ wd;
  ଫେରନ୍ତୁ ଖାଲି;
}


ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  /* POSIX requires a default of -L, but most scripts expect -P.
     Currently shells default to -L, while stand-alone
     pwd implementations default to -P.  */
  bool logical = (getenv ("POSIXLY_CORRECT") != ଖାଲି);

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ (true)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getopt_long (argc, argv, "LP", longopts, ଖାଲି);
      ଯଦି (c == -1)
        ଭାଙ୍ଗ;
      ବାଛ (c)
        {
        ସ୍ଥିତି 'L':
          logical = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'P':
          logical = false;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (optind < argc)
    error (0, 0, _("ignoring non-option arguments"));

  ଯଦି (logical)
    {
      ଅକ୍ଷର ଅଚଳ *wd = logical_getcwd ();
      ଯଦି (wd)
        {
          puts (wd);
          ଫେରନ୍ତୁ EXIT_SUCCESS;
        }
    }

  ଅକ୍ଷର *wd = xgetcwd ();
  ଯଦି (wd != ଖାଲି)
    {
      puts (wd);
      ମୁକ୍ତ (wd);
    }
  ଅନ୍ୟଥା
    {
      ସଂରଚନା file_name *file_name = file_name_init ();
      robust_getcwd (file_name);
      puts (file_name->start);
      file_name_free (file_name);
    }

  ଫେରନ୍ତୁ EXIT_SUCCESS;
}
