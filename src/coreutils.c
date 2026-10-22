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
#यदि HAVE_PRCTL
# include <sys/prctl.h>
#यदि_अन्तः

#अन्तर्भाव "system.h"
#अन्तर्भाव "quote.h"

#यदि_परिभाषितम् SINGLE_द्विमान
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_द्विमान_क्रमादेशः(prog_name_str, main_name) \
  पूर्णाङ्क single_द्विमान_main##main_name (पूर्णाङ्क, वर्ण **);
# include "coreutils.h"
# undef SINGLE_द्विमान_क्रमादेशः
#यदि_अन्तः

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "coreutils"

#परिभाषय AUTHORS \
  proper_name ("Alex Deymo")

स्थिर संरचना option ध्रुव long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};


शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s --coreutils-prog=क्रमादेशः_NAME [PARAMETERS]... \n"),
              क्रमादेशः_name);
      fputs (_("\
Execute the क्रमादेशः_NAME built-अन्तर् क्रमादेशः with the given PARAMETERS.\n\
\n"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);

#यदि_परिभाषितम् SINGLE_द्विमान
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      वर्ण ध्रुव *prog_name_list =
# define SINGLE_द्विमान_क्रमादेशः(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_द्विमान_क्रमादेशः
      ;
      प्रारूप_लिख ("\n\
Built-अन्तर् programs:\n\
%s\n", prog_name_list);
#यदि_अन्तः

      प्रारूप_लिख (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' प्रति individual क्रमादेशः help.\n"),
              क्रमादेशः_name);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

स्थिर शून्य
launch_क्रमादेशः (वर्ण ध्रुव *prog_name, पूर्णाङ्क prog_argc, वर्ण **prog_argv)
{
  पूर्णाङ्क (*prog_main) (पूर्णाङ्क, वर्ण **) = रिक्त;

  /* Ensure that at least one parameter was passed.  */
  यदि (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    प्रत्यागम;

#यदि_परिभाषितम् SINGLE_द्विमान
  यदि (false);
  /* Look up the right main program.  */
# define SINGLE_द्विमान_क्रमादेशः(prog_name_str, main_name) \
  अन्यथा यदि (streq (prog_name_str, prog_name)) \
    prog_main = single_द्विमान_main##main_name;
# include "coreutils.h"
# undef SINGLE_द्विमान_क्रमादेशः
#यदि_अन्तः

  यदि (! prog_main)
    प्रत्यागम;

#यदि HAVE_PRCTL && defined PR_समुच्चयः_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_समुच्चयः_NAME, चरमः_component (prog_argv[0]));
#यदि_अन्तः
#यदि HAVE_PRCTL && defined PR_समुच्चयः_MM_ARG_START
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_समुच्चयः_MM, PR_समुच्चयः_MM_ARG_START, prog_argv[0], 0, 0);
#यदि_अन्तः

  निर्गच्छ (prog_main (prog_argc, prog_argv));
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण *prog_name = चरमः_component (argv[0]);
  पूर्णाङ्क optc;

  /* Map external name to internal name.  */
  वर्ण ginstall[] = "ginstall";
  यदि (streq (prog_name, "install"))
    prog_name = ginstall;

  /* If this program is called directly as "coreutils" or if the value of
     argv[0] is an unknown tool (which "coreutils" is), we proceed and parse
     the options.  */
  launch_क्रमादेशः (prog_name, argc, argv);

  /* No known program was selected via argv[0].  Try parsing the first
     argument as --coreutils-prog=PROGRAM to determine the program.  The
     invocation for this case should be:
       path/to/coreutils --coreutils-prog=someprog someprog ...
     The third argument is what the program will see as argv[0].  */

  size_t nskip = 0;
  यदि (argc >= 2)
    {
      वर्ण *arg_name = रिक्त;

      /* If calling coreutils directly, the "script" name isn't passed.
         Distinguish the two cases with a -shebang suffix.  */
      यदि (STRPREFIX (argv[1], "--coreutils-prog="))
        {
          nskip = 1;
          arg_name = prog_name = argv[1] + वाक्यदैर्घ्यम् ("--coreutils-prog=");
        }
      अन्यथा यदि (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + वाक्यदैर्घ्यम् ("--coreutils-prog-shebang=");
          यदि (argc >= 3)
            arg_name = चरमः_component (argv[2]);
          अन्यथा
            arg_name = prog_name;
        }

      यदि (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_क्रमादेशः (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  यदि (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
    {
      fprintf (stderr, _("%s: unknown program %s\n"),
               क्रमादेशः_NAME, quote (prog_name));
      निर्गच्छ (EXIT_FAILURE);
    }

  /* From here on, we behave like any other coreutils program.  */
  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  atexit (close_stdout);

  यदि ((optc = getopt_long (argc, argv, "", long_options, रिक्त)) != -1)
    विकल्प (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
      }

  usage (EXIT_FAILURE);
}
