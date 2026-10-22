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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "long-options.h"
#समावेश "quote.h"
#समावेश "xgethostname.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "hostname"

#घोषणा AUTHORS proper_name ("Jim Meyering")

#अगर_अघोषित HAVE_SETHOSTNAME
# अगर defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
#  include <sys/systeminfo.h>
# endif

अटल पूर्णांक
sethostname (अक्षर स्थिर *name, माप_प्रकार namelen)
{
# अगर defined HAVE_SYSINFO && defined HAVE_SYS_SYSTEMINFO_H
  /* Using sysinfo() is the SVR4 mechanism to set a hostname. */
  वापस (sysinfo (SI_SET_HOSTNAME, name, namelen) < 0 ? -1 : 0);
# अन्यथा
  त्रुटि_सं = ENOTSUP;
  वापस -1;
# endif
}
#पूर्ण_अगर

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [NAME]\n\
  or:  %s OPTION\n\
Print or set the hostname of the current प्रणाली.\n\
\n\
"),
             program_name, program_name);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अक्षर *hostname;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (अक्षर स्थिर *) शून्य);

  अगर (optind + 1 < argc)
     {
       error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
       usage (निकास_त्रुटि);
     }

  अगर (optind + 1 == argc)
    {
      /* Set hostname to operand.  */
      अक्षर स्थिर *name = argv[optind];
      अगर (sethostname (name, म_माप (name)) != 0)
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot set name to %s"),
               quote (name));
    }
  अन्यथा
    {
      hostname = xgethostname ();
      अगर (hostname == शून्य)
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot determine hostname"));
      माला_दो (hostname);
    }

  main_exit (निकास_सफल);
}
