/* rmdir -- remove directories

   Copyright (C) 1990-2026 Free Software Foundation, Inc.

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

/* Options:
   -p, --parent		Remove any parent dirs that are explicitly mentioned
                        in an argument, if they become empty after the
                        argument file is removed.

   David MacKenzie <djm@ai.mit.edu>  */

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "issymlink.h"
#అన్తర్భావ "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "rmdir"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
స్థిర bool remove_ఖాళీలు_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
స్థిర bool ignore_fail_on_non_ఖాళీలు;

/* If true, output a diagnostic for every directory processed.  */
స్థిర bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  IGNORE_FAIL_ON_NON_ఖాళీలు_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_వాదం, ఖాళీ,
   IGNORE_FAIL_ON_NON_ఖాళీలు_OPTION},

  {"path", no_వాదం, ఖాళీ, 'p'},  /* Deprecated.  */
  {"parents", no_వాదం, ఖాళీ, 'p'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
స్థిర bool
errno_rmdir_non_ఖాళీలు (పూర్ణాంకం error_సంఖ్య)
{
  తిరిగి error_సంఖ్య == ENOTEMPTY || error_సంఖ్య == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
స్థిర bool
errno_may_be_non_ఖాళీలు (పూర్ణాంకం error_సంఖ్య)
{
  ఎంపిక (error_సంఖ్య)
    {
    స్థితి EACCES:
    స్థితి EPERM:
    స్థితి EROFS:
    స్థితి EBUSY:
      తిరిగి true;
    అప్రమేయ:
      తిరిగి false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
స్థిర bool
ignorable_failure (పూర్ణాంకం error_సంఖ్య, అక్షరం స్థిరరాశి *dir)
{
  తిరిగి (ignore_fail_on_non_ఖాళీలు
          && (errno_rmdir_non_ఖాళీలు (error_సంఖ్య)
              || (errno_may_be_non_ఖాళీలు (error_సంఖ్య)
                  && directory_స్థితి (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

స్థిర bool
remove_parents (అక్షరం *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  అయితే (true)
    {
      అక్షరం *slash = strrchr (dir, '/');
      ఒకవేళ (slash == ఖాళీ)
        ఆపు;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      అయితే (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      ఒకవేళ (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      పూర్ణాంకం rmdir_errno = errno;

      ఒకవేళ (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          ఒకవేళ (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          లేకపోతే
            {
              అక్షరం స్థిరరాశి *error_సందేశం;
              ఒకవేళ (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_సందేశం = N_("failed to remove directory %s");
                }
              లేకపోతే
                {
                  /* A path component could be a symbolic link */
                  error_సందేశం = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_సందేశం), quoteaf (dir));
            }
          ఆపు;
        }
    }
  తిరిగి ok;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... DIRECTORY...\n"), కార్యక్రమం_name);
      fputs (_("\
Remove the DIRECTORY(ies), ఒకవేళ they are ఖాళీలు.\n\
\n\
"), stdout);
      oputs (_("\
      --ignore-fail-on-non-ఖాళీలు\n\
         ignore each failure గమ్యం remove a non-ఖాళీలు directory\n\
"));
      oputs (_("\
  -p, --parents\n\
         remove DIRECTORY and its ancestors;\n\
         e.g., 'rmdir -p a/b' is similar గమ్యం 'rmdir a/b a'\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic కొరకు every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  పూర్ణాంకం optc;
  అయితే ((optc = getopt_long (argc, argv, "pv", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'p':
          remove_ఖాళీలు_parents = true;
          ఆపు;
        స్థితి IGNORE_FAIL_ON_NON_ఖాళీలు_OPTION:
          ignore_fail_on_non_ఖాళీలు = true;
          ఆపు;
        స్థితి 'v':
          verbose = true;
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  కొరకు (; optind < argc; ++optind)
    {
      అక్షరం *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      ఒకవేళ (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ఒకవేళ (rmdir (dir) != 0)
        {
          పూర్ణాంకం rmdir_errno = errno;
          ఒకవేళ (ignorable_failure (rmdir_errno, dir))
            కొనసాగు;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          ఒకవేళ (rmdir_errno == ENOTDIR)
            {
              అక్షరం స్థిరరాశి *చివరి_unix_slash = strrchr (dir, '/');
              ఒకవేళ (చివరి_unix_slash && (*(చివరి_unix_slash + 1) == '\0'))
                {
                  నిర్మాణం stat st;
                  పూర్ణాంకం ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  ఒకవేళ ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_బహుళకం)))
                    {
                      /* Ensure the last component was a symlink.  */
                      అక్షరం *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      ఒకవేళ (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      విడుదల (dir_arg);
                    }
                }
            }

          ఒకవేళ (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      లేకపోతే ఒకవేళ (remove_ఖాళీలు_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
