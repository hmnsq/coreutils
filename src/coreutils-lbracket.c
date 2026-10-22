/* lbracket -- wrapper to test with the right test_mode.
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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "test.h"
/* Ensure that the main for test is declared even if the tool is not being
   built in this single-binary. */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା single_ଦ୍ୱିମିକ_main_test (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା single_ଦ୍ୱିମିକ_main_lbracket (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv);

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
single_ଦ୍ୱିମିକ_main_lbracket (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  test_mode = TEST_LBRACKET;
  ଫେରନ୍ତୁ single_ଦ୍ୱିମିକ_main_test (argc, argv);
}
