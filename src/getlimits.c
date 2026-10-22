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

#ଅନ୍ତର୍ଭାଵ <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <float.h>

#ଅନ୍ତର୍ଭାଵ "errno-iter.h"
#ଅନ୍ତର୍ଭାଵ "ftoastr.h"
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "ioblksize.h"
#ଅନ୍ତର୍ଭାଵ "long-options.h"

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

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s\n\
"), program_name);

      fputs (_("\
Output platform dependent limits in a format useful ପାଇଁ shell scripts.\n\
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
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *
decimal_absval_add_one (ଅକ୍ଷର *buf)
{
  bool negative = (buf[1] == '-');
  ଅକ୍ଷର *absnum = buf + 1 + negative;
  ଅକ୍ଷର *p = absnum + strlen (absnum);
  absnum[-1] = '0';
  ଯେତେବେଳେ (*--p == '9')
    *p = '0';
  ++*p;
  ଅକ୍ଷର *result = MIN (absnum, p);
  ଯଦି (negative)
    *--result = '-';
  ଫେରନ୍ତୁ result;
}

#define PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
ସ୍ଥିର ଶୂନ୍ୟ                                                             \
N (T x)                                                                 \
{                                                                       \
  ଅକ୍ଷର buf[BUFSIZE];                                                    \
  FTOASTR (buf, ଆକାର buf, FTOASTR_ବାମ_JUSTIFY, 0, x);                \
  puts (buf);                                                           \
}

PRINT_FLOATTYPE (print_FLT, ଦଶମିକ, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_DBL, ଦ୍ୱିଗୁଣ, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_LDBL, long ଦ୍ୱିଗୁଣ, ldtoastr, LDBL_BUFSIZE_BOUND)

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
print_errno (ଶୂନ୍ୟ *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା e)
{
  ଅକ୍ଷର ଅଚଳ *err_name = name ? name : strerrorname_np (e);
  ଯଦି (err_name)
    printf ("%s=%s\n", err_name,
            quotearg_style (shell_escape_quoting_style, strerror (e)));
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଅକ୍ଷର limit[1 + MAX (INT_BUFSIZE_BOUND (intmax_t),
                      INT_BUFSIZE_BOUND (uintmax_t))];

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   VERSION, true, usage, AUTHORS,
                                   (ଅକ୍ଷର ଅଚଳ *) ଖାଲି);

#define print_int(TYPE)                                                  \
  sprintf (limit + 1, "%ju", (uintmax_t) {TYPE##_MAX});                  \
  printf (#TYPE"_MAX=%s\n", limit + 1);                                  \
  printf (#TYPE"_OFLOW=%s\n", decimal_absval_add_one (limit));           \
  ଯଦି (TYPE##_MIN)                                                        \
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
  errno_iterate (print_errno, ଖାଲି);
  /* Common errno aliases */
#ଯଦି defined ENOTEMPTY && ENOTEMPTY == EEXIST
  print_errno ((ଅକ୍ଷର*)"ENOTEMPTY", EEXIST);
#endif
#ଯଦି defined ENOTSUP && ENOTSUP == EOPNOTSUPP
  print_errno ((ଅକ୍ଷର*)"ENOTSUP", EOPNOTSUPP);
#endif
#ଯଦି defined EWOULDBLOCK && EWOULDBLOCK == EAGAIN
  print_errno ((ଅକ୍ଷର*)"EWOULDBLOCK", EAGAIN);
#endif
#ଯଦି defined EDEADLOCK && EDEADLOCK == EDEADLK
  print_errno ((ଅକ୍ଷର*)"EDEADLOCK", EDEADLK);
#endif

  ଫେରନ୍ତୁ EXIT_SUCCESS;
}
