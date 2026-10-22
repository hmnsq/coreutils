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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "true.h"

#ವ್ಯಾಖ್ಯಾನಿಸು EXIT_ಸ್ಥಿತಿ (true_ಬಹುಲಕ != TRUE_TRUE)
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME (true_ಬಹುಲಕ == TRUE_TRUE ? "true" : "false")

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Jim Meyering")

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಬರೆ (_("\
Usage: %s [ignored command ಸಾಲು arguments]\n\
  or:  %s OPTION\n\
"),
          ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
  ಬರೆ ("%s\n\n",
          _(EXIT_ಸ್ಥಿತಿ == EXIT_SUCCESS
            ? N_("Exit with a status code indicating success.")
            : N_("Exit with a status code indicating failure.")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
  ಬರೆ (USAGE_BUILTIN_WARNING, ಕಾರ್ಯಕ್ರಮ_NAME);
  emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  /* Recognize --help or --version only if it's the only command-line
     argument.  */
  ಆದರೆ (argc == 2)
    {
      initialize_main (&argc, &argv);
      ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
      setlocale (LC_ALL, "");
      bindtextdomain (PACKAGE, LOCALEDIR);
      textdomain (PACKAGE);

      /* Note true(1) will return EXIT_FAILURE in the
         edge case where writes fail with GNU specific options.  */
      atexit (close_stdout);

      ಆದರೆ (streq (argv[1], "--help"))
        usage (EXIT_ಸ್ಥಿತಿ);

      ಆದರೆ (streq (argv[1], "--version"))
        ಆವೃತ್ತಿ_etc (stdout, ಕಾರ್ಯಕ್ರಮ_NAME, PACKAGE_NAME, ಆವೃತ್ತಿ, AUTHORS,
                     (ಅಕ್ಷರ *) ಖಾಲಿ);
    }

  ಹಿಂತಿರುಗಿ EXIT_ಸ್ಥಿತಿ;
}
