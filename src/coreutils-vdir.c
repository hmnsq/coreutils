/* vdir -- wrapper to ls with the right ls_mode.
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

#समावेश <config.h>
#समावेश "system.h"

#समावेश "ls.h"
/* Ensure that the main for ls is declared even if the tool is not being built
   in this single-binary. */
पूर्णांक single_द्विआधारी_main_ls (पूर्णांक argc, अक्षर **argv);
पूर्णांक single_द्विआधारी_main_vdir (पूर्णांक argc, अक्षर **argv);

पूर्णांक
single_द्विआधारी_main_vdir (पूर्णांक argc, अक्षर **argv)
{
  ls_mode = LS_LONG_FORMAT;
  वापस single_द्विआधारी_main_ls (argc, argv);
}
