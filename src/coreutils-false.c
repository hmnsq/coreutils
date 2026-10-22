/* false -- wrapper to true with the right true_mode.
   Copyright (C) 2026 Free Software Foundation, Inc.

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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "true.h"
/* Ensure that the main for true is declared even if the tool is not being
   built in this single-binary. */
പൂർണ്ണസംഖ്യ single_ദ്വിമാന_main_true (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv);
പൂർണ്ണസംഖ്യ single_ദ്വിമാന_main_false (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv);

പൂർണ്ണസംഖ്യ
single_ദ്വിമാന_main_false (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  true_mode = TRUE_FALSE;
  തിരികെ single_ദ്വിമാന_main_true (argc, argv);
}
