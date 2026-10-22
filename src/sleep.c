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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "cl-strtod.h"
#ଅନ୍ତର୍ଭାଵ "dtimespec-bound.h"
#ଅନ୍ତର୍ଭାଵ "long-options.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "xnanosleep.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sleep"

#define AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s ସଂଖ୍ୟା_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause ପାଇଁ ସଂଖ୍ୟା_2 seconds, where ସଂଖ୍ୟା_2 is an integer or floating-point.\n\
SUFFIX may be 's','m','h', or 'd', ପାଇଁ seconds, minutes, hours, days.\n\
With multiple arguments, pause ପାଇଁ the sum of their ମୂଲ୍ୟଗୁଡ଼ିକ.\n\
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

ସ୍ଥିର bool
apply_suffix (ଦ୍ୱିଗୁଣ *x, ଅକ୍ଷର suffix_char)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା multiplier;

  ବାଛ (suffix_char)
    {
    ସ୍ଥିତି 0:
    ସ୍ଥିତି 's':
      multiplier = 1;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'm':
      multiplier = 60;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'h':
      multiplier = 60 * 60;
      ଭାଙ୍ଗ;
    ସ୍ଥିତି 'd':
      multiplier = 60 * 60 * 24;
      ଭାଙ୍ଗ;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  ଫେରନ୍ତୁ true;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଦ୍ୱିଗୁଣ seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (ଅକ୍ଷର ଅଚଳ *) ଖାଲି);

  ଯଦି (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = optind; i < argc; i++)
    {
      ଅକ୍ଷର *p;
      errno = 0;
      ଦ୍ୱିଗୁଣ duration = cl_strtod (argv[i], &p);
      ଦ୍ୱିଗୁଣ s = dtimespec_bound (duration, errno);
      ଯଦି (argv[i] == p
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

  ଯଦି (!ok)
    usage (EXIT_FAILURE);

  ଯଦି (xnanosleep (seconds))
    error (EXIT_FAILURE, errno, _("cannot read realtime clock"));

  ଫେରନ୍ତୁ EXIT_SUCCESS;
}
