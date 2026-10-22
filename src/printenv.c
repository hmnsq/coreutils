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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>

#ଅନ୍ତର୍ଭାଵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "printenv.h"
#ଅନ୍ତର୍ଭାଵ "xbinary-io.h"

/* Exit status for syntax errors, etc.  */
ଗଣନା { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printenv"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"null", no_argument, ଖାଲି, '0'},
  {"quoting-style", required_argument, ଖାଲି, QUOTING_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the ମୂଲ୍ୟଗୁଡ଼ିକ of the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and ମୂଲ୍ୟ pairs ପାଇଁ them all.\n\
\n\
"),
              program_name);
      oputs (_("\
  -0, --null\n\
         end each output ଧାଡ଼ି with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD ପାଇଁ names and ମୂଲ୍ୟଗୁଡ଼ିକ:\n\
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  bool opt_nul_terminate_output = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "+0", longopts, ଖାଲି)) != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          ଭାଙ୍ଗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା qs = quoting_style_opt;
  ଯଦି (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      ଯଦି (qs < 0)
        qs = shell_escape_quoting_style;
    }
  ଯଦି (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (ଖାଲି, qs);
      quote_output = true;
    }

  idx_t ଅଚଳ n_args = argc - optind;
  bool ok;
  ଅକ୍ଷର ଅଚଳ terminator = opt_nul_terminate_output ? '\0' : '\n';

  ଯଦି (opt_nul_terminate_output)
    xset_ଦ୍ୱିମିକ_mode (STDOUT_FILENO, O_ଦ୍ୱିମିକ);

  ଯଦି (n_args <= 0)
    {
      ପାଇଁ (ଅକ୍ଷର **env = environ; *env != ଖାଲି; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  ଅନ୍ୟଥା
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା matches = 0;

      ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          ଯଦି (strchr (argv[i], '='))
            ଜାରି;

          ପାଇଁ (ଅକ୍ଷର **env = environ; *env; ++env)
            {
              ଅକ୍ଷର ଅଚଳ *ep = *env;
              ଅକ୍ଷର ଅଚଳ *ap = argv[i];
              ଯେତେବେଳେ (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  ଯଦି (*ep == '=' && *ap == '\0')
                    {
                      ଅକ୍ଷର ଅଚଳ *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      putchar (terminator);
                      matched = true;
                      ଭାଙ୍ଗ;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
