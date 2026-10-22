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

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"

#అన్తర్భావ "long-options.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "readutmp.h"
#అన్తర్భావ "fprintftime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "uptime"

#నిర్వచించు AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

స్థిర పూర్ణాంకం
print_uptime (idx_t n, STRUCT_UTMP స్థిరరాశి *utmp_buf)
{
  పూర్ణాంకం స్థితి_2 = EXIT_SUCCESS;
  time_t boot_సమయం = 0;

  /* Loop through all the utmp entries we just read and count up the valid
     ones, also in the process possibly gleaning boottime. */
  idx_t entries = 0;
  కొరకు (idx_t i = 0; i < n; i++)
    {
      STRUCT_UTMP స్థిరరాశి *this = &utmp_buf[i];
      entries += IS_USER_PROCESS (this);
      ఒకవేళ (UT_TYPE_BOOT_సమయం (this))
        boot_సమయం = this->ut_ts.tv_sec;
    }
  /* The gnulib module 'readutmp' is supposed to provide a BOOT_TIME entry
     on all platforms.  */
  ఒకవేళ (boot_సమయం == 0)
    {
      error (0, errno, _("couldn't get boot time"));
      స్థితి_2 = EXIT_FAILURE;
    }

  time_t సమయం_now = సమయం (ఖాళీ);
  నిర్మాణం tm *tmn = సమయం_now == (time_t) -1 ? ఖాళీ : localtime (&సమయం_now);
  /* procps' version of uptime also prints the seconds field, but
     previous versions of coreutils don't. */
  ఒకవేళ (tmn)
    /* TRANSLATORS: This prints the current clock time. */
    fprintftime (stdout, _(" %H:%M:%S  "), tmn, (timezone_t) 0, 0);
  లేకపోతే
    {
      రాయి (_(" ??:????  "));
      స్థితి_2 = EXIT_FAILURE;
    }

  intmax_t uptime;
  ఒకవేళ (సమయం_now == (time_t) -1 || boot_సమయం == 0
      || ckd_sub (&uptime, సమయం_now, boot_సమయం) || uptime < 0)
    {
      రాయి (_("up ???? days ??:??,  "));
      స్థితి_2 = EXIT_FAILURE;
    }
  లేకపోతే
    {
      intmax_t updays = uptime / 86400;
      పూర్ణాంకం uphours = uptime % 86400 / 3600;
      పూర్ణాంకం upmins = uptime % 86400 % 3600 / 60;
      ఒకవేళ (0 < updays)
        రాయి (ngettext ("up %jd day %2d:%02d,  ",
                          "up %jd days %2d:%02d,  ",
                          select_plural (updays)),
                updays, uphours, upmins);
      లేకపోతే
        రాయి (_("up %2d:%02d,  "), uphours, upmins);
    }

  రాయి (ngettext ("%td user", "%td users", select_plural (entries)),
          entries);

  ద్విగుణ avg[3];
  పూర్ణాంకం loads = getloadavg (avg, 3);

  ఒకవేళ (loads == -1)
    అక్షరం_రాయి ('\n');
  లేకపోతే
    {
      ఒకవేళ (loads > 0)
        రాయి (_(",  load average: %.2f"), avg[0]);
      ఒకవేళ (loads > 1)
        రాయి (", %.2f", avg[1]);
      ఒకవేళ (loads > 2)
        రాయి (", %.2f", avg[2]);
      ఒకవేళ (loads > 0)
        అక్షరం_రాయి ('\n');
    }

  తిరిగి స్థితి_2;
}

/* Display the system uptime and the number of users on the system,
   according to utmp file FILENAME.  Use read_utmp OPTIONS to read the
   utmp file.  */

స్థిర _Noreturn శూన్య
uptime (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  పూర్ణాంకం read_utmp_స్థితి = (read_utmp (filename, &n_users, &utmp_buf, options) < 0
                          ? EXIT_FAILURE : EXIT_SUCCESS);
  ఒకవేళ (read_utmp_స్థితి != EXIT_SUCCESS)
    {
      error (0, errno, "%s", quotef (filename));
      n_users = 0;
      utmp_buf = ఖాళీ;
    }

  పూర్ణాంకం print_uptime_స్థితి = print_uptime (n_users, utmp_buf);
  నిష్క్రమించు (MAX (read_utmp_స్థితి, print_uptime_స్థితి));
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... [FILE]\n"), కార్యక్రమం_name);
      రాయి (_("\
Print the current సమయం, the నిడివి యొక్క సమయం the system has been పైకి,\n\
the సంఖ్య యొక్క users on the system, and the సరాసరి సంఖ్య యొక్క jobs\n\
లోపల the నిరంతరం_2 వరుస over the చివరి 1, 5 and 15 minutes."));
#నిర్వచితమైతే __linux__
      /* It would be better to introduce a configure test for this,
         but such a test is hard to write.  For the moment then, we
         have a hack which depends on the preprocessor used at compile
         time to tell us what the running kernel is.  Ugh.  */
      రాయి (_("  \
Processes లోపల\n\
an uninterruptible sleep state also contribute గమ్యం the ఎక్కించు సరాసరి.\n"));
#లేకపోతే
      రాయి (_("\n"));
#అయితే_ముగింపు
      రాయి (_("\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, కార్యక్రమం_NAME, PACKAGE_NAME,
                                   సంస్కరణ, true, usage, AUTHORS,
                                   (అక్షరం స్థిరరాశి *) ఖాళీ);

  ఎంపిక (argc - optind)
    {
    స్థితి 0:			/* uptime */
      uptime (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ఆపు;

    స్థితి 1:			/* uptime <utmp file> */
      uptime (argv[optind], 0);
      ఆపు;

    అప్రమేయ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }
}
