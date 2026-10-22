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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/wait.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "sig2str.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "operand2sig.h"

ਬਾਹਰੀ ਪੂਰਨ
operand2sig (ਅੱਖਰ ਅਚਲ *ਸੰਕਾਰਜ_2)
{
  ਪੂਰਨ signum;

  ਜੇ (c_isdigit (*ਸੰਕਾਰਜ_2))
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

      ਅੱਖਰ *endp;
      ਲੰਮਾ ਪੂਰਨ l = (errno = 0, strtol (ਸੰਕਾਰਜ_2, &endp, 10));
      ਪੂਰਨ i = l;
      signum = (ਸੰਕਾਰਜ_2 == endp || *endp || errno || i != l ? -1 : i);

      ਜੇ (signum != -1)
        {
          /* Note AIX uses a different bit pattern for status returned
             from shell and wait(), so we can't use WTERMSIG etc. here.
             Also ksh returns 0xFF + signal number.  */
          signum &= signum >= 0xFF ? 0xFF : 0x7F;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      /* Convert signal to upper case in the C locale, not in the
         current locale.  Don't assume ASCII; it might be EBCDIC.  */
      ਅੱਖਰ *upcased = xstrdup (ਸੰਕਾਰਜ_2);
      ਲਈ (ਅੱਖਰ *p = upcased; *p; p++)
        ਜੇ (strchr ("abcdefghijklmnopqrstuvwxyz", *p))
          *p += 'A' - 'a';

      /* Look for the signal name, possibly prefixed by "SIG",
         and possibly lowercased.  */
      ਜੇ (!(str2sig (upcased, &signum) == 0
            || (upcased[0] == 'S' && upcased[1] == 'I' && upcased[2] == 'G'
                && str2sig (upcased + 3, &signum) == 0)))
        signum = -1;

      ਮੁਕਤ (upcased);
    }

  ਜੇ (0 > signum || signum > SIGNUM_BOUND)
    {
      error (0, 0, _("%s: invalid signal"), quote (ਸੰਕਾਰਜ_2));
      ਵਾਪਸ -1;
    }

  ਵਾਪਸ signum;
}
