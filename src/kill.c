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

स्थिर वर्ण ध्रुव short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

स्थिर संरचना option ध्रुव long_options[] =
{
  {"list", no_argument, रिक्त, 'l'},
  {"signal", required_argument, रिक्त, 's'},
  {"table", no_argument, रिक्त, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
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
  -s, --सङ्केत=SIGNAL, -SIGNAL\n\
         specify the name or संख्या of the सङ्केत to be sent\n\
"));
      oputs (_("\
  -l, --list\n\
         list सङ्केत names, or convert सङ्केत names to/from numbers\n\
"));
      oputs (_("\
  -t, --table\n\
         print a table of सङ्केत information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\n\
SIGNAL may be a सङ्केत name like 'HUP', or a सङ्केत संख्या like '1',\n\
or the निकास status of a process terminated by a सङ्केत.\n\
PID is an integer; यदि negative it identifies a process group.\n\
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
print_table_row (पूर्णाङ्क अंक_width, पूर्णाङ्क signum,
                 पूर्णाङ्क name_width, वर्ण ध्रुव *signame)
{
  वर्ण ध्रुव *description = strsignal (signum);
  म_लिखो ("%*d %-*s %s\n", अंक_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

स्थिर पूर्णाङ्क
list_signals (bool table, वर्ण *ध्रुव *argv)
{
  पूर्णाङ्क status = निकास_सफल;
  वर्ण signame[SIG2STR_MAX];

  यदि (table)
    {
      पूर्णाङ्क name_width = 0;

      /* Compute the maximum width of a signal number.  */
      पूर्णाङ्क अंक_width = 1;
      प्रति (पूर्णाङ्क signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        अंक_width++;

      /* Compute the maximum width of a signal name.  */
      प्रति (पूर्णाङ्क signum = 0; signum <= SIGNUM_BOUND; signum++)
        यदि (sig2str (signum, signame) == 0)
          {
            idx_t len = म_माप (signame);
            यदि (name_width < len)
              name_width = len;
          }

      यदि (argv)
        प्रति (; *argv; argv++)
          {
            पूर्णाङ्क signum = operand2sig (*argv);
            यदि (signum < 0)
              status = निकास_त्रुटि;
            अन्यथा
              {
                यदि (sig2str (signum, signame) != 0)
                  snprintf (signame, परिमाण signame, "SIG%d", signum);
                print_table_row (अंक_width, signum, name_width, signame);
              }
          }
      अन्यथा
        प्रति (पूर्णाङ्क signum = 0; signum <= SIGNUM_BOUND; signum++)
          यदि (sig2str (signum, signame) == 0)
            print_table_row (अंक_width, signum, name_width, signame);
    }
  अन्यथा
    {
      यदि (argv)
        प्रति (; *argv; argv++)
          {
            पूर्णाङ्क signum = operand2sig (*argv);
            यदि (signum < 0)
              status = निकास_त्रुटि;
            अन्यथा यदि (c_isdigit (**argv))
              {
                यदि (sig2str (signum, signame) == 0)
                  माला_दो (signame);
                अन्यथा
                  म_लिखो ("%d\n", signum);
              }
            अन्यथा
              म_लिखो ("%d\n", signum);
          }
      अन्यथा
        प्रति (पूर्णाङ्क signum = 0; signum <= SIGNUM_BOUND; signum++)
          यदि (sig2str (signum, signame) == 0)
            माला_दो (signame);
    }

  प्रत्यागम status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

स्थिर पूर्णाङ्क
send_signals (पूर्णाङ्क signum, वर्ण *ध्रुव *argv)
{
  पूर्णाङ्क status = निकास_सफल;
  वर्ण ध्रुव *arg = *argv;

  कुरु
    {
      वर्ण *endp;
      intmax_t n = (त्रुटि_सं = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      यदि (त्रुटि_सं == दुस्फल || ckd_add (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          status = निकास_त्रुटि;
        }
      अन्यथा यदि (समाप्त (pid, signum) != 0)
        {
          यदि (त्रुटि_सं == EINVAL)
            error (0, त्रुटि_सं, "%d", signum);
          अन्यथा
            error (0, त्रुटि_सं, "%s", quote (arg));
          status = निकास_त्रुटि;
        }
    }
  यावत् ((arg = *++argv));

  प्रत्यागम status;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc;
  bool list = false;
  bool table = false;
  पूर्णाङ्क signum = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  यावत् ((optc = getopt_long (argc, argv, short_options, long_options, रिक्त))
         != -1)
    विकल्प (optc)
      {
      पक्ष '0': पक्ष '1': पक्ष '2': पक्ष '3': पक्ष '4':
      पक्ष '5': पक्ष '6': पक्ष '7': पक्ष '8': पक्ष '9':
        यदि (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            गच्छ no_more_options;
          }
        FALLTHROUGH;
      पक्ष 'A': पक्ष 'B': पक्ष 'C': पक्ष 'D': पक्ष 'E':
      पक्ष 'F': पक्ष 'G': पक्ष 'H': पक्ष 'I': पक्ष 'J':
      पक्ष 'K': /*case 'L':*/ पक्ष 'M': पक्ष 'N': पक्ष 'O':
      पक्ष 'P': पक्ष 'Q': पक्ष 'R': पक्ष 'S': पक्ष 'T':
      पक्ष 'U': पक्ष 'V': पक्ष 'W': पक्ष 'X': पक्ष 'Y':
      पक्ष 'Z':
        यदि (! optarg)
          optarg = argv[optind - 1] + म_माप (argv[optind - 1]);
        यदि (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (निकास_त्रुटि);
          }
        optarg--;
        FALLTHROUGH;
      पक्ष 'n': /* -n is not documented, but is for Bash compatibility.  */
      पक्ष 's':
        यदि (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (निकास_त्रुटि);
          }
        signum = operand2sig (optarg);
        यदि (signum < 0)
          usage (निकास_त्रुटि);
        भङ्ग;

      पक्ष 'L': /* -L is not documented, but is for procps compatibility.  */
      पक्ष 't':
        table = true;
        FALLTHROUGH;
      पक्ष 'l':
        यदि (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (निकास_त्रुटि);
          }
        list = true;
        भङ्ग;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      स्वभाव:
        usage (निकास_त्रुटि);
      }
 no_more_options:

  यदि (signum < 0)
    signum = संक_इति;
  अन्यथा यदि (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (निकास_त्रुटि);
    }

  यदि ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (निकास_त्रुटि);
    }

  प्रत्यागम (list
          ? list_signals (table, optind < argc ? argv + optind : रिक्त)
          : send_signals (signum, argv + optind));
}
