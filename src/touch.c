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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "argmatch.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "fd-reopen.h"
#অন্তর্ভাব "parse-datetime.h"
#অন্তর্ভাব "posixtm.h"
#অন্তর্ভাব "posixver.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "stat-time.h"
#অন্তর্ভাব "utimens.h"

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
স্থির পূর্ণসংখ্যা change_times;

/* (-c) If true, don't create if not already there.  */
স্থির bool no_create;

/* (-r) If true, use times from a reference file.  */
স্থির bool use_ref;

/* (-h) If true, change the times of an existing symlink, if possible.  */
স্থির bool no_dereference;

/* If true, the only thing we have to do is change both the
   modification and access time to the current time, so we don't
   have to own the file, just be able to read and write it.
   On some systems, we can do this if we own the file, even though
   we have neither read nor write access to it.  */
স্থির bool amtime_now;

/* New access and modification times to use when setting time.  */
স্থির কাঠামো timespec newtime[2];

/* File to use for -r. */
স্থির অক্ষর *ref_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  TIME_OPTION = CHAR_MAX + 1
};

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"time", required_argument, ফাঁকা, TIME_OPTION},
  {"no-create", no_argument, ফাঁকা, 'c'},
  {"date", required_argument, ফাঁকা, 'd'},
  {"reference", required_argument, ফাঁকা, 'r'},
  {"no-dereference", no_argument, ফাঁকা, 'h'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

/* Valid arguments to the '--time' option. */
স্থির অক্ষর ধ্রুবক *ধ্রুবক time_args[] =
{
  "atime", "access", "use", "mtime", "modify", ফাঁকা
};

/* The bits in 'change_times' that those arguments set. */
স্থির পূর্ণসংখ্যা ধ্রুবক time_masks[] =
{
  CH_ATIME, CH_ATIME, CH_ATIME, CH_MTIME, CH_MTIME
};

/* The interpretation of FLEX_DATE as a date, relative to NOW.  */

স্থির কাঠামো timespec
date_relative (অক্ষর ধ্রুবক *flex_date, কাঠামো timespec now)
{
  কাঠামো timespec ফলাফল;
  যদি (! parse_datetime (&ফলাফল, flex_date, &now))
    error (EXIT_FAILURE, 0, _("invalid date format %s"), quote (flex_date));
  ফেরত ফলাফল;
}

/* Update the time of file FILE according to the options given.
   Return true if successful.  */

স্থির bool
touch (অক্ষর ধ্রুবক *file)
{
  পূর্ণসংখ্যা fd = -1;
  পূর্ণসংখ্যা open_errno = 0;
  কাঠামো timespec ধ্রুবক *t = newtime;

  যদি (streq (file, "-"))
    fd = STDOUT_FILENO;
  অন্যথা যদি (! (no_create || no_dereference))
    {
      /* Try to open FILE, creating it if necessary.  */
      fd = fd_reopen (STDIN_FILENO, file,
                      O_WRONLY | O_CREAT | O_NONBLOCK | O_NOCTTY, MODE_RW_UGO);
      যদি (fd < 0)
        open_errno = errno;
    }

  যদি (change_times != (CH_ATIME | CH_MTIME))
    {
      /* We're setting only one of the time values.  */
      যদি (change_times == CH_MTIME)
        newtime[0].tv_nsec = UTIME_OMIT;
      অন্যথা
        {
          affirm (change_times == CH_ATIME);
          newtime[1].tv_nsec = UTIME_OMIT;
        }
    }

  যদি (amtime_now)
    {
      /* Pass NULL to futimens so it will not fail if we have
         write access to the file, but don't own it.  */
      t = ফাঁকা;
    }

  অক্ষর ধ্রুবক *file_opt = fd == STDOUT_FILENO ? ফাঁকা : file;
  পূর্ণসংখ্যা atflag = no_dereference ? AT_SYMLINK_NOFOLLOW : 0;
  পূর্ণসংখ্যা utime_errno = (fdutimensat (fd, AT_FDCWD, file_opt, t, atflag) == 0
                     ? 0 : errno);

  যদি (fd == STDIN_FILENO)
    {
      যদি (close (STDIN_FILENO) != 0)
        {
          error (0, errno, _("failed to close %s"), quoteaf (file));
          ফেরত false;
        }
    }
  অন্যথা যদি (fd == STDOUT_FILENO)
    {
      /* Do not diagnose "touch -c - >&-".  */
      যদি (utime_errno == EBADF && no_create)
        ফেরত true;
    }

  যদি (utime_errno != 0)
    {
      /* Don't diagnose with open_errno if FILE is a directory, as that
         would give a bogus diagnostic for e.g., 'touch /' (assuming we
         don't own / or have write access).  On Solaris 10 and probably
         other systems, opening a directory like "." fails with EINVAL.
         (On SunOS 4 it was EPERM but that's obsolete.)  On macOS 26
         opening "/" fails with EEXIST.  */
      কাঠামো stat st;
      যদি (open_errno
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
      অন্যথা
        {
          যদি (no_create && utime_errno == ENOENT)
            ফেরত true;
          error (0, utime_errno, _("setting times of %s"), quoteaf (file));
        }
      ফেরত false;
    }

  ফেরত true;
}

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
         করো not create any files\n\
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

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool date_set = false;
  অক্ষর ধ্রুবক *flex_date = ফাঁকা;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  পূর্ণসংখ্যা c;
  যখন ((c = getopt_long (argc, argv, "acd:fhmr:t:", longopts, ফাঁকা)) != -1)
    {
      নির্বাচন (c)
        {
        ক্ষেত্র 'a':
          change_times |= CH_ATIME;
          বিরতি;

        ক্ষেত্র 'c':
          no_create = true;
          বিরতি;

        ক্ষেত্র 'd':
          flex_date = optarg;
          বিরতি;

        ক্ষেত্র 'f':
          বিরতি;

        ক্ষেত্র 'h':
          no_dereference = true;
          বিরতি;

        ক্ষেত্র 'm':
          change_times |= CH_MTIME;
          বিরতি;

        ক্ষেত্র 'r':
          use_ref = true;
          ref_file = optarg;
          বিরতি;

        ক্ষেত্র 't':
          যদি (! posixtime (&newtime[0].tv_sec, optarg,
                           PDS_LEADING_YEAR | PDS_CENTURY | PDS_SECONDS))
            error (EXIT_FAILURE, 0, _("invalid date format %s"),
                   quote (optarg));
          newtime[0].tv_nsec = 0;
          newtime[1] = newtime[0];
          date_set = true;
          বিরতি;

        ক্ষেত্র TIME_OPTION:	/* --time */
          change_times |= XARGMATCH ("--time", optarg,
                                     time_args, time_masks);
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (change_times == 0)
    change_times = CH_ATIME | CH_MTIME;

  যদি (date_set && (use_ref || flex_date))
    {
      error (0, 0, _("cannot specify times from more than one source"));
      usage (EXIT_FAILURE);
    }

  যদি (use_ref)
    {
      কাঠামো stat ref_stats;
      /* Don't use (no_dereference?lstat:stat) (args), since stat
         might be an object-like macro.  */
      যদি (no_dereference ? lstat (ref_file, &ref_stats)
          : stat (ref_file, &ref_stats))
        error (EXIT_FAILURE, errno,
               _("failed to get attributes of %s"), quoteaf (ref_file));
      newtime[0] = get_stat_atime (&ref_stats);
      newtime[1] = get_stat_mtime (&ref_stats);
      date_set = true;
      যদি (flex_date)
        {
          যদি (change_times & CH_ATIME)
            newtime[0] = date_relative (flex_date, newtime[0]);
          যদি (change_times & CH_MTIME)
            newtime[1] = date_relative (flex_date, newtime[1]);
        }
    }
  অন্যথা
    {
      যদি (flex_date)
        {
          কাঠামো timespec now = current_timespec ();
          newtime[1] = newtime[0] = date_relative (flex_date, now);
          date_set = true;

          /* If neither -a nor -m is specified, treat "-d now" as if
             it were absent; this lets "touch" succeed more often in
             the presence of restrictive permissions.  */
          যদি (change_times == (CH_ATIME | CH_MTIME)
              && timespec_cmp (newtime[0], now) == 0)
            {
              /* Check that it really was "-d now", and not a timestamp
                 that just happens to be the current time.  */
              কাঠামো timespec notnow, notnow1;
              notnow.tv_sec = now.tv_sec ^ 1;
              notnow.tv_nsec = now.tv_nsec;
              notnow1 = date_relative (flex_date, notnow);
              যদি (timespec_cmp (notnow1, notnow) == 0)
                date_set = false;
            }
        }
    }

  /* The obsolete 'MMDDhhmm[YY]' form is valid IFF there are
     two or more non-option arguments.  */
  যদি (!date_set && 2 <= argc - optind && posix2_version () < 200112
      && posixtime (&newtime[0].tv_sec, argv[optind],
                    PDS_TRAILING_YEAR | PDS_PRE_2000))
    {
      newtime[0].tv_nsec = 0;
      newtime[1] = newtime[0];
      date_set = true;

      যদি (! getenv ("POSIXLY_CORRECT"))
        {
          কাঠামো tm ধ্রুবক *tm = localtime (&newtime[0].tv_sec);

          /* Technically, it appears that even a deliberate attempt to cause
             the above localtime to return NULL will always fail because our
             posixtime implementation rejects all dates for which localtime
             would fail.  However, skip the warning if it ever fails.  */
          যদি (tm)
            error (0, 0,
                   _("warning: 'touch %s' is obsolete; use "
                     "'touch -t %04ld%02d%02d%02d%02d.%02d'"),
                   argv[optind],
                   tm->tm_year + 1900L, tm->tm_mon + 1, tm->tm_mday,
                   tm->tm_hour, tm->tm_min, tm->tm_sec);
        }

      optind++;
    }

  যদি (!date_set)
    {
      যদি (change_times == (CH_ATIME | CH_MTIME))
        amtime_now = true;
      অন্যথা
        newtime[1].tv_nsec = newtime[0].tv_nsec = UTIME_NOW;
    }

  যদি (optind == argc)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  জন্য (; optind < argc; ++optind)
    ok &= touch (argv[optind]);

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
