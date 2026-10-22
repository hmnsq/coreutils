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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "issymlink.h"
#अन्तर्भाव "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rmdir"

#define AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
स्थिर bool remove_empty_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
स्थिर bool ignore_fail_on_non_empty;

/* If true, output a diagnostic for every directory processed.  */
स्थिर bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  IGNORE_FAIL_ON_NON_EMPTY_OPTION = CHAR_MAX + 1
};

स्थिर संरचना option ध्रुव longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_argument, रिक्त,
   IGNORE_FAIL_ON_NON_EMPTY_OPTION},

  {"path", no_argument, रिक्त, 'p'},  /* Deprecated.  */
  {"parents", no_argument, रिक्त, 'p'},
  {"verbose", no_argument, रिक्त, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
स्थिर bool
errno_rmdir_non_empty (पूर्णाङ्क error_सङ्ख्या)
{
  पटिनिवत्त error_सङ्ख्या == ENOTEMPTY || error_सङ्ख्या == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
स्थिर bool
errno_may_be_non_empty (पूर्णाङ्क error_सङ्ख्या)
{
  विकल्प (error_सङ्ख्या)
    {
    पक्ष EACCES:
    पक्ष EPERM:
    पक्ष EROFS:
    पक्ष EBUSY:
      पटिनिवत्त true;
    स्वभाव:
      पटिनिवत्त false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
स्थिर bool
ignorable_failure (पूर्णाङ्क error_सङ्ख्या, वर्ण ध्रुव *dir)
{
  पटिनिवत्त (ignore_fail_on_non_empty
          && (errno_rmdir_non_empty (error_सङ्ख्या)
              || (errno_may_be_non_empty (error_सङ्ख्या)
                  && directory_status (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

स्थिर bool
remove_parents (वर्ण *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  याव (true)
    {
      वर्ण *slash = strrchr (dir, '/');
      सचे (slash == रिक्त)
        भङ्ग;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      याव (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      सचे (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      पूर्णाङ्क rmdir_errno = errno;

      सचे (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          सचे (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          अञ्ञथा
            {
              वर्ण ध्रुव *error_सन्देशः;
              सचे (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_सन्देशः = N_("failed to remove directory %s");
                }
              अञ्ञथा
                {
                  /* A path component could be a symbolic link */
                  error_सन्देशः = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_सन्देशः), quoteaf (dir));
            }
          भङ्ग;
        }
    }
  पटिनिवत्त ok;
}

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Remove the DIRECTORY(ies), सचे they are empty.\n\
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
         output a diagnostic अत्थाय every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  पूर्णाङ्क optc;
  याव ((optc = getopt_long (argc, argv, "pv", longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'p':
          remove_empty_parents = true;
          भङ्ग;
        पक्ष IGNORE_FAIL_ON_NON_EMPTY_OPTION:
          ignore_fail_on_non_empty = true;
          भङ्ग;
        पक्ष 'v':
          verbose = true;
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  सचे (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  अत्थाय (; optind < argc; ++optind)
    {
      वर्ण *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      सचे (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      सचे (rmdir (dir) != 0)
        {
          पूर्णाङ्क rmdir_errno = errno;
          सचे (ignorable_failure (rmdir_errno, dir))
            अनुवर्त;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          सचे (rmdir_errno == ENOTDIR)
            {
              वर्ण ध्रुव *last_unix_slash = strrchr (dir, '/');
              सचे (last_unix_slash && (*(last_unix_slash + 1) == '\0'))
                {
                  संरचना stat st;
                  पूर्णाङ्क ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  सचे ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_mode)))
                    {
                      /* Ensure the last component was a symlink.  */
                      वर्ण *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      सचे (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      मुक्त (dir_arg);
                    }
                }
            }

          सचे (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      अञ्ञथा सचे (remove_empty_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  पटिनिवत्त ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
