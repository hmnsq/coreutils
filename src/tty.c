/* tty -- print the name of the terminal connected to standard input
   Copyright (C) 1990-2026 Free Software Foundation, Inc.

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

/* Displays "not a tty" if stdin is not a terminal.
   Displays nothing if -s option is given.
   Exit status 0 if stdin is a tty, 1 if not a tty, 2 if usage error,
   3 if write error.

   Written by David MacKenzie <djm@gnu.ai.mit.edu>.  */

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "quote.h"

/* Exit statuses.  */
شمارش
  {
    TTY_STDIN_NOTTY = 1,
    TTY_USAGE = 2,
    TTY_WRITE_ERROR = 3,
    TTY_TTYNAME_FAILURE = 4
  };

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "tty"

#define AUTHORS proper_name ("David MacKenzie")

/* If true, return an exit status but produce no output. */
ایستا bool silent;

ایستا ساختار option ثابت longopts[] =
{
  {"silent", no_argument, پوچ, 's'},
  {"quiet", no_argument, پوچ, 's'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);
      fputs (_("\
Print the file name of the terminal connected to standard درون_داد.\n\
\n\
"), stdout);
      oputs (_("\
  -s, --silent, --quiet\n\
         print nothing, only بازگشت an exit status\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TTY_WRITE_ERROR);
  atexit (close_stdout);

  وقتی ((optc = getopt_long (argc, argv, "s", longopts, پوچ)) != -1)
    {
      انتخاب (optc)
        {
        حالت 's':
          silent = true;
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (TTY_USAGE);
        }
    }

  اگر (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (TTY_USAGE);
    }

  اگر (silent)
    بازگشت isatty (STDIN_FILENO) ? EXIT_SUCCESS : TTY_STDIN_NOTTY;

  صحیح status;
  نویسه ثابت *tty = ttyname (STDIN_FILENO);

  اگر (tty)
    status = EXIT_SUCCESS;
  وگرنه
    {
      صحیح ttyname_err = errno;
      اگر (isatty (STDIN_FILENO))
        error (TTY_TTYNAME_FAILURE, ttyname_err, _("ttyname error"));
      tty = _("not a tty");
      status = TTY_STDIN_NOTTY;
    }

  puts (tty);
  بازگشت status;
}
