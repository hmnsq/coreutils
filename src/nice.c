/* nice -- run a program with modified niceness
   Copyright (C) 1990-2026 Free Software Foundation, Inc.

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

/* David MacKenzie <djm@gnu.ai.mit.edu> */

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "system.h"

#अगर ! HAVE_NICE
/* Include this after "system.h" so we're sure to have definitions
   (from time.h or sys/time.h) required for e.g. the ru_utime member.  */
# include <sys/resource.h>
#पूर्ण_अगर

#समावेश "c-ctype.h"
#समावेश "quote.h"
#समावेश "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "nice"

#घोषणा AUTHORS proper_name ("David MacKenzie")

#अगर HAVE_NICE
# define GET_NICENESS() nice (0)
#अन्यथा
# define GET_NICENESS() getpriority (PRIO_PROCESS, 0)
#पूर्ण_अगर

#अगर_अघोषित NZERO
# define NZERO 20
#पूर्ण_अगर

/* This is required for Darwin Kernel Version 7.7.0.  */
#अगर NZERO == 0
# undef  NZERO
# define NZERO 20
#पूर्ण_अगर

अटल काष्ठा option स्थिर longopts[] =
{
  {"adjustment", required_argument, शून्य, 'n'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION] [COMMAND [ARG]...]\n"), program_name);
      म_लिखो (_("\
Run COMMAND with an adjusted niceness, which affects process scheduling.\n\
With no COMMAND, print the current niceness.  Niceness मूल्यावली range from\n\
%d (most favorable to the process) to %d (least favorable to the process).\n\
"),
              - NZERO, NZERO - 1);

      emit_mandatory_arg_note ();

      oputs (_("\
  -n, --adjustment=N\n\
         add integer N to the niceness (शेष 10)\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      म_लिखो (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

अटल bool
perm_related_errno (पूर्णांक err)
{
  वापस err == EACCES || err == EPERM;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक current_niceness;
  पूर्णांक adjustment = 10;
  अक्षर स्थिर *adjustment_given = शून्य;
  bool ok;
  पूर्णांक i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  निकास_पर (close_stdout);

  क्रम (i = 1; i < argc; /* empty */)
    {
      अक्षर स्थिर *s = argv[i];

      अगर (s[0] == '-' && c_isdigit (s[1 + (s[1] == '-' || s[1] == '+')]))
        {
          adjustment_given = s + 1;
          ++i;
        }
      अन्यथा
        {
          पूर्णांक c;
          पूर्णांक fake_argc = argc - (i - 1);
          अक्षर **fake_argv = argv + (i - 1);

          /* Ensure that any getopt diagnostics use the right name.  */
          fake_argv[0] = argv[0];

          /* Initialize getopt_long's internal state.  */
          optind = 0;

          c = getopt_long (fake_argc, fake_argv, "+n:", longopts, शून्य);
          i += optind - 1;

          चयन (c)
            {
            हाल 'n':
              adjustment_given = optarg;
              अवरोध;

            हाल -1:
              अवरोध;

            case_GETOPT_HELP_CHAR;

            case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

            शेष:
              usage (EXIT_CANCELED);
              अवरोध;
            }

          अगर (c == -1)
            अवरोध;
        }
    }

  अगर (adjustment_given)
    {
      /* If the requested adjustment is outside the valid range,
         silently bring it to just within range; this mimics what
         "setpriority" and "nice" do.  */
#अगर (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) only supported 0 to (2 * NZERO - 2) niceness
         until glibc 2.43.  */
      क्रमागत { MIN_ADJUSTMENT = 0, MAX_ADJUSTMENT = 2 * NZERO - 2 };
#अन्यथा
      क्रमागत { MIN_ADJUSTMENT = 1 - 2 * NZERO, MAX_ADJUSTMENT = 2 * NZERO - 1 };
#पूर्ण_अगर
      दीर्घ पूर्णांक tmp;
      अगर (LONGINT_OVERFLOW < xstrtol (adjustment_given, शून्य, 10, &tmp, ""))
        error (EXIT_CANCELED, 0, _("invalid adjustment %s"),
               quote (adjustment_given));
#अगर (defined __gnu_hurd__                                               \
     && (__GLIBC__ < 2 || (__GLIBC__ == 2 && __GLIBC_MINOR__ < 43)))
      /* GNU/Hurd's nice(2) also did not clamp the new niceness into the
         supported range until glibc 2.43.
         See <https://sourceware.org/PR33614>.  */
      त्रुटि_सं = 0;
      current_niceness = GET_NICENESS ();
      अगर (current_niceness == -1 && त्रुटि_सं != 0)
        error (EXIT_CANCELED, त्रुटि_सं, _("cannot get niceness"));
      अगर (tmp < 0)
        {
          पूर्णांक योग;
          अगर (ckd_add (&योग, current_niceness, tmp) || योग < MIN_ADJUSTMENT)
            adjustment = MIN_ADJUSTMENT - current_niceness;
          अन्यथा
            adjustment = tmp;
        }
      अन्यथा
        {
          पूर्णांक योग;
          अगर (ckd_add (&योग, current_niceness, tmp) || MAX_ADJUSTMENT < योग)
            adjustment = MAX_ADJUSTMENT - current_niceness;
          अन्यथा
            adjustment = tmp;
        }
#अन्यथा
      adjustment = MAX (MIN_ADJUSTMENT, MIN (tmp, MAX_ADJUSTMENT));
#पूर्ण_अगर
    }

  अगर (i == argc)
    {
      अगर (adjustment_given)
        {
          error (0, 0, _("a command must be given with an adjustment"));
          usage (EXIT_CANCELED);
        }
      /* No command given; print the niceness.  */
      त्रुटि_सं = 0;
      current_niceness = GET_NICENESS ();
      अगर (current_niceness == -1 && त्रुटि_सं != 0)
        error (EXIT_CANCELED, त्रुटि_सं, _("cannot get niceness"));
      म_लिखो ("%d\n", current_niceness);
      वापस निकास_सफल;
    }

  त्रुटि_सं = 0;
#अगर HAVE_NICE
  ok = (nice (adjustment) != -1 || त्रुटि_सं == 0);
#अन्यथा
  current_niceness = GET_NICENESS ();
  अगर (current_niceness == -1 && त्रुटि_सं != 0)
    error (EXIT_CANCELED, त्रुटि_सं, _("cannot get niceness"));
  ok = (setpriority (PRIO_PROCESS, 0, current_niceness + adjustment) == 0);
#पूर्ण_अगर
  अगर (!ok)
    {
      error (perm_related_errno (त्रुटि_सं) ? 0
             : EXIT_CANCELED, त्रुटि_सं, _("cannot set niceness"));
      /* error() flushes stderr, but does not check for write failure.
         Normally, we would catch this via our atexit() hook of
         close_stdout, but execvp() gets in the way.  If stderr
         encountered a write failure, there is no need to try calling
         error() again.  */
      अगर (ख_त्रुटि (मानक_त्रुटि))
        वापस EXIT_CANCELED;
    }

  execvp (argv[i], &argv[i]);

  पूर्णांक exit_status = त्रुटि_सं == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, त्रुटि_सं, "%s", quote (argv[i]));
  वापस exit_status;
}
