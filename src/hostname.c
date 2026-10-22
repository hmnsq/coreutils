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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "long-options.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xgethostname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "hostname"

#నిర్వచించు AUTHORS proper_name ("Jim Meyering")

#నిర్వచితం_కాకపోతే HAVE_SETHOSTNAME
# ఒకవేళ defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
#  include <sys/systeminfo.h>
# endif

స్థిర పూర్ణాంకం
sethostname (అక్షరం స్థిరరాశి *name, size_t namelen)
{
# ఒకవేళ defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
  /* Using sysinfo() is the SVR4 mechanism to set a hostname. */
  తిరిగి (sysinfo (SI_సమితి_HOSTNAME, name, namelen) < 0 ? -1 : 0);
# లేకపోతే
  errno = ENOTSUP;
  తిరిగి -1;
# endif
}
#అయితే_ముగింపు

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [NAME]\n\
  or:  %s OPTION\n\
Print or సమితి_2 the hostname యొక్క the current system.\n\
\n\
"),
             కార్యక్రమం_name, కార్యక్రమం_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం *hostname;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, కార్యక్రమం_NAME, PACKAGE_NAME,
                                   సంస్కరణ, true, usage, AUTHORS,
                                   (అక్షరం స్థిరరాశి *) ఖాళీ);

  ఒకవేళ (optind + 1 < argc)
     {
       error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
       usage (EXIT_FAILURE);
     }

  ఒకవేళ (optind + 1 == argc)
    {
      /* Set hostname to operand.  */
      అక్షరం స్థిరరాశి *name = argv[optind];
      ఒకవేళ (sethostname (name, అక్షరశ్రేణి_పొడవు (name)) != 0)
        error (EXIT_FAILURE, errno, _("cannot set name to %s"),
               quote (name));
    }
  లేకపోతే
    {
      hostname = xgethostname ();
      ఒకవేళ (hostname == ఖాళీ)
        error (EXIT_FAILURE, errno, _("cannot determine hostname"));
      puts (hostname);
    }

  main_exit (EXIT_SUCCESS);
}
