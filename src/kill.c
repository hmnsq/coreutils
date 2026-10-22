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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <signal.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "sig2str.h"
#அந்தர்பாவ "operand2sig.h"
#அந்தர்பாவ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "kill"

#define AUTHORS proper_name ("Paul Eggert")

நிலையான எழுத்து மாறிலி short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"list", no_argument, வெற்று, 'l'},
  {"signal", required_argument, வெற்று, 's'},
  {"table", no_argument, வெற்று, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [-s SIGNAL | -SIGNAL] PID...\n\
  or:  %s -l [SIGNAL]...\n\
  or:  %s -t [SIGNAL]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Send signals to processes, or list signals.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -s, --சமிக்ஞை=SIGNAL, -SIGNAL\n\
         specify the name or எண் of the சமிக்ஞை to be sent\n\
"));
      oputs (_("\
  -l, --list\n\
         list சமிக்ஞை names, or convert சமிக்ஞை names to/from numbers\n\
"));
      oputs (_("\
  -t, --table\n\
         print a table of சமிக்ஞை information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a சமிக்ஞை name like 'HUP', or a சமிக்ஞை எண் like '1',\n\
or the exit status of a process terminated by a சமிக்ஞை.\n\
PID is an integer; எனில் negative it identifies a process group.\n\
"), stdout);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

நிலையான வெறுமை
print_table_row (முழு இலக்கம்_width, முழு signum,
                 முழு name_width, எழுத்து மாறிலி *signame)
{
  எழுத்து மாறிலி *description = strsignal (signum);
  printf ("%*d %-*s %s\n", இலக்கம்_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

நிலையான முழு
list_signals (bool table, எழுத்து *மாறிலி *argv)
{
  முழு status = EXIT_SUCCESS;
  எழுத்து signame[SIG2STR_MAX];

  எனில் (table)
    {
      முழு name_width = 0;

      /* Compute the maximum width of a signal number.  */
      முழு இலக்கம்_width = 1;
      க்கு (முழு signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        இலக்கம்_width++;

      /* Compute the maximum width of a signal name.  */
      க்கு (முழு signum = 0; signum <= SIGNUM_BOUND; signum++)
        எனில் (sig2str (signum, signame) == 0)
          {
            idx_t len = strlen (signame);
            எனில் (name_width < len)
              name_width = len;
          }

      எனில் (argv)
        க்கு (; *argv; argv++)
          {
            முழு signum = operand2sig (*argv);
            எனில் (signum < 0)
              status = EXIT_FAILURE;
            இல்லையெனில்
              {
                எனில் (sig2str (signum, signame) != 0)
                  snprintf (signame, அளவு signame, "SIG%d", signum);
                print_table_row (இலக்கம்_width, signum, name_width, signame);
              }
          }
      இல்லையெனில்
        க்கு (முழு signum = 0; signum <= SIGNUM_BOUND; signum++)
          எனில் (sig2str (signum, signame) == 0)
            print_table_row (இலக்கம்_width, signum, name_width, signame);
    }
  இல்லையெனில்
    {
      எனில் (argv)
        க்கு (; *argv; argv++)
          {
            முழு signum = operand2sig (*argv);
            எனில் (signum < 0)
              status = EXIT_FAILURE;
            இல்லையெனில் எனில் (c_isdigit (**argv))
              {
                எனில் (sig2str (signum, signame) == 0)
                  puts (signame);
                இல்லையெனில்
                  printf ("%d\n", signum);
              }
            இல்லையெனில்
              printf ("%d\n", signum);
          }
      இல்லையெனில்
        க்கு (முழு signum = 0; signum <= SIGNUM_BOUND; signum++)
          எனில் (sig2str (signum, signame) == 0)
            puts (signame);
    }

  திரும்பு status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

நிலையான முழு
send_signals (முழு signum, எழுத்து *மாறிலி *argv)
{
  முழு status = EXIT_SUCCESS;
  எழுத்து மாறிலி *arg = *argv;

  செய்
    {
      எழுத்து *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      எனில் (errno == ERANGE || ckd_add (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          status = EXIT_FAILURE;
        }
      இல்லையெனில் எனில் (kill (pid, signum) != 0)
        {
          எனில் (errno == EINVAL)
            error (0, errno, "%d", signum);
          இல்லையெனில்
            error (0, errno, "%s", quote (arg));
          status = EXIT_FAILURE;
        }
    }
  போது ((arg = *++argv));

  திரும்பு status;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு optc;
  bool list = false;
  bool table = false;
  முழு signum = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((optc = getopt_long (argc, argv, short_options, long_options, வெற்று))
         != -1)
    தேர்வு (optc)
      {
      நிலை '0': நிலை '1': நிலை '2': நிலை '3': நிலை '4':
      நிலை '5': நிலை '6': நிலை '7': நிலை '8': நிலை '9':
        எனில் (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            செல் no_more_options;
          }
        FALLTHROUGH;
      நிலை 'A': நிலை 'B': நிலை 'C': நிலை 'D': நிலை 'E':
      நிலை 'F': நிலை 'G': நிலை 'H': நிலை 'I': நிலை 'J':
      நிலை 'K': /*case 'L':*/ நிலை 'M': நிலை 'N': நிலை 'O':
      நிலை 'P': நிலை 'Q': நிலை 'R': நிலை 'S': நிலை 'T':
      நிலை 'U': நிலை 'V': நிலை 'W': நிலை 'X': நிலை 'Y':
      நிலை 'Z':
        எனில் (! optarg)
          optarg = argv[optind - 1] + strlen (argv[optind - 1]);
        எனில் (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      நிலை 'n': /* -n is not documented, but is for Bash compatibility.  */
      நிலை 's':
        எனில் (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        எனில் (signum < 0)
          usage (EXIT_FAILURE);
        நிறுத்து;

      நிலை 'L': /* -L is not documented, but is for procps compatibility.  */
      நிலை 't':
        table = true;
        FALLTHROUGH;
      நிலை 'l':
        எனில் (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        நிறுத்து;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      இயல்பு:
        usage (EXIT_FAILURE);
      }
 no_more_options:

  எனில் (signum < 0)
    signum = SIGTERM;
  இல்லையெனில் எனில் (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  எனில் ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  திரும்பு (list
          ? list_signals (table, optind < argc ? argv + optind : வெற்று)
          : send_signals (signum, argv + optind));
}
