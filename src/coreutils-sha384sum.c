/* sha384sum -- wrapper to cksum with the right algorithm.
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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cksum.h"

/* Ensure that the main for cksum is declared even if the tool is not built
   in this single-binary. */
ਪੂਰਨ single_ਦੁਆਧਾਰੀ_main_cksum (ਪੂਰਨ argc, ਅੱਖਰ **argv);
ਪੂਰਨ single_ਦੁਆਧਾਰੀ_main_sha384sum (ਪੂਰਨ argc, ਅੱਖਰ **argv);

ਪੂਰਨ
single_ਦੁਆਧਾਰੀ_main_sha384sum (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  cksum_algorithm = sha384;
  ਵਾਪਸ single_ਦੁਆਧਾਰੀ_main_cksum (argc, argv);
}
