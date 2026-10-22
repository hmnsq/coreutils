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
#تعریف_کن برنامه_NAME "uptime"

#تعریف_کن AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

ساكن صحيح
print_uptime (idx_t n, STRUCT_UTMP ثابت *utmp_buf)
{
  صحيح وضعیت = EXIT_SUCCESS;
  time_t boot_زمان = 0;

  /* Loop through all the utmp entries we just read and count up the valid
     ones, also in the process possibly gleaning boottime. */
  idx_t entries = 0;
  لكل (idx_t i = 0; i < n; i++)
    {
      STRUCT_UTMP ثابت *this = &utmp_buf[i];
      entries += IS_USER_PROCESS (this);
      إذا (UT_TYPE_BOOT_زمان (this))
        boot_زمان = this->ut_ts.tv_sec;
    }
  /* The gnulib module 'readutmp' is supposed to provide a BOOT_TIME entry
     on all platforms.  */
  إذا (boot_زمان == 0)
    {
      error (0, errno, _("couldn't get boot time"));
      وضعیت = EXIT_FAILURE;
    }

  time_t زمان_now = زمان (عدم);
  بنية tm *tmn = زمان_now == (time_t) -1 ? عدم : localtime (&زمان_now);
  /* procps' version of uptime also prints the seconds field, but
     previous versions of coreutils don't. */
  إذا (tmn)
    /* TRANSLATORS: This prints the current clock time. */
    fprintftime (stdout, _(" %H:%M:%S  "), tmn, (timezone_t) 0, 0);
  وإلا
    {
      چاپ_کن (_(" ??:????  "));
      وضعیت = EXIT_FAILURE;
    }

  intmax_t uptime;
  إذا (زمان_now == (time_t) -1 || boot_زمان == 0
      || ckd_sub (&uptime, زمان_now, boot_زمان) || uptime < 0)
    {
      چاپ_کن (_("up ???? days ??:??,  "));
      وضعیت = EXIT_FAILURE;
    }
  وإلا
    {
      intmax_t updays = uptime / 86400;
      صحيح uphours = uptime % 86400 / 3600;
      صحيح upmins = uptime % 86400 % 3600 / 60;
      إذا (0 < updays)
        چاپ_کن (ngettext ("up %jd day %2d:%02d,  ",
                          "up %jd days %2d:%02d,  ",
                          select_plural (updays)),
                updays, uphours, upmins);
      وإلا
        چاپ_کن (_("up %2d:%02d,  "), uphours, upmins);
    }

  چاپ_کن (ngettext ("%td user", "%td users", select_plural (entries)),
          entries);

  مزدوج avg[3];
  صحيح loads = getloadavg (avg, 3);

  إذا (loads == -1)
    حرف_چاپ_کن ('\n');
  وإلا
    {
      إذا (loads > 0)
        چاپ_کن (_(",  load average: %.2f"), avg[0]);
      إذا (loads > 1)
        چاپ_کن (", %.2f", avg[1]);
      إذا (loads > 2)
        چاپ_کن (", %.2f", avg[2]);
      إذا (loads > 0)
        حرف_چاپ_کن ('\n');
    }

  أرجع وضعیت;
}

/* Display the system uptime and the number of users on the system,
   according to utmp file FILENAME.  Use read_utmp OPTIONS to read the
   utmp file.  */

ساكن _Noreturn فراغ
uptime (حرف ثابت *filename, صحيح options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  صحيح read_utmp_وضعیت = (read_utmp (filename, &n_users, &utmp_buf, options) < 0
                          ? EXIT_FAILURE : EXIT_SUCCESS);
  إذا (read_utmp_وضعیت != EXIT_SUCCESS)
    {
      error (0, errno, "%s", quotef (filename));
      n_users = 0;
      utmp_buf = عدم;
    }

  صحيح print_uptime_وضعیت = print_uptime (n_users, utmp_buf);
  خروج (MAX (read_utmp_وضعیت, print_uptime_وضعیت));
}

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("Usage: %s [OPTION]... [FILE]\n"), برنامه_name);
      چاپ_کن (_("\
Print the current زمان, the درازی از زمان the system has been به_بالا,\n\
the عدد از users on the system, and the معدل عدد از jobs\n\
درون the پیاپی_2 صف over the آخرین 1, 5 and 15 minutes."));
#اگر_تعریف_شده __linux__
      /* It would be better to introduce a configure test for this,
         but such a test is hard to write.  For the moment then, we
         have a hack which depends on the preprocessor used at compile
         time to tell us what the running kernel is.  Ugh.  */
      چاپ_کن (_("  \
Processes درون\n\
an uninterruptible sleep state also contribute مقصد the بارگیری معدل.\n"));
#وإلا
      چاپ_کن (_("\n"));
#پایان_اگر
      چاپ_کن (_("\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, برنامه_NAME, PACKAGE_NAME,
                                   نسخه, true, usage, AUTHORS,
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
