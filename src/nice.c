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

#include <config.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>

#include "system.h"

#ᬬᬾᬦ᭄ ! HAVE_NICE
/* Include this after "system.h" so we're sure to have definitions
   (from time.h or sys/time.h) required for e.g. the ru_utime member.  */
# include <sys/resource.h>
#endif

#include "c-ctype.h"
#include "quote.h"
#include "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nice"

#define AUTHORS proper_name ("David MacKenzie")

#ᬬᬾᬦ᭄ HAVE_NICE
# define GET_NICENESS() nice (0)
#ᬮᬶᬬ
# define GET_NICENESS() getpriority (PRIO_PROCESS, 0)
#endif

#ifndef NZERO
# define NZERO 20
#endif

/* This is required for Darwin Kernel Version 7.7.0.  */
#ᬬᬾᬦ᭄ NZERO == 0
# undef  NZERO
# define NZERO 20
#endif

static sa_nracanaa option const longopts[] =
{
  {"adjustment", required_argument, NULL, 'n'},
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
      printf (_("Usage: %s [OPTION] [COMMAND [ARG]...]\n"), program_name);
      printf (_("\
Run COMMAND with an adjusted niceness, which affects process scheduling.\n\
With no COMMAND, print the current niceness.  Niceness values range from\n\
%d (most favorable to the process) to %d (least favorable to the process).\n\
"),
              - NZERO, NZERO - 1);

      emit_mandatory_arg_note ();

      oputs (_("\
  -n, --adjustment=N\n\
         add integer N to the niceness (default 10)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

static bool
perm_related_errno (puur_naa_nka err)
{
  ᬯᬮᬶ err == EACCES || err == EPERM;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  puur_naa_nka current_niceness;
  puur_naa_nka adjustment = 10;
  var_na const *adjustment_given = NULL;
  bool ok;
  puur_naa_nka i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  for (i = 1; i < argc; /* empty */)
    {
      var_na const *s = argv[i];

      ᬬᬾᬦ᭄ (s[0] == '-' && c_isdigit (s[1 + (s[1] == '-' || s[1] == '+')]))
        {
          adjustment_given = s + 1;
          ++i;
        }
      ᬮᬶᬬ
        {
          puur_naa_nka c;
          puur_naa_nka fake_argc = argc - (i - 1);
          var_na **fake_argv = argv + (i - 1);

          /* Ensure that any getopt diagnostics use the right name.  */
          fake_argv[0] = argv[0];

          /* Initialize getopt_long's internal state.  */
          optind = 0;

          c = getopt_long (fake_argc, fake_argv, "+n:", longopts, NULL);
          i += optind - 1;

          switch (c)
            {
            pak_sa 'n':
              adjustment_given = optarg;
              bha_nga;

            pak_sa -1:
              bha_nga;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            default:
              usage (EXIT_CANCELED);
              bha_nga;
            }

          ᬬᬾᬦ᭄ (c == -1)
            bha_nga;
        }
    }

  ᬬᬾᬦ᭄ (adjustment_given)
    {
      /* If the requested adjustment is outside the valid range,
         silently bring it to just within range; this mimics what
         "setpriority" and "nice" do.  */
#ᬬᬾᬦ᭄ (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) only supported 0 to (2 * NZERO - 2) niceness
         until glibc 2.43.  */
      ga_nanaa { MIN_ADJUSTMENT = 0, MAX_ADJUSTMENT = 2 * NZERO - 2 };
#ᬮᬶᬬ
      ga_nanaa { MIN_ADJUSTMENT = 1 - 2 * NZERO, MAX_ADJUSTMENT = 2 * NZERO - 1 };
#endif
      long puur_naa_nka tmp;
      ᬬᬾᬦ᭄ (LONGINT_OVERFLOW < xstrtol (adjustment_given, NULL, 10, &tmp, ""))
        error (EXIT_CANCELED, 0, _("invalid adjustment %s"),
               quote (adjustment_given));
#ᬬᬾᬦ᭄ (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) also did not clamp the new niceness into the
         supported range until glibc 2.43.
         See <https://sourceware.org/PR33614>.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      ᬬᬾᬦ᭄ (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      ᬬᬾᬦ᭄ (tmp < 0)
        {
          puur_naa_nka sum;
          ᬬᬾᬦ᭄ (ckd_add (&sum, current_niceness, tmp) || sum < MIN_ADJUSTMENT)
            adjustment = MIN_ADJUSTMENT - current_niceness;
          ᬮᬶᬬ
            adjustment = tmp;
        }
      ᬮᬶᬬ
        {
          puur_naa_nka sum;
          ᬬᬾᬦ᭄ (ckd_add (&sum, current_niceness, tmp) || MAX_ADJUSTMENT < sum)
            adjustment = MAX_ADJUSTMENT - current_niceness;
          ᬮᬶᬬ
            adjustment = tmp;
        }
#ᬮᬶᬬ
      adjustment = MAX (MIN_ADJUSTMENT, MIN (tmp, MAX_ADJUSTMENT));
#endif
    }

  ᬬᬾᬦ᭄ (i == argc)
    {
      ᬬᬾᬦ᭄ (adjustment_given)
        {
          error (0, 0, _("a command must be given with an adjustment"));
          usage (EXIT_CANCELED);
        }
      /* No command given; print the niceness.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      ᬬᬾᬦ᭄ (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      printf ("%d\n", current_niceness);
      ᬯᬮᬶ EXIT_SUCCESS;
    }

  errno = 0;
#ᬬᬾᬦ᭄ HAVE_NICE
  ok = (nice (adjustment) != -1 || errno == 0);
#ᬮᬶᬬ
  current_niceness = GET_NICENESS ();
  ᬬᬾᬦ᭄ (current_niceness == -1 && errno != 0)
    error (EXIT_CANCELED, errno, _("cannot get niceness"));
  ok = (setpriority (PRIO_PROCESS, 0, current_niceness + adjustment) == 0);
#endif
  ᬬᬾᬦ᭄ (!ok)
    {
      error (perm_related_errno (errno) ? 0
             : EXIT_CANCELED, errno, _("cannot set niceness"));
      /* error() flushes stderr, but does not check for write failure.
         Normally, we would catch this via our atexit() hook of
         close_stdout, but execvp() gets in the way.  If stderr
         encountered a write failure, there is no need to try calling
         error() again.  */
      ᬬᬾᬦ᭄ (ferror (stderr))
        ᬯᬮᬶ EXIT_CANCELED;
    }

  execvp (argv[i], &argv[i]);

  puur_naa_nka exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[i]));
  ᬯᬮᬶ exit_status;
}
