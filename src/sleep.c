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
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s संख्या_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause प्रति संख्या_2 seconds, where संख्या_2 is an integer or floating-point.\n\
SUFFIX may be 's','m','h', or 'd', प्रति seconds, minutes, hours, days.\n\
With multiple arguments, छोड़ो प्रति the sum of their मूल्यावली.\n\
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
      प्रत्यागम false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  प्रत्यागम true;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  द्विगुण seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (वर्ण ध्रुव *) रिक्त);

  यदि (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  प्रति (पूर्णाङ्क i = optind; i < argc; i++)
    {
      वर्ण *p;
      त्रुटि_सं = 0;
      द्विगुण duration = cl_strtod (argv[i], &p);
      द्विगुण s = dtimespec_bound (duration, त्रुटि_सं);
      यदि (argv[i] == p
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

  यदि (!ok)
    usage (निकास_त्रुटि);

  यदि (xnanosleep (seconds))
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot read realtime clock"));

  प्रत्यागम निकास_सफल;
}
