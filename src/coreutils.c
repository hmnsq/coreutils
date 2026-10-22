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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <stdio.h>
#എങ്കിൽ HAVE_PRCTL
# include <sys/prctl.h>
#endif

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "quote.h"

#ifdef SINGLE_ദ്വിമാന
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_ദ്വിമാന_PROGRAM(prog_name_str, main_name) \
  പൂർണ്ണസംഖ്യ single_ദ്വിമാന_main##main_name (പൂർണ്ണസംഖ്യ, അക്ഷരം **);
# include "coreutils.h"
# undef SINGLE_ദ്വിമാന_PROGRAM
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "coreutils"

#define AUTHORS \
  proper_name ("Alex Deymo")

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};


ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s --coreutils-prog=PROGRAM_NAME [PARAMETERS]... \n"),
              program_name);
      fputs (_("\
Execute the PROGRAM_NAME built-in program with the given PARAMETERS.\n\
\n"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

#ifdef SINGLE_ദ്വിമാന
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      അക്ഷരം സ്ഥിരം *prog_name_list =
# define SINGLE_ദ്വിമാന_PROGRAM(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_ദ്വിമാന_PROGRAM
      ;
      printf ("\n\
Built-in programs:\n\
%s\n", prog_name_list);
#endif

      printf (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' വേണ്ടി individual program help.\n"),
              program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

സ്ഥിതം ശൂന്യം
launch_program (അക്ഷരം സ്ഥിരം *prog_name, പൂർണ്ണസംഖ്യ prog_argc, അക്ഷരം **prog_argv)
{
  പൂർണ്ണസംഖ്യ (*prog_main) (പൂർണ്ണസംഖ്യ, അക്ഷരം **) = ശൂന്യ;

  /* Ensure that at least one parameter was passed.  */
  എങ്കിൽ (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    തിരികെ;

#ifdef SINGLE_ദ്വിമാന
  എങ്കിൽ (false);
  /* Look up the right main program.  */
# define SINGLE_ദ്വിമാന_PROGRAM(prog_name_str, main_name) \
  അല്ലെങ്കിൽ എങ്കിൽ (streq (prog_name_str, prog_name)) \
    prog_main = single_ദ്വിമാന_main##main_name;
# include "coreutils.h"
# undef SINGLE_ദ്വിമാന_PROGRAM
#endif

  എങ്കിൽ (! prog_main)
    തിരികെ;

#എങ്കിൽ HAVE_PRCTL && defined PR_SET_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_SET_NAME, last_component (prog_argv[0]));
#endif
#എങ്കിൽ HAVE_PRCTL && defined PR_SET_MM_ARG_START
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_SET_MM, PR_SET_MM_ARG_START, prog_argv[0], 0, 0);
#endif

  exit (prog_main (prog_argc, prog_argv));
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  അക്ഷരം *prog_name = last_component (argv[0]);
  പൂർണ്ണസംഖ്യ optc;

  /* Map external name to internal name.  */
  അക്ഷരം ginstall[] = "ginstall";
  എങ്കിൽ (streq (prog_name, "install"))
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
  എങ്കിൽ (argc >= 2)
    {
      അക്ഷരം *arg_name = ശൂന്യ;

      /* If calling coreutils directly, the "script" name isn't passed.
         Distinguish the two cases with a -shebang suffix.  */
      എങ്കിൽ (STRPREFIX (argv[1], "--coreutils-prog="))
        {
          nskip = 1;
          arg_name = prog_name = argv[1] + strlen ("--coreutils-prog=");
        }
      അല്ലെങ്കിൽ എങ്കിൽ (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + strlen ("--coreutils-prog-shebang=");
          എങ്കിൽ (argc >= 3)
            arg_name = last_component (argv[2]);
          അല്ലെങ്കിൽ
            arg_name = prog_name;
        }

      എങ്കിൽ (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_program (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  എങ്കിൽ (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
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

  എങ്കിൽ ((optc = getopt_long (argc, argv, "", long_options, ശൂന്യ)) != -1)
    തിരഞ്ഞെടുപ്പ് (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      }

  usage (EXIT_FAILURE);
}
