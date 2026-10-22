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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <getopt.h>

#अन्तर्भाव "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#अन्तर्भाव "system.h"
#अन्तर्भाव "printenv.h"
#अन्तर्भाव "xbinary-io.h"

/* Exit status for syntax errors, etc.  */
गणना { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#परिभाषय क्रमादेशः_NAME "printenv"

#परिभाषय AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"null", no_तर्कः, रिक्त, '0'},
  {"quoting-style", required_तर्कः, रिक्त, QUOTING_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the मानानि सम्बन्धि the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and मानम् युग्मानि प्रति them all.\n\
\n\
"),
              क्रमादेशः_name);
      oputs (_("\
  -0, --null\n\
         अन्तः each output पङ्क्तिः with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD प्रति नामानि and मानानि:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      प्रारूप_लिख (USAGE_BUILTIN_WARNING, क्रमादेशः_NAME);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool opt_nul_terminate_output = false;
  पूर्णाङ्क quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  पूर्णाङ्क optc;
  यावत् ((optc = getopt_long (argc, argv, "+0", longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          भङ्ग;
        पक्ष QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_तर्काः,
                                         quoting_style_vals);
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  पूर्णाङ्क qs = quoting_style_opt;
  यदि (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      यदि (qs < 0)
        qs = shell_escape_quoting_style;
    }
  यदि (0 <= qs && qs != literal_quoting_style)
    {
      समुच्चयः_quoting_style (रिक्त, qs);
      quote_output = true;
    }

  idx_t ध्रुव n_तर्काः = argc - optind;
  bool ok;
  वर्ण ध्रुव terminator = opt_nul_terminate_output ? '\0' : '\n';

  यदि (opt_nul_terminate_output)
    xset_द्विमान_बहुलकम् (STDOUT_FILENO, O_द्विमान);

  यदि (n_तर्काः <= 0)
    {
      प्रति (वर्ण **env = environ; *env != रिक्त; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  अन्यथा
    {
      पूर्णाङ्क matches = 0;

      प्रति (पूर्णाङ्क i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          यदि (strchr (argv[i], '='))
            अनुवर्त;

          प्रति (वर्ण **env = environ; *env; ++env)
            {
              वर्ण ध्रुव *ep = *env;
              वर्ण ध्रुव *ap = argv[i];
              यावत् (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  यदि (*ep == '=' && *ap == '\0')
                    {
                      वर्ण ध्रुव *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      अक्षरं_लिख (terminator);
                      matched = true;
                      भङ्ग;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
