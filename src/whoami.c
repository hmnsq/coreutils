/* whoami -- print effective userid

   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Equivalent to 'id -un'. */
/* Written by Richard Mlynarik. */

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <pwd.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "long-options.h"
#అన్తర్భావ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "whoami"

#నిర్వచించు AUTHORS proper_name ("Richard Mlynarik")

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]...\n"), కార్యక్రమం_name);
      fputs (_("\
Print the user name associated with the current effective user ID.\n\
Same as id -un.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  uid_t NO_UID = -1;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, కార్యక్రమం_NAME, PACKAGE_NAME,
                                   సంస్కరణ, true, usage, AUTHORS,
                                   (అక్షరం స్థిరరాశి *) ఖాళీ);

  ఒకవేళ (optind != argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  errno = 0;
  uid_t uid = geteuid ();
  నిర్మాణం passwd *pw = uid == NO_UID && errno ? ఖాళీ : getpwuid (uid);
  ఒకవేళ (!pw)
    error (EXIT_FAILURE, errno, _("cannot find name for user ID %ju"),
           (uintmax_t) uid);
  puts (pw->pw_name);
  తిరిగి EXIT_SUCCESS;
}
