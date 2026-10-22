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
  IGNORE_FAIL_ON_NON_EMPTY_OPTION = अक्षर_उच्च + 1
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
errno_rmdir_non_empty (पूर्णाङ्क error_संख्या)
{
  प्रत्यागम error_संख्या == ENOTEMPTY || error_संख्या == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
स्थिर bool
errno_may_be_non_empty (पूर्णाङ्क error_संख्या)
{
  विकल्प (error_संख्या)
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
ignorable_failure (पूर्णाङ्क error_संख्या, वर्ण ध्रुव *dir)
{
  प्रत्यागम (ignore_fail_on_non_empty
          && (errno_rmdir_non_empty (error_संख्या)
              || (errno_may_be_non_empty (error_संख्या)
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
  यावत् (true)
    {
      वर्ण *slash = म_खोजप (dir, '/');
      यदि (slash == रिक्त)
        भङ्ग;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      यावत् (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      यदि (verbose)
        prog_fprintf (मानक_निकास, _("removing directory, %s"), quoteaf (dir));

      ok = (सूची_हटाओ (dir) == 0);
      पूर्णाङ्क rmdir_errno = त्रुटि_सं;

      यदि (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          यदि (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          अन्यथा
            {
              वर्ण ध्रुव *error_संदेश;
              यदि (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_संदेश = N_("failed to remove directory %s");
                }
              अन्यथा
                {
                  /* A path component could be a symbolic link */
                  error_संदेश = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_संदेश), quoteaf (dir));
            }
          भङ्ग;
        }
    }
  प्रत्यागम ok;
}

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      ख_माला_दो (_("\
Remove the DIRECTORY(ies), यदि they are empty.\n\
\n\
"), मानक_निकास);
      oputs (_("\
      --ignore-fail-on-non-empty\n\
         ignore each failure to हटाओ a non-empty directory\n\
"));
      oputs (_("\
  -p, --parents\n\
         हटाओ DIRECTORY and its ancestors;\n\
         e.g., 'rmdir -p a/b' is similar to 'rmdir a/b a'\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic प्रति every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पूर्णाङ्क optc;
  यावत् ((optc = getopt_long (argc, argv, "pv", longopts, रिक्त)) != -1)
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
          usage (निकास_त्रुटि);
        }
    }

  यदि (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  bool ok = true;
  प्रति (; optind < argc; ++optind)
    {
      वर्ण *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      यदि (verbose)
        prog_fprintf (मानक_निकास, _("removing directory, %s"), quoteaf (dir));

      यदि (सूची_हटाओ (dir) != 0)
        {
          पूर्णाङ्क rmdir_errno = त्रुटि_सं;
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
              वर्ण ध्रुव *last_unix_slash = म_खोजप (dir, '/');
              यदि (last_unix_slash && (*(last_unix_slash + 1) == '\0'))
                {
                  संरचना stat st;
                  पूर्णाङ्क ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  यदि ((ret != 0 && त्रुटि_सं != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_mode)))
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
      अन्यथा यदि (remove_empty_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  प्रत्यागम ok ? निकास_सफल : निकास_त्रुटि;
}
