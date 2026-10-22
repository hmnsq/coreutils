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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "cl-strtod.h"
#அந்தர்பாவ "dtimespec-bound.h"
#அந்தர்பாவ "long-options.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "xnanosleep.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sleep"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s எண்_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause க்கு எண்_2 seconds, where எண்_2 is an integer or floating-point.\n\
SUFFIX may be 's','m','h', or 'd', க்கு seconds, minutes, hours, days.\n\
With multiple arguments, pause க்கு the sum of their மதிப்புகள்.\n\
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

நிலையான bool
apply_suffix (இரட்டை *x, எழுத்து suffix_char)
{
  முழு multiplier;

  தேர்வு (suffix_char)
    {
    நிலை 0:
    நிலை 's':
      multiplier = 1;
      நிறுத்து;
    நிலை 'm':
      multiplier = 60;
      நிறுத்து;
    நிலை 'h':
      multiplier = 60 * 60;
      நிறுத்து;
    நிலை 'd':
      multiplier = 60 * 60 * 24;
      நிறுத்து;
    இயல்பு:
      திரும்பு false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  திரும்பு true;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  இரட்டை seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (எழுத்து மாறிலி *) வெற்று);

  எனில் (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  க்கு (முழு i = optind; i < argc; i++)
    {
      எழுத்து *p;
      errno = 0;
      இரட்டை duration = cl_strtod (argv[i], &p);
      இரட்டை s = dtimespec_bound (duration, errno);
      எனில் (argv[i] == p
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

  எனில் (!ok)
    usage (EXIT_FAILURE);

  எனில் (xnanosleep (seconds))
    error (EXIT_FAILURE, errno, _("cannot read realtime clock"));

  திரும்பு EXIT_SUCCESS;
}
