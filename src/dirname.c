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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ಅನ್ತರ್ಭಾವ "system.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "dirname"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

ಸ್ಥಿರ bool quote_output;

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"zero", no_ವಾದ, ಖಾಲಿ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION] NAME...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Output each NAME with its ಕೊನೆಯ non-slash component and trailing slashes\n\
removed; ಆದರೆ NAME ಒಳಗೊಂಡಿದೆ no /'s, output '.' (meaning the current directory).\n\
\n\
"), stdout);
      oputs (_("\
  -z, --zero\n\
         ಕೊನೆ each output ಸಾಲು with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      ಬರೆ (_("\
\n\
Examples:\n\
  %s /usr/bin/          -> \"/usr\"\n\
  %s dir1/str dir2/str  -> \"dir1\" followed by \"dir2\"\n\
  %s stdio.h            -> \".\"\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv, "z", longopts, ಖಾಲಿ);

      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'z':
          use_nuls = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }


  ಆದರೆ (!use_nuls && isatty (STDOUT_FILENO))
    {
      ಪೂರ್ಣಾಂಕ qs = getenv_quoting_style ();
      ಆದರೆ (qs < 0)
        qs = shell_escape_quoting_style;
      ಆದರೆ (qs != literal_quoting_style)
        {
          ಗಣ_quoting_style (ಖಾಲಿ, qs);
          quote_output = true;
        }
    }

  ಗಾಗಿ (; optind < argc; optind++)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಫಲಿತಾಂಶ = argv[optind];
      idx_t len = dir_len (ಫಲಿತಾಂಶ);

      ಆದರೆ (! len)
        {
          ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ dot = '.';
          ಫಲಿತಾಂಶ = &dot;
          len = 1;
        }
      ಆದರೆ (quote_output)
        {
          ಫಲಿತಾಂಶ = quoteN_mem (ಫಲಿತಾಂಶ, len);
          len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಫಲಿತಾಂಶ);
        }
      fwrite (ಫಲಿತಾಂಶ, 1, len, stdout);
      ಅಕ್ಷರ_ಬರೆ (use_nuls ? '\0' :'\n');
    }

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
