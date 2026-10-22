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

#شامل <config.h>
#شامل "system.h"

#شامل "true.h"
/* Ensure that the main for true is declared even if the tool is not being
   built in this single-binary. */
صحیح single_ثنائی_main_true (صحیح argc, حرف **argv);
صحیح single_ثنائی_main_false (صحیح argc, حرف **argv);

صحیح
single_ثنائی_main_false (صحیح argc, حرف **argv)
{
  true_منوال = TRUE_FALSE;
  واپس single_ثنائی_main_true (argc, argv);
}
