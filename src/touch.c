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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fd-reopen.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "parse-datetime.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "posixtm.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "posixver.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat-time.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utimens.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "touch"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Paul Rubin"), \
  proper_name ("Arnold Robbins"), \
  proper_name ("Jim Kingdon"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Randy Smith")

/* Bitmasks for 'change_times'. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CH_ATIME 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CH_MTIME 2

/* Which timestamps to change. */
ਸਥਿਰ ਪੂਰਨ change_times;

/* (-c) If true, don't create if not already there.  */
ਸਥਿਰ bool no_create;

/* (-r) If true, use times from a reference file.  */
ਸਥਿਰ bool use_ref;

/* (-h) If true, change the times of an existing symlink, if possible.  */
ਸਥਿਰ bool no_dereference;

/* If true, the only thing we have to do is change both the
   modification and access time to the current time, so we don't
   have to own the file, just be able to read and write it.
   On some systems, we can do this if we own the file, even though
   we have neither read nor write access to it.  */
ਸਥਿਰ bool amtime_now;

/* New access and modification times to use when setting time.  */
ਸਥਿਰ ਬਣਤਰ timespec newtime[2];

/* File to use for -r. */
ਸਥਿਰ ਅੱਖਰ *ref_file;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  ਸਮਾਂ_OPTION = CHAR_MAX + 1
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"time", required_ਦਲੀਲ, ਨਿਰਾ, ਸਮਾਂ_OPTION},
  {"no-create", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
  {"date", required_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"reference", required_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"no-dereference", no_ਦਲੀਲ, ਨਿਰਾ, 'h'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

/* Valid arguments to the '--time' option. */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ ਸਮਾਂ_ਦਲੀਲਾਂ[] =
{
  "atime", "access", "use", "mtime", "modify", ਨਿਰਾ
};

/* The bits in 'change_times' that those arguments set. */
ਸਥਿਰ ਪੂਰਨ ਅਚਲ ਸਮਾਂ_masks[] =
{
  CH_ATIME, CH_ATIME, CH_ATIME, CH_MTIME, CH_MTIME
};

/* The interpretation of FLEX_DATE as a date, relative to NOW.  */

ਸਥਿਰ ਬਣਤਰ timespec
date_relative (ਅੱਖਰ ਅਚਲ *flex_date, ਬਣਤਰ timespec now)
{
  ਬਣਤਰ timespec ਨਤੀਜਾ;
  ਜੇ (! parse_datetime (&ਨਤੀਜਾ, flex_date, &now))
    error (EXIT_FAILURE, 0, _("invalid date format %s"), quote (flex_date));
  ਵਾਪਸ ਨਤੀਜਾ;
}

/* Update the time of file FILE according to the options given.
   Return true if successful.  */

ਸਥਿਰ bool
touch (ਅੱਖਰ ਅਚਲ *file)
{
  ਪੂਰਨ fd = -1;
  ਪੂਰਨ open_errno = 0;
  ਬਣਤਰ timespec ਅਚਲ *t = newtime;

  ਜੇ (streq (file, "-"))
    fd = STDOUT_FILENO;
  ਨਹੀਂਤਾਂ ਜੇ (! (no_create || no_dereference))
    {
      /* Try to open FILE, creating it if necessary.  */
      fd = fd_reopen (STDIN_FILENO, file,
                      O_WRONLY | O_CREAT | O_NONBLOCK | O_NOCTTY, ਬਹੁਲਕ_RW_UGO);
      ਜੇ (fd < 0)
        open_errno = errno;
    }

  ਜੇ (change_times != (CH_ATIME | CH_MTIME))
    {
      /* We're setting only one of the time values.  */
      ਜੇ (change_times == CH_MTIME)
        newtime[0].tv_nsec = UTIME_OMIT;
      ਨਹੀਂਤਾਂ
        {
          affirm (change_times == CH_ATIME);
          newtime[1].tv_nsec = UTIME_OMIT;
        }
    }

  ਜੇ (amtime_now)
    {
      /* Pass NULL to futimens so it will not fail if we have
         write access to the file, but don't own it.  */
      t = ਨਿਰਾ;
    }

  ਅੱਖਰ ਅਚਲ *file_opt = fd == STDOUT_FILENO ? ਨਿਰਾ : file;
  ਪੂਰਨ atflag = no_dereference ? AT_SYMLINK_NOFOLLOW : 0;
  ਪੂਰਨ utime_errno = (fdutimensat (fd, AT_FDCWD, file_opt, t, atflag) == 0
                     ? 0 : errno);

  ਜੇ (fd == STDIN_FILENO)
    {
      ਜੇ (close (STDIN_FILENO) != 0)
        {
          error (0, errno, _("failed to close %s"), quoteaf (file));
          ਵਾਪਸ false;
        }
    }
  ਨਹੀਂਤਾਂ ਜੇ (fd == STDOUT_FILENO)
    {
      /* Do not diagnose "touch -c - >&-".  */
      ਜੇ (utime_errno == EBADF && no_create)
        ਵਾਪਸ true;
    }

  ਜੇ (utime_errno != 0)
    {
      /* Don't diagnose with open_errno if FILE is a directory, as that
         would give a bogus diagnostic for e.g., 'touch /' (assuming we
         don't own / or have write access).  On Solaris 10 and probably
         other systems, opening a directory like "." fails with EINVAL.
         (On SunOS 4 it was EPERM but that's obsolete.)  On macOS 26
         opening "/" fails with EEXIST.  */
      ਬਣਤਰ stat st;
      ਜੇ (open_errno
          && ! (open_errno == EISDIR
                || ((open_errno == EINVAL || open_errno == EEXIST)
                    && stat (file, &st) == 0 && S_ISDIR (st.st_ਬਹੁਲਕ))))
        {
          /* The wording of this diagnostic should cover at least two cases:
             - the file does not exist, but the parent directory is unwritable
             - the file exists, but it isn't writable
             I think it's not worth trying to distinguish them.  */
          error (0, open_errno, _("cannot touch %s"), quoteaf (file));
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (no_create && utime_errno == ENOENT)
            ਵਾਪਸ true;
          error (0, utime_errno, _("setting times of %s"), quoteaf (file));
        }
      ਵਾਪਸ false;
    }

  ਵਾਪਸ true;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... FILE...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Update the access and modification times ਦਾ each FILE ਮੰਜ਼ਿਲ the current ਸਮਾਂ.\n\
\n\
A FILE ਦਲੀਲ that does not exist is created ਖਾਲੀ_ਥਾਵਾਂ, unless -c or -h\n\
is supplied.\n\
\n\
A FILE ਦਲੀਲ string ਦਾ - is handled specially and causes touch ਮੰਜ਼ਿਲ\n\
change the times ਦਾ the file associated with standard output.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a\n\
         change only the access ਸਮਾਂ\n\
"));
      oputs (_("\
  -c, --no-create\n\
         ਕਰੋ not create any files\n\
"));
      oputs (_("\
  -d, --date=STRING\n\
         parse STRING and use it instead ਦਾ current ਸਮਾਂ\n\
"));
      oputs (_("\
  -f\n\
         (ignored)\n\
"));
      oputs (_("\
  -h, --no-dereference\n\
         affect each symbolic link instead ਦਾ any referenced file;\n\
         useful only on systems that can change the timestamps ਦਾ a symlink\n\
"));
      oputs (_("\
  -m\n\
         change only the modification ਸਮਾਂ\n\
"));
      oputs (_("\
  -r, --reference=FILE\n\
         use this file's times instead ਦਾ current ਸਮਾਂ\n\
"));
      oputs (_("\
  -t [[CC]YY]MMDDhhmm[.ss]\n\
         use specified ਸਮਾਂ instead ਦਾ current ਸਮਾਂ,\n\
         with a date-ਸਮਾਂ format that differs from -d's\n\
"));
      oputs (_("\
      --ਸਮਾਂ=ਸ਼ਬਦ\n\
         specify which ਸਮਾਂ ਮੰਜ਼ਿਲ change:\n\
         access ਸਮਾਂ (-a): 'access', 'atime', 'use';\n\
         modification ਸਮਾਂ (-m): 'modify', 'mtime'\n\
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
  bool date_ਸੈੱਟ = false;
  ਅੱਖਰ ਅਚਲ *flex_date = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਪੂਰਨ c;
  ਜਦੋਂ ((c = getopt_long (argc, argv, "acd:fhmr:t:", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'a':
          change_times |= CH_ATIME;
          ਰੋਕੋ;

        ਹਾਲਤ 'c':
          no_create = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'd':
          flex_date = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 'f':
          ਰੋਕੋ;

        ਹਾਲਤ 'h':
          no_dereference = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'm':
          change_times |= CH_MTIME;
          ਰੋਕੋ;

        ਹਾਲਤ 'r':
          use_ref = true;
          ref_file = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          ਜੇ (! posixtime (&newtime[0].tv_sec, optarg,
                           PDS_LEADING_YEAR | PDS_CENTURY | PDS_SECONDS))
            error (EXIT_FAILURE, 0, _("invalid date format %s"),
                   quote (optarg));
          newtime[0].tv_nsec = 0;
          newtime[1] = newtime[0];
          date_ਸੈੱਟ = true;
          ਰੋਕੋ;

        ਹਾਲਤ ਸਮਾਂ_OPTION:	/* --time */
          change_times |= XARGMATCH ("--time", optarg,
                                     ਸਮਾਂ_ਦਲੀਲਾਂ, ਸਮਾਂ_masks);
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (change_times == 0)
    change_times = CH_ATIME | CH_MTIME;

  ਜੇ (date_ਸੈੱਟ && (use_ref || flex_date))
    {
      error (0, 0, _("cannot specify times from more than one source"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (use_ref)
    {
      ਬਣਤਰ stat ref_stats;
      /* Don't use (no_dereference?lstat:stat) (args), since stat
         might be an object-like macro.  */
      ਜੇ (no_dereference ? lstat (ref_file, &ref_stats)
          : stat (ref_file, &ref_stats))
        error (EXIT_FAILURE, errno,
               _("failed to get attributes of %s"), quoteaf (ref_file));
      newtime[0] = get_stat_atime (&ref_stats);
      newtime[1] = get_stat_mtime (&ref_stats);
      date_ਸੈੱਟ = true;
      ਜੇ (flex_date)
        {
          ਜੇ (change_times & CH_ATIME)
            newtime[0] = date_relative (flex_date, newtime[0]);
          ਜੇ (change_times & CH_MTIME)
            newtime[1] = date_relative (flex_date, newtime[1]);
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (flex_date)
        {
          ਬਣਤਰ timespec now = current_timespec ();
          newtime[1] = newtime[0] = date_relative (flex_date, now);
          date_ਸੈੱਟ = true;

          /* If neither -a nor -m is specified, treat "-d now" as if
             it were absent; this lets "touch" succeed more often in
             the presence of restrictive permissions.  */
          ਜੇ (change_times == (CH_ATIME | CH_MTIME)
              && timespec_cmp (newtime[0], now) == 0)
            {
              /* Check that it really was "-d now", and not a timestamp
                 that just happens to be the current time.  */
              ਬਣਤਰ timespec notnow, notnow1;
              notnow.tv_sec = now.tv_sec ^ 1;
              notnow.tv_nsec = now.tv_nsec;
              notnow1 = date_relative (flex_date, notnow);
              ਜੇ (timespec_cmp (notnow1, notnow) == 0)
                date_ਸੈੱਟ = false;
            }
        }
    }

  /* The obsolete 'MMDDhhmm[YY]' form is valid IFF there are
     two or more non-option arguments.  */
  ਜੇ (!date_ਸੈੱਟ && 2 <= argc - optind && posix2_ਸੰਸਕਰਣ () < 200112
      && posixtime (&newtime[0].tv_sec, argv[optind],
                    PDS_TRAILING_YEAR | PDS_PRE_2000))
    {
      newtime[0].tv_nsec = 0;
      newtime[1] = newtime[0];
      date_ਸੈੱਟ = true;

      ਜੇ (! getenv ("POSIXLY_CORRECT"))
        {
          ਬਣਤਰ tm ਅਚਲ *tm = localtime (&newtime[0].tv_sec);

          /* Technically, it appears that even a deliberate attempt to cause
             the above localtime to return NULL will always fail because our
             posixtime implementation rejects all dates for which localtime
             would fail.  However, skip the warning if it ever fails.  */
          ਜੇ (tm)
            error (0, 0,
                   _("warning: 'touch %s' is obsolete; use "
                     "'touch -t %04ld%02d%02d%02d%02d.%02d'"),
                   argv[optind],
                   tm->tm_year + 1900L, tm->tm_mon + 1, tm->tm_mday,
                   tm->tm_hour, tm->tm_min, tm->tm_sec);
        }

      optind++;
    }

  ਜੇ (!date_ਸੈੱਟ)
    {
      ਜੇ (change_times == (CH_ATIME | CH_MTIME))
        amtime_now = true;
      ਨਹੀਂਤਾਂ
        newtime[1].tv_nsec = newtime[0].tv_nsec = UTIME_NOW;
    }

  ਜੇ (optind == argc)
    {
      error (0, 0, _("missing file operand"));
      usage (EXIT_FAILURE);
    }

  bool ok = true;
  ਲਈ (; optind < argc; ++optind)
    ok &= touch (argv[optind]);

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
