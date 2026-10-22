/* unlink utility for GNU.
   Copyright (C) 2001-2026 Free Software Foundation, Inc.

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

/* Written by Michael Stone */

/* Implementation overview:

   Simply call the system 'unlink' function */

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "long-options.h"
#अन्तर्भाव "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "unlink"

#define AUTHORS proper_name ("Michael Stone")

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s खाता\n\
  or:  %s OPTION\n"), program_name, program_name);
      ख_माला_दो (_("Call the unlink function to remove the specified FILE.\n\n"),
             मानक_निकास);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (अक्षर स्थिरांक *) रिक्त);

  जर (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  जर (optind + 1 < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (निकास_त्रुटि);
    }

  जर (unlink (argv[optind]) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot unlink %s"), quoteaf (argv[optind]));

  परत निकास_सफल;
}
