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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "argmatch.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "fd-reopen.h"
#அந்தர்பாவ "parse-datetime.h"
#அந்தர்பாவ "posixtm.h"
#அந்தர்பாவ "posixver.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "stat-time.h"
#அந்தர்பாவ "utimens.h"

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
நிலையான முழு change_times;

/* (-c) If true, don't create if not already there.  */
நிலையான bool no_create;

/* (-r) If true, use times from a reference file.  */
நிலையான bool use_ref;

/* (-h) If true, change the times of an existing symlink, if possible.  */
நிலையான bool no_dereference;

/* If true, the only thing we have to do is change both the
   modification and access time to the current time, so we don't
   have to own the file, just be able to read and write it.
   On some systems, we can do this if we own the file, even though
   we have neither read nor write access to it.  */
நிலையான bool amtime_now;

/* New access and modification times to use when setting time.  */
நிலையான கட்டமைப்பு timespec newtime[2];

/* File to use for -r. */
நிலையான எழுத்து *ref_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  TIME_OPTION = CHAR_MAX + 1
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"time", required_argument, வெற்று, TIME_OPTION},
  {"no-create", no_argument, வெற்று, 'c'},
  {"date", required_argument, வெற்று, 'd'},
  {"reference", required_argument, வெற்று, 'r'},
  {"no-dereference", no_argument, வெற்று, 'h'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

/* Valid arguments to the '--time' option. */
நிலையான எழுத்து மாறிலி *மாறிலி time_args[] =
{
  "atime", "access", "use", "mtime", "modify", வெற்று
};

/* The bits in 'change_times' that those arguments set. */
நிலையான முழு மாறிலி time_masks[] =
{
  CH_ATIME, CH_ATIME, CH_ATIME, CH_MTIME, CH_MTIME
};

/* The interpretation of FLEX_DATE as a date, relative to NOW.  */

நிலையான கட்டமைப்பு timespec
date_relative (எழுத்து மாறிலி *flex_date, கட்டமைப்பு timespec now)
{
  கட்டமைப்பு timespec result;
  எனில் (! parse_datetime (&result, flex_date, &now))
    error (EXIT_FAILURE, 0, _("invalid date format %s"), quote (flex_date));
  திரும்பு result;
}

/* Update the time of file FILE according to the options given.
   Return true if successful.  */

நிலையான bool
touch (எழுத்து மாறிலி *file)
{
  முழு fd = -1;
  முழு open_errno = 0;
  கட்டமைப்பு timespec மாறிலி *t = newtime;

  எனில் (streq (file, "-"))
    fd = STDOUT_FILENO;
  இல்லையெனில் எனில் (! (no_create || no_dereference))
    {
      /* Try to open FILE, creating it if necessary.  */
      fd = fd_reopen (STDIN_FILENO, file,
                      O_WRONLY | O_CREAT | O_NONBLOCK | O_NOCTTY, MODE_RW_UGO);
      எனில் (fd < 0)
        open_errno = errno;
    }

  எனில் (change_times != (CH_ATIME | CH_MTIME))
    {
      /* We're setting only one of the time values.  */
      எனில் (change_times == CH_MTIME)
        newtime[0].tv_nsec = UTIME_OMIT;
      இல்லையெனில்
        {
          affirm (change_times == CH_ATIME);
          newtime[1].tv_nsec = UTIME_OMIT;
        }
    }

  எனில் (amtime_now)
    {
      /* Pass NULL to futimens so it will not fail if we have
         write access to the file, but don't own it.  */
      t = வெற்று;
    }

  எழுத்து மாறிலி *file_opt = fd == STDOUT_FILENO ? வெற்று : file;
  முழு atflag = no_dereference ? AT_SYMLINK_NOFOLLOW : 0;
  முழு utime_errno = (fdutimensat (fd, AT_FDCWD, file_opt, t, atflag) == 0
                     ? 0 : errno);

  எனில் (fd == STDIN_FILENO)
    {
      எனில் (close (STDIN_FILENO) != 0)
        {
          error (0, errno, _("failed to close %s"), quoteaf (file));
          திரும்பு false;
        }
    }
  இல்லையெனில் எனில் (fd == STDOUT_FILENO)
    {
      /* Do not diagnose "touch -c - >&-".  */
      எனில் (utime_errno == EBADF && no_create)
        திரும்பு true;
    }

  எனில் (utime_errno != 0)
    {
      /* Don't diagnose with open_errno if FILE is a directory, as that
         would give a bogus diagnostic for e.g., 'touch /' (assuming we
         don't own / or have write access).  On Solaris 10 and probably
         other systems, opening a directory like "." fails with EINVAL.
         (On SunOS 4 it was EPERM but that's obsolete.)  On macOS 26
         opening "/" fails with EEXIST.  */
      கட்டமைப்பு stat st;
      எனில் (open_errno
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
      இல்லையெனில்
        {
          எனில் (no_create && utime_errno == ENOENT)
            திரும்பு true;
          error (0, utime_errno, _("setting times of %s"), quoteaf (file));
        }
      திரும்பு false;
    }

  திரும்பு true;
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
         செய் not create any files\n\
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

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool date_set = false;
  எழுத்து மாறிலி *flex_date = வெற்று;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  முழு c;
  போது ((c = getopt_long (argc, argv, "acd:fhmr:t:", longopts, வெற்று)) != -1)
    {
      தேர்வு (c)
        {
        நிலை 'a':
          change_times |= CH_ATIME;
          நிறுத்து;

        நிலை 'c':
          no_create = true;
          நிறுத்து;

        நிலை 'd':
          flex_date = optarg;
          நிறுத்து;

        நிலை 'f':
          நிறுத்து;

        நிலை 'h':
          no_dereference = true;
          நிறுத்து;

        நிலை 'm':
          change_times |= CH_MTIME;
          நிறுத்து;

        நிலை 'r':
          use_ref = true;
          ref_file = optarg;
          நிறுத்து;

        நிலை 't':
          எனில் (! posixtime (&newtime[0].tv_sec, optarg,
                           PDS_LEADING_YEAR | PDS_CENTURY | PDS_SECONDS))
            error (EXIT_FAILURE, 0, _("invalid date format %s"),
                   quote (optarg));
          newtime[0].tv_nsec = 0;
          newtime[1] = newtime[0];
          date_set = true;
          நிறுத்து;

        நிலை TIME_OPTION:	/* --time */
          change_times |= XARGMATCH ("--time", optarg,
                                     time_args, time_masks);
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (change_times == 0)
    change_times = CH_ATIME | CH_MTIME;

  எனில் (date_set && (use_ref || flex_date))
    {
      error (0, 0, _("cannot specify times from more than one source"));
      usage (EXIT_FAILURE);
    }

  எனில் (use_ref)
    {
      கட்டமைப்பு stat ref_stats;
      /* Don't use (no_dereference?lstat:stat) (args), since stat
         might be an object-like macro.  */
      எனில் (no_dereference ? lstat (ref_file, &ref_stats)
          : stat (ref_file, &ref_stats))
        error (EXIT_FAILURE, errno,
               _("failed to get attributes of %s"), quoteaf (ref_file));
      newtime[0] = get_stat_atime (&ref_stats);
      newtime[1] = get_stat_mtime (&ref_stats);
      date_set = true;
      எனில் (flex_date)
        {
          எனில் (change_times & CH_ATIME)
            newtime[0] = date_relative (flex_date, newtime[0]);
          எனில் (change_times & CH_MTIME)
            newtime[1] = date_relative (flex_date, newtime[1]);
        }
    }
  இல்லையெனில்
    {
      எனில் (flex_date)
        {
          கட்டமைப்பு timespec now = current_timespec ();
          newtime[1] = newtime[0] = date_relative (flex_date, now);
          date_set = true;

          /* If neither -a nor -m is specified, treat "-d now" as if
             it were absent; this lets "touch" succeed more often in
             the presence of restrictive permissions.  */
          எனில் (change_times == (CH_ATIME | CH_MTIME)
              && timespec_cmp (newtime[0], now) == 0)
            {
              /* Check that it really was "-d now", and not a timestamp
                 that just happens to be the current time.  */
              கட்டமைப்பு timespec notnow, notnow1;
              notnow.tv_sec = now.tv_sec ^ 1;
              notnow.tv_nsec = now.tv_nsec;
              notnow1 = date_relative (flex_date, notnow);
              எனில் (timespec_cmp (notnow1, notnow) == 0)
                date_set = false;
            }
        }
    }

  /* The obsolete 'MMDDhhmm[YY]' form is valid IFF there are
     two or more non-option arguments.  */
  எனில் (!date_set && 2 <= argc - optind && posix2_version () < 200112
      && posixtime (&newtime[0].tv_sec, argv[optind],
                    PDS_TRAILING_YEAR | PDS_PRE_2000))
    {
      newtime[0].tv_nsec = 0;
      newtime[1] = newtime[0];
      date_set = true;

      எனில் (! getenv ("POSIXLY_CORRECT"))
        {
          கட்டமைப்பு tm மாறிலி *tm = localtime (&newtime[0].tv_sec);

          /* Technically, it appears that even a deliberate attempt to cause
             the above localtime to return NULL will always fail because our
             posixtime implementation rejects all dates for which localtime
             would fail.  However, skip the warning if it ever fails.  */
          எனில் (tm)
            error (0, 0,
                   _("warning: 'touch %s' is obsolete; use "
                     "'touch -t %04ld%02d%02d%02d%02d.%02d'"),
                   argv[optind],
                   tm->tm_year + 1900L, tm->tm_mon + 1, tm->tm_mday,
                   tm->tm_hour, tm->tm_min, tm->tm_sec);
        }

      optind++;
    }

  எனில் (!date_set)
    {
      எனில் (change_times == (CH_ATIME | CH_MTIME))
        amtime_now = true;
      இல்லையெனில்
        newtime[1].tv_nsec = newtime[0].tv_nsec = UTIME_NOW;
    }

  எனில் (optind == argc)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  க்கு (; optind < argc; ++optind)
    ok &= touch (argv[optind]);

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
