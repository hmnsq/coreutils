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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "issymlink.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "prog-fprintf.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "rmdir"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

/* If true, remove empty parent directories.  */
ਸਥਿਰ bool remove_ਖਾਲੀ_ਥਾਵਾਂ_parents;

/* If true, don't treat failure to remove a nonempty directory
   as an error.  */
ਸਥਿਰ bool ignore_fail_on_non_ਖਾਲੀ_ਥਾਵਾਂ;

/* If true, output a diagnostic for every directory processed.  */
ਸਥਿਰ bool verbose;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  IGNORE_FAIL_ON_NON_ਖਾਲੀ_ਥਾਵਾਂ_OPTION = CHAR_MAX + 1
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  /* Don't name this '--force' because it's not close enough in meaning
     to e.g. rm's -f option.  */
  {"ignore-fail-on-non-empty", no_ਦਲੀਲ, ਨਿਰਾ,
   IGNORE_FAIL_ON_NON_ਖਾਲੀ_ਥਾਵਾਂ_OPTION},

  {"path", no_ਦਲੀਲ, ਨਿਰਾ, 'p'},  /* Deprecated.  */
  {"parents", no_ਦਲੀਲ, ਨਿਰਾ, 'p'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Return true if ERROR_NUMBER is one of the values associated
   with a failed rmdir due to non-empty target directory.  */
ਸਥਿਰ bool
errno_rmdir_non_ਖਾਲੀ_ਥਾਵਾਂ (ਪੂਰਨ error_ਸੰਖਿਆ)
{
  ਵਾਪਸ error_ਸੰਖਿਆ == ENOTEMPTY || error_ਸੰਖਿਆ == EEXIST;
}

/* Return true if when rmdir fails with errno == ERROR_NUMBER
   the directory may be non empty.  */
ਸਥਿਰ bool
errno_may_be_non_ਖਾਲੀ_ਥਾਵਾਂ (ਪੂਰਨ error_ਸੰਖਿਆ)
{
  ਚੋਣ (error_ਸੰਖਿਆ)
    {
    ਹਾਲਤ EACCES:
    ਹਾਲਤ EPERM:
    ਹਾਲਤ EROFS:
    ਹਾਲਤ EBUSY:
      ਵਾਪਸ true;
    ਮੂਲ:
      ਵਾਪਸ false;
    }
}

/* Return true if an rmdir failure with errno == error_number
   for DIR is ignorable.  */
ਸਥਿਰ bool
ignorable_failure (ਪੂਰਨ error_ਸੰਖਿਆ, ਅੱਖਰ ਅਚਲ *dir)
{
  ਵਾਪਸ (ignore_fail_on_non_ਖਾਲੀ_ਥਾਵਾਂ
          && (errno_rmdir_non_ਖਾਲੀ_ਥਾਵਾਂ (error_ਸੰਖਿਆ)
              || (errno_may_be_non_ਖਾਲੀ_ਥਾਵਾਂ (error_ਸੰਖਿਆ)
                  && directory_ਸਥਿਤੀ (AT_FDCWD, dir) == DS_NONEMPTY)));
}

/* Remove any empty parent directories of DIR.
   If DIR contains slash characters, at least one of them
   (beginning with the rightmost) is replaced with a NUL byte.
   Return true if successful.  */

ਸਥਿਰ bool
remove_parents (ਅੱਖਰ *dir)
{
  bool ok = true;

  strip_trailing_slashes (dir);
  ਜਦੋਂ (true)
    {
      ਅੱਖਰ *slash = strrchr (dir, '/');
      ਜੇ (slash == ਨਿਰਾ)
        ਰੋਕੋ;
      /* Remove any characters after the slash, skipping any extra
         slashes in a row. */
      ਜਦੋਂ (slash > dir && *slash == '/')
        --slash;
      slash[1] = 0;

      /* Give a diagnostic for each attempted removal if --verbose.  */
      ਜੇ (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ok = (rmdir (dir) == 0);
      ਪੂਰਨ rmdir_errno = errno;

      ਜੇ (! ok)
        {
          /* Stop quietly if --ignore-fail-on-non-empty. */
          ਜੇ (ignorable_failure (rmdir_errno, dir))
            {
              ok = true;
            }
          ਨਹੀਂਤਾਂ
            {
              ਅੱਖਰ ਅਚਲ *error_ਸੁਨੇਹਾ;
              ਜੇ (rmdir_errno != ENOTDIR)
                {
                  /* Barring race conditions,
                     DIR is expected to be a directory.  */
                  error_ਸੁਨੇਹਾ = N_("failed to remove directory %s");
                }
              ਨਹੀਂਤਾਂ
                {
                  /* A path component could be a symbolic link */
                  error_ਸੁਨੇਹਾ = N_("failed to remove %s");
                }
              error (0, rmdir_errno, _(error_ਸੁਨੇਹਾ), quoteaf (dir));
            }
          ਰੋਕੋ;
        }
    }
  ਵਾਪਸ ok;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... DIRECTORY...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Remove the DIRECTORY(ies), ਜੇ they are ਖਾਲੀ_ਥਾਵਾਂ.\n\
\n\
"), stdout);
      oputs (_("\
      --ignore-fail-on-non-ਖਾਲੀ_ਥਾਵਾਂ\n\
         ignore each failure ਮੰਜ਼ਿਲ remove a non-ਖਾਲੀ_ਥਾਵਾਂ directory\n\
"));
      oputs (_("\
  -p, --parents\n\
         remove DIRECTORY and its ancestors;\n\
         e.g., 'rmdir -p a/b' is similar ਮੰਜ਼ਿਲ 'rmdir a/b a'\n\
"));
      oputs (_("\
  -v, --verbose\n\
         output a diagnostic ਲਈ every directory processed\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਪੂਰਨ optc;
  ਜਦੋਂ ((optc = getopt_long (argc, argv, "pv", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'p':
          remove_ਖਾਲੀ_ਥਾਵਾਂ_parents = true;
          ਰੋਕੋ;
        ਹਾਲਤ IGNORE_FAIL_ON_NON_ਖਾਲੀ_ਥਾਵਾਂ_OPTION:
          ignore_fail_on_non_ਖਾਲੀ_ਥਾਵਾਂ = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'v':
          verbose = true;
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  ਲਈ (; optind < argc; ++optind)
    {
      ਅੱਖਰ *dir = argv[optind];

      /* Give a diagnostic for each attempted removal if --verbose.  */
      ਜੇ (verbose)
        prog_fprintf (stdout, _("removing directory, %s"), quoteaf (dir));

      ਜੇ (rmdir (dir) != 0)
        {
          ਪੂਰਨ rmdir_errno = errno;
          ਜੇ (ignorable_failure (rmdir_errno, dir))
            ਜਾਰੀ;

          /* Distinguish the case for a symlink with trailing slash.
             On Linux, rmdir(2) confusingly does not follow the symlink,
             thus giving the errno ENOTDIR, while on other systems the symlink
             is followed.  We don't provide consistent behavior here,
             but at least we provide a more accurate error message.  */
          bool custom_error = false;
          ਜੇ (rmdir_errno == ENOTDIR)
            {
              ਅੱਖਰ ਅਚਲ *ਆਖਰੀ_unix_slash = strrchr (dir, '/');
              ਜੇ (ਆਖਰੀ_unix_slash && (*(ਆਖਰੀ_unix_slash + 1) == '\0'))
                {
                  ਬਣਤਰ stat st;
                  ਪੂਰਨ ret = stat (dir, &st);
                  /* Some other issue following, or is actually a directory. */
                  ਜੇ ((ret != 0 && errno != ENOTDIR)
                      || (ret == 0 && S_ISDIR (st.st_ਬਹੁਲਕ)))
                    {
                      /* Ensure the last component was a symlink.  */
                      ਅੱਖਰ *dir_arg = xstrdup (dir);
                      strip_trailing_slashes (dir);
                      ਜੇ (issymlink (dir) == 1)
                        {
                          error (0, 0,
                                 _("failed to remove %s:"
                                   " Symbolic link not followed"),
                                 quoteaf (dir_arg));
                          custom_error = true;
                        }
                      ਮੁਕਤ (dir_arg);
                    }
                }
            }

          ਜੇ (! custom_error)
            error (0, rmdir_errno, _("failed to remove %s"), quoteaf (dir));

          ok = false;
        }
      ਨਹੀਂਤਾਂ ਜੇ (remove_ਖਾਲੀ_ਥਾਵਾਂ_parents)
        {
          ok &= remove_parents (dir);
        }
    }

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
