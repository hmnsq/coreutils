/* link utility for GNU.
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

   Simply call the system 'link' function */

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "long-options.h"
#अन्तर्भाव "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "link"

#define AUTHORS proper_name ("Michael Stone")

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s FILE1 FILE2\n\
  or:  %s OPTION\n"), program_name, program_name);
      ख_माला_दो (_("Call the link function to create a link named FILE2\
 to an existing FILE1.\n\n"),
             मानक_निकास);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (वर्ण ध्रुव *) रिक्त);

  यदि (argc < optind + 2)
    {
      यदि (argc < optind + 1)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[optind]));
      usage (निकास_त्रुटि);
    }

  यदि (optind + 2 < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (निकास_त्रुटि);
    }

  यदि (link (argv[optind], argv[optind + 1]) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot create link %s to %s"),
           quoteaf_n (0, argv[optind + 1]), quoteaf_n (1, argv[optind]));

  प्रत्यागम निकास_सफल;
}
