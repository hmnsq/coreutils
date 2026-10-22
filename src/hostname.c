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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "long-options.h"
#شامل "quote.h"
#شامل "xgethostname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "hostname"

#تعریف_کرو AUTHORS proper_name ("Jim Meyering")

#اگر_غیر_تعریف_شدہ HAVE_SETHOSTNAME
# اگر defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
#  include <sys/systeminfo.h>
# endif

ساکن صحیح
sethostname (حرف ثابت *name, size_t namelen)
{
# اگر defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
  /* Using sysinfo() is the SVR4 mechanism to set a hostname. */
  واپس (sysinfo (SI_سیٹ_HOSTNAME, name, namelen) < 0 ? -1 : 0);
# ورنہ
  errno = ENOTSUP;
  واپس -1;
# endif
}
#اگر_ختم

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [NAME]\n\
  or:  %s OPTION\n\
Print or سیٹ_2 the hostname کا the current system.\n\
\n\
"),
             پروگرام_name, پروگرام_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  حرف *hostname;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE_NAME,
                                   نسخہ, true, usage, AUTHORS,
                                   (حرف ثابت *) تہی);

  اگر (optind + 1 < argc)
     {
       error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
       usage (EXIT_FAILURE);
     }

  اگر (optind + 1 == argc)
    {
      /* Set hostname to operand.  */
      حرف ثابت *name = argv[optind];
      اگر (sethostname (name, لڑی_لمبائی (name)) != 0)
        error (EXIT_FAILURE, errno, _("cannot set name to %s"),
               quote (name));
    }
  ورنہ
    {
      hostname = xgethostname ();
      اگر (hostname == تہی)
        error (EXIT_FAILURE, errno, _("cannot determine hostname"));
      puts (hostname);
    }

  main_exit (EXIT_SUCCESS);
}
