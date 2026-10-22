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

#समावेश <config.h>             /* sets _FILE_OFFSET_BITS=64 etc. */
#समावेश <signal.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <float.h>

#समावेश "errno-iter.h"
#समावेश "ftoastr.h"
#समावेश "system.h"
#समावेश "ioblksize.h"
#समावेश "long-options.h"

#घोषणा PROGRAM_NAME "getlimits"

#घोषणा AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

#अगर_अघोषित TIME_T_MAX
# define TIME_T_MAX TYPE_MAXIMUM (समय_प्रकार)
#पूर्ण_अगर

#अगर_अघोषित TIME_T_MIN
# define TIME_T_MIN TYPE_MINIMUM (समय_प्रकार)
#पूर्ण_अगर

#अगर_अघोषित SSIZE_MIN
# define SSIZE_MIN TYPE_MINIMUM (ssize_t)
#पूर्ण_अगर

#अगर_अघोषित PID_T_MIN
# define PID_T_MIN TYPE_MINIMUM (pid_t)
#पूर्ण_अगर

#अगर_अघोषित OFF64_T_MAX
# define OFF64_T_MAX TYPE_MAXIMUM (off64_t)
#पूर्ण_अगर

#अगर_अघोषित OFF64_T_MIN
# define OFF64_T_MIN TYPE_MINIMUM (off64_t)
#पूर्ण_अगर

#अगर_अघोषित SIGRTMIN
# define SIGRTMIN 0
# undef SIGRTMAX
#पूर्ण_अगर
#अगर_अघोषित SIGRTMAX
# define SIGRTMAX (SIGRTMIN - 1)
#पूर्ण_अगर

/* These are not interesting to print.
 * Instead of these defines it would be nice to be able to do
 * #ifdef (TYPE##_MIN) in function macro below.  */
#घोषणा आकार_MIN 0
#घोषणा UCHAR_MIN 0
#घोषणा UINT_MIN 0
#घोषणा ULONG_MIN 0
#घोषणा UINTMAX_MIN 0
#घोषणा UID_T_MIN 0
#घोषणा GID_T_MIN 0

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s\n\
"), program_name);

      ख_माला_दो (_("\
Output platform dependent limits in a format useful क्रम shell scripts.\n\
\n\
"), मानक_निकास);
      ख_माला_दो (HELP_OPTION_DESCRIPTION, मानक_निकास);
      ख_माला_दो (VERSION_OPTION_DESCRIPTION, मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Add one to the absolute value of the number whose textual
   representation is BUF + 1.  Do this in-place, in the buffer.
   Return a pointer to the result, which is normally BUF + 1, but is
   BUF if the representation grew in size.  */
अटल अक्षर स्थिर *
decimal_absval_add_one (अक्षर *buf)
{
  bool negative = (buf[1] == '-');
  अक्षर *absnum = buf + 1 + negative;
  अक्षर *p = absnum + म_माप (absnum);
  absnum[-1] = '0';
  जबतक (*--p == '9')
    *p = '0';
  ++*p;
  अक्षर *परिणाम = MIN (absnum, p);
  अगर (negative)
    *--परिणाम = '-';
  वापस परिणाम;
}

#घोषणा PRINT_FLOATTYPE(N, T, FTOASTR, BUFSIZE)                         \
अटल व्योम                                                             \
N (T x)                                                                 \
{                                                                       \
  अक्षर buf[BUFSIZE];                                                    \
  FTOASTR (buf, माप buf, FTOASTR_बायाँ_JUSTIFY, 0, x);                \
  माला_दो (buf);                                                           \
}

PRINT_FLOATTYPE (print_FLT, भग्न, ftoastr, FLT_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_DBL, द्विगुन, dtoastr, DBL_BUFSIZE_BOUND)
PRINT_FLOATTYPE (print_LDBL, दीर्घ द्विगुन, ldtoastr, LDBL_BUFSIZE_BOUND)

अटल पूर्णांक
print_errno (व्योम *name, पूर्णांक e)
{
  अक्षर स्थिर *err_name = name ? name : strerrorname_np (e);
  अगर (err_name)
    म_लिखो ("%s=%s\n", err_name,
            quotearg_style (shell_escape_quoting_style, म_त्रुटि (e)));
  वापस 0;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अक्षर limit[1 + MAX (INT_BUFSIZE_BOUND (intmax_t),
                      INT_BUFSIZE_BOUND (uintmax_t))];

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   VERSION, true, usage, AUTHORS,
                                   (अक्षर स्थिर *) शून्य);

#घोषणा print_int(TYPE)                                                  \
  प्र_लिखो (limit + 1, "%ju", (uintmax_t) {TYPE##_MAX});                  \
  म_लिखो (#TYPE"_MAX=%s\n", limit + 1);                                  \
  म_लिखो (#TYPE"_OFLOW=%s\n", decimal_absval_add_one (limit));           \
  अगर (TYPE##_MIN)                                                        \
    {                                                                    \
      प्र_लिखो (limit + 1, "%jd", (intmax_t) {TYPE##_MIN});               \
      म_लिखो (#TYPE"_MIN=%s\n", limit + 1);                              \
      म_लिखो (#TYPE"_UFLOW=%s\n", decimal_absval_add_one (limit));       \
    }

#घोषणा print_float(TYPE)                                                \
  म_लिखो (#TYPE"_MIN="); print_##TYPE (TYPE##_MIN);                      \
  म_लिखो (#TYPE"_MAX="); print_##TYPE (TYPE##_MAX);

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
  म_लिखो ("SIGRTMIN=%jd\n", (intmax_t) {SIGRTMIN});
  म_लिखो ("SIGRTMAX=%jd\n", (intmax_t) {SIGRTMAX});
  म_लिखो ("IO_BUFSIZE=%ju\n", (uintmax_t) {IO_BUFSIZE});

  /* Errnos */
  errno_iterate (print_errno, शून्य);
  /* Common errno aliases */
#अगर defined ENOTEMPTY && ENOTEMPTY == EEXIST
  print_errno ((अक्षर*)"ENOTEMPTY", EEXIST);
#पूर्ण_अगर
#अगर defined ENOTSUP && ENOTSUP == EOPNOTSUPP
  print_errno ((अक्षर*)"ENOTSUP", EOPNOTSUPP);
#पूर्ण_अगर
#अगर defined EWOULDBLOCK && EWOULDBLOCK == EAGAIN
  print_errno ((अक्षर*)"EWOULDBLOCK", EAGAIN);
#पूर्ण_अगर
#अगर defined EDEADLOCK && EDEADLOCK == EDEADLK
  print_errno ((अक्षर*)"EDEADLOCK", EDEADLK);
#पूर्ण_अगर

  वापस निकास_सफल;
}
