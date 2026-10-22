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
#define PROGRAM_NAME "sleep"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s عدد_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause برای عدد_2 seconds, where عدد_2 is an integer or floating-point.\n\
SUFFIX may be 's','m','h', or 'd', برای seconds, minutes, hours, days.\n\
With multiple arguments, pause برای the sum of their مقادیر.\n\
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

ایستا bool
apply_suffix (دوگانه *x, نویسه suffix_char)
{
  صحیح multiplier;

  انتخاب (suffix_char)
    {
    حالت 0:
    حالت 's':
      multiplier = 1;
      بشکن;
    حالت 'm':
      multiplier = 60;
      بشکن;
    حالت 'h':
      multiplier = 60 * 60;
      بشکن;
    حالت 'd':
      multiplier = 60 * 60 * 24;
      بشکن;
    پیشفرض:
      بازگشت false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  بازگشت true;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  دوگانه seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (نویسه ثابت *) پوچ);

  اگر (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  برای (صحیح i = optind; i < argc; i++)
    {
      نویسه *p;
      errno = 0;
      دوگانه duration = cl_strtod (argv[i], &p);
      دوگانه s = dtimespec_bound (duration, errno);
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

  بازگشت EXIT_SUCCESS;
}
