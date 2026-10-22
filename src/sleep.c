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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "cl-strtod.h"
#അന്തര്ഭാവ "dtimespec-bound.h"
#അന്തര്ഭാവ "long-options.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "xnanosleep.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sleep"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s സംഖ്യ_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause വേണ്ടി സംഖ്യ_2 seconds, where സംഖ്യ_2 is an integer or floating-point.\n\
SUFFIX may be 's','m','h', or 'd', വേണ്ടി seconds, minutes, hours, days.\n\
With multiple arguments, pause വേണ്ടി the sum of their മൂല്യങ്ങൾ.\n\
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

സ്ഥിതം bool
apply_suffix (ഇരട്ട *x, അക്ഷരം suffix_char)
{
  പൂർണ്ണസംഖ്യ multiplier;

  തിരഞ്ഞെടുപ്പ് (suffix_char)
    {
    അവസ്ഥ 0:
    അവസ്ഥ 's':
      multiplier = 1;
      നിർത്തുക;
    അവസ്ഥ 'm':
      multiplier = 60;
      നിർത്തുക;
    അവസ്ഥ 'h':
      multiplier = 60 * 60;
      നിർത്തുക;
    അവസ്ഥ 'd':
      multiplier = 60 * 60 * 24;
      നിർത്തുക;
    സ്ഥിരസ്ഥിതി:
      തിരികെ false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  തിരികെ true;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  ഇരട്ട seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (അക്ഷരം സ്ഥിരം *) ശൂന്യ);

  എങ്കിൽ (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  വേണ്ടി (പൂർണ്ണസംഖ്യ i = optind; i < argc; i++)
    {
      അക്ഷരം *p;
      errno = 0;
      ഇരട്ട duration = cl_strtod (argv[i], &p);
      ഇരട്ട s = dtimespec_bound (duration, errno);
      എങ്കിൽ (argv[i] == p
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

  എങ്കിൽ (!ok)
    usage (EXIT_FAILURE);

  എങ്കിൽ (xnanosleep (seconds))
    error (EXIT_FAILURE, errno, _("cannot read realtime clock"));

  തിരികെ EXIT_SUCCESS;
}
