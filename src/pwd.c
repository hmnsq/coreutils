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

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "root-dev-ino.h"
#అన్తర్భావ "xgetcwd.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "pwd"

#నిర్వచించు AUTHORS proper_name ("Jim Meyering")

నిర్మాణం file_name
{
  idx_t n_alloc;
  అక్షరం *buf COUNTED_BY (n_alloc);
  అక్షరం *ప్రారంభం;
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"logical", no_వాదం, ఖాళీ, 'L'},
  {"physical", no_వాదం, ఖాళీ, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]...\n"), కార్యక్రమం_name);
      fputs (_("\
Print the నిండిన filename యొక్క the current working directory.\n\
\n\
"), stdout);
      oputs (_("\
  -L, --logical\n\
         use PWD from environment, even ఒకవేళ it కలిగియుంది symlinks\n\
"));
      oputs (_("\
  -P, --physical\n\
         resolve all symlinks\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\n\
If no option is specified, -P is assumed.\n\
"), stdout);
      రాయి (USAGE_BUILTIN_WARNING, కార్యక్రమం_NAME);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

స్థిర శూన్య
file_name_free (నిర్మాణం file_name *p)
{
  విడుదల (p->buf);
  విడుదల (p);
}

స్థిర నిర్మాణం file_name *
file_name_init (శూన్య)
{
  నిర్మాణం file_name *p = xmalloc (పరిమాణం *p);

  /* Start with a buffer larger than likely file names, but beware of systems
     on which PATH_MAX is very large -- e.g., INT_MAX.  */
  పూర్ణాంకం init_alloc = 2 * MIN (PATH_MAX, 16 * 1024);
  p->n_alloc = init_alloc;

  p->buf = xmalloc (init_alloc);
  p->ప్రారంభం = p->buf + init_alloc - 1;
  p->ప్రారంభం[0] = '\0';
  తిరిగి p;
}

/* Prepend the name S of length S_LEN, to the growing file_name, P.  */
స్థిర శూన్య
file_name_prepend (నిర్మాణం file_name *p, అక్షరం స్థిరరాశి *s, size_t s_len)
{
  idx_t n_free = p->ప్రారంభం - p->buf;
  ఒకవేళ (n_free < 1 + s_len)
    {
      /* Call xpalloc with NULL not p->buf, since with the latter
         we'd end up copying the data twice: once via realloc, then again
         to align it with the end of the new buffer.  By passing NULL we
         copy it only once.  */
      idx_t n_వాడిన = p->n_alloc - n_free;
      అక్షరం *buf = xpalloc (ఖాళీ, &p->n_alloc, 1 + s_len - n_free, -1, 1);
      p->ప్రారంభం = జ్ఞాపకం_నకలు (buf + p->n_alloc - n_వాడిన, p->ప్రారంభం, n_వాడిన);
      విడుదల (p->buf);
      p->buf = buf;
    }

  p->ప్రారంభం -= 1 + s_len;
  p->ప్రారంభం[0] = '/';
  జ్ఞాపకం_నకలు (p->ప్రారంభం + 1, s, s_len);
}

