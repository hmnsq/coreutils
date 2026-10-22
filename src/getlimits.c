/* getlimits - print various platform dependent limits.
   Copyright (C) 2008-2026 Free Software Foundation, Inc.

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

/* Written by Pádraig Brady  */

#شامل <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#شامل <signal.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <float.h>

#شامل "errno-iter.h"
#شامل "ftoastr.h"
#شامل "system.h"
#شامل "ioblksize.h"
#شامل "long-options.h"

#تعریف_کرو پروگرام_NAME "getlimits"

#تعریف_کرو AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

#اگر_غیر_تعریف_شدہ وقت_T_MAX
# define وقت_T_MAX TYPE_زیادہ_سے_زیادہ (time_t)
#اگر_ختم

#اگر_غیر_تعریف_شدہ وقت_T_MIN
# define وقت_T_MIN TYPE_MINIMUM (time_t)
#اگر_ختم

#اگر_غیر_تعریف_شدہ SSIZE_MIN
# define SSIZE_MIN TYPE_MINIMUM (ssize_t)
#اگر_ختم

#اگر_غیر_تعریف_شدہ PID_T_MIN
# define PID_T_MIN TYPE_MINIMUM (pid_t)
#اگر_ختم

#اگر_غیر_تعریف_شدہ OFF64_T_MAX
# define OFF64_T_MAX TYPE_زیادہ_سے_زیادہ (off64_t)
#اگر_ختم

#اگر_غیر_تعریف_شدہ OFF64_T_MIN
# define OFF64_T_MIN TYPE_MINIMUM (off64_t)
#اگر_ختم

#اگر_غیر_تعریف_شدہ SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#اگر_ختم
#اگر_غیر_تعریف_شدہ SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#اگر_ختم

/* These are not interesting to print.
 * Instead of these defines it would be nice to be able to do
 * #ifdef (TYPE##_MIN) in function macro below.  */
#تعریف_کرو سائز_MIN 0
#تعریف_کرو UCHAR_MIN 0
#تعریف_کرو UINT_MIN 0
#تعریف_کرو ULONG_MIN 0
#تعریف_کرو UINTMAX_MIN 0
#تعریف_کرو UID_T_MIN 0
#تعریف_کرو GID_T_MIN 0

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s\n\
"), پروگرام_name);

      fputs (_("\
Output platform dependent limits اندر a format useful برائے shell scripts.\n\
\n\
"), stdout);
      fputs (HELP_OPTION_DESCRIPTION, stdout);
      fputs (نسخہ_OPTION_DESCRIPTION, stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Add one to the absolute value of the number whose textual
   representation is BUF + 1.  Do this in-place, in the buffer.
   Return a pointer to the result, which is normally BUF + 1, but is
   BUF if the representation grew in size.  */
ساکن حرف ثابت *
decimal_absval_جوڑو_one (حرف *buf)
{
  bool negative = (buf[1] == '-');
  حرف *absnum = buf + 1 + negative;
  حرف *p = absnum + لڑی_لمبائی (absnum);
  absnum[-1] = '0';
  جب (*--p == '9')
    *p = '0';
  ++*p;
  حرف *نتیجہ = MIN (absnum, p);
  اگر (negative)
    *--نتیجہ = '-';
  واپس نتیجہ;
}

#تعریف_کرو PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
ساکن خالی                                                             \
N (T x)                                                                 \
{                                                                       \
  حرف buf[BUFSIZE];                                                    \
  FTOASTR (buf, حجم buf, FTOASTR_بایاں_JUSTIFY, 0, x);                \
  puts (buf);                                                           \
}

PRINT_FLOATTYPE (print_FLT, اعشاری, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_DBL, دوہرا, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_LDBL, طویل دوہرا, ldtoastr, LDBL_BUFSIZE_BOUND)

ساکن صحیح
print_errno (خالی *name, صحیح e)
{
  حرف ثابت *err_name = name ? name : strerrorname_np (e);
  اگر (err_name)
    لکھو ("%s=%s\n", err_name,
            quotearg_style (shell_escape_quoting_style, strerror (e)));
  واپس 0;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  حرف حد_بندی[1 + MAX (INT_BUFSIZE_BOUND (intmax_t),
                      INT_BUFSIZE_BOUND (uintmax_t))];

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE_NAME,
                                   نسخہ_2, true, usage, AUTHORS,
                                   (حرف ثابت *) تہی);

#تعریف_کرو print_int(TYPE)                                                  \
  sprintf (حد_بندی + 1, "%ju", (uintmax_t) {TYPE##_MAX});                  \
  لکھو (#TYPE"_MAX=%s\n", حد_بندی + 1);                                  \
  لکھو (#TYPE"_OFLOW=%s\n", decimal_absval_جوڑو_one (حد_بندی));           \
  اگر (TYPE##_MIN)                                                        \
    {                                                                    \
      sprintf (حد_بندی + 1, "%jd", (intmax_t) {TYPE##_MIN});               \
      لکھو (#TYPE"_MIN=%s\n", حد_بندی + 1);                              \
      لکھو (#TYPE"_UFLOW=%s\n", decimal_absval_جوڑو_one (حد_بندی));       \
    }

#تعریف_کرو print_float(TYPE)                                                \
  لکھو (#TYPE"_MIN="); print_##TYPE (TYPE##_MIN);                      \
  لکھو (#TYPE"_MAX="); print_##TYPE (TYPE##_MAX);

  /* Variable sized ints */
  print_int (CHAR);
  print_int (SCHAR);
  print_int (UCHAR);
  print_int (SHRT);
  print_int (INT);
  print_int (UINT);
  print_int (LONG);
  print_int (ULONG);
  print_int (سائز_2);
  print_int (SSIZE);
  print_int (وقت_T);
  print_int (UID_T);
  print_int (GID_T);
  print_int (PID_T);
  print_int (OFF_T);
  print_int (OFF64_T);
  print_int (INTMAX);
  print_int (UINTMAX);

  /* Variable sized floats */
  print_float (FLT);
  print_float (DBL);
  print_float (LDBL);

  /* Other useful constants */
  لکھو ("SIGRTMIN=%jd\n", (intmax_t) {SIGRTMIN});
  لکھو ("SIGRTMAX=%jd\n", (intmax_t) {SIGRTMAX});
  لکھو ("IO_BUFSIZE=%ju\n", (uintmax_t) {IO_BUFSIZE});

  /* Errnos */
  errno_iterate (print_errno, تہی);
  /* Common errno aliases */
#اگر defined ENOTEMPTY && ENOTEMPTY == EEXIST
  print_errno ((حرف*)"ENOTEMPTY", EEXIST);
#اگر_ختم
#اگر defined ENOTSUP && ENOTSUP == EOPNOTSUPP
  print_errno ((حرف*)"ENOTSUP", EOPNOTSUPP);
#اگر_ختم
#اگر defined EWOULDBLOCK && EWOULDBLOCK == EAGAIN
  print_errno ((حرف*)"EWOULDBLOCK", EAGAIN);
#اگر_ختم
#اگر defined EDEADLOCK && EDEADLOCK == EDEADLK
  print_errno ((حرف*)"EDEADLOCK", EDEADLK);
#اگر_ختم

  واپس EXIT_SUCCESS;
}
