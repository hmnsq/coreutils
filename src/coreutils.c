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

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#जर HAVE_PRCTL
# include <sys/prctl.h>
#endif

#अन्तर्भाव "system.h"
#अन्तर्भाव "quote.h"

#ifdef SINGLE_द्विमान
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_द्विमान_PROGRAM(prog_name_str, main_name) \
  पूर्णांक single_द्विमान_main##main_name (पूर्णांक, अक्षर **);
# include "coreutils.h"
# undef SINGLE_द्विमान_PROGRAM
#endif

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "coreutils"

#define AUTHORS \
  proper_name ("Alex Deymo")

स्थिर संरचना option स्थिरांक long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};


शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s --coreutils-prog=PROGRAM_NAME [PARAMETERS]... \n"),
              program_name);
      ख_माला_दो (_("\
Execute the PROGRAM_NAME built-in program with the given PARAMETERS.\n\
\n"), मानक_निकास);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);

#ifdef SINGLE_द्विमान
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      अक्षर स्थिरांक *prog_name_list =
# define SINGLE_द्विमान_PROGRAM(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_द्विमान_PROGRAM
      ;
      म_लिखो ("\n\
Built-in programs:\n\
%s\n", prog_name_list);
#endif

      म_लिखो (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' साठी individual program help.\n"),
              program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

स्थिर शून्य
launch_program (अक्षर स्थिरांक *prog_name, पूर्णांक prog_argc, अक्षर **prog_argv)
{
  पूर्णांक (*prog_main) (पूर्णांक, अक्षर **) = रिक्त;

  /* Ensure that at least one parameter was passed.  */
  जर (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    परत;

#ifdef SINGLE_द्विमान
  जर (false);
  /* Look up the right main program.  */
# define SINGLE_द्विमान_PROGRAM(prog_name_str, main_name) \
  अन्यथा जर (streq (prog_name_str, prog_name)) \
    prog_main = single_द्विमान_main##main_name;
# include "coreutils.h"
# undef SINGLE_द्विमान_PROGRAM
#endif

  जर (! prog_main)
    परत;

#जर HAVE_PRCTL && defined PR_SET_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_SET_NAME, last_component (prog_argv[0]));
#endif
#जर HAVE_PRCTL && defined PR_SET_MM_ARG_START
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_SET_MM, PR_SET_MM_ARG_START, prog_argv[0], 0, 0);
#endif

  निकास (prog_main (prog_argc, prog_argv));
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  अक्षर *prog_name = last_component (argv[0]);
  पूर्णांक optc;

  /* Map external name to internal name.  */
  अक्षर ginstall[] = "ginstall";
  जर (streq (prog_name, "install"))
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

  माप_प्रकार nskip = 0;
  जर (argc >= 2)
    {
      अक्षर *arg_name = रिक्त;

      /* If calling coreutils directly, the "script" name isn't passed.
         Distinguish the two cases with a -shebang suffix.  */
      जर (STRPREFIX (argv[1], "--coreutils-prog="))
        {
          nskip = 1;
          arg_name = prog_name = argv[1] + म_माप ("--coreutils-prog=");
        }
      अन्यथा जर (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + म_माप ("--coreutils-prog-shebang=");
          जर (argc >= 3)
            arg_name = last_component (argv[2]);
          अन्यथा
            arg_name = prog_name;
        }

      जर (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_program (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  जर (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
    {
      ख_लिखो (मानक_त्रुटि, _("%s: unknown program %s\n"),
               PROGRAM_NAME, quote (prog_name));
      निकास (निकास_त्रुटि);
    }

  /* From here on, we behave like any other coreutils program.  */
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  निकास_पर (close_stdout);

  जर ((optc = getopt_long (argc, argv, "", long_options, रिक्त)) != -1)
    निवड (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      }

  usage (निकास_त्रुटि);
}
