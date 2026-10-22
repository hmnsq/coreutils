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
#परिभाषय क्रमादेशः_NAME "kill"

#परिभाषय AUTHORS proper_name ("Paul Eggert")

स्थिर वर्ण ध्रुव short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

स्थिर संरचना option ध्रुव long_options[] =
{
  {"list", no_तर्कः, रिक्त, 'l'},
  {"signal", required_तर्कः, रिक्त, 's'},
  {"table", no_तर्कः, रिक्त, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [-s SIGNAL | -SIGNAL] PID...\n\
  or:  %s -l [SIGNAL]...\n\
  or:  %s -t [SIGNAL]...\n\
"),
              क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name);
      fputs (_("\
Send सङ्केताः गन्तव्यम् processes, or list सङ्केताः.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -s, --सङ्केत=SIGNAL, -SIGNAL\n\
         specify the name or सङ्ख्या सम्बन्धि the सङ्केत गन्तव्यम् be प्रेषितम्\n\
"));
      oputs (_("\
  -l, --list\n\
         list सङ्केत नामानि, or convert सङ्केत नामानि गन्तव्यम्/from numbers\n\
"));
      oputs (_("\
  -t, --सारणी\n\
         print a सारणी सम्बन्धि सङ्केत information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a सङ्केत name like 'HUP', or a सङ्केत सङ्ख्या like '1',\n\
or the निर्गच्छ स्थितिः सम्बन्धि a process terminated by a सङ्केत.\n\
PID is an integer; यदि negative it identifies a process गणः.\n\
"), stdout);
      प्रारूप_लिख (USAGE_BUILTIN_WARNING, क्रमादेशः_NAME);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

स्थिर शून्य
print_सारणी_row (पूर्णाङ्क अङ्कः_विस्तारः, पूर्णाङ्क signum,
                 पूर्णाङ्क name_विस्तारः, वर्ण ध्रुव *signame)
{
  वर्ण ध्रुव *description = strsignal (signum);
  प्रारूप_लिख ("%*d %-*s %s\n", अङ्कः_विस्तारः, signum, name_विस्तारः, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

स्थिर पूर्णाङ्क
list_सङ्केताः (bool सारणी, वर्ण *ध्रुव *argv)
{
  पूर्णाङ्क स्थितिः = EXIT_SUCCESS;
  वर्ण signame[SIG2STR_MAX];

  यदि (सारणी)
    {
      पूर्णाङ्क name_विस्तारः = 0;

      /* Compute the maximum width of a signal number.  */
      पूर्णाङ्क अङ्कः_विस्तारः = 1;
      प्रति (पूर्णाङ्क signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        अङ्कः_विस्तारः++;

      /* Compute the maximum width of a signal name.  */
      प्रति (पूर्णाङ्क signum = 0; signum <= SIGNUM_BOUND; signum++)
        यदि (sig2str (signum, signame) == 0)
          {
            idx_t len = वाक्यदैर्घ्यम् (signame);
            यदि (name_विस्तारः < len)
              name_विस्तारः = len;
          }

      यदि (argv)
        प्रति (; *argv; argv++)
          {
            पूर्णाङ्क signum = operand2sig (*argv);
            यदि (signum < 0)
              स्थितिः = EXIT_FAILURE;
            अन्यथा
              {
                यदि (sig2str (signum, signame) != 0)
                  snprintf (signame, परिमाण signame, "SIG%d", signum);
                print_सारणी_row (अङ्कः_विस्तारः, signum, name_विस्तारः, signame);
              }
          }
      अन्यथा
        प्रति (पूर्णाङ्क signum = 0; signum <= SIGNUM_BOUND; signum++)
          यदि (sig2str (signum, signame) == 0)
            print_सारणी_row (अङ्कः_विस्तारः, signum, name_विस्तारः, signame);
    }
  अन्यथा
    {
      यदि (argv)
        प्रति (; *argv; argv++)
          {
            पूर्णाङ्क signum = operand2sig (*argv);
            यदि (signum < 0)
              स्थितिः = EXIT_FAILURE;
            अन्यथा यदि (c_isdigit (**argv))
              {
                यदि (sig2str (signum, signame) == 0)
                  पङ्क्तिं_लिख (signame);
                अन्यथा
                  प्रारूप_लिख ("%d\n", signum);
              }
            अन्यथा
              प्रारूप_लिख ("%d\n", signum);
          }
      अन्यथा
        प्रति (पूर्णाङ्क signum = 0; signum <= SIGNUM_BOUND; signum++)
          यदि (sig2str (signum, signame) == 0)
            पङ्क्तिं_लिख (signame);
    }

  प्रत्यागम स्थितिः;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

स्थिर पूर्णाङ्क
send_सङ्केताः (पूर्णाङ्क signum, वर्ण *ध्रुव *argv)
{
  पूर्णाङ्क स्थितिः = EXIT_SUCCESS;
  वर्ण ध्रुव *arg = *argv;

  कुरु
    {
      वर्ण *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      यदि (errno == ERANGE || ckd_सङ्कलय (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          स्थितिः = EXIT_FAILURE;
        }
      अन्यथा यदि (kill (pid, signum) != 0)
        {
          यदि (errno == EINVAL)
            error (0, errno, "%d", signum);
          अन्यथा
            error (0, errno, "%s", quote (arg));
          स्थितिः = EXIT_FAILURE;
        }
    }
  यावत् ((arg = *++argv));

  प्रत्यागम स्थितिः;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc;
  bool list = false;
  bool सारणी = false;
  पूर्णाङ्क signum = -1;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

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
            गच्छ no_अधिकम्_options;
          }
        FALLTHROUGH;
      पक्ष 'A': पक्ष 'B': पक्ष 'C': पक्ष 'D': पक्ष 'E':
      पक्ष 'F': पक्ष 'G': पक्ष 'H': पक्ष 'I': पक्ष 'J':
      पक्ष 'K': /*case 'L':*/ पक्ष 'M': पक्ष 'N': पक्ष 'O':
      पक्ष 'P': पक्ष 'Q': पक्ष 'R': पक्ष 'S': पक्ष 'T':
      पक्ष 'U': पक्ष 'V': पक्ष 'W': पक्ष 'X': पक्ष 'Y':
      पक्ष 'Z':
        यदि (! optarg)
          optarg = argv[optind - 1] + वाक्यदैर्घ्यम् (argv[optind - 1]);
        यदि (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      पक्ष 'n': /* -n is not documented, but is for Bash compatibility.  */
      पक्ष 's':
        यदि (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        यदि (signum < 0)
          usage (EXIT_FAILURE);
        भङ्ग;

      पक्ष 'L': /* -L is not documented, but is for procps compatibility.  */
      पक्ष 't':
        सारणी = true;
        FALLTHROUGH;
      पक्ष 'l':
        यदि (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        भङ्ग;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
      स्वभाव:
        usage (EXIT_FAILURE);
      }
 no_अधिकम्_options:

  यदि (signum < 0)
    signum = SIGTERM;
  अन्यथा यदि (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  यदि ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  प्रत्यागम (list
          ? list_सङ्केताः (सारणी, optind < argc ? argv + optind : रिक्त)
          : send_सङ्केताः (signum, argv + optind));
}
