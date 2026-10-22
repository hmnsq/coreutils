/* operand2sig.c -- common function for parsing signal specifications
   Copyright (C) 2008-2026 Free Software Foundation, Inc.

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

/* Extracted from kill.c/timeout.c by Pádraig Brady.
   FIXME: Move this to gnulib/str2sig.c */


/* Convert OPERAND to a signal number.  Return the signal number, or -1 if
   unsuccessful.  */

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <sys/wait.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "sig2str.h"
#అన్తర్భావ "operand2sig.h"

బాహ్య పూర్ణాంకం
operand2sig (అక్షరం స్థిరరాశి *నిర్వాహ్యం_2)
{
  పూర్ణాంకం signum;

  ఒకవేళ (c_isdigit (*నిర్వాహ్యం_2))
    {
      /* Note we don't put a limit on the maximum value passed,
         because we're checking shell $? values here, and ksh for
         example will add 256 to the signal value, thus being wider
         than the number of WEXITSTATUS bits.
         We could validate that values were not above say
         ((WEXITSTATUS (~0) << 1) + 1), which would cater for ksh.
         But some shells may use other adjustments in future to be
         (forward) compatible with systems that support
         wider exit status values as discussed at
         https://austingroupbugs.net/view.php?id=947  */

      అక్షరం *endp;
      దీర్ఘ పూర్ణాంకం l = (errno = 0, strtol (నిర్వాహ్యం_2, &endp, 10));
      పూర్ణాంకం i = l;
      signum = (నిర్వాహ్యం_2 == endp || *endp || errno || i != l ? -1 : i);

      ఒకవేళ (signum != -1)
        {
          /* Note AIX uses a different bit pattern for status returned
             from shell and wait(), so we can't use WTERMSIG etc. here.
             Also ksh returns 0xFF + signal number.  */
          signum &= signum >= 0xFF ? 0xFF : 0x7F;
        }
    }
  లేకపోతే
    {
      /* Convert signal to upper case in the C locale, not in the
         current locale.  Don't assume ASCII; it might be EBCDIC.  */
      అక్షరం *upcased = xstrdup (నిర్వాహ్యం_2);
      కొరకు (అక్షరం *p = upcased; *p; p++)
        ఒకవేళ (strchr ("abcdefghijklmnopqrstuvwxyz", *p))
          *p += 'A' - 'a';

      /* Look for the signal name, possibly prefixed by "SIG",
         and possibly lowercased.  */
      ఒకవేళ (!(str2sig (upcased, &signum) == 0
            || (upcased[0] == 'S' && upcased[1] == 'I' && upcased[2] == 'G'
                && str2sig (upcased + 3, &signum) == 0)))
        signum = -1;

      విడుదల (upcased);
    }

  ఒకవేళ (0 > signum || signum > SIGNUM_BOUND)
    {
      error (0, 0, _("%s: invalid signal"), quote (నిర్వాహ్యం_2));
      తిరిగి -1;
    }

  తిరిగి signum;
}
