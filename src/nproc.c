/* nproc - print the number of processors.
   Copyright (C) 2009-2026 Free Software Foundation, Inc.

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

/* Written by Giuseppe Scrivano.  */

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "nproc.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "nproc"

#నిర్వచించు AUTHORS proper_name ("Giuseppe Scrivano")

గణన
{
  ALL_OPTION = CHAR_MAX + 1,
  IGNORE_OPTION
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"all", no_వాదం, ఖాళీ, ALL_OPTION},
  {"ignore", required_వాదం, ఖాళీ, IGNORE_OPTION},
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
Print the సంఖ్య యొక్క processing units అందుబాటు గమ్యం the current process,\n\
which may be less than the సంఖ్య యొక్క online processors.\n\
If the 'OMP_NUM_THREADS' or 'OMP_THREAD_LIMIT' environment variables are సమితి_2,\n\
then they will determine the minimum and గరిష్ఠ returned విలువ respectively.\n\
\n\
"), stdout);
      oputs (_("\
      --all\n\
         print the సంఖ్య యొక్క installed processors,\n\
         disregarding any OpenMP environment variables, or CPU quotas.\n\
"));
      oputs (_("\
      --ignore=N\n\
         ఒకవేళ possible, exclude N processing units.\n\
         The ఫలితం is guaranteed గమ్యం be at కనిష్ఠ 1.\n\
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
  గుర్తులేని దీర్ఘ ignore = 0;
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  గణన nproc_query బహుళకం_2 = NPROC_CURRENT_OVERRIDABLE;

  అయితే (true)
    {
      పూర్ణాంకం c = getopt_long (argc, argv, "", longopts, ఖాళీ);
      ఒకవేళ (c == -1)
        ఆపు;
      ఎంపిక (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        స్థితి ALL_OPTION:
          బహుళకం_2 = NPROC_ALL;
          ఆపు;

        స్థితి IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, ULONG_MAX, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          ఆపు;

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  గుర్తులేని దీర్ఘ nproc = అంకె_processors (బహుళకం_2);

  ఒకవేళ (ignore < nproc)
    nproc -= ignore;
  లేకపోతే
    nproc = 1;

  రాయి ("%lu\n", nproc);

  తిరిగి EXIT_SUCCESS;
}
