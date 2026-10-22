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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#અન્તર્ભાવ "system.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dirname"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

સ્થિર bool quote_output;

સ્થિર સંરચના option અચલ longopts[] =
{
  {"zero", no_argument, ખાલી, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION] NAME...\n\
"),
              program_name);
      fputs (_("\
Output each NAME with its last non-slash component and trailing slashes\n\
removed; જો NAME contains no /'s, output '.' (meaning the current directory).\n\
\n\
"), stdout);
      oputs (_("\
  -z, --zero\n\
         end each output લીટી with NUL, not newline\n\
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

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે (true)
    {
      પૂર્ણાંક c = getopt_long (argc, argv, "z", longopts, ખાલી);

      જો (c == -1)
        તોડો;

      પસંદગી (c)
        {
        સ્થિતિ 'z':
          use_nuls = true;
          તોડો;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }


  જો (!use_nuls && isatty (STDOUT_FILENO))
    {
      પૂર્ણાંક qs = getenv_quoting_style ();
      જો (qs < 0)
        qs = shell_escape_quoting_style;
      જો (qs != literal_quoting_style)
        {
          set_quoting_style (ખાલી, qs);
          quote_output = true;
        }
    }

  માટે (; optind < argc; optind++)
    {
      અક્ષર અચલ *result = argv[optind];
      idx_t len = dir_len (result);

      જો (! len)
        {
          સ્થિર અક્ષર અચલ dot = '.';
          result = &dot;
          len = 1;
        }
      જો (quote_output)
        {
          result = quoteN_mem (result, len);
          len = strlen (result);
        }
      fwrite (result, 1, len, stdout);
      putchar (use_nuls ? '\0' :'\n');
    }

  પાછા EXIT_SUCCESS;
}
