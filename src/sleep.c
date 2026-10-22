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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "cl-strtod.h"
#شامل "dtimespec-bound.h"
#شامل "long-options.h"
#شامل "quote.h"
#شامل "xnanosleep.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "sleep"

#تعریف_کرو AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s نمبر_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause برائے نمبر_2 seconds, where نمبر_2 is an integer or floating-نقطہ.\n\
SUFFIX may be 's','m','h', or 'd', برائے seconds, minutes, hours, days.\n\
With مضرب arguments, pause برائے the مجموع کا their اقدار.\n\
\n\
"),
              پروگرام_name, پروگرام_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

ساکن bool
apply_suffix (دوہرا *x, حرف suffix_char)
{
  صحیح multiplier;

  انتخاب (suffix_char)
    {
    حالت 0:
    حالت 's':
      multiplier = 1;
      توڑو;
    حالت 'm':
      multiplier = 60;
      توڑو;
    حالت 'h':
      multiplier = 60 * 60;
      توڑو;
    حالت 'd':
      multiplier = 60 * 60 * 24;
      توڑو;
    طےشدہ:
      واپس false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  واپس true;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  دوہرا seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE_NAME,
                                   نسخہ, true, usage, AUTHORS,
                                   (حرف ثابت *) تہی);

  اگر (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  برائے (صحیح i = optind; i < argc; i++)
    {
      حرف *p;
      errno = 0;
      دوہرا duration = cl_strtod (argv[i], &p);
      دوہرا s = dtimespec_bound (duration, errno);
      اگر (argv[i] == p
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

  اگر (!ok)
    usage (EXIT_FAILURE);

  اگر (xnanosleep (seconds))
    error (EXIT_FAILURE, errno, _("cannot read realtime clock"));

  واپس EXIT_SUCCESS;
}
