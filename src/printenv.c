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
شمار { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "printenv"

#تعریف_کرو AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

ساکن ساخت option ثابت longopts[] =
{
  {"null", no_دلیل, تہی, '0'},
  {"quoting-style", required_دلیل, تہی, QUOTING_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the اقدار کا the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and قدر جوڑے برائے them all.\n\
\n\
"),
              پروگرام_name);
      oputs (_("\
  -0, --null\n\
         انتہا each output سطر with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=لفظ\n\
         use quoting style لفظ برائے نام and اقدار:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      لکھو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool opt_nul_terminate_output = false;
  صحیح quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  صحیح optc;
  جب ((optc = getopt_long (argc, argv, "+0", longopts, تہی)) != -1)
    {
      انتخاب (optc)
        {
        حالت '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          توڑو;
        حالت QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_دلائل,
                                         quoting_style_vals);
          توڑو;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
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
      سیٹ_quoting_style (تہی, qs);
      quote_output = true;
    }

  idx_t ثابت n_دلائل = argc - optind;
  bool ok;
  حرف ثابت terminator = opt_nul_terminate_output ? '\0' : '\n';

  اگر (opt_nul_terminate_output)
    xset_ثنائی_منوال (STDOUT_FILENO, O_ثنائی);

  اگر (n_دلائل <= 0)
    {
      برائے (حرف **env = environ; *env != تہی; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  ورنہ
    {
      صحیح matches = 0;

      برائے (صحیح i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          اگر (strchr (argv[i], '='))
            جاری;

          برائے (حرف **env = environ; *env; ++env)
            {
              حرف ثابت *ep = *env;
              حرف ثابت *ap = argv[i];
              جب (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  اگر (*ep == '=' && *ap == '\0')
                    {
                      حرف ثابت *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      حرف_لکھو (terminator);
                      matched = true;
                      توڑو;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
