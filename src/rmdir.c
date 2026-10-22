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
#تعریف_کرو پروگرام_NAME "rmdir"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
ساکن bool remove_خالی_خانے_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
ساکن bool ignore_fail_on_non_خالی_خانے;

/* If true, output a diagnostic for every directory processed.  */
ساکن bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  IGNORE_FAIL_ON_NON_خالی_خانے_OPTION = CHAR_MAX + 1
};

ساکن ساخت option ثابت longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_دلیل, تہی,
   IGNORE_FAIL_ON_NON_خالی_خانے_OPTION},

  {"path", no_دلیل, تہی, 'p'},  /* Deprecated.  */
  {"parents", no_دلیل, تہی, 'p'},
  {"verbose", no_دلیل, تہی, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
ساکن bool
errno_rmdir_non_خالی_خانے (صحیح error_نمبر)
{
  واپس error_نمبر == ENOTEMPTY || error_نمبر == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
ساکن bool
errno_may_be_non_خالی_خانے (صحیح error_نمبر)
{
  انتخاب (error_نمبر)
    {
    حالت EACCES:
    حالت EPERM:
    حالت EROFS:
    حالت EBUSY:
      واپس true;
    طےشدہ:
      واپس false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
ساکن bool
ignorable_failure (صحیح error_نمبر, حرف ثابت *dir)
{
  واپس (ignore_fail_on_non_خالی_خانے
          && (errno_rmdir_non_خالی_خانے (error_نمبر)
              || (errno_may_be_non_خالی_خانے (error_نمبر)
                  && directory_حالت (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

ساکن bool
remove_parents (حرف *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  جب (true)
    {
      حرف *slash = strrchr (dir, '/');
      اگر (slash == تہی)
        توڑو;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      جب (slash > dir && *slash == '/')
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
          ورنہ
            {
              حرف ثابت *error_پیغام;
              اگر (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_پیغام = N_("failed to remove directory %s");
                }
              ورنہ
                {
                  /* A path component could be a symbolic link */
                  error_پیغام = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_پیغام), quoteaf (dir));
            }
          توڑو;
        }
    }
  واپس ok;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... DIRECTORY...\n"), پروگرام_name);
      fputs (_("\
Remove the DIRECTORY(ies), اگر they are خالی_خانے.\n\
\n\
"), stdout);
      oputs (_("\
      --ignore-fail-on-non-خالی_خانے\n\
         ignore each failure مقصد remove a non-خالی_خانے directory\n\
"));
      oputs (_("\
  -p, --parents\n\
         remove DIRECTORY and its ancestors;\n\
         e.g., 'rmdir -p a/b' is similar مقصد 'rmdir a/b a'\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic برائے every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحیح optc;
  جب ((optc = getopt_long (argc, argv, "pv", longopts, تہی)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'p':
          remove_خالی_خانے_parents = true;
          توڑو;
        حالت IGNORE_FAIL_ON_NON_خالی_خانے_OPTION:
          ignore_fail_on_non_خالی_خانے = true;
          توڑو;
        حالت 'v':
          verbose = true;
          توڑو;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  برائے (; optind < argc; ++optind)
    {
      حرف *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      اگر (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      اگر (rmdir (dir) != 0)
        {
          صحیح rmdir_errno = errno;
          اگر (ignorable_failure (rmdir_errno, dir))
            جاری;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          اگر (rmdir_errno == ENOTDIR)
            {
              حرف ثابت *آخری_unix_slash = strrchr (dir, '/');
              اگر (آخری_unix_slash && (*(آخری_unix_slash + 1) == '\0'))
                {
                  ساخت stat st;
                  صحیح ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  اگر ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_منوال)))
                    {
                      /* Ensure the last component was a symlink.  */
                      حرف *dir_arg = xstrdup (dir);
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
      ورنہ اگر (remove_خالی_خانے_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
