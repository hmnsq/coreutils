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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <getopt.h>

#അന്തര്ഭാവ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "printenv.h"
#അന്തര്ഭാവ "xbinary-io.h"

/* Exit status for syntax errors, etc.  */
എണ്ണൽ { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printenv"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"null", no_argument, ശൂന്യ, '0'},
  {"quoting-style", required_argument, ശൂന്യ, QUOTING_STYLE_OPTION},
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
Usage: %s [OPTION] [VARIABLE]...\n\
Print the മൂല്യങ്ങൾ of the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and മൂല്യം pairs വേണ്ടി them all.\n\
\n\
"),
              program_name);
      oputs (_("\
  -0, --null\n\
         end each output വരി with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD വേണ്ടി names and മൂല്യങ്ങൾ:\n\
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  bool opt_nul_terminate_output = false;
  പൂർണ്ണസംഖ്യ quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  പൂർണ്ണസംഖ്യ optc;
  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "+0", longopts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          നിർത്തുക;
        അവസ്ഥ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  പൂർണ്ണസംഖ്യ qs = quoting_style_opt;
  എങ്കിൽ (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      എങ്കിൽ (qs < 0)
        qs = shell_escape_quoting_style;
    }
  എങ്കിൽ (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (ശൂന്യ, qs);
      quote_output = true;
    }

  idx_t സ്ഥിരം n_args = argc - optind;
  bool ok;
  അക്ഷരം സ്ഥിരം terminator = opt_nul_terminate_output ? '\0' : '\n';

  എങ്കിൽ (opt_nul_terminate_output)
    xset_ദ്വിമാന_mode (STDOUT_FILENO, O_ദ്വിമാന);

  എങ്കിൽ (n_args <= 0)
    {
      വേണ്ടി (അക്ഷരം **env = environ; *env != ശൂന്യ; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  അല്ലെങ്കിൽ
    {
      പൂർണ്ണസംഖ്യ matches = 0;

      വേണ്ടി (പൂർണ്ണസംഖ്യ i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          എങ്കിൽ (strchr (argv[i], '='))
            തുടരുക;

          വേണ്ടി (അക്ഷരം **env = environ; *env; ++env)
            {
              അക്ഷരം സ്ഥിരം *ep = *env;
              അക്ഷരം സ്ഥിരം *ap = argv[i];
              ആകുമ്പോൾ (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  എങ്കിൽ (*ep == '=' && *ap == '\0')
                    {
                      അക്ഷരം സ്ഥിരം *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      putchar (terminator);
                      matched = true;
                      നിർത്തുക;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
