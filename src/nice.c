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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"

#ಆದರೆ ! HAVE_NICE
/* Include this after "system.h" so we're sure to have definitions
   (from time.h or sys/time.h) required for e.g. the ru_utime member.  */
# include <sys/resource.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "nice"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

#ಆದರೆ HAVE_NICE
# define GET_NICENESS() nice (0)
#ಇಲ್ಲದಿದ್ದರೆ
# define GET_NICENESS() getpriority (PRIO_PROCESS, 0)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NZERO
# define NZERO 20
#ಆದರೆ_ಅಂತ್ಯ

/* This is required for Darwin Kernel Version 7.7.0.  */
#ಆದರೆ NZERO == 0
# undef  NZERO
# define NZERO 20
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"adjustment", required_ವಾದ, ಖಾಲಿ, 'n'},
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
      ಬರೆ (_("Usage: %s [OPTION] [COMMAND [ARG]...]\n"), ಕಾರ್ಯಕ್ರಮ_name);
      ಬರೆ (_("\
ಸತತ COMMAND with an adjusted niceness, which affects process scheduling.\n\
With no COMMAND, print the current niceness.  Niceness ಮೌಲ್ಯಗಳು range from\n\
%d (most favorable ಗಮ್ಯ the process) ಗಮ್ಯ %d (ಕನಿಷ್ಠ favorable ಗಮ್ಯ the process).\n\
"),
              - NZERO, NZERO - 1);

      emit_mandatory_arg_note ();

      oputs (_("\
  -n, --adjustment=N\n\
         ಕೂಡಿಸು integer N ಗಮ್ಯ the niceness (ಪೂರ್ವನಿಯೋಜಿತ 10)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      ಬರೆ (USAGE_BUILTIN_WARNING, ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_exec_ಸ್ಥಿತಿ (ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಸ್ಥಿರ bool
perm_related_errno (ಪೂರ್ಣಾಂಕ err)
{
  ಹಿಂತಿರುಗಿ err == EACCES || err == EPERM;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ current_niceness;
  ಪೂರ್ಣಾಂಕ adjustment = 10;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *adjustment_given = ಖಾಲಿ;
  bool ok;
  ಪೂರ್ಣಾಂಕ i;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ಗಾಗಿ (i = 1; i < argc; /*ಖಾಲಿಗಳು*/)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s = argv[i];

      ಆದರೆ (s[0] == '-' && c_isdigit (s[1 + (s[1] == '-' || s[1] == '+')]))
        {
          adjustment_given = s + 1;
          ++i;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಪೂರ್ಣಾಂಕ c;
          ಪೂರ್ಣಾಂಕ fake_argc = argc - (i - 1);
          ಅಕ್ಷರ **fake_argv = argv + (i - 1);

          /* Ensure that any getopt diagnostics use the right name.  */
          fake_argv[0] = argv[0];

          /* Initialize getopt_long's internal state.  */
          optind = 0;

          c = getopt_long (fake_argc, fake_argv, "+n:", longopts, ಖಾಲಿ);
          i += optind - 1;

          ಆಯ್ಕೆ (c)
            {
            ಸ್ಥಿತಿ 'n':
              adjustment_given = optarg;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ -1:
              ನಿಲ್ಲಿಸು;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

            ಪೂರ್ವನಿಯೋಜಿತ:
              usage (EXIT_CANCELED);
              ನಿಲ್ಲಿಸು;
            }

          ಆದರೆ (c == -1)
            ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (adjustment_given)
    {
      /* If the requested adjustment is outside the valid range,
         silently bring it to just within range; this mimics what
         "setpriority" and "nice" do.  */
#ಆದರೆ (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) only supported 0 to (2 * NZERO - 2) niceness
         until glibc 2.43.  */
      ಎಣಿಕೆ { MIN_ADJUSTMENT = 0, MAX_ADJUSTMENT = 2 * NZERO - 2 };
#ಇಲ್ಲದಿದ್ದರೆ
      ಎಣಿಕೆ { MIN_ADJUSTMENT = 1 - 2 * NZERO, MAX_ADJUSTMENT = 2 * NZERO - 1 };
#ಆದರೆ_ಅಂತ್ಯ
      ದೀರ್ಘ ಪೂರ್ಣಾಂಕ tmp;
      ಆದರೆ (LONGINT_ಅತಿಪ್ರವಾಹ < xstrtol (adjustment_given, ಖಾಲಿ, 10, &tmp, ""))
        error (EXIT_CANCELED, 0, _("invalid adjustment %s"),
               quote (adjustment_given));
#ಆದರೆ (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) also did not clamp the new niceness into the
         supported range until glibc 2.43.
         See <https://sourceware.org/PR33614>.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      ಆದರೆ (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      ಆದರೆ (tmp < 0)
        {
          ಪೂರ್ಣಾಂಕ ಮೊತ್ತ;
          ಆದರೆ (ckd_ಕೂಡಿಸು (&ಮೊತ್ತ, current_niceness, tmp) || ಮೊತ್ತ < MIN_ADJUSTMENT)
            adjustment = MIN_ADJUSTMENT - current_niceness;
          ಇಲ್ಲದಿದ್ದರೆ
            adjustment = tmp;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಪೂರ್ಣಾಂಕ ಮೊತ್ತ;
          ಆದರೆ (ckd_ಕೂಡಿಸು (&ಮೊತ್ತ, current_niceness, tmp) || MAX_ADJUSTMENT < ಮೊತ್ತ)
            adjustment = MAX_ADJUSTMENT - current_niceness;
          ಇಲ್ಲದಿದ್ದರೆ
            adjustment = tmp;
        }
#ಇಲ್ಲದಿದ್ದರೆ
      adjustment = MAX (MIN_ADJUSTMENT, MIN (tmp, MAX_ADJUSTMENT));
#ಆದರೆ_ಅಂತ್ಯ
    }

  ಆದರೆ (i == argc)
    {
      ಆದರೆ (adjustment_given)
        {
          error (0, 0, _("a command must be given with an adjustment"));
          usage (EXIT_CANCELED);
        }
      /* No command given; print the niceness.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      ಆದರೆ (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      ಬರೆ ("%d\n", current_niceness);
      ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
    }

  errno = 0;
#ಆದರೆ HAVE_NICE
  ok = (nice (adjustment) != -1 || errno == 0);
#ಇಲ್ಲದಿದ್ದರೆ
  current_niceness = GET_NICENESS ();
  ಆದರೆ (current_niceness == -1 && errno != 0)
    error (EXIT_CANCELED, errno, _("cannot get niceness"));
  ok = (setpriority (PRIO_PROCESS, 0, current_niceness + adjustment) == 0);
#ಆದರೆ_ಅಂತ್ಯ
  ಆದರೆ (!ok)
    {
      error (perm_related_errno (errno) ? 0
             : EXIT_CANCELED, errno, _("cannot set niceness"));
      /* error() flushes stderr, but does not check for write failure.
         Normally, we would catch this via our atexit() hook of
         close_stdout, but execvp() gets in the way.  If stderr
         encountered a write failure, there is no need to try calling
         error() again.  */
      ಆದರೆ (ferror (stderr))
        ಹಿಂತಿರುಗಿ EXIT_CANCELED;
    }

  execvp (argv[i], &argv[i]);

  ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[i]));
  ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}
