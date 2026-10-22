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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "root-dev-ino.h"
#ಅನ್ತರ್ಭಾವ "xgetcwd.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "pwd"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Jim Meyering")

ರಚನೆ file_name
{
  idx_t n_alloc;
  ಅಕ್ಷರ *buf COUNTED_BY (n_alloc);
  ಅಕ್ಷರ *ಆರಂಭ;
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"logical", no_ವಾದ, ಖಾಲಿ, 'L'},
  {"physical", no_ವಾದ, ಖಾಲಿ, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Print the ತುಂಬಿದ filename ದ the current working directory.\n\
\n\
"), stdout);
      oputs (_("\
  -L, --logical\n\
         use PWD from environment, even ಆದರೆ it ಒಳಗೊಂಡಿದೆ symlinks\n\
"));
      oputs (_("\
  -P, --physical\n\
         resolve all symlinks\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\n\
If no option is specified, -P is assumed.\n\
"), stdout);
      ಬರೆ (USAGE_BUILTIN_WARNING, ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಸ್ಥಿರ ಶೂನ್ಯ
file_name_free (ರಚನೆ file_name *p)
{
  ಬಿಡುಗಡೆ (p->buf);
  ಬಿಡುಗಡೆ (p);
}

ಸ್ಥಿರ ರಚನೆ file_name *
file_name_init (ಶೂನ್ಯ)
{
  ರಚನೆ file_name *p = xmalloc (ಗಾತ್ರ *p);

  /* Start with a buffer larger than likely file names, but beware of systems
     on which PATH_MAX is very large -- e.g., INT_MAX.  */
  ಪೂರ್ಣಾಂಕ init_alloc = 2 * MIN (PATH_MAX, 16 * 1024);
  p->n_alloc = init_alloc;

  p->buf = xmalloc (init_alloc);
  p->ಆರಂಭ = p->buf + init_alloc - 1;
  p->ಆರಂಭ[0] = '\0';
  ಹಿಂತಿರುಗಿ p;
}

/* Prepend the name S of length S_LEN, to the growing file_name, P.  */
ಸ್ಥಿರ ಶೂನ್ಯ
file_name_prepend (ರಚನೆ file_name *p, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s, size_t s_len)
{
  idx_t n_free = p->ಆರಂಭ - p->buf;
  ಆದರೆ (n_free < 1 + s_len)
    {
      /* Call xpalloc with NULL not p->buf, since with the latter
         we'd end up copying the data twice: once via realloc, then again
         to align it with the end of the new buffer.  By passing NULL we
         copy it only once.  */
      idx_t n_ಬಳಸಿದ = p->n_alloc - n_free;
      ಅಕ್ಷರ *buf = xpalloc (ಖಾಲಿ, &p->n_alloc, 1 + s_len - n_free, -1, 1);
      p->ಆರಂಭ = ಸ್ಮರಣೆ_ನಕಲು (buf + p->n_alloc - n_ಬಳಸಿದ, p->ಆರಂಭ, n_ಬಳಸಿದ);
      ಬಿಡುಗಡೆ (p->buf);
      p->buf = buf;
    }

  p->ಆರಂಭ -= 1 + s_len;
  p->ಆರಂಭ[0] = '/';
  ಸ್ಮರಣೆ_ನಕಲು (p->ಆರಂಭ + 1, s, s_len);
}

/* Return a string (malloc'd) consisting of N '/'-separated ".." components.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
nth_ಪೋಷಕ (size_t n)
{
  ಅಕ್ಷರ *buf = xnmalloc (3, n);
  ಅಕ್ಷರ *p = buf;

  ಗಾಗಿ (size_t i = 0; i < n; i++)
    {
      ಸ್ಮರಣೆ_ನಕಲು (p, "../", 3);
      p += 3;
    }
  p[-1] = '\0';
  ಹಿಂತಿರುಗಿ buf;
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

ಸ್ಥಿರ ಶೂನ್ಯ
ಕಂಡುಹಿಡಿ_dir_ನಮೂದು (ರಚನೆ stat *dot_sb, ರಚನೆ file_name *file_name,
                size_t ಪೋಷಕ_ಎತ್ತರ)
{
  DIR *dirp = opendir ("..");
  ಆದರೆ (dirp == ಖಾಲಿ)
    error (EXIT_FAILURE, errno, _("cannot open directory %s"),
           quote (nth_ಪೋಷಕ (ಪೋಷಕ_ಎತ್ತರ)));

  ಪೂರ್ಣಾಂಕ fd = dirfd (dirp);
  ಆದರೆ ((0 <= fd ? fchdir (fd) : chdir ("..")) < 0)
    error (EXIT_FAILURE, errno, _("failed to chdir to %s"),
           quote (nth_ಪೋಷಕ (ಪೋಷಕ_ಎತ್ತರ)));

  ರಚನೆ stat ಪೋಷಕ_sb;
  ಆದರೆ ((0 <= fd ? fstat (fd, &ಪೋಷಕ_sb) : stat (".", &ಪೋಷಕ_sb)) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"),
           quote (nth_ಪೋಷಕ (ಪೋಷಕ_ಎತ್ತರ)));

  /* If parent and child directory are on different devices, then we
     can't rely on d_ino for useful i-node numbers; use lstat instead.  */
  bool use_lstat = (ಪೋಷಕ_sb.st_dev != dot_sb->st_dev);

  bool ಸಿಕ್ಕಿತು = false;
  ಆಗ (true)
    {
      ರಚನೆ dirent ಸ್ಥಿರಾಂಕ *dp;

      errno = 0;
      ಆದರೆ ((dp = readdir_ignoring_dot_and_dotdot (dirp)) == ಖಾಲಿ)
        {
          ಆದರೆ (errno)
            {
              /* Save/restore errno across closedir call.  */
              ಪೂರ್ಣಾಂಕ e = errno;
              closedir (dirp);
              errno = e;

              /* Arrange to give a diagnostic after exiting this loop.  */
              dirp = ಖಾಲಿ;
            }
          ನಿಲ್ಲಿಸು;
        }

      ino_t ino = D_INO (dp);

      ರಚನೆ stat ent_sb;
      ಆದರೆ (ino == NOT_AN_INODE_ಸಂಖ್ಯೆ || use_lstat)
        {
          ಆದರೆ (lstat (dp->d_name, &ent_sb) < 0)
            {
              /* Skip any entry we can't stat.  */
              ಮುಂದುವರಿ;
            }
          ino = ent_sb.st_ino;
        }

      ಆದರೆ (ino != dot_sb->st_ino)
        ಮುಂದುವರಿ;

      /* If we're not crossing a device boundary, then a simple i-node
         match is enough.  */
      ಆದರೆ ( ! use_lstat || ent_sb.st_dev == dot_sb->st_dev)
        {
          file_name_prepend (file_name, dp->d_name, _D_EXACT_NAMLEN (dp));
          ಸಿಕ್ಕಿತು = true;
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (dirp == ಖಾಲಿ || closedir (dirp) != 0)
    {
      /* Note that this diagnostic serves for both readdir
         and closedir failures.  */
      error (EXIT_FAILURE, errno, _("reading directory %s"),
             quote (nth_ಪೋಷಕ (ಪೋಷಕ_ಎತ್ತರ)));
    }

  ಆದರೆ ( ! ಸಿಕ್ಕಿತು)
    error (EXIT_FAILURE, 0,
           _("couldn't find directory entry in %s with matching i-node"),
           quote (nth_ಪೋಷಕ (ಪೋಷಕ_ಎತ್ತರ)));

  *dot_sb = ಪೋಷಕ_sb;
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

ಸ್ಥಿರ ಶೂನ್ಯ
robust_getcwd (ರಚನೆ file_name *file_name)
{
  size_t ಎತ್ತರ = 1;
  ರಚನೆ dev_ino dev_ino_buf;
  ರಚನೆ dev_ino *ಬೇರು_dev_ino = get_ಬೇರು_dev_ino (&dev_ino_buf);

  ಆದರೆ (ಬೇರು_dev_ino == ಖಾಲಿ)
    error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
           quoteaf ("/"));

  ರಚನೆ stat dot_sb;
  ಆದರೆ (stat (".", &dot_sb) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf ("."));

  ಆಗ (true)
    {
      /* If we've reached the root, we're done.  */
      ಆದರೆ (PSAME_INODE (&dot_sb, ಬೇರು_dev_ino))
        ನಿಲ್ಲಿಸು;

      ಕಂಡುಹಿಡಿ_dir_ನಮೂದು (&dot_sb, file_name, ಎತ್ತರ++);
    }

  /* See if a leading slash is needed; file_name_prepend adds one.  */
  ಆದರೆ (file_name->ಆರಂಭ[0] == '\0')
    file_name_prepend (file_name, "", 0);
}


/* Return PWD from the environment if it is acceptable for 'pwd -L'
   output, otherwise NULL.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
logical_getcwd (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *wd = getenv ("PWD");

  /* Textual validation first.  */
  ಆದರೆ (!wd || wd[0] != '/')
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = wd;
  ಆಗ ((p = ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (p, "/.")))
    {
      ಆದರೆ (!p[2] || p[2] == '/'
          || (p[2] == '.' && (!p[3] || p[3] == '/')))
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
      p++;
    }

  /* System call validation.  */
  ರಚನೆ stat st1;
  ರಚನೆ stat st2;
  ಆದರೆ (stat (wd, &st1) == 0 && stat (".", &st2) == 0 && psame_inode (&st1, &st2))
    ಹಿಂತಿರುಗಿ wd;
  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}


ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  /* POSIX requires a default of -L, but most scripts expect -P.
     Currently shells default to -L, while stand-alone
     pwd implementations default to -P.  */
  bool logical = (getenv ("POSIXLY_CORRECT") != ಖಾಲಿ);

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv, "LP", longopts, ಖಾಲಿ);
      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'L':
          logical = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'P':
          logical = false;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (optind < argc)
    error (0, 0, _("ignoring non-option arguments"));

  ಆದರೆ (logical)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *wd = logical_getcwd ();
      ಆದರೆ (wd)
        {
          puts (wd);
          ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
        }
    }

  ಅಕ್ಷರ *wd = xgetcwd ();
  ಆದರೆ (wd != ಖಾಲಿ)
    {
      puts (wd);
      ಬಿಡುಗಡೆ (wd);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ರಚನೆ file_name *file_name = file_name_init ();
      robust_getcwd (file_name);
      puts (file_name->ಆರಂಭ);
      file_name_free (file_name);
    }

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
