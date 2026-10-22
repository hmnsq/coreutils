/* sha384sum -- wrapper to cksum with the right algorithm.
   Copyright (C) 2025-2026 Free Software Foundation, Inc.

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

#अन्तर्भाव <config.h>
#अन्तर्भाव "system.h"
#अन्तर्भाव "cksum.h"

/* Ensure that the main for cksum is declared even if the tool is not built
   in this single-binary. */
पूर्णाङ्क single_द्विआधारी_main_cksum (पूर्णाङ्क argc, वर्ण **argv);
पूर्णाङ्क single_द्विआधारी_main_sha384sum (पूर्णाङ्क argc, वर्ण **argv);

पूर्णाङ्क
single_द्विआधारी_main_sha384sum (पूर्णाङ्क argc, वर्ण **argv)
{
  cksum_algorithm = sha384;
  प्रत्यागम single_द्विआधारी_main_cksum (argc, argv);
}
