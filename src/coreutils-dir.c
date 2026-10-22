/* dir -- wrapper to ls with the right ls_mode.
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

#شامل <config.h>
#شامل "system.h"

#شامل "ls.h"
/* Ensure that the main for ls is declared even if the tool is not being built
   in this single-binary. */
صحیح single_ثنائی_main_ls (صحیح argc, حرف **argv);
صحیح single_ثنائی_main_dir (صحیح argc, حرف **argv);

صحیح
single_ثنائی_main_dir (صحیح argc, حرف **argv)
{
  ls_منوال = LS_MULTI_ستون;
  واپس single_ثنائی_main_ls (argc, argv);
}
