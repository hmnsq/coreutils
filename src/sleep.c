/* sleep - delay for a specified amount of time.
   Copyright (C) 1984-2026 Free Software Foundation, Inc.

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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "cl-strtod.h"
#समावेश "dtimespec-bound.h"
#समावेश "long-options.h"
#समावेश "quote.h"
#समावेश "xnanosleep.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "sleep"

#घोषणा AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s संख्या_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause क्रम संख्या_2 seconds, where संख्या_2 is an integer or floating-point.\n\
SUFFIX may be 's','m','h', or 'd', क्रम seconds, minutes, hours, days.\n\
With multiple arguments, छोड़ो क्रम the योग of their मूल्यावली.\n\
\n\
"),
              program_name, program_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

अटल bool
apply_suffix (द्विगुन *x, अक्षर suffix_char)
{
  पूर्णांक multiplier;

  चयन (suffix_char)
    {
    हाल 0:
    हाल 's':
      multiplier = 1;
      अवरोध;
    हाल 'm':
      multiplier = 60;
      अवरोध;
    हाल 'h':
      multiplier = 60 * 60;
      अवरोध;
    हाल 'd':
      multiplier = 60 * 60 * 24;
      अवरोध;
    शेष:
      वापस false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  वापस true;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  द्विगुन seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (अक्षर स्थिर *) शून्य);

  अगर (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  क्रम (पूर्णांक i = optind; i < argc; i++)
    {
      अक्षर *p;
      त्रुटि_सं = 0;
      द्विगुन duration = cl_strtod (argv[i], &p);
      द्विगुन s = dtimespec_bound (duration, त्रुटि_सं);
      अगर (argv[i] == p
          /* Nonnegative interval.  */
          || ! (0 <= s)
          /* No extra chars after the number and an optional s,m,h,d char.  */
          || (*p && *(p + 1))
          /* Check any suffix char and update S based on the suffix.  */
          || ! apply_suffix (&s, *p))
        {
          error (0, 0, _("invalid time interval %s"), quote (argv[i]));
          ok = false;
        }

      seconds = dtimespec_bound (seconds + s, 0);
    }

  अगर (!ok)
    usage (निकास_त्रुटि);

  अगर (xnanosleep (seconds))
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot read realtime clock"));

  वापस निकास_सफल;
}
