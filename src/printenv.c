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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <getopt.h>

#అన్తర్భావ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#అన్తర్భావ "system.h"
#అన్తర్భావ "printenv.h"
#అన్తర్భావ "xbinary-io.h"

/* Exit status for syntax errors, etc.  */
గణన { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "printenv"

#నిర్వచించు AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"null", no_వాదం, ఖాళీ, '0'},
  {"quoting-style", required_వాదం, ఖాళీ, QUOTING_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the విలువలు యొక్క the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and విలువ జతలు కొరకు them all.\n\
\n\
"),
              కార్యక్రమం_name);
      oputs (_("\
  -0, --null\n\
         చివర each output పంక్తి with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=మాట\n\
         use quoting style మాట కొరకు పేర్లు and విలువలు:\n\
           literal, locale, shell, shell-always,\n\
           shell-escape, shell-escape-always, c, escape\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      రాయి (USAGE_BUILTIN_WARNING, కార్యక్రమం_NAME);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool opt_nul_terminate_output = false;
  పూర్ణాంకం quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  పూర్ణాంకం optc;
  అయితే ((optc = getopt_long (argc, argv, "+0", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          ఆపు;
        స్థితి QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_వాదనలు,
                                         quoting_style_vals);
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  పూర్ణాంకం qs = quoting_style_opt;
  ఒకవేళ (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      ఒకవేళ (qs < 0)
        qs = shell_escape_quoting_style;
    }
  ఒకవేళ (0 <= qs && qs != literal_quoting_style)
    {
      సమితి_quoting_style (ఖాళీ, qs);
      quote_output = true;
    }

  idx_t స్థిరరాశి n_వాదనలు = argc - optind;
  bool ok;
  అక్షరం స్థిరరాశి terminator = opt_nul_terminate_output ? '\0' : '\n';

  ఒకవేళ (opt_nul_terminate_output)
    xset_ద్విమాన_బహుళకం (STDOUT_FILENO, O_ద్విమాన);

  ఒకవేళ (n_వాదనలు <= 0)
    {
      కొరకు (అక్షరం **env = environ; *env != ఖాళీ; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  లేకపోతే
    {
      పూర్ణాంకం matches = 0;

      కొరకు (పూర్ణాంకం i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          ఒకవేళ (strchr (argv[i], '='))
            కొనసాగు;

          కొరకు (అక్షరం **env = environ; *env; ++env)
            {
              అక్షరం స్థిరరాశి *ep = *env;
              అక్షరం స్థిరరాశి *ap = argv[i];
              అయితే (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  ఒకవేళ (*ep == '=' && *ap == '\0')
                    {
                      అక్షరం స్థిరరాశి *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      అక్షరం_రాయి (terminator);
                      matched = true;
                      ఆపు;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
