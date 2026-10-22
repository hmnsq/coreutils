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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "issymlink.h"
#ಅನ್ತರ್ಭಾವ "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "rmdir"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
ಸ್ಥಿರ bool remove_ಖಾಲಿಗಳು_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
ಸ್ಥಿರ bool ignore_fail_on_non_ಖಾಲಿಗಳು;

/* If true, output a diagnostic for every directory processed.  */
ಸ್ಥಿರ bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  IGNORE_FAIL_ON_NON_ಖಾಲಿಗಳು_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_ವಾದ, ಖಾಲಿ,
   IGNORE_FAIL_ON_NON_ಖಾಲಿಗಳು_OPTION},

  {"path", no_ವಾದ, ಖಾಲಿ, 'p'},  /* Deprecated.  */
  {"parents", no_ವಾದ, ಖಾಲಿ, 'p'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
ಸ್ಥಿರ bool
errno_rmdir_non_ಖಾಲಿಗಳು (ಪೂರ್ಣಾಂಕ error_ಸಂಖ್ಯೆ)
{
  ಹಿಂತಿರುಗಿ error_ಸಂಖ್ಯೆ == ENOTEMPTY || error_ಸಂಖ್ಯೆ == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
ಸ್ಥಿರ bool
errno_may_be_non_ಖಾಲಿಗಳು (ಪೂರ್ಣಾಂಕ error_ಸಂಖ್ಯೆ)
{
  ಆಯ್ಕೆ (error_ಸಂಖ್ಯೆ)
    {
    ಸ್ಥಿತಿ EACCES:
    ಸ್ಥಿತಿ EPERM:
    ಸ್ಥಿತಿ EROFS:
    ಸ್ಥಿತಿ EBUSY:
      ಹಿಂತಿರುಗಿ true;
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
ಸ್ಥಿರ bool
ignorable_failure (ಪೂರ್ಣಾಂಕ error_ಸಂಖ್ಯೆ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dir)
{
  ಹಿಂತಿರುಗಿ (ignore_fail_on_non_ಖಾಲಿಗಳು
          && (errno_rmdir_non_ಖಾಲಿಗಳು (error_ಸಂಖ್ಯೆ)
              || (errno_may_be_non_ಖಾಲಿಗಳು (error_ಸಂಖ್ಯೆ)
                  && directory_ಸ್ಥಿತಿ (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

ಸ್ಥಿರ bool
remove_parents (ಅಕ್ಷರ *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  ಆಗ (true)
    {
      ಅಕ್ಷರ *slash = strrchr (dir, '/');
      ಆದರೆ (slash == ಖಾಲಿ)
        ನಿಲ್ಲಿಸು;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      ಆಗ (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      ಆದರೆ (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      ಪೂರ್ಣಾಂಕ rmdir_errno = errno;

      ಆದರೆ (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          ಆದರೆ (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *error_ಸಂದೇಶ;
              ಆದರೆ (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_ಸಂದೇಶ = N_("failed to remove directory %s");
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  /* A path component could be a symbolic link */
                  error_ಸಂದೇಶ = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_ಸಂದೇಶ), quoteaf (dir));
            }
          ನಿಲ್ಲಿಸು;
        }
    }
  ಹಿಂತಿರುಗಿ ok;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... DIRECTORY...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Remove the DIRECTORY(ies), ಆದರೆ they are ಖಾಲಿಗಳು.\n\
\n\
"), stdout);
      oputs (_("\
      --ignore-fail-on-non-ಖಾಲಿಗಳು\n\
         ignore each failure ಗಮ್ಯ remove a non-ಖಾಲಿಗಳು directory\n\
"));
      oputs (_("\
  -p, --parents\n\
         remove DIRECTORY and its ancestors;\n\
         e.g., 'rmdir -p a/b' is similar ಗಮ್ಯ 'rmdir a/b a'\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic ಗಾಗಿ every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಂಕ optc;
  ಆಗ ((optc = getopt_long (argc, argv, "pv", longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'p':
          remove_ಖಾಲಿಗಳು_parents = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ IGNORE_FAIL_ON_NON_ಖಾಲಿಗಳು_OPTION:
          ignore_fail_on_non_ಖಾಲಿಗಳು = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'v':
          verbose = true;
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  ಗಾಗಿ (; optind < argc; ++optind)
    {
      ಅಕ್ಷರ *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      ಆದರೆ (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ಆದರೆ (rmdir (dir) != 0)
        {
          ಪೂರ್ಣಾಂಕ rmdir_errno = errno;
          ಆದರೆ (ignorable_failure (rmdir_errno, dir))
            ಮುಂದುವರಿ;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          ಆದರೆ (rmdir_errno == ENOTDIR)
            {
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಕೊನೆಯ_unix_slash = strrchr (dir, '/');
              ಆದರೆ (ಕೊನೆಯ_unix_slash && (*(ಕೊನೆಯ_unix_slash + 1) == '\0'))
                {
                  ರಚನೆ stat st;
                  ಪೂರ್ಣಾಂಕ ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  ಆದರೆ ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_ಬಹುಲಕ)))
                    {
                      /* Ensure the last component was a symlink.  */
                      ಅಕ್ಷರ *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      ಆದರೆ (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      ಬಿಡುಗಡೆ (dir_arg);
                    }
                }
            }

          ಆದರೆ (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (remove_ಖಾಲಿಗಳು_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
