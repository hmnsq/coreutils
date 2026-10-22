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

#समावेश <config.h>
#समावेश <getopt.h>
#समावेश <stdio.h>
#अगर HAVE_PRCTL
# include <sys/prctl.h>
#पूर्ण_अगर

#समावेश "system.h"
#समावेश "quote.h"

#अगर_घोषित SINGLE_द्विआधारी
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_द्विआधारी_PROGRAM(prog_name_str, main_name) \
  पूर्णांक single_द्विआधारी_main##main_name (पूर्णांक, अक्षर **);
# include "coreutils.h"
# undef SINGLE_द्विआधारी_PROGRAM
#पूर्ण_अगर

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "coreutils"

#घोषणा AUTHORS \
  proper_name ("Alex Deymo")

अटल काष्ठा option स्थिर long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};


व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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

#अगर_घोषित SINGLE_द्विआधारी
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      अक्षर स्थिर *prog_name_list =
# define SINGLE_द्विआधारी_PROGRAM(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_द्विआधारी_PROGRAM
      ;
      म_लिखो ("\n\
Built-in programs:\n\
%s\n", prog_name_list);
#पूर्ण_अगर

      म_लिखो (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' क्रम individual program help.\n"),
              program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

अटल व्योम
launch_program (अक्षर स्थिर *prog_name, पूर्णांक prog_argc, अक्षर **prog_argv)
{
  पूर्णांक (*prog_main) (पूर्णांक, अक्षर **) = शून्य;

  /* Ensure that at least one parameter was passed.  */
  अगर (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    वापस;

#अगर_घोषित SINGLE_द्विआधारी
  अगर (false);
  /* Look up the right main program.  */
# define SINGLE_द्विआधारी_PROGRAM(prog_name_str, main_name) \
  अन्यथा अगर (streq (prog_name_str, prog_name)) \
    prog_main = single_द्विआधारी_main##main_name;
# include "coreutils.h"
# undef SINGLE_द्विआधारी_PROGRAM
#पूर्ण_अगर

  अगर (! prog_main)
    वापस;

#अगर HAVE_PRCTL && defined PR_SET_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_SET_NAME, last_component (prog_argv[0]));
#पूर्ण_अगर
#अगर HAVE_PRCTL && defined PR_SET_MM_ARG_START
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_SET_MM, PR_SET_MM_ARG_START, prog_argv[0], 0, 0);
#पूर्ण_अगर

  निकास (prog_main (prog_argc, prog_argv));
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अक्षर *prog_name = last_component (argv[0]);
  पूर्णांक optc;

  /* Map external name to internal name.  */
  अक्षर ginstall[] = "ginstall";
  अगर (streq (prog_name, "install"))
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
  अगर (argc >= 2)
    {
      अक्षर *arg_name = शून्य;

      /* If calling coreutils directly, the "script" name isn't passed.
         Distinguish the two cases with a -shebang suffix.  */
      अगर (STRPREFIX (argv[1], "--coreutils-prog="))
        {
          nskip = 1;
          arg_name = prog_name = argv[1] + म_माप ("--coreutils-prog=");
        }
      अन्यथा अगर (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + म_माप ("--coreutils-prog-shebang=");
          अगर (argc >= 3)
            arg_name = last_component (argv[2]);
          अन्यथा
            arg_name = prog_name;
        }

      अगर (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_program (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  अगर (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
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

  अगर ((optc = getopt_long (argc, argv, "", long_options, शून्य)) != -1)
    चयन (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      }

  usage (निकास_त्रुटि);
}
