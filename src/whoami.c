/* whoami -- print effective userid

   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Equivalent to 'id -un'. */
/* Written by Richard Mlynarik. */

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <pwd.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "long-options.h"
#अन्तर्भाव "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "whoami"

#define AUTHORS proper_name ("Richard Mlynarik")

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]...\n"), program_name);
      ख_माला_दो (_("\
Print the user name associated with the current effective user ID.\n\
Same as id -un.\n\
\n\
"), मानक_निकास);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  uid_t NO_UID = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (अक्षर स्थिरांक *) रिक्त);

  जर (optind != argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (निकास_त्रुटि);
    }

  त्रुटि_सं = 0;
  uid_t uid = geteuid ();
  संरचना passwd *pw = uid == NO_UID && त्रुटि_सं ? रिक्त : getpwuid (uid);
  जर (!pw)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot find name for user ID %ju"),
           (uintmax_t) uid);
  माला_दो (pw->pw_name);
  परत निकास_सफल;
}
