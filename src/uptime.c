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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "long-options.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "readutmp.h"
#ಅನ್ತರ್ಭಾವ "fprintftime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "uptime"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
print_uptime (idx_t n, STRUCT_UTMP ಸ್ಥಿರಾಂಕ *utmp_buf)
{
  ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = EXIT_SUCCESS;
  time_t boot_ಸಮಯ = 0;

  /* Loop through all the utmp entries we just read and count up the valid
     ones, also in the process possibly gleaning boottime. */
  idx_t entries = 0;
  ಗಾಗಿ (idx_t i = 0; i < n; i++)
    {
      STRUCT_UTMP ಸ್ಥಿರಾಂಕ *this = &utmp_buf[i];
      entries += IS_USER_PROCESS (this);
      ಆದರೆ (UT_TYPE_BOOT_ಸಮಯ (this))
        boot_ಸಮಯ = this->ut_ts.tv_sec;
    }
  /* The gnulib module 'readutmp' is supposed to provide a BOOT_TIME entry
     on all platforms.  */
  ಆದರೆ (boot_ಸಮಯ == 0)
    {
      error (0, errno, _("couldn't get boot time"));
      ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
    }

  time_t ಸಮಯ_now = ಸಮಯ (ಖಾಲಿ);
  ರಚನೆ tm *tmn = ಸಮಯ_now == (time_t) -1 ? ಖಾಲಿ : localtime (&ಸಮಯ_now);
  /* procps' version of uptime also prints the seconds field, but
     previous versions of coreutils don't. */
  ಆದರೆ (tmn)
    /* TRANSLATORS: This prints the current clock time. */
    fprintftime (stdout, _(" %H:%M:%S  "), tmn, (timezone_t) 0, 0);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_(" ??:????  "));
      ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
    }

  intmax_t uptime;
  ಆದರೆ (ಸಮಯ_now == (time_t) -1 || boot_ಸಮಯ == 0
      || ckd_sub (&uptime, ಸಮಯ_now, boot_ಸಮಯ) || uptime < 0)
    {
      ಬರೆ (_("up ???? days ??:??,  "));
      ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      intmax_t updays = uptime / 86400;
      ಪೂರ್ಣಾಂಕ uphours = uptime % 86400 / 3600;
      ಪೂರ್ಣಾಂಕ upmins = uptime % 86400 % 3600 / 60;
      ಆದರೆ (0 < updays)
        ಬರೆ (ngettext ("up %jd day %2d:%02d,  ",
                          "up %jd days %2d:%02d,  ",
                          select_plural (updays)),
                updays, uphours, upmins);
      ಇಲ್ಲದಿದ್ದರೆ
        ಬರೆ (_("up %2d:%02d,  "), uphours, upmins);
    }

  ಬರೆ (ngettext ("%td user", "%td users", select_plural (entries)),
          entries);

  ದ್ವಿಗುಣ avg[3];
  ಪೂರ್ಣಾಂಕ loads = getloadavg (avg, 3);

  ಆದರೆ (loads == -1)
    ಅಕ್ಷರ_ಬರೆ ('\n');
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (loads > 0)
        ಬರೆ (_(",  load average: %.2f"), avg[0]);
      ಆದರೆ (loads > 1)
        ಬರೆ (", %.2f", avg[1]);
      ಆದರೆ (loads > 2)
        ಬರೆ (", %.2f", avg[2]);
      ಆದರೆ (loads > 0)
        ಅಕ್ಷರ_ಬರೆ ('\n');
    }

  ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}

/* Display the system uptime and the number of users on the system,
   according to utmp file FILENAME.  Use read_utmp OPTIONS to read the
   utmp file.  */

ಸ್ಥಿರ _Noreturn ಶೂನ್ಯ
uptime (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ಪೂರ್ಣಾಂಕ read_utmp_ಸ್ಥಿತಿ = (read_utmp (filename, &n_users, &utmp_buf, options) < 0
                          ? EXIT_FAILURE : EXIT_SUCCESS);
  ಆದರೆ (read_utmp_ಸ್ಥಿತಿ != EXIT_SUCCESS)
    {
      error (0, errno, "%s", quotef (filename));
      n_users = 0;
      utmp_buf = ಖಾಲಿ;
    }

  ಪೂರ್ಣಾಂಕ print_uptime_ಸ್ಥಿತಿ = print_uptime (n_users, utmp_buf);
  ನಿರ್ಗಮಿಸು (MAX (read_utmp_ಸ್ಥಿತಿ, print_uptime_ಸ್ಥಿತಿ));
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... [FILE]\n"), ಕಾರ್ಯಕ್ರಮ_name);
      ಬರೆ (_("\
Print the current ಸಮಯ, the ಉದ್ದಳತೆ ದ ಸಮಯ the system has been ಮೇಲೆ,\n\
the ಸಂಖ್ಯೆ ದ users on the system, and the ಸರಾಸರಿ_ಮೌಲ್ಯ ಸಂಖ್ಯೆ ದ jobs\n\
ಒಳ the ಸತತ_2 ಸರತಿ over the ಕೊನೆಯ 1, 5 and 15 minutes."));
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __linux__
      /* It would be better to introduce a configure test for this,
         but such a test is hard to write.  For the moment then, we
         have a hack which depends on the preprocessor used at compile
         time to tell us what the running kernel is.  Ugh.  */
      ಬರೆ (_("  \
Processes ಒಳ\n\
an uninterruptible sleep state also contribute ಗಮ್ಯ the ತುಂಬು ಸರಾಸರಿ_ಮೌಲ್ಯ.\n"));
#ಇಲ್ಲದಿದ್ದರೆ
      ಬರೆ (_("\n"));
#ಆದರೆ_ಅಂತ್ಯ
      ಬರೆ (_("\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, ಕಾರ್ಯಕ್ರಮ_NAME, PACKAGE_NAME,
                                   ಆವೃತ್ತಿ, true, usage, AUTHORS,
                                   (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) ಖಾಲಿ);

  ಆಯ್ಕೆ (argc - optind)
    {
    ಸ್ಥಿತಿ 0:			/* uptime */
      uptime (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 1:			/* uptime <utmp file> */
      uptime (argv[optind], 0);
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }
}
