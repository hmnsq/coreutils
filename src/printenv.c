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
ಗಣನಾ { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printenv"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"null", no_argument, ರಿಕ್ತ, '0'},
  {"quoting-style", required_argument, ರಿಕ್ತ, QUOTING_STYLE_OPTION},
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
      printf (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the ಮೌಲ್ಯಗಳು of the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and ಮೌಲ್ಯ pairs ಪ್ರತಿ them all.\n\
\n\
"),
              program_name);
      oputs (_("\
  -0, --null\n\
         end each output ಸಾಲು with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD ಪ್ರತಿ names and ಮೌಲ್ಯಗಳು:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  bool opt_nul_terminate_output = false;
  ಪೂರ್ಣಾಙ್ಕ quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  ಪೂರ್ಣಾಙ್ಕ optc;
  ಯಾವತ್ ((optc = getopt_long (argc, argv, "+0", longopts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          ಭಙ್ಗ;
        ಪಕ್ಷ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  ಪೂರ್ಣಾಙ್ಕ qs = quoting_style_opt;
  ಯದಿ (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      ಯದಿ (qs < 0)
        qs = shell_escape_quoting_style;
    }
  ಯದಿ (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (ರಿಕ್ತ, qs);
      quote_output = true;
    }

  idx_t ಧ್ರುವ n_args = argc - optind;
  bool ok;
  ವರ್ಣ ಧ್ರುವ terminator = opt_nul_terminate_output ? '\0' : '\n';

  ಯದಿ (opt_nul_terminate_output)
    xset_ದ್ವಿಮಾನ_mode (STDOUT_FILENO, O_ದ್ವಿಮಾನ);

  ಯದಿ (n_args <= 0)
    {
      ಪ್ರತಿ (ವರ್ಣ **env = environ; *env != ರಿಕ್ತ; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  ಅನ್ಯಥಾ
    {
      ಪೂರ್ಣಾಙ್ಕ matches = 0;

      ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          ಯದಿ (strchr (argv[i], '='))
            ಅನುವರ್ತ;

          ಪ್ರತಿ (ವರ್ಣ **env = environ; *env; ++env)
            {
              ವರ್ಣ ಧ್ರುವ *ep = *env;
              ವರ್ಣ ಧ್ರುವ *ap = argv[i];
              ಯಾವತ್ (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  ಯದಿ (*ep == '=' && *ap == '\0')
                    {
                      ವರ್ಣ ಧ್ರುವ *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      putchar (terminator);
                      matched = true;
                      ಭಙ್ಗ;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
