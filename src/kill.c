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

#include <config.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <signal.h>

#include "system.h"
#include "c-ctype.h"
#include "sig2str.h"
#include "operand2sig.h"
#include "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "kill"

#define AUTHORS proper_name ("Paul Eggert")

static char const short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

static struct option const long_options[] =
{
  {"list", no_argument, NULL, 'l'},
  {"signal", required_argument, NULL, 's'},
  {"table", no_argument, NULL, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (số_nguyên status)
{
  nếu (status != EXIT_SUCCESS)
    emit_try_help ();
  không_thì
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
  -s, --signal=SIGNAL, -SIGNAL\n\
         specify the name or number of the signal to be sent\n\
"));
      oputs (_("\
  -l, --list\n\
         list signal names, or convert signal names to/from numbers\n\
"));
      oputs (_("\
  -t, --table\n\
         print a table of signal information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a signal name like 'HUP', or a signal number like '1',\n\
or the exit status of a process terminated by a signal.\n\
PID is an integer; nếu negative it identifies a process group.\n\
"), stdout);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

static void
print_table_row (số_nguyên num_width, số_nguyên signum,
                 số_nguyên name_width, char const *signame)
{
  char const *description = strsignal (signum);
  printf ("%*d %-*s %s\n", num_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

static số_nguyên
list_signals (bool table, char *const *argv)
{
  số_nguyên status = EXIT_SUCCESS;
  char signame[SIG2STR_MAX];

  nếu (table)
    {
      số_nguyên name_width = 0;

      /* Compute the maximum width of a signal number.  */
      số_nguyên num_width = 1;
      cho (số_nguyên signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        num_width++;

      /* Compute the maximum width of a signal name.  */
      cho (số_nguyên signum = 0; signum <= SIGNUM_BOUND; signum++)
        nếu (sig2str (signum, signame) == 0)
          {
            idx_t len = strlen (signame);
            nếu (name_width < len)
              name_width = len;
          }

      nếu (argv)
        cho (; *argv; argv++)
          {
            số_nguyên signum = operand2sig (*argv);
            nếu (signum < 0)
              status = EXIT_FAILURE;
            không_thì
              {
                nếu (sig2str (signum, signame) != 0)
                  snprintf (signame, sizeof signame, "SIG%d", signum);
                print_table_row (num_width, signum, name_width, signame);
              }
          }
      không_thì
        cho (số_nguyên signum = 0; signum <= SIGNUM_BOUND; signum++)
          nếu (sig2str (signum, signame) == 0)
            print_table_row (num_width, signum, name_width, signame);
    }
  không_thì
    {
      nếu (argv)
        cho (; *argv; argv++)
          {
            số_nguyên signum = operand2sig (*argv);
            nếu (signum < 0)
              status = EXIT_FAILURE;
            không_thì nếu (c_isdigit (**argv))
              {
                nếu (sig2str (signum, signame) == 0)
                  puts (signame);
                không_thì
                  printf ("%d\n", signum);
              }
            không_thì
              printf ("%d\n", signum);
          }
      không_thì
        cho (số_nguyên signum = 0; signum <= SIGNUM_BOUND; signum++)
          nếu (sig2str (signum, signame) == 0)
            puts (signame);
    }

  trả_về status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

static số_nguyên
send_signals (số_nguyên signum, char *const *argv)
{
  số_nguyên status = EXIT_SUCCESS;
  char const *arg = *argv;

  do
    {
      char *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      nếu (errno == ERANGE || ckd_add (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          status = EXIT_FAILURE;
        }
      không_thì nếu (kill (pid, signum) != 0)
        {
          nếu (errno == EINVAL)
            error (0, errno, "%d", signum);
          không_thì
            error (0, errno, "%s", quote (arg));
          status = EXIT_FAILURE;
        }
    }
  trong_khi ((arg = *++argv));

  trả_về status;
}

số_nguyên
main (số_nguyên argc, char **argv)
{
  số_nguyên optc;
  bool list = false;
  bool table = false;
  số_nguyên signum = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  trong_khi ((optc = getopt_long (argc, argv, short_options, long_options, NULL))
         != -1)
    switch (optc)
      {
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
        nếu (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            goto no_more_options;
          }
        FALLTHROUGH;
      case 'A': case 'B': case 'C': case 'D': case 'E':
      case 'F': case 'G': case 'H': case 'I': case 'J':
      case 'K': /*case 'L':*/ case 'M': case 'N': case 'O':
      case 'P': case 'Q': case 'R': case 'S': case 'T':
      case 'U': case 'V': case 'W': case 'X': case 'Y':
      case 'Z':
        nếu (! optarg)
          optarg = argv[optind - 1] + strlen (argv[optind - 1]);
        nếu (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      case 'n': /* -n is not documented, but is for Bash compatibility.  */
      case 's':
        nếu (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        nếu (signum < 0)
          usage (EXIT_FAILURE);
        break;

      case 'L': /* -L is not documented, but is for procps compatibility.  */
      case 't':
        table = true;
        FALLTHROUGH;
      case 'l':
        nếu (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        break;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      default:
        usage (EXIT_FAILURE);
      }
 no_more_options:

  nếu (signum < 0)
    signum = SIGTERM;
  không_thì nếu (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  nếu ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  trả_về (list
          ? list_signals (table, optind < argc ? argv + optind : NULL)
          : send_signals (signum, argv + optind));
}
