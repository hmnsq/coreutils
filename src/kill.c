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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <signal.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "sig2str.h"
#ಅನ್ತರ್ಭಾವ "operand2sig.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "kill"

#define AUTHORS proper_name ("Paul Eggert")

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"list", no_argument, ರಿಕ್ತ, 'l'},
  {"signal", required_argument, ರಿಕ್ತ, 's'},
  {"table", no_argument, ರಿಕ್ತ, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
  -s, --ಸಙ್ಕೇತ=SIGNAL, -SIGNAL\n\
         specify the name or ಸಂಖ್ಯೆ of the ಸಙ್ಕೇತ to be sent\n\
"));
      oputs (_("\
  -l, --list\n\
         list ಸಙ್ಕೇತ names, or convert ಸಙ್ಕೇತ names to/from numbers\n\
"));
      oputs (_("\
  -t, --table\n\
         print a table of ಸಙ್ಕೇತ information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a ಸಙ್ಕೇತ name like 'HUP', or a ಸಙ್ಕೇತ ಸಂಖ್ಯೆ like '1',\n\
or the exit status of a process terminated by a ಸಙ್ಕೇತ.\n\
PID is an integer; ಯದಿ negative it identifies a process group.\n\
"), stdout);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_table_row (ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_width, ಪೂರ್ಣಾಙ್ಕ signum,
                 ಪೂರ್ಣಾಙ್ಕ name_width, ವರ್ಣ ಧ್ರುವ *signame)
{
  ವರ್ಣ ಧ್ರುವ *description = strsignal (signum);
  printf ("%*d %-*s %s\n", ಅಂಕೆ_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
list_signals (bool table, ವರ್ಣ *ಧ್ರುವ *argv)
{
  ಪೂರ್ಣಾಙ್ಕ status = EXIT_SUCCESS;
  ವರ್ಣ signame[SIG2STR_MAX];

  ಯದಿ (table)
    {
      ಪೂರ್ಣಾಙ್ಕ name_width = 0;

      /* Compute the maximum width of a signal number.  */
      ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_width = 1;
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        ಅಂಕೆ_width++;

      /* Compute the maximum width of a signal name.  */
      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ signum = 0; signum <= SIGNUM_BOUND; signum++)
        ಯದಿ (sig2str (signum, signame) == 0)
          {
            idx_t len = strlen (signame);
            ಯದಿ (name_width < len)
              name_width = len;
          }

      ಯದಿ (argv)
        ಪ್ರತಿ (; *argv; argv++)
          {
            ಪೂರ್ಣಾಙ್ಕ signum = operand2sig (*argv);
            ಯದಿ (signum < 0)
              status = EXIT_FAILURE;
            ಅನ್ಯಥಾ
              {
                ಯದಿ (sig2str (signum, signame) != 0)
                  snprintf (signame, ಪರಿಮಾಣ signame, "SIG%d", signum);
                print_table_row (ಅಂಕೆ_width, signum, name_width, signame);
              }
          }
      ಅನ್ಯಥಾ
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ signum = 0; signum <= SIGNUM_BOUND; signum++)
          ಯದಿ (sig2str (signum, signame) == 0)
            print_table_row (ಅಂಕೆ_width, signum, name_width, signame);
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (argv)
        ಪ್ರತಿ (; *argv; argv++)
          {
            ಪೂರ್ಣಾಙ್ಕ signum = operand2sig (*argv);
            ಯದಿ (signum < 0)
              status = EXIT_FAILURE;
            ಅನ್ಯಥಾ ಯದಿ (c_isdigit (**argv))
              {
                ಯದಿ (sig2str (signum, signame) == 0)
                  puts (signame);
                ಅನ್ಯಥಾ
                  printf ("%d\n", signum);
              }
            ಅನ್ಯಥಾ
              printf ("%d\n", signum);
          }
      ಅನ್ಯಥಾ
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ signum = 0; signum <= SIGNUM_BOUND; signum++)
          ಯದಿ (sig2str (signum, signame) == 0)
            puts (signame);
    }

  ಪ್ರತ್ಯಾಗಮ status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
send_signals (ಪೂರ್ಣಾಙ್ಕ signum, ವರ್ಣ *ಧ್ರುವ *argv)
{
  ಪೂರ್ಣಾಙ್ಕ status = EXIT_SUCCESS;
  ವರ್ಣ ಧ್ರುವ *arg = *argv;

  ಕುರು
    {
      ವರ್ಣ *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      ಯದಿ (errno == ERANGE || ckd_add (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          status = EXIT_FAILURE;
        }
      ಅನ್ಯಥಾ ಯದಿ (kill (pid, signum) != 0)
        {
          ಯದಿ (errno == EINVAL)
            error (0, errno, "%d", signum);
          ಅನ್ಯಥಾ
            error (0, errno, "%s", quote (arg));
          status = EXIT_FAILURE;
        }
    }
  ಯಾವತ್ ((arg = *++argv));

  ಪ್ರತ್ಯಾಗಮ status;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ optc;
  bool list = false;
  bool table = false;
  ಪೂರ್ಣಾಙ್ಕ signum = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, short_options, long_options, ರಿಕ್ತ))
         != -1)
    ವಿಕಲ್ಪ (optc)
      {
      ಪಕ್ಷ '0': ಪಕ್ಷ '1': ಪಕ್ಷ '2': ಪಕ್ಷ '3': ಪಕ್ಷ '4':
      ಪಕ್ಷ '5': ಪಕ್ಷ '6': ಪಕ್ಷ '7': ಪಕ್ಷ '8': ಪಕ್ಷ '9':
        ಯದಿ (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            ಗಚ್ಛ no_more_options;
          }
        FALLTHROUGH;
      ಪಕ್ಷ 'A': ಪಕ್ಷ 'B': ಪಕ್ಷ 'C': ಪಕ್ಷ 'D': ಪಕ್ಷ 'E':
      ಪಕ್ಷ 'F': ಪಕ್ಷ 'G': ಪಕ್ಷ 'H': ಪಕ್ಷ 'I': ಪಕ್ಷ 'J':
      ಪಕ್ಷ 'K': /*case 'L':*/ ಪಕ್ಷ 'M': ಪಕ್ಷ 'N': ಪಕ್ಷ 'O':
      ಪಕ್ಷ 'P': ಪಕ್ಷ 'Q': ಪಕ್ಷ 'R': ಪಕ್ಷ 'S': ಪಕ್ಷ 'T':
      ಪಕ್ಷ 'U': ಪಕ್ಷ 'V': ಪಕ್ಷ 'W': ಪಕ್ಷ 'X': ಪಕ್ಷ 'Y':
      ಪಕ್ಷ 'Z':
        ಯದಿ (! optarg)
          optarg = argv[optind - 1] + strlen (argv[optind - 1]);
        ಯದಿ (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      ಪಕ್ಷ 'n': /* -n is not documented, but is for Bash compatibility.  */
      ಪಕ್ಷ 's':
        ಯದಿ (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        ಯದಿ (signum < 0)
          usage (EXIT_FAILURE);
        ಭಙ್ಗ;

      ಪಕ್ಷ 'L': /* -L is not documented, but is for procps compatibility.  */
      ಪಕ್ಷ 't':
        table = true;
        FALLTHROUGH;
      ಪಕ್ಷ 'l':
        ಯದಿ (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        ಭಙ್ಗ;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      ಸ್ವಭಾವ:
        usage (EXIT_FAILURE);
      }
 no_more_options:

  ಯದಿ (signum < 0)
    signum = SIGTERM;
  ಅನ್ಯಥಾ ಯದಿ (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  ಪ್ರತ್ಯಾಗಮ (list
          ? list_signals (table, optind < argc ? argv + optind : ರಿಕ್ತ)
          : send_signals (signum, argv + optind));
}
