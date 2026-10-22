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

#అన్తర్భావ <config.h>
#అన్తర్భావ "system.h"

#అన్తర్భావ "ls.h"
/* Ensure that the main for ls is declared even if the tool is not being built
   in this single-binary. */
పూర్ణాంకం single_ద్విమాన_main_ls (పూర్ణాంకం argc, అక్షరం **argv);
పూర్ణాంకం single_ద్విమాన_main_dir (పూర్ణాంకం argc, అక్షరం **argv);

పూర్ణాంకం
single_ద్విమాన_main_dir (పూర్ణాంకం argc, అక్షరం **argv)
{
  ls_బహుళకం = LS_MULTI_నిలువు;
  తిరిగి single_ద్విమాన_main_ls (argc, argv);
}
