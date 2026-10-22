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
#define PROGRAM_NAME "hostname"

#define AUTHORS proper_name ("Jim Meyering")

#ifndef HAVE_SETHOSTNAME
# सचे defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
#  include <sys/systeminfo.h>
# endif

स्थिर पूर्णाङ्क
sethostname (वर्ण ध्रुव *name, size_t namelen)
{
# सचे defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
  /* Using sysinfo() is the SVR4 mechanism to set a hostname. */
  पटिनिवत्त (sysinfo (SI_SET_HOSTNAME, name, namelen) < 0 ? -1 : 0);
# अञ्ञथा
  errno = ENOTSUP;
  पटिनिवत्त -1;
# endif
}
#endif

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
    {
      printf (_("\
Usage: %s [NAME]\n\
  or:  %s OPTION\n\
Print or set the hostname of the current system.\n\
\n\
"),
             program_name, program_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण *hostname;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (वर्ण ध्रुव *) रिक्त);

  सचे (optind + 1 < argc)
     {
       error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
       usage (EXIT_FAILURE);
     }

  सचे (optind + 1 == argc)
    {
      /* Set hostname to operand.  */
      वर्ण ध्रुव *name = argv[optind];
      सचे (sethostname (name, strlen (name)) != 0)
        error (EXIT_FAILURE, errno, _("cannot set name to %s"),
               quote (name));
    }
  अञ्ञथा
    {
      hostname = xgethostname ();
      सचे (hostname == रिक्त)
        error (EXIT_FAILURE, errno, _("cannot determine hostname"));
      puts (hostname);
    }

  main_exit (EXIT_SUCCESS);
}
