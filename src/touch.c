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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "argmatch.h"
#شامل "assure.h"
#شامل "fd-reopen.h"
#شامل "parse-datetime.h"
#شامل "posixtm.h"
#شامل "posixver.h"
#شامل "quote.h"
#شامل "stat-time.h"
#شامل "utimens.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "touch"

#تعریف_کرو AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("Arnold Robbins"), \
  proper_name ("Jim Kingdon"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Randy Smith")

/* Bitmasks for 'change_times'. */
#تعریف_کرو CH_ATIME 1
#تعریف_کرو CH_MTIME 2

/* Which timestamps to change. */
ساکن صحیح change_times;

/* (-c) If true, don't create if not already there.  */
ساکن bool no_create;

/* (-r) If true, use times from a reference file.  */
ساکن bool use_ref;

/* (-h) If true, change the times of an existing symlink, if possible.  */
ساکن bool no_dereference;

/* If true, the only thing we have to do is change both the
   modification and access time to the current time, so we don't
   have to own the file, just be able to read and write it.
   On some systems, we can do this if we own the file, even though
   we have neither read nor write access to it.  */
ساکن bool amtime_now;

/* New access and modification times to use when setting time.  */
ساکن ساخت timespec newtime[2];

/* File to use for -r. */
ساکن حرف *ref_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  وقت_OPTION = CHAR_MAX + 1
};

