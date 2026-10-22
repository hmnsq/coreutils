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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"

#જો ! HAVE_NICE
/* Include this after "system.h" so we're sure to have definitions
   (from time.h or sys/time.h) required for e.g. the ru_utime member.  */
# include <sys/resource.h>
#endif

#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nice"

#define AUTHORS proper_name ("David MacKenzie")

#જો HAVE_NICE
# define GET_NICENESS() nice (0)
#નહીંતર
# define GET_NICENESS() getpriority (PRIO_PROCESS, 0)
#endif

#ifndef NZERO
# define NZERO 20
#endif

/* This is required for Darwin Kernel Version 7.7.0.  */
#જો NZERO == 0
# undef  NZERO
# define NZERO 20
#endif

સ્થિર સંરચના option અચલ longopts[] =
{
  {"adjustment", required_argument, ખાલી, 'n'},
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
      printf (_("Usage: %s [OPTION] [COMMAND [ARG]...]\n"), program_name);
      printf (_("\
Run COMMAND with an adjusted niceness, which affects process scheduling.\n\
With no COMMAND, print the current niceness.  Niceness કિંમતો range from\n\
%d (most favorable to the process) to %d (least favorable to the process).\n\
"),
              - NZERO, NZERO - 1);

      emit_mandatory_arg_note ();

      oputs (_("\
  -n, --adjustment=N\n\
         add integer N to the niceness (મૂળભૂત 10)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

સ્થિર bool
perm_related_errno (પૂર્ણાંક err)
{
  પાછા err == EACCES || err == EPERM;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક current_niceness;
  પૂર્ણાંક adjustment = 10;
  અક્ષર અચલ *adjustment_given = ખાલી;
  bool ok;
  પૂર્ણાંક i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  માટે (i = 1; i < argc; /* empty */)
    {
      અક્ષર અચલ *s = argv[i];

      જો (s[0] == '-' && c_isdigit (s[1 + (s[1] == '-' || s[1] == '+')]))
        {
          adjustment_given = s + 1;
          ++i;
        }
      નહીંતર
        {
          પૂર્ણાંક c;
          પૂર્ણાંક fake_argc = argc - (i - 1);
          અક્ષર **fake_argv = argv + (i - 1);

          /* Ensure that any getopt diagnostics use the right name.  */
          fake_argv[0] = argv[0];

          /* Initialize getopt_long's internal state.  */
          optind = 0;

          c = getopt_long (fake_argc, fake_argv, "+n:", longopts, ખાલી);
          i += optind - 1;

          પસંદગી (c)
            {
            સ્થિતિ 'n':
              adjustment_given = optarg;
              તોડો;

            સ્થિતિ -1:
              તોડો;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            મૂળભૂત:
              usage (EXIT_CANCELED);
              તોડો;
            }

          જો (c == -1)
            તોડો;
        }
    }

  જો (adjustment_given)
    {
      /* If the requested adjustment is outside the valid range,
         silently bring it to just within range; this mimics what
         "setpriority" and "nice" do.  */
#જો (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) only supported 0 to (2 * NZERO - 2) niceness
         until glibc 2.43.  */
      ગણના { MIN_ADJUSTMENT = 0, MAX_ADJUSTMENT = 2 * NZERO - 2 };
#નહીંતર
      ગણના { MIN_ADJUSTMENT = 1 - 2 * NZERO, MAX_ADJUSTMENT = 2 * NZERO - 1 };
#endif
      long પૂર્ણાંક tmp;
      જો (LONGINT_OVERFLOW < xstrtol (adjustment_given, ખાલી, 10, &tmp, ""))
        error (EXIT_CANCELED, 0, _("invalid adjustment %s"),
               quote (adjustment_given));
#જો (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) also did not clamp the new niceness into the
         supported range until glibc 2.43.
         See <https://sourceware.org/PR33614>.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      જો (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      જો (tmp < 0)
        {
          પૂર્ણાંક sum;
          જો (ckd_add (&sum, current_niceness, tmp) || sum < MIN_ADJUSTMENT)
            adjustment = MIN_ADJUSTMENT - current_niceness;
          નહીંતર
            adjustment = tmp;
        }
      નહીંતર
        {
          પૂર્ણાંક sum;
          જો (ckd_add (&sum, current_niceness, tmp) || MAX_ADJUSTMENT < sum)
            adjustment = MAX_ADJUSTMENT - current_niceness;
          નહીંતર
            adjustment = tmp;
        }
#નહીંતર
      adjustment = MAX (MIN_ADJUSTMENT, MIN (tmp, MAX_ADJUSTMENT));
#endif
    }

  જો (i == argc)
    {
      જો (adjustment_given)
        {
          error (0, 0, _("a command must be given with an adjustment"));
          usage (EXIT_CANCELED);
        }
      /* No command given; print the niceness.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      જો (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      printf ("%d\n", current_niceness);
      પાછા EXIT_SUCCESS;
    }

  errno = 0;
#જો HAVE_NICE
  ok = (nice (adjustment) != -1 || errno == 0);
#નહીંતર
  current_niceness = GET_NICENESS ();
  જો (current_niceness == -1 && errno != 0)
    error (EXIT_CANCELED, errno, _("cannot get niceness"));
  ok = (setpriority (PRIO_PROCESS, 0, current_niceness + adjustment) == 0);
#endif
  જો (!ok)
    {
      error (perm_related_errno (errno) ? 0
             : EXIT_CANCELED, errno, _("cannot set niceness"));
      /* error() flushes stderr, but does not check for write failure.
         Normally, we would catch this via our atexit() hook of
         close_stdout, but execvp() gets in the way.  If stderr
         encountered a write failure, there is no need to try calling
         error() again.  */
      જો (ferror (stderr))
        પાછા EXIT_CANCELED;
    }

  execvp (argv[i], &argv[i]);

  પૂર્ણાંક exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[i]));
  પાછા exit_status;
}
