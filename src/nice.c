/* nice -- run a program with modified niceness
   Copyright (C) 1990-2026 Free Software Foundation, Inc.

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

/* David MacKenzie <djm@gnu.ai.mit.edu> */

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"

#ఒకవేళ ! HAVE_NICE
/* Include this after "system.h" so we're sure to have definitions
   (from time.h or sys/time.h) required for e.g. the ru_utime member.  */
# include <sys/resource.h>
#అయితే_ముగింపు

#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "nice"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

#ఒకవేళ HAVE_NICE
# define GET_NICENESS() nice (0)
#లేకపోతే
# define GET_NICENESS() getpriority (PRIO_PROCESS, 0)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే NZERO
# define NZERO 20
#అయితే_ముగింపు

/* This is required for Darwin Kernel Version 7.7.0.  */
#ఒకవేళ NZERO == 0
# undef  NZERO
# define NZERO 20
#అయితే_ముగింపు

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"adjustment", required_వాదం, ఖాళీ, 'n'},
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
      రాయి (_("Usage: %s [OPTION] [COMMAND [ARG]...]\n"), కార్యక్రమం_name);
      రాయి (_("\
నిరంతరం COMMAND with an adjusted niceness, which affects process scheduling.\n\
With no COMMAND, print the current niceness.  Niceness విలువలు range from\n\
%d (most favorable గమ్యం the process) గమ్యం %d (కనిష్ఠ favorable గమ్యం the process).\n\
"),
              - NZERO, NZERO - 1);

      emit_mandatory_arg_note ();

      oputs (_("\
  -n, --adjustment=N\n\
         కూడు integer N గమ్యం the niceness (అప్రమేయ 10)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      రాయి (USAGE_BUILTIN_WARNING, కార్యక్రమం_NAME);
      emit_exec_స్థితి (కార్యక్రమం_NAME);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

స్థిర bool
perm_related_errno (పూర్ణాంకం err)
{
  తిరిగి err == EACCES || err == EPERM;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం current_niceness;
  పూర్ణాంకం adjustment = 10;
  అక్షరం స్థిరరాశి *adjustment_given = ఖాళీ;
  bool ok;
  పూర్ణాంకం i;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  కొరకు (i = 1; i < argc; /*ఖాళీలు*/)
    {
      అక్షరం స్థిరరాశి *s = argv[i];

      ఒకవేళ (s[0] == '-' && c_isdigit (s[1 + (s[1] == '-' || s[1] == '+')]))
        {
          adjustment_given = s + 1;
          ++i;
        }
      లేకపోతే
        {
          పూర్ణాంకం c;
          పూర్ణాంకం fake_argc = argc - (i - 1);
          అక్షరం **fake_argv = argv + (i - 1);

          /* Ensure that any getopt diagnostics use the right name.  */
          fake_argv[0] = argv[0];

          /* Initialize getopt_long's internal state.  */
          optind = 0;

          c = getopt_long (fake_argc, fake_argv, "+n:", longopts, ఖాళీ);
          i += optind - 1;

          ఎంపిక (c)
            {
            స్థితి 'n':
              adjustment_given = optarg;
              ఆపు;

            స్థితి -1:
              ఆపు;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

            అప్రమేయ:
              usage (EXIT_CANCELED);
              ఆపు;
            }

          ఒకవేళ (c == -1)
            ఆపు;
        }
    }

  ఒకవేళ (adjustment_given)
    {
      /* If the requested adjustment is outside the valid range,
         silently bring it to just within range; this mimics what
         "setpriority" and "nice" do.  */
#ఒకవేళ (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) only supported 0 to (2 * NZERO - 2) niceness
         until glibc 2.43.  */
      గణన { MIN_ADJUSTMENT = 0, MAX_ADJUSTMENT = 2 * NZERO - 2 };
#లేకపోతే
      గణన { MIN_ADJUSTMENT = 1 - 2 * NZERO, MAX_ADJUSTMENT = 2 * NZERO - 1 };
#అయితే_ముగింపు
      దీర్ఘ పూర్ణాంకం tmp;
      ఒకవేళ (LONGINT_అధికప్రవాహం < xstrtol (adjustment_given, ఖాళీ, 10, &tmp, ""))
        error (EXIT_CANCELED, 0, _("invalid adjustment %s"),
               quote (adjustment_given));
#ఒకవేళ (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) also did not clamp the new niceness into the
         supported range until glibc 2.43.
         See <https://sourceware.org/PR33614>.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      ఒకవేళ (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      ఒకవేళ (tmp < 0)
        {
          పూర్ణాంకం కూడిక;
          ఒకవేళ (ckd_కూడు (&కూడిక, current_niceness, tmp) || కూడిక < MIN_ADJUSTMENT)
            adjustment = MIN_ADJUSTMENT - current_niceness;
          లేకపోతే
            adjustment = tmp;
        }
      లేకపోతే
        {
          పూర్ణాంకం కూడిక;
          ఒకవేళ (ckd_కూడు (&కూడిక, current_niceness, tmp) || MAX_ADJUSTMENT < కూడిక)
            adjustment = MAX_ADJUSTMENT - current_niceness;
          లేకపోతే
            adjustment = tmp;
        }
#లేకపోతే
      adjustment = MAX (MIN_ADJUSTMENT, MIN (tmp, MAX_ADJUSTMENT));
#అయితే_ముగింపు
    }

  ఒకవేళ (i == argc)
    {
      ఒకవేళ (adjustment_given)
        {
          error (0, 0, _("a command must be given with an adjustment"));
          usage (EXIT_CANCELED);
        }
      /* No command given; print the niceness.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      ఒకవేళ (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      రాయి ("%d\n", current_niceness);
      తిరిగి EXIT_SUCCESS;
    }

  errno = 0;
#ఒకవేళ HAVE_NICE
  ok = (nice (adjustment) != -1 || errno == 0);
#లేకపోతే
  current_niceness = GET_NICENESS ();
  ఒకవేళ (current_niceness == -1 && errno != 0)
    error (EXIT_CANCELED, errno, _("cannot get niceness"));
  ok = (setpriority (PRIO_PROCESS, 0, current_niceness + adjustment) == 0);
#అయితే_ముగింపు
  ఒకవేళ (!ok)
    {
      error (perm_related_errno (errno) ? 0
             : EXIT_CANCELED, errno, _("cannot set niceness"));
      /* error() flushes stderr, but does not check for write failure.
         Normally, we would catch this via our atexit() hook of
         close_stdout, but execvp() gets in the way.  If stderr
         encountered a write failure, there is no need to try calling
         error() again.  */
      ఒకవేళ (ferror (stderr))
        తిరిగి EXIT_CANCELED;
    }

  execvp (argv[i], &argv[i]);

  పూర్ణాంకం exit_స్థితి = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[i]));
  తిరిగి exit_స్థితి;
}
