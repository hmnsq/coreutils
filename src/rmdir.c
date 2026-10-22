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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "issymlink.h"
#ضمن "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rmdir"

#define AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
ساكن bool remove_empty_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
ساكن bool ignore_fail_on_non_empty;

/* If true, output a diagnostic for every directory processed.  */
ساكن bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  IGNORE_FAIL_ON_NON_EMPTY_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_argument, عدم,
   IGNORE_FAIL_ON_NON_EMPTY_OPTION},

  {"path", no_argument, عدم, 'p'},  /* Deprecated.  */
  {"parents", no_argument, عدم, 'p'},
  {"verbose", no_argument, عدم, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
ساكن bool
errno_rmdir_non_empty (صحيح error_ژمار)
{
  أرجع error_ژمار == ENOTEMPTY || error_ژمار == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
ساكن bool
errno_may_be_non_empty (صحيح error_ژمار)
{
  بدل (error_ژمار)
    {
    حالة EACCES:
    حالة EPERM:
    حالة EROFS:
    حالة EBUSY:
      أرجع true;
    افتراضي:
      أرجع false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
ساكن bool
ignorable_failure (صحيح error_ژمار, حرف ثابت *dir)
{
  أرجع (ignore_fail_on_non_empty
          && (errno_rmdir_non_empty (error_ژمار)
              || (errno_may_be_non_empty (error_ژمار)
                  && directory_status (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

ساكن bool
remove_parents (حرف *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  طالما (true)
    {
      حرف *slash = strrchr (dir, '/');
      إذا (slash == عدم)
        اكسر;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      طالما (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      إذا (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      صحيح rmdir_errno = errno;

      إذا (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          إذا (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          وإلا
            {
              حرف ثابت *error_پەیام;
              إذا (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_پەیام = N_("failed to remove directory %s");
                }
              وإلا
                {
                  /* A path component could be a symbolic link */
                  error_پەیام = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_پەیام), quoteaf (dir));
            }
          اكسر;
        }
    }
  أرجع ok;
}

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Remove the DIRECTORY(ies), إذا they are empty.\n\
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
         output a diagnostic لكل every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحيح
main (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحيح optc;
  طالما ((optc = getopt_long (argc, argv, "pv", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'p':
          remove_empty_parents = true;
          اكسر;
        حالة IGNORE_FAIL_ON_NON_EMPTY_OPTION:
          ignore_fail_on_non_empty = true;
          اكسر;
        حالة 'v':
          verbose = true;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  لكل (; optind < argc; ++optind)
    {
      حرف *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      إذا (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      إذا (rmdir (dir) != 0)
        {
          صحيح rmdir_errno = errno;
          إذا (ignorable_failure (rmdir_errno, dir))
            استمر;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          إذا (rmdir_errno == ENOTDIR)
            {
              حرف ثابت *last_unix_slash = strrchr (dir, '/');
              إذا (last_unix_slash && (*(last_unix_slash + 1) == '\0'))
                {
                  بنية stat st;
                  صحيح ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  إذا ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_mode)))
                    {
                      /* Ensure the last component was a symlink.  */
                      حرف *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      إذا (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      حرر (dir_arg);
                    }
                }
            }

          إذا (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      وإلا إذا (remove_empty_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
