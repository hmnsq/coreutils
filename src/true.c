/* Exit with a status code indicating success.
   Copyright (C) 1999-2026 Free Software Foundation, Inc.

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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"
#అన్తర్భావ "true.h"

#నిర్వచించు EXIT_స్థితి (true_బహుళకం != TRUE_TRUE)
#నిర్వచించు కార్యక్రమం_NAME (true_బహుళకం == TRUE_TRUE ? "true" : "false")

#నిర్వచించు AUTHORS proper_name ("Jim Meyering")

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  రాయి (_("\
Usage: %s [ignored command పంక్తి arguments]\n\
  or:  %s OPTION\n\
"),
          కార్యక్రమం_name, కార్యక్రమం_name);
  రాయి ("%s\n\n",
          _(EXIT_స్థితి == EXIT_SUCCESS
            ? N_("Exit with a status code indicating success.")
            : N_("Exit with a status code indicating failure.")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (సంస్కరణ_OPTION_DESCRIPTION);
  రాయి (USAGE_BUILTIN_WARNING, కార్యక్రమం_NAME);
  emit_ancillary_info (కార్యక్రమం_NAME);
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  /* Recognize --help or --version only if it's the only command-line
     argument.  */
  ఒకవేళ (argc == 2)
    {
      initialize_main (&argc, &argv);
      సమితి_కార్యక్రమం_name (argv[0]);
      setlocale (LC_ALL, "");
      bindtextdomain (PACKAGE, LOCALEDIR);
      textdomain (PACKAGE);

      /* Note true(1) will return EXIT_FAILURE in the
         edge case where writes fail with GNU specific options.  */
      atexit (close_stdout);

      ఒకవేళ (streq (argv[1], "--help"))
        usage (EXIT_స్థితి);

      ఒకవేళ (streq (argv[1], "--version"))
        సంస్కరణ_etc (stdout, కార్యక్రమం_NAME, PACKAGE_NAME, సంస్కరణ, AUTHORS,
                     (అక్షరం *) ఖాళీ);
    }

  తిరిగి EXIT_స్థితి;
}
