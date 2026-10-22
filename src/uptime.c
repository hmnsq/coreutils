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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "long-options.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "readutmp.h"
#અન્તર્ભાવ "fprintftime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "uptime"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

સ્થિર પૂર્ણાંક
print_uptime (idx_t n, STRUCT_UTMP અચલ *utmp_buf)
{
  પૂર્ણાંક status = EXIT_SUCCESS;
  time_t boot_time = 0;

  /* Loop through all the utmp entries we just read and count up the valid
     ones, also in the process possibly gleaning boottime. */
  idx_t entries = 0;
  માટે (idx_t i = 0; i < n; i++)
    {
      STRUCT_UTMP અચલ *this = &utmp_buf[i];
      entries += IS_USER_PROCESS (this);
      જો (UT_TYPE_BOOT_TIME (this))
        boot_time = this->ut_ts.tv_sec;
    }
  /* The gnulib module 'readutmp' is supposed to provide a BOOT_TIME entry
     on all platforms.  */
  જો (boot_time == 0)
    {
      error (0, errno, _("couldn't get boot time"));
      status = EXIT_FAILURE;
    }

  time_t time_now = time (ખાલી);
  સંરચના tm *tmn = time_now == (time_t) -1 ? ખાલી : localtime (&time_now);
  /* procps' version of uptime also prints the seconds field, but
     previous versions of coreutils don't. */
  જો (tmn)
    /* TRANSLATORS: This prints the current clock time. */
    fprintftime (stdout, _(" %H:%M:%S  "), tmn, (timezone_t) 0, 0);
  નહીંતર
    {
      printf (_(" ??:????  "));
      status = EXIT_FAILURE;
    }

  intmax_t uptime;
  જો (time_now == (time_t) -1 || boot_time == 0
      || ckd_sub (&uptime, time_now, boot_time) || uptime < 0)
    {
      printf (_("up ???? days ??:??,  "));
      status = EXIT_FAILURE;
    }
  નહીંતર
    {
      intmax_t updays = uptime / 86400;
      પૂર્ણાંક uphours = uptime % 86400 / 3600;
      પૂર્ણાંક upmins = uptime % 86400 % 3600 / 60;
      જો (0 < updays)
        printf (ngettext ("up %jd day %2d:%02d,  ",
                          "up %jd days %2d:%02d,  ",
                          select_plural (updays)),
                updays, uphours, upmins);
      નહીંતર
        printf (_("up %2d:%02d,  "), uphours, upmins);
    }

  printf (ngettext ("%td user", "%td users", select_plural (entries)),
          entries);

  દ્વિગુણ avg[3];
  પૂર્ણાંક loads = getloadavg (avg, 3);

  જો (loads == -1)
    putchar ('\n');
  નહીંતર
    {
      જો (loads > 0)
        printf (_(",  load average: %.2f"), avg[0]);
      જો (loads > 1)
        printf (", %.2f", avg[1]);
      જો (loads > 2)
        printf (", %.2f", avg[2]);
      જો (loads > 0)
        putchar ('\n');
    }

  પાછા status;
}

/* Display the system uptime and the number of users on the system,
   according to utmp file FILENAME.  Use read_utmp OPTIONS to read the
   utmp file.  */

સ્થિર _Noreturn શૂન્ય
uptime (અક્ષર અચલ *filename, પૂર્ણાંક options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  પૂર્ણાંક read_utmp_status = (read_utmp (filename, &n_users, &utmp_buf, options) < 0
                          ? EXIT_FAILURE : EXIT_SUCCESS);
  જો (read_utmp_status != EXIT_SUCCESS)
    {
      error (0, errno, "%s", quotef (filename));
      n_users = 0;
      utmp_buf = ખાલી;
    }

  પૂર્ણાંક print_uptime_status = print_uptime (n_users, utmp_buf);
  exit (MAX (read_utmp_status, print_uptime_status));
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]... [FILE]\n"), program_name);
      printf (_("\
Print the current time, the length of time the system has been up,\n\
the સંખ્યા of users on the system, and the average સંખ્યા of jobs\n\
in the run queue over the last 1, 5 and 15 minutes."));
#ifdef __linux__
      /* It would be better to introduce a configure test for this,
         but such a test is hard to write.  For the moment then, we
         have a hack which depends on the preprocessor used at compile
         time to tell us what the running kernel is.  Ugh.  */
      printf (_("  \
Processes in\n\
an uninterruptible sleep state also contribute to the load average.\n"));
#નહીંતર
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

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (અક્ષર અચલ *) ખાલી);

  પસંદગી (argc - optind)
    {
    સ્થિતિ 0:			/* uptime */
      uptime (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      તોડો;

    સ્થિતિ 1:			/* uptime <utmp file> */
      uptime (argv[optind], 0);
      તોડો;

    મૂળભૂત:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }
}
