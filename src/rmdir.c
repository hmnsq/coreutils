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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "issymlink.h"
#شامل "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rmdir"

#define AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
ایستا bool remove_empty_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
ایستا bool ignore_fail_on_non_empty;

/* If true, output a diagnostic for every directory processed.  */
ایستا bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  IGNORE_FAIL_ON_NON_EMPTY_OPTION = CHAR_MAX + 1
};

ایستا ساختار option ثابت longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_argument, پوچ,
   IGNORE_FAIL_ON_NON_EMPTY_OPTION},

  {"path", no_argument, پوچ, 'p'},  /* Deprecated.  */
  {"parents", no_argument, پوچ, 'p'},
  {"verbose", no_argument, پوچ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
ایستا bool
errno_rmdir_non_empty (صحیح error_عدد)
{
  بازگشت error_عدد == ENOTEMPTY || error_عدد == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
ایستا bool
errno_may_be_non_empty (صحیح error_عدد)
{
  انتخاب (error_عدد)
    {
    حالت EACCES:
    حالت EPERM:
    حالت EROFS:
    حالت EBUSY:
      بازگشت true;
    پیشفرض:
      بازگشت false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
ایستا bool
ignorable_failure (صحیح error_عدد, نویسه ثابت *dir)
{
  بازگشت (ignore_fail_on_non_empty
          && (errno_rmdir_non_empty (error_عدد)
              || (errno_may_be_non_empty (error_عدد)
                  && directory_status (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

ایستا bool
remove_parents (نویسه *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  وقتی (true)
    {
      نویسه *slash = strrchr (dir, '/');
      اگر (slash == پوچ)
        بشکن;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      وقتی (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      اگر (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      صحیح rmdir_errno = errno;

      اگر (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          اگر (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          وگرنه
            {
              نویسه ثابت *error_پیام;
              اگر (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_پیام = N_("failed to remove directory %s");
                }
              وگرنه
                {
                  /* A path component could be a symbolic link */
                  error_پیام = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_پیام), quoteaf (dir));
            }
          بشکن;
        }
    }
  بازگشت ok;
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Remove the DIRECTORY(ies), اگر they are empty.\n\
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
         output a diagnostic برای every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحیح optc;
  وقتی ((optc = getopt_long (argc, argv, "pv", longopts, پوچ)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'p':
          remove_empty_parents = true;
          بشکن;
        حالت IGNORE_FAIL_ON_NON_EMPTY_OPTION:
          ignore_fail_on_non_empty = true;
          بشکن;
        حالت 'v':
          verbose = true;
          بشکن;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  برای (; optind < argc; ++optind)
    {
      نویسه *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      اگر (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      اگر (rmdir (dir) != 0)
        {
          صحیح rmdir_errno = errno;
          اگر (ignorable_failure (rmdir_errno, dir))
            ادامه;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          اگر (rmdir_errno == ENOTDIR)
            {
              نویسه ثابت *last_unix_slash = strrchr (dir, '/');
              اگر (last_unix_slash && (*(last_unix_slash + 1) == '\0'))
                {
                  ساختار stat st;
                  صحیح ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  اگر ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_mode)))
                    {
                      /* Ensure the last component was a symlink.  */
                      نویسه *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      اگر (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      آزاد (dir_arg);
                    }
                }
            }

          اگر (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      وگرنه اگر (remove_empty_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
