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
#define PROGRAM_NAME "printenv"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  QUOTING_STYLE_OPTION = अक्षर_उच्च + 1,
};

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"null", no_argument, रिक्त, '0'},
  {"quoting-style", required_argument, रिक्त, QUOTING_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the मूल्ये of the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and मूल्य pairs साठी them all.\n\
\n\
"),
              program_name);
      oputs (_("\
  -0, --null\n\
         end each output ओळ with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD साठी names and मूल्ये:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      म_लिखो (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  bool opt_nul_terminate_output = false;
  पूर्णांक quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  निकास_पर (close_stdout);

  पूर्णांक optc;
  पर्यंत ((optc = getopt_long (argc, argv, "+0", longopts, रिक्त)) != -1)
    {
      निवड (optc)
        {
        स्थिती '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          थांबा;
        स्थिती QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          थांबा;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        पूर्वनिर्धारित:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  पूर्णांक qs = quoting_style_opt;
  जर (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      जर (qs < 0)
        qs = shell_escape_quoting_style;
    }
  जर (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (रिक्त, qs);
      quote_output = true;
    }

  idx_t स्थिरांक n_args = argc - optind;
  bool ok;
  अक्षर स्थिरांक terminator = opt_nul_terminate_output ? '\0' : '\n';

  जर (opt_nul_terminate_output)
    xset_द्विमान_mode (STDOUT_FILENO, O_द्विमान);

  जर (n_args <= 0)
    {
      साठी (अक्षर **env = environ; *env != रिक्त; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  अन्यथा
    {
      पूर्णांक matches = 0;

      साठी (पूर्णांक i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          जर (म_अक्षर (argv[i], '='))
            सुरूठेवा;

          साठी (अक्षर **env = environ; *env; ++env)
            {
              अक्षर स्थिरांक *ep = *env;
              अक्षर स्थिरांक *ap = argv[i];
              पर्यंत (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  जर (*ep == '=' && *ap == '\0')
                    {
                      अक्षर स्थिरांक *val = ep + 1;
                      ख_माला_दो (quote_output ? quoteN (val) : val, मानक_निकास);
                      अक्षर_दो (terminator);
                      matched = true;
                      थांबा;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  परत ok ? निकास_सफल : निकास_त्रुटि;
}
