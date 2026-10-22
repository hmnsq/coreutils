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

#ضمن <config.h>
#ضمن <getopt.h>
#ضمن <stdio.h>
#إذا HAVE_PRCTL
# include <sys/prctl.h>
#د_که_پای

#ضمن "system.h"
#ضمن "quote.h"

#که_تعریف_شوی SINGLE_دوګونی
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_دوګونی_پروګرام(prog_name_str, main_name) \
  صحيح single_دوګونی_main##main_name (صحيح, حرف **);
# include "coreutils.h"
# undef SINGLE_دوګونی_پروګرام
#د_که_پای

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کړه پروګرام_NAME "coreutils"

#تعریف_کړه AUTHORS \
  proper_name ("Alex Deymo")

ساكن بنية option ثابت long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};


فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s --coreutils-prog=پروګرام_NAME [PARAMETERS]... \n"),
              پروګرام_name);
      fputs (_("\
Execute the پروګرام_NAME built-دننه پروګرام with the given PARAMETERS.\n\
\n"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);

#که_تعریف_شوی SINGLE_دوګونی
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      حرف ثابت *prog_name_list =
# define SINGLE_دوګونی_پروګرام(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_دوګونی_پروګرام
      ;
      چاپ_کړه ("\n\
Built-دننه programs:\n\
%s\n", prog_name_list);
#د_که_پای

      چاپ_کړه (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' لكل individual پروګرام help.\n"),
              پروګرام_name);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

ساكن فراغ
launch_پروګرام (حرف ثابت *prog_name, صحيح prog_argc, حرف **prog_argv)
{
  صحيح (*prog_main) (صحيح, حرف **) = عدم;

  /* Ensure that at least one parameter was passed.  */
  إذا (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    أرجع;

#که_تعریف_شوی SINGLE_دوګونی
  إذا (false);
  /* Look up the right main program.  */
# define SINGLE_دوګونی_پروګرام(prog_name_str, main_name) \
  وإلا إذا (streq (prog_name_str, prog_name)) \
    prog_main = single_دوګونی_main##main_name;
# include "coreutils.h"
# undef SINGLE_دوګونی_پروګرام
#د_که_پای

  إذا (! prog_main)
    أرجع;

#إذا HAVE_PRCTL && defined PR_سېټ_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_سېټ_NAME, وروستی_component (prog_argv[0]));
#د_که_پای
#إذا HAVE_PRCTL && defined PR_سېټ_MM_ARG_پیل
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_سېټ_MM, PR_سېټ_MM_ARG_پیل, prog_argv[0], 0, 0);
#د_که_پای

  وتل (prog_main (prog_argc, prog_argv));
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  حرف *prog_name = وروستی_component (argv[0]);
  صحيح optc;

  /* Map external name to internal name.  */
  حرف ginstall[] = "ginstall";
  إذا (streq (prog_name, "install"))
    prog_name = ginstall;

  /* If this program is called directly as "coreutils" or if the value of
     argv[0] is an unknown tool (which "coreutils" is), we proceed and parse
     the options.  */
  launch_پروګرام (prog_name, argc, argv);

  /* No known program was selected via argv[0].  Try parsing the first
     argument as --coreutils-prog=PROGRAM to determine the program.  The
     invocation for this case should be:
       path/to/coreutils --coreutils-prog=someprog someprog ...
     The third argument is what the program will see as argv[0].  */

  size_t nskip = 0;
  إذا (argc >= 2)
    {
      حرف *arg_name = عدم;

      /* If calling coreutils directly, the "script" name isn't passed.
         Distinguish the two cases with a -shebang suffix.  */
      إذا (STRPREFIX (argv[1], "--coreutils-prog="))
        {
          nskip = 1;
          arg_name = prog_name = argv[1] + مزي_اوږدوالی ("--coreutils-prog=");
        }
      وإلا إذا (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + مزي_اوږدوالی ("--coreutils-prog-shebang=");
          إذا (argc >= 3)
            arg_name = وروستی_component (argv[2]);
          وإلا
            arg_name = prog_name;
        }

      إذا (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_پروګرام (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  إذا (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
    {
      fprintf (stderr, _("%s: unknown program %s\n"),
               پروګرام_NAME, quote (prog_name));
      وتل (EXIT_FAILURE);
    }

  /* From here on, we behave like any other coreutils program.  */
  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  atexit (close_stdout);

  إذا ((optc = getopt_long (argc, argv, "", long_options, عدم)) != -1)
    بدل (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);
      }

  usage (EXIT_FAILURE);
}
