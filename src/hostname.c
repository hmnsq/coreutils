/* hostname - set or print the name of current host system
   Copyright (C) 1994-2026 Free Software Foundation, Inc.

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

/* Written by Jim Meyering.  */

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "long-options.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "xgethostname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "hostname"

#परिभाषय AUTHORS proper_name ("Jim Meyering")

#यदि_अपरिभाषितम् HAVE_SETHOSTNAME
# यदि defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
#  include <sys/systeminfo.h>
# endif

स्थिर पूर्णाङ्क
sethostname (वर्ण ध्रुव *name, size_t namelen)
{
# यदि defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
  /* Using sysinfo() is the SVR4 mechanism to set a hostname. */
  प्रत्यागम (sysinfo (SI_समुच्चयः_HOSTNAME, name, namelen) < 0 ? -1 : 0);
# अन्यथा
  errno = ENOTSUP;
  प्रत्यागम -1;
# endif
}
#यदि_अन्तः

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [NAME]\n\
  or:  %s OPTION\n\
Print or समुच्चयः_2 the hostname सम्बन्धि the current system.\n\
\n\
"),
             क्रमादेशः_name, क्रमादेशः_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण *hostname;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, क्रमादेशः_NAME, PACKAGE_NAME,
                                   संस्करणम्, true, usage, AUTHORS,
                                   (वर्ण ध्रुव *) रिक्त);

  यदि (optind + 1 < argc)
     {
       error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
       usage (EXIT_FAILURE);
     }

  यदि (optind + 1 == argc)
    {
      /* Set hostname to operand.  */
      वर्ण ध्रुव *name = argv[optind];
      यदि (sethostname (name, वाक्यदैर्घ्यम् (name)) != 0)
        error (EXIT_FAILURE, errno, _("cannot set name to %s"),
               quote (name));
    }
  अन्यथा
    {
      hostname = xgethostname ();
      यदि (hostname == रिक्त)
        error (EXIT_FAILURE, errno, _("cannot determine hostname"));
      पङ्क्तिं_लिख (hostname);
    }

  main_exit (EXIT_SUCCESS);
}
