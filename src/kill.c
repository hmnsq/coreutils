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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "kill"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Paul Eggert")

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"list", no_ವಾದ, ಖಾಲಿ, 'l'},
  {"signal", required_ವಾದ, ಖಾಲಿ, 's'},
  {"table", no_ವಾದ, ಖಾಲಿ, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [-s SIGNAL | -SIGNAL] PID...\n\
  or:  %s -l [SIGNAL]...\n\
  or:  %s -t [SIGNAL]...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Send ಸಂಕೇತಗಳು ಗಮ್ಯ processes, or list ಸಂಕೇತಗಳು.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -s, --ಸಂಕೇತ=SIGNAL, -SIGNAL\n\
         specify the name or ಸಂಖ್ಯೆ ದ the ಸಂಕೇತ ಗಮ್ಯ be ಕಳುಹಿಸಿದ\n\
"));
      oputs (_("\
  -l, --list\n\
         list ಸಂಕೇತ ಹೆಸರುಗಳು, or convert ಸಂಕೇತ ಹೆಸರುಗಳು ಗಮ್ಯ/from numbers\n\
"));
      oputs (_("\
  -t, --ಕೋಷ್ಟಕ\n\
         print a ಕೋಷ್ಟಕ ದ ಸಂಕೇತ information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a ಸಂಕೇತ name like 'HUP', or a ಸಂಕೇತ ಸಂಖ್ಯೆ like '1',\n\
or the ನಿರ್ಗಮಿಸು ಸ್ಥಿತಿ_2 ದ a process terminated by a ಸಂಕೇತ.\n\
PID is an integer; ಆದರೆ negative it identifies a process ಗುಂಪು.\n\
"), stdout);
      ಬರೆ (USAGE_BUILTIN_WARNING, ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

ಸ್ಥಿರ ಶೂನ್ಯ
print_ಕೋಷ್ಟಕ_ಅಡ್ಡಸಾಲು (ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಅಗಲ, ಪೂರ್ಣಾಂಕ signum,
                 ಪೂರ್ಣಾಂಕ name_ಅಗಲ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *signame)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *description = strsignal (signum);
  ಬರೆ ("%*d %-*s %s\n", ಅಂಕೆ_ಅಗಲ, signum, name_ಅಗಲ, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
list_ಸಂಕೇತಗಳು (bool ಕೋಷ್ಟಕ, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *argv)
{
  ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = EXIT_SUCCESS;
  ಅಕ್ಷರ signame[SIG2STR_MAX];

  ಆದರೆ (ಕೋಷ್ಟಕ)
    {
      ಪೂರ್ಣಾಂಕ name_ಅಗಲ = 0;

      /* Compute the maximum width of a signal number.  */
      ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಅಗಲ = 1;
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        ಅಂಕೆ_ಅಗಲ++;

      /* Compute the maximum width of a signal name.  */
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ signum = 0; signum <= SIGNUM_BOUND; signum++)
        ಆದರೆ (sig2str (signum, signame) == 0)
          {
            idx_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (signame);
            ಆದರೆ (name_ಅಗಲ < len)
              name_ಅಗಲ = len;
          }

      ಆದರೆ (argv)
        ಗಾಗಿ (; *argv; argv++)
          {
            ಪೂರ್ಣಾಂಕ signum = operand2sig (*argv);
            ಆದರೆ (signum < 0)
              ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
            ಇಲ್ಲದಿದ್ದರೆ
              {
                ಆದರೆ (sig2str (signum, signame) != 0)
                  snprintf (signame, ಗಾತ್ರ signame, "SIG%d", signum);
                print_ಕೋಷ್ಟಕ_ಅಡ್ಡಸಾಲು (ಅಂಕೆ_ಅಗಲ, signum, name_ಅಗಲ, signame);
              }
          }
      ಇಲ್ಲದಿದ್ದರೆ
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ signum = 0; signum <= SIGNUM_BOUND; signum++)
          ಆದರೆ (sig2str (signum, signame) == 0)
            print_ಕೋಷ್ಟಕ_ಅಡ್ಡಸಾಲು (ಅಂಕೆ_ಅಗಲ, signum, name_ಅಗಲ, signame);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (argv)
        ಗಾಗಿ (; *argv; argv++)
          {
            ಪೂರ್ಣಾಂಕ signum = operand2sig (*argv);
            ಆದರೆ (signum < 0)
              ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c_isdigit (**argv))
              {
                ಆದರೆ (sig2str (signum, signame) == 0)
                  puts (signame);
                ಇಲ್ಲದಿದ್ದರೆ
                  ಬರೆ ("%d\n", signum);
              }
            ಇಲ್ಲದಿದ್ದರೆ
              ಬರೆ ("%d\n", signum);
          }
      ಇಲ್ಲದಿದ್ದರೆ
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ signum = 0; signum <= SIGNUM_BOUND; signum++)
          ಆದರೆ (sig2str (signum, signame) == 0)
            puts (signame);
    }

  ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
