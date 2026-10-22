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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cl-strtod.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dtimespec-bound.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "long-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xnanosleep.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "sleep"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Jim Meyering"), \
  proper_name ("Paul Eggert")

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s ਸੰਖਿਆ_2[SUFFIX]...\n\
  or:  %s OPTION\n\
Pause ਲਈ ਸੰਖਿਆ_2 seconds, where ਸੰਖਿਆ_2 is an integer or floating-ਬਿੰਦੂ.\n\
SUFFIX may be 's','m','h', or 'd', ਲਈ seconds, minutes, hours, days.\n\
With ਗੁਣਜ arguments, pause ਲਈ the ਜੋੜ ਦਾ their ਮੁੱਲਾਂ.\n\
\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Given a floating point value *X, and a suffix character, SUFFIX_CHAR,
   scale *X by the multiplier implied by SUFFIX_CHAR.  SUFFIX_CHAR may
   be the NUL byte or 's' to denote seconds, 'm' for minutes, 'h' for
   hours, or 'd' for days.  If SUFFIX_CHAR is invalid, don't modify *X
   and return false.  Otherwise return true.  */

ਸਥਿਰ bool
apply_suffix (ਦੁੱਗਣਾ *x, ਅੱਖਰ suffix_char)
{
  ਪੂਰਨ multiplier;

  ਚੋਣ (suffix_char)
    {
    ਹਾਲਤ 0:
    ਹਾਲਤ 's':
      multiplier = 1;
      ਰੋਕੋ;
    ਹਾਲਤ 'm':
      multiplier = 60;
      ਰੋਕੋ;
    ਹਾਲਤ 'h':
      multiplier = 60 * 60;
      ਰੋਕੋ;
    ਹਾਲਤ 'd':
      multiplier = 60 * 60 * 24;
      ਰੋਕੋ;
    ਮੂਲ:
      ਵਾਪਸ false;
    }

  *x = dtimespec_bound (*x * multiplier, 0);

  ਵਾਪਸ true;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਦੁੱਗਣਾ seconds = 0.0;
  bool ok = true;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, ਪ੍ਰੋਗਰਾਮ_NAME, PACKAGE_NAME,
                                   ਸੰਸਕਰਣ, true, usage, AUTHORS,
                                   (ਅੱਖਰ ਅਚਲ *) ਨਿਰਾ);

  ਜੇ (argc == 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ਲਈ (ਪੂਰਨ i = optind; i < argc; i++)
    {
      ਅੱਖਰ *p;
      errno = 0;
      ਦੁੱਗਣਾ duration = cl_strtod (argv[i], &p);
      ਦੁੱਗਣਾ s = dtimespec_bound (duration, errno);
      ਜੇ (argv[i] == p
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

  ਜੇ (!ok)
    usage (EXIT_FAILURE);

  ਜੇ (xnanosleep (seconds))
    error (EXIT_FAILURE, errno, _("cannot read realtime clock"));

  ਵਾਪਸ EXIT_SUCCESS;
}
