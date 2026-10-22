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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "root-dev-ino.h"
#અન્તર્ભાવ "xgetcwd.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "pwd"

#define AUTHORS proper_name ("Jim Meyering")

સંરચના file_name
{
  idx_t n_alloc;
  અક્ષર *buf COUNTED_BY (n_alloc);
  અક્ષર *start;
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"logical", no_argument, ખાલી, 'L'},
  {"physical", no_argument, ખાલી, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);
      fputs (_("\
Print the full filename of the current working directory.\n\
\n\
"), stdout);
      oputs (_("\
  -L, --logical\n\
         use PWD from environment, even જો it contains symlinks\n\
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

સ્થિર શૂન્ય
file_name_free (સંરચના file_name *p)
{
  મુક્ત (p->buf);
  મુક્ત (p);
}

સ્થિર સંરચના file_name *
file_name_init (શૂન્ય)
{
  સંરચના file_name *p = xmalloc (કદ *p);

  /* Start with a buffer larger than likely file names, but beware of systems
     on which PATH_MAX is very large -- e.g., INT_MAX.  */
  પૂર્ણાંક init_alloc = 2 * MIN (PATH_MAX, 16 * 1024);
  p->n_alloc = init_alloc;

  p->buf = xmalloc (init_alloc);
  p->start = p->buf + init_alloc - 1;
  p->start[0] = '\0';
  પાછા p;
}

/* Prepend the name S of length S_LEN, to the growing file_name, P.  */
સ્થિર શૂન્ય
file_name_prepend (સંરચના file_name *p, અક્ષર અચલ *s, size_t s_len)
{
  idx_t n_free = p->start - p->buf;
  જો (n_free < 1 + s_len)
    {
      /* Call xpalloc with NULL not p->buf, since with the latter
         we'd end up copying the data twice: once via realloc, then again
         to align it with the end of the new buffer.  By passing NULL we
         copy it only once.  */
      idx_t n_used = p->n_alloc - n_free;
      અક્ષર *buf = xpalloc (ખાલી, &p->n_alloc, 1 + s_len - n_free, -1, 1);
      p->start = memcpy (buf + p->n_alloc - n_used, p->start, n_used);
      મુક્ત (p->buf);
      p->buf = buf;
    }

  p->start -= 1 + s_len;
  p->start[0] = '/';
  memcpy (p->start + 1, s, s_len);
}

/* Return a string (malloc'd) consisting of N '/'-separated ".." components.  */
સ્થિર અક્ષર *
nth_parent (size_t n)
{
  અક્ષર *buf = xnmalloc (3, n);
  અક્ષર *p = buf;

  માટે (size_t i = 0; i < n; i++)
    {
      memcpy (p, "../", 3);
      p += 3;
    }
  p[-1] = '\0';
  પાછા buf;
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

સ્થિર શૂન્ય
find_dir_entry (સંરચના stat *dot_sb, સંરચના file_name *file_name,
                size_t parent_height)
{
  DIR *dirp = opendir ("..");
  જો (dirp == ખાલી)
    error (EXIT_FAILURE, errno, _("cannot open directory %s"),
           quote (nth_parent (parent_height)));

  પૂર્ણાંક fd = dirfd (dirp);
  જો ((0 <= fd ? fchdir (fd) : chdir ("..")) < 0)
    error (EXIT_FAILURE, errno, _("failed to chdir to %s"),
           quote (nth_parent (parent_height)));

  સંરચના stat parent_sb;
  જો ((0 <= fd ? fstat (fd, &parent_sb) : stat (".", &parent_sb)) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"),
           quote (nth_parent (parent_height)));

  /* If parent and child directory are on different devices, then we
     can't rely on d_ino for useful i-node numbers; use lstat instead.  */
  bool use_lstat = (parent_sb.st_dev != dot_sb->st_dev);

  bool found = false;
  જ્યારે (true)
    {
      સંરચના dirent અચલ *dp;

      errno = 0;
      જો ((dp = readdir_ignoring_dot_and_dotdot (dirp)) == ખાલી)
        {
          જો (errno)
            {
              /* Save/restore errno across closedir call.  */
              પૂર્ણાંક e = errno;
              closedir (dirp);
              errno = e;

              /* Arrange to give a diagnostic after exiting this loop.  */
              dirp = ખાલી;
            }
          તોડો;
        }

      ino_t ino = D_INO (dp);

      સંરચના stat ent_sb;
      જો (ino == NOT_AN_INODE_સંખ્યા || use_lstat)
        {
          જો (lstat (dp->d_name, &ent_sb) < 0)
            {
              /* Skip any entry we can't stat.  */
              ચાલુ;
            }
          ino = ent_sb.st_ino;
        }

      જો (ino != dot_sb->st_ino)
        ચાલુ;

      /* If we're not crossing a device boundary, then a simple i-node
         match is enough.  */
      જો ( ! use_lstat || ent_sb.st_dev == dot_sb->st_dev)
        {
          file_name_prepend (file_name, dp->d_name, _D_EXACT_NAMLEN (dp));
          found = true;
          તોડો;
        }
    }

  જો (dirp == ખાલી || closedir (dirp) != 0)
    {
      /* Note that this diagnostic serves for both readdir
         and closedir failures.  */
      error (EXIT_FAILURE, errno, _("reading directory %s"),
             quote (nth_parent (parent_height)));
    }

  જો ( ! found)
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

સ્થિર શૂન્ય
robust_getcwd (સંરચના file_name *file_name)
{
  size_t height = 1;
  સંરચના dev_ino dev_ino_buf;
  સંરચના dev_ino *મૂળ_dev_ino = get_મૂળ_dev_ino (&dev_ino_buf);

  જો (મૂળ_dev_ino == ખાલી)
    error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
           quoteaf ("/"));

  સંરચના stat dot_sb;
  જો (stat (".", &dot_sb) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf ("."));

  જ્યારે (true)
    {
      /* If we've reached the root, we're done.  */
      જો (PSAME_INODE (&dot_sb, મૂળ_dev_ino))
        તોડો;

      find_dir_entry (&dot_sb, file_name, height++);
    }

  /* See if a leading slash is needed; file_name_prepend adds one.  */
  જો (file_name->start[0] == '\0')
    file_name_prepend (file_name, "", 0);
}


/* Return PWD from the environment if it is acceptable for 'pwd -L'
   output, otherwise NULL.  */
સ્થિર અક્ષર અચલ *
logical_getcwd (શૂન્ય)
{
  અક્ષર અચલ *wd = getenv ("PWD");

  /* Textual validation first.  */
  જો (!wd || wd[0] != '/')
    પાછા ખાલી;
  અક્ષર અચલ *p = wd;
  જ્યારે ((p = strstr (p, "/.")))
    {
      જો (!p[2] || p[2] == '/'
          || (p[2] == '.' && (!p[3] || p[3] == '/')))
        પાછા ખાલી;
      p++;
    }

  /* System call validation.  */
  સંરચના stat st1;
  સંરચના stat st2;
  જો (stat (wd, &st1) == 0 && stat (".", &st2) == 0 && psame_inode (&st1, &st2))
    પાછા wd;
  પાછા ખાલી;
}


પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  /* POSIX requires a default of -L, but most scripts expect -P.
     Currently shells default to -L, while stand-alone
     pwd implementations default to -P.  */
  bool logical = (getenv ("POSIXLY_CORRECT") != ખાલી);

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે (true)
    {
      પૂર્ણાંક c = getopt_long (argc, argv, "LP", longopts, ખાલી);
      જો (c == -1)
        તોડો;
      પસંદગી (c)
        {
        સ્થિતિ 'L':
          logical = true;
          તોડો;
        સ્થિતિ 'P':
          logical = false;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (optind < argc)
    error (0, 0, _("ignoring non-option arguments"));

  જો (logical)
    {
      અક્ષર અચલ *wd = logical_getcwd ();
      જો (wd)
        {
          puts (wd);
          પાછા EXIT_SUCCESS;
        }
    }

  અક્ષર *wd = xgetcwd ();
  જો (wd != ખાલી)
    {
      puts (wd);
      મુક્ત (wd);
    }
  નહીંતર
    {
      સંરચના file_name *file_name = file_name_init ();
      robust_getcwd (file_name);
      puts (file_name->start);
      file_name_free (file_name);
    }

  પાછા EXIT_SUCCESS;
}
