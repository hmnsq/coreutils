/* touch -- change modification and access times of files
   Copyright (C) 1987-2026 Free Software Foundation, Inc.

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

/* Written by Paul Rubin, Arnold Robbins, Jim Kingdon, David MacKenzie,
   and Randy Smith. */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "argmatch.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "fd-reopen.h"
#ಅನ್ತರ್ಭಾವ "parse-datetime.h"
#ಅನ್ತರ್ಭಾವ "posixtm.h"
#ಅನ್ತರ್ಭಾವ "posixver.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "stat-time.h"
#ಅನ್ತರ್ಭಾವ "utimens.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "touch"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("Arnold Robbins"), \
  proper_name ("Jim Kingdon"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Randy Smith")

/* Bitmasks for 'change_times'. */
#ವ್ಯಾಖ್ಯಾನಿಸು CH_ATIME 1
#ವ್ಯಾಖ್ಯಾನಿಸು CH_MTIME 2

/* Which timestamps to change. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ change_times;

/* (-c) If true, don't create if not already there.  */
ಸ್ಥಿರ bool no_create;

/* (-r) If true, use times from a reference file.  */
ಸ್ಥಿರ bool use_ref;

/* (-h) If true, change the times of an existing symlink, if possible.  */
ಸ್ಥಿರ bool no_dereference;

/* If true, the only thing we have to do is change both the
   modification and access time to the current time, so we don't
   have to own the file, just be able to read and write it.
   On some systems, we can do this if we own the file, even though
   we have neither read nor write access to it.  */
ಸ್ಥಿರ bool amtime_now;

/* New access and modification times to use when setting time.  */
ಸ್ಥಿರ ರಚನೆ timespec newtime[2];

/* File to use for -r. */
ಸ್ಥಿರ ಅಕ್ಷರ *ref_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  ಸಮಯ_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"time", required_ವಾದ, ಖಾಲಿ, ಸಮಯ_OPTION},
  {"no-create", no_ವಾದ, ಖಾಲಿ, 'c'},
  {"date", required_ವಾದ, ಖಾಲಿ, 'd'},
  {"reference", required_ವಾದ, ಖಾಲಿ, 'r'},
  {"no-dereference", no_ವಾದ, ಖಾಲಿ, 'h'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* Valid arguments to the '--time' option. */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ ಸಮಯ_ವಾದಗಳು[] =
{
  "atime", "access", "use", "mtime", "modify", ಖಾಲಿ
};

/* The bits in 'change_times' that those arguments set. */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸ್ಥಿರಾಂಕ ಸಮಯ_masks[] =
{
  CH_ATIME, CH_ATIME, CH_ATIME, CH_MTIME, CH_MTIME
};

/* The interpretation of FLEX_DATE as a date, relative to NOW.  */

ಸ್ಥಿರ ರಚನೆ timespec
date_relative (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *flex_date, ರಚನೆ timespec now)
{
  ರಚನೆ timespec ಫಲಿತಾಂಶ;
  ಆದರೆ (! parse_datetime (&ಫಲಿತಾಂಶ, flex_date, &now))
    error (EXIT_FAILURE, 0, _("invalid date format %s"), quote (flex_date));
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

/* Update the time of file FILE according to the options given.
   Return true if successful.  */

ಸ್ಥಿರ bool
touch (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  ಪೂರ್ಣಾಂಕ fd = -1;
  ಪೂರ್ಣಾಂಕ open_errno = 0;
  ರಚನೆ timespec ಸ್ಥಿರಾಂಕ *t = newtime;

  ಆದರೆ (streq (file, "-"))
    fd = STDOUT_FILENO;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! (no_create || no_dereference))
    {
      /* Try to open FILE, creating it if necessary.  */
      fd = fd_reopen (STDIN_FILENO, file,
                      O_WRONLY | O_CREAT | O_NONBLOCK | O_NOCTTY, ಬಹುಲಕ_RW_UGO);
      ಆದರೆ (fd < 0)
        open_errno = errno;
    }

  ಆದರೆ (change_times != (CH_ATIME | CH_MTIME))
    {
      /* We're setting only one of the time values.  */
      ಆದರೆ (change_times == CH_MTIME)
        newtime[0].tv_nsec = UTIME_OMIT;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          affirm (change_times == CH_ATIME);
          newtime[1].tv_nsec = UTIME_OMIT;
        }
    }

  ಆದರೆ (amtime_now)
    {
      /* Pass NULL to futimens so it will not fail if we have
         write access to the file, but don't own it.  */
      t = ಖಾಲಿ;
    }

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file_opt = fd == STDOUT_FILENO ? ಖಾಲಿ : file;
  ಪೂರ್ಣಾಂಕ atflag = no_dereference ? AT_SYMLINK_NOFOLLOW : 0;
  ಪೂರ್ಣಾಂಕ utime_errno = (fdutimensat (fd, AT_FDCWD, file_opt, t, atflag) == 0
                     ? 0 : errno);

  ಆದರೆ (fd == STDIN_FILENO)
    {
      ಆದರೆ (close (STDIN_FILENO) != 0)
        {
          error (0, errno, _("failed to close %s"), quoteaf (file));
          ಹಿಂತಿರುಗಿ false;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (fd == STDOUT_FILENO)
    {
      /* Do not diagnose "touch -c - >&-".  */
      ಆದರೆ (utime_errno == EBADF && no_create)
        ಹಿಂತಿರುಗಿ true;
    }

  ಆದರೆ (utime_errno != 0)
    {
      /* Don't diagnose with open_errno if FILE is a directory, as that
         would give a bogus diagnostic for e.g., 'touch /' (assuming we
         don't own / or have write access).  On Solaris 10 and probably
         other systems, opening a directory like "." fails with EINVAL.
         (On SunOS 4 it was EPERM but that's obsolete.)  On macOS 26
         opening "/" fails with EEXIST.  */
      ರಚನೆ stat st;
      ಆದರೆ (open_errno
          && ! (open_errno == EISDIR
                || ((open_errno == EINVAL || open_errno == EEXIST)
                    && stat (file, &st) == 0 && S_ISDIR (st.st_ಬಹುಲಕ))))
        {
          /* The wording of this diagnostic should cover at least two cases:
             - the file does not exist, but the parent directory is unwritable
             - the file exists, but it isn't writable
             I think it's not worth trying to distinguish them.  */
          error (0, open_errno, _("cannot touch %s"), quoteaf (file));
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (no_create && utime_errno == ENOENT)
            ಹಿಂತಿರುಗಿ true;
          error (0, utime_errno, _("setting times of %s"), quoteaf (file));
        }
      ಹಿಂತಿರುಗಿ false;
    }

  ಹಿಂತಿರುಗಿ true;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... FILE...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Update the access and modification times ದ each FILE ಗಮ್ಯ the current ಸಮಯ.\n\
\n\
A FILE ವಾದ that does not exist is created ಖಾಲಿಗಳು, unless -c or -h\n\
is supplied.\n\
\n\
A FILE ವಾದ string ದ - is handled specially and causes touch ಗಮ್ಯ\n\
change the times ದ the file associated with standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a\n\
         change only the access ಸಮಯ\n\
"));
      oputs (_("\
  -c, --no-create\n\
         ಮಾಡು not create any files\n\
"));
      oputs (_("\
  -d, --date=STRING\n\
         parse STRING and use it instead ದ current ಸಮಯ\n\
"));
      oputs (_("\
  -f\n\
         (ignored)\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link instead ದ any referenced file;\n\
         useful only on systems that can change the timestamps ದ a symlink\n\
"));
      oputs (_("\
  -m\n\
         change only the modification ಸಮಯ\n\
"));
      oputs (_("\
  -r, --reference=FILE\n\
         use this file's times instead ದ current ಸಮಯ\n\
"));
      oputs (_("\
  -t [[CC]YY]MMDDhhmm[.ss]\n\
         use specified ಸಮಯ instead ದ current ಸಮಯ,\n\
         with a date-ಸಮಯ format that differs from -d's\n\
"));
      oputs (_("\
      --ಸಮಯ=ಶಬ್ದ\n\
         specify which ಸಮಯ ಗಮ್ಯ change:\n\
         access ಸಮಯ (-a): 'access', 'atime', 'use';\n\
         modification ಸಮಯ (-m): 'modify', 'mtime'\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool date_ಗಣ = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *flex_date = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಂಕ c;
  ಆಗ ((c = getopt_long (argc, argv, "acd:fhmr:t:", longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'a':
          change_times |= CH_ATIME;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'c':
          no_create = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'd':
          flex_date = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'f':
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'h':
          no_dereference = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'm':
          change_times |= CH_MTIME;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'r':
          use_ref = true;
          ref_file = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          ಆದರೆ (! posixtime (&newtime[0].tv_sec, optarg,
                           PDS_LEADING_YEAR | PDS_CENTURY | PDS_SECONDS))
            error (EXIT_FAILURE, 0, _("invalid date format %s"),
                   quote (optarg));
          newtime[0].tv_nsec = 0;
          newtime[1] = newtime[0];
          date_ಗಣ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಸಮಯ_OPTION:	/* --time */
          change_times |= XARGMATCH ("--time", optarg,
                                     ಸಮಯ_ವಾದಗಳು, ಸಮಯ_masks);
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (change_times == 0)
    change_times = CH_ATIME | CH_MTIME;

  ಆದರೆ (date_ಗಣ && (use_ref || flex_date))
    {
      error (0, 0, _("cannot specify times from more than one source"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (use_ref)
    {
      ರಚನೆ stat ref_stats;
      /* Don't use (no_dereference?lstat:stat) (args), since stat
         might be an object-like macro.  */
      ಆದರೆ (no_dereference ? lstat (ref_file, &ref_stats)
          : stat (ref_file, &ref_stats))
        error (EXIT_FAILURE, errno,
               _("failed to get attributes of %s"), quoteaf (ref_file));
      newtime[0] = get_stat_atime (&ref_stats);
      newtime[1] = get_stat_mtime (&ref_stats);
      date_ಗಣ = true;
      ಆದರೆ (flex_date)
        {
          ಆದರೆ (change_times & CH_ATIME)
            newtime[0] = date_relative (flex_date, newtime[0]);
          ಆದರೆ (change_times & CH_MTIME)
            newtime[1] = date_relative (flex_date, newtime[1]);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (flex_date)
        {
          ರಚನೆ timespec now = current_timespec ();
          newtime[1] = newtime[0] = date_relative (flex_date, now);
          date_ಗಣ = true;

          /* If neither -a nor -m is specified, treat "-d now" as if
             it were absent; this lets "touch" succeed more often in
             the presence of restrictive permissions.  */
          ಆದರೆ (change_times == (CH_ATIME | CH_MTIME)
              && timespec_cmp (newtime[0], now) == 0)
            {
              /* Check that it really was "-d now", and not a timestamp
                 that just happens to be the current time.  */
              ರಚನೆ timespec notnow, notnow1;
              notnow.tv_sec = now.tv_sec ^ 1;
              notnow.tv_nsec = now.tv_nsec;
              notnow1 = date_relative (flex_date, notnow);
              ಆದರೆ (timespec_cmp (notnow1, notnow) == 0)
                date_ಗಣ = false;
            }
        }
    }

  /* The obsolete 'MMDDhhmm[YY]' form is valid IFF there are
     two or more non-option arguments.  */
  ಆದರೆ (!date_ಗಣ && 2 <= argc - optind && posix2_ಆವೃತ್ತಿ () < 200112
      && posixtime (&newtime[0].tv_sec, argv[optind],
                    PDS_TRAILING_YEAR | PDS_PRE_2000))
    {
      newtime[0].tv_nsec = 0;
      newtime[1] = newtime[0];
      date_ಗಣ = true;

      ಆದರೆ (! getenv ("POSIXLY_CORRECT"))
        {
          ರಚನೆ tm ಸ್ಥಿರಾಂಕ *tm = localtime (&newtime[0].tv_sec);

          /* Technically, it appears that even a deliberate attempt to cause
             the above localtime to return NULL will always fail because our
             posixtime implementation rejects all dates for which localtime
             would fail.  However, skip the warning if it ever fails.  */
          ಆದರೆ (tm)
            error (0, 0,
                   _("warning: 'touch %s' is obsolete; use "
                     "'touch -t %04ld%02d%02d%02d%02d.%02d'"),
                   argv[optind],
                   tm->tm_year + 1900L, tm->tm_mon + 1, tm->tm_mday,
                   tm->tm_hour, tm->tm_min, tm->tm_sec);
        }

      optind++;
    }

  ಆದರೆ (!date_ಗಣ)
    {
      ಆದರೆ (change_times == (CH_ATIME | CH_MTIME))
        amtime_now = true;
      ಇಲ್ಲದಿದ್ದರೆ
        newtime[1].tv_nsec = newtime[0].tv_nsec = UTIME_NOW;
    }

  ಆದರೆ (optind == argc)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  ಗಾಗಿ (; optind < argc; ++optind)
    ok &= touch (argv[optind]);

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
