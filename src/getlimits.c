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

#અન્તર્ભાવ <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#અન્તર્ભાવ <signal.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <float.h>

#અન્તર્ભાવ "errno-iter.h"
#અન્તર્ભાવ "ftoastr.h"
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "ioblksize.h"
#અન્તર્ભાવ "long-options.h"

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

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s\n\
"), program_name);

      fputs (_("\
Output platform dependent limits in a format useful માટે shell scripts.\n\
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
સ્થિર અક્ષર અચલ *
decimal_absval_add_one (અક્ષર *buf)
{
  bool negative = (buf[1] == '-');
  અક્ષર *absnum = buf + 1 + negative;
  અક્ષર *p = absnum + strlen (absnum);
  absnum[-1] = '0';
  જ્યારે (*--p == '9')
    *p = '0';
  ++*p;
  અક્ષર *result = MIN (absnum, p);
  જો (negative)
    *--result = '-';
  પાછા result;
}

#define PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
સ્થિર શૂન્ય                                                             \
N (T x)                                                                 \
{                                                                       \
  અક્ષર buf[BUFSIZE];                                                    \
  FTOASTR (buf, કદ buf, FTOASTR_ડાબું_JUSTIFY, 0, x);                \
  puts (buf);                                                           \
}

PRINT_FLOATTYPE (print_FLT, દશાંશ, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_DBL, દ્વિગુણ, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_LDBL, long દ્વિગુણ, ldtoastr, LDBL_BUFSIZE_BOUND)

સ્થિર પૂર્ણાંક
print_errno (શૂન્ય *name, પૂર્ણાંક e)
{
  અક્ષર અચલ *err_name = name ? name : strerrorname_np (e);
  જો (err_name)
    printf ("%s=%s\n", err_name,
            quotearg_style (shell_escape_quoting_style, strerror (e)));
  પાછા 0;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  અક્ષર limit[1 + MAX (INT_BUFSIZE_BOUND (intmax_t),
                      INT_BUFSIZE_BOUND (uintmax_t))];

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   VERSION, true, usage, AUTHORS,
                                   (અક્ષર અચલ *) ખાલી);

#define print_int(TYPE)                                                  \
  sprintf (limit + 1, "%ju", (uintmax_t) {TYPE##_MAX});                  \
  printf (#TYPE"_MAX=%s\n", limit + 1);                                  \
  printf (#TYPE"_OFLOW=%s\n", decimal_absval_add_one (limit));           \
  જો (TYPE##_MIN)                                                        \
    {                                                                    \
      sprintf (limit + 1, "%jd", (intmax_t) {TYPE##_MIN});               \
      printf (#TYPE"_MIN=%s\n", limit + 1);                              \
      printf (#TYPE"_UFLOW=%s\n", decimal_absval_add_one (limit));       \
    }

#define print_float(TYPE)                                                \
  printf (#TYPE"_MIN="); print_##TYPE (TYPE##_MIN);                      \
  printf (#TYPE"_MAX="); print_##TYPE (TYPE##_MAX);

  /* Variable sized ints */
  print_int (CHAR);
  print_int (SCHAR);
  print_int (UCHAR);
  print_int (SHRT);
  print_int (INT);
  print_int (UINT);
  print_int (LONG);
  print_int (ULONG);
  print_int (SIZE);
  print_int (SSIZE);
  print_int (TIME_T);
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
  printf ("SIGRTMIN=%jd\n", (intmax_t) {SIGRTMIN});
  printf ("SIGRTMAX=%jd\n", (intmax_t) {SIGRTMAX});
  printf ("IO_BUFSIZE=%ju\n", (uintmax_t) {IO_BUFSIZE});

  /* Errnos */
  errno_iterate (print_errno, ખાલી);
  /* Common errno aliases */
#જો defined ENOTEMPTY && ENOTEMPTY == EEXIST
  print_errno ((અક્ષર*)"ENOTEMPTY", EEXIST);
#endif
#જો defined ENOTSUP && ENOTSUP == EOPNOTSUPP
  print_errno ((અક્ષર*)"ENOTSUP", EOPNOTSUPP);
#endif
#જો defined EWOULDBLOCK && EWOULDBLOCK == EAGAIN
  print_errno ((અક્ષર*)"EWOULDBLOCK", EAGAIN);
#endif
#જો defined EDEADLOCK && EDEADLOCK == EDEADLK
  print_errno ((અક્ષર*)"EDEADLOCK", EDEADLK);
#endif

  પાછા EXIT_SUCCESS;
}
