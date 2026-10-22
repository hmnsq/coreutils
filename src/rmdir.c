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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "issymlink.h"
#অন্তর্ভাব "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rmdir"

#define AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
স্থির bool remove_empty_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
স্থির bool ignore_fail_on_non_empty;

/* If true, output a diagnostic for every directory processed.  */
স্থির bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  IGNORE_FAIL_ON_NON_EMPTY_OPTION = CHAR_MAX + 1
};

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_argument, ফাঁকা,
   IGNORE_FAIL_ON_NON_EMPTY_OPTION},

  {"path", no_argument, ফাঁকা, 'p'},  /* Deprecated.  */
  {"parents", no_argument, ফাঁকা, 'p'},
  {"verbose", no_argument, ফাঁকা, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
স্থির bool
errno_rmdir_non_empty (পূর্ণসংখ্যা error_সংখ্যা)
{
  ফেরত error_সংখ্যা == ENOTEMPTY || error_সংখ্যা == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
স্থির bool
errno_may_be_non_empty (পূর্ণসংখ্যা error_সংখ্যা)
{
  নির্বাচন (error_সংখ্যা)
    {
    ক্ষেত্র EACCES:
    ক্ষেত্র EPERM:
    ক্ষেত্র EROFS:
    ক্ষেত্র EBUSY:
      ফেরত true;
    পূর্বনির্ধারিত:
      ফেরত false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
স্থির bool
ignorable_failure (পূর্ণসংখ্যা error_সংখ্যা, অক্ষর ধ্রুবক *dir)
{
  ফেরত (ignore_fail_on_non_empty
          && (errno_rmdir_non_empty (error_সংখ্যা)
              || (errno_may_be_non_empty (error_সংখ্যা)
                  && directory_status (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

স্থির bool
remove_parents (অক্ষর *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  যখন (true)
    {
      অক্ষর *slash = strrchr (dir, '/');
      যদি (slash == ফাঁকা)
        বিরতি;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      যখন (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      যদি (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      পূর্ণসংখ্যা rmdir_errno = errno;

      যদি (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          যদি (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          অন্যথা
            {
              অক্ষর ধ্রুবক *error_বার্তা;
              যদি (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_বার্তা = N_("failed to remove directory %s");
                }
              অন্যথা
                {
                  /* A path component could be a symbolic link */
                  error_বার্তা = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_বার্তা), quoteaf (dir));
            }
          বিরতি;
        }
    }
  ফেরত ok;
}

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Remove the DIRECTORY(ies), যদি they are empty.\n\
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
         output a diagnostic জন্য every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  পূর্ণসংখ্যা optc;
  যখন ((optc = getopt_long (argc, argv, "pv", longopts, ফাঁকা)) != -1)
    {
      নির্বাচন (optc)
        {
        ক্ষেত্র 'p':
          remove_empty_parents = true;
          বিরতি;
        ক্ষেত্র IGNORE_FAIL_ON_NON_EMPTY_OPTION:
          ignore_fail_on_non_empty = true;
          বিরতি;
        ক্ষেত্র 'v':
          verbose = true;
          বিরতি;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  জন্য (; optind < argc; ++optind)
    {
      অক্ষর *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      যদি (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      যদি (rmdir (dir) != 0)
        {
          পূর্ণসংখ্যা rmdir_errno = errno;
          যদি (ignorable_failure (rmdir_errno, dir))
            চালু;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          যদি (rmdir_errno == ENOTDIR)
            {
              অক্ষর ধ্রুবক *last_unix_slash = strrchr (dir, '/');
              যদি (last_unix_slash && (*(last_unix_slash + 1) == '\0'))
                {
                  কাঠামো stat st;
                  পূর্ণসংখ্যা ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  যদি ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_mode)))
                    {
                      /* Ensure the last component was a symlink.  */
                      অক্ষর *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      যদি (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      মুক্ত (dir_arg);
                    }
                }
            }

          যদি (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      অন্যথা যদি (remove_empty_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