ساکن ساخت option ثابت longopts[] =
{
  {"time", required_دلیل, تہی, وقت_OPTION},
  {"no-create", no_دلیل, تہی, 'c'},
  {"date", required_دلیل, تہی, 'd'},
  {"reference", required_دلیل, تہی, 'r'},
  {"no-dereference", no_دلیل, تہی, 'h'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* Valid arguments to the '--time' option. */
ساکن حرف ثابت *ثابت وقت_دلائل[] =
{
  "atime", "access", "use", "mtime", "modify", تہی
};

/* The bits in 'change_times' that those arguments set. */
ساکن صحیح ثابت وقت_masks[] =
{
  CH_ATIME, CH_ATIME, CH_ATIME, CH_MTIME, CH_MTIME
};

/* The interpretation of FLEX_DATE as a date, relative to NOW.  */

ساکن ساخت timespec
date_relative (حرف ثابت *flex_date, ساخت timespec now)
{
  ساخت timespec نتیجہ;
  اگر (! parse_datetime (&نتیجہ, flex_date, &now))
    error (EXIT_FAILURE, 0, _("invalid date format %s"), quote (flex_date));
  واپس نتیجہ;
}

/* Update the time of file FILE according to the options given.
   Return true if successful.  */

ساکن bool
touch (حرف ثابت *file)
{
  صحیح fd = -1;
  صحیح open_errno = 0;
  ساخت timespec ثابت *t = newtime;

  اگر (streq (file, "-"))
    fd = STDOUT_FILENO;
  ورنہ اگر (! (no_create || no_dereference))
    {
      /* Try to open FILE, creating it if necessary.  */
      fd = fd_reopen (STDIN_FILENO, file,
                      O_WRONLY | O_CREAT | O_NONBLOCK | O_NOCTTY, منوال_RW_UGO);
      اگر (fd < 0)
        open_errno = errno;
    }

  اگر (change_times != (CH_ATIME | CH_MTIME))
    {
      /* We're setting only one of the time values.  */
      اگر (change_times == CH_MTIME)
        newtime[0].tv_nsec = UTIME_OMIT;
      ورنہ
        {
          affirm (change_times == CH_ATIME);
          newtime[1].tv_nsec = UTIME_OMIT;
        }
    }

  اگر (amtime_now)
    {
      /* Pass NULL to futimens so it will not fail if we have
         write access to the file, but don't own it.  */
      t = تہی;
    }

  حرف ثابت *file_opt = fd == STDOUT_FILENO ? تہی : file;
  صحیح atflag = no_dereference ? AT_SYMLINK_NOFOLLOW : 0;
  صحیح utime_errno = (fdutimensat (fd, AT_FDCWD, file_opt, t, atflag) == 0
                     ? 0 : errno);

  اگر (fd == STDIN_FILENO)
    {
      اگر (close (STDIN_FILENO) != 0)
        {
          error (0, errno, _("failed to close %s"), quoteaf (file));
          واپس false;
        }
    }
  ورنہ اگر (fd == STDOUT_FILENO)
    {
      /* Do not diagnose "touch -c - >&-".  */
      اگر (utime_errno == EBADF && no_create)
        واپس true;
    }

  اگر (utime_errno != 0)
    {
      /* Don't diagnose with open_errno if FILE is a directory, as that
         would give a bogus diagnostic for e.g., 'touch /' (assuming we
         don't own / or have write access).  On Solaris 10 and probably
         other systems, opening a directory like "." fails with EINVAL.
         (On SunOS 4 it was EPERM but that's obsolete.)  On macOS 26
         opening "/" fails with EEXIST.  */
      ساخت stat st;
      اگر (open_errno
          && ! (open_errno == EISDIR
                || ((open_errno == EINVAL || open_errno == EEXIST)
                    && stat (file, &st) == 0 && S_ISDIR (st.st_منوال))))
        {
          /* The wording of this diagnostic should cover at least two cases:
             - the file does not exist, but the parent directory is unwritable
             - the file exists, but it isn't writable
             I think it's not worth trying to distinguish them.  */
          error (0, open_errno, _("cannot touch %s"), quoteaf (file));
        }
      ورنہ
        {
          اگر (no_create && utime_errno == ENOENT)
            واپس true;
          error (0, utime_errno, _("setting times of %s"), quoteaf (file));
        }
      واپس false;
    }

  واپس true;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... FILE...\n"), پروگرام_name);
      fputs (_("\
Update the access and modification times کا each FILE مقصد the current وقت.\n\
\n\
A FILE دلیل that does not exist is created خالی_خانے, unless -c or -h\n\
is supplied.\n\
\n\
A FILE دلیل string کا - is handled specially and causes touch مقصد\n\
change the times کا the file associated with standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a\n\
         change only the access وقت\n\
"));
      oputs (_("\
  -c, --no-create\n\
         کرو not create any files\n\
"));
      oputs (_("\
  -d, --date=STRING\n\
         parse STRING and use it instead کا current وقت\n\
"));
      oputs (_("\
  -f\n\
         (ignored)\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link instead کا any referenced file;\n\
         useful only on systems that can change the timestamps کا a symlink\n\
"));
      oputs (_("\
  -m\n\
         change only the modification وقت\n\
"));
      oputs (_("\
  -r, --reference=FILE\n\
         use this file's times instead کا current وقت\n\
"));
      oputs (_("\
  -t [[CC]YY]MMDDhhmm[.ss]\n\
         use specified وقت instead کا current وقت,\n\
         with a date-وقت format that differs from -d's\n\
"));
      oputs (_("\
      --وقت=لفظ\n\
         specify which وقت مقصد change:\n\
         access وقت (-a): 'access', 'atime', 'use';\n\
         modification وقت (-m): 'modify', 'mtime'\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool date_سیٹ = false;
  حرف ثابت *flex_date = تہی;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحیح c;
  جب ((c = getopt_long (argc, argv, "acd:fhmr:t:", longopts, تہی)) != -1)
    {
      انتخاب (c)
        {
        حالت 'a':
          change_times |= CH_ATIME;
          توڑو;

        حالت 'c':
          no_create = true;
          توڑو;

        حالت 'd':
          flex_date = optarg;
          توڑو;

        حالت 'f':
          توڑو;

        حالت 'h':
          no_dereference = true;
          توڑو;

        حالت 'm':
          change_times |= CH_MTIME;
          توڑو;

        حالت 'r':
          use_ref = true;
          ref_file = optarg;
          توڑو;

        حالت 't':
          اگر (! posixtime (&newtime[0].tv_sec, optarg,
                           PDS_LEADING_YEAR | PDS_CENTURY | PDS_SECONDS))
            error (EXIT_FAILURE, 0, _("invalid date format %s"),
                   quote (optarg));
          newtime[0].tv_nsec = 0;
          newtime[1] = newtime[0];
          date_سیٹ = true;
          توڑو;

        حالت وقت_OPTION:	/* --time */
          change_times |= XARGMATCH ("--time", optarg,
                                     وقت_دلائل, وقت_masks);
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (change_times == 0)
    change_times = CH_ATIME | CH_MTIME;

  اگر (date_سیٹ && (use_ref || flex_date))
    {
      error (0, 0, _("cannot specify times from more than one source"));
      usage (EXIT_FAILURE);
    }

  اگر (use_ref)
    {
      ساخت stat ref_stats;
      /* Don't use (no_dereference?lstat:stat) (args), since stat
         might be an object-like macro.  */
      اگر (no_dereference ? lstat (ref_file, &ref_stats)
          : stat (ref_file, &ref_stats))
        error (EXIT_FAILURE, errno,
               _("failed to get attributes of %s"), quoteaf (ref_file));
      newtime[0] = get_stat_atime (&ref_stats);
      newtime[1] = get_stat_mtime (&ref_stats);
      date_سیٹ = true;
      اگر (flex_date)
        {
          اگر (change_times & CH_ATIME)
            newtime[0] = date_relative (flex_date, newtime[0]);
          اگر (change_times & CH_MTIME)
            newtime[1] = date_relative (flex_date, newtime[1]);
        }
    }
  ورنہ
    {
      اگر (flex_date)
        {
          ساخت timespec now = current_timespec ();
          newtime[1] = newtime[0] = date_relative (flex_date, now);
          date_سیٹ = true;

          /* If neither -a nor -m is specified, treat "-d now" as if
             it were absent; this lets "touch" succeed more often in
             the presence of restrictive permissions.  */
          اگر (change_times == (CH_ATIME | CH_MTIME)
              && timespec_cmp (newtime[0], now) == 0)
            {
              /* Check that it really was "-d now", and not a timestamp
                 that just happens to be the current time.  */
              ساخت timespec notnow, notnow1;
              notnow.tv_sec = now.tv_sec ^ 1;
              notnow.tv_nsec = now.tv_nsec;
              notnow1 = date_relative (flex_date, notnow);
              اگر (timespec_cmp (notnow1, notnow) == 0)
                date_سیٹ = false;
            }
        }
    }

  /* The obsolete 'MMDDhhmm[YY]' form is valid IFF there are
     two or more non-option arguments.  */
  اگر (!date_سیٹ && 2 <= argc - optind && posix2_نسخہ () < 200112
      && posixtime (&newtime[0].tv_sec, argv[optind],
                    PDS_TRAILING_YEAR | PDS_PRE_2000))
    {
      newtime[0].tv_nsec = 0;
      newtime[1] = newtime[0];
      date_سیٹ = true;

      اگر (! getenv ("POSIXLY_CORRECT"))
        {
          ساخت tm ثابت *tm = localtime (&newtime[0].tv_sec);

          /* Technically, it appears that even a deliberate attempt to cause
             the above localtime to return NULL will always fail because our
             posixtime implementation rejects all dates for which localtime
             would fail.  However, skip the warning if it ever fails.  */
          اگر (tm)
            error (0, 0,
                   _("warning: 'touch %s' is obsolete; use "
                     "'touch -t %04ld%02d%02d%02d%02d.%02d'"),
                   argv[optind],
                   tm->tm_year + 1900L, tm->tm_mon + 1, tm->tm_mday,
                   tm->tm_hour, tm->tm_min, tm->tm_sec);
        }

      optind++;
    }

  اگر (!date_سیٹ)
    {
      اگر (change_times == (CH_ATIME | CH_MTIME))
        amtime_now = true;
      ورنہ
        newtime[1].tv_nsec = newtime[0].tv_nsec = UTIME_NOW;
    }

  اگر (optind == argc)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  برائے (; optind < argc; ++optind)
    ok &= touch (argv[optind]);

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
