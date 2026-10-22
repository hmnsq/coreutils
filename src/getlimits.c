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

#अन्तर्भाव <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#अन्तर्भाव <signal.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <float.h>

#अन्तर्भाव "errno-iter.h"
#अन्तर्भाव "ftoastr.h"
#अन्तर्भाव "system.h"
#अन्तर्भाव "ioblksize.h"
#अन्तर्भाव "long-options.h"

#परिभाषय क्रमादेशः_NAME "getlimits"

#परिभाषय AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

#यदि_अपरिभाषितम् TIME_T_MAX
# define TIME_T_MAX TYPE_अधिकतमम् (time_t)
#यदि_अन्तः

#यदि_अपरिभाषितम् TIME_T_MIN
# define TIME_T_MIN TYPE_MINIMUM (time_t)
#यदि_अन्तः

#यदि_अपरिभाषितम् SSIZE_MIN
# define SSIZE_MIN TYPE_MINIMUM (ssize_t)
#यदि_अन्तः

#यदि_अपरिभाषितम् PID_T_MIN
# define PID_T_MIN TYPE_MINIMUM (pid_t)
#यदि_अन्तः

#यदि_अपरिभाषितम् OFF64_T_MAX
# define OFF64_T_MAX TYPE_अधिकतमम् (off64_t)
#यदि_अन्तः

#यदि_अपरिभाषितम् OFF64_T_MIN
# define OFF64_T_MIN TYPE_MINIMUM (off64_t)
#यदि_अन्तः

#यदि_अपरिभाषितम् SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#यदि_अन्तः
#यदि_अपरिभाषितम् SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#यदि_अन्तः

/* These are not interesting to print.
 * Instead of these defines it would be nice to be able to do
 * #ifdef (TYPE##_MIN) in function macro below.  */
#परिभाषय आकार_MIN 0
#परिभाषय UCHAR_MIN 0
#परिभाषय UINT_MIN 0
#परिभाषय ULONG_MIN 0
#परिभाषय UINTMAX_MIN 0
#परिभाषय UID_T_MIN 0
#परिभाषय GID_T_MIN 0

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s\n\
"), क्रमादेशः_name);

      fputs (_("\
Output platform dependent limits अन्तर् a format useful प्रति shell scripts.\n\
\n\
"), stdout);
      fputs (HELP_OPTION_DESCRIPTION, stdout);
      fputs (संस्करणम्_OPTION_DESCRIPTION, stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Add one to the absolute value of the number whose textual
   representation is BUF + 1.  Do this in-place, in the buffer.
   Return a pointer to the result, which is normally BUF + 1, but is
   BUF if the representation grew in size.  */
स्थिर वर्ण ध्रुव *
decimal_absval_सङ्कलय_one (वर्ण *buf)
{
  bool negative = (buf[1] == '-');
  वर्ण *absnum = buf + 1 + negative;
  वर्ण *p = absnum + वाक्यदैर्घ्यम् (absnum);
  absnum[-1] = '0';
  यावत् (*--p == '9')
    *p = '0';
  ++*p;
  वर्ण *परिणाम = MIN (absnum, p);
  यदि (negative)
    *--परिणाम = '-';
  प्रत्यागम परिणाम;
}

#परिभाषय PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
स्थिर शून्य                                                             \
N (T x)                                                                 \
{                                                                       \
  वर्ण buf[BUFSIZE];                                                    \
  FTOASTR (buf, परिमाण buf, FTOASTR_वामः_JUSTIFY, 0, x);                \
  पङ्क्तिं_लिख (buf);                                                           \
}

PRINT_FLOATTYPE (print_FLT, भिन्न, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_DBL, द्विगुण, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_LDBL, दीर्घ द्विगुण, ldtoastr, LDBL_BUFSIZE_BOUND)

स्थिर पूर्णाङ्क
print_errno (शून्य *name, पूर्णाङ्क e)
{
  वर्ण ध्रुव *err_name = name ? name : strerrorname_np (e);
  यदि (err_name)
    प्रारूप_लिख ("%s=%s\n", err_name,
            quotearg_style (shell_escape_quoting_style, strerror (e)));
  प्रत्यागम 0;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण सीमा[1 + MAX (INT_BUFSIZE_BOUND (intmax_t),
                      INT_BUFSIZE_BOUND (uintmax_t))];

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, क्रमादेशः_NAME, PACKAGE_NAME,
                                   संस्करणम्_2, true, usage, AUTHORS,
                                   (वर्ण ध्रुव *) रिक्त);

#परिभाषय print_int(TYPE)                                                  \
  sprintf (सीमा + 1, "%ju", (uintmax_t) {TYPE##_MAX});                  \
  प्रारूप_लिख (#TYPE"_MAX=%s\n", सीमा + 1);                                  \
  प्रारूप_लिख (#TYPE"_OFLOW=%s\n", decimal_absval_सङ्कलय_one (सीमा));           \
  यदि (TYPE##_MIN)                                                        \
    {                                                                    \
      sprintf (सीमा + 1, "%jd", (intmax_t) {TYPE##_MIN});               \
      प्रारूप_लिख (#TYPE"_MIN=%s\n", सीमा + 1);                              \
      प्रारूप_लिख (#TYPE"_UFLOW=%s\n", decimal_absval_सङ्कलय_one (सीमा));       \
    }

#परिभाषय print_float(TYPE)                                                \
  प्रारूप_लिख (#TYPE"_MIN="); print_##TYPE (TYPE##_MIN);                      \
  प्रारूप_लिख (#TYPE"_MAX="); print_##TYPE (TYPE##_MAX);

  /* Variable sized ints */
  print_int (CHAR);
  print_int (SCHAR);
  print_int (UCHAR);
  print_int (SHRT);
  print_int (INT);
  print_int (UINT);
  print_int (LONG);
  print_int (ULONG);
  print_int (आकार_2);
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
  प्रारूप_लिख ("SIGRTMIN=%jd\n", (intmax_t) {SIGRTMIN});
  प्रारूप_लिख ("SIGRTMAX=%jd\n", (intmax_t) {SIGRTMAX});
  प्रारूप_लिख ("IO_BUFSIZE=%ju\n", (uintmax_t) {IO_BUFSIZE});

  /* Errnos */
  errno_iterate (print_errno, रिक्त);
  /* Common errno aliases */
#यदि defined ENOTEMPTY && ENOTEMPTY == EEXIST
  print_errno ((वर्ण*)"ENOTEMPTY", EEXIST);
#यदि_अन्तः
#यदि defined ENOTSUP && ENOTSUP == EOPNOTSUPP
  print_errno ((वर्ण*)"ENOTSUP", EOPNOTSUPP);
#यदि_अन्तः
#यदि defined EWOULDBLOCK && EWOULDBLOCK == EAGAIN
  print_errno ((वर्ण*)"EWOULDBLOCK", EAGAIN);
#यदि_अन्तः
#यदि defined EDEADLOCK && EDEADLOCK == EDEADLK
  print_errno ((वर्ण*)"EDEADLOCK", EDEADLK);
#यदि_अन्तः

  प्रत्यागम EXIT_SUCCESS;
}
