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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل "system.h"
#شامل "true.h"

#تعریف_کرو EXIT_حالت (true_منوال != TRUE_TRUE)
#تعریف_کرو پروگرام_NAME (true_منوال == TRUE_TRUE ? "true" : "false")

#تعریف_کرو AUTHORS proper_name ("Jim Meyering")

خالی
usage (صحیح حالت_2)
{
  لکھو (_("\
Usage: %s [ignored command سطر arguments]\n\
  or:  %s OPTION\n\
"),
          پروگرام_name, پروگرام_name);
  لکھو ("%s\n\n",
          _(EXIT_حالت == EXIT_SUCCESS
            ? N_("Exit with a status code indicating success.")
            : N_("Exit with a status code indicating failure.")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (نسخہ_OPTION_DESCRIPTION);
  لکھو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
  emit_ancillary_info (پروگرام_NAME);
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  /* Recognize --help or --version only if it's the only command-line
     argument.  */
  اگر (argc == 2)
    {
      initialize_main (&argc, &argv);
      سیٹ_پروگرام_name (argv[0]);
      setlocale (LC_ALL, "");
      bindtextdomain (PACKAGE, LOCALEDIR);
      textdomain (PACKAGE);

      /* Note true(1) will return EXIT_FAILURE in the
         edge case where writes fail with GNU specific options.  */
      atexit (close_stdout);

      اگر (streq (argv[1], "--help"))
        usage (EXIT_حالت);

      اگر (streq (argv[1], "--version"))
        نسخہ_etc (stdout, پروگرام_NAME, PACKAGE_NAME, نسخہ, AUTHORS,
                     (حرف *) تہی);
    }

  واپس EXIT_حالت;
}
