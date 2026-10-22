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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "argmatch.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "fd-reopen.h"
#अन्तर्भाव "parse-datetime.h"
#अन्तर्भाव "posixtm.h"
#अन्तर्भाव "posixver.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "stat-time.h"
#अन्तर्भाव "utimens.h"

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
स्थिर पूर्णांक change_times;

/* (-c) If true, don't create if not already there.  */
स्थिर bool no_create;

/* (-r) If true, use times from a reference file.  */
स्थिर bool use_ref;

/* (-h) If true, change the times of an existing symlink, if possible.  */
स्थिर bool no_dereference;

/* If true, the only thing we have to do is change both the
   modification and access time to the current time, so we don't
   have to own the file, just be able to read and write it.
   On some systems, we can do this if we own the file, even though
   we have neither read nor write access to it.  */
स्थिर bool amtime_now;

/* New access and modification times to use when setting time.  */
स्थिर संरचना timespec newtime[2];

/* File to use for -r. */
स्थिर अक्षर *ref_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  TIME_OPTION = अक्षर_उच्च + 1
};

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"time", required_argument, रिक्त, TIME_OPTION},
  {"no-create", no_argument, रिक्त, 'c'},
  {"date", required_argument, रिक्त, 'd'},
  {"reference", required_argument, रिक्त, 'r'},
  {"no-dereference", no_argument, रिक्त, 'h'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

/* Valid arguments to the '--time' option. */
स्थिर अक्षर स्थिरांक *स्थिरांक time_args[] =
{
  "atime", "access", "use", "mtime", "modify", रिक्त
};

/* The bits in 'change_times' that those arguments set. */
स्थिर पूर्णांक स्थिरांक time_masks[] =
{
  CH_ATIME, CH_ATIME, CH_ATIME, CH_MTIME, CH_MTIME
};

/* The interpretation of FLEX_DATE as a date, relative to NOW.  */

स्थिर संरचना timespec
date_relative (अक्षर स्थिरांक *flex_date, संरचना timespec now)
{
  संरचना timespec result;
  जर (! parse_datetime (&result, flex_date, &now))
    error (निकास_त्रुटि, 0, _("invalid date format %s"), quote (flex_date));
  परत result;
}

/* Update the time of file FILE according to the options given.
   Return true if successful.  */

स्थिर bool
touch (अक्षर स्थिरांक *file)
{
  पूर्णांक fd = -1;
  पूर्णांक open_errno = 0;
  संरचना timespec स्थिरांक *t = newtime;

  जर (streq (file, "-"))
    fd = STDOUT_FILENO;
  अन्यथा जर (! (no_create || no_dereference))
    {
      /* Try to open FILE, creating it if necessary.  */
      fd = fd_reopen (STDIN_FILENO, file,
                      O_WRONLY | O_CREAT | O_NONBLOCK | O_NOCTTY, MODE_RW_UGO);
      जर (fd < 0)
        open_errno = त्रुटि_सं;
    }

  जर (change_times != (CH_ATIME | CH_MTIME))
    {
      /* We're setting only one of the time values.  */
      जर (change_times == CH_MTIME)
        newtime[0].tv_nsec = UTIME_OMIT;
      अन्यथा
        {
          affirm (change_times == CH_ATIME);
          newtime[1].tv_nsec = UTIME_OMIT;
        }
    }

  जर (amtime_now)
    {
      /* Pass NULL to futimens so it will not fail if we have
         write access to the file, but don't own it.  */
      t = रिक्त;
    }

  अक्षर स्थिरांक *file_opt = fd == STDOUT_FILENO ? रिक्त : file;
  पूर्णांक atflag = no_dereference ? AT_SYMLINK_NOFOLLOW : 0;
  पूर्णांक utime_errno = (fdutimensat (fd, AT_FDCWD, file_opt, t, atflag) == 0
                     ? 0 : त्रुटि_सं);

  जर (fd == STDIN_FILENO)
    {
      जर (बंद (STDIN_FILENO) != 0)
        {
          error (0, त्रुटि_सं, _("failed to close %s"), quoteaf (file));
          परत false;
        }
    }
  अन्यथा जर (fd == STDOUT_FILENO)
    {
      /* Do not diagnose "touch -c - >&-".  */
      जर (utime_errno == EBADF && no_create)
        परत true;
    }

  जर (utime_errno != 0)
    {
      /* Don't diagnose with open_errno if FILE is a directory, as that
         would give a bogus diagnostic for e.g., 'touch /' (assuming we
         don't own / or have write access).  On Solaris 10 and probably
         other systems, opening a directory like "." fails with EINVAL.
         (On SunOS 4 it was EPERM but that's obsolete.)  On macOS 26
         opening "/" fails with EEXIST.  */
      संरचना stat st;
      जर (open_errno
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
      अन्यथा
        {
          जर (no_create && utime_errno == ENOENT)
            परत true;
          error (0, utime_errno, _("setting times of %s"), quoteaf (file));
        }
      परत false;
    }

  परत true;
}

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      ख_माला_दो (_("\
Update the access and modification बार of each खाता to the current समय.\n\
\n\
A खाता argument that does not exist is created empty, unless -c or -h\n\
is supplied.\n\
\n\
A खाता argument string of - is handled specially and causes touch to\n\
change the बार of the file associated with standard output.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a\n\
         change only the access समय\n\
"));
      oputs (_("\
  -c, --no-create\n\
         करा not create any files\n\
"));
      oputs (_("\
  -d, --date=STRING\n\
         parse STRING and use it instead of current समय\n\
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
         change only the modification समय\n\
"));
      oputs (_("\
  -r, --reference=खाता\n\
         use this file's बार instead of current समय\n\
"));
      oputs (_("\
  -t [[CC]YY]MMDDhhmm[.ss]\n\
         use specified समय instead of current समय,\n\
         with a date-समय format that differs from -d's\n\
"));
      oputs (_("\
      --समय=WORD\n\
         specify which समय to change:\n\
         access समय (-a): 'access', 'atime', 'use';\n\
         modification समय (-m): 'modify', 'mtime'\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  bool date_set = false;
  अक्षर स्थिरांक *flex_date = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पूर्णांक c;
  पर्यंत ((c = getopt_long (argc, argv, "acd:fhmr:t:", longopts, रिक्त)) != -1)
    {
      निवड (c)
        {
        स्थिती 'a':
          change_times |= CH_ATIME;
          थांबा;

        स्थिती 'c':
          no_create = true;
          थांबा;

        स्थिती 'd':
          flex_date = optarg;
          थांबा;

        स्थिती 'f':
          थांबा;

        स्थिती 'h':
          no_dereference = true;
          थांबा;

        स्थिती 'm':
          change_times |= CH_MTIME;
          थांबा;

        स्थिती 'r':
          use_ref = true;
          ref_file = optarg;
          थांबा;

        स्थिती 't':
          जर (! posixtime (&newtime[0].tv_sec, optarg,
                           PDS_LEADING_YEAR | PDS_CENTURY | PDS_SECONDS))
            error (निकास_त्रुटि, 0, _("invalid date format %s"),
                   quote (optarg));
          newtime[0].tv_nsec = 0;
          newtime[1] = newtime[0];
          date_set = true;
          थांबा;

        स्थिती TIME_OPTION:	/* --time */
          change_times |= XARGMATCH ("--time", optarg,
                                     time_args, time_masks);
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (change_times == 0)
    change_times = CH_ATIME | CH_MTIME;

  जर (date_set && (use_ref || flex_date))
    {
      error (0, 0, _("cannot specify times from more than one source"));
      usage (निकास_त्रुटि);
    }

  जर (use_ref)
    {
      संरचना stat ref_stats;
      /* Don't use (no_dereference?lstat:stat) (args), since stat
         might be an object-like macro.  */
      जर (no_dereference ? lstat (ref_file, &ref_stats)
          : stat (ref_file, &ref_stats))
        error (निकास_त्रुटि, त्रुटि_सं,
               _("failed to get attributes of %s"), quoteaf (ref_file));
      newtime[0] = get_stat_atime (&ref_stats);
      newtime[1] = get_stat_mtime (&ref_stats);
      date_set = true;
      जर (flex_date)
        {
          जर (change_times & CH_ATIME)
            newtime[0] = date_relative (flex_date, newtime[0]);
          जर (change_times & CH_MTIME)
            newtime[1] = date_relative (flex_date, newtime[1]);
        }
    }
  अन्यथा
    {
      जर (flex_date)
        {
          संरचना timespec now = current_timespec ();
          newtime[1] = newtime[0] = date_relative (flex_date, now);
          date_set = true;

          /* If neither -a nor -m is specified, treat "-d now" as if
             it were absent; this lets "touch" succeed more often in
             the presence of restrictive permissions.  */
          जर (change_times == (CH_ATIME | CH_MTIME)
              && timespec_cmp (newtime[0], now) == 0)
            {
              /* Check that it really was "-d now", and not a timestamp
                 that just happens to be the current time.  */
              संरचना timespec notnow, notnow1;
              notnow.tv_sec = now.tv_sec ^ 1;
              notnow.tv_nsec = now.tv_nsec;
              notnow1 = date_relative (flex_date, notnow);
              जर (timespec_cmp (notnow1, notnow) == 0)
                date_set = false;
            }
        }
    }

  /* The obsolete 'MMDDhhmm[YY]' form is valid IFF there are
     two or more non-option arguments.  */
  जर (!date_set && 2 <= argc - optind && posix2_version () < 200112
      && posixtime (&newtime[0].tv_sec, argv[optind],
                    PDS_TRAILING_YEAR | PDS_PRE_2000))
    {
      newtime[0].tv_nsec = 0;
      newtime[1] = newtime[0];
      date_set = true;

      जर (! दो_पर्या ("POSIXLY_CORRECT"))
        {
          संरचना पंचांग स्थिरांक *पंचांग = स_स्थानीय (&newtime[0].tv_sec);

          /* Technically, it appears that even a deliberate attempt to cause
             the above localtime to return NULL will always fail because our
             posixtime implementation rejects all dates for which localtime
             would fail.  However, skip the warning if it ever fails.  */
          जर (पंचांग)
            error (0, 0,
                   _("warning: 'touch %s' is obsolete; use "
                     "'touch -t %04ld%02d%02d%02d%02d.%02d'"),
                   argv[optind],
                   पंचांग->tm_year + 1900L, पंचांग->tm_mon + 1, पंचांग->tm_mday,
                   पंचांग->tm_hour, पंचांग->tm_min, पंचांग->tm_sec);
        }

      optind++;
    }

  जर (!date_set)
    {
      जर (change_times == (CH_ATIME | CH_MTIME))
        amtime_now = true;
      अन्यथा
        newtime[1].tv_nsec = newtime[0].tv_nsec = UTIME_NOW;
    }

  जर (optind == argc)
    {
      error (0, 0, _("missing file operand"));
      usage (निकास_त्रुटि);
    }

  bool ok = true;
  साठी (; optind < argc; ++optind)
    ok &= touch (argv[optind]);

  परत ok ? निकास_सफल : निकास_त्रुटि;
}
