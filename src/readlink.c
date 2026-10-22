/* readlink -- display value of a symbolic link.
   Copyright (C) 2002-2026 Free Software Foundation, Inc.

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

/* Written by Dmitry V. Levin */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "canonicalize.h"
#ಅನ್ತರ್ಭಾವ "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "readlink"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
ಸ್ಥಿರ bool no_newline;

/* If true, report error messages.  */
ಸ್ಥಿರ bool verbose;

/* If true, quote output according to the selected quoting style.  */
ಸ್ಥಿರ bool quote_output;

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"canonicalize", no_ವಾದ, ಖಾಲಿ, 'f'},
  {"canonicalize-existing", no_ವಾದ, ಖಾಲಿ, 'e'},
  {"canonicalize-missing", no_ವಾದ, ಖಾಲಿ, 'm'},
  {"no-newline", no_ವಾದ, ಖಾಲಿ, 'n'},
  {"quiet", no_ವಾದ, ಖಾಲಿ, 'q'},
  {"silent", no_ವಾದ, ಖಾಲಿ, 's'},
  {"verbose", no_ವಾದ, ಖಾಲಿ, 'v'},
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
      ಬರೆ (_("Usage: %s [OPTION]... FILE...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("Print value of a symbolic link or canonical file name\n\n"),
             stdout);
      oputs (_("\
  -f, --canonicalize\n\
         canonicalize by following every symlink\n\
         ಒಳ every component ದ the given name recursively;\n\
         all but the ಕೊನೆಯ component must exist\n\
"));
      oputs (_("\
  -e, --canonicalize-existing\n\
         canonicalize by following every symlink\n\
         ಒಳ every component ದ the given name recursively;\n\
         all components must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing\n\
         canonicalize by following every symlink\n\
         ಒಳ every component ದ the given name recursively,\n\
         without requirements on components existence\n\
"));
      oputs (_("\
  -n, --no-newline\n\
         ಮಾಡು not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by ಪೂರ್ವನಿಯೋಜಿತ ಆದರೆ POSIXLY_CORRECT is not ಗಣ_2)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by ಪೂರ್ವನಿಯೋಜಿತ ಆದರೆ POSIXLY_CORRECT is ಗಣ_2)\n\
"));
      oputs (_("\
  -z, --zero\n\
         ಕೊನೆ each output ಸಾಲು with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  /* If not -1, use this method to canonicalize.  */
  ಪೂರ್ಣಾಂಕ can_ಬಹುಲಕ = -1;
  ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2 = EXIT_SUCCESS;
  ಪೂರ್ಣಾಂಕ optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'e':
          can_ಬಹುಲಕ = CAN_EXISTING;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'f':
          can_ಬಹುಲಕ = CAN_ALL_BUT_ಕೊನೆಯ;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'm':
          can_ಬಹುಲಕ = CAN_MISSING;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'n':
          no_newline = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'q':
        ಸ್ಥಿತಿ 's':
          verbose = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'v':
          verbose = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'z':
          use_nuls = true;
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (argc - optind > 1)
    {
      ಆದರೆ (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
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

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  ಆದರೆ (getenv ("POSIXLY_CORRECT") != ಖಾಲಿ)
    verbose = true;

  ಗಾಗಿ (; optind < argc; ++optind)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fname = argv[optind];
      ಅಕ್ಷರ *ಮೌಲ್ಯ = (can_ಬಹುಲಕ != -1
                     ? canonicalize_filename_ಬಹುಲಕ (fname, can_ಬಹುಲಕ)
                     : areadlink_with_ಗಾತ್ರ (fname, 63));
      ಆದರೆ (ಮೌಲ್ಯ)
        {
          fputs (quote_output ? quoteN (ಮೌಲ್ಯ) : ಮೌಲ್ಯ, stdout);
          ಆದರೆ (! no_newline)
            ಅಕ್ಷರ_ಬರೆ (use_nuls ? '\0' : '\n');
          ಬಿಡುಗಡೆ (ಮೌಲ್ಯ);
          ಆದರೆ (ferror (stdout))
            write_error ();
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಸ್ಥಿತಿ_2 = EXIT_FAILURE;
          ಆದರೆ (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  ಹಿಂತಿರುಗಿ ಸ್ಥಿತಿ_2;
}
