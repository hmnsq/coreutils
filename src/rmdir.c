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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "issymlink.h"
#અન્તર્ભાવ "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "rmdir"

#define AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
સ્થિર bool remove_empty_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
સ્થિર bool ignore_fail_on_non_empty;

/* If true, output a diagnostic for every directory processed.  */
સ્થિર bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  IGNORE_FAIL_ON_NON_EMPTY_OPTION = CHAR_MAX + 1
};

સ્થિર સંરચના option અચલ longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_argument, ખાલી,
   IGNORE_FAIL_ON_NON_EMPTY_OPTION},

  {"path", no_argument, ખાલી, 'p'},  /* Deprecated.  */
  {"parents", no_argument, ખાલી, 'p'},
  {"verbose", no_argument, ખાલી, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
સ્થિર bool
errno_rmdir_non_empty (પૂર્ણાંક error_સંખ્યા)
{
  પાછા error_સંખ્યા == ENOTEMPTY || error_સંખ્યા == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
સ્થિર bool
errno_may_be_non_empty (પૂર્ણાંક error_સંખ્યા)
{
  પસંદગી (error_સંખ્યા)
    {
    સ્થિતિ EACCES:
    સ્થિતિ EPERM:
    સ્થિતિ EROFS:
    સ્થિતિ EBUSY:
      પાછા true;
    મૂળભૂત:
      પાછા false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
સ્થિર bool
ignorable_failure (પૂર્ણાંક error_સંખ્યા, અક્ષર અચલ *dir)
{
  પાછા (ignore_fail_on_non_empty
          && (errno_rmdir_non_empty (error_સંખ્યા)
              || (errno_may_be_non_empty (error_સંખ્યા)
                  && directory_status (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

સ્થિર bool
remove_parents (અક્ષર *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  જ્યારે (true)
    {
      અક્ષર *slash = strrchr (dir, '/');
      જો (slash == ખાલી)
        તોડો;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      જ્યારે (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      જો (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      પૂર્ણાંક rmdir_errno = errno;

      જો (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          જો (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          નહીંતર
            {
              અક્ષર અચલ *error_સંદેશ;
              જો (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_સંદેશ = N_("failed to remove directory %s");
                }
              નહીંતર
                {
                  /* A path component could be a symbolic link */
                  error_સંદેશ = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_સંદેશ), quoteaf (dir));
            }
          તોડો;
        }
    }
  પાછા ok;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]... DIRECTORY...\n"), program_name);
      fputs (_("\
Remove the DIRECTORY(ies), જો they are empty.\n\
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
         output a diagnostic માટે every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  પૂર્ણાંક optc;
  જ્યારે ((optc = getopt_long (argc, argv, "pv", longopts, ખાલી)) != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'p':
          remove_empty_parents = true;
          તોડો;
        સ્થિતિ IGNORE_FAIL_ON_NON_EMPTY_OPTION:
          ignore_fail_on_non_empty = true;
          તોડો;
        સ્થિતિ 'v':
          verbose = true;
          તોડો;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  માટે (; optind < argc; ++optind)
    {
      અક્ષર *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      જો (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      જો (rmdir (dir) != 0)
        {
          પૂર્ણાંક rmdir_errno = errno;
          જો (ignorable_failure (rmdir_errno, dir))
            ચાલુ;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          જો (rmdir_errno == ENOTDIR)
            {
              અક્ષર અચલ *last_unix_slash = strrchr (dir, '/');
              જો (last_unix_slash && (*(last_unix_slash + 1) == '\0'))
                {
                  સંરચના stat st;
                  પૂર્ણાંક ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  જો ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_mode)))
                    {
                      /* Ensure the last component was a symlink.  */
                      અક્ષર *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      જો (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      મુક્ત (dir_arg);
                    }
                }
            }

          જો (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      નહીંતર જો (remove_empty_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
