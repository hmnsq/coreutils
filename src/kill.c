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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <signal.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "sig2str.h"
#അന്തര്ഭാവ "operand2sig.h"
#അന്തര്ഭാവ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "kill"

#define AUTHORS proper_name ("Paul Eggert")

സ്ഥിതം അക്ഷരം സ്ഥിരം short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"list", no_argument, ശൂന്യ, 'l'},
  {"signal", required_argument, ശൂന്യ, 's'},
  {"table", no_argument, ശൂന്യ, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
  -s, --സൂചന=SIGNAL, -SIGNAL\n\
         specify the name or സംഖ്യ of the സൂചന to be sent\n\
"));
      oputs (_("\
  -l, --list\n\
         list സൂചന names, or convert സൂചന names to/from numbers\n\
"));
      oputs (_("\
  -t, --table\n\
         print a table of സൂചന information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a സൂചന name like 'HUP', or a സൂചന സംഖ്യ like '1',\n\
or the exit status of a process terminated by a സൂചന.\n\
PID is an integer; എങ്കിൽ negative it identifies a process group.\n\
"), stdout);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

സ്ഥിതം ശൂന്യം
print_table_row (പൂർണ്ണസംഖ്യ അക്കം_width, പൂർണ്ണസംഖ്യ signum,
                 പൂർണ്ണസംഖ്യ name_width, അക്ഷരം സ്ഥിരം *signame)
{
  അക്ഷരം സ്ഥിരം *description = strsignal (signum);
  printf ("%*d %-*s %s\n", അക്കം_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
list_signals (bool table, അക്ഷരം *സ്ഥിരം *argv)
{
  പൂർണ്ണസംഖ്യ status = EXIT_SUCCESS;
  അക്ഷരം signame[SIG2STR_MAX];

  എങ്കിൽ (table)
    {
      പൂർണ്ണസംഖ്യ name_width = 0;

      /* Compute the maximum width of a signal number.  */
      പൂർണ്ണസംഖ്യ അക്കം_width = 1;
      വേണ്ടി (പൂർണ്ണസംഖ്യ signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        അക്കം_width++;

      /* Compute the maximum width of a signal name.  */
      വേണ്ടി (പൂർണ്ണസംഖ്യ signum = 0; signum <= SIGNUM_BOUND; signum++)
        എങ്കിൽ (sig2str (signum, signame) == 0)
          {
            idx_t len = strlen (signame);
            എങ്കിൽ (name_width < len)
              name_width = len;
          }

      എങ്കിൽ (argv)
        വേണ്ടി (; *argv; argv++)
          {
            പൂർണ്ണസംഖ്യ signum = operand2sig (*argv);
            എങ്കിൽ (signum < 0)
              status = EXIT_FAILURE;
            അല്ലെങ്കിൽ
              {
                എങ്കിൽ (sig2str (signum, signame) != 0)
                  snprintf (signame, വലുപ്പം signame, "SIG%d", signum);
                print_table_row (അക്കം_width, signum, name_width, signame);
              }
          }
      അല്ലെങ്കിൽ
        വേണ്ടി (പൂർണ്ണസംഖ്യ signum = 0; signum <= SIGNUM_BOUND; signum++)
          എങ്കിൽ (sig2str (signum, signame) == 0)
            print_table_row (അക്കം_width, signum, name_width, signame);
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (argv)
        വേണ്ടി (; *argv; argv++)
          {
            പൂർണ്ണസംഖ്യ signum = operand2sig (*argv);
            എങ്കിൽ (signum < 0)
              status = EXIT_FAILURE;
            അല്ലെങ്കിൽ എങ്കിൽ (c_isdigit (**argv))
              {
                എങ്കിൽ (sig2str (signum, signame) == 0)
                  puts (signame);
                അല്ലെങ്കിൽ
                  printf ("%d\n", signum);
              }
            അല്ലെങ്കിൽ
              printf ("%d\n", signum);
          }
      അല്ലെങ്കിൽ
        വേണ്ടി (പൂർണ്ണസംഖ്യ signum = 0; signum <= SIGNUM_BOUND; signum++)
          എങ്കിൽ (sig2str (signum, signame) == 0)
            puts (signame);
    }

  തിരികെ status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

സ്ഥിതം പൂർണ്ണസംഖ്യ
send_signals (പൂർണ്ണസംഖ്യ signum, അക്ഷരം *സ്ഥിരം *argv)
{
  പൂർണ്ണസംഖ്യ status = EXIT_SUCCESS;
  അക്ഷരം സ്ഥിരം *arg = *argv;

  ചെയ്യുക
    {
      അക്ഷരം *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      എങ്കിൽ (errno == ERANGE || ckd_add (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          status = EXIT_FAILURE;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (kill (pid, signum) != 0)
        {
          എങ്കിൽ (errno == EINVAL)
            error (0, errno, "%d", signum);
          അല്ലെങ്കിൽ
            error (0, errno, "%s", quote (arg));
          status = EXIT_FAILURE;
        }
    }
  ആകുമ്പോൾ ((arg = *++argv));

  തിരികെ status;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ optc;
  bool list = false;
  bool table = false;
  പൂർണ്ണസംഖ്യ signum = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, short_options, long_options, ശൂന്യ))
         != -1)
    തിരഞ്ഞെടുപ്പ് (optc)
      {
      അവസ്ഥ '0': അവസ്ഥ '1': അവസ്ഥ '2': അവസ്ഥ '3': അവസ്ഥ '4':
      അവസ്ഥ '5': അവസ്ഥ '6': അവസ്ഥ '7': അവസ്ഥ '8': അവസ്ഥ '9':
        എങ്കിൽ (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            പോകുക no_more_options;
          }
        FALLTHROUGH;
      അവസ്ഥ 'A': അവസ്ഥ 'B': അവസ്ഥ 'C': അവസ്ഥ 'D': അവസ്ഥ 'E':
      അവസ്ഥ 'F': അവസ്ഥ 'G': അവസ്ഥ 'H': അവസ്ഥ 'I': അവസ്ഥ 'J':
      അവസ്ഥ 'K': /*case 'L':*/ അവസ്ഥ 'M': അവസ്ഥ 'N': അവസ്ഥ 'O':
      അവസ്ഥ 'P': അവസ്ഥ 'Q': അവസ്ഥ 'R': അവസ്ഥ 'S': അവസ്ഥ 'T':
      അവസ്ഥ 'U': അവസ്ഥ 'V': അവസ്ഥ 'W': അവസ്ഥ 'X': അവസ്ഥ 'Y':
      അവസ്ഥ 'Z':
        എങ്കിൽ (! optarg)
          optarg = argv[optind - 1] + strlen (argv[optind - 1]);
        എങ്കിൽ (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      അവസ്ഥ 'n': /* -n is not documented, but is for Bash compatibility.  */
      അവസ്ഥ 's':
        എങ്കിൽ (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        എങ്കിൽ (signum < 0)
          usage (EXIT_FAILURE);
        നിർത്തുക;

      അവസ്ഥ 'L': /* -L is not documented, but is for procps compatibility.  */
      അവസ്ഥ 't':
        table = true;
        FALLTHROUGH;
      അവസ്ഥ 'l':
        എങ്കിൽ (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        നിർത്തുക;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      സ്ഥിരസ്ഥിതി:
        usage (EXIT_FAILURE);
      }
 no_more_options:

  എങ്കിൽ (signum < 0)
    signum = SIGTERM;
  അല്ലെങ്കിൽ എങ്കിൽ (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  തിരികെ (list
          ? list_signals (table, optind < argc ? argv + optind : ശൂന്യ)
          : send_signals (signum, argv + optind));
}
