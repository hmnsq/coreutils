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

#అన్తర్భావ <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#అన్తర్భావ <signal.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <float.h>

#అన్తర్భావ "errno-iter.h"
#అన్తర్భావ "ftoastr.h"
#అన్తర్భావ "system.h"
#అన్తర్భావ "ioblksize.h"
#అన్తర్భావ "long-options.h"

#నిర్వచించు కార్యక్రమం_NAME "getlimits"

#నిర్వచించు AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

#నిర్వచితం_కాకపోతే సమయం_T_MAX
# define సమయం_T_MAX TYPE_గరిష్ఠ (time_t)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే సమయం_T_MIN
# define సమయం_T_MIN TYPE_MINIMUM (time_t)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే SSIZE_MIN
# define SSIZE_MIN TYPE_MINIMUM (ssize_t)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే PID_T_MIN
# define PID_T_MIN TYPE_MINIMUM (pid_t)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే OFF64_T_MAX
# define OFF64_T_MAX TYPE_గరిష్ఠ (off64_t)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే OFF64_T_MIN
# define OFF64_T_MIN TYPE_MINIMUM (off64_t)
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#అయితే_ముగింపు

/* These are not interesting to print.
 * Instead of these defines it would be nice to be able to do
 * #ifdef (TYPE##_MIN) in function macro below.  */
#నిర్వచించు కొలత_MIN 0
#నిర్వచించు UCHAR_MIN 0
#నిర్వచించు UINT_MIN 0
#నిర్వచించు ULONG_MIN 0
#నిర్వచించు UINTMAX_MIN 0
#నిర్వచించు UID_T_MIN 0
#నిర్వచించు GID_T_MIN 0

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s\n\
"), కార్యక్రమం_name);

      fputs (_("\
Output platform dependent limits లోపల a format useful కొరకు shell scripts.\n\
\n\
"), stdout);
      fputs (HELP_OPTION_DESCRIPTION, stdout);
      fputs (సంస్కరణ_OPTION_DESCRIPTION, stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Add one to the absolute value of the number whose textual
   representation is BUF + 1.  Do this in-place, in the buffer.
   Return a pointer to the result, which is normally BUF + 1, but is
   BUF if the representation grew in size.  */
స్థిర అక్షరం స్థిరరాశి *
decimal_absval_కూడు_one (అక్షరం *buf)
{
  bool negative = (buf[1] == '-');
  అక్షరం *absnum = buf + 1 + negative;
  అక్షరం *p = absnum + అక్షరశ్రేణి_పొడవు (absnum);
  absnum[-1] = '0';
  అయితే (*--p == '9')
    *p = '0';
  ++*p;
  అక్షరం *ఫలితం = MIN (absnum, p);
  ఒకవేళ (negative)
    *--ఫలితం = '-';
  తిరిగి ఫలితం;
}

#నిర్వచించు PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
స్థిర శూన్య                                                             \
N (T x)                                                                 \
{                                                                       \
  అక్షరం buf[BUFSIZE];                                                    \
  FTOASTR (buf, పరిమాణం buf, FTOASTR_ఎడమ_JUSTIFY, 0, x);                \
  puts (buf);                                                           \
}

PRINT_FLOATTYPE (print_FLT, దశాంశం, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_DBL, ద్విగుణ, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_LDBL, దీర్ఘ ద్విగుణ, ldtoastr, LDBL_BUFSIZE_BOUND)

స్థిర పూర్ణాంకం
print_errno (శూన్య *name, పూర్ణాంకం e)
{
  అక్షరం స్థిరరాశి *err_name = name ? name : strerrorname_np (e);
  ఒకవేళ (err_name)
    రాయి ("%s=%s\n", err_name,
            quotearg_style (shell_escape_quoting_style, strerror (e)));
  తిరిగి 0;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం హద్దు[1 + MAX (INT_BUFSIZE_BOUND (intmax_t),
                      INT_BUFSIZE_BOUND (uintmax_t))];

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, కార్యక్రమం_NAME, PACKAGE_NAME,
                                   సంస్కరణ_2, true, usage, AUTHORS,
                                   (అక్షరం స్థిరరాశి *) ఖాళీ);

#నిర్వచించు print_int(TYPE)                                                  \
  sprintf (హద్దు + 1, "%ju", (uintmax_t) {TYPE##_MAX});                  \
  రాయి (#TYPE"_MAX=%s\n", హద్దు + 1);                                  \
  రాయి (#TYPE"_OFLOW=%s\n", decimal_absval_కూడు_one (హద్దు));           \
  ఒకవేళ (TYPE##_MIN)                                                        \
    {                                                                    \
      sprintf (హద్దు + 1, "%jd", (intmax_t) {TYPE##_MIN});               \
      రాయి (#TYPE"_MIN=%s\n", హద్దు + 1);                              \
      రాయి (#TYPE"_UFLOW=%s\n", decimal_absval_కూడు_one (హద్దు));       \
    }

#నిర్వచించు print_float(TYPE)                                                \
  రాయి (#TYPE"_MIN="); print_##TYPE (TYPE##_MIN);                      \
  రాయి (#TYPE"_MAX="); print_##TYPE (TYPE##_MAX);

  /* Variable sized ints */
  print_int (CHAR);
  print_int (SCHAR);
  print_int (UCHAR);
  print_int (SHRT);
  print_int (INT);
  print_int (UINT);
  print_int (LONG);
  print_int (ULONG);
  print_int (కొలత_2);
  print_int (SSIZE);
  print_int (సమయం_T);
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
  రాయి ("SIGRTMIN=%jd\n", (intmax_t) {SIGRTMIN});
  రాయి ("SIGRTMAX=%jd\n", (intmax_t) {SIGRTMAX});
  రాయి ("IO_BUFSIZE=%ju\n", (uintmax_t) {IO_BUFSIZE});

  /* Errnos */
  errno_iterate (print_errno, ఖాళీ);
  /* Common errno aliases */
#ఒకవేళ defined ENOTEMPTY && ENOTEMPTY == EEXIST
  print_errno ((అక్షరం*)"ENOTEMPTY", EEXIST);
#అయితే_ముగింపు
#ఒకవేళ defined ENOTSUP && ENOTSUP == EOPNOTSUPP
  print_errno ((అక్షరం*)"ENOTSUP", EOPNOTSUPP);
#అయితే_ముగింపు
#ఒకవేళ defined EWOULDBLOCK && EWOULDBLOCK == EAGAIN
  print_errno ((అక్షరం*)"EWOULDBLOCK", EAGAIN);
#అయితే_ముగింపు
#ఒకవేళ defined EDEADLOCK && EDEADLOCK == EDEADLK
  print_errno ((అక్షరం*)"EDEADLOCK", EDEADLK);
#అయితే_ముగింపు

  తిరిగి EXIT_SUCCESS;
}
