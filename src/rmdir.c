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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "issymlink.h"
#ଅନ୍ତର୍ଭାଵ "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rmdir"

#define AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
ସ୍ଥିର bool remove_empty_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
ସ୍ଥିର bool ignore_fail_on_non_empty;

/* If true, output a diagnostic for every directory processed.  */
ସ୍ଥିର bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  IGNORE_FAIL_ON_NON_EMPTY_OPTION = CHAR_MAX + 1
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_argument, ଖାଲି,
   IGNORE_FAIL_ON_NON_EMPTY_OPTION},

  {"path", no_argument, ଖାଲି, 'p'},  /* Deprecated.  */
  {"parents", no_argument, ଖାଲି, 'p'},
  {"verbose", no_argument, ଖାଲି, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
ସ୍ଥିର bool
errno_rmdir_non_empty (ପୂର୍ଣ୍ଣସଂଖ୍ୟା error_ସଂଖ୍ୟା)
{
  ଫେରନ୍ତୁ error_ସଂଖ୍ୟା == ENOTEMPTY || error_ସଂଖ୍ୟା == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
ସ୍ଥିର bool
errno_may_be_non_empty (ପୂର୍ଣ୍ଣସଂଖ୍ୟା error_ସଂଖ୍ୟା)
{
  ବାଛ (error_ସଂଖ୍ୟା)
    {
    ସ୍ଥିତି EACCES:
    ସ୍ଥିତି EPERM:
    ସ୍ଥିତି EROFS:
    ସ୍ଥିତି EBUSY:
      ଫେରନ୍ତୁ true;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
ସ୍ଥିର bool
ignorable_failure (ପୂର୍ଣ୍ଣସଂଖ୍ୟା error_ସଂଖ୍ୟା, ଅକ୍ଷର ଅଚଳ *dir)
{
  ଫେରନ୍ତୁ (ignore_fail_on_non_empty
          && (errno_rmdir_non_empty (error_ସଂଖ୍ୟା)
              || (errno_may_be_non_empty (error_ସଂଖ୍ୟା)
                  && directory_status (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

ସ୍ଥିର bool
remove_parents (ଅକ୍ଷର *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  ଯେତେବେଳେ (true)
    {
      ଅକ୍ଷର *slash = strrchr (dir, '/');
      ଯଦି (slash == ଖାଲି)
        ଭାଙ୍ଗ;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      ଯେତେବେଳେ (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      ଯଦି (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା rmdir_errno = errno;

      ଯଦି (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          ଯଦି (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          ଅନ୍ୟଥା
            {
              ଅକ୍ଷର ଅଚଳ *error_ସନ୍ଦେଶ;
              ଯଦି (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_ସନ୍ଦେଶ = N_("failed to remove directory %s");
                }
              ଅନ୍ୟଥା
                {
                  /* A path component could be a symbolic link */
                  error_ସନ୍ଦେଶ = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_ସନ୍ଦେଶ), quoteaf (dir));
            }
          ଭାଙ୍ଗ;
        }
    }
  ଫେରନ୍ତୁ ok;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Remove the DIRECTORY(ies), ଯଦି they are empty.\n\
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
         output a diagnostic ପାଇଁ every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "pv", longopts, ଖାଲି)) != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'p':
          remove_empty_parents = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି IGNORE_FAIL_ON_NON_EMPTY_OPTION:
          ignore_fail_on_non_empty = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'v':
          verbose = true;
          ଭାଙ୍ଗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  ପାଇଁ (; optind < argc; ++optind)
    {
      ଅକ୍ଷର *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      ଯଦି (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ଯଦି (rmdir (dir) != 0)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା rmdir_errno = errno;
          ଯଦି (ignorable_failure (rmdir_errno, dir))
            ଜାରି;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          ଯଦି (rmdir_errno == ENOTDIR)
            {
              ଅକ୍ଷର ଅଚଳ *last_unix_slash = strrchr (dir, '/');
              ଯଦି (last_unix_slash && (*(last_unix_slash + 1) == '\0'))
                {
                  ସଂରଚନା stat st;
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  ଯଦି ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_mode)))
                    {
                      /* Ensure the last component was a symlink.  */
                      ଅକ୍ଷର *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      ଯଦି (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      ମୁକ୍ତ (dir_arg);
                    }
                }
            }

          ଯଦି (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      ଅନ୍ୟଥା ଯଦି (remove_empty_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
