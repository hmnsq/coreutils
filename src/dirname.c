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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#അന്തര്ഭാവ "system.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dirname"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

സ്ഥിതം bool quote_output;

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"zero", no_argument, ശൂന്യ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION] NAME...\n\
"),
              program_name);
      fputs (_("\
Output each NAME with its last non-slash component and trailing slashes\n\
removed; എങ്കിൽ NAME contains no /'s, output '.' (meaning the current directory).\n\
\n\
"), stdout);
      oputs (_("\
  -z, --zero\n\
         end each output വരി with NUL, not newline\n\
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ (true)
    {
      പൂർണ്ണസംഖ്യ c = getopt_long (argc, argv, "z", longopts, ശൂന്യ);

      എങ്കിൽ (c == -1)
        നിർത്തുക;

      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'z':
          use_nuls = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }


  എങ്കിൽ (!use_nuls && isatty (STDOUT_FILENO))
    {
      പൂർണ്ണസംഖ്യ qs = getenv_quoting_style ();
      എങ്കിൽ (qs < 0)
        qs = shell_escape_quoting_style;
      എങ്കിൽ (qs != literal_quoting_style)
        {
          set_quoting_style (ശൂന്യ, qs);
          quote_output = true;
        }
    }

  വേണ്ടി (; optind < argc; optind++)
    {
      അക്ഷരം സ്ഥിരം *result = argv[optind];
      idx_t len = dir_len (result);

      എങ്കിൽ (! len)
        {
          സ്ഥിതം അക്ഷരം സ്ഥിരം dot = '.';
          result = &dot;
          len = 1;
        }
      എങ്കിൽ (quote_output)
        {
          result = quoteN_mem (result, len);
          len = strlen (result);
        }
      fwrite (result, 1, len, stdout);
      putchar (use_nuls ? '\0' :'\n');
    }

  തിരികെ EXIT_SUCCESS;
}
