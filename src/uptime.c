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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"

#अन्तर्भाव "long-options.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "readutmp.h"
#अन्तर्भाव "fprintftime.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "uptime"

#परिभाषय AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

स्थिर पूर्णाङ्क
print_uptime (idx_t n, STRUCT_UTMP ध्रुव *utmp_buf)
{
  पूर्णाङ्क स्थितिः = EXIT_SUCCESS;
  time_t boot_time = 0;

  /* Loop through all the utmp entries we just read and count up the valid
     ones, also in the process possibly gleaning boottime. */
  idx_t entries = 0;
  प्रति (idx_t i = 0; i < n; i++)
    {
      STRUCT_UTMP ध्रुव *this = &utmp_buf[i];
      entries += IS_USER_PROCESS (this);
      यदि (UT_TYPE_BOOT_TIME (this))
        boot_time = this->ut_ts.tv_sec;
    }
  /* The gnulib module 'readutmp' is supposed to provide a BOOT_TIME entry
     on all platforms.  */
  यदि (boot_time == 0)
    {
      error (0, errno, _("couldn't get boot time"));
      स्थितिः = EXIT_FAILURE;
    }

  time_t time_now = time (रिक्त);
  संरचना tm *tmn = time_now == (time_t) -1 ? रिक्त : localtime (&time_now);
  /* procps' version of uptime also prints the seconds field, but
     previous versions of coreutils don't. */
  यदि (tmn)
    /* TRANSLATORS: This prints the current clock time. */
    fprintftime (stdout, _(" %H:%M:%S  "), tmn, (timezone_t) 0, 0);
  अन्यथा
    {
      प्रारूप_लिख (_(" ??:????  "));
      स्थितिः = EXIT_FAILURE;
    }

  intmax_t uptime;
  यदि (time_now == (time_t) -1 || boot_time == 0
      || ckd_sub (&uptime, time_now, boot_time) || uptime < 0)
    {
      प्रारूप_लिख (_("up ???? days ??:??,  "));
      स्थितिः = EXIT_FAILURE;
    }
  अन्यथा
    {
      intmax_t updays = uptime / 86400;
      पूर्णाङ्क uphours = uptime % 86400 / 3600;
      पूर्णाङ्क upmins = uptime % 86400 % 3600 / 60;
      यदि (0 < updays)
        प्रारूप_लिख (ngettext ("up %jd day %2d:%02d,  ",
                          "up %jd days %2d:%02d,  ",
                          select_plural (updays)),
                updays, uphours, upmins);
      अन्यथा
        प्रारूप_लिख (_("up %2d:%02d,  "), uphours, upmins);
    }

  प्रारूप_लिख (ngettext ("%td user", "%td users", select_plural (entries)),
          entries);

  द्विगुण avg[3];
  पूर्णाङ्क loads = getloadavg (avg, 3);

  यदि (loads == -1)
    अक्षरं_लिख ('\n');
  अन्यथा
    {
      यदि (loads > 0)
        प्रारूप_लिख (_(",  load average: %.2f"), avg[0]);
      यदि (loads > 1)
        प्रारूप_लिख (", %.2f", avg[1]);
      यदि (loads > 2)
        प्रारूप_लिख (", %.2f", avg[2]);
      यदि (loads > 0)
        अक्षरं_लिख ('\n');
    }

  प्रत्यागम स्थितिः;
}

/* Display the system uptime and the number of users on the system,
   according to utmp file FILENAME.  Use read_utmp OPTIONS to read the
   utmp file.  */

स्थिर _Noreturn शून्य
uptime (वर्ण ध्रुव *filename, पूर्णाङ्क options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  पूर्णाङ्क read_utmp_स्थितिः = (read_utmp (filename, &n_users, &utmp_buf, options) < 0
                          ? EXIT_FAILURE : EXIT_SUCCESS);
  यदि (read_utmp_स्थितिः != EXIT_SUCCESS)
    {
      error (0, errno, "%s", quotef (filename));
      n_users = 0;
      utmp_buf = रिक्त;
    }

  पूर्णाङ्क print_uptime_स्थितिः = print_uptime (n_users, utmp_buf);
  निर्गच्छ (MAX (read_utmp_स्थितिः, print_uptime_स्थितिः));
}

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION]... [FILE]\n"), क्रमादेशः_name);
      प्रारूप_लिख (_("\
Print the current time, the दीर्घता सम्बन्धि time the system has been ऊर्ध्वम्,\n\
the सङ्ख्या सम्बन्धि users on the system, and the मध्यमानम् सङ्ख्या सम्बन्धि jobs\n\
अन्तर् the सततम्_2 queue over the चरमः 1, 5 and 15 minutes."));
#यदि_परिभाषितम् __linux__
      /* It would be better to introduce a configure test for this,
         but such a test is hard to write.  For the moment then, we
         have a hack which depends on the preprocessor used at compile
         time to tell us what the running kernel is.  Ugh.  */
      प्रारूप_लिख (_("  \
Processes अन्तर्\n\
an uninterruptible sleep state also contribute गन्तव्यम् the आरोपय मध्यमानम्.\n"));
#अन्यथा
      प्रारूप_लिख (_("\n"));
#यदि_अन्तः
      प्रारूप_लिख (_("\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, क्रमादेशः_NAME, PACKAGE_NAME,
                                   संस्करणम्, true, usage, AUTHORS,
                                   (वर्ण ध्रुव *) रिक्त);

  विकल्प (argc - optind)
    {
    पक्ष 0:			/* uptime */
      uptime (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      भङ्ग;

    पक्ष 1:			/* uptime <utmp file> */
      uptime (argv[optind], 0);
      भङ्ग;

    स्वभाव:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }
}
