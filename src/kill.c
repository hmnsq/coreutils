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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <signal.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "sig2str.h"
#અન્તર્ભાવ "operand2sig.h"
#અન્તર્ભાવ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "kill"

#define AUTHORS proper_name ("Paul Eggert")

સ્થિર અક્ષર અચલ short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

સ્થિર સંરચના option અચલ long_options[] =
{
  {"list", no_argument, ખાલી, 'l'},
  {"signal", required_argument, ખાલી, 's'},
  {"table", no_argument, ખાલી, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
  -s, --સંકેત=SIGNAL, -SIGNAL\n\
         specify the name or સંખ્યા of the સંકેત to be sent\n\
"));
      oputs (_("\
  -l, --list\n\
         list સંકેત names, or convert સંકેત names to/from numbers\n\
"));
      oputs (_("\
  -t, --table\n\
         print a table of સંકેત information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a સંકેત name like 'HUP', or a સંકેત સંખ્યા like '1',\n\
or the exit status of a process terminated by a સંકેત.\n\
PID is an integer; જો negative it identifies a process group.\n\
"), stdout);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

સ્થિર શૂન્ય
print_table_row (પૂર્ણાંક અંક_width, પૂર્ણાંક signum,
                 પૂર્ણાંક name_width, અક્ષર અચલ *signame)
{
  અક્ષર અચલ *description = strsignal (signum);
  printf ("%*d %-*s %s\n", અંક_width, signum, name_width, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

સ્થિર પૂર્ણાંક
list_signals (bool table, અક્ષર *અચલ *argv)
{
  પૂર્ણાંક status = EXIT_SUCCESS;
  અક્ષર signame[SIG2STR_MAX];

  જો (table)
    {
      પૂર્ણાંક name_width = 0;

      /* Compute the maximum width of a signal number.  */
      પૂર્ણાંક અંક_width = 1;
      માટે (પૂર્ણાંક signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        અંક_width++;

      /* Compute the maximum width of a signal name.  */
      માટે (પૂર્ણાંક signum = 0; signum <= SIGNUM_BOUND; signum++)
        જો (sig2str (signum, signame) == 0)
          {
            idx_t len = strlen (signame);
            જો (name_width < len)
              name_width = len;
          }

      જો (argv)
        માટે (; *argv; argv++)
          {
            પૂર્ણાંક signum = operand2sig (*argv);
            જો (signum < 0)
              status = EXIT_FAILURE;
            નહીંતર
              {
                જો (sig2str (signum, signame) != 0)
                  snprintf (signame, કદ signame, "SIG%d", signum);
                print_table_row (અંક_width, signum, name_width, signame);
              }
          }
      નહીંતર
        માટે (પૂર્ણાંક signum = 0; signum <= SIGNUM_BOUND; signum++)
          જો (sig2str (signum, signame) == 0)
            print_table_row (અંક_width, signum, name_width, signame);
    }
  નહીંતર
    {
      જો (argv)
        માટે (; *argv; argv++)
          {
            પૂર્ણાંક signum = operand2sig (*argv);
            જો (signum < 0)
              status = EXIT_FAILURE;
            નહીંતર જો (c_isdigit (**argv))
              {
                જો (sig2str (signum, signame) == 0)
                  puts (signame);
                નહીંતર
                  printf ("%d\n", signum);
              }
            નહીંતર
              printf ("%d\n", signum);
          }
      નહીંતર
        માટે (પૂર્ણાંક signum = 0; signum <= SIGNUM_BOUND; signum++)
          જો (sig2str (signum, signame) == 0)
            puts (signame);
    }

  પાછા status;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

સ્થિર પૂર્ણાંક
send_signals (પૂર્ણાંક signum, અક્ષર *અચલ *argv)
{
  પૂર્ણાંક status = EXIT_SUCCESS;
  અક્ષર અચલ *arg = *argv;

  કરો
    {
      અક્ષર *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      જો (errno == ERANGE || ckd_add (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          status = EXIT_FAILURE;
        }
      નહીંતર જો (kill (pid, signum) != 0)
        {
          જો (errno == EINVAL)
            error (0, errno, "%d", signum);
          નહીંતર
            error (0, errno, "%s", quote (arg));
          status = EXIT_FAILURE;
        }
    }
  જ્યારે ((arg = *++argv));

  પાછા status;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક optc;
  bool list = false;
  bool table = false;
  પૂર્ણાંક signum = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((optc = getopt_long (argc, argv, short_options, long_options, ખાલી))
         != -1)
    પસંદગી (optc)
      {
      સ્થિતિ '0': સ્થિતિ '1': સ્થિતિ '2': સ્થિતિ '3': સ્થિતિ '4':
      સ્થિતિ '5': સ્થિતિ '6': સ્થિતિ '7': સ્થિતિ '8': સ્થિતિ '9':
        જો (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            જાઓ no_more_options;
          }
        FALLTHROUGH;
      સ્થિતિ 'A': સ્થિતિ 'B': સ્થિતિ 'C': સ્થિતિ 'D': સ્થિતિ 'E':
      સ્થિતિ 'F': સ્થિતિ 'G': સ્થિતિ 'H': સ્થિતિ 'I': સ્થિતિ 'J':
      સ્થિતિ 'K': /*case 'L':*/ સ્થિતિ 'M': સ્થિતિ 'N': સ્થિતિ 'O':
      સ્થિતિ 'P': સ્થિતિ 'Q': સ્થિતિ 'R': સ્થિતિ 'S': સ્થિતિ 'T':
      સ્થિતિ 'U': સ્થિતિ 'V': સ્થિતિ 'W': સ્થિતિ 'X': સ્થિતિ 'Y':
      સ્થિતિ 'Z':
        જો (! optarg)
          optarg = argv[optind - 1] + strlen (argv[optind - 1]);
        જો (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      સ્થિતિ 'n': /* -n is not documented, but is for Bash compatibility.  */
      સ્થિતિ 's':
        જો (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        જો (signum < 0)
          usage (EXIT_FAILURE);
        તોડો;

      સ્થિતિ 'L': /* -L is not documented, but is for procps compatibility.  */
      સ્થિતિ 't':
        table = true;
        FALLTHROUGH;
      સ્થિતિ 'l':
        જો (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        તોડો;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      મૂળભૂત:
        usage (EXIT_FAILURE);
      }
 no_more_options:

  જો (signum < 0)
    signum = SIGTERM;
  નહીંતર જો (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  જો ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  પાછા (list
          ? list_signals (table, optind < argc ? argv + optind : ખાલી)
          : send_signals (signum, argv + optind));
}
