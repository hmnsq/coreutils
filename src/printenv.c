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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <getopt.h>

#அந்தர்பாவ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "printenv.h"
#அந்தர்பாவ "xbinary-io.h"

/* Exit status for syntax errors, etc.  */
எண்ணிக்கை { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printenv"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"null", no_argument, வெற்று, '0'},
  {"quoting-style", required_argument, வெற்று, QUOTING_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the மதிப்புகள் of the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and மதிப்பு pairs க்கு them all.\n\
\n\
"),
              program_name);
      oputs (_("\
  -0, --null\n\
         end each output வரி with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD க்கு names and மதிப்புகள்:\n\
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

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool opt_nul_terminate_output = false;
  முழு quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  முழு optc;
  போது ((optc = getopt_long (argc, argv, "+0", longopts, வெற்று)) != -1)
    {
      தேர்வு (optc)
        {
        நிலை '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          நிறுத்து;
        நிலை QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          நிறுத்து;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  முழு qs = quoting_style_opt;
  எனில் (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      எனில் (qs < 0)
        qs = shell_escape_quoting_style;
    }
  எனில் (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (வெற்று, qs);
      quote_output = true;
    }

  idx_t மாறிலி n_args = argc - optind;
  bool ok;
  எழுத்து மாறிலி terminator = opt_nul_terminate_output ? '\0' : '\n';

  எனில் (opt_nul_terminate_output)
    xset_இருமம்_mode (STDOUT_FILENO, O_இருமம்);

  எனில் (n_args <= 0)
    {
      க்கு (எழுத்து **env = environ; *env != வெற்று; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  இல்லையெனில்
    {
      முழு matches = 0;

      க்கு (முழு i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          எனில் (strchr (argv[i], '='))
            தொடர்;

          க்கு (எழுத்து **env = environ; *env; ++env)
            {
              எழுத்து மாறிலி *ep = *env;
              எழுத்து மாறிலி *ap = argv[i];
              போது (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  எனில் (*ep == '=' && *ap == '\0')
                    {
                      எழுத்து மாறிலி *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      putchar (terminator);
                      matched = true;
                      நிறுத்து;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
