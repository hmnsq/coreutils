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
#define PROGRAM_NAME "readlink"

#define AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
ಸ್ಥಿರ bool no_newline;

/* If true, report error messages.  */
ಸ್ಥಿರ bool verbose;

/* If true, quote output according to the selected quoting style.  */
ಸ್ಥಿರ bool quote_output;

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"canonicalize", no_argument, ರಿಕ್ತ, 'f'},
  {"canonicalize-existing", no_argument, ರಿಕ್ತ, 'e'},
  {"canonicalize-missing", no_argument, ರಿಕ್ತ, 'm'},
  {"no-newline", no_argument, ರಿಕ್ತ, 'n'},
  {"quiet", no_argument, ರಿಕ್ತ, 'q'},
  {"silent", no_argument, ರಿಕ್ತ, 's'},
  {"verbose", no_argument, ರಿಕ್ತ, 'v'},
  {"zero", no_argument, ರಿಕ್ತ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("Print value of a symbolic link or canonical file name\n\n"),
             stdout);
      oputs (_("\
  -f, --canonicalize\n\
         canonicalize by following every symlink\n\
         in every component of the given name recursively;\n\
         all but the last component must exist\n\
"));
      oputs (_("\
  -e, --canonicalize-existing\n\
         canonicalize by following every symlink\n\
         in every component of the given name recursively;\n\
         all components must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing\n\
         canonicalize by following every symlink\n\
         in every component of the given name recursively,\n\
         without requirements on components existence\n\
"));
      oputs (_("\
  -n, --no-newline\n\
         ಕುರು not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by ಸ್ವಭಾವ ಯದಿ POSIXLY_CORRECT is not set)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by ಸ್ವಭಾವ ಯದಿ POSIXLY_CORRECT is set)\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output ಸಾಲು with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  /* If not -1, use this method to canonicalize.  */
  ಪೂರ್ಣಾಙ್ಕ can_mode = -1;
  ಪೂರ್ಣಾಙ್ಕ status = EXIT_SUCCESS;
  ಪೂರ್ಣಾಙ್ಕ optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'e':
          can_mode = CAN_EXISTING;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'f':
          can_mode = CAN_ALL_BUT_LAST;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'm':
          can_mode = CAN_MISSING;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'n':
          no_newline = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'q':
        ಪಕ್ಷ 's':
          verbose = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'v':
          verbose = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'z':
          use_nuls = true;
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (argc - optind > 1)
    {
      ಯದಿ (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
    }

  ಯದಿ (!use_nuls && isatty (STDOUT_FILENO))
    {
      ಪೂರ್ಣಾಙ್ಕ qs = getenv_quoting_style ();
      ಯದಿ (qs < 0)
        qs = shell_escape_quoting_style;
      ಯದಿ (qs != literal_quoting_style)
        {
          set_quoting_style (ರಿಕ್ತ, qs);
          quote_output = true;
        }
    }

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  ಯದಿ (getenv ("POSIXLY_CORRECT") != ರಿಕ್ತ)
    verbose = true;

  ಪ್ರತಿ (; optind < argc; ++optind)
    {
      ವರ್ಣ ಧ್ರುವ *fname = argv[optind];
      ವರ್ಣ *ಮೌಲ್ಯ = (can_mode != -1
                     ? canonicalize_filename_mode (fname, can_mode)
                     : areadlink_with_size (fname, 63));
      ಯದಿ (ಮೌಲ್ಯ)
        {
          fputs (quote_output ? quoteN (ಮೌಲ್ಯ) : ಮೌಲ್ಯ, stdout);
          ಯದಿ (! no_newline)
            putchar (use_nuls ? '\0' : '\n');
          ಮುಕ್ತ (ಮೌಲ್ಯ);
          ಯದಿ (ferror (stdout))
            write_error ();
        }
      ಅನ್ಯಥಾ
        {
          status = EXIT_FAILURE;
          ಯದಿ (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  ಪ್ರತ್ಯಾಗಮ status;
}
