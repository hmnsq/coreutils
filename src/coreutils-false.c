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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "true.h"
/* Ensure that the main for true is declared even if the tool is not being
   built in this single-binary. */
ಪೂರ್ಣಾಂಕ single_ದ್ವಿಮಾನ_main_true (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv);
ಪೂರ್ಣಾಂಕ single_ದ್ವಿಮಾನ_main_false (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv);

ಪೂರ್ಣಾಂಕ
single_ದ್ವಿಮಾನ_main_false (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  true_ಬಹುಲಕ = TRUE_FALSE;
  ಹಿಂತಿರುಗಿ single_ದ್ವಿಮಾನ_main_true (argc, argv);
}
