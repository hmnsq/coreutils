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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "issymlink.h"
#அந்தர்பாவ "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rmdir"

#define AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
நிலையான bool remove_empty_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
நிலையான bool ignore_fail_on_non_empty;

/* If true, output a diagnostic for every directory processed.  */
நிலையான bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  IGNORE_FAIL_ON_NON_EMPTY_OPTION = CHAR_MAX + 1
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_argument, வெற்று,
   IGNORE_FAIL_ON_NON_EMPTY_OPTION},

  {"path", no_argument, வெற்று, 'p'},  /* Deprecated.  */
  {"parents", no_argument, வெற்று, 'p'},
  {"verbose", no_argument, வெற்று, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
நிலையான bool
errno_rmdir_non_empty (முழு error_எண்)
{
  திரும்பு error_எண் == ENOTEMPTY || error_எண் == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
நிலையான bool
errno_may_be_non_empty (முழு error_எண்)
{
  தேர்வு (error_எண்)
    {
    நிலை EACCES:
    நிலை EPERM:
    நிலை EROFS:
    நிலை EBUSY:
      திரும்பு true;
    இயல்பு:
      திரும்பு false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
நிலையான bool
ignorable_failure (முழு error_எண், எழுத்து மாறிலி *dir)
{
  திரும்பு (ignore_fail_on_non_empty
          && (errno_rmdir_non_empty (error_எண்)
              || (errno_may_be_non_empty (error_எண்)
                  && directory_status (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

நிலையான bool
remove_parents (எழுத்து *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  போது (true)
    {
      எழுத்து *slash = strrchr (dir, '/');
      எனில் (slash == வெற்று)
        நிறுத்து;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      போது (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      எனில் (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      முழு rmdir_errno = errno;

      எனில் (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          எனில் (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          இல்லையெனில்
            {
              எழுத்து மாறிலி *error_செய்தி;
              எனில் (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_செய்தி = N_("failed to remove directory %s");
                }
              இல்லையெனில்
                {
                  /* A path component could be a symbolic link */
                  error_செய்தி = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_செய்தி), quoteaf (dir));
            }
          நிறுத்து;
        }
    }
  திரும்பு ok;
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Remove the DIRECTORY(ies), எனில் they are empty.\n\
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
         output a diagnostic க்கு every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  முழு optc;
  போது ((optc = getopt_long (argc, argv, "pv", longopts, வெற்று)) != -1)
    {
      தேர்வு (optc)
        {
        நிலை 'p':
          remove_empty_parents = true;
          நிறுத்து;
        நிலை IGNORE_FAIL_ON_NON_EMPTY_OPTION:
          ignore_fail_on_non_empty = true;
          நிறுத்து;
        நிலை 'v':
          verbose = true;
          நிறுத்து;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  க்கு (; optind < argc; ++optind)
    {
      எழுத்து *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      எனில் (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      எனில் (rmdir (dir) != 0)
        {
          முழு rmdir_errno = errno;
          எனில் (ignorable_failure (rmdir_errno, dir))
            தொடர்;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          எனில் (rmdir_errno == ENOTDIR)
            {
              எழுத்து மாறிலி *last_unix_slash = strrchr (dir, '/');
              எனில் (last_unix_slash && (*(last_unix_slash + 1) == '\0'))
                {
                  கட்டமைப்பு stat st;
                  முழு ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  எனில் ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_mode)))
                    {
                      /* Ensure the last component was a symlink.  */
                      எழுத்து *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      எனில் (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      விடுவி (dir_arg);
                    }
                }
            }

          எனில் (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      இல்லையெனில் எனில் (remove_empty_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
