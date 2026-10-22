/* hostname - set or print the name of current host system
   Copyright (C) 1994-2026 Free Software Foundation, Inc.

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

/* Written by Jim Meyering.  */

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "long-options.h"
#ضمن "quote.h"
#ضمن "xgethostname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "hostname"

#تعریف_کن AUTHORS proper_name ("Jim Meyering")

#اگر_تعریف_نشده HAVE_SETHOSTNAME
# إذا defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
#  include <sys/systeminfo.h>
# endif

ساكن صحيح
sethostname (حرف ثابت *name, size_t namelen)
{
# إذا defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
  /* Using sysinfo() is the SVR4 mechanism to set a hostname. */
  أرجع (sysinfo (SI_مجموعه_HOSTNAME, name, namelen) < 0 ? -1 : 0);
# وإلا
  errno = ENOTSUP;
  أرجع -1;
# endif
}
#پایان_اگر

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [NAME]\n\
  or:  %s OPTION\n\
Print or مجموعه_2 the hostname از the current system.\n\
\n\
"),
             برنامه_name, برنامه_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  حرف *hostname;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, برنامه_NAME, PACKAGE_NAME,
                                   نسخه, true, usage, AUTHORS,
                                   (حرف ثابت *) عدم);

  إذا (optind + 1 < argc)
     {
       error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
       usage (EXIT_FAILURE);
     }

  إذا (optind + 1 == argc)
    {
      /* Set hostname to operand.  */
      حرف ثابت *name = argv[optind];
      إذا (sethostname (name, طول_رشته (name)) != 0)
        error (EXIT_FAILURE, errno, _("cannot set name to %s"),
               quote (name));
    }
  وإلا
    {
      hostname = xgethostname ();
      إذا (hostname == عدم)
        error (EXIT_FAILURE, errno, _("cannot determine hostname"));
      puts (hostname);
    }

  main_exit (EXIT_SUCCESS);
}
