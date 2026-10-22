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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن "system.h"
#ضمن "true.h"

#تعریف_کړه EXIT_حالت (true_ډېرپېښ != TRUE_TRUE)
#تعریف_کړه پروګرام_NAME (true_ډېرپېښ == TRUE_TRUE ? "true" : "false")

#تعریف_کړه AUTHORS proper_name ("Jim Meyering")

فراغ
usage (صحيح حالت)
{
  چاپ_کړه (_("\
Usage: %s [ignored command کرښه arguments]\n\
  or:  %s OPTION\n\
"),
          پروګرام_name, پروګرام_name);
  چاپ_کړه ("%s\n\n",
          _(EXIT_حالت == EXIT_SUCCESS
            ? N_("Exit with a status code indicating success.")
            : N_("Exit with a status code indicating failure.")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (نسخه_OPTION_DESCRIPTION);
  چاپ_کړه (USAGE_BUILTIN_WARNING, پروګرام_NAME);
  emit_ancillary_info (پروګرام_NAME);
  وتل (حالت);
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  /* Recognize --help or --version only if it's the only command-line
     argument.  */
  إذا (argc == 2)
    {
      initialize_main (&argc, &argv);
      سېټ_پروګرام_name (argv[0]);
      setlocale (LC_ALL, "");
      bindtextdomain (PACKAGE, LOCALEDIR);
      textdomain (PACKAGE);

      /* Note true(1) will return EXIT_FAILURE in the
         edge case where writes fail with GNU specific options.  */
      atexit (close_stdout);

      إذا (streq (argv[1], "--help"))
        usage (EXIT_حالت);

      إذا (streq (argv[1], "--version"))
        نسخه_etc (stdout, پروګرام_NAME, PACKAGE_NAME, نسخه, AUTHORS,
                     (حرف *) عدم);
    }

  أرجع EXIT_حالت;
}
