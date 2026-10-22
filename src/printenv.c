/* printenv -- print all or part of environment
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Usage: printenv [variable...]

   If no arguments are given, print the entire environment.
   If one or more variable names are given, print the value of
   each one that is set, and nothing for ones that are not set.

   Exit status:
   0 if all variables specified were found
   1 if not
   2 if some other error occurred

   David MacKenzie and Richard Mlynarik */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

#ಅನ್ತರ್ಭಾವ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "printenv.h"
#ಅನ್ತರ್ಭಾವ "xbinary-io.h"

/* Exit status for syntax errors, etc.  */
ಎಣಿಕೆ { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "printenv"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"null", no_ವಾದ, ಖಾಲಿ, '0'},
  {"quoting-style", required_ವಾದ, ಖಾಲಿ, QUOTING_STYLE_OPTION},
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
Usage: %s [OPTION] [VARIABLE]...\n\
Print the ಮೌಲ್ಯಗಳು ದ the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and ಮೌಲ್ಯ ಜೋಡಿಗಳು ಗಾಗಿ them all.\n\
\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      oputs (_("\
  -0, --null\n\
         ಕೊನೆ each output ಸಾಲು with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=ಶಬ್ದ\n\
         use quoting style ಶಬ್ದ ಗಾಗಿ ಹೆಸರುಗಳು and ಮೌಲ್ಯಗಳು:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      ಬರೆ (USAGE_BUILTIN_WARNING, ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool opt_nul_terminate_output = false;
  ಪೂರ್ಣಾಂಕ quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  ಪೂರ್ಣಾಂಕ optc;
  ಆಗ ((optc = getopt_long (argc, argv, "+0", longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_ವಾದಗಳು,
                                         quoting_style_vals);
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  ಪೂರ್ಣಾಂಕ qs = quoting_style_opt;
  ಆದರೆ (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      ಆದರೆ (qs < 0)
        qs = shell_escape_quoting_style;
    }
  ಆದರೆ (0 <= qs && qs != literal_quoting_style)
    {
      ಗಣ_quoting_style (ಖಾಲಿ, qs);
      quote_output = true;
    }

  idx_t ಸ್ಥಿರಾಂಕ n_ವಾದಗಳು = argc - optind;
  bool ok;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ terminator = opt_nul_terminate_output ? '\0' : '\n';

  ಆದರೆ (opt_nul_terminate_output)
    xset_ದ್ವಿಮಾನ_ಬಹುಲಕ (STDOUT_FILENO, O_ದ್ವಿಮಾನ);

  ಆದರೆ (n_ವಾದಗಳು <= 0)
    {
      ಗಾಗಿ (ಅಕ್ಷರ **env = environ; *env != ಖಾಲಿ; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಪೂರ್ಣಾಂಕ matches = 0;

      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          ಆದರೆ (strchr (argv[i], '='))
            ಮುಂದುವರಿ;

          ಗಾಗಿ (ಅಕ್ಷರ **env = environ; *env; ++env)
            {
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ep = *env;
              ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ap = argv[i];
              ಆಗ (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  ಆದರೆ (*ep == '=' && *ap == '\0')
                    {
                      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      ಅಕ್ಷರ_ಬರೆ (terminator);
                      matched = true;
                      ನಿಲ್ಲಿಸು;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
