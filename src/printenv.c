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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <getopt.h>

#અન્તર્ભાવ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "printenv.h"
#અન્તર્ભાવ "xbinary-io.h"

/* Exit status for syntax errors, etc.  */
ગણના { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printenv"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"null", no_argument, ખાલી, '0'},
  {"quoting-style", required_argument, ખાલી, QUOTING_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the કિંમતો of the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and કિંમત pairs માટે them all.\n\
\n\
"),
              program_name);
      oputs (_("\
  -0, --null\n\
         end each output લીટી with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD માટે names and કિંમતો:\n\
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

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  bool opt_nul_terminate_output = false;
  પૂર્ણાંક quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  પૂર્ણાંક optc;
  જ્યારે ((optc = getopt_long (argc, argv, "+0", longopts, ખાલી)) != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          તોડો;
        સ્થિતિ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          તોડો;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  પૂર્ણાંક qs = quoting_style_opt;
  જો (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      જો (qs < 0)
        qs = shell_escape_quoting_style;
    }
  જો (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (ખાલી, qs);
      quote_output = true;
    }

  idx_t અચલ n_args = argc - optind;
  bool ok;
  અક્ષર અચલ terminator = opt_nul_terminate_output ? '\0' : '\n';

  જો (opt_nul_terminate_output)
    xset_દ્વિઅંકી_mode (STDOUT_FILENO, O_દ્વિઅંકી);

  જો (n_args <= 0)
    {
      માટે (અક્ષર **env = environ; *env != ખાલી; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  નહીંતર
    {
      પૂર્ણાંક matches = 0;

      માટે (પૂર્ણાંક i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          જો (strchr (argv[i], '='))
            ચાલુ;

          માટે (અક્ષર **env = environ; *env; ++env)
            {
              અક્ષર અચલ *ep = *env;
              અક્ષર અચલ *ap = argv[i];
              જ્યારે (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  જો (*ep == '=' && *ap == '\0')
                    {
                      અક્ષર અચલ *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      putchar (terminator);
                      matched = true;
                      તોડો;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
