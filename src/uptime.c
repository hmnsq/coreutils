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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "long-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "readutmp.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "fprintftime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "uptime"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

ਸਥਿਰ ਪੂਰਨ
print_uptime (idx_t n, STRUCT_UTMP ਅਚਲ *utmp_buf)
{
  ਪੂਰਨ ਸਥਿਤੀ = EXIT_SUCCESS;
  time_t boot_ਸਮਾਂ = 0;

  /* Loop through all the utmp entries we just read and count up the valid
     ones, also in the process possibly gleaning boottime. */
  idx_t entries = 0;
  ਲਈ (idx_t i = 0; i < n; i++)
    {
      STRUCT_UTMP ਅਚਲ *this = &utmp_buf[i];
      entries += IS_USER_PROCESS (this);
      ਜੇ (UT_TYPE_BOOT_ਸਮਾਂ (this))
        boot_ਸਮਾਂ = this->ut_ts.tv_sec;
    }
  /* The gnulib module 'readutmp' is supposed to provide a BOOT_TIME entry
     on all platforms.  */
  ਜੇ (boot_ਸਮਾਂ == 0)
    {
      error (0, errno, _("couldn't get boot time"));
      ਸਥਿਤੀ = EXIT_FAILURE;
    }

  time_t ਸਮਾਂ_now = ਸਮਾਂ (ਨਿਰਾ);
  ਬਣਤਰ tm *tmn = ਸਮਾਂ_now == (time_t) -1 ? ਨਿਰਾ : localtime (&ਸਮਾਂ_now);
  /* procps' version of uptime also prints the seconds field, but
     previous versions of coreutils don't. */
  ਜੇ (tmn)
    /* TRANSLATORS: This prints the current clock time. */
    fprintftime (stdout, _(" %H:%M:%S  "), tmn, (timezone_t) 0, 0);
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_(" ??:????  "));
      ਸਥਿਤੀ = EXIT_FAILURE;
    }

  intmax_t uptime;
  ਜੇ (ਸਮਾਂ_now == (time_t) -1 || boot_ਸਮਾਂ == 0
      || ckd_sub (&uptime, ਸਮਾਂ_now, boot_ਸਮਾਂ) || uptime < 0)
    {
      ਲਿਖੋ (_("up ???? days ??:??,  "));
      ਸਥਿਤੀ = EXIT_FAILURE;
    }
  ਨਹੀਂਤਾਂ
    {
      intmax_t updays = uptime / 86400;
      ਪੂਰਨ uphours = uptime % 86400 / 3600;
      ਪੂਰਨ upmins = uptime % 86400 % 3600 / 60;
      ਜੇ (0 < updays)
        ਲਿਖੋ (ngettext ("up %jd day %2d:%02d,  ",
                          "up %jd days %2d:%02d,  ",
                          select_plural (updays)),
                updays, uphours, upmins);
      ਨਹੀਂਤਾਂ
        ਲਿਖੋ (_("up %2d:%02d,  "), uphours, upmins);
    }

  ਲਿਖੋ (ngettext ("%td user", "%td users", select_plural (entries)),
          entries);

  ਦੁੱਗਣਾ avg[3];
  ਪੂਰਨ loads = getloadavg (avg, 3);

  ਜੇ (loads == -1)
    ਅੱਖਰ_ਲਿਖੋ ('\n');
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (loads > 0)
        ਲਿਖੋ (_(",  load average: %.2f"), avg[0]);
      ਜੇ (loads > 1)
        ਲਿਖੋ (", %.2f", avg[1]);
      ਜੇ (loads > 2)
        ਲਿਖੋ (", %.2f", avg[2]);
      ਜੇ (loads > 0)
        ਅੱਖਰ_ਲਿਖੋ ('\n');
    }

  ਵਾਪਸ ਸਥਿਤੀ;
}

/* Display the system uptime and the number of users on the system,
   according to utmp file FILENAME.  Use read_utmp OPTIONS to read the
   utmp file.  */

ਸਥਿਰ _Noreturn ਖਾਲੀ
uptime (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  ਪੂਰਨ read_utmp_ਸਥਿਤੀ = (read_utmp (filename, &n_users, &utmp_buf, options) < 0
                          ? EXIT_FAILURE : EXIT_SUCCESS);
  ਜੇ (read_utmp_ਸਥਿਤੀ != EXIT_SUCCESS)
    {
      error (0, errno, "%s", quotef (filename));
      n_users = 0;
      utmp_buf = ਨਿਰਾ;
    }

  ਪੂਰਨ print_uptime_ਸਥਿਤੀ = print_uptime (n_users, utmp_buf);
  ਬਾਹਰ_ਨਿਕਲੋ (MAX (read_utmp_ਸਥਿਤੀ, print_uptime_ਸਥਿਤੀ));
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... [FILE]\n"), ਪ੍ਰੋਗਰਾਮ_name);
      ਲਿਖੋ (_("\
Print the current ਸਮਾਂ, the ਲੰਮਾਈ ਦਾ ਸਮਾਂ the system has been ਉੱਪਰ,\n\
the ਸੰਖਿਆ ਦਾ users on the system, and the ਔਸਤਨ ਸੰਖਿਆ ਦਾ jobs\n\
ਵਿੱਚ the ਲੜੀਵਾਰ_2 ਕਤਾਰ over the ਆਖਰੀ 1, 5 and 15 minutes."));
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __linux__
      /* It would be better to introduce a configure test for this,
         but such a test is hard to write.  For the moment then, we
         have a hack which depends on the preprocessor used at compile
         time to tell us what the running kernel is.  Ugh.  */
      ਲਿਖੋ (_("  \
Processes ਵਿੱਚ\n\
an uninterruptible sleep state also contribute ਮੰਜ਼ਿਲ the ਲੱਦੋ ਔਸਤਨ.\n"));
#ਨਹੀਂਤਾਂ
      ਲਿਖੋ (_("\n"));
#ਜੇ_ਅੰਤ
      ਲਿਖੋ (_("\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, ਪ੍ਰੋਗਰਾਮ_NAME, PACKAGE_NAME,
                                   ਸੰਸਕਰਣ, true, usage, AUTHORS,
                                   (ਅੱਖਰ ਅਚਲ *) ਨਿਰਾ);

  ਚੋਣ (argc - optind)
    {
    ਹਾਲਤ 0:			/* uptime */
      uptime (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ਰੋਕੋ;

    ਹਾਲਤ 1:			/* uptime <utmp file> */
      uptime (argv[optind], 0);
      ਰੋਕੋ;

    ਮੂਲ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }
}
