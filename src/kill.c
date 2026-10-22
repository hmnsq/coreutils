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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <signal.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "sig2str.h"
#అన్తర్భావ "operand2sig.h"
#అన్తర్భావ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "kill"

#నిర్వచించు AUTHORS proper_name ("Paul Eggert")

స్థిర అక్షరం స్థిరరాశి short_options[] =
  "0::1::2::3::4::5::6::7::8::9::"
  "A::B::C::D::E::F::G::H::I::J::K::M::"
  "N::O::P::Q::R::S::T::U::V::W::X::Y::Z::"
  "Lln:s:t";

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"list", no_వాదం, ఖాళీ, 'l'},
  {"signal", required_వాదం, ఖాళీ, 's'},
  {"table", no_వాదం, ఖాళీ, 't'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [-s SIGNAL | -SIGNAL] PID...\n\
  or:  %s -l [SIGNAL]...\n\
  or:  %s -t [SIGNAL]...\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Send సంకేతాలు గమ్యం processes, or list సంకేతాలు.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -s, --సంకేతం=SIGNAL, -SIGNAL\n\
         specify the name or సంఖ్య యొక్క the సంకేతం గమ్యం be పంపిన\n\
"));
      oputs (_("\
  -l, --list\n\
         list సంకేతం పేర్లు, or convert సంకేతం పేర్లు గమ్యం/from numbers\n\
"));
      oputs (_("\
  -t, --పట్టిక\n\
         print a పట్టిక యొక్క సంకేతం information\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\n\
SIGNAL may be a సంకేతం name like 'HUP', or a సంకేతం సంఖ్య like '1',\n\
or the నిష్క్రమించు స్థితి_2 యొక్క a process terminated by a సంకేతం.\n\
PID is an integer; ఒకవేళ negative it identifies a process సమూహం.\n\
"), stdout);
      రాయి (USAGE_BUILTIN_WARNING, కార్యక్రమం_NAME);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Print a row of 'kill -t' output.  NUM_WIDTH is the maximum signal
   number width, and SIGNUM is the signal number to print.  The
   maximum name width is NAME_WIDTH, and SIGNAME is the name to print.  */

స్థిర శూన్య
print_పట్టిక_అడ్డువరుస (పూర్ణాంకం అంకె_వెడల్పు, పూర్ణాంకం signum,
                 పూర్ణాంకం name_వెడల్పు, అక్షరం స్థిరరాశి *signame)
{
  అక్షరం స్థిరరాశి *description = strsignal (signum);
  రాయి ("%*d %-*s %s\n", అంకె_వెడల్పు, signum, name_వెడల్పు, signame,
          description ? description : "?");
}

/* Print a list of signal names.  If TABLE, print a table.
   Print the names specified by ARGV if nonzero; otherwise,
   print all known names.  Return a suitable exit status.  */

స్థిర పూర్ణాంకం
list_సంకేతాలు (bool పట్టిక, అక్షరం *స్థిరరాశి *argv)
{
  పూర్ణాంకం స్థితి_2 = EXIT_SUCCESS;
  అక్షరం signame[SIG2STR_MAX];

  ఒకవేళ (పట్టిక)
    {
      పూర్ణాంకం name_వెడల్పు = 0;

      /* Compute the maximum width of a signal number.  */
      పూర్ణాంకం అంకె_వెడల్పు = 1;
      కొరకు (పూర్ణాంకం signum = 1; signum <= SIGNUM_BOUND / 10; signum *= 10)
        అంకె_వెడల్పు++;

      /* Compute the maximum width of a signal name.  */
      కొరకు (పూర్ణాంకం signum = 0; signum <= SIGNUM_BOUND; signum++)
        ఒకవేళ (sig2str (signum, signame) == 0)
          {
            idx_t len = అక్షరశ్రేణి_పొడవు (signame);
            ఒకవేళ (name_వెడల్పు < len)
              name_వెడల్పు = len;
          }

      ఒకవేళ (argv)
        కొరకు (; *argv; argv++)
          {
            పూర్ణాంకం signum = operand2sig (*argv);
            ఒకవేళ (signum < 0)
              స్థితి_2 = EXIT_FAILURE;
            లేకపోతే
              {
                ఒకవేళ (sig2str (signum, signame) != 0)
                  snprintf (signame, పరిమాణం signame, "SIG%d", signum);
                print_పట్టిక_అడ్డువరుస (అంకె_వెడల్పు, signum, name_వెడల్పు, signame);
              }
          }
      లేకపోతే
        కొరకు (పూర్ణాంకం signum = 0; signum <= SIGNUM_BOUND; signum++)
          ఒకవేళ (sig2str (signum, signame) == 0)
            print_పట్టిక_అడ్డువరుస (అంకె_వెడల్పు, signum, name_వెడల్పు, signame);
    }
  లేకపోతే
    {
      ఒకవేళ (argv)
        కొరకు (; *argv; argv++)
          {
            పూర్ణాంకం signum = operand2sig (*argv);
            ఒకవేళ (signum < 0)
              స్థితి_2 = EXIT_FAILURE;
            లేకపోతే ఒకవేళ (c_isdigit (**argv))
              {
                ఒకవేళ (sig2str (signum, signame) == 0)
                  puts (signame);
                లేకపోతే
                  రాయి ("%d\n", signum);
              }
            లేకపోతే
              రాయి ("%d\n", signum);
          }
      లేకపోతే
        కొరకు (పూర్ణాంకం signum = 0; signum <= SIGNUM_BOUND; signum++)
          ఒకవేళ (sig2str (signum, signame) == 0)
            puts (signame);
    }

  తిరిగి స్థితి_2;
}

/* Send signal SIGNUM to all the processes or process groups specified
   by ARGV.  Return a suitable exit status.  */

స్థిర పూర్ణాంకం
send_సంకేతాలు (పూర్ణాంకం signum, అక్షరం *స్థిరరాశి *argv)
{
  పూర్ణాంకం స్థితి_2 = EXIT_SUCCESS;
  అక్షరం స్థిరరాశి *arg = *argv;

  చేయి
    {
      అక్షరం *endp;
      intmax_t n = (errno = 0, strtoimax (arg, &endp, 10));
      pid_t pid;

      ఒకవేళ (errno == ERANGE || ckd_కూడు (&pid, n, 0)
          || arg == endp || *endp)
        {
          error (0, 0, _("%s: invalid process id"), quote (arg));
          స్థితి_2 = EXIT_FAILURE;
        }
      లేకపోతే ఒకవేళ (kill (pid, signum) != 0)
        {
          ఒకవేళ (errno == EINVAL)
            error (0, errno, "%d", signum);
          లేకపోతే
            error (0, errno, "%s", quote (arg));
          స్థితి_2 = EXIT_FAILURE;
        }
    }
  అయితే ((arg = *++argv));

  తిరిగి స్థితి_2;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం optc;
  bool list = false;
  bool పట్టిక = false;
  పూర్ణాంకం signum = -1;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, short_options, long_options, ఖాళీ))
         != -1)
    ఎంపిక (optc)
      {
      స్థితి '0': స్థితి '1': స్థితి '2': స్థితి '3': స్థితి '4':
      స్థితి '5': స్థితి '6': స్థితి '7': స్థితి '8': స్థితి '9':
        ఒకవేళ (optind != 2)
          {
            /* This option is actually a process-id.  */
            optind--;
            వెళ్ళు no_మరిన్ని_options;
          }
        FALLTHROUGH;
      స్థితి 'A': స్థితి 'B': స్థితి 'C': స్థితి 'D': స్థితి 'E':
      స్థితి 'F': స్థితి 'G': స్థితి 'H': స్థితి 'I': స్థితి 'J':
      స్థితి 'K': /*case 'L':*/ స్థితి 'M': స్థితి 'N': స్థితి 'O':
      స్థితి 'P': స్థితి 'Q': స్థితి 'R': స్థితి 'S': స్థితి 'T':
      స్థితి 'U': స్థితి 'V': స్థితి 'W': స్థితి 'X': స్థితి 'Y':
      స్థితి 'Z':
        ఒకవేళ (! optarg)
          optarg = argv[optind - 1] + అక్షరశ్రేణి_పొడవు (argv[optind - 1]);
        ఒకవేళ (optarg != argv[optind - 1] + 2)
          {
            error (0, 0, _("invalid option -- %c"), optc);
            usage (EXIT_FAILURE);
          }
        optarg--;
        FALLTHROUGH;
      స్థితి 'n': /* -n is not documented, but is for Bash compatibility.  */
      స్థితి 's':
        ఒకవేళ (0 <= signum)
          {
            error (0, 0, _("%s: multiple signals specified"), quote (optarg));
            usage (EXIT_FAILURE);
          }
        signum = operand2sig (optarg);
        ఒకవేళ (signum < 0)
          usage (EXIT_FAILURE);
        ఆపు;

      స్థితి 'L': /* -L is not documented, but is for procps compatibility.  */
      స్థితి 't':
        పట్టిక = true;
        FALLTHROUGH;
      స్థితి 'l':
        ఒకవేళ (list)
          {
            error (0, 0, _("multiple -l or -t options specified"));
            usage (EXIT_FAILURE);
          }
        list = true;
        ఆపు;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
      అప్రమేయ:
        usage (EXIT_FAILURE);
      }
 no_మరిన్ని_options:

  ఒకవేళ (signum < 0)
    signum = SIGTERM;
  లేకపోతే ఒకవేళ (list)
    {
      error (0, 0, _("cannot combine signal with -l or -t"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ ( ! list && argc <= optind)
    {
      error (0, 0, _("no process ID specified"));
      usage (EXIT_FAILURE);
    }

  తిరిగి (list
          ? list_సంకేతాలు (పట్టిక, optind < argc ? argv + optind : ఖాళీ)
          : send_సంకేతాలు (signum, argv + optind));
}
