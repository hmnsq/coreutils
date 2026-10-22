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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "argmatch.h"
#समावेश "assure.h"
#समावेश "fd-reopen.h"
#समावेश "parse-datetime.h"
#समावेश "posixtm.h"
#समावेश "posixver.h"
#समावेश "quote.h"
#समावेश "stat-time.h"
#समावेश "utimens.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "touch"

#घोषणा AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("Arnold Robbins"), \
  proper_name ("Jim Kingdon"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Randy Smith")

/* Bitmasks for 'change_times'. */
#घोषणा CH_ATIME 1
#घोषणा CH_MTIME 2

/* Which timestamps to change. */
अटल पूर्णांक change_times;

/* (-c) If true, don't create if not already there.  */
अटल bool no_create;

/* (-r) If true, use times from a reference file.  */
अटल bool use_ref;

/* (-h) If true, change the times of an existing symlink, if possible.  */
अटल bool no_dereference;

/* If true, the only thing we have to do is change both the
   modification and access time to the current time, so we don't
   have to own the file, just be able to read and write it.
   On some systems, we can do this if we own the file, even though
   we have neither read nor write access to it.  */
अटल bool amtime_now;

/* New access and modification times to use when setting time.  */
अटल काष्ठा timespec newtime[2];

/* File to use for -r. */
अटल अक्षर *ref_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  TIME_OPTION = अक्षर_उच्च + 1
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"time", required_argument, शून्य, TIME_OPTION},
  {"no-create", no_argument, शून्य, 'c'},
  {"date", required_argument, शून्य, 'd'},
  {"reference", required_argument, शून्य, 'r'},
  {"no-dereference", no_argument, शून्य, 'h'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

/* Valid arguments to the '--time' option. */
अटल अक्षर स्थिर *स्थिर time_args[] =
{
  "atime", "access", "use", "mtime", "modify", शून्य
};

/* The bits in 'change_times' that those arguments set. */
अटल पूर्णांक स्थिर time_masks[] =
{
  CH_ATIME, CH_ATIME, CH_ATIME, CH_MTIME, CH_MTIME
};

/* The interpretation of FLEX_DATE as a date, relative to NOW.  */

अटल काष्ठा timespec
date_relative (अक्षर स्थिर *flex_date, काष्ठा timespec now)
{
  काष्ठा timespec परिणाम;
  अगर (! parse_datetime (&परिणाम, flex_date, &now))
    error (निकास_त्रुटि, 0, _("invalid date format %s"), quote (flex_date));
  वापस परिणाम;
}

/* Update the time of file FILE according to the options given.
   Return true if successful.  */

अटल bool
touch (अक्षर स्थिर *file)
{
  पूर्णांक fd = -1;
  पूर्णांक open_errno = 0;
  काष्ठा timespec स्थिर *t = newtime;

  अगर (streq (file, "-"))
    fd = STDOUT_FILENO;
  अन्यथा अगर (! (no_create || no_dereference))
    {
      /* Try to open FILE, creating it if necessary.  */
      fd = fd_reopen (STDIN_FILENO, file,
                      O_WRONLY | O_CREAT | O_NONBLOCK | O_NOCTTY, MODE_RW_UGO);
      अगर (fd < 0)
        open_errno = त्रुटि_सं;
    }

  अगर (change_times != (CH_ATIME | CH_MTIME))
    {
      /* We're setting only one of the time values.  */
      अगर (change_times == CH_MTIME)
        newtime[0].tv_nsec = UTIME_OMIT;
      अन्यथा
        {
          affirm (change_times == CH_ATIME);
          newtime[1].tv_nsec = UTIME_OMIT;
        }
    }

  अगर (amtime_now)
    {
      /* Pass NULL to futimens so it will not fail if we have
         write access to the file, but don't own it.  */
      t = शून्य;
    }

  अक्षर स्थिर *file_opt = fd == STDOUT_FILENO ? शून्य : file;
  पूर्णांक atflag = no_dereference ? AT_SYMLINK_NOFOLLOW : 0;
  पूर्णांक utime_errno = (fdutimensat (fd, AT_FDCWD, file_opt, t, atflag) == 0
                     ? 0 : त्रुटि_सं);

  अगर (fd == STDIN_FILENO)
    {
      अगर (बंद (STDIN_FILENO) != 0)
        {
          error (0, त्रुटि_सं, _("failed to close %s"), quoteaf (file));
          वापस false;
        }
    }
  अन्यथा अगर (fd == STDOUT_FILENO)
    {
      /* Do not diagnose "touch -c - >&-".  */
      अगर (utime_errno == EBADF && no_create)
        वापस true;
    }

  अगर (utime_errno != 0)
    {
      /* Don't diagnose with open_errno if FILE is a directory, as that
         would give a bogus diagnostic for e.g., 'touch /' (assuming we
         don't own / or have write access).  On Solaris 10 and probably
         other systems, opening a directory like "." fails with EINVAL.
         (On SunOS 4 it was EPERM but that's obsolete.)  On macOS 26
         opening "/" fails with EEXIST.  */
      काष्ठा stat st;
      अगर (open_errno
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
          अगर (no_create && utime_errno == ENOENT)
            वापस true;
          error (0, utime_errno, _("setting times of %s"), quoteaf (file));
        }
      वापस false;
    }

  वापस true;
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
         करो not create any files\n\
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
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  bool date_set = false;
  अक्षर स्थिर *flex_date = शून्य;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पूर्णांक c;
  जबतक ((c = getopt_long (argc, argv, "acd:fhmr:t:", longopts, शून्य)) != -1)
    {
      चयन (c)
        {
        हाल 'a':
          change_times |= CH_ATIME;
          अवरोध;

        हाल 'c':
          no_create = true;
          अवरोध;

        हाल 'd':
          flex_date = optarg;
          अवरोध;

        हाल 'f':
          अवरोध;

        हाल 'h':
          no_dereference = true;
          अवरोध;

        हाल 'm':
          change_times |= CH_MTIME;
          अवरोध;

        हाल 'r':
          use_ref = true;
          ref_file = optarg;
          अवरोध;

        हाल 't':
          अगर (! posixtime (&newtime[0].tv_sec, optarg,
                           PDS_LEADING_YEAR | PDS_CENTURY | PDS_SECONDS))
            error (निकास_त्रुटि, 0, _("invalid date format %s"),
                   quote (optarg));
          newtime[0].tv_nsec = 0;
          newtime[1] = newtime[0];
          date_set = true;
          अवरोध;

        हाल TIME_OPTION:	/* --time */
          change_times |= XARGMATCH ("--time", optarg,
                                     time_args, time_masks);
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (change_times == 0)
    change_times = CH_ATIME | CH_MTIME;

  अगर (date_set && (use_ref || flex_date))
    {
      error (0, 0, _("cannot specify times from more than one source"));
      usage (निकास_त्रुटि);
    }

  अगर (use_ref)
    {
      काष्ठा stat ref_stats;
      /* Don't use (no_dereference?lstat:stat) (args), since stat
         might be an object-like macro.  */
      अगर (no_dereference ? lstat (ref_file, &ref_stats)
          : stat (ref_file, &ref_stats))
        error (निकास_त्रुटि, त्रुटि_सं,
               _("failed to get attributes of %s"), quoteaf (ref_file));
      newtime[0] = get_stat_atime (&ref_stats);
      newtime[1] = get_stat_mtime (&ref_stats);
      date_set = true;
      अगर (flex_date)
        {
          अगर (change_times & CH_ATIME)
            newtime[0] = date_relative (flex_date, newtime[0]);
          अगर (change_times & CH_MTIME)
            newtime[1] = date_relative (flex_date, newtime[1]);
        }
    }
  अन्यथा
    {
      अगर (flex_date)
        {
          काष्ठा timespec now = current_timespec ();
          newtime[1] = newtime[0] = date_relative (flex_date, now);
          date_set = true;

          /* If neither -a nor -m is specified, treat "-d now" as if
             it were absent; this lets "touch" succeed more often in
             the presence of restrictive permissions.  */
          अगर (change_times == (CH_ATIME | CH_MTIME)
              && timespec_cmp (newtime[0], now) == 0)
            {
              /* Check that it really was "-d now", and not a timestamp
                 that just happens to be the current time.  */
              काष्ठा timespec notnow, notnow1;
              notnow.tv_sec = now.tv_sec ^ 1;
              notnow.tv_nsec = now.tv_nsec;
              notnow1 = date_relative (flex_date, notnow);
              अगर (timespec_cmp (notnow1, notnow) == 0)
                date_set = false;
            }
        }
    }

  /* The obsolete 'MMDDhhmm[YY]' form is valid IFF there are
     two or more non-option arguments.  */
  अगर (!date_set && 2 <= argc - optind && posix2_version () < 200112
      && posixtime (&newtime[0].tv_sec, argv[optind],
                    PDS_TRAILING_YEAR | PDS_PRE_2000))
    {
      newtime[0].tv_nsec = 0;
      newtime[1] = newtime[0];
      date_set = true;

      अगर (! दो_पर्या ("POSIXLY_CORRECT"))
        {
          काष्ठा पंचांग स्थिर *पंचांग = स_स्थानीय (&newtime[0].tv_sec);

          /* Technically, it appears that even a deliberate attempt to cause
             the above localtime to return NULL will always fail because our
             posixtime implementation rejects all dates for which localtime
             would fail.  However, skip the warning if it ever fails.  */
          अगर (पंचांग)
            error (0, 0,
                   _("warning: 'touch %s' is obsolete; use "
                     "'touch -t %04ld%02d%02d%02d%02d.%02d'"),
                   argv[optind],
                   पंचांग->tm_year + 1900L, पंचांग->tm_mon + 1, पंचांग->tm_mday,
                   पंचांग->tm_hour, पंचांग->tm_min, पंचांग->tm_sec);
        }

      optind++;
    }

  अगर (!date_set)
    {
      अगर (change_times == (CH_ATIME | CH_MTIME))
        amtime_now = true;
      अन्यथा
        newtime[1].tv_nsec = newtime[0].tv_nsec = UTIME_NOW;
    }

  अगर (optind == argc)
    {
      error (0, 0, _("missing file operand"));
      usage (निकास_त्रुटि);
    }

  bool ok = true;
  क्रम (; optind < argc; ++optind)
    ok &= touch (argv[optind]);

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
