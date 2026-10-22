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
#परिभाषय क्रमादेशः_NAME "rmdir"

#परिभाषय AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
स्थिर bool remove_रिक्तानि_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
स्थिर bool ignore_fail_on_non_रिक्तानि;

/* If true, output a diagnostic for every directory processed.  */
स्थिर bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  IGNORE_FAIL_ON_NON_रिक्तानि_OPTION = CHAR_MAX + 1
};

स्थिर संरचना option ध्रुव longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_तर्कः, रिक्त,
   IGNORE_FAIL_ON_NON_रिक्तानि_OPTION},

  {"path", no_तर्कः, रिक्त, 'p'},  /* Deprecated.  */
  {"parents", no_तर्कः, रिक्त, 'p'},
  {"verbose", no_तर्कः, रिक्त, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
स्थिर bool
errno_rmdir_non_रिक्तानि (पूर्णाङ्क error_सङ्ख्या)
{
  प्रत्यागम error_सङ्ख्या == ENOTEMPTY || error_सङ्ख्या == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
स्थिर bool
errno_may_be_non_रिक्तानि (पूर्णाङ्क error_सङ्ख्या)
{
  विकल्प (error_सङ्ख्या)
    {
    पक्ष EACCES:
    पक्ष EPERM:
    पक्ष EROFS:
    पक्ष EBUSY:
      प्रत्यागम true;
    स्वभाव:
      प्रत्यागम false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
स्थिर bool
ignorable_failure (पूर्णाङ्क error_सङ्ख्या, वर्ण ध्रुव *dir)
{
  प्रत्यागम (ignore_fail_on_non_रिक्तानि
          && (errno_rmdir_non_रिक्तानि (error_सङ्ख्या)
              || (errno_may_be_non_रिक्तानि (error_सङ्ख्या)
                  && directory_स्थितिः (AT_FDCWD, dir) == DS_NONEMPTY)));
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
  यावत् (true)
    {
      वर्ण *slash = strrchr (dir, '/');
      यदि (slash == रिक्त)
        भङ्ग;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      यावत् (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      यदि (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      पूर्णाङ्क rmdir_errno = errno;

      यदि (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          यदि (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          अन्यथा
            {
              वर्ण ध्रुव *error_सन्देशः;
              यदि (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_सन्देशः = N_("failed to remove directory %s");
                }
              अन्यथा
                {
                  /* A path component could be a symbolic link */
                  error_सन्देशः = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_सन्देशः), quoteaf (dir));
            }
          भङ्ग;
        }
    }
  प्रत्यागम ok;
}

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION]... DIRECTORY...\n"), क्रमादेशः_name);
      fputs (_("\
Remove the DIRECTORY(ies), यदि they are रिक्तानि.\n\
\n\
"), stdout);
      oputs (_("\
      --ignore-fail-on-non-रिक्तानि\n\
         ignore each failure गन्तव्यम् remove a non-रिक्तानि directory\n\
"));
      oputs (_("\
  -p, --parents\n\
         remove DIRECTORY and its ancestors;\n\
         e.g., 'rmdir -p a/b' is similar गन्तव्यम् 'rmdir a/b a'\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic प्रति every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  पूर्णाङ्क optc;
  यावत् ((optc = getopt_long (argc, argv, "pv", longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'p':
          remove_रिक्तानि_parents = true;
          भङ्ग;
        पक्ष IGNORE_FAIL_ON_NON_रिक्तानि_OPTION:
          ignore_fail_on_non_रिक्तानि = true;
          भङ्ग;
        पक्ष 'v':
          verbose = true;
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  प्रति (; optind < argc; ++optind)
    {
      वर्ण *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      यदि (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      यदि (rmdir (dir) != 0)
        {
          पूर्णाङ्क rmdir_errno = errno;
          यदि (ignorable_failure (rmdir_errno, dir))
            अनुवर्त;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          यदि (rmdir_errno == ENOTDIR)
            {
              वर्ण ध्रुव *चरमः_unix_slash = strrchr (dir, '/');
              यदि (चरमः_unix_slash && (*(चरमः_unix_slash + 1) == '\0'))
                {
                  संरचना stat st;
                  पूर्णाङ्क ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  यदि ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_बहुलकम्)))
                    {
                      /* Ensure the last component was a symlink.  */
                      वर्ण *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      यदि (issymlink (dir) == 1)
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

          यदि (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      अन्यथा यदि (remove_रिक्तानि_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
