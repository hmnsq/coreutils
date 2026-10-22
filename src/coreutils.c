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

#شامل <config.h>
#شامل <getopt.h>
#شامل <stdio.h>
#اگر HAVE_PRCTL
# include <sys/prctl.h>
#اگر_ختم

#شامل "system.h"
#شامل "quote.h"

#اگر_تعریف_شدہ SINGLE_ثنائی
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_ثنائی_پروگرام(prog_name_str, main_name) \
  صحیح single_ثنائی_main##main_name (صحیح, حرف **);
# include "coreutils.h"
# undef SINGLE_ثنائی_پروگرام
#اگر_ختم

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "coreutils"

#تعریف_کرو AUTHORS \
  proper_name ("Alex Deymo")

ساکن ساخت option ثابت long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};


خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s --coreutils-prog=پروگرام_NAME [PARAMETERS]... \n"),
              پروگرام_name);
      fputs (_("\
Execute the پروگرام_NAME built-اندر پروگرام with the given PARAMETERS.\n\
\n"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);

#اگر_تعریف_شدہ SINGLE_ثنائی
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      حرف ثابت *prog_name_list =
# define SINGLE_ثنائی_پروگرام(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_ثنائی_پروگرام
      ;
      لکھو ("\n\
Built-اندر programs:\n\
%s\n", prog_name_list);
#اگر_ختم

      لکھو (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' برائے individual پروگرام help.\n"),
              پروگرام_name);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

ساکن خالی
launch_پروگرام (حرف ثابت *prog_name, صحیح prog_argc, حرف **prog_argv)
{
  صحیح (*prog_main) (صحیح, حرف **) = تہی;

  /* Ensure that at least one parameter was passed.  */
  اگر (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    واپس;

#اگر_تعریف_شدہ SINGLE_ثنائی
  اگر (false);
  /* Look up the right main program.  */
# define SINGLE_ثنائی_پروگرام(prog_name_str, main_name) \
  ورنہ اگر (streq (prog_name_str, prog_name)) \
    prog_main = single_ثنائی_main##main_name;
# include "coreutils.h"
# undef SINGLE_ثنائی_پروگرام
#اگر_ختم

  اگر (! prog_main)
    واپس;

#اگر HAVE_PRCTL && defined PR_سیٹ_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_سیٹ_NAME, آخری_component (prog_argv[0]));
#اگر_ختم
#اگر HAVE_PRCTL && defined PR_سیٹ_MM_ARG_آغاز
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_سیٹ_MM, PR_سیٹ_MM_ARG_آغاز, prog_argv[0], 0, 0);
#اگر_ختم

  باہر_نکلو (prog_main (prog_argc, prog_argv));
}

صحیح
مین (صحیح argc, حرف **argv)
{
  حرف *prog_name = آخری_component (argv[0]);
  صحیح optc;

  /* Map external name to internal name.  */
  حرف ginstall[] = "ginstall";
  اگر (streq (prog_name, "install"))
    prog_name = ginstall;

  /* If this program is called directly as "coreutils" or if the value of
     argv[0] is an unknown tool (which "coreutils" is), we proceed and parse
     the options.  */
  launch_پروگرام (prog_name, argc, argv);

  /* No known program was selected via argv[0].  Try parsing the first
     argument as --coreutils-prog=PROGRAM to determine the program.  The
     invocation for this case should be:
       path/to/coreutils --coreutils-prog=someprog someprog ...
     The third argument is what the program will see as argv[0].  */

  size_t nskip = 0;
  اگر (argc >= 2)
    {
      حرف *arg_name = تہی;

      /* If calling coreutils directly, the "script" name isn't passed.
         Distinguish the two cases with a -shebang suffix.  */
      اگر (STRPREFIX (argv[1], "--coreutils-prog="))
        {
          nskip = 1;
          arg_name = prog_name = argv[1] + لڑی_لمبائی ("--coreutils-prog=");
        }
      ورنہ اگر (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + لڑی_لمبائی ("--coreutils-prog-shebang=");
          اگر (argc >= 3)
            arg_name = آخری_component (argv[2]);
          ورنہ
            arg_name = prog_name;
        }

      اگر (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_پروگرام (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  اگر (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
    {
      fprintf (stderr, _("%s: unknown program %s\n"),
               پروگرام_NAME, quote (prog_name));
      باہر_نکلو (EXIT_FAILURE);
    }

  /* From here on, we behave like any other coreutils program.  */
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  atexit (close_stdout);

  اگر ((optc = getopt_long (argc, argv, "", long_options, تہی)) != -1)
    انتخاب (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
      }

  usage (EXIT_FAILURE);
}
