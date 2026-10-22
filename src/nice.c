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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"

#اگر ! HAVE_NICE
/* Include this after "system.h" so we're sure to have definitions
   (from time.h or sys/time.h) required for e.g. the ru_utime member.  */
# include <sys/resource.h>
#اگر_ختم

#شامل "c-ctype.h"
#شامل "quote.h"
#شامل "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "nice"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

#اگر HAVE_NICE
# define GET_NICENESS() nice (0)
#ورنہ
# define GET_NICENESS() getpriority (PRIO_PROCESS, 0)
#اگر_ختم

#اگر_غیر_تعریف_شدہ NZERO
# define NZERO 20
#اگر_ختم

/* This is required for Darwin Kernel Version 7.7.0.  */
#اگر NZERO == 0
# undef  NZERO
# define NZERO 20
#اگر_ختم

ساکن ساخت option ثابت longopts[] =
{
  {"adjustment", required_دلیل, تہی, 'n'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION] [COMMAND [ARG]...]\n"), پروگرام_name);
      لکھو (_("\
تسلسل COMMAND with an adjusted niceness, which affects process scheduling.\n\
With no COMMAND, print the current niceness.  Niceness اقدار range from\n\
%d (most favorable مقصد the process) مقصد %d (کم_سے_کم favorable مقصد the process).\n\
"),
              - NZERO, NZERO - 1);

      emit_mandatory_arg_note ();

      oputs (_("\
  -n, --adjustment=N\n\
         جوڑو integer N مقصد the niceness (طےشدہ 10)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      لکھو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
      emit_exec_حالت (پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

ساکن bool
perm_related_errno (صحیح err)
{
  واپس err == EACCES || err == EPERM;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح current_niceness;
  صحیح adjustment = 10;
  حرف ثابت *adjustment_given = تہی;
  bool ok;
  صحیح i;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  برائے (i = 1; i < argc; /*خالی_خانے*/)
    {
      حرف ثابت *s = argv[i];

      اگر (s[0] == '-' && c_isdigit (s[1 + (s[1] == '-' || s[1] == '+')]))
        {
          adjustment_given = s + 1;
          ++i;
        }
      ورنہ
        {
          صحیح c;
          صحیح fake_argc = argc - (i - 1);
          حرف **fake_argv = argv + (i - 1);

          /* Ensure that any getopt diagnostics use the right name.  */
          fake_argv[0] = argv[0];

          /* Initialize getopt_long's internal state.  */
          optind = 0;

          c = getopt_long (fake_argc, fake_argv, "+n:", longopts, تہی);
          i += optind - 1;

          انتخاب (c)
            {
            حالت 'n':
              adjustment_given = optarg;
              توڑو;

            حالت -1:
              توڑو;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

            طےشدہ:
              usage (EXIT_CANCELED);
              توڑو;
            }

          اگر (c == -1)
            توڑو;
        }
    }

  اگر (adjustment_given)
    {
      /* If the requested adjustment is outside the valid range,
         silently bring it to just within range; this mimics what
         "setpriority" and "nice" do.  */
#اگر (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) only supported 0 to (2 * NZERO - 2) niceness
         until glibc 2.43.  */
      شمار { MIN_ADJUSTMENT = 0, MAX_ADJUSTMENT = 2 * NZERO - 2 };
#ورنہ
      شمار { MIN_ADJUSTMENT = 1 - 2 * NZERO, MAX_ADJUSTMENT = 2 * NZERO - 1 };
#اگر_ختم
      طویل صحیح tmp;
      اگر (LONGINT_حد_سے_تجاوز < xstrtol (adjustment_given, تہی, 10, &tmp, ""))
        error (EXIT_CANCELED, 0, _("invalid adjustment %s"),
               quote (adjustment_given));
#اگر (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) also did not clamp the new niceness into the
         supported range until glibc 2.43.
         See <https://sourceware.org/PR33614>.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      اگر (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      اگر (tmp < 0)
        {
          صحیح مجموع;
          اگر (ckd_جوڑو (&مجموع, current_niceness, tmp) || مجموع < MIN_ADJUSTMENT)
            adjustment = MIN_ADJUSTMENT - current_niceness;
          ورنہ
            adjustment = tmp;
        }
      ورنہ
        {
          صحیح مجموع;
          اگر (ckd_جوڑو (&مجموع, current_niceness, tmp) || MAX_ADJUSTMENT < مجموع)
            adjustment = MAX_ADJUSTMENT - current_niceness;
          ورنہ
            adjustment = tmp;
        }
#ورنہ
      adjustment = MAX (MIN_ADJUSTMENT, MIN (tmp, MAX_ADJUSTMENT));
#اگر_ختم
    }

  اگر (i == argc)
    {
      اگر (adjustment_given)
        {
          error (0, 0, _("a command must be given with an adjustment"));
          usage (EXIT_CANCELED);
        }
      /* No command given; print the niceness.  */
      errno = 0;
      current_niceness = GET_NICENESS ();
      اگر (current_niceness == -1 && errno != 0)
        error (EXIT_CANCELED, errno, _("cannot get niceness"));
      لکھو ("%d\n", current_niceness);
      واپس EXIT_SUCCESS;
    }

  errno = 0;
#اگر HAVE_NICE
  ok = (nice (adjustment) != -1 || errno == 0);
#ورنہ
  current_niceness = GET_NICENESS ();
  اگر (current_niceness == -1 && errno != 0)
    error (EXIT_CANCELED, errno, _("cannot get niceness"));
  ok = (setpriority (PRIO_PROCESS, 0, current_niceness + adjustment) == 0);
#اگر_ختم
  اگر (!ok)
    {
      error (perm_related_errno (errno) ? 0
             : EXIT_CANCELED, errno, _("cannot set niceness"));
      /* error() flushes stderr, but does not check for write failure.
         Normally, we would catch this via our atexit() hook of
         close_stdout, but execvp() gets in the way.  If stderr
         encountered a write failure, there is no need to try calling
         error() again.  */
      اگر (ferror (stderr))
        واپس EXIT_CANCELED;
    }

  execvp (argv[i], &argv[i]);

  صحیح exit_حالت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[i]));
  واپس exit_حالت;
}
