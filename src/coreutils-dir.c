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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "ls.h"
/* Ensure that the main for ls is declared even if the tool is not being built
   in this single-binary. */
ಪೂರ್ಣಾಂಕ single_ದ್ವಿಮಾನ_main_ls (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv);
ಪೂರ್ಣಾಂಕ single_ದ್ವಿಮಾನ_main_dir (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv);

ಪೂರ್ಣಾಂಕ
single_ದ್ವಿಮಾನ_main_dir (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ls_ಬಹುಲಕ = LS_MULTI_ಲಂಬಸಾಲು;
  ಹಿಂತಿರುಗಿ single_ದ್ವಿಮಾನ_main_ls (argc, argv);
}
