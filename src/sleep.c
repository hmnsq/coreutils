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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "cl-strtod.h"
#અન્તર્ભાવ "dtimespec-bound.h"
#અન્તર્ભાવ "long-options.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "xnanosleep.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sleep"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s સંખ્યા_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause માટે સંખ્યા_2 seconds, where સંખ્યા_2 is an integer or floating-point.\n\
SUFFIX may be 's','m','h', or 'd', માટે seconds, minutes, hours, days.\n\
With multiple arguments, pause માટે the sum of their કિંમતો.\n\
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

સ્થિર bool
apply_suffix (દ્વિગુણ *x, અક્ષર suffix_char)
{
  પૂર્ણાંક multiplier;

  પસંદગી (suffix_char)
    {
    સ્થિતિ 0:
    સ્થિતિ 's':
      multiplier = 1;
      તોડો;
    સ્થિતિ 'm':
      multiplier = 60;
      તોડો;
    સ્થિતિ 'h':
      multiplier = 60 * 60;
      તોડો;
    સ્થિતિ 'd':
      multiplier = 60 * 60 * 24;
      તોડો;
    મૂળભૂત:
      પાછા false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  પાછા true;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  દ્વિગુણ seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (અક્ષર અચલ *) ખાલી);

  જો (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  માટે (પૂર્ણાંક i = optind; i < argc; i++)
    {
      અક્ષર *p;
      errno = 0;
      દ્વિગુણ duration = cl_strtod (argv[i], &p);
      દ્વિગુણ s = dtimespec_bound (duration, errno);
      જો (argv[i] == p
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

  જો (!ok)
    usage (EXIT_FAILURE);

  જો (xnanosleep (seconds))
    error (EXIT_FAILURE, errno, _("cannot read realtime clock"));

  પાછા EXIT_SUCCESS;
}
