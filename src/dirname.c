/* dirname -- strip suffix from file name

   Copyright (C) 1990-2026 Free Software Foundation, Inc.

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

/* Written by David MacKenzie and Jim Meyering. */

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#অন্তর্ভাব "system.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dirname"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

স্থির bool quote_output;

স্থির সংরচনা option ধ্রুব longopts[] =
{
  {"zero", no_argument, রিক্ত, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION] NAME...\n\
"),
              program_name);
      fputs (_("\
Output each NAME with its last non-slash component and trailing slashes\n\
removed; যদি NAME contains no /'s, output '.' (meaning the current directory).\n\
\n\
"), stdout);
      oputs (_("\
  -z, --zero\n\
         end each output সারি with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
Examples:\n\
  %s /usr/bin/          -> \"/usr\"\n\
  %s dir1/str dir2/str  -> \"dir1\" followed by \"dir2\"\n\
  %s stdio.h            -> \".\"\n\
"),
              program_name, program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যাবত্ (true)
    {
      পূর্ণাঙ্ক c = getopt_long (argc, argv, "z", longopts, রিক্ত);

      যদি (c == -1)
        ভঙ্গ;

      বিকল্প (c)
        {
        পক্ষ 'z':
          use_nuls = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্বভাব:
          usage (EXIT_FAILURE);
        }
    }

  যদি (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }


  যদি (!use_nuls && isatty (STDOUT_FILENO))
    {
      পূর্ণাঙ্ক qs = getenv_quoting_style ();
      যদি (qs < 0)
        qs = shell_escape_quoting_style;
      যদি (qs != literal_quoting_style)
        {
          set_quoting_style (রিক্ত, qs);
          quote_output = true;
        }
    }

  প্রতি (; optind < argc; optind++)
    {
      বর্ণ ধ্রুব *result = argv[optind];
      idx_t len = dir_len (result);

      যদি (! len)
        {
          স্থির বর্ণ ধ্রুব dot = '.';
          result = &dot;
          len = 1;
        }
      যদি (quote_output)
        {
          result = quoteN_mem (result, len);
          len = strlen (result);
        }
      fwrite (result, 1, len, stdout);
      putchar (use_nuls ? '\0' :'\n');
    }

  প্রত্যাগম EXIT_SUCCESS;
}
