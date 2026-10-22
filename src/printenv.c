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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <getopt.h>

#شامل "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#شامل "system.h"
#شامل "printenv.h"
#شامل "xbinary-io.h"

/* Exit status for syntax errors, etc.  */
شمارش { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printenv"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

ایستا ساختار option ثابت longopts[] =
{
  {"null", no_argument, پوچ, '0'},
  {"quoting-style", required_argument, پوچ, QUOTING_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the مقادیر of the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and مقدار pairs برای them all.\n\
\n\
"),
              program_name);
      oputs (_("\
  -0, --null\n\
         end each output سطر with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD برای names and مقادیر:\n\
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

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  bool opt_nul_terminate_output = false;
  صحیح quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  صحیح optc;
  وقتی ((optc = getopt_long (argc, argv, "+0", longopts, پوچ)) != -1)
    {
      انتخاب (optc)
        {
        حالت '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          بشکن;
        حالت QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          بشکن;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  صحیح qs = quoting_style_opt;
  اگر (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      اگر (qs < 0)
        qs = shell_escape_quoting_style;
    }
  اگر (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (پوچ, qs);
      quote_output = true;
    }

  idx_t ثابت n_args = argc - optind;
  bool ok;
  نویسه ثابت terminator = opt_nul_terminate_output ? '\0' : '\n';

  اگر (opt_nul_terminate_output)
    xset_دودویی_mode (STDOUT_FILENO, O_دودویی);

  اگر (n_args <= 0)
    {
      برای (نویسه **env = environ; *env != پوچ; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  وگرنه
    {
      صحیح matches = 0;

      برای (صحیح i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          اگر (strchr (argv[i], '='))
            ادامه;

          برای (نویسه **env = environ; *env; ++env)
            {
              نویسه ثابت *ep = *env;
              نویسه ثابت *ap = argv[i];
              وقتی (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  اگر (*ep == '=' && *ap == '\0')
                    {
                      نویسه ثابت *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      putchar (terminator);
                      matched = true;
                      بشکن;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
