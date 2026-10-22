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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <getopt.h>

#समावेश "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#समावेश "system.h"
#समावेश "printenv.h"
#समावेश "xbinary-io.h"

/* Exit status for syntax errors, etc.  */
क्रमागत { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "printenv"

#घोषणा AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  QUOTING_STYLE_OPTION = अक्षर_उच्च + 1,
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"null", no_argument, शून्य, '0'},
  {"quoting-style", required_argument, शून्य, QUOTING_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the मूल्यावली of the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and मूल्य pairs क्रम them all.\n\
\n\
"),
              program_name);
      oputs (_("\
  -0, --null\n\
         end each output पंक्ति with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD क्रम names and मूल्यावली:\n\
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
मुख्य (पूर्णांक argc, अक्षर **argv)
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
  जबतक ((optc = getopt_long (argc, argv, "+0", longopts, शून्य)) != -1)
    {
      चयन (optc)
        {
        हाल '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          अवरोध;
        हाल QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          अवरोध;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  पूर्णांक qs = quoting_style_opt;
  अगर (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      अगर (qs < 0)
        qs = shell_escape_quoting_style;
    }
  अगर (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (शून्य, qs);
      quote_output = true;
    }

  idx_t स्थिर n_args = argc - optind;
  bool ok;
  अक्षर स्थिर terminator = opt_nul_terminate_output ? '\0' : '\n';

  अगर (opt_nul_terminate_output)
    xset_द्विआधारी_mode (STDOUT_FILENO, O_द्विआधारी);

  अगर (n_args <= 0)
    {
      क्रम (अक्षर **env = environ; *env != शून्य; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  अन्यथा
    {
      पूर्णांक matches = 0;

      क्रम (पूर्णांक i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          अगर (म_अक्षर (argv[i], '='))
            जारी;

          क्रम (अक्षर **env = environ; *env; ++env)
            {
              अक्षर स्थिर *ep = *env;
              अक्षर स्थिर *ap = argv[i];
              जबतक (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  अगर (*ep == '=' && *ap == '\0')
                    {
                      अक्षर स्थिर *val = ep + 1;
                      ख_माला_दो (quote_output ? quoteN (val) : val, मानक_निकास);
                      अक्षर_दो (terminator);
                      matched = true;
                      अवरोध;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
