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

static var_na const short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

static sa_nracanaa option const long_options[] =
{
  {"list", no_argument, NULL, 'l'},
  {"signal", required_argument, NULL, 's'},
  {"table", no_argument, NULL, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (puur_naa_nka status)
{
  ᬬᬾᬦ᭄ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᬮᬶᬬ
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
  -s, --sa_nkeeta=SIGNAL, -SIGNAL\n\
         specify the name or number of the sa_nkeeta to be sent\n\
"));
      oputs (_("\
  -l, --list\n\
         list sa_nkeeta names, or convert sa_nkeeta names to/from numbers\n\
"));
      oputs (_("\
  -t, --table\n\
         print a table of sa_nkeeta information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a sa_nkeeta name like 'HUP', or a sa_nkeeta number like '1',\n\
or the exit status of a process terminated by a sa_nkeeta.\n\
PID is an integer; ᬬᬾᬦ᭄ negative it identifies a process group.\n\
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
print_table_row (puur_naa_nka num_width, puur_naa_nka signum,
                 puur_naa_nka name_width, var_na const *signame)
{
  var_na const *description = strsignal (signum);
  printf ("%*d %-*s %s\n", num_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

static puur_naa_nka
list_signals (bool table, var_na *const *argv)
{
  puur_naa_nka status = EXIT_SUCCESS;
  var_na signame[SIG2STR_MAX];

  ᬬᬾᬦ᭄ (table)
    {
      puur_naa_nka name_width = 0;

      /* Compute the maximum width of a signal number.  */
      puur_naa_nka num_width = 1;
      for (puur_naa_nka signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        num_width++;

      /* Compute the maximum width of a signal name.  */
      for (puur_naa_nka signum = 0; signum <= SIGNUM_BOUND; signum++)
        ᬬᬾᬦ᭄ (sig2str (signum, signame) == 0)
          {
            idx_t len = strlen (signame);
            ᬬᬾᬦ᭄ (name_width < len)
              name_width = len;
          }

      ᬬᬾᬦ᭄ (argv)
        for (; *argv; argv++)
          {
            puur_naa_nka signum = operand2sig (*argv);
            ᬬᬾᬦ᭄ (signum < 0)
              status = EXIT_FAILURE;
            ᬮᬶᬬ
              {
                ᬬᬾᬦ᭄ (sig2str (signum, signame) != 0)
                  snprintf (signame, parimaa_na signame, "SIG%d", signum);
                print_table_row (num_width, signum, name_width, signame);
              }
          }
      ᬮᬶᬬ
        for (puur_naa_nka signum = 0; signum <= SIGNUM_BOUND; signum++)
          ᬬᬾᬦ᭄ (sig2str (signum, signame) == 0)
            print_table_row (num_width, signum, name_width, signame);
    }
  ᬮᬶᬬ
    {
      ᬬᬾᬦ᭄ (argv)
        for (; *argv; argv++)
          {
            puur_naa_nka signum = operand2sig (*argv);
            ᬬᬾᬦ᭄ (signum < 0)
              status = EXIT_FAILURE;
            ᬮᬶᬬ ᬬᬾᬦ᭄ (c_isdigit (**argv))
              {
                ᬬᬾᬦ᭄ (sig2str (signum, signame) == 0)
                  puts (signame);
                ᬮᬶᬬ
                  printf ("%d\n", signum);
              }
            ᬮᬶᬬ
              printf ("%d\n", signum);
          }
      ᬮᬶᬬ
        for (puur_naa_nka signum = 0; signum <= SIGNUM_BOUND; signum++)
          ᬬᬾᬦ᭄ (sig2str (signum, signame) == 0)
            puts (signame);
    }

  ᬯᬮᬶ status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

static puur_naa_nka
send_signals (puur_naa_nka signum, var_na *const *argv)
{
  puur_naa_nka status = EXIT_SUCCESS;
  var_na const *arg = *argv;

  do
    {
      var_na *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      ᬬᬾᬦ᭄ (errno == ERANGE || ckd_add (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          status = EXIT_FAILURE;
        }
      ᬮᬶᬬ ᬬᬾᬦ᭄ (kill (pid, signum) != 0)
        {
          ᬬᬾᬦ᭄ (errno == EINVAL)
            error (0, errno, "%d", signum);
          ᬮᬶᬬ
            error (0, errno, "%s", quote (arg));
          status = EXIT_FAILURE;
        }
    }
  ᬲᭂᬤᭂᬮ ((arg = *++argv));

  ᬯᬮᬶ status;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  puur_naa_nka optc;
  bool list = false;
  bool table = false;
  puur_naa_nka signum = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ᬲᭂᬤᭂᬮ ((optc = getopt_long (argc, argv, short_options, long_options, NULL))
         != -1)
    switch (optc)
      {
      pak_sa '0': pak_sa '1': pak_sa '2': pak_sa '3': pak_sa '4':
      pak_sa '5': pak_sa '6': pak_sa '7': pak_sa '8': pak_sa '9':
        ᬬᬾᬦ᭄ (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            goto no_more_options;
          }
        FALLTHROUGH;
      pak_sa 'A': pak_sa 'B': pak_sa 'C': pak_sa 'D': pak_sa 'E':
      pak_sa 'F': pak_sa 'G': pak_sa 'H': pak_sa 'I': pak_sa 'J':
      pak_sa 'K': /*case 'L':*/ pak_sa 'M': pak_sa 'N': pak_sa 'O':
      pak_sa 'P': pak_sa 'Q': pak_sa 'R': pak_sa 'S': pak_sa 'T':
      pak_sa 'U': pak_sa 'V': pak_sa 'W': pak_sa 'X': pak_sa 'Y':
      pak_sa 'Z':
        ᬬᬾᬦ᭄ (! optarg)
          optarg = argv[optind - 1] + strlen (argv[optind - 1]);
        ᬬᬾᬦ᭄ (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      pak_sa 'n': /* -n is not documented, but is for Bash compatibility.  */
      pak_sa 's':
        ᬬᬾᬦ᭄ (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        ᬬᬾᬦ᭄ (signum < 0)
          usage (EXIT_FAILURE);
        bha_nga;

      pak_sa 'L': /* -L is not documented, but is for procps compatibility.  */
      pak_sa 't':
        table = true;
        FALLTHROUGH;
      pak_sa 'l':
        ᬬᬾᬦ᭄ (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        bha_nga;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      default:
        usage (EXIT_FAILURE);
      }
 no_more_options:

  ᬬᬾᬦ᭄ (signum < 0)
    signum = SIGTERM;
  ᬮᬶᬬ ᬬᬾᬦ᭄ (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  ᬬᬾᬦ᭄ ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  ᬯᬮᬶ (list
          ? list_signals (table, optind < argc ? argv + optind : NULL)
          : send_signals (signum, argv + optind));
}
