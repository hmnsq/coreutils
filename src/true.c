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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"
#अन्तर्भाव "true.h"

#परिभाषय EXIT_स्थितिः (true_बहुलकम् != TRUE_TRUE)
#परिभाषय क्रमादेशः_NAME (true_बहुलकम् == TRUE_TRUE ? "true" : "false")

#परिभाषय AUTHORS proper_name ("Jim Meyering")

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  प्रारूप_लिख (_("\
Usage: %s [ignored command पङ्क्तिः arguments]\n\
  or:  %s OPTION\n\
"),
          क्रमादेशः_name, क्रमादेशः_name);
  प्रारूप_लिख ("%s\n\n",
          _(EXIT_स्थितिः == EXIT_SUCCESS
            ? N_("Exit with a status code indicating success.")
            : N_("Exit with a status code indicating failure.")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (संस्करणम्_OPTION_DESCRIPTION);
  प्रारूप_लिख (USAGE_BUILTIN_WARNING, क्रमादेशः_NAME);
  emit_ancillary_info (क्रमादेशः_NAME);
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  /* Recognize --help or --version only if it's the only command-line
     argument.  */
  यदि (argc == 2)
    {
      initialize_main (&argc, &argv);
      समुच्चयः_क्रमादेशः_name (argv[0]);
      setlocale (LC_ALL, "");
      bindtextdomain (PACKAGE, LOCALEDIR);
      textdomain (PACKAGE);

      /* Note true(1) will return EXIT_FAILURE in the
         edge case where writes fail with GNU specific options.  */
      atexit (close_stdout);

      यदि (streq (argv[1], "--help"))
        usage (EXIT_स्थितिः);

      यदि (streq (argv[1], "--version"))
        संस्करणम्_etc (stdout, क्रमादेशः_NAME, PACKAGE_NAME, संस्करणम्, AUTHORS,
                     (वर्ण *) रिक्त);
    }

  प्रत्यागम EXIT_स्थितिः;
}
