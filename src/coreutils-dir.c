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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "ls.h"
/* Ensure that the main for ls is declared even if the tool is not being built
   in this single-binary. */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା single_ଦ୍ୱିମିକ_main_ls (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା single_ଦ୍ୱିମିକ_main_dir (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
single_ଦ୍ୱିମିକ_main_dir (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ls_mode = LS_MULTI_COL;
  ଫେରନ୍ତୁ single_ଦ୍ୱିମିକ_main_ls (argc, argv);
}
