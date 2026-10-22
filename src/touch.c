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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "argmatch.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "fd-reopen.h"
#അന്തര്ഭാവ "parse-datetime.h"
#അന്തര്ഭാവ "posixtm.h"
#അന്തര്ഭാവ "posixver.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "stat-time.h"
#അന്തര്ഭാവ "utimens.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "touch"

#define AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("Arnold Robbins"), \
  proper_name ("Jim Kingdon"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Randy Smith")

/* Bitmasks for 'change_times'. */
#define CH_ATIME 1
#define CH_MTIME 2

/* Which timestamps to change. */
സ്ഥിതം പൂർണ്ണസംഖ്യ change_times;

/* (-c) If true, don't create if not already there.  */
സ്ഥിതം bool no_create;

/* (-r) If true, use times from a reference file.  */
സ്ഥിതം bool use_ref;

/* (-h) If true, change the times of an existing symlink, if possible.  */
സ്ഥിതം bool no_dereference;

/* If true, the only thing we have to do is change both the
   modification and access time to the current time, so we don't
   have to own the file, just be able to read and write it.
   On some systems, we can do this if we own the file, even though
   we have neither read nor write access to it.  */
സ്ഥിതം bool amtime_now;

/* New access and modification times to use when setting time.  */
സ്ഥിതം ഘടന timespec newtime[2];

/* File to use for -r. */
സ്ഥിതം അക്ഷരം *ref_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  TIME_OPTION = CHAR_MAX + 1
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"time", required_argument, ശൂന്യ, TIME_OPTION},
  {"no-create", no_argument, ശൂന്യ, 'c'},
  {"date", required_argument, ശൂന്യ, 'd'},
  {"reference", required_argument, ശൂന്യ, 'r'},
  {"no-dereference", no_argument, ശൂന്യ, 'h'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

/* Valid arguments to the '--time' option. */
സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം time_args[] =
{
  "atime", "access", "use", "mtime", "modify", ശൂന്യ
};

/* The bits in 'change_times' that those arguments set. */
സ്ഥിതം പൂർണ്ണസംഖ്യ സ്ഥിരം time_masks[] =
{
  CH_ATIME, CH_ATIME, CH_ATIME, CH_MTIME, CH_MTIME
};

/* The interpretation of FLEX_DATE as a date, relative to NOW.  */

സ്ഥിതം ഘടന timespec
date_relative (അക്ഷരം സ്ഥിരം *flex_date, ഘടന timespec now)
{
  ഘടന timespec result;
  എങ്കിൽ (! parse_datetime (&result, flex_date, &now))
    error (EXIT_FAILURE, 0, _("invalid date format %s"), quote (flex_date));
  തിരികെ result;
}

/* Update the time of file FILE according to the options given.
   Return true if successful.  */

സ്ഥിതം bool
touch (അക്ഷരം സ്ഥിരം *file)
{
  പൂർണ്ണസംഖ്യ fd = -1;
  പൂർണ്ണസംഖ്യ open_errno = 0;
  ഘടന timespec സ്ഥിരം *t = newtime;

  എങ്കിൽ (streq (file, "-"))
    fd = STDOUT_FILENO;
  അല്ലെങ്കിൽ എങ്കിൽ (! (no_create || no_dereference))
    {
      /* Try to open FILE, creating it if necessary.  */
      fd = fd_reopen (STDIN_FILENO, file,
                      O_WRONLY | O_CREAT | O_NONBLOCK | O_NOCTTY, MODE_RW_UGO);
      എങ്കിൽ (fd < 0)
        open_errno = errno;
    }

  എങ്കിൽ (change_times != (CH_ATIME | CH_MTIME))
    {
      /* We're setting only one of the time values.  */
      എങ്കിൽ (change_times == CH_MTIME)
        newtime[0].tv_nsec = UTIME_OMIT;
      അല്ലെങ്കിൽ
        {
          affirm (change_times == CH_ATIME);
          newtime[1].tv_nsec = UTIME_OMIT;
        }
    }

  എങ്കിൽ (amtime_now)
    {
      /* Pass NULL to futimens so it will not fail if we have
         write access to the file, but don't own it.  */
      t = ശൂന്യ;
    }

  അക്ഷരം സ്ഥിരം *file_opt = fd == STDOUT_FILENO ? ശൂന്യ : file;
  പൂർണ്ണസംഖ്യ atflag = no_dereference ? AT_SYMLINK_NOFOLLOW : 0;
  പൂർണ്ണസംഖ്യ utime_errno = (fdutimensat (fd, AT_FDCWD, file_opt, t, atflag) == 0
                     ? 0 : errno);

  എങ്കിൽ (fd == STDIN_FILENO)
    {
      എങ്കിൽ (close (STDIN_FILENO) != 0)
        {
          error (0, errno, _("failed to close %s"), quoteaf (file));
          തിരികെ false;
        }
    }
  അല്ലെങ്കിൽ എങ്കിൽ (fd == STDOUT_FILENO)
    {
      /* Do not diagnose "touch -c - >&-".  */
      എങ്കിൽ (utime_errno == EBADF && no_create)
        തിരികെ true;
    }

  എങ്കിൽ (utime_errno != 0)
    {
      /* Don't diagnose with open_errno if FILE is a directory, as that
         would give a bogus diagnostic for e.g., 'touch /' (assuming we
         don't own / or have write access).  On Solaris 10 and probably
         other systems, opening a directory like "." fails with EINVAL.
         (On SunOS 4 it was EPERM but that's obsolete.)  On macOS 26
         opening "/" fails with EEXIST.  */
      ഘടന stat st;
      എങ്കിൽ (open_errno
          && ! (open_errno == EISDIR
                || ((open_errno == EINVAL || open_errno == EEXIST)
                    && stat (file, &st) == 0 && S_ISDIR (st.st_mode))))
        {
          /* The wording of this diagnostic should cover at least two cases:
             - the file does not exist, but the parent directory is unwritable
             - the file exists, but it isn't writable
             I think it's not worth trying to distinguish them.  */
          error (0, open_errno, _("cannot touch %s"), quoteaf (file));
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (no_create && utime_errno == ENOENT)
            തിരികെ true;
          error (0, utime_errno, _("setting times of %s"), quoteaf (file));
        }
      തിരികെ false;
    }

  തിരികെ true;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Update the access and modification times of each FILE to the current time.\n\
\n\
A FILE argument that does not exist is created empty, unless -c or -h\n\
is supplied.\n\
\n\
A FILE argument string of - is handled specially and causes touch to\n\
change the times of the file associated with standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a\n\
         change only the access time\n\
"));
      oputs (_("\
  -c, --no-create\n\
         ചെയ്യുക not create any files\n\
"));
      oputs (_("\
  -d, --date=STRING\n\
         parse STRING and use it instead of current time\n\
"));
      oputs (_("\
  -f\n\
         (ignored)\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link instead of any referenced file;\n\
         useful only on systems that can change the timestamps of a symlink\n\
"));
      oputs (_("\
  -m\n\
         change only the modification time\n\
"));
      oputs (_("\
  -r, --reference=FILE\n\
         use this file's times instead of current time\n\
"));
      oputs (_("\
  -t [[CC]YY]MMDDhhmm[.ss]\n\
         use specified time instead of current time,\n\
         with a date-time format that differs from -d's\n\
"));
      oputs (_("\
      --time=WORD\n\
         specify which time to change:\n\
         access time (-a): 'access', 'atime', 'use';\n\
         modification time (-m): 'modify', 'mtime'\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  bool date_set = false;
  അക്ഷരം സ്ഥിരം *flex_date = ശൂന്യ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  പൂർണ്ണസംഖ്യ c;
  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "acd:fhmr:t:", longopts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'a':
          change_times |= CH_ATIME;
          നിർത്തുക;

        അവസ്ഥ 'c':
          no_create = true;
          നിർത്തുക;

        അവസ്ഥ 'd':
          flex_date = optarg;
          നിർത്തുക;

        അവസ്ഥ 'f':
          നിർത്തുക;

        അവസ്ഥ 'h':
          no_dereference = true;
          നിർത്തുക;

        അവസ്ഥ 'm':
          change_times |= CH_MTIME;
          നിർത്തുക;

        അവസ്ഥ 'r':
          use_ref = true;
          ref_file = optarg;
          നിർത്തുക;

        അവസ്ഥ 't':
          എങ്കിൽ (! posixtime (&newtime[0].tv_sec, optarg,
                           PDS_LEADING_YEAR | PDS_CENTURY | PDS_SECONDS))
            error (EXIT_FAILURE, 0, _("invalid date format %s"),
                   quote (optarg));
          newtime[0].tv_nsec = 0;
          newtime[1] = newtime[0];
          date_set = true;
          നിർത്തുക;

        അവസ്ഥ TIME_OPTION:	/* --time */
          change_times |= XARGMATCH ("--time", optarg,
                                     time_args, time_masks);
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (change_times == 0)
    change_times = CH_ATIME | CH_MTIME;

  എങ്കിൽ (date_set && (use_ref || flex_date))
    {
      error (0, 0, _("cannot specify times from more than one source"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (use_ref)
    {
      ഘടന stat ref_stats;
      /* Don't use (no_dereference?lstat:stat) (args), since stat
         might be an object-like macro.  */
      എങ്കിൽ (no_dereference ? lstat (ref_file, &ref_stats)
          : stat (ref_file, &ref_stats))
        error (EXIT_FAILURE, errno,
               _("failed to get attributes of %s"), quoteaf (ref_file));
      newtime[0] = get_stat_atime (&ref_stats);
      newtime[1] = get_stat_mtime (&ref_stats);
      date_set = true;
      എങ്കിൽ (flex_date)
        {
          എങ്കിൽ (change_times & CH_ATIME)
            newtime[0] = date_relative (flex_date, newtime[0]);
          എങ്കിൽ (change_times & CH_MTIME)
            newtime[1] = date_relative (flex_date, newtime[1]);
        }
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (flex_date)
        {
          ഘടന timespec now = current_timespec ();
          newtime[1] = newtime[0] = date_relative (flex_date, now);
          date_set = true;

          /* If neither -a nor -m is specified, treat "-d now" as if
             it were absent; this lets "touch" succeed more often in
             the presence of restrictive permissions.  */
          എങ്കിൽ (change_times == (CH_ATIME | CH_MTIME)
              && timespec_cmp (newtime[0], now) == 0)
            {
              /* Check that it really was "-d now", and not a timestamp
                 that just happens to be the current time.  */
              ഘടന timespec notnow, notnow1;
              notnow.tv_sec = now.tv_sec ^ 1;
              notnow.tv_nsec = now.tv_nsec;
              notnow1 = date_relative (flex_date, notnow);
              എങ്കിൽ (timespec_cmp (notnow1, notnow) == 0)
                date_set = false;
            }
        }
    }

  /* The obsolete 'MMDDhhmm[YY]' form is valid IFF there are
     two or more non-option arguments.  */
  എങ്കിൽ (!date_set && 2 <= argc - optind && posix2_version () < 200112
      && posixtime (&newtime[0].tv_sec, argv[optind],
                    PDS_TRAILING_YEAR | PDS_PRE_2000))
    {
      newtime[0].tv_nsec = 0;
      newtime[1] = newtime[0];
      date_set = true;

      എങ്കിൽ (! getenv ("POSIXLY_CORRECT"))
        {
          ഘടന tm സ്ഥിരം *tm = localtime (&newtime[0].tv_sec);

          /* Technically, it appears that even a deliberate attempt to cause
             the above localtime to return NULL will always fail because our
             posixtime implementation rejects all dates for which localtime
             would fail.  However, skip the warning if it ever fails.  */
          എങ്കിൽ (tm)
            error (0, 0,
                   _("warning: 'touch %s' is obsolete; use "
                     "'touch -t %04ld%02d%02d%02d%02d.%02d'"),
                   argv[optind],
                   tm->tm_year + 1900L, tm->tm_mon + 1, tm->tm_mday,
                   tm->tm_hour, tm->tm_min, tm->tm_sec);
        }

      optind++;
    }

  എങ്കിൽ (!date_set)
    {
      എങ്കിൽ (change_times == (CH_ATIME | CH_MTIME))
        amtime_now = true;
      അല്ലെങ്കിൽ
        newtime[1].tv_nsec = newtime[0].tv_nsec = UTIME_NOW;
    }

  എങ്കിൽ (optind == argc)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  വേണ്ടി (; optind < argc; ++optind)
    ok &= touch (argv[optind]);

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
