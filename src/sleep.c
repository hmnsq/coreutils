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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "cl-strtod.h"
#అన్తర్భావ "dtimespec-bound.h"
#అన్తర్భావ "long-options.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "xnanosleep.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "sleep"

#నిర్వచించు AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s సంఖ్య_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause కొరకు సంఖ్య_2 seconds, where సంఖ్య_2 is an integer or floating-బిందువు.\n\
SUFFIX may be 's','m','h', or 'd', కొరకు seconds, minutes, hours, days.\n\
With గుణిజం arguments, pause కొరకు the కూడిక యొక్క their విలువలు.\n\
\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

స్థిర bool
apply_suffix (ద్విగుణ *x, అక్షరం suffix_char)
{
  పూర్ణాంకం multiplier;

  ఎంపిక (suffix_char)
    {
    స్థితి 0:
    స్థితి 's':
      multiplier = 1;
      ఆపు;
    స్థితి 'm':
      multiplier = 60;
      ఆపు;
    స్థితి 'h':
      multiplier = 60 * 60;
      ఆపు;
    స్థితి 'd':
      multiplier = 60 * 60 * 24;
      ఆపు;
    అప్రమేయ:
      తిరిగి false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  తిరిగి true;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  ద్విగుణ seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, కార్యక్రమం_NAME, PACKAGE_NAME,
                                   సంస్కరణ, true, usage, AUTHORS,
                                   (అక్షరం స్థిరరాశి *) ఖాళీ);

  ఒకవేళ (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  కొరకు (పూర్ణాంకం i = optind; i < argc; i++)
    {
      అక్షరం *p;
      errno = 0;
      ద్విగుణ duration = cl_strtod (argv[i], &p);
      ద్విగుణ s = dtimespec_bound (duration, errno);
      ఒకవేళ (argv[i] == p
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

  ఒకవేళ (!ok)
    usage (EXIT_FAILURE);

  ఒకవేళ (xnanosleep (seconds))
    error (EXIT_FAILURE, errno, _("cannot read realtime clock"));

  తిరిగి EXIT_SUCCESS;
}
