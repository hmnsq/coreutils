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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل <signal.h>

#شامل "system.h"
#شامل "c-ctype.h"
#شامل "sig2str.h"
#شامل "operand2sig.h"
#شامل "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "kill"

#define AUTHORS proper_name ("Paul Eggert")

ایستا نویسه ثابت short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

ایستا ساختار option ثابت long_options[] =
{
  {"list", no_argument, پوچ, 'l'},
  {"signal", required_argument, پوچ, 's'},
  {"table", no_argument, پوچ, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
  -s, --سیگنال=SIGNAL, -SIGNAL\n\
         specify the name or عدد of the سیگنال to be sent\n\
"));
      oputs (_("\
  -l, --list\n\
         list سیگنال names, or convert سیگنال names to/from numbers\n\
"));
      oputs (_("\
  -t, --table\n\
         print a table of سیگنال information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a سیگنال name like 'HUP', or a سیگنال عدد like '1',\n\
or the exit status of a process terminated by a سیگنال.\n\
PID is an integer; اگر negative it identifies a process group.\n\
"), stdout);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

ایستا تهی
print_table_row (صحیح رقم_width, صحیح signum,
                 صحیح name_width, نویسه ثابت *signame)
{
  نویسه ثابت *description = strsignal (signum);
  printf ("%*d %-*s %s\n", رقم_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

ایستا صحیح
list_signals (bool table, نویسه *ثابت *argv)
{
  صحیح status = EXIT_SUCCESS;
  نویسه signame[SIG2STR_MAX];

  اگر (table)
    {
      صحیح name_width = 0;

      /* Compute the maximum width of a signal number.  */
      صحیح رقم_width = 1;
      برای (صحیح signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        رقم_width++;

      /* Compute the maximum width of a signal name.  */
      برای (صحیح signum = 0; signum <= SIGNUM_BOUND; signum++)
        اگر (sig2str (signum, signame) == 0)
          {
            idx_t len = strlen (signame);
            اگر (name_width < len)
              name_width = len;
          }

      اگر (argv)
        برای (; *argv; argv++)
          {
            صحیح signum = operand2sig (*argv);
            اگر (signum < 0)
              status = EXIT_FAILURE;
            وگرنه
              {
                اگر (sig2str (signum, signame) != 0)
                  snprintf (signame, اندازه signame, "SIG%d", signum);
                print_table_row (رقم_width, signum, name_width, signame);
              }
          }
      وگرنه
        برای (صحیح signum = 0; signum <= SIGNUM_BOUND; signum++)
          اگر (sig2str (signum, signame) == 0)
            print_table_row (رقم_width, signum, name_width, signame);
    }
  وگرنه
    {
      اگر (argv)
        برای (; *argv; argv++)
          {
            صحیح signum = operand2sig (*argv);
            اگر (signum < 0)
              status = EXIT_FAILURE;
            وگرنه اگر (c_isdigit (**argv))
              {
                اگر (sig2str (signum, signame) == 0)
                  puts (signame);
                وگرنه
                  printf ("%d\n", signum);
              }
            وگرنه
              printf ("%d\n", signum);
          }
      وگرنه
        برای (صحیح signum = 0; signum <= SIGNUM_BOUND; signum++)
          اگر (sig2str (signum, signame) == 0)
            puts (signame);
    }

  بازگشت status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

ایستا صحیح
send_signals (صحیح signum, نویسه *ثابت *argv)
{
  صحیح status = EXIT_SUCCESS;
  نویسه ثابت *arg = *argv;

  انجامبده
    {
      نویسه *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      اگر (errno == ERANGE || ckd_add (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          status = EXIT_FAILURE;
        }
      وگرنه اگر (kill (pid, signum) != 0)
        {
          اگر (errno == EINVAL)
            error (0, errno, "%d", signum);
          وگرنه
            error (0, errno, "%s", quote (arg));
          status = EXIT_FAILURE;
        }
    }
  وقتی ((arg = *++argv));

  بازگشت status;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح optc;
  bool list = false;
  bool table = false;
  صحیح signum = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی ((optc = getopt_long (argc, argv, short_options, long_options, پوچ))
         != -1)
    انتخاب (optc)
      {
      حالت '0': حالت '1': حالت '2': حالت '3': حالت '4':
      حالت '5': حالت '6': حالت '7': حالت '8': حالت '9':
        اگر (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            برو no_more_options;
          }
        FALLTHROUGH;
      حالت 'A': حالت 'B': حالت 'C': حالت 'D': حالت 'E':
      حالت 'F': حالت 'G': حالت 'H': حالت 'I': حالت 'J':
      حالت 'K': /*case 'L':*/ حالت 'M': حالت 'N': حالت 'O':
      حالت 'P': حالت 'Q': حالت 'R': حالت 'S': حالت 'T':
      حالت 'U': حالت 'V': حالت 'W': حالت 'X': حالت 'Y':
      حالت 'Z':
        اگر (! optarg)
          optarg = argv[optind - 1] + strlen (argv[optind - 1]);
        اگر (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      حالت 'n': /* -n is not documented, but is for Bash compatibility.  */
      حالت 's':
        اگر (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        اگر (signum < 0)
          usage (EXIT_FAILURE);
        بشکن;

      حالت 'L': /* -L is not documented, but is for procps compatibility.  */
      حالت 't':
        table = true;
        FALLTHROUGH;
      حالت 'l':
        اگر (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        بشکن;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      پیشفرض:
        usage (EXIT_FAILURE);
      }
 no_more_options:

  اگر (signum < 0)
    signum = SIGTERM;
  وگرنه اگر (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  اگر ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  بازگشت (list
          ? list_signals (table, optind < argc ? argv + optind : پوچ)
          : send_signals (signum, argv + optind));
}
