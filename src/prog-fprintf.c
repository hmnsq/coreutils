/* prog-fprintf.c - common formatting output functions and definitions
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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdarg.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"

#అన్తర్భావ "prog-fprintf.h"

/* Display program name followed by variable list.
   Used for e.g. verbose output */
శూన్య
prog_fprintf (FILE *fp, అక్షరం స్థిరరాశి *fmt, ...)
{
  va_list ap;
  fputs (కార్యక్రమం_name, fp);
  fputs (": ", fp);
  va_ప్రారంభం (ap, fmt);
  vfprintf (fp, fmt, ap);
  va_చివర (ap);
  fputc ('\n', fp);
}
