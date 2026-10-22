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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಯದಿ HAVE_PRCTL
# include <sys/prctl.h>
#endif

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

#ifdef SINGLE_ದ್ವಿಮಾನ
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_ದ್ವಿಮಾನ_PROGRAM(prog_name_str, main_name) \
  ಪೂರ್ಣಾಙ್ಕ single_ದ್ವಿಮಾನ_main##main_name (ಪೂರ್ಣಾಙ್ಕ, ವರ್ಣ **);
# include "coreutils.h"
# undef SINGLE_ದ್ವಿಮಾನ_PROGRAM
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "coreutils"

#define AUTHORS \
  proper_name ("Alex Deymo")

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};


ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s --coreutils-prog=PROGRAM_NAME [PARAMETERS]... \n"),
              program_name);
      fputs (_("\
Execute the PROGRAM_NAME built-in program with the given PARAMETERS.\n\
\n"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

#ifdef SINGLE_ದ್ವಿಮಾನ
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      ವರ್ಣ ಧ್ರುವ *prog_name_list =
# define SINGLE_ದ್ವಿಮಾನ_PROGRAM(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_ದ್ವಿಮಾನ_PROGRAM
      ;
      printf ("\n\
Built-in programs:\n\
%s\n", prog_name_list);
#endif

      printf (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' ಪ್ರತಿ individual program help.\n"),
              program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಸ್ಥಿರ ಶೂನ್ಯ
launch_program (ವರ್ಣ ಧ್ರುವ *prog_name, ಪೂರ್ಣಾಙ್ಕ prog_argc, ವರ್ಣ **prog_argv)
{
  ಪೂರ್ಣಾಙ್ಕ (*prog_main) (ಪೂರ್ಣಾಙ್ಕ, ವರ್ಣ **) = ರಿಕ್ತ;

  /* Ensure that at least one parameter was passed.  */
  ಯದಿ (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    ಪ್ರತ್ಯಾಗಮ;

#ifdef SINGLE_ದ್ವಿಮಾನ
  ಯದಿ (false);
  /* Look up the right main program.  */
# define SINGLE_ದ್ವಿಮಾನ_PROGRAM(prog_name_str, main_name) \
  ಅನ್ಯಥಾ ಯದಿ (streq (prog_name_str, prog_name)) \
    prog_main = single_ದ್ವಿಮಾನ_main##main_name;
# include "coreutils.h"
# undef SINGLE_ದ್ವಿಮಾನ_PROGRAM
#endif

  ಯದಿ (! prog_main)
    ಪ್ರತ್ಯಾಗಮ;

#ಯದಿ HAVE_PRCTL && defined PR_SET_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_SET_NAME, last_component (prog_argv[0]));
#endif
#ಯದಿ HAVE_PRCTL && defined PR_SET_MM_ARG_START
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_SET_MM, PR_SET_MM_ARG_START, prog_argv[0], 0, 0);
#endif

  exit (prog_main (prog_argc, prog_argv));
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ವರ್ಣ *prog_name = last_component (argv[0]);
  ಪೂರ್ಣಾಙ್ಕ optc;

  /* Map external name to internal name.  */
  ವರ್ಣ ginstall[] = "ginstall";
  ಯದಿ (streq (prog_name, "install"))
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
  ಯದಿ (argc >= 2)
    {
      ವರ್ಣ *arg_name = ರಿಕ್ತ;

      /* If calling coreutils directly, the "script" name isn't passed.
         Distinguish the two cases with a -shebang suffix.  */
      ಯದಿ (STRPREFIX (argv[1], "--coreutils-prog="))
        {
          nskip = 1;
          arg_name = prog_name = argv[1] + strlen ("--coreutils-prog=");
        }
      ಅನ್ಯಥಾ ಯದಿ (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + strlen ("--coreutils-prog-shebang=");
          ಯದಿ (argc >= 3)
            arg_name = last_component (argv[2]);
          ಅನ್ಯಥಾ
            arg_name = prog_name;
        }

      ಯದಿ (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_program (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  ಯದಿ (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
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

  ಯದಿ ((optc = getopt_long (argc, argv, "", long_options, ರಿಕ್ತ)) != -1)
    ವಿಕಲ್ಪ (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      }

  usage (EXIT_FAILURE);
}
