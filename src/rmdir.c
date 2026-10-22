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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "issymlink.h"
#समावेश "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "rmdir"

#घोषणा AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
अटल bool remove_empty_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
अटल bool ignore_fail_on_non_empty;

/* If true, output a diagnostic for every directory processed.  */
अटल bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  IGNORE_FAIL_ON_NON_EMPTY_OPTION = अक्षर_उच्च + 1
};

अटल काष्ठा option स्थिर longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_argument, शून्य,
   IGNORE_FAIL_ON_NON_EMPTY_OPTION},

  {"path", no_argument, शून्य, 'p'},  /* Deprecated.  */
  {"parents", no_argument, शून्य, 'p'},
  {"verbose", no_argument, शून्य, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
अटल bool
errno_rmdir_non_empty (पूर्णांक error_संख्या)
{
  वापस error_संख्या == ENOTEMPTY || error_संख्या == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
अटल bool
errno_may_be_non_empty (पूर्णांक error_संख्या)
{
  चयन (error_संख्या)
    {
    हाल EACCES:
    हाल EPERM:
    हाल EROFS:
    हाल EBUSY:
      वापस true;
    शेष:
      वापस false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
अटल bool
ignorable_failure (पूर्णांक error_संख्या, अक्षर स्थिर *dir)
{
  वापस (ignore_fail_on_non_empty
          && (errno_rmdir_non_empty (error_संख्या)
              || (errno_may_be_non_empty (error_संख्या)
                  && directory_status (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

अटल bool
remove_parents (अक्षर *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  जबतक (true)
    {
      अक्षर *slash = म_खोजप (dir, '/');
      अगर (slash == शून्य)
        अवरोध;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      जबतक (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      अगर (verbose)
        prog_fprintf (मानक_निकास, _("removing directory, %s"), quoteaf (dir));

      ok = (सूची_हटाओ (dir) == 0);
      पूर्णांक rmdir_errno = त्रुटि_सं;

      अगर (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          अगर (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          अन्यथा
            {
              अक्षर स्थिर *error_संदेश;
              अगर (rmdir_errno != ENOTDIR)
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
          अवरोध;
        }
    }
  वापस ok;
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      ख_माला_दो (_("\
Remove the DIRECTORY(ies), अगर they are empty.\n\
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
         output a diagnostic क्रम every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पूर्णांक optc;
  जबतक ((optc = getopt_long (argc, argv, "pv", longopts, शून्य)) != -1)
    {
      चयन (optc)
        {
        हाल 'p':
          remove_empty_parents = true;
          अवरोध;
        हाल IGNORE_FAIL_ON_NON_EMPTY_OPTION:
          ignore_fail_on_non_empty = true;
          अवरोध;
        हाल 'v':
          verbose = true;
          अवरोध;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  bool ok = true;
  क्रम (; optind < argc; ++optind)
    {
      अक्षर *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      अगर (verbose)
        prog_fprintf (मानक_निकास, _("removing directory, %s"), quoteaf (dir));

      अगर (सूची_हटाओ (dir) != 0)
        {
          पूर्णांक rmdir_errno = त्रुटि_सं;
          अगर (ignorable_failure (rmdir_errno, dir))
            जारी;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          अगर (rmdir_errno == ENOTDIR)
            {
              अक्षर स्थिर *last_unix_slash = म_खोजप (dir, '/');
              अगर (last_unix_slash && (*(last_unix_slash + 1) == '\0'))
                {
                  काष्ठा stat st;
                  पूर्णांक ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  अगर ((ret != 0 && त्रुटि_सं != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_mode)))
                    {
                      /* Ensure the last component was a symlink.  */
                      अक्षर *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      अगर (issymlink (dir) == 1)
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

          अगर (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      अन्यथा अगर (remove_empty_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
