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

#అన్తర్భావ <config.h>
#అన్తర్భావ "system.h"

#అన్తర్భావ "true.h"
/* Ensure that the main for true is declared even if the tool is not being
   built in this single-binary. */
పూర్ణాంకం single_ద్విమాన_main_true (పూర్ణాంకం argc, అక్షరం **argv);
పూర్ణాంకం single_ద్విమాన_main_false (పూర్ణాంకం argc, అక్షరం **argv);

పూర్ణాంకం
single_ద్విమాన_main_false (పూర్ణాంకం argc, అక్షరం **argv)
{
  true_బహుళకం = TRUE_FALSE;
  తిరిగి single_ద్విమాన_main_true (argc, argv);
}
