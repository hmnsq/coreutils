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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"

#إذا ! HAVE_NICE
/* Include this after "system.h" so we're sure to have definitions
   (from time.h or sys/time.h) required for e.g. the ru_utime member.  */
# include <sys/resource.h>
#جے_انت

#ضمن "c-ctype.h"
#ضمن "quote.h"
#ضمن "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "nice"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

#إذا HAVE_NICE
# define GET_NICENESS() nice (0)
#وإلا
# define GET_NICENESS() getpriority (PRIO_PROCESS, 0)
#جے_انت

#جے_بنا_تعریف NZERO
# define NZERO 20
#جے_انت

/* This is required for Darwin Kernel Version 7.7.0.  */
#إذا NZERO == 0
# undef  NZERO
# define NZERO 20
#جے_انت

ساكن بنية option ثابت longopts[] =
{
  {"adjustment", required_دلیل, عدم, 'n'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [OPTION] [COMMAND [ARG]...]\n"), پروگرام_name);
      لکھو (_("\
لگاتار COMMAND with an adjusted niceness, which affects process scheduling.\n\
With no COMMAND, print the current niceness.  Niceness اقدار range from\n\
%d (most favorable منزل the process) منزل %d (گھٹ_توں_گھٹ favorable منزل the process).\n\
"),
              - NZERO, NZERO - 1);

      emit_mandatory_arg_note ();

      oputs (_("\
  -n, --adjustment=N\n\
         جوڑو integer N منزل the niceness (افتراضي 10)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      لکھو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
      emit_exec_حالت (پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

ساكن bool
perm_related_errno (صحيح err)
{
  أرجع err == EACCES || err == EPERM;
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  صحيح current_niceness;
  صحيح adjustment = 10;
  حرف ثابت *adjustment_given = عدم;
  bool ok;
  صحيح i;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  لكل (i = 1; i < argc; /*خالی_تھاواں*/)
    {
      حرف ثابت *s = argv[i];

      إذا (s[0] == '-' && c_isdigit (s[1 + (s[1] == '-' || s[1] == '+')]))
        {
          adjustment_given = s + 1;
          ++i;
        }
      وإلا
        {
          صحيح c;
          صحيح fake_argc = argc - (i - 1);
          حرف **fake_argv = argv + (i - 1);

          /* Ensure that any getopt diagnostics use the right name.  */
          fake_argv[0] = argv[0];

          /* Initialize getopt_long's internal state.  */
          optind = 0;

          c = getopt_long (fake_argc, fake_argv, "+n:", longopts, عدم);
          i += optind - 1;

          بدل (c)
            {
            حالة 'n':
              adjustment_given = optarg;
              اكسر;

            حالة -1:
              اكسر;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

            افتراضي:
              usage (EXIT_CANCELED);
              اكسر;
            }

          إذا (c == -1)
            اكسر;
        }
    }

  إذا (adjustment_given)
    {
      /* If the requested adjustment is outside the valid range,
         silently bring it to just within range; this mimics what
         "setpriority" and "nice" do.  */
#إذا (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) only supported 0 to (2 * NZERO - 2) niceness
         until glibc 2.43.  */
      تعداد { MIN_ADJUSTMENT = 0, MAX_ADJUSTMENT = 2 * NZERO - 2 };
#وإلا
      تعداد { MIN_ADJUSTMENT = 1 - 2 * NZERO, MAX_ADJUSTMENT = 2 * NZERO - 1 };
#جے_انت
      لما صحيح tmp;
      إذا (LONGINT_حد_توں_ودھ < xstrtol (adjustment_given, عدم, 10, &tmp, ""))
        error (EXIT_CANCELED, 0, _("invalid adjustment %s"),
               quote (adjustment_given));
#إذا (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) also did not clamp the new niceness into the
         supported range until glibc 2.43.
         See <https://sourceware.org/PR33614>.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      إذا (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      إذا (tmp < 0)
        {
          صحيح جوڑ;
          إذا (ckd_جوڑو (&جوڑ, current_niceness, tmp) || جوڑ < MIN_ADJUSTMENT)
            adjustment = MIN_ADJUSTMENT - current_niceness;
          وإلا
            adjustment = tmp;
        }
      وإلا
        {
          صحيح جوڑ;
          إذا (ckd_جوڑو (&جوڑ, current_niceness, tmp) || MAX_ADJUSTMENT < جوڑ)
            adjustment = MAX_ADJUSTMENT - current_niceness;
          وإلا
            adjustment = tmp;
        }
#وإلا
      adjustment = MAX (MIN_ADJUSTMENT, MIN (tmp, MAX_ADJUSTMENT));
#جے_انت
    }

  إذا (i == argc)
    {
      إذا (adjustment_given)
        {
          error (0, 0, _("a command must be given with an adjustment"));
          usage (EXIT_CANCELED);
        }
      /* No command given; print the niceness.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      إذا (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      لکھو ("%d\n", current_niceness);
      أرجع EXIT_SUCCESS;
    }

  errno = 0;
#إذا HAVE_NICE
  ok = (nice (adjustment) != -1 || errno == 0);
#وإلا
  current_niceness = GET_NICENESS ();
  إذا (current_niceness == -1 && errno != 0)
    error (EXIT_CANCELED, errno, _("cannot get niceness"));
  ok = (setpriority (PRIO_PROCESS, 0, current_niceness + adjustment) == 0);
#جے_انت
  إذا (!ok)
    {
      error (perm_related_errno (errno) ? 0
             : EXIT_CANCELED, errno, _("cannot set niceness"));
      /* error() flushes stderr, but does not check for write failure.
         Normally, we would catch this via our atexit() hook of
         close_stdout, but execvp() gets in the way.  If stderr
         encountered a write failure, there is no need to try calling
         error() again.  */
      إذا (ferror (stderr))
        أرجع EXIT_CANCELED;
    }

  execvp (argv[i], &argv[i]);

  صحيح exit_حالت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[i]));
  أرجع exit_حالت;
}
