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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <getopt.h>

#অন্তর্ভাৱ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "printenv.h"
#অন্তর্ভাৱ "xbinary-io.h"

/* Exit status for syntax errors, etc.  */
গণনা { PRINTENV_FAILURE = 2 };

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "printenv"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Richard Mlynarik")

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  QUOTING_STYLE_OPTION = CHAR_MAX + 1,
};

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {"null", no_argument, রিক্ত, '0'},
  {"quoting-style", required_argument, রিক্ত, QUOTING_STYLE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION] [VARIABLE]...\n\
Print the মানসমূহ of the specified environment VARIABLE(s).\n\
If no VARIABLE is specified, print name and মান pairs বাবে them all.\n\
\n\
"),
              program_name);
      oputs (_("\
  -0, --null\n\
         end each output সাৰি with NUL, not newline\n\
"));
      oputs (_("\
      --quoting-style=WORD\n\
         use quoting style WORD বাবে names and মানসমূহ:\n\
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

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  bool opt_nul_terminate_output = false;
  পূর্ণাঙ্ক quoting_style_opt = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (PRINTENV_FAILURE);
  atexit (close_stdout);

  পূর্ণাঙ্ক optc;
  যেতিয়ালৈ ((optc = getopt_long (argc, argv, "+0", longopts, রিক্ত)) != -1)
    {
      ৱিকল্প (optc)
        {
        পক্ষ '0':
          opt_nul_terminate_output = true;
          quoting_style_opt = literal_quoting_style;
          ভঙ্গ;
        পক্ষ QUOTING_STYLE_OPTION:
          quoting_style_opt = XARGMATCH ("--quoting-style", optarg,
                                         quoting_style_args,
                                         quoting_style_vals);
          ভঙ্গ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
          usage (PRINTENV_FAILURE);
        }
    }

  bool quote_output = false;
  পূর্ণাঙ্ক qs = quoting_style_opt;
  যদি (qs < 0 && isatty (STDOUT_FILENO))
    {
      qs = getenv_quoting_style ();
      যদি (qs < 0)
        qs = shell_escape_quoting_style;
    }
  যদি (0 <= qs && qs != literal_quoting_style)
    {
      set_quoting_style (রিক্ত, qs);
      quote_output = true;
    }

  idx_t ধ্রুৱ n_args = argc - optind;
  bool ok;
  ৱর্ণ ধ্রুৱ terminator = opt_nul_terminate_output ? '\0' : '\n';

  যদি (opt_nul_terminate_output)
    xset_দ্বিমিক_mode (STDOUT_FILENO, O_দ্বিমিক);

  যদি (n_args <= 0)
    {
      বাবে (ৱর্ণ **env = environ; *env != রিক্ত; ++env)
        print_envvar (*env, terminator, quote_output);
      ok = true;
    }
  অন্যথা
    {
      পূর্ণাঙ্ক matches = 0;

      বাবে (পূর্ণাঙ্ক i = optind; i < argc; ++i)
        {
          bool matched = false;

          /* 'printenv a=b' is silent, even if 'a=b=c' is in environ.  */
          যদি (strchr (argv[i], '='))
            অনুৱর্ত;

          বাবে (ৱর্ণ **env = environ; *env; ++env)
            {
              ৱর্ণ ধ্রুৱ *ep = *env;
              ৱর্ণ ধ্রুৱ *ap = argv[i];
              যেতিয়ালৈ (*ep != '\0' && *ap != '\0' && *ep++ == *ap++)
                {
                  যদি (*ep == '=' && *ap == '\0')
                    {
                      ৱর্ণ ধ্রুৱ *val = ep + 1;
                      fputs (quote_output ? quoteN (val) : val, stdout);
                      putchar (terminator);
                      matched = true;
                      ভঙ্গ;
                    }
                }
            }

          matches += matched;
        }

      ok = (matches == argc - optind);
    }

  ঘূৰাই ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
