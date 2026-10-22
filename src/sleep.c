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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "cl-strtod.h"
#অন্তর্ভাব "dtimespec-bound.h"
#অন্তর্ভাব "long-options.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "xnanosleep.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sleep"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s সংখ্যা_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause প্রতি সংখ্যা_2 seconds, where সংখ্যা_2 is an integer or floating-point.\n\
SUFFIX may be 's','m','h', or 'd', প্রতি seconds, minutes, hours, days.\n\
With multiple arguments, pause প্রতি the sum of their মানসমূহ.\n\
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

স্থির bool
apply_suffix (দ্বিগুণ *x, বর্ণ suffix_char)
{
  পূর্ণাঙ্ক multiplier;

  বিকল্প (suffix_char)
    {
    পক্ষ 0:
    পক্ষ 's':
      multiplier = 1;
      ভঙ্গ;
    পক্ষ 'm':
      multiplier = 60;
      ভঙ্গ;
    পক্ষ 'h':
      multiplier = 60 * 60;
      ভঙ্গ;
    পক্ষ 'd':
      multiplier = 60 * 60 * 24;
      ভঙ্গ;
    স্বভাব:
      প্রত্যাগম false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  প্রত্যাগম true;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  দ্বিগুণ seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (বর্ণ ধ্রুব *) রিক্ত);

  যদি (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  প্রতি (পূর্ণাঙ্ক i = optind; i < argc; i++)
    {
      বর্ণ *p;
      errno = 0;
      দ্বিগুণ duration = cl_strtod (argv[i], &p);
      দ্বিগুণ s = dtimespec_bound (duration, errno);
      যদি (argv[i] == p
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

  যদি (!ok)
    usage (EXIT_FAILURE);

  যদি (xnanosleep (seconds))
    error (EXIT_FAILURE, errno, _("cannot read realtime clock"));

  প্রত্যাগম EXIT_SUCCESS;
}
