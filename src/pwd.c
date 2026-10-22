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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "root-dev-ino.h"
#অন্তর্ভাৱ "xgetcwd.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pwd"

#define AUTHORS proper_name ("Jim Meyering")

সংরচনা file_name
{
  idx_t n_alloc;
  ৱর্ণ *buf COUNTED_BY (n_alloc);
  ৱর্ণ *start;
};

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {"logical", no_argument, রিক্ত, 'L'},
  {"physical", no_argument, রিক্ত, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);
      fputs (_("\
Print the full filename of the current working directory.\n\
\n\
"), stdout);
      oputs (_("\
  -L, --logical\n\
         use PWD from environment, even যদি it contains symlinks\n\
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

স্থির শূন্য
file_name_free (সংরচনা file_name *p)
{
  মুক্ত (p->buf);
  মুক্ত (p);
}

স্থির সংরচনা file_name *
file_name_init (শূন্য)
{
  সংরচনা file_name *p = xmalloc (পরিমাণ *p);

  /* Start with a buffer larger than likely file names, but beware of systems
     on which PATH_MAX is very large -- e.g., INT_MAX.  */
  পূর্ণাঙ্ক init_alloc = 2 * MIN (PATH_MAX, 16 * 1024);
  p->n_alloc = init_alloc;

  p->buf = xmalloc (init_alloc);
  p->start = p->buf + init_alloc - 1;
  p->start[0] = '\0';
  ঘূৰাই p;
}

/* Prepend the name S of length S_LEN, to the growing file_name, P.  */
স্থির শূন্য
file_name_prepend (সংরচনা file_name *p, ৱর্ণ ধ্রুৱ *s, size_t s_len)
{
  idx_t n_free = p->start - p->buf;
  যদি (n_free < 1 + s_len)
    {
      /* Call xpalloc with NULL not p->buf, since with the latter
         we'd end up copying the data twice: once via realloc, then again
         to align it with the end of the new buffer.  By passing NULL we
         copy it only once.  */
      idx_t n_used = p->n_alloc - n_free;
      ৱর্ণ *buf = xpalloc (রিক্ত, &p->n_alloc, 1 + s_len - n_free, -1, 1);
      p->start = memcpy (buf + p->n_alloc - n_used, p->start, n_used);
      মুক্ত (p->buf);
      p->buf = buf;
    }

  p->start -= 1 + s_len;
  p->start[0] = '/';
  memcpy (p->start + 1, s, s_len);
}

/* Return a string (malloc'd) consisting of N '/'-separated ".." components.  */
স্থির ৱর্ণ *
nth_parent (size_t n)
{
  ৱর্ণ *buf = xnmalloc (3, n);
  ৱর্ণ *p = buf;

  বাবে (size_t i = 0; i < n; i++)
    {
      memcpy (p, "../", 3);
      p += 3;
    }
  p[-1] = '\0';
  ঘূৰাই buf;
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

স্থির শূন্য
find_dir_entry (সংরচনা stat *dot_sb, সংরচনা file_name *file_name,
                size_t parent_height)
{
  DIR *dirp = opendir ("..");
  যদি (dirp == রিক্ত)
    error (EXIT_FAILURE, errno, _("cannot open directory %s"),
           quote (nth_parent (parent_height)));

  পূর্ণাঙ্ক fd = dirfd (dirp);
  যদি ((0 <= fd ? fchdir (fd) : chdir ("..")) < 0)
    error (EXIT_FAILURE, errno, _("failed to chdir to %s"),
           quote (nth_parent (parent_height)));

  সংরচনা stat parent_sb;
  যদি ((0 <= fd ? fstat (fd, &parent_sb) : stat (".", &parent_sb)) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"),
           quote (nth_parent (parent_height)));

  /* If parent and child directory are on different devices, then we
     can't rely on d_ino for useful i-node numbers; use lstat instead.  */
  bool use_lstat = (parent_sb.st_dev != dot_sb->st_dev);

  bool found = false;
  যেতিয়ালৈ (true)
    {
      সংরচনা dirent ধ্রুৱ *dp;

      errno = 0;
      যদি ((dp = readdir_ignoring_dot_and_dotdot (dirp)) == রিক্ত)
        {
          যদি (errno)
            {
              /* Save/restore errno across closedir call.  */
              পূর্ণাঙ্ক e = errno;
              closedir (dirp);
              errno = e;

              /* Arrange to give a diagnostic after exiting this loop.  */
              dirp = রিক্ত;
            }
          ভঙ্গ;
        }

      ino_t ino = D_INO (dp);

      সংরচনা stat ent_sb;
      যদি (ino == NOT_AN_INODE_সংখ্যা || use_lstat)
        {
          যদি (lstat (dp->d_name, &ent_sb) < 0)
            {
              /* Skip any entry we can't stat.  */
              অনুৱর্ত;
            }
          ino = ent_sb.st_ino;
        }

      যদি (ino != dot_sb->st_ino)
        অনুৱর্ত;

      /* If we're not crossing a device boundary, then a simple i-node
         match is enough.  */
      যদি ( ! use_lstat || ent_sb.st_dev == dot_sb->st_dev)
        {
          file_name_prepend (file_name, dp->d_name, _D_EXACT_NAMLEN (dp));
          found = true;
          ভঙ্গ;
        }
    }

  যদি (dirp == রিক্ত || closedir (dirp) != 0)
    {
      /* Note that this diagnostic serves for both readdir
         and closedir failures.  */
      error (EXIT_FAILURE, errno, _("reading directory %s"),
             quote (nth_parent (parent_height)));
    }

  যদি ( ! found)
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

স্থির শূন্য
robust_getcwd (সংরচনা file_name *file_name)
{
  size_t height = 1;
  সংরচনা dev_ino dev_ino_buf;
  সংরচনা dev_ino *মূল_dev_ino = get_মূল_dev_ino (&dev_ino_buf);

  যদি (মূল_dev_ino == রিক্ত)
    error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
           quoteaf ("/"));

  সংরচনা stat dot_sb;
  যদি (stat (".", &dot_sb) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf ("."));

  যেতিয়ালৈ (true)
    {
      /* If we've reached the root, we're done.  */
      যদি (PSAME_INODE (&dot_sb, মূল_dev_ino))
        ভঙ্গ;

      find_dir_entry (&dot_sb, file_name, height++);
    }

  /* See if a leading slash is needed; file_name_prepend adds one.  */
  যদি (file_name->start[0] == '\0')
    file_name_prepend (file_name, "", 0);
}


/* Return PWD from the environment if it is acceptable for 'pwd -L'
   output, otherwise NULL.  */
স্থির ৱর্ণ ধ্রুৱ *
logical_getcwd (শূন্য)
{
  ৱর্ণ ধ্রুৱ *wd = getenv ("PWD");

  /* Textual validation first.  */
  যদি (!wd || wd[0] != '/')
    ঘূৰাই রিক্ত;
  ৱর্ণ ধ্রুৱ *p = wd;
  যেতিয়ালৈ ((p = strstr (p, "/.")))
    {
      যদি (!p[2] || p[2] == '/'
          || (p[2] == '.' && (!p[3] || p[3] == '/')))
        ঘূৰাই রিক্ত;
      p++;
    }

  /* System call validation.  */
  সংরচনা stat st1;
  সংরচনা stat st2;
  যদি (stat (wd, &st1) == 0 && stat (".", &st2) == 0 && psame_inode (&st1, &st2))
    ঘূৰাই wd;
  ঘূৰাই রিক্ত;
}


পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  /* POSIX requires a default of -L, but most scripts expect -P.
     Currently shells default to -L, while stand-alone
     pwd implementations default to -P.  */
  bool logical = (getenv ("POSIXLY_CORRECT") != রিক্ত);

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ (true)
    {
      পূর্ণাঙ্ক c = getopt_long (argc, argv, "LP", longopts, রিক্ত);
      যদি (c == -1)
        ভঙ্গ;
      ৱিকল্প (c)
        {
        পক্ষ 'L':
          logical = true;
          ভঙ্গ;
        পক্ষ 'P':
          logical = false;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  যদি (optind < argc)
    error (0, 0, _("ignoring non-option arguments"));

  যদি (logical)
    {
      ৱর্ণ ধ্রুৱ *wd = logical_getcwd ();
      যদি (wd)
        {
          puts (wd);
          ঘূৰাই EXIT_SUCCESS;
        }
    }

  ৱর্ণ *wd = xgetcwd ();
  যদি (wd != রিক্ত)
    {
      puts (wd);
      মুক্ত (wd);
    }
  অন্যথা
    {
      সংরচনা file_name *file_name = file_name_init ();
      robust_getcwd (file_name);
      puts (file_name->start);
      file_name_free (file_name);
    }

  ঘূৰাই EXIT_SUCCESS;
}