/* Return a string (malloc'd) consisting of N '/'-separated ".." components.  */
స్థిర అక్షరం *
nth_మాతృ (size_t n)
{
  అక్షరం *buf = xnmalloc (3, n);
  అక్షరం *p = buf;

  కొరకు (size_t i = 0; i < n; i++)
    {
      జ్ఞాపకం_నకలు (p, "../", 3);
      p += 3;
    }
  p[-1] = '\0';
  తిరిగి buf;
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

స్థిర శూన్య
కనుగొను_dir_నమోదు (నిర్మాణం stat *dot_sb, నిర్మాణం file_name *file_name,
                size_t మాతృ_ఎత్తు)
{
  DIR *dirp = opendir ("..");
  ఒకవేళ (dirp == ఖాళీ)
    error (EXIT_FAILURE, errno, _("cannot open directory %s"),
           quote (nth_మాతృ (మాతృ_ఎత్తు)));

  పూర్ణాంకం fd = dirfd (dirp);
  ఒకవేళ ((0 <= fd ? fchdir (fd) : chdir ("..")) < 0)
    error (EXIT_FAILURE, errno, _("failed to chdir to %s"),
           quote (nth_మాతృ (మాతృ_ఎత్తు)));

  నిర్మాణం stat మాతృ_sb;
  ఒకవేళ ((0 <= fd ? fstat (fd, &మాతృ_sb) : stat (".", &మాతృ_sb)) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"),
           quote (nth_మాతృ (మాతృ_ఎత్తు)));

  /* If parent and child directory are on different devices, then we
     can't rely on d_ino for useful i-node numbers; use lstat instead.  */
  bool use_lstat = (మాతృ_sb.st_dev != dot_sb->st_dev);

  bool దొరికింది = false;
  అయితే (true)
    {
      నిర్మాణం dirent స్థిరరాశి *dp;

      errno = 0;
      ఒకవేళ ((dp = readdir_ignoring_dot_and_dotdot (dirp)) == ఖాళీ)
        {
          ఒకవేళ (errno)
            {
              /* Save/restore errno across closedir call.  */
              పూర్ణాంకం e = errno;
              closedir (dirp);
              errno = e;

              /* Arrange to give a diagnostic after exiting this loop.  */
              dirp = ఖాళీ;
            }
          ఆపు;
        }

      ino_t ino = D_INO (dp);

      నిర్మాణం stat ent_sb;
      ఒకవేళ (ino == NOT_AN_INODE_సంఖ్య || use_lstat)
        {
          ఒకవేళ (lstat (dp->d_name, &ent_sb) < 0)
            {
              /* Skip any entry we can't stat.  */
              కొనసాగు;
            }
          ino = ent_sb.st_ino;
        }

      ఒకవేళ (ino != dot_sb->st_ino)
        కొనసాగు;

      /* If we're not crossing a device boundary, then a simple i-node
         match is enough.  */
      ఒకవేళ ( ! use_lstat || ent_sb.st_dev == dot_sb->st_dev)
        {
          file_name_prepend (file_name, dp->d_name, _D_EXACT_NAMLEN (dp));
          దొరికింది = true;
          ఆపు;
        }
    }

  ఒకవేళ (dirp == ఖాళీ || closedir (dirp) != 0)
    {
      /* Note that this diagnostic serves for both readdir
         and closedir failures.  */
      error (EXIT_FAILURE, errno, _("reading directory %s"),
             quote (nth_మాతృ (మాతృ_ఎత్తు)));
    }

  ఒకవేళ ( ! దొరికింది)
    error (EXIT_FAILURE, 0,
           _("couldn't find directory entry in %s with matching i-node"),
           quote (nth_మాతృ (మాతృ_ఎత్తు)));

  *dot_sb = మాతృ_sb;
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

స్థిర శూన్య
robust_getcwd (నిర్మాణం file_name *file_name)
{
  size_t ఎత్తు = 1;
  నిర్మాణం dev_ino dev_ino_buf;
  నిర్మాణం dev_ino *మూలం_dev_ino = get_మూలం_dev_ino (&dev_ino_buf);

  ఒకవేళ (మూలం_dev_ino == ఖాళీ)
    error (EXIT_FAILURE, errno, _("failed to get attributes of %s"),
           quoteaf ("/"));

  నిర్మాణం stat dot_sb;
  ఒకవేళ (stat (".", &dot_sb) < 0)
    error (EXIT_FAILURE, errno, _("failed to stat %s"), quoteaf ("."));

  అయితే (true)
    {
      /* If we've reached the root, we're done.  */
      ఒకవేళ (PSAME_INODE (&dot_sb, మూలం_dev_ino))
        ఆపు;

      కనుగొను_dir_నమోదు (&dot_sb, file_name, ఎత్తు++);
    }

  /* See if a leading slash is needed; file_name_prepend adds one.  */
  ఒకవేళ (file_name->ప్రారంభం[0] == '\0')
    file_name_prepend (file_name, "", 0);
}


/* Return PWD from the environment if it is acceptable for 'pwd -L'
   output, otherwise NULL.  */
స్థిర అక్షరం స్థిరరాశి *
logical_getcwd (శూన్య)
{
  అక్షరం స్థిరరాశి *wd = getenv ("PWD");

  /* Textual validation first.  */
  ఒకవేళ (!wd || wd[0] != '/')
    తిరిగి ఖాళీ;
  అక్షరం స్థిరరాశి *p = wd;
  అయితే ((p = అక్షరశ్రేణి_శోధన (p, "/.")))
    {
      ఒకవేళ (!p[2] || p[2] == '/'
          || (p[2] == '.' && (!p[3] || p[3] == '/')))
        తిరిగి ఖాళీ;
      p++;
    }

  /* System call validation.  */
  నిర్మాణం stat st1;
  నిర్మాణం stat st2;
  ఒకవేళ (stat (wd, &st1) == 0 && stat (".", &st2) == 0 && psame_inode (&st1, &st2))
    తిరిగి wd;
  తిరిగి ఖాళీ;
}


పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  /* POSIX requires a default of -L, but most scripts expect -P.
     Currently shells default to -L, while stand-alone
     pwd implementations default to -P.  */
  bool logical = (getenv ("POSIXLY_CORRECT") != ఖాళీ);

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే (true)
    {
      పూర్ణాంకం c = getopt_long (argc, argv, "LP", longopts, ఖాళీ);
      ఒకవేళ (c == -1)
        ఆపు;
      ఎంపిక (c)
        {
        స్థితి 'L':
          logical = true;
          ఆపు;
        స్థితి 'P':
          logical = false;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (optind < argc)
    error (0, 0, _("ignoring non-option arguments"));

  ఒకవేళ (logical)
    {
      అక్షరం స్థిరరాశి *wd = logical_getcwd ();
      ఒకవేళ (wd)
        {
          puts (wd);
          తిరిగి EXIT_SUCCESS;
        }
    }

  అక్షరం *wd = xgetcwd ();
  ఒకవేళ (wd != ఖాళీ)
    {
      puts (wd);
      విడుదల (wd);
    }
  లేకపోతే
    {
      నిర్మాణం file_name *file_name = file_name_init ();
      robust_getcwd (file_name);
      puts (file_name->ప్రారంభం);
      file_name_free (file_name);
    }

  తిరిగి EXIT_SUCCESS;
}
