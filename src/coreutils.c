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
#پایان_اگر

#ضمن "system.h"
#ضمن "quote.h"

#اگر_تعریف_شده SINGLE_دودویی
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_دودویی_برنامه(prog_name_str, main_name) \
  صحيح single_دودویی_main##main_name (صحيح, حرف **);
# include "coreutils.h"
# undef SINGLE_دودویی_برنامه
#پایان_اگر

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "coreutils"

#تعریف_کن AUTHORS \
  proper_name ("Alex Deymo")

ساكن بنية option ثابت long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};


فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s --coreutils-prog=برنامه_NAME [PARAMETERS]... \n"),
              برنامه_name);
      fputs (_("\
Execute the برنامه_NAME built-درون برنامه with the given PARAMETERS.\n\
\n"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);

#اگر_تعریف_شده SINGLE_دودویی
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      حرف ثابت *prog_name_list =
# define SINGLE_دودویی_برنامه(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_دودویی_برنامه
      ;
      چاپ_کن ("\n\
Built-درون programs:\n\
%s\n", prog_name_list);
#پایان_اگر

      چاپ_کن (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' لكل individual برنامه help.\n"),
              برنامه_name);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

ساكن فراغ
launch_برنامه (حرف ثابت *prog_name, صحيح prog_argc, حرف **prog_argv)
{
  صحيح (*prog_main) (صحيح, حرف **) = عدم;

  /* Ensure that at least one parameter was passed.  */
  إذا (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    أرجع;

#اگر_تعریف_شده SINGLE_دودویی
  إذا (false);
  /* Look up the right main program.  */
# define SINGLE_دودویی_برنامه(prog_name_str, main_name) \
  وإلا إذا (streq (prog_name_str, prog_name)) \
    prog_main = single_دودویی_main##main_name;
# include "coreutils.h"
# undef SINGLE_دودویی_برنامه
#پایان_اگر

  إذا (! prog_main)
    أرجع;

#إذا HAVE_PRCTL && defined PR_مجموعه_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_مجموعه_NAME, آخرین_component (prog_argv[0]));
#پایان_اگر
#إذا HAVE_PRCTL && defined PR_مجموعه_MM_ARG_شروع
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_مجموعه_MM, PR_مجموعه_MM_ARG_شروع, prog_argv[0], 0, 0);
#پایان_اگر

  خروج (prog_main (prog_argc, prog_argv));
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  حرف *prog_name = آخرین_component (argv[0]);
  صحيح optc;

  /* Map external name to internal name.  */
  حرف ginstall[] = "ginstall";
  إذا (streq (prog_name, "install"))
    prog_name = ginstall;

  /* If this program is called directly as "coreutils" or if the value of
     argv[0] is an unknown tool (which "coreutils" is), we proceed and parse
     the options.  */
  launch_برنامه (prog_name, argc, argv);

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
          arg_name = prog_name = argv[1] + طول_رشته ("--coreutils-prog=");
        }
      وإلا إذا (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + طول_رشته ("--coreutils-prog-shebang=");
          إذا (argc >= 3)
            arg_name = آخرین_component (argv[2]);
          وإلا
            arg_name = prog_name;
        }

      إذا (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_برنامه (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  إذا (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
    {
      fprintf (stderr, _("%s: unknown program %s\n"),
               برنامه_NAME, quote (prog_name));
      خروج (EXIT_FAILURE);
    }

  /* From here on, we behave like any other coreutils program.  */
  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  atexit (close_stdout);

  إذا ((optc = getopt_long (argc, argv, "", long_options, عدم)) != -1)
    بدل (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);
      }

  usage (EXIT_FAILURE);
}
