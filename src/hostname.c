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
#define PROGRAM_NAME "hostname"

#define AUTHORS proper_name ("Jim Meyering")

#ifndef HAVE_SETHOSTNAME
# اگر defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
#  include <sys/systeminfo.h>
# endif

ایستا صحیح
sethostname (نویسه ثابت *name, size_t namelen)
{
# اگر defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
  /* Using sysinfo() is the SVR4 mechanism to set a hostname. */
  بازگشت (sysinfo (SI_SET_HOSTNAME, name, namelen) < 0 ? -1 : 0);
# وگرنه
  errno = ENOTSUP;
  بازگشت -1;
# endif
}
#endif

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [NAME]\n\
  or:  %s OPTION\n\
Print or set the hostname of the current system.\n\
\n\
"),
             program_name, program_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  نویسه *hostname;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (نویسه ثابت *) پوچ);

  اگر (optind + 1 < argc)
     {
       error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
       usage (EXIT_FAILURE);
     }

  اگر (optind + 1 == argc)
    {
      /* Set hostname to operand.  */
      نویسه ثابت *name = argv[optind];
      اگر (sethostname (name, strlen (name)) != 0)
        error (EXIT_FAILURE, errno, _("cannot set name to %s"),
               quote (name));
    }
  وگرنه
    {
      hostname = xgethostname ();
      اگر (hostname == پوچ)
        error (EXIT_FAILURE, errno, _("cannot determine hostname"));
      puts (hostname);
    }

  main_exit (EXIT_SUCCESS);
}
