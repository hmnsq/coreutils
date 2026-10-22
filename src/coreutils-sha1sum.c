/* sha1sum -- wrapper to cksum with the right algorithm.
   Copyright (C) 2025-2026 Free Software Foundation, Inc.

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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "cksum.h"

/* Ensure that the main for cksum is declared even if the tool is not built
   in this single-binary. */
முழு single_இருமம்_main_cksum (முழு argc, எழுத்து **argv);
முழு single_இருமம்_main_sha1sum (முழு argc, எழுத்து **argv);

முழு
single_இருமம்_main_sha1sum (முழு argc, எழுத்து **argv)
{
  cksum_algorithm = sha1;
  திரும்பு single_இருமம்_main_cksum (argc, argv);
}
