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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "issymlink.h"
#അന്തര്ഭാവ "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rmdir"

#define AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
സ്ഥിതം bool remove_empty_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
സ്ഥിതം bool ignore_fail_on_non_empty;

/* If true, output a diagnostic for every directory processed.  */
സ്ഥിതം bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  IGNORE_FAIL_ON_NON_EMPTY_OPTION = CHAR_MAX + 1
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_argument, ശൂന്യ,
   IGNORE_FAIL_ON_NON_EMPTY_OPTION},

  {"path", no_argument, ശൂന്യ, 'p'},  /* Deprecated.  */
  {"parents", no_argument, ശൂന്യ, 'p'},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
സ്ഥിതം bool
errno_rmdir_non_empty (പൂർണ്ണസംഖ്യ error_സംഖ്യ)
{
  തിരികെ error_സംഖ്യ == ENOTEMPTY || error_സംഖ്യ == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
സ്ഥിതം bool
errno_may_be_non_empty (പൂർണ്ണസംഖ്യ error_സംഖ്യ)
{
  തിരഞ്ഞെടുപ്പ് (error_സംഖ്യ)
    {
    അവസ്ഥ EACCES:
    അവസ്ഥ EPERM:
    അവസ്ഥ EROFS:
    അവസ്ഥ EBUSY:
      തിരികെ true;
    സ്ഥിരസ്ഥിതി:
      തിരികെ false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
സ്ഥിതം bool
ignorable_failure (പൂർണ്ണസംഖ്യ error_സംഖ്യ, അക്ഷരം സ്ഥിരം *dir)
{
  തിരികെ (ignore_fail_on_non_empty
          && (errno_rmdir_non_empty (error_സംഖ്യ)
              || (errno_may_be_non_empty (error_സംഖ്യ)
                  && directory_status (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

സ്ഥിതം bool
remove_parents (അക്ഷരം *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  ആകുമ്പോൾ (true)
    {
      അക്ഷരം *slash = strrchr (dir, '/');
      എങ്കിൽ (slash == ശൂന്യ)
        നിർത്തുക;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      ആകുമ്പോൾ (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      എങ്കിൽ (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      പൂർണ്ണസംഖ്യ rmdir_errno = errno;

      എങ്കിൽ (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          എങ്കിൽ (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          അല്ലെങ്കിൽ
            {
              അക്ഷരം സ്ഥിരം *error_സന്ദേശം;
              എങ്കിൽ (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_സന്ദേശം = N_("failed to remove directory %s");
                }
              അല്ലെങ്കിൽ
                {
                  /* A path component could be a symbolic link */
                  error_സന്ദേശം = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_സന്ദേശം), quoteaf (dir));
            }
          നിർത്തുക;
        }
    }
  തിരികെ ok;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Remove the DIRECTORY(ies), എങ്കിൽ they are empty.\n\
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
         output a diagnostic വേണ്ടി every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  പൂർണ്ണസംഖ്യ optc;
  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "pv", longopts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'p':
          remove_empty_parents = true;
          നിർത്തുക;
        അവസ്ഥ IGNORE_FAIL_ON_NON_EMPTY_OPTION:
          ignore_fail_on_non_empty = true;
          നിർത്തുക;
        അവസ്ഥ 'v':
          verbose = true;
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  വേണ്ടി (; optind < argc; ++optind)
    {
      അക്ഷരം *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      എങ്കിൽ (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      എങ്കിൽ (rmdir (dir) != 0)
        {
          പൂർണ്ണസംഖ്യ rmdir_errno = errno;
          എങ്കിൽ (ignorable_failure (rmdir_errno, dir))
            തുടരുക;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          എങ്കിൽ (rmdir_errno == ENOTDIR)
            {
              അക്ഷരം സ്ഥിരം *last_unix_slash = strrchr (dir, '/');
              എങ്കിൽ (last_unix_slash && (*(last_unix_slash + 1) == '\0'))
                {
                  ഘടന stat st;
                  പൂർണ്ണസംഖ്യ ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  എങ്കിൽ ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_mode)))
                    {
                      /* Ensure the last component was a symlink.  */
                      അക്ഷരം *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      എങ്കിൽ (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      സ്വതന്ത്രം (dir_arg);
                    }
                }
            }

          എങ്കിൽ (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (remove_empty_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
