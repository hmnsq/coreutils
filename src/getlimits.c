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

#ضمن <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#ضمن <signal.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <float.h>

#ضمن "errno-iter.h"
#ضمن "ftoastr.h"
#ضمن "system.h"
#ضمن "ioblksize.h"
#ضمن "long-options.h"

#وصف_ڪريو پروگرام_NAME "getlimits"

#وصف_ڪريو AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

#جيڪڏهن_وصف_نه_ٿيل وقت_T_MAX
# define وقت_T_MAX TYPE_وڌ_کان_وڌ (time_t)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل وقت_T_MIN
# define وقت_T_MIN TYPE_MINIMUM (time_t)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل SSIZE_MIN
# define SSIZE_MIN TYPE_MINIMUM (ssize_t)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل PID_T_MIN
# define PID_T_MIN TYPE_MINIMUM (pid_t)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل OFF64_T_MAX
# define OFF64_T_MAX TYPE_وڌ_کان_وڌ (off64_t)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل OFF64_T_MIN
# define OFF64_T_MIN TYPE_MINIMUM (off64_t)
#جيڪڏهن_پورو

#جيڪڏهن_وصف_نه_ٿيل SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#جيڪڏهن_پورو

/* These are not interesting to print.
 * Instead of these defines it would be nice to be able to do
 * #ifdef (TYPE##_MIN) in function macro below.  */
#وصف_ڪريو سائيز_MIN 0
#وصف_ڪريو UCHAR_MIN 0
#وصف_ڪريو UINT_MIN 0
#وصف_ڪريو ULONG_MIN 0
#وصف_ڪريو UINTMAX_MIN 0
#وصف_ڪريو UID_T_MIN 0
#وصف_ڪريو GID_T_MIN 0

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s\n\
"), پروگرام_name);

      fputs (_("\
Output platform dependent limits اندريون a format useful لكل shell scripts.\n\
\n\
"), stdout);
      fputs (HELP_OPTION_DESCRIPTION, stdout);
      fputs (نسخو_OPTION_DESCRIPTION, stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Add one to the absolute value of the number whose textual
   representation is BUF + 1.  Do this in-place, in the buffer.
   Return a pointer to the result, which is normally BUF + 1, but is
   BUF if the representation grew in size.  */
ساكن حرف ثابت *
decimal_absval_جوڙيو_one (حرف *buf)
{
  bool negative = (buf[1] == '-');
  حرف *absnum = buf + 1 + negative;
  حرف *p = absnum + لڙي_ڊيگهه (absnum);
  absnum[-1] = '0';
  طالما (*--p == '9')
    *p = '0';
  ++*p;
  حرف *نتيجو = MIN (absnum, p);
  إذا (negative)
    *--نتيجو = '-';
  أرجع نتيجو;
}

#وصف_ڪريو PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
ساكن فراغ                                                             \
N (T x)                                                                 \
{                                                                       \
  حرف buf[BUFSIZE];                                                    \
  FTOASTR (buf, حجم buf, FTOASTR_کاٻو_JUSTIFY, 0, x);                \
  puts (buf);                                                           \
}

PRINT_FLOATTYPE (print_FLT, عائم, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_DBL, مزدوج, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_LDBL, ڊگهو مزدوج, ldtoastr, LDBL_BUFSIZE_BOUND)

ساكن صحيح
print_errno (فراغ *name, صحيح e)
{
  حرف ثابت *err_name = name ? name : strerrorname_np (e);
  إذا (err_name)
    لکو ("%s=%s\n", err_name,
            quotearg_style (shell_escape_quoting_style, strerror (e)));
  أرجع 0;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  حرف حد_بندي[1 + MAX (INT_BUFSIZE_BOUND (intmax_t),
                      INT_BUFSIZE_BOUND (uintmax_t))];

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE_NAME,
                                   نسخو_2, true, usage, AUTHORS,
                                   (حرف ثابت *) عدم);

#وصف_ڪريو print_int(TYPE)                                                  \
  sprintf (حد_بندي + 1, "%ju", (uintmax_t) {TYPE##_MAX});                  \
  لکو (#TYPE"_MAX=%s\n", حد_بندي + 1);                                  \
  لکو (#TYPE"_OFLOW=%s\n", decimal_absval_جوڙيو_one (حد_بندي));           \
  إذا (TYPE##_MIN)                                                        \
    {                                                                    \
      sprintf (حد_بندي + 1, "%jd", (intmax_t) {TYPE##_MIN});               \
      لکو (#TYPE"_MIN=%s\n", حد_بندي + 1);                              \
      لکو (#TYPE"_UFLOW=%s\n", decimal_absval_جوڙيو_one (حد_بندي));       \
    }

#وصف_ڪريو print_float(TYPE)                                                \
  لکو (#TYPE"_MIN="); print_##TYPE (TYPE##_MIN);                      \
  لکو (#TYPE"_MAX="); print_##TYPE (TYPE##_MAX);

  /* Variable sized ints */
  print_int (CHAR);
  print_int (SCHAR);
  print_int (UCHAR);
  print_int (SHRT);
  print_int (INT);
  print_int (UINT);
  print_int (LONG);
  print_int (ULONG);
  print_int (سائيز_2);
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
  لکو ("SIGRTMIN=%jd\n", (intmax_t) {SIGRTMIN});
  لکو ("SIGRTMAX=%jd\n", (intmax_t) {SIGRTMAX});
  لکو ("IO_BUFSIZE=%ju\n", (uintmax_t) {IO_BUFSIZE});

  /* Errnos */
  errno_iterate (print_errno, عدم);
  /* Common errno aliases */
#إذا defined ENOTEMPTY && ENOTEMPTY == EEXIST
  print_errno ((حرف*)"ENOTEMPTY", EEXIST);
#جيڪڏهن_پورو
#إذا defined ENOTSUP && ENOTSUP == EOPNOTSUPP
  print_errno ((حرف*)"ENOTSUP", EOPNOTSUPP);
#جيڪڏهن_پورو
#إذا defined EWOULDBLOCK && EWOULDBLOCK == EAGAIN
  print_errno ((حرف*)"EWOULDBLOCK", EAGAIN);
#جيڪڏهن_پورو
#إذا defined EDEADLOCK && EDEADLOCK == EDEADLK
  print_errno ((حرف*)"EDEADLOCK", EDEADLK);
#جيڪڏهن_پورو

  أرجع EXIT_SUCCESS;
}