send_ಸಂಕೇತಗಳು (ಪೂರ್ಣಾಂಕ signum, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *argv)
{
  ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = EXIT_SUCCESS;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg = *argv;

  ಮಾಡು
    {
      ಅಕ್ಷರ *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      ಆದರೆ (errno == ERANGE || ckd_ಕೂಡಿಸು (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (kill (pid, signum) != 0)
        {
          ಆದರೆ (errno == EINVAL)
            error (0, errno, "%d", signum);
          ಇಲ್ಲದಿದ್ದರೆ
            error (0, errno, "%s", quote (arg));
          ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
        }
    }
  ಆಗ ((arg = *++argv));

  ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ optc;
  bool list = false;
  bool ಕೋಷ್ಟಕ = false;
  ಪೂರ್ಣಾಂಕ signum = -1;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((optc = getopt_long (argc, argv, short_options, long_options, ಖಾಲಿ))
         != -1)
    ಆಯ್ಕೆ (optc)
      {
      ಸ್ಥಿತಿ '0': ಸ್ಥಿತಿ '1': ಸ್ಥಿತಿ '2': ಸ್ಥಿತಿ '3': ಸ್ಥಿತಿ '4':
      ಸ್ಥಿತಿ '5': ಸ್ಥಿತಿ '6': ಸ್ಥಿತಿ '7': ಸ್ಥಿತಿ '8': ಸ್ಥಿತಿ '9':
        ಆದರೆ (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            ಹೋಗು no_ಇನ್ನಷ್ಟು_options;
          }
        FALLTHROUGH;
      ಸ್ಥಿತಿ 'A': ಸ್ಥಿತಿ 'B': ಸ್ಥಿತಿ 'C': ಸ್ಥಿತಿ 'D': ಸ್ಥಿತಿ 'E':
      ಸ್ಥಿತಿ 'F': ಸ್ಥಿತಿ 'G': ಸ್ಥಿತಿ 'H': ಸ್ಥಿತಿ 'I': ಸ್ಥಿತಿ 'J':
      ಸ್ಥಿತಿ 'K': /*case 'L':*/ ಸ್ಥಿತಿ 'M': ಸ್ಥಿತಿ 'N': ಸ್ಥಿತಿ 'O':
      ಸ್ಥಿತಿ 'P': ಸ್ಥಿತಿ 'Q': ಸ್ಥಿತಿ 'R': ಸ್ಥಿತಿ 'S': ಸ್ಥಿತಿ 'T':
      ಸ್ಥಿತಿ 'U': ಸ್ಥಿತಿ 'V': ಸ್ಥಿತಿ 'W': ಸ್ಥಿತಿ 'X': ಸ್ಥಿತಿ 'Y':
      ಸ್ಥಿತಿ 'Z':
        ಆದರೆ (! optarg)
          optarg = argv[optind - 1] + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (argv[optind - 1]);
        ಆದರೆ (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      ಸ್ಥಿತಿ 'n': /* -n is not documented, but is for Bash compatibility.  */
      ಸ್ಥಿತಿ 's':
        ಆದರೆ (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        ಆದರೆ (signum < 0)
          usage (EXIT_FAILURE);
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'L': /* -L is not documented, but is for procps compatibility.  */
      ಸ್ಥಿತಿ 't':
        ಕೋಷ್ಟಕ = true;
        FALLTHROUGH;
      ಸ್ಥಿತಿ 'l':
        ಆದರೆ (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        ನಿಲ್ಲಿಸು;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
      ಪೂರ್ವನಿಯೋಜಿತ:
        usage (EXIT_FAILURE);
      }
 no_ಇನ್ನಷ್ಟು_options:

  ಆದರೆ (signum < 0)
    signum = SIGTERM;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  ಹಿಂತಿರುಗಿ (list
          ? list_ಸಂಕೇತಗಳು (ಕೋಷ್ಟಕ, optind < argc ? argv + optind : ಖಾಲಿ)
          : send_ಸಂಕೇತಗಳು (signum, argv + optind));
}
