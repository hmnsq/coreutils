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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ "system.h"

#অন্তর্ভাৱ "long-options.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "readutmp.h"
#অন্তর্ভাৱ "fprintftime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "uptime"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

স্থির পূর্ণাঙ্ক
print_uptime (idx_t n, STRUCT_UTMP ধ্রুৱ *utmp_buf)
{
  পূর্ণাঙ্ক status = EXIT_SUCCESS;
  time_t boot_time = 0;

  /* Loop through all the utmp entries we just read and count up the valid
     ones, also in the process possibly gleaning boottime. */
  idx_t entries = 0;
  বাবে (idx_t i = 0; i < n; i++)
    {
      STRUCT_UTMP ধ্রুৱ *this = &utmp_buf[i];
      entries += IS_USER_PROCESS (this);
      যদি (UT_TYPE_BOOT_TIME (this))
        boot_time = this->ut_ts.tv_sec;
    }
  /* The gnulib module 'readutmp' is supposed to provide a BOOT_TIME entry
     on all platforms.  */
  যদি (boot_time == 0)
    {
      error (0, errno, _("couldn't get boot time"));
      status = EXIT_FAILURE;
    }

  time_t time_now = time (রিক্ত);
  সংরচনা tm *tmn = time_now == (time_t) -1 ? রিক্ত : localtime (&time_now);
  /* procps' version of uptime also prints the seconds field, but
     previous versions of coreutils don't. */
  যদি (tmn)
    /* TRANSLATORS: This prints the current clock time. */
    fprintftime (stdout, _(" %H:%M:%S  "), tmn, (timezone_t) 0, 0);
  অন্যথা
    {
      printf (_(" ??:????  "));
      status = EXIT_FAILURE;
    }

  intmax_t uptime;
  যদি (time_now == (time_t) -1 || boot_time == 0
      || ckd_sub (&uptime, time_now, boot_time) || uptime < 0)
    {
      printf (_("up ???? days ??:??,  "));
      status = EXIT_FAILURE;
    }
  অন্যথা
    {
      intmax_t updays = uptime / 86400;
      পূর্ণাঙ্ক uphours = uptime % 86400 / 3600;
      পূর্ণাঙ্ক upmins = uptime % 86400 % 3600 / 60;
      যদি (0 < updays)
        printf (ngettext ("up %jd day %2d:%02d,  ",
                          "up %jd days %2d:%02d,  ",
                          select_plural (updays)),
                updays, uphours, upmins);
      অন্যথা
        printf (_("up %2d:%02d,  "), uphours, upmins);
    }

  printf (ngettext ("%td user", "%td users", select_plural (entries)),
          entries);

  দ্ৱিগুণ avg[3];
  পূর্ণাঙ্ক loads = getloadavg (avg, 3);

  যদি (loads == -1)
    putchar ('\n');
  অন্যথা
    {
      যদি (loads > 0)
        printf (_(",  load average: %.2f"), avg[0]);
      যদি (loads > 1)
        printf (", %.2f", avg[1]);
      যদি (loads > 2)
        printf (", %.2f", avg[2]);
      যদি (loads > 0)
        putchar ('\n');
    }

  ঘূৰাই status;
}

/* Display the system uptime and the number of users on the system,
   according to utmp file FILENAME.  Use read_utmp OPTIONS to read the
   utmp file.  */

স্থির _Noreturn শূন্য
uptime (ৱর্ণ ধ্রুৱ *filename, পূর্ণাঙ্ক options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  পূর্ণাঙ্ক read_utmp_status = (read_utmp (filename, &n_users, &utmp_buf, options) < 0
                          ? EXIT_FAILURE : EXIT_SUCCESS);
  যদি (read_utmp_status != EXIT_SUCCESS)
    {
      error (0, errno, "%s", quotef (filename));
      n_users = 0;
      utmp_buf = রিক্ত;
    }

  পূর্ণাঙ্ক print_uptime_status = print_uptime (n_users, utmp_buf);
  exit (MAX (read_utmp_status, print_uptime_status));
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... [FILE]\n"), program_name);
      printf (_("\
Print the current time, the length of time the system has been up,\n\
the সংখ্যা of users on the system, and the average সংখ্যা of jobs\n\
in the run queue over the last 1, 5 and 15 minutes."));
#ifdef __linux__
      /* It would be better to introduce a configure test for this,
         but such a test is hard to write.  For the moment then, we
         have a hack which depends on the preprocessor used at compile
         time to tell us what the running kernel is.  Ugh.  */
      printf (_("  \
Processes in\n\
an uninterruptible sleep state also contribute to the load average.\n"));
#অন্যথা
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

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (ৱর্ণ ধ্রুৱ *) রিক্ত);

  ৱিকল্প (argc - optind)
    {
    পক্ষ 0:			/* uptime */
      uptime (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ভঙ্গ;

    পক্ষ 1:			/* uptime <utmp file> */
      uptime (argv[optind], 0);
      ভঙ্গ;

    স্ৱভাৱ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }
}
