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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <signal.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "c-ctype.h"
#অন্তর্ভাৱ "sig2str.h"
#অন্তর্ভাৱ "operand2sig.h"
#অন্তর্ভাৱ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "kill"

#define AUTHORS proper_name ("Paul Eggert")

স্থির ৱর্ণ ধ্রুৱ short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

স্থির সংরচনা option ধ্রুৱ long_options[] =
{
  {"list", no_argument, রিক্ত, 'l'},
  {"signal", required_argument, রিক্ত, 's'},
  {"table", no_argument, রিক্ত, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
  -s, --সঙ্কেত=SIGNAL, -SIGNAL\n\
         specify the name or সংখ্যা of the সঙ্কেত to be sent\n\
"));
      oputs (_("\
  -l, --list\n\
         list সঙ্কেত names, or convert সঙ্কেত names to/from numbers\n\
"));
      oputs (_("\
  -t, --table\n\
         print a table of সঙ্কেত information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a সঙ্কেত name like 'HUP', or a সঙ্কেত সংখ্যা like '1',\n\
or the exit status of a process terminated by a সঙ্কেত.\n\
PID is an integer; যদি negative it identifies a process group.\n\
"), stdout);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

স্থির শূন্য
print_table_row (পূর্ণাঙ্ক অঙ্ক_width, পূর্ণাঙ্ক signum,
                 পূর্ণাঙ্ক name_width, ৱর্ণ ধ্রুৱ *signame)
{
  ৱর্ণ ধ্রুৱ *description = strsignal (signum);
  printf ("%*d %-*s %s\n", অঙ্ক_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

স্থির পূর্ণাঙ্ক
list_signals (bool table, ৱর্ণ *ধ্রুৱ *argv)
{
  পূর্ণাঙ্ক status = EXIT_SUCCESS;
  ৱর্ণ signame[SIG2STR_MAX];

  যদি (table)
    {
      পূর্ণাঙ্ক name_width = 0;

      /* Compute the maximum width of a signal number.  */
      পূর্ণাঙ্ক অঙ্ক_width = 1;
      বাবে (পূর্ণাঙ্ক signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        অঙ্ক_width++;

      /* Compute the maximum width of a signal name.  */
      বাবে (পূর্ণাঙ্ক signum = 0; signum <= SIGNUM_BOUND; signum++)
        যদি (sig2str (signum, signame) == 0)
          {
            idx_t len = strlen (signame);
            যদি (name_width < len)
              name_width = len;
          }

      যদি (argv)
        বাবে (; *argv; argv++)
          {
            পূর্ণাঙ্ক signum = operand2sig (*argv);
            যদি (signum < 0)
              status = EXIT_FAILURE;
            অন্যথা
              {
                যদি (sig2str (signum, signame) != 0)
                  snprintf (signame, পরিমাণ signame, "SIG%d", signum);
                print_table_row (অঙ্ক_width, signum, name_width, signame);
              }
          }
      অন্যথা
        বাবে (পূর্ণাঙ্ক signum = 0; signum <= SIGNUM_BOUND; signum++)
          যদি (sig2str (signum, signame) == 0)
            print_table_row (অঙ্ক_width, signum, name_width, signame);
    }
  অন্যথা
    {
      যদি (argv)
        বাবে (; *argv; argv++)
          {
            পূর্ণাঙ্ক signum = operand2sig (*argv);
            যদি (signum < 0)
              status = EXIT_FAILURE;
            অন্যথা যদি (c_isdigit (**argv))
              {
                যদি (sig2str (signum, signame) == 0)
                  puts (signame);
                অন্যথা
                  printf ("%d\n", signum);
              }
            অন্যথা
              printf ("%d\n", signum);
          }
      অন্যথা
        বাবে (পূর্ণাঙ্ক signum = 0; signum <= SIGNUM_BOUND; signum++)
          যদি (sig2str (signum, signame) == 0)
            puts (signame);
    }

  ঘূৰাই status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

স্থির পূর্ণাঙ্ক
send_signals (পূর্ণাঙ্ক signum, ৱর্ণ *ধ্রুৱ *argv)
{
  পূর্ণাঙ্ক status = EXIT_SUCCESS;
  ৱর্ণ ধ্রুৱ *arg = *argv;

  কৰা
    {
      ৱর্ণ *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      যদি (errno == ERANGE || ckd_add (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          status = EXIT_FAILURE;
        }
      অন্যথা যদি (kill (pid, signum) != 0)
        {
          যদি (errno == EINVAL)
            error (0, errno, "%d", signum);
          অন্যথা
            error (0, errno, "%s", quote (arg));
          status = EXIT_FAILURE;
        }
    }
  যেতিয়ালৈ ((arg = *++argv));

  ঘূৰাই status;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  পূর্ণাঙ্ক optc;
  bool list = false;
  bool table = false;
  পূর্ণাঙ্ক signum = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ ((optc = getopt_long (argc, argv, short_options, long_options, রিক্ত))
         != -1)
    ৱিকল্প (optc)
      {
      পক্ষ '0': পক্ষ '1': পক্ষ '2': পক্ষ '3': পক্ষ '4':
      পক্ষ '5': পক্ষ '6': পক্ষ '7': পক্ষ '8': পক্ষ '9':
        যদি (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            গচ্ছ no_more_options;
          }
        FALLTHROUGH;
      পক্ষ 'A': পক্ষ 'B': পক্ষ 'C': পক্ষ 'D': পক্ষ 'E':
      পক্ষ 'F': পক্ষ 'G': পক্ষ 'H': পক্ষ 'I': পক্ষ 'J':
      পক্ষ 'K': /*case 'L':*/ পক্ষ 'M': পক্ষ 'N': পক্ষ 'O':
      পক্ষ 'P': পক্ষ 'Q': পক্ষ 'R': পক্ষ 'S': পক্ষ 'T':
      পক্ষ 'U': পক্ষ 'V': পক্ষ 'W': পক্ষ 'X': পক্ষ 'Y':
      পক্ষ 'Z':
        যদি (! optarg)
          optarg = argv[optind - 1] + strlen (argv[optind - 1]);
        যদি (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      পক্ষ 'n': /* -n is not documented, but is for Bash compatibility.  */
      পক্ষ 's':
        যদি (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        যদি (signum < 0)
          usage (EXIT_FAILURE);
        ভঙ্গ;

      পক্ষ 'L': /* -L is not documented, but is for procps compatibility.  */
      পক্ষ 't':
        table = true;
        FALLTHROUGH;
      পক্ষ 'l':
        যদি (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        ভঙ্গ;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      স্ৱভাৱ:
        usage (EXIT_FAILURE);
      }
 no_more_options:

  যদি (signum < 0)
    signum = SIGTERM;
  অন্যথা যদি (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  যদি ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  ঘূৰাই (list
          ? list_signals (table, optind < argc ? argv + optind : রিক্ত)
          : send_signals (signum, argv + optind));
}
