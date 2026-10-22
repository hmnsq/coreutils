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

#شامل <config.h>
#شامل <stdarg.h>
#شامل <sys/types.h>

#شامل "system.h"

#شامل "prog-fprintf.h"

/* Display program name followed by variable list.
   Used for e.g. verbose output */
خالی
prog_fprintf (FILE *fp, حرف ثابت *fmt, ...)
{
  va_list ap;
  fputs (پروگرام_name, fp);
  fputs (": ", fp);
  va_آغاز (ap, fmt);
  vfprintf (fp, fmt, ap);
  va_انتہا (ap);
  fputc ('\n', fp);
}
