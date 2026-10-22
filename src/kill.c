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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>
#समावेश <signal.h>

#समावेश "system.h"
#समावेश "c-ctype.h"
#समावेश "sig2str.h"
#समावेश "operand2sig.h"
#समावेश "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "kill"

#घोषणा AUTHORS proper_name ("Paul Eggert")

अटल अक्षर स्थिर short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

अटल काष्ठा option स्थिर long_options[] =
{
  {"list", no_argument, शून्य, 'l'},
  {"signal", required_argument, शून्य, 's'},
  {"table", no_argument, शून्य, 't'},
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
PID is an integer; अगर negative it identifies a process group.\n\
"), मानक_निकास);
      म_लिखो (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

अटल व्योम
print_table_row (पूर्णांक अंक_width, पूर्णांक signum,
                 पूर्णांक name_width, अक्षर स्थिर *signame)
{
  अक्षर स्थिर *description = strsignal (signum);
  म_लिखो ("%*d %-*s %s\n", अंक_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

अटल पूर्णांक
list_signals (bool table, अक्षर *स्थिर *argv)
{
  पूर्णांक status = निकास_सफल;
  अक्षर signame[SIG2STR_MAX];

  अगर (table)
    {
      पूर्णांक name_width = 0;

      /* Compute the maximum width of a signal number.  */
      पूर्णांक अंक_width = 1;
      क्रम (पूर्णांक signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        अंक_width++;

      /* Compute the maximum width of a signal name.  */
      क्रम (पूर्णांक signum = 0; signum <= SIGNUM_BOUND; signum++)
        अगर (sig2str (signum, signame) == 0)
          {
            idx_t len = म_माप (signame);
            अगर (name_width < len)
              name_width = len;
          }

      अगर (argv)
        क्रम (; *argv; argv++)
          {
            पूर्णांक signum = operand2sig (*argv);
            अगर (signum < 0)
              status = निकास_त्रुटि;
            अन्यथा
              {
                अगर (sig2str (signum, signame) != 0)
                  snprintf (signame, माप signame, "SIG%d", signum);
                print_table_row (अंक_width, signum, name_width, signame);
              }
          }
      अन्यथा
        क्रम (पूर्णांक signum = 0; signum <= SIGNUM_BOUND; signum++)
          अगर (sig2str (signum, signame) == 0)
            print_table_row (अंक_width, signum, name_width, signame);
    }
  अन्यथा
    {
      अगर (argv)
        क्रम (; *argv; argv++)
          {
            पूर्णांक signum = operand2sig (*argv);
            अगर (signum < 0)
              status = निकास_त्रुटि;
            अन्यथा अगर (c_isdigit (**argv))
              {
                अगर (sig2str (signum, signame) == 0)
                  माला_दो (signame);
                अन्यथा
                  म_लिखो ("%d\n", signum);
              }
            अन्यथा
              म_लिखो ("%d\n", signum);
          }
      अन्यथा
        क्रम (पूर्णांक signum = 0; signum <= SIGNUM_BOUND; signum++)
          अगर (sig2str (signum, signame) == 0)
            माला_दो (signame);
    }

  वापस status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

अटल पूर्णांक
send_signals (पूर्णांक signum, अक्षर *स्थिर *argv)
{
  पूर्णांक status = निकास_सफल;
  अक्षर स्थिर *arg = *argv;

  करो
    {
      अक्षर *endp;
      intmax_t n = (त्रुटि_सं = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      अगर (त्रुटि_सं == दुस्फल || ckd_add (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          status = निकास_त्रुटि;
        }
      अन्यथा अगर (समाप्त (pid, signum) != 0)
        {
          अगर (त्रुटि_सं == EINVAL)
            error (0, त्रुटि_सं, "%d", signum);
          अन्यथा
            error (0, त्रुटि_सं, "%s", quote (arg));
          status = निकास_त्रुटि;
        }
    }
  जबतक ((arg = *++argv));

  वापस status;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
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

  जबतक ((optc = getopt_long (argc, argv, short_options, long_options, शून्य))
         != -1)
    चयन (optc)
      {
      हाल '0': हाल '1': हाल '2': हाल '3': हाल '4':
      हाल '5': हाल '6': हाल '7': हाल '8': हाल '9':
        अगर (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            जाओ no_more_options;
          }
        FALLTHROUGH;
      हाल 'A': हाल 'B': हाल 'C': हाल 'D': हाल 'E':
      हाल 'F': हाल 'G': हाल 'H': हाल 'I': हाल 'J':
      हाल 'K': /*case 'L':*/ हाल 'M': हाल 'N': हाल 'O':
      हाल 'P': हाल 'Q': हाल 'R': हाल 'S': हाल 'T':
      हाल 'U': हाल 'V': हाल 'W': हाल 'X': हाल 'Y':
      हाल 'Z':
        अगर (! optarg)
          optarg = argv[optind - 1] + म_माप (argv[optind - 1]);
        अगर (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (निकास_त्रुटि);
          }
        optarg--;
        FALLTHROUGH;
      हाल 'n': /* -n is not documented, but is for Bash compatibility.  */
      हाल 's':
        अगर (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (निकास_त्रुटि);
          }
        signum = operand2sig (optarg);
        अगर (signum < 0)
          usage (निकास_त्रुटि);
        अवरोध;

      हाल 'L': /* -L is not documented, but is for procps compatibility.  */
      हाल 't':
        table = true;
        FALLTHROUGH;
      हाल 'l':
        अगर (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (निकास_त्रुटि);
          }
        list = true;
        अवरोध;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      शेष:
        usage (निकास_त्रुटि);
      }
 no_more_options:

  अगर (signum < 0)
    signum = संक_इति;
  अन्यथा अगर (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (निकास_त्रुटि);
    }

  अगर ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (निकास_त्रुटि);
    }

  वापस (list
          ? list_signals (table, optind < argc ? argv + optind : शून्य)
          : send_signals (signum, argv + optind));
}
