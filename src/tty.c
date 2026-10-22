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
شمار
  {
    TTY_STDIN_NOTTY = 1,
    TTY_USAGE = 2,
    TTY_WRITE_ERROR = 3,
    TTY_TTYNAME_FAILURE = 4
  };

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "tty"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

/* If true, return an exit status but produce no output. */
ساکن bool silent;

ساکن ساخت option ثابت longopts[] =
{
  {"silent", no_دلیل, تہی, 's'},
  {"quiet", no_دلیل, تہی, 's'},
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
      لکھو (_("Usage: %s [OPTION]...\n"), پروگرام_name);
      fputs (_("\
Print the file name کا the terminal connected مقصد standard داخلہ.\n\
\n\
"), stdout);
      oputs (_("\
  -s, --silent, --quiet\n\
         print nothing, only واپس an باہر_نکلو حالت_2\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح optc;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TTY_WRITE_ERROR);
  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, "s", longopts, تہی)) != -1)
    {
      انتخاب (optc)
        {
        حالت 's':
          silent = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (TTY_USAGE);
        }
    }

  اگر (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (TTY_USAGE);
    }

  اگر (silent)
    واپس isatty (STDIN_FILENO) ? EXIT_SUCCESS : TTY_STDIN_NOTTY;

  صحیح حالت_2;
  حرف ثابت *tty = ttyname (STDIN_FILENO);

  اگر (tty)
    حالت_2 = EXIT_SUCCESS;
  ورنہ
    {
      صحیح ttyname_err = errno;
      اگر (isatty (STDIN_FILENO))
        error (TTY_TTYNAME_FAILURE, ttyname_err, _("ttyname error"));
      tty = _("not a tty");
      حالت_2 = TTY_STDIN_NOTTY;
    }

  puts (tty);
  واپس حالت_2;
}
