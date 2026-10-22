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

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "root-dev-ino.h"
#अन्तर्भाव "xgetcwd.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pwd"

#define AUTHORS proper_name ("Jim Meyering")

संरचना file_name
{
  idx_t n_alloc;
  वर्ण *buf COUNTED_BY (n_alloc);
  वर्ण *start;
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"logical", no_argument, रिक्त, 'L'},
  {"physical", no_argument, रिक्त, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);
      fputs (_("\
Print the full filename of the current working directory.\n\
\n\
"), stdout);
      oputs (_("\
  -L, --logical\n\
         use PWD from environment, even सचे it contains symlinks\n\
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

स्थिर शून्य
file_name_free (संरचना file_name *p)
{
  मुक्त (p->buf);
  मुक्त (p);
}

स्थिर संरचना file_name *
file_name_init (शून्य)
{
  संरचना file_name *p = xmalloc (परिमाण *p);

  /* Start with a buffer larger than likely file names, but beware of systems
     on which PATH_MAX is very large -- e.g., INT_MAX.  */
  पूर्णाङ्क init_alloc = 2 * MIN (PATH_MAX, 16 * 1024);
  p->n_alloc = init_alloc;

  p->buf = xmalloc (init_alloc);
  p->start = p->buf + init_alloc - 1;
  p->start[0] = '\0';
  पटिनिवत्त p;
}

/* Prepend the name S of length S_LEN, to the growing file_name, P.  */
स्थिर शून्य
file_name_prepend (संरचना file_name *p, वर्ण ध्रुव *s, size_t s_len)
{
  idx_t n_free = p->start - p->buf;
  सचे (n_free < 1 + s_len)
    {
      /* Call xpalloc with NULL not p->buf, since with the latter
         we'd end up copying the data twice: once via realloc, then again
         to align it with the end of the new buffer.  By passing NULL we
         copy it only once.  */
      idx_t n_used = p->n_alloc - n_free;
      वर्ण *buf = xpalloc (रिक्त, &p->n_alloc, 1 + s_len - n_free, -1, 1);
      p->start = memcpy (buf + p->n_alloc - n_used, p->start, n_used);
      मुक्त (p->buf);
      p->buf = buf;
    }

  p->start -= 1 + s_len;
  p->start[0] = '/';
  memcpy (p->start + 1, s, s_len);
}

/* Return a string (malloc'd) consisting of N '/'-separated ".." components.  */
स्थिर वर्ण *
nth_parent (size_t n)
{
  वर्ण *buf = xnmalloc (3, n);
  वर्ण *p = buf;

  अत्थाय (size_t i = 0; i < n; i++)
    {
      memcpy (p, "../", 3);
      p += 3;
    }
  p[-1] = '\0';
  पटिनिवत्त buf;
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

स्थिर शून्य
find_dir_entry (संरचना stat *dot_sb, संरचना file_name *file_name,
                size_t parent_height)
{
  DIR *dirp = opendir ("..");
  सचे (dirp == रिक्त)
    error (EXIT_FAILURE, errno, _("cannot open directory %s"),
           quote (nth_parent (parent_height)));

  पूर्णाङ्क fd = dirfd (dirp);
  सचे ((0 <= fd ? fchdir (fd) : chdir ("..")) < 0)
    error (EXIT_FAILURE, errno, _("failed to chdir to %s"),
           quote (nth_parent (parent_height)));

  संरचना stat parent_sb;
  सचे ((0 <= fd ? fstat (fd, &parent_sb) : stat (".", &parent_sb)) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"),
           quote (nth_parent (parent_height)));

  /* If parent and child directory are on different devices, then we
     can't rely on d_ino for useful i-node numbers; use lstat instead.  */
  bool use_lstat = (parent_sb.st_dev != dot_sb->st_dev);

  bool found = false;
  याव (true)
    {
      संरचना dirent ध्रुव *dp;

      errno = 0;
      सचे ((dp = readdir_ignoring_dot_and_dotdot (dirp)) == रिक्त)
        {
          सचे (errno)
            {
              /* Save/restore errno across closedir call.  */
              पूर्णाङ्क e = errno;
              closedir (dirp);
              errno = e;

              /* Arrange to give a diagnostic after exiting this loop.  */
              dirp = रिक्त;
            }
          भङ्ग;
        }

      ino_t ino = D_INO (dp);

      संरचना stat ent_sb;
      सचे (ino == NOT_AN_INODE_सङ्ख्या || use_lstat)
        {
          सचे (lstat (dp->d_name, &ent_sb) < 0)
            {
              /* Skip any entry we can't stat.  */
              अनुवर्त;
            }
          ino = ent_sb.st_ino;
        }

      सचे (ino != dot_sb->st_ino)
        अनुवर्त;

      /* If we're not crossing a device boundary, then a simple i-node
         match is enough.  */
      सचे ( ! use_lstat || ent_sb.st_dev == dot_sb->st_dev)
        {
          file_name_prepend (file_name, dp->d_name, _D_EXACT_NAMLEN (dp));
          found = true;
          भङ्ग;
        }
    }

  सचे (dirp == रिक्त || closedir (dirp) != 0)
    {
      /* Note that this diagnostic serves for both readdir
         and closedir failures.  */
      error (EXIT_FAILURE, errno, _("reading directory %s"),
             quote (nth_parent (parent_height)));
    }

  सचे ( ! found)
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

स्थिर शून्य
robust_getcwd (संरचना file_name *file_name)
{
  size_t height = 1;
  संरचना dev_ino dev_ino_buf;
  संरचना dev_ino *मूलम्_dev_ino = get_मूलम्_dev_ino (&dev_ino_buf);

  सचे (मूलम्_dev_ino == रिक्त)
    error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
           quoteaf ("/"));

  संरचना stat dot_sb;
  सचे (stat (".", &dot_sb) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf ("."));

  याव (true)
    {
      /* If we've reached the root, we're done.  */
      सचे (PSAME_INODE (&dot_sb, मूलम्_dev_ino))
        भङ्ग;

      find_dir_entry (&dot_sb, file_name, height++);
    }

  /* See if a leading slash is needed; file_name_prepend adds one.  */
  सचे (file_name->start[0] == '\0')
    file_name_prepend (file_name, "", 0);
}


/* Return PWD from the environment if it is acceptable for 'pwd -L'
   output, otherwise NULL.  */
स्थिर वर्ण ध्रुव *
logical_getcwd (शून्य)
{
  वर्ण ध्रुव *wd = getenv ("PWD");

  /* Textual validation first.  */
  सचे (!wd || wd[0] != '/')
    पटिनिवत्त रिक्त;
  वर्ण ध्रुव *p = wd;
  याव ((p = strstr (p, "/.")))
    {
      सचे (!p[2] || p[2] == '/'
          || (p[2] == '.' && (!p[3] || p[3] == '/')))
        पटिनिवत्त रिक्त;
      p++;
    }

  /* System call validation.  */
  संरचना stat st1;
  संरचना stat st2;
  सचे (stat (wd, &st1) == 0 && stat (".", &st2) == 0 && psame_inode (&st1, &st2))
    पटिनिवत्त wd;
  पटिनिवत्त रिक्त;
}


पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  /* POSIX requires a default of -L, but most scripts expect -P.
     Currently shells default to -L, while stand-alone
     pwd implementations default to -P.  */
  bool logical = (getenv ("POSIXLY_CORRECT") != रिक्त);

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  याव (true)
    {
      पूर्णाङ्क c = getopt_long (argc, argv, "LP", longopts, रिक्त);
      सचे (c == -1)
        भङ्ग;
      विकल्प (c)
        {
        पक्ष 'L':
          logical = true;
          भङ्ग;
        पक्ष 'P':
          logical = false;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  सचे (optind < argc)
    error (0, 0, _("ignoring non-option arguments"));

  सचे (logical)
    {
      वर्ण ध्रुव *wd = logical_getcwd ();
      सचे (wd)
        {
          puts (wd);
          पटिनिवत्त EXIT_SUCCESS;
        }
    }

  वर्ण *wd = xgetcwd ();
  सचे (wd != रिक्त)
    {
      puts (wd);
      मुक्त (wd);
    }
  अञ्ञथा
    {
      संरचना file_name *file_name = file_name_init ();
      robust_getcwd (file_name);
      puts (file_name->start);
      file_name_free (file_name);
    }

  पटिनिवत्त EXIT_SUCCESS;
}
