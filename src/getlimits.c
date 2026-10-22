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

#ಅನ್ತರ್ಭಾವ <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#ಅನ್ತರ್ಭಾವ <signal.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <float.h>

#ಅನ್ತರ್ಭಾವ "errno-iter.h"
#ಅನ್ತರ್ಭಾವ "ftoastr.h"
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"
#ಅನ್ತರ್ಭಾವ "long-options.h"

#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "getlimits"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಸಮಯ_T_MAX
# define ಸಮಯ_T_MAX TYPE_ಗರಿಷ್ಠ (time_t)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಸಮಯ_T_MIN
# define ಸಮಯ_T_MIN TYPE_MINIMUM (time_t)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SSIZE_MIN
# define SSIZE_MIN TYPE_MINIMUM (ssize_t)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PID_T_MIN
# define PID_T_MIN TYPE_MINIMUM (pid_t)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OFF64_T_MAX
# define OFF64_T_MAX TYPE_ಗರಿಷ್ಠ (off64_t)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OFF64_T_MIN
# define OFF64_T_MIN TYPE_MINIMUM (off64_t)
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#ಆದರೆ_ಅಂತ್ಯ

/* These are not interesting to print.
 * Instead of these defines it would be nice to be able to do
 * #ifdef (TYPE##_MIN) in function macro below.  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಗಾತ್ರ_MIN 0
#ವ್ಯಾಖ್ಯಾನಿಸು UCHAR_MIN 0
#ವ್ಯಾಖ್ಯಾನಿಸು UINT_MIN 0
#ವ್ಯಾಖ್ಯಾನಿಸು ULONG_MIN 0
#ವ್ಯಾಖ್ಯಾನಿಸು UINTMAX_MIN 0
#ವ್ಯಾಖ್ಯಾನಿಸು UID_T_MIN 0
#ವ್ಯಾಖ್ಯಾನಿಸು GID_T_MIN 0

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s\n\
"), ಕಾರ್ಯಕ್ರಮ_name);

      fputs (_("\
Output platform dependent limits ಒಳ a format useful ಗಾಗಿ shell scripts.\n\
\n\
"), stdout);
      fputs (HELP_OPTION_DESCRIPTION, stdout);
      fputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION, stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Add one to the absolute value of the number whose textual
   representation is BUF + 1.  Do this in-place, in the buffer.
   Return a pointer to the result, which is normally BUF + 1, but is
   BUF if the representation grew in size.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *
decimal_absval_ಕೂಡಿಸು_one (ಅಕ್ಷರ *buf)
{
  bool negative = (buf[1] == '-');
  ಅಕ್ಷರ *absnum = buf + 1 + negative;
  ಅಕ್ಷರ *p = absnum + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (absnum);
  absnum[-1] = '0';
  ಆಗ (*--p == '9')
    *p = '0';
  ++*p;
  ಅಕ್ಷರ *ಫಲಿತಾಂಶ = MIN (absnum, p);
  ಆದರೆ (negative)
    *--ಫಲಿತಾಂಶ = '-';
  ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

#ವ್ಯಾಖ್ಯಾನಿಸು PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
ಸ್ಥಿರ ಶೂನ್ಯ                                                             \
N (T x)                                                                 \
{                                                                       \
  ಅಕ್ಷರ buf[BUFSIZE];                                                    \
  FTOASTR (buf, ಗಾತ್ರ buf, FTOASTR_ಎಡ_JUSTIFY, 0, x);                \
  puts (buf);                                                           \
}

PRINT_FLOATTYPE (print_FLT, ದಶಮಾಂಶ, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_DBL, ದ್ವಿಗುಣ, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_LDBL, ದೀರ್ಘ ದ್ವಿಗುಣ, ldtoastr, LDBL_BUFSIZE_BOUND)

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
print_errno (ಶೂನ್ಯ *name, ಪೂರ್ಣಾಂಕ e)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *err_name = name ? name : strerrorname_np (e);
  ಆದರೆ (err_name)
    ಬರೆ ("%s=%s\n", err_name,
            quotearg_style (shell_escape_quoting_style, strerror (e)));
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ ಎಲ್ಲೆ[1 + MAX (INT_BUFSIZE_BOUND (intmax_t),
                      INT_BUFSIZE_BOUND (uintmax_t))];

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, ಕಾರ್ಯಕ್ರಮ_NAME, PACKAGE_NAME,
                                   ಆವೃತ್ತಿ_2, true, usage, AUTHORS,
                                   (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) ಖಾಲಿ);

#ವ್ಯಾಖ್ಯಾನಿಸು print_int(TYPE)                                                  \
  sprintf (ಎಲ್ಲೆ + 1, "%ju", (uintmax_t) {TYPE##_MAX});                  \
  ಬರೆ (#TYPE"_MAX=%s\n", ಎಲ್ಲೆ + 1);                                  \
  ಬರೆ (#TYPE"_OFLOW=%s\n", decimal_absval_ಕೂಡಿಸು_one (ಎಲ್ಲೆ));           \
  ಆದರೆ (TYPE##_MIN)                                                        \
    {                                                                    \
      sprintf (ಎಲ್ಲೆ + 1, "%jd", (intmax_t) {TYPE##_MIN});               \
      ಬರೆ (#TYPE"_MIN=%s\n", ಎಲ್ಲೆ + 1);                              \
      ಬರೆ (#TYPE"_UFLOW=%s\n", decimal_absval_ಕೂಡಿಸು_one (ಎಲ್ಲೆ));       \
    }

#ವ್ಯಾಖ್ಯಾನಿಸು print_float(TYPE)                                                \
  ಬರೆ (#TYPE"_MIN="); print_##TYPE (TYPE##_MIN);                      \
  ಬರೆ (#TYPE"_MAX="); print_##TYPE (TYPE##_MAX);

  /* Variable sized ints */
  print_int (CHAR);
  print_int (SCHAR);
  print_int (UCHAR);
  print_int (SHRT);
  print_int (INT);
  print_int (UINT);
  print_int (LONG);
  print_int (ULONG);
  print_int (ಗಾತ್ರ_3);
  print_int (SSIZE);
  print_int (ಸಮಯ_T);
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
  ಬರೆ ("SIGRTMIN=%jd\n", (intmax_t) {SIGRTMIN});
  ಬರೆ ("SIGRTMAX=%jd\n", (intmax_t) {SIGRTMAX});
  ಬರೆ ("IO_BUFSIZE=%ju\n", (uintmax_t) {IO_BUFSIZE});

  /* Errnos */
  errno_iterate (print_errno, ಖಾಲಿ);
  /* Common errno aliases */
#ಆದರೆ defined ENOTEMPTY && ENOTEMPTY == EEXIST
  print_errno ((ಅಕ್ಷರ*)"ENOTEMPTY", EEXIST);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined ENOTSUP && ENOTSUP == EOPNOTSUPP
  print_errno ((ಅಕ್ಷರ*)"ENOTSUP", EOPNOTSUPP);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined EWOULDBLOCK && EWOULDBLOCK == EAGAIN
  print_errno ((ಅಕ್ಷರ*)"EWOULDBLOCK", EAGAIN);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined EDEADLOCK && EDEADLOCK == EDEADLK
  print_errno ((ಅಕ್ಷರ*)"EDEADLOCK", EDEADLK);
#ಆದರೆ_ಅಂತ್ಯ

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
