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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਜੇ HAVE_PRCTL
# include <sys/prctl.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SINGLE_ਦੁਆਧਾਰੀ
/* Declare the main function on each one of the selected tools.  This name
   needs to match the one passed as CFLAGS on single-binary.mk (generated
   by gen-single-binary.sh). */
# define SINGLE_ਦੁਆਧਾਰੀ_ਪ੍ਰੋਗਰਾਮ(prog_name_str, main_name) \
  ਪੂਰਨ single_ਦੁਆਧਾਰੀ_main##main_name (ਪੂਰਨ, ਅੱਖਰ **);
# include "coreutils.h"
# undef SINGLE_ਦੁਆਧਾਰੀ_ਪ੍ਰੋਗਰਾਮ
#ਜੇ_ਅੰਤ

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "coreutils"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Alex Deymo")

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};


ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s --coreutils-prog=ਪ੍ਰੋਗਰਾਮ_NAME [PARAMETERS]... \n"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Execute the ਪ੍ਰੋਗਰਾਮ_NAME built-ਵਿੱਚ ਪ੍ਰੋਗਰਾਮ with the given PARAMETERS.\n\
\n"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SINGLE_ਦੁਆਧਾਰੀ
/* XXX: Ideally we'd like to present "install" here, not "ginstall".  */
      ਅੱਖਰ ਅਚਲ *prog_name_list =
# define SINGLE_ਦੁਆਧਾਰੀ_ਪ੍ਰੋਗਰਾਮ(prog_name_str, main_name) " " prog_name_str
# include "coreutils.h"
# undef SINGLE_ਦੁਆਧਾਰੀ_ਪ੍ਰੋਗਰਾਮ
      ;
      ਲਿਖੋ ("\n\
Built-ਵਿੱਚ programs:\n\
%s\n", prog_name_list);
#ਜੇ_ਅੰਤ

      ਲਿਖੋ (_("\
\n\
Use: '%s --coreutils-prog=PROGRAM_NAME --help' ਲਈ individual ਪ੍ਰੋਗਰਾਮ help.\n"),
              ਪ੍ਰੋਗਰਾਮ_name);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਸਥਿਰ ਖਾਲੀ
launch_ਪ੍ਰੋਗਰਾਮ (ਅੱਖਰ ਅਚਲ *prog_name, ਪੂਰਨ prog_argc, ਅੱਖਰ **prog_argv)
{
  ਪੂਰਨ (*prog_main) (ਪੂਰਨ, ਅੱਖਰ **) = ਨਿਰਾ;

  /* Ensure that at least one parameter was passed.  */
  ਜੇ (!prog_argc || !prog_argv || !prog_argv[0] || !prog_name)
    ਵਾਪਸ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ SINGLE_ਦੁਆਧਾਰੀ
  ਜੇ (false);
  /* Look up the right main program.  */
# define SINGLE_ਦੁਆਧਾਰੀ_ਪ੍ਰੋਗਰਾਮ(prog_name_str, main_name) \
  ਨਹੀਂਤਾਂ ਜੇ (streq (prog_name_str, prog_name)) \
    prog_main = single_ਦੁਆਧਾਰੀ_main##main_name;
# include "coreutils.h"
# undef SINGLE_ਦੁਆਧਾਰੀ_ਪ੍ਰੋਗਰਾਮ
#ਜੇ_ਅੰਤ

  ਜੇ (! prog_main)
    ਵਾਪਸ;

#ਜੇ HAVE_PRCTL && defined PR_ਸੈੱਟ_NAME
  /* Not being able to set the program name is not a fatal error.  */
  prctl (PR_ਸੈੱਟ_NAME, ਆਖਰੀ_component (prog_argv[0]));
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_PRCTL && defined PR_ਸੈੱਟ_MM_ARG_ਸ਼ੁਰੂ
  /* Shift the beginning of the command line to prog_argv[0] (if set) so
     /proc/$pid/cmdline reflects a more specific value.  Note one needs
     CAP_SYS_RESOURCE or root privileges for this to succeed.  */
  prctl (PR_ਸੈੱਟ_MM, PR_ਸੈੱਟ_MM_ARG_ਸ਼ੁਰੂ, prog_argv[0], 0, 0);
#ਜੇ_ਅੰਤ

  ਬਾਹਰ_ਨਿਕਲੋ (prog_main (prog_argc, prog_argv));
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ *prog_name = ਆਖਰੀ_component (argv[0]);
  ਪੂਰਨ optc;

  /* Map external name to internal name.  */
  ਅੱਖਰ ginstall[] = "ginstall";
  ਜੇ (streq (prog_name, "install"))
    prog_name = ginstall;

  /* If this program is called directly as "coreutils" or if the value of
     argv[0] is an unknown tool (which "coreutils" is), we proceed and parse
     the options.  */
  launch_ਪ੍ਰੋਗਰਾਮ (prog_name, argc, argv);

  /* No known program was selected via argv[0].  Try parsing the first
     argument as --coreutils-prog=PROGRAM to determine the program.  The
     invocation for this case should be:
       path/to/coreutils --coreutils-prog=someprog someprog ...
     The third argument is what the program will see as argv[0].  */

  size_t nskip = 0;
  ਜੇ (argc >= 2)
    {
      ਅੱਖਰ *arg_name = ਨਿਰਾ;

      /* If calling coreutils directly, the "script" name isn't passed.
         Distinguish the two cases with a -shebang suffix.  */
      ਜੇ (STRPREFIX (argv[1], "--coreutils-prog="))
        {
          nskip = 1;
          arg_name = prog_name = argv[1] + ਲੜੀ_ਲੰਬਾਈ ("--coreutils-prog=");
        }
      ਨਹੀਂਤਾਂ ਜੇ (STRPREFIX (argv[1], "--coreutils-prog-shebang="))
        {
          nskip = 2;
          prog_name = argv[1] + ਲੜੀ_ਲੰਬਾਈ ("--coreutils-prog-shebang=");
          ਜੇ (argc >= 3)
            arg_name = ਆਖਰੀ_component (argv[2]);
          ਨਹੀਂਤਾਂ
            arg_name = prog_name;
        }

      ਜੇ (nskip)
        {
          argv[nskip] = arg_name; /* XXX: Discards any specified path.  */
          launch_ਪ੍ਰੋਗਰਾਮ (prog_name, argc - nskip, argv + nskip);
        }
    }

  /* Only process options if calling multi-call binary directly,
     otherwise `foo --version` would succeed.  */
  ਜੇ (nskip || (prog_name && !str_endswith (prog_name, "coreutils")))
    {
      fprintf (stderr, _("%s: unknown program %s\n"),
               ਪ੍ਰੋਗਰਾਮ_NAME, quote (prog_name));
      ਬਾਹਰ_ਨਿਕਲੋ (EXIT_FAILURE);
    }

  /* From here on, we behave like any other coreutils program.  */
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  atexit (close_stdout);

  ਜੇ ((optc = getopt_long (argc, argv, "", long_options, ਨਿਰਾ)) != -1)
    ਚੋਣ (optc)
      {
      case_GETOPT_HELP_CHAR;

      case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
      }

  usage (EXIT_FAILURE);
}
