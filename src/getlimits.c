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

#define PROGRAM_NAME "getlimits"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

#ifndef TIME_T_MAX
# define TIME_T_MAX TYPE_MAXIMUM (time_t)
#endif

#ifndef TIME_T_MIN
# define TIME_T_MIN TYPE_MINIMUM (time_t)
#endif

#ifndef SSIZE_MIN
# define SSIZE_MIN TYPE_MINIMUM (ssize_t)
#endif

#ifndef PID_T_MIN
# define PID_T_MIN TYPE_MINIMUM (pid_t)
#endif

#ifndef OFF64_T_MAX
# define OFF64_T_MAX TYPE_MAXIMUM (off64_t)
#endif

#ifndef OFF64_T_MIN
# define OFF64_T_MIN TYPE_MINIMUM (off64_t)
#endif

#ifndef SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#endif
#ifndef SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#endif

/* These are not interesting to print.
 * Instead of these defines it would be nice to be able to do
 * #ifdef (TYPE##_MIN) in function macro below.  */
#define SIZE_MIN 0
#define UCHAR_MIN 0
#define UINT_MIN 0
#define ULONG_MIN 0
#define UINTMAX_MIN 0
#define UID_T_MIN 0
#define GID_T_MIN 0

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("\
Usage: %s\n\
"), program_name);

      fputs (_("\
Output platform dependent limits in a format useful لكل shell scripts.\n\
\n\
"), stdout);
      fputs (HELP_OPTION_DESCRIPTION, stdout);
      fputs (VERSION_OPTION_DESCRIPTION, stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Add one to the absolute value of the number whose textual
   representation is BUF + 1.  Do this in-place, in the buffer.
   Return a pointer to the result, which is normally BUF + 1, but is
   BUF if the representation grew in size.  */
ساكن حرف ثابت *
decimal_absval_add_one (حرف *buf)
{
  bool negative = (buf[1] == '-');
  حرف *absnum = buf + 1 + negative;
  حرف *p = absnum + strlen (absnum);
  absnum[-1] = '0';
  طالما (*--p == '9')
    *p = '0';
  ++*p;
  حرف *result = MIN (absnum, p);
  إذا (negative)
    *--result = '-';
  أرجع result;
}

#define بغ_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
ساكن فراغ                                                             \
N (T x)                                                                 \
{                                                                       \
  حرف buf[BUFSIZE];                                                    \
  FTOASTR (buf, حجم buf, FTOASTR_هغو_JUSTIFY, 0, x);                \
  puts (buf);                                                           \
}

بغ_FLOATTYPE (بغ_FLT, عائم, ftoastr, FLT_BUFSIZE_BOUND)
بغ_FLOATTYPE (بغ_DBL, مزدوج, dtoastr, DBL_BUFSIZE_BOUND)
بغ_FLOATTYPE (بغ_LDBL, long مزدوج, ldtoastr, LDBL_BUFSIZE_BOUND)

ساكن صحيح
بغ_errno (فراغ *name, صحيح e)
{
  حرف ثابت *err_name = name ? name : strerrorname_np (e);
  إذا (err_name)
    printf ("%s=%s\n", err_name,
            quotearg_style (shell_escape_quoting_style, strerror (e)));
  أرجع 0;
}

صحيح
main (صحيح argc, حرف **argv)
{
  حرف limit[1 + MAX (INT_BUFSIZE_BOUND (intmax_t),
                      INT_BUFSIZE_BOUND (uintmax_t))];

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   VERSION, true, usage, AUTHORS,
                                   (حرف ثابت *) عدم);

#define بغ_int(TYPE)                                                  \
  sprintf (limit + 1, "%ju", (uintmax_t) {TYPE##_MAX});                  \
  printf (#TYPE"_MAX=%s\n", limit + 1);                                  \
  printf (#TYPE"_OFLOW=%s\n", decimal_absval_add_one (limit));           \
  إذا (TYPE##_MIN)                                                        \
    {                                                                    \
      sprintf (limit + 1, "%jd", (intmax_t) {TYPE##_MIN});               \
      printf (#TYPE"_MIN=%s\n", limit + 1);                              \
      printf (#TYPE"_UFLOW=%s\n", decimal_absval_add_one (limit));       \
    }

#define بغ_float(TYPE)                                                \
  printf (#TYPE"_MIN="); بغ_3##TYPE (TYPE##_MIN);                      \
  printf (#TYPE"_MAX="); بغ_3##TYPE (TYPE##_MAX);

  /* Variable sized ints */
  بغ_int (CHAR);
  بغ_int (SCHAR);
  بغ_int (UCHAR);
  بغ_int (SHRT);
  بغ_int (INT);
  بغ_int (UINT);
  بغ_int (LONG);
  بغ_int (ULONG);
  بغ_int (SIZE);
  بغ_int (SSIZE);
  بغ_int (TIME_T);
  بغ_int (UID_T);
  بغ_int (GID_T);
  بغ_int (PID_T);
  بغ_int (OFF_T);
  بغ_int (OFF64_T);
  بغ_int (INTMAX);
  بغ_int (UINTMAX);

  /* Variable sized floats */
  بغ_float (FLT);
  بغ_float (DBL);
  بغ_float (LDBL);

  /* Other useful constants */
  printf ("SIGRTMIN=%jd\n", (intmax_t) {SIGRTMIN});
  printf ("SIGRTMAX=%jd\n", (intmax_t) {SIGRTMAX});
  printf ("IO_BUFSIZE=%ju\n", (uintmax_t) {IO_BUFSIZE});

  /* Errnos */
  errno_iterate (بغ_errno, عدم);
  /* Common errno aliases */
#إذا defined ENOTEMPTY && ENOTEMPTY == EEXIST
  بغ_errno ((حرف*)"ENOTEMPTY", EEXIST);
#endif
#إذا defined ENOTSUP && ENOTSUP == EOPNOTSUPP
  بغ_errno ((حرف*)"ENOTSUP", EOPNOTSUPP);
#endif
#إذا defined EWOULDBLOCK && EWOULDBLOCK == EAGAIN
  بغ_errno ((حرف*)"EWOULDBLOCK", EAGAIN);
#endif
#إذا defined EDEADLOCK && EDEADLOCK == EDEADLK
  بغ_errno ((حرف*)"EDEADLOCK", EDEADLK);
#endif

  أرجع EXIT_SUCCESS;
}
