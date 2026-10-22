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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "cl-strtod.h"
#अन्तर्भाव "dtimespec-bound.h"
#अन्तर्भाव "long-options.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "xnanosleep.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sleep"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
    {
      printf (_("\
Usage: %s सङ्ख्या_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause अत्थाय सङ्ख्या_2 seconds, where सङ्ख्या_2 is an integer or floating-point.\n\
SUFFIX may be 's','m','h', or 'd', अत्थाय seconds, minutes, hours, days.\n\
With multiple arguments, pause अत्थाय the sum of their मानानि.\n\
\n\
"),
              program_name, program_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

स्थिर bool
apply_suffix (द्विगुण *x, वर्ण suffix_char)
{
  पूर्णाङ्क multiplier;

  विकल्प (suffix_char)
    {
    पक्ष 0:
    पक्ष 's':
      multiplier = 1;
      भङ्ग;
    पक्ष 'm':
      multiplier = 60;
      भङ्ग;
    पक्ष 'h':
      multiplier = 60 * 60;
      भङ्ग;
    पक्ष 'd':
      multiplier = 60 * 60 * 24;
      भङ्ग;
    स्वभाव:
      पटिनिवत्त false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  पटिनिवत्त true;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  द्विगुण seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (वर्ण ध्रुव *) रिक्त);

  सचे (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  अत्थाय (पूर्णाङ्क i = optind; i < argc; i++)
    {
      वर्ण *p;
      errno = 0;
      द्विगुण duration = cl_strtod (argv[i], &p);
      द्विगुण s = dtimespec_bound (duration, errno);
      सचे (argv[i] == p
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

  सचे (!ok)
    usage (EXIT_FAILURE);

  सचे (xnanosleep (seconds))
    error (EXIT_FAILURE, errno, _("cannot read realtime clock"));

  पटिनिवत्त EXIT_SUCCESS;
}
