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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "argmatch.h"
#ضمن "assure.h"
#ضمن "fd-reopen.h"
#ضمن "parse-datetime.h"
#ضمن "posixtm.h"
#ضمن "posixver.h"
#ضمن "quote.h"
#ضمن "stat-time.h"
#ضمن "utimens.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "touch"

#تعریف_کن AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("Arnold Robbins"), \
  proper_name ("Jim Kingdon"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Randy Smith")

/* Bitmasks for 'change_times'. */
#تعریف_کن CH_ATIME 1
#تعریف_کن CH_MTIME 2

/* Which timestamps to change. */
ساكن صحيح change_times;

/* (-c) If true, don't create if not already there.  */
ساكن bool no_create;

/* (-r) If true, use times from a reference file.  */
ساكن bool use_ref;

/* (-h) If true, change the times of an existing symlink, if possible.  */
ساكن bool no_dereference;

/* If true, the only thing we have to do is change both the
   modification and access time to the current time, so we don't
   have to own the file, just be able to read and write it.
   On some systems, we can do this if we own the file, even though
   we have neither read nor write access to it.  */
ساكن bool amtime_now;

/* New access and modification times to use when setting time.  */
ساكن بنية timespec newtime[2];

/* File to use for -r. */
ساكن حرف *ref_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  زمان_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"time", required_آرگومان, عدم, زمان_OPTION},
  {"no-create", no_آرگومان, عدم, 'c'},
  {"date", required_آرگومان, عدم, 'd'},
  {"reference", required_آرگومان, عدم, 'r'},
  {"no-dereference", no_آرگومان, عدم, 'h'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

/* Valid arguments to the '--time' option. */
ساكن حرف ثابت *ثابت زمان_آرگومانها[] =
{
  "atime", "access", "use", "mtime", "modify", عدم
};

/* The bits in 'change_times' that those arguments set. */
ساكن صحيح ثابت زمان_masks[] =
{
  CH_ATIME, CH_ATIME, CH_ATIME, CH_MTIME, CH_MTIME
};

/* The interpretation of FLEX_DATE as a date, relative to NOW.  */

ساكن بنية timespec
date_relative (حرف ثابت *flex_date, بنية timespec now)
{
  بنية timespec نتیجه;
  إذا (! parse_datetime (&نتیجه, flex_date, &now))
    error (EXIT_FAILURE, 0, _("invalid date format %s"), quote (flex_date));
  أرجع نتیجه;
}

/* Update the time of file FILE according to the options given.
   Return true if successful.  */

ساكن bool
touch (حرف ثابت *file)
{
  صحيح fd = -1;
  صحيح open_errno = 0;
  بنية timespec ثابت *t = newtime;

  إذا (streq (file, "-"))
    fd = STDOUT_FILENO;
  وإلا إذا (! (no_create || no_dereference))
    {
      /* Try to open FILE, creating it if necessary.  */
      fd = fd_reopen (STDIN_FILENO, file,
                      O_WRONLY | O_CREAT | O_NONBLOCK | O_NOCTTY, نما_RW_UGO);
      إذا (fd < 0)
        open_errno = errno;
    }

  إذا (change_times != (CH_ATIME | CH_MTIME))
    {
      /* We're setting only one of the time values.  */
      إذا (change_times == CH_MTIME)
        newtime[0].tv_nsec = UTIME_OMIT;
      وإلا
        {
          affirm (change_times == CH_ATIME);
          newtime[1].tv_nsec = UTIME_OMIT;
        }
    }

  إذا (amtime_now)
    {
      /* Pass NULL to futimens so it will not fail if we have
         write access to the file, but don't own it.  */
      t = عدم;
    }

  حرف ثابت *file_opt = fd == STDOUT_FILENO ? عدم : file;
  صحيح atflag = no_dereference ? AT_SYMLINK_NOFOLLOW : 0;
  صحيح utime_errno = (fdutimensat (fd, AT_FDCWD, file_opt, t, atflag) == 0
                     ? 0 : errno);

  إذا (fd == STDIN_FILENO)
    {
      إذا (close (STDIN_FILENO) != 0)
        {
          error (0, errno, _("failed to close %s"), quoteaf (file));
          أرجع false;
        }
    }
  وإلا إذا (fd == STDOUT_FILENO)
    {
      /* Do not diagnose "touch -c - >&-".  */
      إذا (utime_errno == EBADF && no_create)
        أرجع true;
    }

  إذا (utime_errno != 0)
    {
      /* Don't diagnose with open_errno if FILE is a directory, as that
         would give a bogus diagnostic for e.g., 'touch /' (assuming we
         don't own / or have write access).  On Solaris 10 and probably
         other systems, opening a directory like "." fails with EINVAL.
         (On SunOS 4 it was EPERM but that's obsolete.)  On macOS 26
         opening "/" fails with EEXIST.  */
      بنية stat st;
      إذا (open_errno
          && ! (open_errno == EISDIR
                || ((open_errno == EINVAL || open_errno == EEXIST)
                    && stat (file, &st) == 0 && S_ISDIR (st.st_نما))))
        {
          /* The wording of this diagnostic should cover at least two cases:
             - the file does not exist, but the parent directory is unwritable
             - the file exists, but it isn't writable
             I think it's not worth trying to distinguish them.  */
          error (0, open_errno, _("cannot touch %s"), quoteaf (file));
        }
      وإلا
        {
          إذا (no_create && utime_errno == ENOENT)
            أرجع true;
          error (0, utime_errno, _("setting times of %s"), quoteaf (file));
        }
      أرجع false;
    }

  أرجع true;
}

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("Usage: %s [OPTION]... FILE...\n"), برنامه_name);
      fputs (_("\
Update the access and modification times از each FILE مقصد the current زمان.\n\
\n\
A FILE آرگومان that does not exist is created خالی, unless -c or -h\n\
is supplied.\n\
\n\
A FILE آرگومان string از - is handled specially and causes touch مقصد\n\
change the times از the file associated with standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a\n\
         change only the access زمان\n\
"));
      oputs (_("\
  -c, --no-create\n\
         افعل not create any files\n\
"));
      oputs (_("\
  -d, --date=STRING\n\
         parse STRING and use it instead از current زمان\n\
"));
      oputs (_("\
  -f\n\
         (ignored)\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link instead از any referenced file;\n\
         useful only on systems that can change the timestamps از a symlink\n\
"));
      oputs (_("\
  -m\n\
         change only the modification زمان\n\
"));
      oputs (_("\
  -r, --reference=FILE\n\
         use this file's times instead از current زمان\n\
"));
      oputs (_("\
  -t [[CC]YY]MMDDhhmm[.ss]\n\
         use specified زمان instead از current زمان,\n\
         with a date-زمان format that differs from -d's\n\
"));
      oputs (_("\
      --زمان=کلمه\n\
         specify which زمان مقصد change:\n\
         access زمان (-a): 'access', 'atime', 'use';\n\
         modification زمان (-m): 'modify', 'mtime'\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  bool date_مجموعه = false;
  حرف ثابت *flex_date = عدم;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحيح c;
  طالما ((c = getopt_long (argc, argv, "acd:fhmr:t:", longopts, عدم)) != -1)
    {
      بدل (c)
        {
        حالة 'a':
          change_times |= CH_ATIME;
          اكسر;

        حالة 'c':
          no_create = true;
          اكسر;

        حالة 'd':
          flex_date = optarg;
          اكسر;

        حالة 'f':
          اكسر;

        حالة 'h':
          no_dereference = true;
          اكسر;

        حالة 'm':
          change_times |= CH_MTIME;
          اكسر;

        حالة 'r':
          use_ref = true;
          ref_file = optarg;
          اكسر;

        حالة 't':
          إذا (! posixtime (&newtime[0].tv_sec, optarg,
                           PDS_LEADING_YEAR | PDS_CENTURY | PDS_SECONDS))
            error (EXIT_FAILURE, 0, _("invalid date format %s"),
                   quote (optarg));
          newtime[0].tv_nsec = 0;
          newtime[1] = newtime[0];
          date_مجموعه = true;
          اكسر;

        حالة زمان_OPTION:	/* --time */
          change_times |= XARGMATCH ("--time", optarg,
                                     زمان_آرگومانها, زمان_masks);
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (change_times == 0)
    change_times = CH_ATIME | CH_MTIME;

  إذا (date_مجموعه && (use_ref || flex_date))
    {
      error (0, 0, _("cannot specify times from more than one source"));
      usage (EXIT_FAILURE);
    }

  إذا (use_ref)
    {
      بنية stat ref_stats;
      /* Don't use (no_dereference?lstat:stat) (args), since stat
         might be an object-like macro.  */
      إذا (no_dereference ? lstat (ref_file, &ref_stats)
          : stat (ref_file, &ref_stats))
        error (EXIT_FAILURE, errno,
               _("failed to get attributes of %s"), quoteaf (ref_file));
      newtime[0] = get_stat_atime (&ref_stats);
      newtime[1] = get_stat_mtime (&ref_stats);
      date_مجموعه = true;
      إذا (flex_date)
        {
          إذا (change_times & CH_ATIME)
            newtime[0] = date_relative (flex_date, newtime[0]);
          إذا (change_times & CH_MTIME)
            newtime[1] = date_relative (flex_date, newtime[1]);
        }
    }
  وإلا
    {
      إذا (flex_date)
        {
          بنية timespec now = current_timespec ();
          newtime[1] = newtime[0] = date_relative (flex_date, now);
          date_مجموعه = true;

          /* If neither -a nor -m is specified, treat "-d now" as if
             it were absent; this lets "touch" succeed more often in
             the presence of restrictive permissions.  */
          إذا (change_times == (CH_ATIME | CH_MTIME)
              && timespec_cmp (newtime[0], now) == 0)
            {
              /* Check that it really was "-d now", and not a timestamp
                 that just happens to be the current time.  */
              بنية timespec notnow, notnow1;
              notnow.tv_sec = now.tv_sec ^ 1;
              notnow.tv_nsec = now.tv_nsec;
              notnow1 = date_relative (flex_date, notnow);
              إذا (timespec_cmp (notnow1, notnow) == 0)
                date_مجموعه = false;
            }
        }
    }

  /* The obsolete 'MMDDhhmm[YY]' form is valid IFF there are
     two or more non-option arguments.  */
  إذا (!date_مجموعه && 2 <= argc - optind && posix2_نسخه () < 200112
      && posixtime (&newtime[0].tv_sec, argv[optind],
                    PDS_TRAILING_YEAR | PDS_PRE_2000))
    {
      newtime[0].tv_nsec = 0;
      newtime[1] = newtime[0];
      date_مجموعه = true;

      إذا (! getenv ("POSIXLY_CORRECT"))
        {
          بنية tm ثابت *tm = localtime (&newtime[0].tv_sec);

          /* Technically, it appears that even a deliberate attempt to cause
             the above localtime to return NULL will always fail because our
             posixtime implementation rejects all dates for which localtime
             would fail.  However, skip the warning if it ever fails.  */
          إذا (tm)
            error (0, 0,
                   _("warning: 'touch %s' is obsolete; use "
                     "'touch -t %04ld%02d%02d%02d%02d.%02d'"),
                   argv[optind],
                   tm->tm_year + 1900L, tm->tm_mon + 1, tm->tm_mday,
                   tm->tm_hour, tm->tm_min, tm->tm_sec);
        }

      optind++;
    }

  إذا (!date_مجموعه)
    {
      إذا (change_times == (CH_ATIME | CH_MTIME))
        amtime_now = true;
      وإلا
        newtime[1].tv_nsec = newtime[0].tv_nsec = UTIME_NOW;
    }

  إذا (optind == argc)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  لكل (; optind < argc; ++optind)
    ok &= touch (argv[optind]);

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
