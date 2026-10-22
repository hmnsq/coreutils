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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <signal.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "sig2str.h"
#অন্তর্ভাব "operand2sig.h"
#অন্তর্ভাব "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "kill"

#define AUTHORS proper_name ("Paul Eggert")

স্থির অক্ষর ধ্রুবক short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"list", no_argument, ফাঁকা, 'l'},
  {"signal", required_argument, ফাঁকা, 's'},
  {"table", no_argument, ফাঁকা, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
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
  -s, --সংকেত=SIGNAL, -SIGNAL\n\
         specify the name or সংখ্যা of the সংকেত to be sent\n\
"));
      oputs (_("\
  -l, --list\n\
         list সংকেত names, or convert সংকেত names to/from numbers\n\
"));
      oputs (_("\
  -t, --table\n\
         print a table of সংকেত information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a সংকেত name like 'HUP', or a সংকেত সংখ্যা like '1',\n\
or the exit status of a process terminated by a সংকেত.\n\
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
print_table_row (পূর্ণসংখ্যা অঙ্ক_width, পূর্ণসংখ্যা signum,
                 পূর্ণসংখ্যা name_width, অক্ষর ধ্রুবক *signame)
{
  অক্ষর ধ্রুবক *description = strsignal (signum);
  printf ("%*d %-*s %s\n", অঙ্ক_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

স্থির পূর্ণসংখ্যা
list_signals (bool table, অক্ষর *ধ্রুবক *argv)
{
  পূর্ণসংখ্যা status = EXIT_SUCCESS;
  অক্ষর signame[SIG2STR_MAX];

  যদি (table)
    {
      পূর্ণসংখ্যা name_width = 0;

      /* Compute the maximum width of a signal number.  */
      পূর্ণসংখ্যা অঙ্ক_width = 1;
      জন্য (পূর্ণসংখ্যা signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        অঙ্ক_width++;

      /* Compute the maximum width of a signal name.  */
      জন্য (পূর্ণসংখ্যা signum = 0; signum <= SIGNUM_BOUND; signum++)
        যদি (sig2str (signum, signame) == 0)
          {
            idx_t len = strlen (signame);
            যদি (name_width < len)
              name_width = len;
          }

      যদি (argv)
        জন্য (; *argv; argv++)
          {
            পূর্ণসংখ্যা signum = operand2sig (*argv);
            যদি (signum < 0)
              status = EXIT_FAILURE;
            অন্যথা
              {
                যদি (sig2str (signum, signame) != 0)
                  snprintf (signame, আকার signame, "SIG%d", signum);
                print_table_row (অঙ্ক_width, signum, name_width, signame);
              }
          }
      অন্যথা
        জন্য (পূর্ণসংখ্যা signum = 0; signum <= SIGNUM_BOUND; signum++)
          যদি (sig2str (signum, signame) == 0)
            print_table_row (অঙ্ক_width, signum, name_width, signame);
    }
  অন্যথা
    {
      যদি (argv)
        জন্য (; *argv; argv++)
          {
            পূর্ণসংখ্যা signum = operand2sig (*argv);
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
        জন্য (পূর্ণসংখ্যা signum = 0; signum <= SIGNUM_BOUND; signum++)
          যদি (sig2str (signum, signame) == 0)
            puts (signame);
    }

  ফেরত status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

স্থির পূর্ণসংখ্যা
send_signals (পূর্ণসংখ্যা signum, অক্ষর *ধ্রুবক *argv)
{
  পূর্ণসংখ্যা status = EXIT_SUCCESS;
  অক্ষর ধ্রুবক *arg = *argv;

  করো
    {
      অক্ষর *endp;
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
  যখন ((arg = *++argv));

  ফেরত status;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা optc;
  bool list = false;
  bool table = false;
  পূর্ণসংখ্যা signum = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন ((optc = getopt_long (argc, argv, short_options, long_options, ফাঁকা))
         != -1)
    নির্বাচন (optc)
      {
      ক্ষেত্র '0': ক্ষেত্র '1': ক্ষেত্র '2': ক্ষেত্র '3': ক্ষেত্র '4':
      ক্ষেত্র '5': ক্ষেত্র '6': ক্ষেত্র '7': ক্ষেত্র '8': ক্ষেত্র '9':
        যদি (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            যাও no_more_options;
          }
        FALLTHROUGH;
      ক্ষেত্র 'A': ক্ষেত্র 'B': ক্ষেত্র 'C': ক্ষেত্র 'D': ক্ষেত্র 'E':
      ক্ষেত্র 'F': ক্ষেত্র 'G': ক্ষেত্র 'H': ক্ষেত্র 'I': ক্ষেত্র 'J':
      ক্ষেত্র 'K': /*case 'L':*/ ক্ষেত্র 'M': ক্ষেত্র 'N': ক্ষেত্র 'O':
      ক্ষেত্র 'P': ক্ষেত্র 'Q': ক্ষেত্র 'R': ক্ষেত্র 'S': ক্ষেত্র 'T':
      ক্ষেত্র 'U': ক্ষেত্র 'V': ক্ষেত্র 'W': ক্ষেত্র 'X': ক্ষেত্র 'Y':
      ক্ষেত্র 'Z':
        যদি (! optarg)
          optarg = argv[optind - 1] + strlen (argv[optind - 1]);
        যদি (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      ক্ষেত্র 'n': /* -n is not documented, but is for Bash compatibility.  */
      ক্ষেত্র 's':
        যদি (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        যদি (signum < 0)
          usage (EXIT_FAILURE);
        বিরতি;

      ক্ষেত্র 'L': /* -L is not documented, but is for procps compatibility.  */
      ক্ষেত্র 't':
        table = true;
        FALLTHROUGH;
      ক্ষেত্র 'l':
        যদি (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        বিরতি;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      পূর্বনির্ধারিত:
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

  ফেরত (list
          ? list_signals (table, optind < argc ? argv + optind : ফাঁকা)
          : send_signals (signum, argv + optind));
}
