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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "long-options.h"
#অন্তর্ভাব "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "link"

#define AUTHORS proper_name ("Michael Stone")

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s FILE1 FILE2\n\
  or:  %s OPTION\n"), program_name, program_name);
      fputs (_("Call the link function to create a link named FILE2\
 to an existing FILE1.\n\n"),
             stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (বর্ণ ধ্রুব *) রিক্ত);

  যদি (argc < optind + 2)
    {
      যদি (argc < optind + 1)
        error (0, 0, _("missing operand"));
      অন্যথা
        error (0, 0, _("missing operand after %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  যদি (optind + 2 < argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
      usage (EXIT_FAILURE);
    }

  যদি (link (argv[optind], argv[optind + 1]) != 0)
    error (EXIT_FAILURE, errno, _("cannot create link %s to %s"),
           quoteaf_n (0, argv[optind + 1]), quoteaf_n (1, argv[optind]));

  প্রত্যাগম EXIT_SUCCESS;
}
