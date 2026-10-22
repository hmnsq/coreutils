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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"

#എങ്കിൽ ! HAVE_NICE
/* Include this after "system.h" so we're sure to have definitions
   (from time.h or sys/time.h) required for e.g. the ru_utime member.  */
# include <sys/resource.h>
#endif

#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nice"

#define AUTHORS proper_name ("David MacKenzie")

#എങ്കിൽ HAVE_NICE
# define GET_NICENESS() nice (0)
#അല്ലെങ്കിൽ
# define GET_NICENESS() getpriority (PRIO_PROCESS, 0)
#endif

#ifndef NZERO
# define NZERO 20
#endif

/* This is required for Darwin Kernel Version 7.7.0.  */
#എങ്കിൽ NZERO == 0
# undef  NZERO
# define NZERO 20
#endif

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"adjustment", required_argument, ശൂന്യ, 'n'},
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
      printf (_("Usage: %s [OPTION] [COMMAND [ARG]...]\n"), program_name);
      printf (_("\
Run COMMAND with an adjusted niceness, which affects process scheduling.\n\
With no COMMAND, print the current niceness.  Niceness മൂല്യങ്ങൾ range from\n\
%d (most favorable to the process) to %d (least favorable to the process).\n\
"),
              - NZERO, NZERO - 1);

      emit_mandatory_arg_note ();

      oputs (_("\
  -n, --adjustment=N\n\
         add integer N to the niceness (സ്ഥിരസ്ഥിതി 10)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

സ്ഥിതം bool
perm_related_errno (പൂർണ്ണസംഖ്യ err)
{
  തിരികെ err == EACCES || err == EPERM;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ current_niceness;
  പൂർണ്ണസംഖ്യ adjustment = 10;
  അക്ഷരം സ്ഥിരം *adjustment_given = ശൂന്യ;
  bool ok;
  പൂർണ്ണസംഖ്യ i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  വേണ്ടി (i = 1; i < argc; /* empty */)
    {
      അക്ഷരം സ്ഥിരം *s = argv[i];

      എങ്കിൽ (s[0] == '-' && c_isdigit (s[1 + (s[1] == '-' || s[1] == '+')]))
        {
          adjustment_given = s + 1;
          ++i;
        }
      അല്ലെങ്കിൽ
        {
          പൂർണ്ണസംഖ്യ c;
          പൂർണ്ണസംഖ്യ fake_argc = argc - (i - 1);
          അക്ഷരം **fake_argv = argv + (i - 1);

          /* Ensure that any getopt diagnostics use the right name.  */
          fake_argv[0] = argv[0];

          /* Initialize getopt_long's internal state.  */
          optind = 0;

          c = getopt_long (fake_argc, fake_argv, "+n:", longopts, ശൂന്യ);
          i += optind - 1;

          തിരഞ്ഞെടുപ്പ് (c)
            {
            അവസ്ഥ 'n':
              adjustment_given = optarg;
              നിർത്തുക;

            അവസ്ഥ -1:
              നിർത്തുക;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            സ്ഥിരസ്ഥിതി:
              usage (EXIT_CANCELED);
              നിർത്തുക;
            }

          എങ്കിൽ (c == -1)
            നിർത്തുക;
        }
    }

  എങ്കിൽ (adjustment_given)
    {
      /* If the requested adjustment is outside the valid range,
         silently bring it to just within range; this mimics what
         "setpriority" and "nice" do.  */
#എങ്കിൽ (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) only supported 0 to (2 * NZERO - 2) niceness
         until glibc 2.43.  */
      എണ്ണൽ { MIN_ADJUSTMENT = 0, MAX_ADJUSTMENT = 2 * NZERO - 2 };
#അല്ലെങ്കിൽ
      എണ്ണൽ { MIN_ADJUSTMENT = 1 - 2 * NZERO, MAX_ADJUSTMENT = 2 * NZERO - 1 };
#endif
      long പൂർണ്ണസംഖ്യ tmp;
      എങ്കിൽ (LONGINT_OVERFLOW < xstrtol (adjustment_given, ശൂന്യ, 10, &tmp, ""))
        error (EXIT_CANCELED, 0, _("invalid adjustment %s"),
               quote (adjustment_given));
#എങ്കിൽ (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) also did not clamp the new niceness into the
         supported range until glibc 2.43.
         See <https://sourceware.org/PR33614>.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      എങ്കിൽ (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      എങ്കിൽ (tmp < 0)
        {
          പൂർണ്ണസംഖ്യ sum;
          എങ്കിൽ (ckd_add (&sum, current_niceness, tmp) || sum < MIN_ADJUSTMENT)
            adjustment = MIN_ADJUSTMENT - current_niceness;
          അല്ലെങ്കിൽ
            adjustment = tmp;
        }
      അല്ലെങ്കിൽ
        {
          പൂർണ്ണസംഖ്യ sum;
          എങ്കിൽ (ckd_add (&sum, current_niceness, tmp) || MAX_ADJUSTMENT < sum)
            adjustment = MAX_ADJUSTMENT - current_niceness;
          അല്ലെങ്കിൽ
            adjustment = tmp;
        }
#അല്ലെങ്കിൽ
      adjustment = MAX (MIN_ADJUSTMENT, MIN (tmp, MAX_ADJUSTMENT));
#endif
    }

  എങ്കിൽ (i == argc)
    {
      എങ്കിൽ (adjustment_given)
        {
          error (0, 0, _("a command must be given with an adjustment"));
          usage (EXIT_CANCELED);
        }
      /* No command given; print the niceness.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      എങ്കിൽ (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      printf ("%d\n", current_niceness);
      തിരികെ EXIT_SUCCESS;
    }

  errno = 0;
#എങ്കിൽ HAVE_NICE
  ok = (nice (adjustment) != -1 || errno == 0);
#അല്ലെങ്കിൽ
  current_niceness = GET_NICENESS ();
  എങ്കിൽ (current_niceness == -1 && errno != 0)
    error (EXIT_CANCELED, errno, _("cannot get niceness"));
  ok = (setpriority (PRIO_PROCESS, 0, current_niceness + adjustment) == 0);
#endif
  എങ്കിൽ (!ok)
    {
      error (perm_related_errno (errno) ? 0
             : EXIT_CANCELED, errno, _("cannot set niceness"));
      /* error() flushes stderr, but does not check for write failure.
         Normally, we would catch this via our atexit() hook of
         close_stdout, but execvp() gets in the way.  If stderr
         encountered a write failure, there is no need to try calling
         error() again.  */
      എങ്കിൽ (ferror (stderr))
        തിരികെ EXIT_CANCELED;
    }

  execvp (argv[i], &argv[i]);

  പൂർണ്ണസംഖ്യ exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[i]));
  തിരികെ exit_status;
}
