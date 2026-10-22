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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "quote.h"

/* Exit statuses.  */
क्रमागत
  {
    TTY_STDIN_NOTTY = 1,
    TTY_USAGE = 2,
    TTY_WRITE_ERROR = 3,
    TTY_TTYNAME_FAILURE = 4
  };

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "tty"

#घोषणा AUTHORS proper_name ("David MacKenzie")

/* If true, return an exit status but produce no output. */
अटल bool silent;

अटल काष्ठा option स्थिर longopts[] =
{
  {"silent", no_argument, शून्य, 's'},
  {"quiet", no_argument, शून्य, 's'},
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
      म_लिखो (_("Usage: %s [OPTION]...\n"), program_name);
      ख_माला_दो (_("\
Print the file name of the terminal connected to standard निवेश.\n\
\n\
"), मानक_निकास);
      oputs (_("\
  -s, --silent, --quiet\n\
         print nothing, only वापस an निकास status\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (TTY_WRITE_ERROR);
  निकास_पर (close_stdout);

  जबतक ((optc = getopt_long (argc, argv, "s", longopts, शून्य)) != -1)
    {
      चयन (optc)
        {
        हाल 's':
          silent = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (TTY_USAGE);
        }
    }

  अगर (optind < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (TTY_USAGE);
    }

  अगर (silent)
    वापस isatty (STDIN_FILENO) ? निकास_सफल : TTY_STDIN_NOTTY;

  पूर्णांक status;
  अक्षर स्थिर *tty = ttyname (STDIN_FILENO);

  अगर (tty)
    status = निकास_सफल;
  अन्यथा
    {
      पूर्णांक ttyname_err = त्रुटि_सं;
      अगर (isatty (STDIN_FILENO))
        error (TTY_TTYNAME_FAILURE, ttyname_err, _("ttyname error"));
      tty = _("not a tty");
      status = TTY_STDIN_NOTTY;
    }

  माला_दो (tty);
  वापस status;
}
