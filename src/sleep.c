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
#define PROGRAM_NAME "sleep"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s ಸಂಖ್ಯೆ_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause ಪ್ರತಿ ಸಂಖ್ಯೆ_2 seconds, where ಸಂಖ್ಯೆ_2 is an integer or floating-point.\n\
SUFFIX may be 's','m','h', or 'd', ಪ್ರತಿ seconds, minutes, hours, days.\n\
With multiple arguments, pause ಪ್ರತಿ the sum of their ಮೌಲ್ಯಗಳು.\n\
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

ಸ್ಥಿರ bool
apply_suffix (ದ್ವಿಗುಣ *x, ವರ್ಣ suffix_char)
{
  ಪೂರ್ಣಾಙ್ಕ multiplier;

  ವಿಕಲ್ಪ (suffix_char)
    {
    ಪಕ್ಷ 0:
    ಪಕ್ಷ 's':
      multiplier = 1;
      ಭಙ್ಗ;
    ಪಕ್ಷ 'm':
      multiplier = 60;
      ಭಙ್ಗ;
    ಪಕ್ಷ 'h':
      multiplier = 60 * 60;
      ಭಙ್ಗ;
    ಪಕ್ಷ 'd':
      multiplier = 60 * 60 * 24;
      ಭಙ್ಗ;
    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  ಪ್ರತ್ಯಾಗಮ true;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ದ್ವಿಗುಣ seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (ವರ್ಣ ಧ್ರುವ *) ರಿಕ್ತ);

  ಯದಿ (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = optind; i < argc; i++)
    {
      ವರ್ಣ *p;
      errno = 0;
      ದ್ವಿಗುಣ duration = cl_strtod (argv[i], &p);
      ದ್ವಿಗುಣ s = dtimespec_bound (duration, errno);
      ಯದಿ (argv[i] == p
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

  ಯದಿ (!ok)
    usage (EXIT_FAILURE);

  ಯದಿ (xnanosleep (seconds))
    error (EXIT_FAILURE, errno, _("cannot read realtime clock"));

  ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
}
