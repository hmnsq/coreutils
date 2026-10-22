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

#समावेश <config.h>
#समावेश <stdarg.h>
#समावेश <sys/types.h>

#समावेश "system.h"

#समावेश "prog-fprintf.h"

/* Display program name followed by variable list.
   Used for e.g. verbose output */
व्योम
prog_fprintf (खाता *fp, अक्षर स्थिर *fmt, ...)
{
  बहु_सूची ap;
  ख_माला_दो (program_name, fp);
  ख_माला_दो (": ", fp);
  बहु_शुरू (ap, fmt);
  भख_लिखो (fp, fmt, ap);
  बहु_पूर्ण (ap);
  ख_अक्षर_दो ('\n', fp);
}
