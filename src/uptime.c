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

#شامل <config.h>

#شامل <stdio.h>
#شامل <sys/types.h>

#شامل "system.h"

#شامل "long-options.h"
#شامل "quote.h"
#شامل "readutmp.h"
#شامل "fprintftime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "uptime"

#تعریف_کرو AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

ساکن صحیح
print_uptime (idx_t n, STRUCT_UTMP ثابت *utmp_buf)
{
  صحیح حالت_2 = EXIT_SUCCESS;
  time_t boot_وقت = 0;

  /* Loop through all the utmp entries we just read and count up the valid
     ones, also in the process possibly gleaning boottime. */
  idx_t entries = 0;
  برائے (idx_t i = 0; i < n; i++)
    {
      STRUCT_UTMP ثابت *this = &utmp_buf[i];
      entries += IS_USER_PROCESS (this);
      اگر (UT_TYPE_BOOT_وقت (this))
        boot_وقت = this->ut_ts.tv_sec;
    }
  /* The gnulib module 'readutmp' is supposed to provide a BOOT_TIME entry
     on all platforms.  */
  اگر (boot_وقت == 0)
    {
      error (0, errno, _("couldn't get boot time"));
      حالت_2 = EXIT_FAILURE;
    }

  time_t وقت_now = وقت (تہی);
  ساخت tm *tmn = وقت_now == (time_t) -1 ? تہی : localtime (&وقت_now);
  /* procps' version of uptime also prints the seconds field, but
     previous versions of coreutils don't. */
  اگر (tmn)
    /* TRANSLATORS: This prints the current clock time. */
    fprintftime (stdout, _(" %H:%M:%S  "), tmn, (timezone_t) 0, 0);
  ورنہ
    {
      لکھو (_(" ??:????  "));
      حالت_2 = EXIT_FAILURE;
    }

  intmax_t uptime;
  اگر (وقت_now == (time_t) -1 || boot_وقت == 0
      || ckd_sub (&uptime, وقت_now, boot_وقت) || uptime < 0)
    {
      لکھو (_("up ???? days ??:??,  "));
      حالت_2 = EXIT_FAILURE;
    }
  ورنہ
    {
      intmax_t updays = uptime / 86400;
      صحیح uphours = uptime % 86400 / 3600;
      صحیح upmins = uptime % 86400 % 3600 / 60;
      اگر (0 < updays)
        لکھو (ngettext ("up %jd day %2d:%02d,  ",
                          "up %jd days %2d:%02d,  ",
                          select_plural (updays)),
                updays, uphours, upmins);
      ورنہ
        لکھو (_("up %2d:%02d,  "), uphours, upmins);
    }

  لکھو (ngettext ("%td user", "%td users", select_plural (entries)),
          entries);

  دوہرا avg[3];
  صحیح loads = getloadavg (avg, 3);

  اگر (loads == -1)
    حرف_لکھو ('\n');
  ورنہ
    {
      اگر (loads > 0)
        لکھو (_(",  load average: %.2f"), avg[0]);
      اگر (loads > 1)
        لکھو (", %.2f", avg[1]);
      اگر (loads > 2)
        لکھو (", %.2f", avg[2]);
      اگر (loads > 0)
        حرف_لکھو ('\n');
    }

  واپس حالت_2;
}

/* Display the system uptime and the number of users on the system,
   according to utmp file FILENAME.  Use read_utmp OPTIONS to read the
   utmp file.  */

ساکن _Noreturn خالی
uptime (حرف ثابت *filename, صحیح options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  صحیح read_utmp_حالت = (read_utmp (filename, &n_users, &utmp_buf, options) < 0
                          ? EXIT_FAILURE : EXIT_SUCCESS);
  اگر (read_utmp_حالت != EXIT_SUCCESS)
    {
      error (0, errno, "%s", quotef (filename));
      n_users = 0;
      utmp_buf = تہی;
    }

  صحیح print_uptime_حالت = print_uptime (n_users, utmp_buf);
  باہر_نکلو (MAX (read_utmp_حالت, print_uptime_حالت));
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... [FILE]\n"), پروگرام_name);
      لکھو (_("\
Print the current وقت, the لمبائی کا وقت the system has been اوپر,\n\
the نمبر کا users on the system, and the اوسط_قدر نمبر کا jobs\n\
اندر the تسلسل_2 قطار over the آخری 1, 5 and 15 minutes."));
#اگر_تعریف_شدہ __linux__
      /* It would be better to introduce a configure test for this,
         but such a test is hard to write.  For the moment then, we
         have a hack which depends on the preprocessor used at compile
         time to tell us what the running kernel is.  Ugh.  */
      لکھو (_("  \
Processes اندر\n\
an uninterruptible sleep state also contribute مقصد the بھرو اوسط_قدر.\n"));
#ورنہ
      لکھو (_("\n"));
#اگر_ختم
      لکھو (_("\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE_NAME,
                                   نسخہ, true, usage, AUTHORS,
                                   (حرف ثابت *) تہی);

  انتخاب (argc - optind)
    {
    حالت 0:			/* uptime */
      uptime (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      توڑو;

    حالت 1:			/* uptime <utmp file> */
      uptime (argv[optind], 0);
      توڑو;

    طےشدہ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }
}
