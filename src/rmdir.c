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
#define PROGRAM_NAME "rmdir"

#define AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
ಸ್ಥಿರ bool remove_empty_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
ಸ್ಥಿರ bool ignore_fail_on_non_empty;

/* If true, output a diagnostic for every directory processed.  */
ಸ್ಥಿರ bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  IGNORE_FAIL_ON_NON_EMPTY_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_argument, ರಿಕ್ತ,
   IGNORE_FAIL_ON_NON_EMPTY_OPTION},

  {"path", no_argument, ರಿಕ್ತ, 'p'},  /* Deprecated.  */
  {"parents", no_argument, ರಿಕ್ತ, 'p'},
  {"verbose", no_argument, ರಿಕ್ತ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
ಸ್ಥಿರ bool
errno_rmdir_non_empty (ಪೂರ್ಣಾಙ್ಕ error_ಸಂಖ್ಯೆ)
{
  ಪ್ರತ್ಯಾಗಮ error_ಸಂಖ್ಯೆ == ENOTEMPTY || error_ಸಂಖ್ಯೆ == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
ಸ್ಥಿರ bool
errno_may_be_non_empty (ಪೂರ್ಣಾಙ್ಕ error_ಸಂಖ್ಯೆ)
{
  ವಿಕಲ್ಪ (error_ಸಂಖ್ಯೆ)
    {
    ಪಕ್ಷ EACCES:
    ಪಕ್ಷ EPERM:
    ಪಕ್ಷ EROFS:
    ಪಕ್ಷ EBUSY:
      ಪ್ರತ್ಯಾಗಮ true;
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
ಸ್ಥಿರ bool
ignorable_failure (ಪೂರ್ಣಾಙ್ಕ error_ಸಂಖ್ಯೆ, ವರ್ಣ ಧ್ರುವ *dir)
{
  ಪ್ರತ್ಯಾಗಮ (ignore_fail_on_non_empty
          && (errno_rmdir_non_empty (error_ಸಂಖ್ಯೆ)
              || (errno_may_be_non_empty (error_ಸಂಖ್ಯೆ)
                  && directory_status (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

ಸ್ಥಿರ bool
remove_parents (ವರ್ಣ *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  ಯಾವತ್ (true)
    {
      ವರ್ಣ *slash = strrchr (dir, '/');
      ಯದಿ (slash == ರಿಕ್ತ)
        ಭಙ್ಗ;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      ಯಾವತ್ (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      ಯದಿ (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      ಪೂರ್ಣಾಙ್ಕ rmdir_errno = errno;

      ಯದಿ (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          ಯದಿ (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          ಅನ್ಯಥಾ
            {
              ವರ್ಣ ಧ್ರುವ *error_ಸಂದೇಶ;
              ಯದಿ (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_ಸಂದೇಶ = N_("failed to remove directory %s");
                }
              ಅನ್ಯಥಾ
                {
                  /* A path component could be a symbolic link */
                  error_ಸಂದೇಶ = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_ಸಂದೇಶ), quoteaf (dir));
            }
          ಭಙ್ಗ;
        }
    }
  ಪ್ರತ್ಯಾಗಮ ok;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Remove the DIRECTORY(ies), ಯದಿ they are empty.\n\
\n\
"), stdout);
      oputs (_("\
      --ignore-fail-on-non-empty\n\
         ignore each failure to remove a non-empty directory\n\
"));
      oputs (_("\
  -p, --parents\n\
         remove DIRECTORY and its ancestors;\n\
         e.g., 'rmdir -p a/b' is similar to 'rmdir a/b a'\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic ಪ್ರತಿ every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಙ್ಕ optc;
  ಯಾವತ್ ((optc = getopt_long (argc, argv, "pv", longopts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'p':
          remove_empty_parents = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ IGNORE_FAIL_ON_NON_EMPTY_OPTION:
          ignore_fail_on_non_empty = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'v':
          verbose = true;
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  ಪ್ರತಿ (; optind < argc; ++optind)
    {
      ವರ್ಣ *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      ಯದಿ (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ಯದಿ (rmdir (dir) != 0)
        {
          ಪೂರ್ಣಾಙ್ಕ rmdir_errno = errno;
          ಯದಿ (ignorable_failure (rmdir_errno, dir))
            ಅನುವರ್ತ;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          ಯದಿ (rmdir_errno == ENOTDIR)
            {
              ವರ್ಣ ಧ್ರುವ *last_unix_slash = strrchr (dir, '/');
              ಯದಿ (last_unix_slash && (*(last_unix_slash + 1) == '\0'))
                {
                  ಸಂರಚನಾ stat st;
                  ಪೂರ್ಣಾಙ್ಕ ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  ಯದಿ ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_mode)))
                    {
                      /* Ensure the last component was a symlink.  */
                      ವರ್ಣ *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      ಯದಿ (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      ಮುಕ್ತ (dir_arg);
                    }
                }
            }

          ಯದಿ (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      ಅನ್ಯಥಾ ಯದಿ (remove_empty_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
