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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "argmatch.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "fd-reopen.h"
#అన్తర్భావ "parse-datetime.h"
#అన్తర్భావ "posixtm.h"
#అన్తర్భావ "posixver.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "stat-time.h"
#అన్తర్భావ "utimens.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "touch"

#నిర్వచించు AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("Arnold Robbins"), \
  proper_name ("Jim Kingdon"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Randy Smith")

/* Bitmasks for 'change_times'. */
#నిర్వచించు CH_ATIME 1
#నిర్వచించు CH_MTIME 2

/* Which timestamps to change. */
స్థిర పూర్ణాంకం change_times;

/* (-c) If true, don't create if not already there.  */
స్థిర bool no_create;

/* (-r) If true, use times from a reference file.  */
స్థిర bool use_ref;

/* (-h) If true, change the times of an existing symlink, if possible.  */
స్థిర bool no_dereference;

/* If true, the only thing we have to do is change both the
   modification and access time to the current time, so we don't
   have to own the file, just be able to read and write it.
   On some systems, we can do this if we own the file, even though
   we have neither read nor write access to it.  */
స్థిర bool amtime_now;

/* New access and modification times to use when setting time.  */
స్థిర నిర్మాణం timespec newtime[2];

/* File to use for -r. */
స్థిర అక్షరం *ref_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  సమయం_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"time", required_వాదం, ఖాళీ, సమయం_OPTION},
  {"no-create", no_వాదం, ఖాళీ, 'c'},
  {"date", required_వాదం, ఖాళీ, 'd'},
  {"reference", required_వాదం, ఖాళీ, 'r'},
  {"no-dereference", no_వాదం, ఖాళీ, 'h'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* Valid arguments to the '--time' option. */
స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి సమయం_వాదనలు[] =
{
  "atime", "access", "use", "mtime", "modify", ఖాళీ
};

/* The bits in 'change_times' that those arguments set. */
స్థిర పూర్ణాంకం స్థిరరాశి సమయం_masks[] =
{
  CH_ATIME, CH_ATIME, CH_ATIME, CH_MTIME, CH_MTIME
};

/* The interpretation of FLEX_DATE as a date, relative to NOW.  */

స్థిర నిర్మాణం timespec
date_relative (అక్షరం స్థిరరాశి *flex_date, నిర్మాణం timespec now)
{
  నిర్మాణం timespec ఫలితం;
  ఒకవేళ (! parse_datetime (&ఫలితం, flex_date, &now))
    error (EXIT_FAILURE, 0, _("invalid date format %s"), quote (flex_date));
  తిరిగి ఫలితం;
}

/* Update the time of file FILE according to the options given.
   Return true if successful.  */

స్థిర bool
touch (అక్షరం స్థిరరాశి *file)
{
  పూర్ణాంకం fd = -1;
  పూర్ణాంకం open_errno = 0;
  నిర్మాణం timespec స్థిరరాశి *t = newtime;

  ఒకవేళ (streq (file, "-"))
    fd = STDOUT_FILENO;
  లేకపోతే ఒకవేళ (! (no_create || no_dereference))
    {
      /* Try to open FILE, creating it if necessary.  */
      fd = fd_reopen (STDIN_FILENO, file,
                      O_WRONLY | O_CREAT | O_NONBLOCK | O_NOCTTY, బహుళకం_RW_UGO);
      ఒకవేళ (fd < 0)
        open_errno = errno;
    }

  ఒకవేళ (change_times != (CH_ATIME | CH_MTIME))
    {
      /* We're setting only one of the time values.  */
      ఒకవేళ (change_times == CH_MTIME)
        newtime[0].tv_nsec = UTIME_OMIT;
      లేకపోతే
        {
          affirm (change_times == CH_ATIME);
          newtime[1].tv_nsec = UTIME_OMIT;
        }
    }

  ఒకవేళ (amtime_now)
    {
      /* Pass NULL to futimens so it will not fail if we have
         write access to the file, but don't own it.  */
      t = ఖాళీ;
    }

  అక్షరం స్థిరరాశి *file_opt = fd == STDOUT_FILENO ? ఖాళీ : file;
  పూర్ణాంకం atflag = no_dereference ? AT_SYMLINK_NOFOLLOW : 0;
  పూర్ణాంకం utime_errno = (fdutimensat (fd, AT_FDCWD, file_opt, t, atflag) == 0
                     ? 0 : errno);

  ఒకవేళ (fd == STDIN_FILENO)
    {
      ఒకవేళ (close (STDIN_FILENO) != 0)
        {
          error (0, errno, _("failed to close %s"), quoteaf (file));
          తిరిగి false;
        }
    }
  లేకపోతే ఒకవేళ (fd == STDOUT_FILENO)
    {
      /* Do not diagnose "touch -c - >&-".  */
      ఒకవేళ (utime_errno == EBADF && no_create)
        తిరిగి true;
    }

  ఒకవేళ (utime_errno != 0)
    {
      /* Don't diagnose with open_errno if FILE is a directory, as that
         would give a bogus diagnostic for e.g., 'touch /' (assuming we
         don't own / or have write access).  On Solaris 10 and probably
         other systems, opening a directory like "." fails with EINVAL.
         (On SunOS 4 it was EPERM but that's obsolete.)  On macOS 26
         opening "/" fails with EEXIST.  */
      నిర్మాణం stat st;
      ఒకవేళ (open_errno
          && ! (open_errno == EISDIR
                || ((open_errno == EINVAL || open_errno == EEXIST)
                    && stat (file, &st) == 0 && S_ISDIR (st.st_బహుళకం))))
        {
          /* The wording of this diagnostic should cover at least two cases:
             - the file does not exist, but the parent directory is unwritable
             - the file exists, but it isn't writable
             I think it's not worth trying to distinguish them.  */
          error (0, open_errno, _("cannot touch %s"), quoteaf (file));
        }
      లేకపోతే
        {
          ఒకవేళ (no_create && utime_errno == ENOENT)
            తిరిగి true;
          error (0, utime_errno, _("setting times of %s"), quoteaf (file));
        }
      తిరిగి false;
    }

  తిరిగి true;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... FILE...\n"), కార్యక్రమం_name);
      fputs (_("\
Update the access and modification times యొక్క each FILE గమ్యం the current సమయం.\n\
\n\
A FILE వాదం that does not exist is created ఖాళీలు, unless -c or -h\n\
is supplied.\n\
\n\
A FILE వాదం string యొక్క - is handled specially and causes touch గమ్యం\n\
change the times యొక్క the file associated with standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a\n\
         change only the access సమయం\n\
"));
      oputs (_("\
  -c, --no-create\n\
         చేయి not create any files\n\
"));
      oputs (_("\
  -d, --date=STRING\n\
         parse STRING and use it instead యొక్క current సమయం\n\
"));
      oputs (_("\
  -f\n\
         (ignored)\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link instead యొక్క any referenced file;\n\
         useful only on systems that can change the timestamps యొక్క a symlink\n\
"));
      oputs (_("\
  -m\n\
         change only the modification సమయం\n\
"));
      oputs (_("\
  -r, --reference=FILE\n\
         use this file's times instead యొక్క current సమయం\n\
"));
      oputs (_("\
  -t [[CC]YY]MMDDhhmm[.ss]\n\
         use specified సమయం instead యొక్క current సమయం,\n\
         with a date-సమయం format that differs from -d's\n\
"));
      oputs (_("\
      --సమయం=మాట\n\
         specify which సమయం గమ్యం change:\n\
         access సమయం (-a): 'access', 'atime', 'use';\n\
         modification సమయం (-m): 'modify', 'mtime'\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool date_సమితి = false;
  అక్షరం స్థిరరాశి *flex_date = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  పూర్ణాంకం c;
  అయితే ((c = getopt_long (argc, argv, "acd:fhmr:t:", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'a':
          change_times |= CH_ATIME;
          ఆపు;

        స్థితి 'c':
          no_create = true;
          ఆపు;

        స్థితి 'd':
          flex_date = optarg;
          ఆపు;

        స్థితి 'f':
          ఆపు;

        స్థితి 'h':
          no_dereference = true;
          ఆపు;

        స్థితి 'm':
          change_times |= CH_MTIME;
          ఆపు;

        స్థితి 'r':
          use_ref = true;
          ref_file = optarg;
          ఆపు;

        స్థితి 't':
          ఒకవేళ (! posixtime (&newtime[0].tv_sec, optarg,
                           PDS_LEADING_YEAR | PDS_CENTURY | PDS_SECONDS))
            error (EXIT_FAILURE, 0, _("invalid date format %s"),
                   quote (optarg));
          newtime[0].tv_nsec = 0;
          newtime[1] = newtime[0];
          date_సమితి = true;
          ఆపు;

        స్థితి సమయం_OPTION:	/* --time */
          change_times |= XARGMATCH ("--time", optarg,
                                     సమయం_వాదనలు, సమయం_masks);
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (change_times == 0)
    change_times = CH_ATIME | CH_MTIME;

  ఒకవేళ (date_సమితి && (use_ref || flex_date))
    {
      error (0, 0, _("cannot specify times from more than one source"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (use_ref)
    {
      నిర్మాణం stat ref_stats;
      /* Don't use (no_dereference?lstat:stat) (args), since stat
         might be an object-like macro.  */
      ఒకవేళ (no_dereference ? lstat (ref_file, &ref_stats)
          : stat (ref_file, &ref_stats))
        error (EXIT_FAILURE, errno,
               _("failed to get attributes of %s"), quoteaf (ref_file));
      newtime[0] = get_stat_atime (&ref_stats);
      newtime[1] = get_stat_mtime (&ref_stats);
      date_సమితి = true;
      ఒకవేళ (flex_date)
        {
          ఒకవేళ (change_times & CH_ATIME)
            newtime[0] = date_relative (flex_date, newtime[0]);
          ఒకవేళ (change_times & CH_MTIME)
            newtime[1] = date_relative (flex_date, newtime[1]);
        }
    }
  లేకపోతే
    {
      ఒకవేళ (flex_date)
        {
          నిర్మాణం timespec now = current_timespec ();
          newtime[1] = newtime[0] = date_relative (flex_date, now);
          date_సమితి = true;

          /* If neither -a nor -m is specified, treat "-d now" as if
             it were absent; this lets "touch" succeed more often in
             the presence of restrictive permissions.  */
          ఒకవేళ (change_times == (CH_ATIME | CH_MTIME)
              && timespec_cmp (newtime[0], now) == 0)
            {
              /* Check that it really was "-d now", and not a timestamp
                 that just happens to be the current time.  */
              నిర్మాణం timespec notnow, notnow1;
              notnow.tv_sec = now.tv_sec ^ 1;
              notnow.tv_nsec = now.tv_nsec;
              notnow1 = date_relative (flex_date, notnow);
              ఒకవేళ (timespec_cmp (notnow1, notnow) == 0)
                date_సమితి = false;
            }
        }
    }

  /* The obsolete 'MMDDhhmm[YY]' form is valid IFF there are
     two or more non-option arguments.  */
  ఒకవేళ (!date_సమితి && 2 <= argc - optind && posix2_సంస్కరణ () < 200112
      && posixtime (&newtime[0].tv_sec, argv[optind],
                    PDS_TRAILING_YEAR | PDS_PRE_2000))
    {
      newtime[0].tv_nsec = 0;
      newtime[1] = newtime[0];
      date_సమితి = true;

      ఒకవేళ (! getenv ("POSIXLY_CORRECT"))
        {
          నిర్మాణం tm స్థిరరాశి *tm = localtime (&newtime[0].tv_sec);

          /* Technically, it appears that even a deliberate attempt to cause
             the above localtime to return NULL will always fail because our
             posixtime implementation rejects all dates for which localtime
             would fail.  However, skip the warning if it ever fails.  */
          ఒకవేళ (tm)
            error (0, 0,
                   _("warning: 'touch %s' is obsolete; use "
                     "'touch -t %04ld%02d%02d%02d%02d.%02d'"),
                   argv[optind],
                   tm->tm_year + 1900L, tm->tm_mon + 1, tm->tm_mday,
                   tm->tm_hour, tm->tm_min, tm->tm_sec);
        }

      optind++;
    }

  ఒకవేళ (!date_సమితి)
    {
      ఒకవేళ (change_times == (CH_ATIME | CH_MTIME))
        amtime_now = true;
      లేకపోతే
        newtime[1].tv_nsec = newtime[0].tv_nsec = UTIME_NOW;
    }

  ఒకవేళ (optind == argc)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  కొరకు (; optind < argc; ++optind)
    ok &= touch (argv[optind]);

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
