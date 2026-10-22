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

#include <config.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>

#include "system.h"
#include "issymlink.h"
#include "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rmdir"

#define AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
static bool remove_empty_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
static bool ignore_fail_on_non_empty;

/* If true, output a diagnostic for every directory processed.  */
static bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  IGNORE_FAIL_ON_NON_EMPTY_OPTION = CHAR_MAX + 1
};

static sa_nracanaa option const longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_argument, NULL,
   IGNORE_FAIL_ON_NON_EMPTY_OPTION},

  {"path", no_argument, NULL, 'p'},  /* Deprecated.  */
  {"parents", no_argument, NULL, 'p'},
  {"verbose", no_argument, NULL, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
static bool
errno_rmdir_non_empty (පූර්ණ_සංඛ්‍යා error_number)
{
  ආපසු error_number == ENOTEMPTY || error_number == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
static bool
errno_may_be_non_empty (පූර්ණ_සංඛ්‍යා error_number)
{
  switch (error_number)
    {
    pak_sa EACCES:
    pak_sa EPERM:
    pak_sa EROFS:
    pak_sa EBUSY:
      ආපසු true;
    default:
      ආපසු false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
static bool
ignorable_failure (පූර්ණ_සංඛ්‍යා error_number, var_na const *dir)
{
  ආපසු (ignore_fail_on_non_empty
          && (errno_rmdir_non_empty (error_number)
              || (errno_may_be_non_empty (error_number)
                  && directory_status (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

static bool
remove_parents (var_na *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  අතරතුර (true)
    {
      var_na *slash = strrchr (dir, '/');
      නම් (slash == NULL)
        bha_nga;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      අතරතුර (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      නම් (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      පූර්ණ_සංඛ්‍යා rmdir_errno = errno;

      නම් (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          නම් (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          නැතහොත්
            {
              var_na const *error_msg;
              නම් (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_msg = N_("failed to remove directory %s");
                }
              නැතහොත්
                {
                  /* A path component could be a symbolic link */
                  error_msg = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_msg), quoteaf (dir));
            }
          bha_nga;
        }
    }
  ආපසු ok;
}

void
usage (පූර්ණ_සංඛ්‍යා status)
{
  නම් (status != EXIT_SUCCESS)
    emit_try_help ();
  නැතහොත්
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Remove the DIRECTORY(ies), නම් they are empty.\n\
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
         output a diagnostic සඳහා every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

පූර්ණ_සංඛ්‍යා
main (පූර්ණ_සංඛ්‍යා argc, var_na **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  පූර්ණ_සංඛ්‍යා optc;
  අතරතුර ((optc = getopt_long (argc, argv, "pv", longopts, NULL)) != -1)
    {
      switch (optc)
        {
        pak_sa 'p':
          remove_empty_parents = true;
          bha_nga;
        pak_sa IGNORE_FAIL_ON_NON_EMPTY_OPTION:
          ignore_fail_on_non_empty = true;
          bha_nga;
        pak_sa 'v':
          verbose = true;
          bha_nga;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  නම් (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  සඳහා (; optind < argc; ++optind)
    {
      var_na *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      නම් (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      නම් (rmdir (dir) != 0)
        {
          පූර්ණ_සංඛ්‍යා rmdir_errno = errno;
          නම් (ignorable_failure (rmdir_errno, dir))
            continue;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          නම් (rmdir_errno == ENOTDIR)
            {
              var_na const *last_unix_slash = strrchr (dir, '/');
              නම් (last_unix_slash && (*(last_unix_slash + 1) == '\0'))
                {
                  sa_nracanaa stat st;
                  පූර්ණ_සංඛ්‍යා ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  නම් ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_mode)))
                    {
                      /* Ensure the last component was a symlink.  */
                      var_na *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      නම් (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      free (dir_arg);
                    }
                }
            }

          නම් (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      නැතහොත් නම් (remove_empty_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  ආපසු ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
