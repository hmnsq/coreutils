/* Exit with a status code indicating success.
   Copyright (C) 1999-2026 Free Software Foundation, Inc.

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

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include "system.h"
#include "true.h"

#define EXIT_STATUS (true_mode != TRUE_TRUE)
#define PROGRAM_NAME (true_mode == TRUE_TRUE ? "true" : "false")

#define AUTHORS proper_name ("Jim Meyering")

void
usage (පූර්ණ_සංඛ්‍යා status)
{
  printf (_("\
Usage: %s [ignored command line arguments]\n\
  or:  %s OPTION\n\
"),
          program_name, program_name);
  printf ("%s\n\n",
          _(EXIT_STATUS == EXIT_SUCCESS
            ? N_("Exit with a status code indicating success.")
            : N_("Exit with a status code indicating failure.")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (VERSION_OPTION_DESCRIPTION);
  printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
  emit_ancillary_info (PROGRAM_NAME);
  exit (status);
}

පූර්ණ_සංඛ්‍යා
main (පූර්ණ_සංඛ්‍යා argc, var_na **argv)
{
  /* Recognize --help or --version only if it's the only command-line
     argument.  */
  නම් (argc == 2)
    {
      initialize_main (&argc, &argv);
      set_program_name (argv[0]);
      setlocale (LC_ALL, "");
      bindtextdomain (PACKAGE, LOCALEDIR);
      textdomain (PACKAGE);

      /* Note true(1) will return EXIT_FAILURE in the
         edge case where writes fail with GNU specific options.  */
      atexit (close_stdout);

      නම් (streq (argv[1], "--help"))
        usage (EXIT_STATUS);

      නම් (streq (argv[1], "--version"))
        version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                     (var_na *) NULL);
    }

  ආපසු EXIT_STATUS;
}
