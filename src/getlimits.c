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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#ਅਨ੍ਤਰ੍ਭਾਵ <signal.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <float.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "errno-iter.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ftoastr.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "long-options.h"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "getlimits"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਸਮਾਂ_T_MAX
# define ਸਮਾਂ_T_MAX TYPE_ਵੱਧ_ਤੋਂ_ਵੱਧ (time_t)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਸਮਾਂ_T_MIN
# define ਸਮਾਂ_T_MIN TYPE_MINIMUM (time_t)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SSIZE_MIN
# define SSIZE_MIN TYPE_MINIMUM (ssize_t)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PID_T_MIN
# define PID_T_MIN TYPE_MINIMUM (pid_t)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OFF64_T_MAX
# define OFF64_T_MAX TYPE_ਵੱਧ_ਤੋਂ_ਵੱਧ (off64_t)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OFF64_T_MIN
# define OFF64_T_MIN TYPE_MINIMUM (off64_t)
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#ਜੇ_ਅੰਤ

/* These are not interesting to print.
 * Instead of these defines it would be nice to be able to do
 * #ifdef (TYPE##_MIN) in function macro below.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਮਾਪ_MIN 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UCHAR_MIN 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UINT_MIN 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ULONG_MIN 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UINTMAX_MIN 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UID_T_MIN 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GID_T_MIN 0

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s\n\
"), ਪ੍ਰੋਗਰਾਮ_name);

      fputs (_("\
Output platform dependent limits ਵਿੱਚ a format useful ਲਈ shell scripts.\n\
\n\
"), stdout);
      fputs (HELP_OPTION_DESCRIPTION, stdout);
      fputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION, stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Add one to the absolute value of the number whose textual
   representation is BUF + 1.  Do this in-place, in the buffer.
   Return a pointer to the result, which is normally BUF + 1, but is
   BUF if the representation grew in size.  */
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *
decimal_absval_ਜੋੜੋ_one (ਅੱਖਰ *buf)
{
  bool negative = (buf[1] == '-');
  ਅੱਖਰ *absnum = buf + 1 + negative;
  ਅੱਖਰ *p = absnum + ਲੜੀ_ਲੰਬਾਈ (absnum);
  absnum[-1] = '0';
  ਜਦੋਂ (*--p == '9')
    *p = '0';
  ++*p;
  ਅੱਖਰ *ਨਤੀਜਾ = MIN (absnum, p);
  ਜੇ (negative)
    *--ਨਤੀਜਾ = '-';
  ਵਾਪਸ ਨਤੀਜਾ;
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
ਸਥਿਰ ਖਾਲੀ                                                             \
N (T x)                                                                 \
{                                                                       \
  ਅੱਖਰ buf[BUFSIZE];                                                    \
  FTOASTR (buf, ਆਕਾਰ buf, FTOASTR_ਖੱਬਾ_JUSTIFY, 0, x);                \
  puts (buf);                                                           \
}

PRINT_FLOATTYPE (print_FLT, ਦਸ਼ਮਲਵ, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_DBL, ਦੁੱਗਣਾ, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_LDBL, ਲੰਮਾ ਦੁੱਗਣਾ, ldtoastr, LDBL_BUFSIZE_BOUND)

ਸਥਿਰ ਪੂਰਨ
print_errno (ਖਾਲੀ *name, ਪੂਰਨ e)
{
  ਅੱਖਰ ਅਚਲ *err_name = name ? name : strerrorname_np (e);
  ਜੇ (err_name)
    ਲਿਖੋ ("%s=%s\n", err_name,
            quotearg_style (shell_escape_quoting_style, strerror (e)));
  ਵਾਪਸ 0;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ ਹੱਦਬੰਦੀ[1 + MAX (INT_BUFSIZE_BOUND (intmax_t),
                      INT_BUFSIZE_BOUND (uintmax_t))];

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, ਪ੍ਰੋਗਰਾਮ_NAME, PACKAGE_NAME,
                                   ਸੰਸਕਰਣ_2, true, usage, AUTHORS,
                                   (ਅੱਖਰ ਅਚਲ *) ਨਿਰਾ);

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ print_int(TYPE)                                                  \
  sprintf (ਹੱਦਬੰਦੀ + 1, "%ju", (uintmax_t) {TYPE##_MAX});                  \
  ਲਿਖੋ (#TYPE"_MAX=%s\n", ਹੱਦਬੰਦੀ + 1);                                  \
  ਲਿਖੋ (#TYPE"_OFLOW=%s\n", decimal_absval_ਜੋੜੋ_one (ਹੱਦਬੰਦੀ));           \
  ਜੇ (TYPE##_MIN)                                                        \
    {                                                                    \
      sprintf (ਹੱਦਬੰਦੀ + 1, "%jd", (intmax_t) {TYPE##_MIN});               \
      ਲਿਖੋ (#TYPE"_MIN=%s\n", ਹੱਦਬੰਦੀ + 1);                              \
      ਲਿਖੋ (#TYPE"_UFLOW=%s\n", decimal_absval_ਜੋੜੋ_one (ਹੱਦਬੰਦੀ));       \
    }

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ print_float(TYPE)                                                \
  ਲਿਖੋ (#TYPE"_MIN="); print_##TYPE (TYPE##_MIN);                      \
  ਲਿਖੋ (#TYPE"_MAX="); print_##TYPE (TYPE##_MAX);

  /* Variable sized ints */
  print_int (CHAR);
  print_int (SCHAR);
  print_int (UCHAR);
  print_int (SHRT);
  print_int (INT);
  print_int (UINT);
  print_int (LONG);
  print_int (ULONG);
  print_int (ਮਾਪ_2);
  print_int (SSIZE);
  print_int (ਸਮਾਂ_T);
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
  ਲਿਖੋ ("SIGRTMIN=%jd\n", (intmax_t) {SIGRTMIN});
  ਲਿਖੋ ("SIGRTMAX=%jd\n", (intmax_t) {SIGRTMAX});
  ਲਿਖੋ ("IO_BUFSIZE=%ju\n", (uintmax_t) {IO_BUFSIZE});

  /* Errnos */
  errno_iterate (print_errno, ਨਿਰਾ);
  /* Common errno aliases */
#ਜੇ defined ENOTEMPTY && ENOTEMPTY == EEXIST
  print_errno ((ਅੱਖਰ*)"ENOTEMPTY", EEXIST);
#ਜੇ_ਅੰਤ
#ਜੇ defined ENOTSUP && ENOTSUP == EOPNOTSUPP
  print_errno ((ਅੱਖਰ*)"ENOTSUP", EOPNOTSUPP);
#ਜੇ_ਅੰਤ
#ਜੇ defined EWOULDBLOCK && EWOULDBLOCK == EAGAIN
  print_errno ((ਅੱਖਰ*)"EWOULDBLOCK", EAGAIN);
#ਜੇ_ਅੰਤ
#ਜੇ defined EDEADLOCK && EDEADLOCK == EDEADLK
  print_errno ((ਅੱਖਰ*)"EDEADLOCK", EDEADLK);
#ਜੇ_ਅੰਤ

  ਵਾਪਸ EXIT_SUCCESS;
}
