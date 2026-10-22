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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "uname.h"
/* Ensure that the main for uname is declared even if the tool is not being
   built in this single-binary. */
முழு single_இருமம்_main_uname (முழு argc, எழுத்து **argv);
முழு single_இருமம்_main_arch (முழு argc, எழுத்து **argv);

முழு
single_இருமம்_main_arch (முழு argc, எழுத்து **argv)
{
  uname_mode = UNAME_ARCH;
  திரும்பு single_இருமம்_main_uname (argc, argv);
}
