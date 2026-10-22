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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਜੇ ! HAVE_NICE
/* Include this after "system.h" so we're sure to have definitions
   (from time.h or sys/time.h) required for e.g. the ru_utime member.  */
# include <sys/resource.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "nice"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

#ਜੇ HAVE_NICE
# define GET_NICENESS() nice (0)
#ਨਹੀਂਤਾਂ
# define GET_NICENESS() getpriority (PRIO_PROCESS, 0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NZERO
# define NZERO 20
#ਜੇ_ਅੰਤ

/* This is required for Darwin Kernel Version 7.7.0.  */
#ਜੇ NZERO == 0
# undef  NZERO
# define NZERO 20
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"adjustment", required_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION] [COMMAND [ARG]...]\n"), ਪ੍ਰੋਗਰਾਮ_name);
      ਲਿਖੋ (_("\
ਲੜੀਵਾਰ COMMAND with an adjusted niceness, which affects process scheduling.\n\
With no COMMAND, print the current niceness.  Niceness ਮੁੱਲਾਂ range from\n\
%d (most favorable ਮੰਜ਼ਿਲ the process) ਮੰਜ਼ਿਲ %d (ਘੱਟ_ਤੋਂ_ਘੱਟ favorable ਮੰਜ਼ਿਲ the process).\n\
"),
              - NZERO, NZERO - 1);

      emit_mandatory_arg_note ();

      oputs (_("\
  -n, --adjustment=N\n\
         ਜੋੜੋ integer N ਮੰਜ਼ਿਲ the niceness (ਮੂਲ 10)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      ਲਿਖੋ (USAGE_BUILTIN_WARNING, ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_exec_ਸਥਿਤੀ (ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਸਥਿਰ bool
perm_related_errno (ਪੂਰਨ err)
{
  ਵਾਪਸ err == EACCES || err == EPERM;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ current_niceness;
  ਪੂਰਨ adjustment = 10;
  ਅੱਖਰ ਅਚਲ *adjustment_given = ਨਿਰਾ;
  bool ok;
  ਪੂਰਨ i;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ਲਈ (i = 1; i < argc; /*ਖਾਲੀ_ਥਾਵਾਂ*/)
    {
      ਅੱਖਰ ਅਚਲ *s = argv[i];

      ਜੇ (s[0] == '-' && c_isdigit (s[1 + (s[1] == '-' || s[1] == '+')]))
        {
          adjustment_given = s + 1;
          ++i;
        }
      ਨਹੀਂਤਾਂ
        {
          ਪੂਰਨ c;
          ਪੂਰਨ fake_argc = argc - (i - 1);
          ਅੱਖਰ **fake_argv = argv + (i - 1);

          /* Ensure that any getopt diagnostics use the right name.  */
          fake_argv[0] = argv[0];

          /* Initialize getopt_long's internal state.  */
          optind = 0;

          c = getopt_long (fake_argc, fake_argv, "+n:", longopts, ਨਿਰਾ);
          i += optind - 1;

          ਚੋਣ (c)
            {
            ਹਾਲਤ 'n':
              adjustment_given = optarg;
              ਰੋਕੋ;

            ਹਾਲਤ -1:
              ਰੋਕੋ;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

            ਮੂਲ:
              usage (EXIT_CANCELED);
              ਰੋਕੋ;
            }

          ਜੇ (c == -1)
            ਰੋਕੋ;
        }
    }

  ਜੇ (adjustment_given)
    {
      /* If the requested adjustment is outside the valid range,
         silently bring it to just within range; this mimics what
         "setpriority" and "nice" do.  */
#ਜੇ (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) only supported 0 to (2 * NZERO - 2) niceness
         until glibc 2.43.  */
      ਗਿਣਤੀ { MIN_ADJUSTMENT = 0, MAX_ADJUSTMENT = 2 * NZERO - 2 };
#ਨਹੀਂਤਾਂ
      ਗਿਣਤੀ { MIN_ADJUSTMENT = 1 - 2 * NZERO, MAX_ADJUSTMENT = 2 * NZERO - 1 };
#ਜੇ_ਅੰਤ
      ਲੰਮਾ ਪੂਰਨ tmp;
      ਜੇ (LONGINT_ਉਛਾਲ < xstrtol (adjustment_given, ਨਿਰਾ, 10, &tmp, ""))
        error (EXIT_CANCELED, 0, _("invalid adjustment %s"),
               quote (adjustment_given));
#ਜੇ (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) also did not clamp the new niceness into the
         supported range until glibc 2.43.
         See <https://sourceware.org/PR33614>.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      ਜੇ (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      ਜੇ (tmp < 0)
        {
          ਪੂਰਨ ਜੋੜ;
          ਜੇ (ckd_ਜੋੜੋ (&ਜੋੜ, current_niceness, tmp) || ਜੋੜ < MIN_ADJUSTMENT)
            adjustment = MIN_ADJUSTMENT - current_niceness;
          ਨਹੀਂਤਾਂ
            adjustment = tmp;
        }
      ਨਹੀਂਤਾਂ
        {
          ਪੂਰਨ ਜੋੜ;
          ਜੇ (ckd_ਜੋੜੋ (&ਜੋੜ, current_niceness, tmp) || MAX_ADJUSTMENT < ਜੋੜ)
            adjustment = MAX_ADJUSTMENT - current_niceness;
          ਨਹੀਂਤਾਂ
            adjustment = tmp;
        }
#ਨਹੀਂਤਾਂ
      adjustment = MAX (MIN_ADJUSTMENT, MIN (tmp, MAX_ADJUSTMENT));
#ਜੇ_ਅੰਤ
    }

  ਜੇ (i == argc)
    {
      ਜੇ (adjustment_given)
        {
          error (0, 0, _("a command must be given with an adjustment"));
          usage (EXIT_CANCELED);
        }
      /* No command given; print the niceness.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      ਜੇ (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      ਲਿਖੋ ("%d\n", current_niceness);
      ਵਾਪਸ EXIT_SUCCESS;
    }

  errno = 0;
#ਜੇ HAVE_NICE
  ok = (nice (adjustment) != -1 || errno == 0);
#ਨਹੀਂਤਾਂ
  current_niceness = GET_NICENESS ();
  ਜੇ (current_niceness == -1 && errno != 0)
    error (EXIT_CANCELED, errno, _("cannot get niceness"));
  ok = (setpriority (PRIO_PROCESS, 0, current_niceness + adjustment) == 0);
#ਜੇ_ਅੰਤ
  ਜੇ (!ok)
    {
      error (perm_related_errno (errno) ? 0
             : EXIT_CANCELED, errno, _("cannot set niceness"));
      /* error() flushes stderr, but does not check for write failure.
         Normally, we would catch this via our atexit() hook of
         close_stdout, but execvp() gets in the way.  If stderr
         encountered a write failure, there is no need to try calling
         error() again.  */
      ਜੇ (ferror (stderr))
        ਵਾਪਸ EXIT_CANCELED;
    }

  execvp (argv[i], &argv[i]);

  ਪੂਰਨ exit_ਸਥਿਤੀ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[i]));
  ਵਾਪਸ exit_ਸਥਿਤੀ;
}
