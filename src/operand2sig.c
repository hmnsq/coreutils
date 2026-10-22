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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <sys/wait.h>

#ضمن "system.h"
#ضمن "c-ctype.h"
#ضمن "quote.h"
#ضمن "sig2str.h"
#ضمن "operand2sig.h"

خارجي صحيح
operand2sig (حرف ثابت *عمليو_2)
{
  صحيح signum;

  إذا (c_isdigit (*عمليو_2))
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

      حرف *endp;
      ڊگهو صحيح l = (errno = 0, strtol (عمليو_2, &endp, 10));
      صحيح i = l;
      signum = (عمليو_2 == endp || *endp || errno || i != l ? -1 : i);

      إذا (signum != -1)
        {
          /* Note AIX uses a different bit pattern for status returned
             from shell and wait(), so we can't use WTERMSIG etc. here.
             Also ksh returns 0xFF + signal number.  */
          signum &= signum >= 0xFF ? 0xFF : 0x7F;
        }
    }
  وإلا
    {
      /* Convert signal to upper case in the C locale, not in the
         current locale.  Don't assume ASCII; it might be EBCDIC.  */
      حرف *upcased = xstrdup (عمليو_2);
      لكل (حرف *p = upcased; *p; p++)
        إذا (strchr ("abcdefghijklmnopqrstuvwxyz", *p))
          *p += 'A' - 'a';

      /* Look for the signal name, possibly prefixed by "SIG",
         and possibly lowercased.  */
      إذا (!(str2sig (upcased, &signum) == 0
            || (upcased[0] == 'S' && upcased[1] == 'I' && upcased[2] == 'G'
                && str2sig (upcased + 3, &signum) == 0)))
        signum = -1;

      حرر (upcased);
    }

  إذا (0 > signum || signum > SIGNUM_BOUND)
    {
      error (0, 0, _("%s: invalid signal"), quote (عمليو_2));
      أرجع -1;
    }

  أرجع signum;
}
