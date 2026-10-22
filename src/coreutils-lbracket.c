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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "system.h"

#অন্তর্ভাৱ "test.h"
/* Ensure that the main for test is declared even if the tool is not being
   built in this single-binary. */
পূর্ণাঙ্ক single_দ্বিমিক_main_test (পূর্ণাঙ্ক argc, ৱর্ণ **argv);
পূর্ণাঙ্ক single_দ্বিমিক_main_lbracket (পূর্ণাঙ্ক argc, ৱর্ণ **argv);

পূর্ণাঙ্ক
single_দ্বিমিক_main_lbracket (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  test_mode = TEST_LBRACKET;
  ঘূৰাই single_দ্বিমিক_main_test (argc, argv);
}
