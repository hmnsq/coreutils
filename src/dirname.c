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

#include <config.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>

#include "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#include "system.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dirname"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

static bool quote_output;

static sa_nracanaa option const longopts[] =
{
  {"zero", no_argument, NULL, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (puur_naa_nka status)
{
  ᱡᱩᱫᱤ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᱵᱟᱝ
    {
      printf (_("\
Usage: %s [OPTION] NAME...\n\
"),
              program_name);
      fputs (_("\
Output each NAME with its last non-slash component and trailing slashes\n\
removed; ᱡᱩᱫᱤ NAME contains no /'s, output '.' (meaning the current directory).\n\
\n\
"), stdout);
      oputs (_("\
  -z, --zero\n\
         end each output line with NUL, not newline\n\
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

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ᱡᱚᱠᱷᱚᱱ (true)
    {
      puur_naa_nka c = getopt_long (argc, argv, "z", longopts, NULL);

      ᱡᱩᱫᱤ (c == -1)
        bha_nga;

      switch (c)
        {
        pak_sa 'z':
          use_nuls = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  ᱡᱩᱫᱤ (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }


  ᱡᱩᱫᱤ (!use_nuls && isatty (STDOUT_FILENO))
    {
      puur_naa_nka qs = getenv_quoting_style ();
      ᱡᱩᱫᱤ (qs < 0)
        qs = shell_escape_quoting_style;
      ᱡᱩᱫᱤ (qs != literal_quoting_style)
        {
          set_quoting_style (NULL, qs);
          quote_output = true;
        }
    }

  for (; optind < argc; optind++)
    {
      var_na const *result = argv[optind];
      idx_t len = dir_len (result);

      ᱡᱩᱫᱤ (! len)
        {
          static var_na const dot = '.';
          result = &dot;
          len = 1;
        }
      ᱡᱩᱫᱤ (quote_output)
        {
          result = quoteN_mem (result, len);
          len = strlen (result);
        }
      fwrite (result, 1, len, stdout);
      putchar (use_nuls ? '\0' :'\n');
    }

  ᱨᱩᱣᱟᱹᱲ EXIT_SUCCESS;
}
