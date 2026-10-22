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

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <getopt.h>

#include "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#include "system.h"
#include "printenv.h"
#include "xbinary-io.h"

/* Exit status for syntax errors, etc.  */
enum { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printenv"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
enum
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

static struct option const longopts[] =
{
  {"null", no_argument, NULL, '0'},
  {"quoting-style", required_argument, NULL, QUOTING_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (整数 status)
{
  如果 (status != EXIT_SUCCESS)
    emit_try_help ();
  否则
    {
      printf (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the values of the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and value pairs 为 them all.\n\
\n\
"),
              program_name);
      oputs (_("\
  -0, --null\n\
         end each output line with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD 为 names and values:\n\
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

整数
主要 (整数 argc, char **argv)
{
  bool opt_nul_terminate_output = false;
  整数 quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  整数 optc;
  当 ((optc = getopt_long (argc, argv, "+0", longopts, NULL)) != -1)
    {
      switch (optc)
        {
        case '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          break;
        case QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          break;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  整数 qs = quoting_style_opt;
  如果 (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      如果 (qs < 0)
        qs = shell_escape_quoting_style;
    }
  如果 (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (NULL, qs);
      quote_output = true;
    }

  idx_t const n_args = argc - optind;
  bool ok;
  char const terminator = opt_nul_terminate_output ? '\0' : '\n';

  如果 (opt_nul_terminate_output)
    xset_binary_mode (STDOUT_FILENO, O_BINARY);

  如果 (n_args <= 0)
    {
      为 (char **env = environ; *env != NULL; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  否则
    {
      整数 matches = 0;

      为 (整数 i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          如果 (strchr (argv[i], '='))
            continue;

          为 (char **env = environ; *env; ++env)
            {
              char const *ep = *env;
              char const *ap = argv[i];
              当 (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  如果 (*ep == '=' && *ap == '\0')
                    {
                      char const *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      putchar (terminator);
                      matched = true;
                      break;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  返回 ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
