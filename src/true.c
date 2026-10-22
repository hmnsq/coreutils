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

#تعریف_کن EXIT_وضعیت (true_نما != TRUE_TRUE)
#تعریف_کن برنامه_NAME (true_نما == TRUE_TRUE ? "true" : "false")

#تعریف_کن AUTHORS proper_name ("Jim Meyering")

فراغ
usage (صحيح وضعیت)
{
  چاپ_کن (_("\
Usage: %s [ignored command سطر arguments]\n\
  or:  %s OPTION\n\
"),
          برنامه_name, برنامه_name);
  چاپ_کن ("%s\n\n",
          _(EXIT_وضعیت == EXIT_SUCCESS
            ? N_("Exit with a status code indicating success.")
            : N_("Exit with a status code indicating failure.")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (نسخه_OPTION_DESCRIPTION);
  چاپ_کن (USAGE_BUILTIN_WARNING, برنامه_NAME);
  emit_ancillary_info (برنامه_NAME);
  خروج (وضعیت);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  /* Recognize --help or --version only if it's the only command-line
     argument.  */
  إذا (argc == 2)
    {
      initialize_main (&argc, &argv);
      مجموعه_برنامه_name (argv[0]);
      setlocale (LC_ALL, "");
      bindtextdomain (PACKAGE, LOCALEDIR);
      textdomain (PACKAGE);

      /* Note true(1) will return EXIT_FAILURE in the
         edge case where writes fail with GNU specific options.  */
      atexit (close_stdout);

      إذا (streq (argv[1], "--help"))
        usage (EXIT_وضعیت);

      إذا (streq (argv[1], "--version"))
        نسخه_etc (stdout, برنامه_NAME, PACKAGE_NAME, نسخه, AUTHORS,
                     (حرف *) عدم);
    }

  أرجع EXIT_وضعیت;
}
