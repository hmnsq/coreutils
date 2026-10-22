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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "cl-strtod.h"
#ضمن "dtimespec-bound.h"
#ضمن "long-options.h"
#ضمن "quote.h"
#ضمن "xnanosleep.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "sleep"

#وصف_ڪريو AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s عدد_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause لكل عدد_2 seconds, where عدد_2 is an integer or floating-نقطو.\n\
SUFFIX may be 's','m','h', or 'd', لكل seconds, minutes, hours, days.\n\
With گڻو arguments, pause لكل the جوڙ جو their قدرون.\n\
\n\
"),
              پروگرام_name, پروگرام_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

ساكن bool
apply_suffix (مزدوج *x, حرف suffix_char)
{
  صحيح multiplier;

  بدل (suffix_char)
    {
    حالة 0:
    حالة 's':
      multiplier = 1;
      اكسر;
    حالة 'm':
      multiplier = 60;
      اكسر;
    حالة 'h':
      multiplier = 60 * 60;
      اكسر;
    حالة 'd':
      multiplier = 60 * 60 * 24;
      اكسر;
    افتراضي:
      أرجع false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  أرجع true;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  مزدوج seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE_NAME,
                                   نسخو, true, usage, AUTHORS,
                                   (حرف ثابت *) عدم);

  إذا (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  لكل (صحيح i = optind; i < argc; i++)
    {
      حرف *p;
      errno = 0;
      مزدوج duration = cl_strtod (argv[i], &p);
      مزدوج s = dtimespec_bound (duration, errno);
      إذا (argv[i] == p
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

  إذا (!ok)
    usage (EXIT_FAILURE);

  إذا (xnanosleep (seconds))
    error (EXIT_FAILURE, errno, _("cannot read realtime clock"));

  أرجع EXIT_SUCCESS;
}
