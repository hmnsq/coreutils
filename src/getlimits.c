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

#അന്തര്ഭാവ <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <float.h>

#അന്തര്ഭാവ "errno-iter.h"
#അന്തര്ഭാവ "ftoastr.h"
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "ioblksize.h"
#അന്തര്ഭാവ "long-options.h"

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

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s\n\
"), program_name);

      fputs (_("\
Output platform dependent limits in a format useful വേണ്ടി shell scripts.\n\
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
സ്ഥിതം അക്ഷരം സ്ഥിരം *
decimal_absval_add_one (അക്ഷരം *buf)
{
  bool negative = (buf[1] == '-');
  അക്ഷരം *absnum = buf + 1 + negative;
  അക്ഷരം *p = absnum + strlen (absnum);
  absnum[-1] = '0';
  ആകുമ്പോൾ (*--p == '9')
    *p = '0';
  ++*p;
  അക്ഷരം *result = MIN (absnum, p);
  എങ്കിൽ (negative)
    *--result = '-';
  തിരികെ result;
}

#define PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
സ്ഥിതം ശൂന്യം                                                             \
N (T x)                                                                 \
{                                                                       \
  അക്ഷരം buf[BUFSIZE];                                                    \
  FTOASTR (buf, വലുപ്പം buf, FTOASTR_ഇടത്_JUSTIFY, 0, x);                \
  puts (buf);                                                           \
}

PRINT_FLOATTYPE (print_FLT, ദശാംശം, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_DBL, ഇരട്ട, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_LDBL, long ഇരട്ട, ldtoastr, LDBL_BUFSIZE_BOUND)

സ്ഥിതം പൂർണ്ണസംഖ്യ
print_errno (ശൂന്യം *name, പൂർണ്ണസംഖ്യ e)
{
  അക്ഷരം സ്ഥിരം *err_name = name ? name : strerrorname_np (e);
  എങ്കിൽ (err_name)
    printf ("%s=%s\n", err_name,
            quotearg_style (shell_escape_quoting_style, strerror (e)));
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  അക്ഷരം limit[1 + MAX (INT_BUFSIZE_BOUND (intmax_t),
                      INT_BUFSIZE_BOUND (uintmax_t))];

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   VERSION, true, usage, AUTHORS,
                                   (അക്ഷരം സ്ഥിരം *) ശൂന്യ);

#define print_int(TYPE)                                                  \
  sprintf (limit + 1, "%ju", (uintmax_t) {TYPE##_MAX});                  \
  printf (#TYPE"_MAX=%s\n", limit + 1);                                  \
  printf (#TYPE"_OFLOW=%s\n", decimal_absval_add_one (limit));           \
  എങ്കിൽ (TYPE##_MIN)                                                        \
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
  errno_iterate (print_errno, ശൂന്യ);
  /* Common errno aliases */
#എങ്കിൽ defined ENOTEMPTY && ENOTEMPTY == EEXIST
  print_errno ((അക്ഷരം*)"ENOTEMPTY", EEXIST);
#endif
#എങ്കിൽ defined ENOTSUP && ENOTSUP == EOPNOTSUPP
  print_errno ((അക്ഷരം*)"ENOTSUP", EOPNOTSUPP);
#endif
#എങ്കിൽ defined EWOULDBLOCK && EWOULDBLOCK == EAGAIN
  print_errno ((അക്ഷരം*)"EWOULDBLOCK", EAGAIN);
#endif
#എങ്കിൽ defined EDEADLOCK && EDEADLOCK == EDEADLK
  print_errno ((അക്ഷരം*)"EDEADLOCK", EDEADLK);
#endif

  തിരികെ EXIT_SUCCESS;
}
