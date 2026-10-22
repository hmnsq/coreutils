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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "cl-strtod.h"
#ಅನ್ತರ್ಭಾವ "dtimespec-bound.h"
#ಅನ್ತರ್ಭಾವ "long-options.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xnanosleep.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "sleep"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s ಸಂಖ್ಯೆ_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause ಗಾಗಿ ಸಂಖ್ಯೆ_2 seconds, where ಸಂಖ್ಯೆ_2 is an integer or floating-ಬಿಂದು.\n\
SUFFIX may be 's','m','h', or 'd', ಗಾಗಿ seconds, minutes, hours, days.\n\
With ಅಪವರ್ತ್ಯ arguments, pause ಗಾಗಿ the ಮೊತ್ತ ದ their ಮೌಲ್ಯಗಳು.\n\
\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

ಸ್ಥಿರ bool
apply_suffix (ದ್ವಿಗುಣ *x, ಅಕ್ಷರ suffix_char)
{
  ಪೂರ್ಣಾಂಕ multiplier;

  ಆಯ್ಕೆ (suffix_char)
    {
    ಸ್ಥಿತಿ 0:
    ಸ್ಥಿತಿ 's':
      multiplier = 1;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'm':
      multiplier = 60;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'h':
      multiplier = 60 * 60;
      ನಿಲ್ಲಿಸು;
    ಸ್ಥಿತಿ 'd':
      multiplier = 60 * 60 * 24;
      ನಿಲ್ಲಿಸು;
    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  ಹಿಂತಿರುಗಿ true;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ದ್ವಿಗುಣ seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, ಕಾರ್ಯಕ್ರಮ_NAME, PACKAGE_NAME,
                                   ಆವೃತ್ತಿ, true, usage, AUTHORS,
                                   (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) ಖಾಲಿ);

  ಆದರೆ (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = optind; i < argc; i++)
    {
      ಅಕ್ಷರ *p;
      errno = 0;
      ದ್ವಿಗುಣ duration = cl_strtod (argv[i], &p);
      ದ್ವಿಗುಣ s = dtimespec_bound (duration, errno);
      ಆದರೆ (argv[i] == p
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

  ಆದರೆ (!ok)
    usage (EXIT_FAILURE);

  ಆದರೆ (xnanosleep (seconds))
    error (EXIT_FAILURE, errno, _("cannot read realtime clock"));

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
