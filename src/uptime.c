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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن "system.h"

#ضمن "long-options.h"
#ضمن "quote.h"
#ضمن "readutmp.h"
#ضمن "fprintftime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "uptime"

#تعریف_کرو AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

ساكن صحيح
print_uptime (idx_t n, STRUCT_UTMP ثابت *utmp_buf)
{
  صحيح حالت = EXIT_SUCCESS;
  time_t boot_ویلا = 0;

  /* Loop through all the utmp entries we just read and count up the valid
     ones, also in the process possibly gleaning boottime. */
  idx_t entries = 0;
  لكل (idx_t i = 0; i < n; i++)
    {
      STRUCT_UTMP ثابت *this = &utmp_buf[i];
      entries += IS_USER_PROCESS (this);
      إذا (UT_TYPE_BOOT_ویلا (this))
        boot_ویلا = this->ut_ts.tv_sec;
    }
  /* The gnulib module 'readutmp' is supposed to provide a BOOT_TIME entry
     on all platforms.  */
  إذا (boot_ویلا == 0)
    {
      error (0, errno, _("couldn't get boot time"));
      حالت = EXIT_FAILURE;
    }

  time_t ویلا_now = ویلا (عدم);
  بنية tm *tmn = ویلا_now == (time_t) -1 ? عدم : localtime (&ویلا_now);
  /* procps' version of uptime also prints the seconds field, but
     previous versions of coreutils don't. */
  إذا (tmn)
    /* TRANSLATORS: This prints the current clock time. */
    fprintftime (stdout, _(" %H:%M:%S  "), tmn, (timezone_t) 0, 0);
  وإلا
    {
      لکھو (_(" ??:????  "));
      حالت = EXIT_FAILURE;
    }

  intmax_t uptime;
  إذا (ویلا_now == (time_t) -1 || boot_ویلا == 0
      || ckd_sub (&uptime, ویلا_now, boot_ویلا) || uptime < 0)
    {
      لکھو (_("up ???? days ??:??,  "));
      حالت = EXIT_FAILURE;
    }
  وإلا
    {
      intmax_t updays = uptime / 86400;
      صحيح uphours = uptime % 86400 / 3600;
      صحيح upmins = uptime % 86400 % 3600 / 60;
      إذا (0 < updays)
        لکھو (ngettext ("up %jd day %2d:%02d,  ",
                          "up %jd days %2d:%02d,  ",
                          select_plural (updays)),
                updays, uphours, upmins);
      وإلا
        لکھو (_("up %2d:%02d,  "), uphours, upmins);
    }

  لکھو (ngettext ("%td user", "%td users", select_plural (entries)),
          entries);

  مزدوج avg[3];
  صحيح loads = getloadavg (avg, 3);

  إذا (loads == -1)
    حرف_لکھو ('\n');
  وإلا
    {
      إذا (loads > 0)
        لکھو (_(",  load average: %.2f"), avg[0]);
      إذا (loads > 1)
        لکھو (", %.2f", avg[1]);
      إذا (loads > 2)
        لکھو (", %.2f", avg[2]);
      إذا (loads > 0)
        حرف_لکھو ('\n');
    }

  أرجع حالت;
}

/* Display the system uptime and the number of users on the system,
   according to utmp file FILENAME.  Use read_utmp OPTIONS to read the
   utmp file.  */

ساكن _Noreturn فراغ
uptime (حرف ثابت *filename, صحيح options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  صحيح read_utmp_حالت = (read_utmp (filename, &n_users, &utmp_buf, options) < 0
                          ? EXIT_FAILURE : EXIT_SUCCESS);
  إذا (read_utmp_حالت != EXIT_SUCCESS)
    {
      error (0, errno, "%s", quotef (filename));
      n_users = 0;
      utmp_buf = عدم;
    }

  صحيح print_uptime_حالت = print_uptime (n_users, utmp_buf);
  باہر_نکلو (MAX (read_utmp_حالت, print_uptime_حالت));
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [OPTION]... [FILE]\n"), پروگرام_name);
      لکھو (_("\
Print the current ویلا, the لمائی دا ویلا the system has been اتے,\n\
the نمبر دا users on the system, and the اوسطن نمبر دا jobs\n\
وچ the لگاتار_2 قطار over the آخری 1, 5 and 15 minutes."));
#جے_تعریف_شدہ __linux__
      /* It would be better to introduce a configure test for this,
         but such a test is hard to write.  For the moment then, we
         have a hack which depends on the preprocessor used at compile
         time to tell us what the running kernel is.  Ugh.  */
      لکھو (_("  \
Processes وچ\n\
an uninterruptible sleep state also contribute منزل the لدو اوسطن.\n"));
#وإلا
      لکھو (_("\n"));
#جے_انت
      لکھو (_("\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE_NAME,
                                   نسخہ, true, usage, AUTHORS,
                                   (حرف ثابت *) عدم);

  بدل (argc - optind)
    {
    حالة 0:			/* uptime */
      uptime (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      اكسر;

    حالة 1:			/* uptime <utmp file> */
      uptime (argv[optind], 0);
      اكسر;

    افتراضي:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }
}
