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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"

#अन्तर्भाव "prog-fprintf.h"

/* Display program name followed by variable list.
   Used for e.g. verbose output */
शून्य
prog_fprintf (खाता *fp, वर्ण ध्रुव *fmt, ...)
{
  बहु_सूची ap;
  ख_माला_दो (program_name, fp);
  ख_माला_दो (": ", fp);
  बहु_शुरू (ap, fmt);
  भख_लिखो (fp, fmt, ap);
  बहु_पूर्ण (ap);
  ख_अक्षर_दो ('\n', fp);
}
