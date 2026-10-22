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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "sig2str.h"
#ଅନ୍ତର୍ଭାଵ "operand2sig.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "kill"

#define AUTHORS proper_name ("Paul Eggert")

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"list", no_argument, ଖାଲି, 'l'},
  {"signal", required_argument, ଖାଲି, 's'},
  {"table", no_argument, ଖାଲି, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
  -s, --ସଙ୍କେତ=SIGNAL, -SIGNAL\n\
         specify the name or ସଂଖ୍ୟା of the ସଙ୍କେତ to be sent\n\
"));
      oputs (_("\
  -l, --list\n\
         list ସଙ୍କେତ names, or convert ସଙ୍କେତ names to/from numbers\n\
"));
      oputs (_("\
  -t, --table\n\
         print a table of ସଙ୍କେତ information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a ସଙ୍କେତ name like 'HUP', or a ସଙ୍କେତ ସଂଖ୍ୟା like '1',\n\
or the exit status of a process terminated by a ସଙ୍କେତ.\n\
PID is an integer; ଯଦି negative it identifies a process group.\n\
"), stdout);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

ସ୍ଥିର ଶୂନ୍ୟ
print_table_row (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା signum,
                 ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_width, ଅକ୍ଷର ଅଚଳ *signame)
{
  ଅକ୍ଷର ଅଚଳ *description = strsignal (signum);
  printf ("%*d %-*s %s\n", ଅଙ୍କ_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
list_signals (bool table, ଅକ୍ଷର *ଅଚଳ *argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = EXIT_SUCCESS;
  ଅକ୍ଷର signame[SIG2STR_MAX];

  ଯଦି (table)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା name_width = 0;

      /* Compute the maximum width of a signal number.  */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_width = 1;
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        ଅଙ୍କ_width++;

      /* Compute the maximum width of a signal name.  */
      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା signum = 0; signum <= SIGNUM_BOUND; signum++)
        ଯଦି (sig2str (signum, signame) == 0)
          {
            idx_t len = strlen (signame);
            ଯଦି (name_width < len)
              name_width = len;
          }

      ଯଦି (argv)
        ପାଇଁ (; *argv; argv++)
          {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା signum = operand2sig (*argv);
            ଯଦି (signum < 0)
              status = EXIT_FAILURE;
            ଅନ୍ୟଥା
              {
                ଯଦି (sig2str (signum, signame) != 0)
                  snprintf (signame, ଆକାର signame, "SIG%d", signum);
                print_table_row (ଅଙ୍କ_width, signum, name_width, signame);
              }
          }
      ଅନ୍ୟଥା
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା signum = 0; signum <= SIGNUM_BOUND; signum++)
          ଯଦି (sig2str (signum, signame) == 0)
            print_table_row (ଅଙ୍କ_width, signum, name_width, signame);
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (argv)
        ପାଇଁ (; *argv; argv++)
          {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା signum = operand2sig (*argv);
            ଯଦି (signum < 0)
              status = EXIT_FAILURE;
            ଅନ୍ୟଥା ଯଦି (c_isdigit (**argv))
              {
                ଯଦି (sig2str (signum, signame) == 0)
                  puts (signame);
                ଅନ୍ୟଥା
                  printf ("%d\n", signum);
              }
            ଅନ୍ୟଥା
              printf ("%d\n", signum);
          }
      ଅନ୍ୟଥା
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା signum = 0; signum <= SIGNUM_BOUND; signum++)
          ଯଦି (sig2str (signum, signame) == 0)
            puts (signame);
    }

  ଫେରନ୍ତୁ status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
send_signals (ପୂର୍ଣ୍ଣସଂଖ୍ୟା signum, ଅକ୍ଷର *ଅଚଳ *argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = EXIT_SUCCESS;
  ଅକ୍ଷର ଅଚଳ *arg = *argv;

  କର
    {
      ଅକ୍ଷର *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      ଯଦି (errno == ERANGE || ckd_add (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          status = EXIT_FAILURE;
        }
      ଅନ୍ୟଥା ଯଦି (kill (pid, signum) != 0)
        {
          ଯଦି (errno == EINVAL)
            error (0, errno, "%d", signum);
          ଅନ୍ୟଥା
            error (0, errno, "%s", quote (arg));
          status = EXIT_FAILURE;
        }
    }
  ଯେତେବେଳେ ((arg = *++argv));

  ଫେରନ୍ତୁ status;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  bool list = false;
  bool table = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା signum = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, short_options, long_options, ଖାଲି))
         != -1)
    ବାଛ (optc)
      {
      ସ୍ଥିତି '0': ସ୍ଥିତି '1': ସ୍ଥିତି '2': ସ୍ଥିତି '3': ସ୍ଥିତି '4':
      ସ୍ଥିତି '5': ସ୍ଥିତି '6': ସ୍ଥିତି '7': ସ୍ଥିତି '8': ସ୍ଥିତି '9':
        ଯଦି (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            ଯାଅ no_more_options;
          }
        FALLTHROUGH;
      ସ୍ଥିତି 'A': ସ୍ଥିତି 'B': ସ୍ଥିତି 'C': ସ୍ଥିତି 'D': ସ୍ଥିତି 'E':
      ସ୍ଥିତି 'F': ସ୍ଥିତି 'G': ସ୍ଥିତି 'H': ସ୍ଥିତି 'I': ସ୍ଥିତି 'J':
      ସ୍ଥିତି 'K': /*case 'L':*/ ସ୍ଥିତି 'M': ସ୍ଥିତି 'N': ସ୍ଥିତି 'O':
      ସ୍ଥିତି 'P': ସ୍ଥିତି 'Q': ସ୍ଥିତି 'R': ସ୍ଥିତି 'S': ସ୍ଥିତି 'T':
      ସ୍ଥିତି 'U': ସ୍ଥିତି 'V': ସ୍ଥିତି 'W': ସ୍ଥିତି 'X': ସ୍ଥିତି 'Y':
      ସ୍ଥିତି 'Z':
        ଯଦି (! optarg)
          optarg = argv[optind - 1] + strlen (argv[optind - 1]);
        ଯଦି (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      ସ୍ଥିତି 'n': /* -n is not documented, but is for Bash compatibility.  */
      ସ୍ଥିତି 's':
        ଯଦି (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        ଯଦି (signum < 0)
          usage (EXIT_FAILURE);
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'L': /* -L is not documented, but is for procps compatibility.  */
      ସ୍ଥିତି 't':
        table = true;
        FALLTHROUGH;
      ସ୍ଥିତି 'l':
        ଯଦି (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        ଭାଙ୍ଗ;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        usage (EXIT_FAILURE);
      }
 no_more_options:

  ଯଦି (signum < 0)
    signum = SIGTERM;
  ଅନ୍ୟଥା ଯଦି (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  ଯଦି ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  ଫେରନ୍ତୁ (list
          ? list_signals (table, optind < argc ? argv + optind : ଖାଲି)
          : send_signals (signum, argv + optind));
}
