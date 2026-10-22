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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <stdio.h>
#જો HAVE_PRCTL
# include <sys/prctl.h>
#endif

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "quote.h"

#ifdef SINGLE_દ્વિઅંકી
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_દ્વિઅંકી_PROGRAM(prog_name_str, main_name) \
  પૂર્ણાંક single_દ્વિઅંકી_main##main_name (પૂર્ણાંક, અક્ષર **);
# include "coreutils.h"
# undef SINGLE_દ્વિઅંકી_PROGRAM
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "coreutils"

#define AUTHORS \
  proper_name ("Alex Deymo")

સ્થિર સંરચના option અચલ long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};


શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s --coreutils-prog=PROGRAM_NAME [PARAMETERS]... \n"),
              program_name);
      fputs (_("\
Execute the PROGRAM_NAME built-in program with the given PARAMETERS.\n\
\n"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

#ifdef SINGLE_દ્વિઅંકી
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      અક્ષર અચલ *prog_name_list =
# define SINGLE_દ્વિઅંકી_PROGRAM(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_દ્વિઅંકી_PROGRAM
      ;
      printf ("\n\
Built-in programs:\n\
%s\n", prog_name_list);
#endif

      printf (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' માટે individual program help.\n"),
              program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

સ્થિર શૂન્ય
launch_program (અક્ષર અચલ *prog_name, પૂર્ણાંક prog_argc, અક્ષર **prog_argv)
{
  પૂર્ણાંક (*prog_main) (પૂર્ણાંક, અક્ષર **) = ખાલી;

  /* Ensure that at least one parameter was passed.  */
  જો (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    પાછા;

#ifdef SINGLE_દ્વિઅંકી
  જો (false);
  /* Look up the right main program.  */
# define SINGLE_દ્વિઅંકી_PROGRAM(prog_name_str, main_name) \
  નહીંતર જો (streq (prog_name_str, prog_name)) \
    prog_main = single_દ્વિઅંકી_main##main_name;
# include "coreutils.h"
# undef SINGLE_દ્વિઅંકી_PROGRAM
#endif

  જો (! prog_main)
    પાછા;

#જો HAVE_PRCTL && defined PR_SET_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_SET_NAME, last_component (prog_argv[0]));
#endif
#જો HAVE_PRCTL && defined PR_SET_MM_ARG_START
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_SET_MM, PR_SET_MM_ARG_START, prog_argv[0], 0, 0);
#endif

  exit (prog_main (prog_argc, prog_argv));
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  અક્ષર *prog_name = last_component (argv[0]);
  પૂર્ણાંક optc;

  /* Map external name to internal name.  */
  અક્ષર ginstall[] = "ginstall";
  જો (streq (prog_name, "install"))
    prog_name = ginstall;

  /* If this program is called directly as "coreutils" or if the value of
     argv[0] is an unknown tool (which "coreutils" is), we proceed and parse
     the options.  */
  launch_program (prog_name, argc, argv);

  /* No known program was selected via argv[0].  Try parsing the first
     argument as --coreutils-prog=PROGRAM to determine the program.  The
     invocation for this case should be:
       path/to/coreutils --coreutils-prog=someprog someprog ...
     The third argument is what the program will see as argv[0].  */

  size_t nskip = 0;
  જો (argc >= 2)
    {
      અક્ષર *arg_name = ખાલી;

      /* If calling coreutils directly, the "script" name isn't passed.
         Distinguish the two cases with a -shebang suffix.  */
      જો (STRPREFIX (argv[1], "--coreutils-prog="))
        {
          nskip = 1;
          arg_name = prog_name = argv[1] + strlen ("--coreutils-prog=");
        }
      નહીંતર જો (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + strlen ("--coreutils-prog-shebang=");
          જો (argc >= 3)
            arg_name = last_component (argv[2]);
          નહીંતર
            arg_name = prog_name;
        }

      જો (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_program (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  જો (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
    {
      fprintf (stderr, _("%s: unknown program %s\n"),
               PROGRAM_NAME, quote (prog_name));
      exit (EXIT_FAILURE);
    }

  /* From here on, we behave like any other coreutils program.  */
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  atexit (close_stdout);

  જો ((optc = getopt_long (argc, argv, "", long_options, ખાલી)) != -1)
    પસંદગી (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      }

  usage (EXIT_FAILURE);
}
