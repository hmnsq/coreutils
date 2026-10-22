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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश "system.h"
#समावेश "true.h"

#घोषणा EXIT_STATUS (true_mode != TRUE_TRUE)
#घोषणा PROGRAM_NAME (true_mode == TRUE_TRUE ? "true" : "false")

#घोषणा AUTHORS proper_name ("Jim Meyering")

व्योम
usage (पूर्णांक status)
{
  म_लिखो (_("\
Usage: %s [ignored command पंक्ति arguments]\n\
  or:  %s OPTION\n\
"),
          program_name, program_name);
  म_लिखो ("%s\n\n",
          _(EXIT_STATUS == निकास_सफल
            ? N_("Exit with a status code indicating success.")
            : N_("Exit with a status code indicating failure.")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (VERSION_OPTION_DESCRIPTION);
  म_लिखो (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
  emit_ancillary_info (PROGRAM_NAME);
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  /* Recognize --help or --version only if it's the only command-line
     argument.  */
  अगर (argc == 2)
    {
      initialize_main (&argc, &argv);
      set_program_name (argv[0]);
      रखो_क्षेत्र (LC_ALL, "");
      bindtextdomain (PACKAGE, LOCALEDIR);
      textdomain (PACKAGE);

      /* Note true(1) will return EXIT_FAILURE in the
         edge case where writes fail with GNU specific options.  */
      निकास_पर (close_stdout);

      अगर (streq (argv[1], "--help"))
        usage (EXIT_STATUS);

      अगर (streq (argv[1], "--version"))
        version_etc (मानक_निकास, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                     (अक्षर *) शून्य);
    }

  वापस EXIT_STATUS;
}
