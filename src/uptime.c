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
#define PROGRAM_NAME "uptime"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie"), \
  proper_name ("Kaveh Ghazi")

स्थिर पूर्णाङ्क
print_uptime (idx_t n, STRUCT_UTMP ध्रुव *utmp_buf)
{
  पूर्णाङ्क status = निकास_सफल;
  समय_प्रकार boot_time = 0;

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
      error (0, त्रुटि_सं, _("couldn't get boot time"));
      status = निकास_त्रुटि;
    }

  समय_प्रकार time_now = समय (रिक्त);
  संरचना पंचांग *tmn = time_now == (समय_प्रकार) -1 ? रिक्त : स_स्थानीय (&time_now);
  /* procps' version of uptime also prints the seconds field, but
     previous versions of coreutils don't. */
  यदि (tmn)
    /* TRANSLATORS: This prints the current clock time. */
    fprintftime (मानक_निकास, _(" %H:%M:%S  "), tmn, (timezone_t) 0, 0);
  अन्यथा
    {
      म_लिखो (_(" ??:????  "));
      status = निकास_त्रुटि;
    }

  intmax_t uptime;
  यदि (time_now == (समय_प्रकार) -1 || boot_time == 0
      || ckd_sub (&uptime, time_now, boot_time) || uptime < 0)
    {
      म_लिखो (_("up ???? days ??:??,  "));
      status = निकास_त्रुटि;
    }
  अन्यथा
    {
      intmax_t updays = uptime / 86400;
      पूर्णाङ्क uphours = uptime % 86400 / 3600;
      पूर्णाङ्क upmins = uptime % 86400 % 3600 / 60;
      यदि (0 < updays)
        म_लिखो (ngettext ("up %jd day %2d:%02d,  ",
                          "up %jd days %2d:%02d,  ",
                          select_plural (updays)),
                updays, uphours, upmins);
      अन्यथा
        म_लिखो (_("up %2d:%02d,  "), uphours, upmins);
    }

  म_लिखो (ngettext ("%td user", "%td users", select_plural (entries)),
          entries);

  द्विगुण avg[3];
  पूर्णाङ्क loads = getloadavg (avg, 3);

  यदि (loads == -1)
    अक्षर_दो ('\n');
  अन्यथा
    {
      यदि (loads > 0)
        म_लिखो (_(",  load average: %.2f"), avg[0]);
      यदि (loads > 1)
        म_लिखो (", %.2f", avg[1]);
      यदि (loads > 2)
        म_लिखो (", %.2f", avg[2]);
      यदि (loads > 0)
        अक्षर_दो ('\n');
    }

  प्रत्यागम status;
}

/* Display the system uptime and the number of users on the system,
   according to utmp file FILENAME.  Use read_utmp OPTIONS to read the
   utmp file.  */

स्थिर _Noreturn शून्य
uptime (वर्ण ध्रुव *filename, पूर्णाङ्क options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  पूर्णाङ्क read_utmp_status = (read_utmp (filename, &n_users, &utmp_buf, options) < 0
                          ? निकास_त्रुटि : निकास_सफल);
  यदि (read_utmp_status != निकास_सफल)
    {
      error (0, त्रुटि_सं, "%s", quotef (filename));
      n_users = 0;
      utmp_buf = रिक्त;
    }

  पूर्णाङ्क print_uptime_status = print_uptime (n_users, utmp_buf);
  निकास (MAX (read_utmp_status, print_uptime_status));
}

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... [FILE]\n"), program_name);
      म_लिखो (_("\
Print the current समय, the length of समय the प्रणाली has been up,\n\
the संख्या of users on the प्रणाली, and the average संख्या of jobs\n\
in the run queue over the last 1, 5 and 15 minutes."));
#ifdef __linux__
      /* It would be better to introduce a configure test for this,
         but such a test is hard to write.  For the moment then, we
         have a hack which depends on the preprocessor used at compile
         time to tell us what the running kernel is.  Ugh.  */
      म_लिखो (_("  \
Processes in\n\
an uninterruptible sleep state also contribute to the load average.\n"));
#अन्यथा
      म_लिखो (_("\n"));
#endif
      म_लिखो (_("\
If खाता is not specified, use %s.  %s as खाता is common.\n\
\n"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
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
      usage (निकास_त्रुटि);
    }
}
