/* basename -- strip directory and suffix from file names
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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "basename"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

ಸ್ಥಿರ bool quote_output;

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"multiple", no_ವಾದ, ಖಾಲಿ, 'a'},
  {"suffix", required_ವಾದ, ಖಾಲಿ, 's'},
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
Usage: %s NAME [SUFFIX]\n\
  or:  %s OPTION... NAME...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Print NAME with any leading directory components removed.\n\
If specified, also remove a trailing SUFFIX.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --ಅಪವರ್ತ್ಯ\n\
         support ಅಪವರ್ತ್ಯ arguments and treat each as a NAME\n\
"));
      oputs (_("\
  -s, --suffix=SUFFIX\n\
         remove a trailing SUFFIX; ಸೂಚಿಸುತ್ತದೆ -a\n\
"));
      oputs (_("\
  -z, --zero\n\
         ಕೊನೆ each output ಸಾಲು with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      ಬರೆ (_("\
\n\
Examples:\n\
  %s /usr/bin/ವಿಂಗಡಿಸು          -> \"ವಿಂಗಡಿಸು\"\n\
  %s include/stdio.h .h     -> \"stdio\"\n\
  %s -s .h include/stdio.h  -> \"stdio\"\n\
  %s -a any/str1 any/str2   -> \"str1\" followed by \"str2\"\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Remove SUFFIX from the end of NAME if it is there, unless NAME
   consists entirely of SUFFIX.  */

ಸ್ಥಿರ ಶೂನ್ಯ
remove_suffix (ಅಕ್ಷರ *name, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *suffix, idx_t suffix_len)
{
  ಅಕ್ಷರ *np = name + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (name);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *sp = suffix + suffix_len;

  ಆಗ (np > name && sp > suffix)
    ಆದರೆ (*--np != *--sp)
      ಹಿಂತಿರುಗಿ;
  ಆದರೆ (np > name)
    *np = '\0';
}

/* Perform the basename operation on STRING.  If SUFFIX has a positive
   length of SUFFIX_LEN bytes, remove the trailing SUFFIX.
   Finally, output the result string.  */

ಸ್ಥಿರ ಶೂನ್ಯ
perform_basename (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *string, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *suffix, idx_t suffix_len,
                  bool use_nuls)
{
  ಅಕ್ಷರ *name = ತಳ_name (string);
  strip_trailing_slashes (name);

  /* Per POSIX, 'basename // /' must return '//' on platforms with
     distinct //.  On platforms with drive letters, this generalizes
     to making 'basename c: :' return 'c:'.  This rule is captured by
     skipping suffix stripping if base_name returned an absolute path
     or a drive letter (only possible if name is a file-system
     root).  */
  ಆದರೆ (0 < suffix_len && IS_RELATIVE_FILE_NAME (name)
      && ! FILE_SYSTEM_ಪೂರ್ವಪ್ರತ್ಯಯ_LEN (name))
    remove_suffix (name, suffix, suffix_len);

  fputs (quote_output ? quoteN (name) : name, stdout);
  ಅಕ್ಷರ_ಬರೆ (use_nuls ? '\0' : '\n');
  ಬಿಡುಗಡೆ (name);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool ಅಪವರ್ತ್ಯ_ಹೆಸರುಗಳು = false;
  bool use_nuls = false;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *suffix = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv, "+as:z", longopts, ಖಾಲಿ);

      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;

      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 's':
          suffix = optarg;
          /* -s implies -a, so...  */
          FALLTHROUGH;

        ಸ್ಥಿತಿ 'a':
          ಅಪವರ್ತ್ಯ_ಹೆಸರುಗಳು = true;
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

  ಆದರೆ (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (! ಅಪವರ್ತ್ಯ_ಹೆಸರುಗಳು)
    {
      ಆದರೆ (optind + 2 == argc)
        suffix = argv[optind + 1];
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (optind + 2 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
          usage (EXIT_FAILURE);
        }
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

  idx_t suffix_len = suffix ? ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (suffix) : 0;

  ಅಕ್ಷರ **file = argv + optind;
  ಪೂರ್ಣಾಂಕ n_files = ಅಪವರ್ತ್ಯ_ಹೆಸರುಗಳು ? argc - optind : 1;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_files; ++i)
    perform_basename (file[i], suffix, suffix_len, use_nuls);

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
