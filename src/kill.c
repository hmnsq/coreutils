/* kill -- send a signal to a process
   Copyright (C) 2002-2026 Free Software Foundation, Inc.

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

/* Written by Paul Eggert.  */

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <signal.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "sig2str.h"
#अन्तर्भाव "operand2sig.h"
#अन्तर्भाव "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "kill"

#define AUTHORS proper_name ("Paul Eggert")

स्थिर अक्षर स्थिरांक short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

स्थिर संरचना option स्थिरांक long_options[] =
{
  {"list", no_argument, रिक्त, 'l'},
  {"signal", required_argument, रिक्त, 's'},
  {"table", no_argument, रिक्त, 't'},
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
Usage: %s [-s SIGNAL | -SIGNAL] PID...\n\
  or:  %s -l [SIGNAL]...\n\
  or:  %s -t [SIGNAL]...\n\
"),
              program_name, program_name, program_name);
      ख_माला_दो (_("\
Send signals to processes, or list signals.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -s, --संकेत=SIGNAL, -SIGNAL\n\
         specify the name or संख्या of the संकेत to be sent\n\
"));
      oputs (_("\
  -l, --list\n\
         list संकेत names, or convert संकेत names to/from numbers\n\
"));
      oputs (_("\
  -t, --table\n\
         print a table of संकेत information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\n\
SIGNAL may be a संकेत name like 'HUP', or a संकेत संख्या like '1',\n\
or the निकास status of a process terminated by a संकेत.\n\
PID is an integer; जर negative it identifies a process group.\n\
"), मानक_निकास);
      म_लिखो (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

स्थिर शून्य
print_table_row (पूर्णांक अंक_width, पूर्णांक signum,
                 पूर्णांक name_width, अक्षर स्थिरांक *signame)
{
  अक्षर स्थिरांक *description = strsignal (signum);
  म_लिखो ("%*d %-*s %s\n", अंक_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

स्थिर पूर्णांक
list_signals (bool table, अक्षर *स्थिरांक *argv)
{
  पूर्णांक status = निकास_सफल;
  अक्षर signame[SIG2STR_MAX];

  जर (table)
    {
      पूर्णांक name_width = 0;

      /* Compute the maximum width of a signal number.  */
      पूर्णांक अंक_width = 1;
      साठी (पूर्णांक signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        अंक_width++;

      /* Compute the maximum width of a signal name.  */
      साठी (पूर्णांक signum = 0; signum <= SIGNUM_BOUND; signum++)
        जर (sig2str (signum, signame) == 0)
          {
            idx_t len = म_माप (signame);
            जर (name_width < len)
              name_width = len;
          }

      जर (argv)
        साठी (; *argv; argv++)
          {
            पूर्णांक signum = operand2sig (*argv);
            जर (signum < 0)
              status = निकास_त्रुटि;
            अन्यथा
              {
                जर (sig2str (signum, signame) != 0)
                  snprintf (signame, आकार signame, "SIG%d", signum);
                print_table_row (अंक_width, signum, name_width, signame);
              }
          }
      अन्यथा
        साठी (पूर्णांक signum = 0; signum <= SIGNUM_BOUND; signum++)
          जर (sig2str (signum, signame) == 0)
            print_table_row (अंक_width, signum, name_width, signame);
    }
  अन्यथा
    {
      जर (argv)
        साठी (; *argv; argv++)
          {
            पूर्णांक signum = operand2sig (*argv);
            जर (signum < 0)
              status = निकास_त्रुटि;
            अन्यथा जर (c_isdigit (**argv))
              {
                जर (sig2str (signum, signame) == 0)
                  माला_दो (signame);
                अन्यथा
                  म_लिखो ("%d\n", signum);
              }
            अन्यथा
              म_लिखो ("%d\n", signum);
          }
      अन्यथा
        साठी (पूर्णांक signum = 0; signum <= SIGNUM_BOUND; signum++)
          जर (sig2str (signum, signame) == 0)
            माला_दो (signame);
    }

  परत status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

स्थिर पूर्णांक
send_signals (पूर्णांक signum, अक्षर *स्थिरांक *argv)
{
  पूर्णांक status = निकास_सफल;
  अक्षर स्थिरांक *arg = *argv;

  करा
    {
      अक्षर *endp;
      intmax_t n = (त्रुटि_सं = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      जर (त्रुटि_सं == दुस्फल || ckd_add (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          status = निकास_त्रुटि;
        }
      अन्यथा जर (समाप्त (pid, signum) != 0)
        {
          जर (त्रुटि_सं == EINVAL)
            error (0, त्रुटि_सं, "%d", signum);
          अन्यथा
            error (0, त्रुटि_सं, "%s", quote (arg));
          status = निकास_त्रुटि;
        }
    }
  पर्यंत ((arg = *++argv));

  परत status;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc;
  bool list = false;
  bool table = false;
  पूर्णांक signum = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत ((optc = getopt_long (argc, argv, short_options, long_options, रिक्त))
         != -1)
    निवड (optc)
      {
      स्थिती '0': स्थिती '1': स्थिती '2': स्थिती '3': स्थिती '4':
      स्थिती '5': स्थिती '6': स्थिती '7': स्थिती '8': स्थिती '9':
        जर (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            जा no_more_options;
          }
        FALLTHROUGH;
      स्थिती 'A': स्थिती 'B': स्थिती 'C': स्थिती 'D': स्थिती 'E':
      स्थिती 'F': स्थिती 'G': स्थिती 'H': स्थिती 'I': स्थिती 'J':
      स्थिती 'K': /*case 'L':*/ स्थिती 'M': स्थिती 'N': स्थिती 'O':
      स्थिती 'P': स्थिती 'Q': स्थिती 'R': स्थिती 'S': स्थिती 'T':
      स्थिती 'U': स्थिती 'V': स्थिती 'W': स्थिती 'X': स्थिती 'Y':
      स्थिती 'Z':
        जर (! optarg)
          optarg = argv[optind - 1] + म_माप (argv[optind - 1]);
        जर (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (निकास_त्रुटि);
          }
        optarg--;
        FALLTHROUGH;
      स्थिती 'n': /* -n is not documented, but is for Bash compatibility.  */
      स्थिती 's':
        जर (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (निकास_त्रुटि);
          }
        signum = operand2sig (optarg);
        जर (signum < 0)
          usage (निकास_त्रुटि);
        थांबा;

      स्थिती 'L': /* -L is not documented, but is for procps compatibility.  */
      स्थिती 't':
        table = true;
        FALLTHROUGH;
      स्थिती 'l':
        जर (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (निकास_त्रुटि);
          }
        list = true;
        थांबा;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      पूर्वनिर्धारित:
        usage (निकास_त्रुटि);
      }
 no_more_options:

  जर (signum < 0)
    signum = संक_इति;
  अन्यथा जर (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (निकास_त्रुटि);
    }

  जर ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (निकास_त्रुटि);
    }

  परत (list
          ? list_signals (table, optind < argc ? argv + optind : रिक्त)
          : send_signals (signum, argv + optind));
}
