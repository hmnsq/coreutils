/* Copyright (C) 2014-2026 Free Software Foundation, Inc.

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

/* coreutils.c aggregates the functionality of every other tool into a single
   binary multiplexed by the value of argv[0]. This is enabled by passing
   --enable-single-binary to configure.

   Written by Alex Deymo <deymo@chromium.org>.  */

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>
#ఒకవేళ HAVE_PRCTL
# include <sys/prctl.h>
#అయితే_ముగింపు

#అన్తర్భావ "system.h"
#అన్తర్భావ "quote.h"

#నిర్వచితమైతే SINGLE_ద్విమాన
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_ద్విమాన_కార్యక్రమం(prog_name_str, main_name) \
  పూర్ణాంకం single_ద్విమాన_main##main_name (పూర్ణాంకం, అక్షరం **);
# include "coreutils.h"
# undef SINGLE_ద్విమాన_కార్యక్రమం
#అయితే_ముగింపు

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "coreutils"

#నిర్వచించు AUTHORS \
  proper_name ("Alex Deymo")

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
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
      రాయి (_("\
Usage: %s --coreutils-prog=కార్యక్రమం_NAME [PARAMETERS]... \n"),
              కార్యక్రమం_name);
      fputs (_("\
Execute the కార్యక్రమం_NAME built-లోపల కార్యక్రమం with the given PARAMETERS.\n\
\n"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);

#నిర్వచితమైతే SINGLE_ద్విమాన
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      అక్షరం స్థిరరాశి *prog_name_list =
# define SINGLE_ద్విమాన_కార్యక్రమం(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_ద్విమాన_కార్యక్రమం
      ;
      రాయి ("\n\
Built-లోపల programs:\n\
%s\n", prog_name_list);
#అయితే_ముగింపు

      రాయి (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' కొరకు individual కార్యక్రమం help.\n"),
              కార్యక్రమం_name);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

స్థిర శూన్య
launch_కార్యక్రమం (అక్షరం స్థిరరాశి *prog_name, పూర్ణాంకం prog_argc, అక్షరం **prog_argv)
{
  పూర్ణాంకం (*prog_main) (పూర్ణాంకం, అక్షరం **) = ఖాళీ;

  /* Ensure that at least one parameter was passed.  */
  ఒకవేళ (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    తిరిగి;

#నిర్వచితమైతే SINGLE_ద్విమాన
  ఒకవేళ (false);
  /* Look up the right main program.  */
# define SINGLE_ద్విమాన_కార్యక్రమం(prog_name_str, main_name) \
  లేకపోతే ఒకవేళ (streq (prog_name_str, prog_name)) \
    prog_main = single_ద్విమాన_main##main_name;
# include "coreutils.h"
# undef SINGLE_ద్విమాన_కార్యక్రమం
#అయితే_ముగింపు

  ఒకవేళ (! prog_main)
    తిరిగి;

#ఒకవేళ HAVE_PRCTL && defined PR_సమితి_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_సమితి_NAME, చివరి_component (prog_argv[0]));
#అయితే_ముగింపు
#ఒకవేళ HAVE_PRCTL && defined PR_సమితి_MM_ARG_ప్రారంభం
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_సమితి_MM, PR_సమితి_MM_ARG_ప్రారంభం, prog_argv[0], 0, 0);
#అయితే_ముగింపు

  నిష్క్రమించు (prog_main (prog_argc, prog_argv));
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం *prog_name = చివరి_component (argv[0]);
  పూర్ణాంకం optc;

  /* Map external name to internal name.  */
  అక్షరం ginstall[] = "ginstall";
  ఒకవేళ (streq (prog_name, "install"))
    prog_name = ginstall;

  /* If this program is called directly as "coreutils" or if the value of
     argv[0] is an unknown tool (which "coreutils" is), we proceed and parse
     the options.  */
  launch_కార్యక్రమం (prog_name, argc, argv);

  /* No known program was selected via argv[0].  Try parsing the first
     argument as --coreutils-prog=PROGRAM to determine the program.  The
     invocation for this case should be:
       path/to/coreutils --coreutils-prog=someprog someprog ...
     The third argument is what the program will see as argv[0].  */

  size_t nskip = 0;
  ఒకవేళ (argc >= 2)
    {
      అక్షరం *arg_name = ఖాళీ;

      /* If calling coreutils directly, the "script" name isn't passed.
         Distinguish the two cases with a -shebang suffix.  */
      ఒకవేళ (STRPREFIX (argv[1], "--coreutils-prog="))
        {
          nskip = 1;
          arg_name = prog_name = argv[1] + అక్షరశ్రేణి_పొడవు ("--coreutils-prog=");
        }
      లేకపోతే ఒకవేళ (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + అక్షరశ్రేణి_పొడవు ("--coreutils-prog-shebang=");
          ఒకవేళ (argc >= 3)
            arg_name = చివరి_component (argv[2]);
          లేకపోతే
            arg_name = prog_name;
        }

      ఒకవేళ (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_కార్యక్రమం (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  ఒకవేళ (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
    {
      fprintf (stderr, _("%s: unknown program %s\n"),
               కార్యక్రమం_NAME, quote (prog_name));
      నిష్క్రమించు (EXIT_FAILURE);
    }

  /* From here on, we behave like any other coreutils program.  */
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  atexit (close_stdout);

  ఒకవేళ ((optc = getopt_long (argc, argv, "", long_options, ఖాళీ)) != -1)
    ఎంపిక (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
      }

  usage (EXIT_FAILURE);
}
