/* GNU's uptime.
   Copyright (C) 1992-2026 Free Software Foundation, Inc.

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

/* Created by hacking who.c by Kaveh Ghazi ghazi@caip.rutgers.edu.  */

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "long-options.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "readutmp.h"
#അന്തര്ഭാവ "fprintftime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "uptime"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

സ്ഥിതം പൂർണ്ണസംഖ്യ
print_uptime (idx_t n, STRUCT_UTMP സ്ഥിരം *utmp_buf)
{
  പൂർണ്ണസംഖ്യ status = EXIT_SUCCESS;
  time_t boot_time = 0;

  /* Loop through all the utmp entries we just read and count up the valid
     ones, also in the process possibly gleaning boottime. */
  idx_t entries = 0;
  വേണ്ടി (idx_t i = 0; i < n; i++)
    {
      STRUCT_UTMP സ്ഥിരം *this = &utmp_buf[i];
      entries += IS_USER_PROCESS (this);
      എങ്കിൽ (UT_TYPE_BOOT_TIME (this))
        boot_time = this->ut_ts.tv_sec;
    }
  /* The gnulib module 'readutmp' is supposed to provide a BOOT_TIME entry
     on all platforms.  */
  എങ്കിൽ (boot_time == 0)
    {
      error (0, errno, _("couldn't get boot time"));
      status = EXIT_FAILURE;
    }

  time_t time_now = time (ശൂന്യ);
  ഘടന tm *tmn = time_now == (time_t) -1 ? ശൂന്യ : localtime (&time_now);
  /* procps' version of uptime also prints the seconds field, but
     previous versions of coreutils don't. */
  എങ്കിൽ (tmn)
    /* TRANSLATORS: This prints the current clock time. */
    fprintftime (stdout, _(" %H:%M:%S  "), tmn, (timezone_t) 0, 0);
  അല്ലെങ്കിൽ
    {
      printf (_(" ??:????  "));
      status = EXIT_FAILURE;
    }

  intmax_t uptime;
  എങ്കിൽ (time_now == (time_t) -1 || boot_time == 0
      || ckd_sub (&uptime, time_now, boot_time) || uptime < 0)
    {
      printf (_("up ???? days ??:??,  "));
      status = EXIT_FAILURE;
    }
  അല്ലെങ്കിൽ
    {
      intmax_t updays = uptime / 86400;
      പൂർണ്ണസംഖ്യ uphours = uptime % 86400 / 3600;
      പൂർണ്ണസംഖ്യ upmins = uptime % 86400 % 3600 / 60;
      എങ്കിൽ (0 < updays)
        printf (ngettext ("up %jd day %2d:%02d,  ",
                          "up %jd days %2d:%02d,  ",
                          select_plural (updays)),
                updays, uphours, upmins);
      അല്ലെങ്കിൽ
        printf (_("up %2d:%02d,  "), uphours, upmins);
    }

  printf (ngettext ("%td user", "%td users", select_plural (entries)),
          entries);

  ഇരട്ട avg[3];
  പൂർണ്ണസംഖ്യ loads = getloadavg (avg, 3);

  എങ്കിൽ (loads == -1)
    putchar ('\n');
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (loads > 0)
        printf (_(",  load average: %.2f"), avg[0]);
      എങ്കിൽ (loads > 1)
        printf (", %.2f", avg[1]);
      എങ്കിൽ (loads > 2)
        printf (", %.2f", avg[2]);
      എങ്കിൽ (loads > 0)
        putchar ('\n');
    }

  തിരികെ status;
}

/* Display the system uptime and the number of users on the system,
   according to utmp file FILENAME.  Use read_utmp OPTIONS to read the
   utmp file.  */

സ്ഥിതം _Noreturn ശൂന്യം
uptime (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  പൂർണ്ണസംഖ്യ read_utmp_status = (read_utmp (filename, &n_users, &utmp_buf, options) < 0
                          ? EXIT_FAILURE : EXIT_SUCCESS);
  എങ്കിൽ (read_utmp_status != EXIT_SUCCESS)
    {
      error (0, errno, "%s", quotef (filename));
      n_users = 0;
      utmp_buf = ശൂന്യ;
    }

  പൂർണ്ണസംഖ്യ print_uptime_status = print_uptime (n_users, utmp_buf);
  exit (MAX (read_utmp_status, print_uptime_status));
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]... [FILE]\n"), program_name);
      printf (_("\
Print the current time, the length of time the system has been up,\n\
the സംഖ്യ of users on the system, and the average സംഖ്യ of jobs\n\
in the run queue over the last 1, 5 and 15 minutes."));
#ifdef __linux__
      /* It would be better to introduce a configure test for this,
         but such a test is hard to write.  For the moment then, we
         have a hack which depends on the preprocessor used at compile
         time to tell us what the running kernel is.  Ugh.  */
      printf (_("  \
Processes in\n\
an uninterruptible sleep state also contribute to the load average.\n"));
#അല്ലെങ്കിൽ
      printf (_("\n"));
#endif
      printf (_("\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (അക്ഷരം സ്ഥിരം *) ശൂന്യ);

  തിരഞ്ഞെടുപ്പ് (argc - optind)
    {
    അവസ്ഥ 0:			/* uptime */
      uptime (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      നിർത്തുക;

    അവസ്ഥ 1:			/* uptime <utmp file> */
      uptime (argv[optind], 0);
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }
}
