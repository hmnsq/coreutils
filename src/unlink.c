/* unlink utility for GNU.
   Copyright (C) 2001-2026 Free Software Foundation, Inc.

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

/* Written by Michael Stone */

/* Implementation overview:

   Simply call the system 'unlink' function */

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "long-options.h"
#అన్తర్భావ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "unlink"

#నిర్వచించు AUTHORS proper_name ("Michael Stone")

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s FILE\n\
  or:  %s OPTION\n"), కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("Call the unlink function to remove the specified FILE.\n\n"),
             stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, కార్యక్రమం_NAME, PACKAGE_NAME,
                                   సంస్కరణ, true, usage, AUTHORS,
                                   (అక్షరం స్థిరరాశి *) ఖాళీ);

  ఒకవేళ (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (optind + 1 < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (unlink (argv[optind]) != 0)
    error (EXIT_FAILURE, errno, _("cannot unlink %s"), quoteaf (argv[optind]));

  తిరిగి EXIT_SUCCESS;
}
