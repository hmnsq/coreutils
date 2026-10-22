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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "printenv.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xbinary-io.h"

/* Exit status for syntax errors, etc.  */
ਗਿਣਤੀ { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "printenv"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"null", no_ਦਲੀਲ, ਨਿਰਾ, '0'},
  {"quoting-style", required_ਦਲੀਲ, ਨਿਰਾ, QUOTING_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the ਮੁੱਲਾਂ ਦਾ the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and ਮੁੱਲ ਜੋੜੇ ਲਈ them all.\n\
\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      oputs (_("\
  -0, --null\n\
         ਸਿਰਾ each output ਸਤਰ with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=ਸ਼ਬਦ\n\
         use quoting style ਸ਼ਬਦ ਲਈ ਨਾਂ and ਮੁੱਲਾਂ:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      ਲਿਖੋ (USAGE_BUILTIN_WARNING, ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool opt_nul_terminate_output = false;
  ਪੂਰਨ quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  ਪੂਰਨ optc;
  ਜਦੋਂ ((optc = getopt_long (argc, argv, "+0", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          ਰੋਕੋ;
        ਹਾਲਤ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_ਦਲੀਲਾਂ,
                                         quoting_style_vals);
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  ਪੂਰਨ qs = quoting_style_opt;
  ਜੇ (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      ਜੇ (qs < 0)
        qs = shell_escape_quoting_style;
    }
  ਜੇ (0 <= qs && qs != literal_quoting_style)
    {
      ਸੈੱਟ_quoting_style (ਨਿਰਾ, qs);
      quote_output = true;
    }

  idx_t ਅਚਲ n_ਦਲੀਲਾਂ = argc - optind;
  bool ok;
  ਅੱਖਰ ਅਚਲ terminator = opt_nul_terminate_output ? '\0' : '\n';

  ਜੇ (opt_nul_terminate_output)
    xset_ਦੁਆਧਾਰੀ_ਬਹੁਲਕ (STDOUT_FILENO, O_ਦੁਆਧਾਰੀ);

  ਜੇ (n_ਦਲੀਲਾਂ <= 0)
    {
      ਲਈ (ਅੱਖਰ **env = environ; *env != ਨਿਰਾ; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  ਨਹੀਂਤਾਂ
    {
      ਪੂਰਨ matches = 0;

      ਲਈ (ਪੂਰਨ i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          ਜੇ (strchr (argv[i], '='))
            ਜਾਰੀ;

          ਲਈ (ਅੱਖਰ **env = environ; *env; ++env)
            {
              ਅੱਖਰ ਅਚਲ *ep = *env;
              ਅੱਖਰ ਅਚਲ *ap = argv[i];
              ਜਦੋਂ (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  ਜੇ (*ep == '=' && *ap == '\0')
                    {
                      ਅੱਖਰ ਅਚਲ *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      ਅੱਖਰ_ਲਿਖੋ (terminator);
                      matched = true;
                      ਰੋਕੋ;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
