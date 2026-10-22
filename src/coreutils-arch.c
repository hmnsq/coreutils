/* arch -- wrapper to uname with the right uname_mode.
   Copyright (C) 2014-2026 Free Software Foundation, Inc.

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

/* Written by Alex Deymo <deymo@chromium.org>.  */

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "uname.h"
/* Ensure that the main for uname is declared even if the tool is not being
   built in this single-binary. */
પૂર્ણાંક single_દ્વિઅંકી_main_uname (પૂર્ણાંક argc, અક્ષર **argv);
પૂર્ણાંક single_દ્વિઅંકી_main_arch (પૂર્ણાંક argc, અક્ષર **argv);

પૂર્ણાંક
single_દ્વિઅંકી_main_arch (પૂર્ણાંક argc, અક્ષર **argv)
{
  uname_mode = UNAME_ARCH;
  પાછા single_દ્વિઅંકી_main_uname (argc, argv);
}
