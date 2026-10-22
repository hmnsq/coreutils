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
#ಆದರೆ HAVE_PRCTL
# include <sys/prctl.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SINGLE_ದ್ವಿಮಾನ
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_ದ್ವಿಮಾನ_ಕಾರ್ಯಕ್ರಮ(prog_name_str, main_name) \
  ಪೂರ್ಣಾಂಕ single_ದ್ವಿಮಾನ_main##main_name (ಪೂರ್ಣಾಂಕ, ಅಕ್ಷರ **);
# include "coreutils.h"
# undef SINGLE_ದ್ವಿಮಾನ_ಕಾರ್ಯಕ್ರಮ
#ಆದರೆ_ಅಂತ್ಯ

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "coreutils"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Alex Deymo")

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};


ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s --coreutils-prog=ಕಾರ್ಯಕ್ರಮ_NAME [PARAMETERS]... \n"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Execute the ಕಾರ್ಯಕ್ರಮ_NAME built-ಒಳ ಕಾರ್ಯಕ್ರಮ with the given PARAMETERS.\n\
\n"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SINGLE_ದ್ವಿಮಾನ
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prog_name_list =
# define SINGLE_ದ್ವಿಮಾನ_ಕಾರ್ಯಕ್ರಮ(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_ದ್ವಿಮಾನ_ಕಾರ್ಯಕ್ರಮ
      ;
      ಬರೆ ("\n\
Built-ಒಳ programs:\n\
%s\n", prog_name_list);
#ಆದರೆ_ಅಂತ್ಯ

      ಬರೆ (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' ಗಾಗಿ individual ಕಾರ್ಯಕ್ರಮ help.\n"),
              ಕಾರ್ಯಕ್ರಮ_name);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಸ್ಥಿರ ಶೂನ್ಯ
launch_ಕಾರ್ಯಕ್ರಮ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *prog_name, ಪೂರ್ಣಾಂಕ prog_argc, ಅಕ್ಷರ **prog_argv)
{
  ಪೂರ್ಣಾಂಕ (*prog_main) (ಪೂರ್ಣಾಂಕ, ಅಕ್ಷರ **) = ಖಾಲಿ;

  /* Ensure that at least one parameter was passed.  */
  ಆದರೆ (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    ಹಿಂತಿರುಗಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ SINGLE_ದ್ವಿಮಾನ
  ಆದರೆ (false);
  /* Look up the right main program.  */
# define SINGLE_ದ್ವಿಮಾನ_ಕಾರ್ಯಕ್ರಮ(prog_name_str, main_name) \
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (streq (prog_name_str, prog_name)) \
    prog_main = single_ದ್ವಿಮಾನ_main##main_name;
# include "coreutils.h"
# undef SINGLE_ದ್ವಿಮಾನ_ಕಾರ್ಯಕ್ರಮ
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ (! prog_main)
    ಹಿಂತಿರುಗಿ;

#ಆದರೆ HAVE_PRCTL && defined PR_ಗಣ_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_ಗಣ_NAME, ಕೊನೆಯ_component (prog_argv[0]));
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_PRCTL && defined PR_ಗಣ_MM_ARG_ಆರಂಭ
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_ಗಣ_MM, PR_ಗಣ_MM_ARG_ಆರಂಭ, prog_argv[0], 0, 0);
#ಆದರೆ_ಅಂತ್ಯ

  ನಿರ್ಗಮಿಸು (prog_main (prog_argc, prog_argv));
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ *prog_name = ಕೊನೆಯ_component (argv[0]);
  ಪೂರ್ಣಾಂಕ optc;

  /* Map external name to internal name.  */
  ಅಕ್ಷರ ginstall[] = "ginstall";
  ಆದರೆ (streq (prog_name, "install"))
    prog_name = ginstall;

  /* If this program is called directly as "coreutils" or if the value of
     argv[0] is an unknown tool (which "coreutils" is), we proceed and parse
     the options.  */
  launch_ಕಾರ್ಯಕ್ರಮ (prog_name, argc, argv);

  /* No known program was selected via argv[0].  Try parsing the first
     argument as --coreutils-prog=PROGRAM to determine the program.  The
     invocation for this case should be:
       path/to/coreutils --coreutils-prog=someprog someprog ...
     The third argument is what the program will see as argv[0].  */

  size_t nskip = 0;
  ಆದರೆ (argc >= 2)
    {
      ಅಕ್ಷರ *arg_name = ಖಾಲಿ;

      /* If calling coreutils directly, the "script" name isn't passed.
         Distinguish the two cases with a -shebang suffix.  */
      ಆದರೆ (STRPREFIX (argv[1], "--coreutils-prog="))
        {
          nskip = 1;
          arg_name = prog_name = argv[1] + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("--coreutils-prog=");
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ ("--coreutils-prog-shebang=");
          ಆದರೆ (argc >= 3)
            arg_name = ಕೊನೆಯ_component (argv[2]);
          ಇಲ್ಲದಿದ್ದರೆ
            arg_name = prog_name;
        }

      ಆದರೆ (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_ಕಾರ್ಯಕ್ರಮ (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  ಆದರೆ (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
    {
      fprintf (stderr, _("%s: unknown program %s\n"),
               ಕಾರ್ಯಕ್ರಮ_NAME, quote (prog_name));
      ನಿರ್ಗಮಿಸು (EXIT_FAILURE);
    }

  /* From here on, we behave like any other coreutils program.  */
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  atexit (close_stdout);

  ಆದರೆ ((optc = getopt_long (argc, argv, "", long_options, ಖಾಲಿ)) != -1)
    ಆಯ್ಕೆ (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
      }

  usage (EXIT_FAILURE);
}
