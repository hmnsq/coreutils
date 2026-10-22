/* tty -- print the name of the terminal connected to standard input
   Copyright (C) 1990-2026 Free Software Foundation, Inc.

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

/* Displays "not a tty" if stdin is not a terminal.
   Displays nothing if -s option is given.
   Exit status 0 if stdin is a tty, 1 if not a tty, 2 if usage error,
   3 if write error.

   Written by David MacKenzie <djm@gnu.ai.mit.edu>.  */

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "quote.h"

/* Exit statuses.  */
గణన
  {
    TTY_STDIN_NOTTY = 1,
    TTY_USAGE = 2,
    TTY_WRITE_ERROR = 3,
    TTY_TTYNAME_FAILURE = 4
  };

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "tty"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

/* If true, return an exit status but produce no output. */
స్థిర bool silent;

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"silent", no_వాదం, ఖాళీ, 's'},
  {"quiet", no_వాదం, ఖాళీ, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]...\n"), కార్యక్రమం_name);
      fputs (_("\
Print the file name యొక్క the terminal connected గమ్యం standard ఆదానం.\n\
\n\
"), stdout);
      oputs (_("\
  -s, --silent, --quiet\n\
         print nothing, only తిరిగి an నిష్క్రమించు స్థితి_2\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం optc;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TTY_WRITE_ERROR);
  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, "s", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 's':
          silent = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (TTY_USAGE);
        }
    }

  ఒకవేళ (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (TTY_USAGE);
    }

  ఒకవేళ (silent)
    తిరిగి isatty (STDIN_FILENO) ? EXIT_SUCCESS : TTY_STDIN_NOTTY;

  పూర్ణాంకం స్థితి_2;
  అక్షరం స్థిరరాశి *tty = ttyname (STDIN_FILENO);

  ఒకవేళ (tty)
    స్థితి_2 = EXIT_SUCCESS;
  లేకపోతే
    {
      పూర్ణాంకం ttyname_err = errno;
      ఒకవేళ (isatty (STDIN_FILENO))
        error (TTY_TTYNAME_FAILURE, ttyname_err, _("ttyname error"));
      tty = _("not a tty");
      స్థితి_2 = TTY_STDIN_NOTTY;
    }

  puts (tty);
  తిరిగి స్థితి_2;
}
