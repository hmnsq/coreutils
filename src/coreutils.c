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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଯଦି HAVE_PRCTL
# include <sys/prctl.h>
#endif

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"

#ifdef SINGLE_ଦ୍ୱିମିକ
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_ଦ୍ୱିମିକ_PROGRAM(prog_name_str, main_name) \
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା single_ଦ୍ୱିମିକ_main##main_name (ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅକ୍ଷର **);
# include "coreutils.h"
# undef SINGLE_ଦ୍ୱିମିକ_PROGRAM
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "coreutils"

#define AUTHORS \
  proper_name ("Alex Deymo")

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};


ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s --coreutils-prog=PROGRAM_NAME [PARAMETERS]... \n"),
              program_name);
      fputs (_("\
Execute the PROGRAM_NAME built-in program with the given PARAMETERS.\n\
\n"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

#ifdef SINGLE_ଦ୍ୱିମିକ
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      ଅକ୍ଷର ଅଚଳ *prog_name_list =
# define SINGLE_ଦ୍ୱିମିକ_PROGRAM(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_ଦ୍ୱିମିକ_PROGRAM
      ;
      printf ("\n\
Built-in programs:\n\
%s\n", prog_name_list);
#endif

      printf (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' ପାଇଁ individual program help.\n"),
              program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ସ୍ଥିର ଶୂନ୍ୟ
launch_program (ଅକ୍ଷର ଅଚଳ *prog_name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା prog_argc, ଅକ୍ଷର **prog_argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*prog_main) (ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ଅକ୍ଷର **) = ଖାଲି;

  /* Ensure that at least one parameter was passed.  */
  ଯଦି (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    ଫେରନ୍ତୁ;

#ifdef SINGLE_ଦ୍ୱିମିକ
  ଯଦି (false);
  /* Look up the right main program.  */
# define SINGLE_ଦ୍ୱିମିକ_PROGRAM(prog_name_str, main_name) \
  ଅନ୍ୟଥା ଯଦି (streq (prog_name_str, prog_name)) \
    prog_main = single_ଦ୍ୱିମିକ_main##main_name;
# include "coreutils.h"
# undef SINGLE_ଦ୍ୱିମିକ_PROGRAM
#endif

  ଯଦି (! prog_main)
    ଫେରନ୍ତୁ;

#ଯଦି HAVE_PRCTL && defined PR_SET_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_SET_NAME, last_component (prog_argv[0]));
#endif
#ଯଦି HAVE_PRCTL && defined PR_SET_MM_ARG_START
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_SET_MM, PR_SET_MM_ARG_START, prog_argv[0], 0, 0);
#endif

  exit (prog_main (prog_argc, prog_argv));
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଅକ୍ଷର *prog_name = last_component (argv[0]);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;

  /* Map external name to internal name.  */
  ଅକ୍ଷର ginstall[] = "ginstall";
  ଯଦି (streq (prog_name, "install"))
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
  ଯଦି (argc >= 2)
    {
      ଅକ୍ଷର *arg_name = ଖାଲି;

      /* If calling coreutils directly, the "script" name isn't passed.
         Distinguish the two cases with a -shebang suffix.  */
      ଯଦି (STRPREFIX (argv[1], "--coreutils-prog="))
        {
          nskip = 1;
          arg_name = prog_name = argv[1] + strlen ("--coreutils-prog=");
        }
      ଅନ୍ୟଥା ଯଦି (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + strlen ("--coreutils-prog-shebang=");
          ଯଦି (argc >= 3)
            arg_name = last_component (argv[2]);
          ଅନ୍ୟଥା
            arg_name = prog_name;
        }

      ଯଦି (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_program (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  ଯଦି (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
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

  ଯଦି ((optc = getopt_long (argc, argv, "", long_options, ଖାଲି)) != -1)
    ବାଛ (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      }

  usage (EXIT_FAILURE);
}
