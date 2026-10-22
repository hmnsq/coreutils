/* dircolors - output commands to set the LS_COLOR environment variable
   Copyright (C) 1996-2026 Free Software Foundation, Inc.
   Copyright (C) 1994, 1995, 1997, 1998, 1999, 2000 H. Peter Anvin

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

#అన్తర్భావ <config.h>

#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <fnmatch.h>
#అన్తర్భావ <getopt.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "dircolors.h"
#అన్తర్భావ "c-ctype.h"
#అన్తర్భావ "c-strcase.h"
#అన్తర్భావ "obstack.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "dircolors"

#నిర్వచించు AUTHORS proper_name ("H. Peter Anvin")

#నిర్వచించు obstack_chunk_alloc జ్ఞాపకం_ఇవ్వు
#నిర్వచించు obstack_chunk_free విడుదల

గణన Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#నిర్వచించు APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
స్థిర నిర్మాణం obstack lsc_obstack;

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", ఖాళీ
};

స్థిర అక్షరం స్థిరరాశి *స్థిరరాశి ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  ఖాళీ
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
స్థిర bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
  {
    {"bourne-shell", no_వాదం, ఖాళీ, 'b'},
    {"sh", no_వాదం, ఖాళీ, 'b'},
    {"csh", no_వాదం, ఖాళీ, 'c'},
    {"c-shell", no_వాదం, ఖాళీ, 'c'},
    {"print-database", no_వాదం, ఖాళీ, 'p'},
    {"print-ls-colors", no_వాదం, ఖాళీ, PRINT_LS_COLORS_OPTION},
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
      రాయి (_("Usage: %s [OPTION]... [FILE]\n"), కార్యక్రమం_name);
      fputs (_("\
Output commands గమ్యం సమితి_2 the LS_COLORS environment variable.\n\
\n\
Determine format యొక్క output:\n\
"), stdout);
      oputs (_("\
  -b, --sh, --bourne-shell\n\
         output Bourne shell సంకేతం_2 గమ్యం సమితి_2 LS_COLORS\n\
"));
      oputs (_("\
  -c, --csh, --c-shell\n\
         output C shell సంకేతం_2 గమ్యం సమితి_2 LS_COLORS\n\
"));
      oputs (_("\
  -p, --print-database\n\
         output defaults\n\
"));
      oputs (_("\
      --print-ls-colors\n\
         output fully escaped colors కొరకు display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, పఠ it గమ్యం determine which colors గమ్యం use కొరకు which\n\
file types and extensions.  Otherwise, a precompiled database is వాడిన.\n\
For details on the format యొక్క these files, నిరంతరం_2 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }

  నిష్క్రమించు (స్థితి_2);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

స్థిర గణన Shell_syntax
guess_shell_syntax (శూన్య)
{
  అక్షరం *shell;

  shell = getenv ("SHELL");
  ఒకవేళ (shell == ఖాళీ || *shell == '\0')
    తిరిగి SHELL_SYNTAX_UNKNOWN;

  shell = చివరి_component (shell);

  ఒకవేళ (streq (shell, "csh") || streq (shell, "tcsh"))
    తిరిగి SHELL_SYNTAX_C;

  తిరిగి SHELL_SYNTAX_BOURNE;
}

స్థిర శూన్య
parse_పంక్తి (అక్షరం స్థిరరాశి *పంక్తి, అక్షరం **keyword, అక్షరం **arg)
{
  అక్షరం స్థిరరాశి *p;
  అక్షరం స్థిరరాశి *keyword_ప్రారంభం;
  అక్షరం స్థిరరాశి *arg_ప్రారంభం;

  *keyword = ఖాళీ;
  *arg = ఖాళీ;

  కొరకు (p = పంక్తి; c_isspace (*p); ++p)
    కొనసాగు;

  /* Ignore blank lines and shell-style comments.  */
  ఒకవేళ (*p == '\0' || *p == '#')
    తిరిగి;

  keyword_ప్రారంభం = p;

  అయితే (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_ప్రారంభం, p - keyword_ప్రారంభం);
  ఒకవేళ (*p  == '\0')
    తిరిగి;

  చేయి
    {
      ++p;
    }
  అయితే (c_isspace (*p));

  ఒకవేళ (*p == '\0' || *p == '#')
    తిరిగి;

  arg_ప్రారంభం = p;

  అయితే (*p != '\0' && *p != '#')
    ++p;

  కొరకు (--p; c_isspace (*p); --p)
    కొనసాగు;
  ++p;

  *arg = ximemdup0 (arg_ప్రారంభం, p - arg_ప్రారంభం);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

స్థిర శూన్య
append_quoted (అక్షరం స్థిరరాశి *str)
{
  bool need_backslash = true;

  అయితే (*str != '\0')
    {
      ఒకవేళ (! print_ls_colors)
        ఎంపిక (*str)
          {
          స్థితి '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            ఆపు;

          స్థితి '\\':
          స్థితి '^':
            need_backslash = !need_backslash;
            ఆపు;

          స్థితి ':':
          స్థితి '=':
            ఒకవేళ (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          అప్రమేయ:
            need_backslash = true;
            ఆపు;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

స్థిర శూన్య
append_నమోదు (అక్షరం ఉపసర్గ, అక్షరం స్థిరరాశి *అంశం, అక్షరం స్థిరరాశి *arg)
{
  ఒకవేళ (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  ఒకవేళ (ఉపసర్గ)
    APPEND_CHAR (ఉపసర్గ);
  append_quoted (అంశం);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  ఒకవేళ (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

స్థిర bool
dc_parse_ప్రవాహం (FILE *fp, అక్షరం స్థిరరాశి *filename)
{
  idx_t పంక్తి_సంఖ్య = 0;
  అక్షరం స్థిరరాశి *తదుపరి_G_పంక్తి = G_పంక్తి;
  అక్షరం *ఆదానం_పంక్తి = ఖాళీ;
  size_t ఆదానం_పంక్తి_కొలత = 0;
  అక్షరం స్థిరరాశి *పంక్తి;
  అక్షరం స్థిరరాశి *పదం;
  అక్షరం స్థిరరాశి *colorterm;
  bool ok = true;

  /* State for the parser.  */
  గణన { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  పదం = getenv ("TERM");
  ఒకవేళ (పదం == ఖాళీ || *పదం == '\0')
    పదం = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  ఒకవేళ (colorterm == ఖాళీ)
    colorterm = "";  /* Doesn't match default "?*"  */

  అయితే (true)
    {
      అక్షరం *keywd, *arg;
      bool unrecognized;

      ++పంక్తి_సంఖ్య;

      ఒకవేళ (fp)
        {
          ఒకవేళ (getline (&ఆదానం_పంక్తి, &ఆదానం_పంక్తి_కొలత, fp) <= 0)
            {
              ఒకవేళ (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              విడుదల (ఆదానం_పంక్తి);
              ఆపు;
            }
          పంక్తి = ఆదానం_పంక్తి;
        }
      లేకపోతే
        {
          ఒకవేళ (తదుపరి_G_పంక్తి == G_పంక్తి + పరిమాణం G_పంక్తి)
            ఆపు;
          పంక్తి = తదుపరి_G_పంక్తి;
          తదుపరి_G_పంక్తి += అక్షరశ్రేణి_పొడవు (తదుపరి_G_పంక్తి) + 1;
        }

      parse_పంక్తి (పంక్తి, &keywd, &arg);

      ఒకవేళ (keywd == ఖాళీ)
        కొనసాగు;

      ఒకవేళ (arg == ఖాళీ)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), పంక్తి_సంఖ్య);
          ok = false;
          విడుదల (keywd);
          కొనసాగు;
        }

      unrecognized = false;
      ఒకవేళ (c_strcasecmp (keywd, "TERM") == 0)
        {
          ఒకవేళ (state != ST_TERMSURE)
            state = fnmatch (arg, పదం, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      లేకపోతే ఒకవేళ (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          ఒకవేళ (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      లేకపోతే
        {
          ఒకవేళ (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          ఒకవేళ (state != ST_TERMNO)
            {
              ఒకవేళ (keywd[0] == '.')
                append_నమోదు ('*', keywd, arg);
              లేకపోతే ఒకవేళ (keywd[0] == '*')
                append_నమోదు (0, keywd, arg);
              లేకపోతే ఒకవేళ (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              లేకపోతే
                {
                  పూర్ణాంకం i;

                  కొరకు (i = 0; slack_codes[i] != ఖాళీ; ++i)
                    ఒకవేళ (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      ఆపు;

                  ఒకవేళ (slack_codes[i] != ఖాళీ)
                    append_నమోదు (0, ls_codes[i], arg);
                  లేకపోతే
                    unrecognized = true;
                }
            }
          లేకపోతే
            unrecognized = true;
        }

      ఒకవేళ (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 పంక్తి_సంఖ్య, keywd);
          ok = false;
        }

      విడుదల (keywd);
      విడుదల (arg);
    }

  తిరిగి ok;
}

స్థిర bool
dc_parse_file (అక్షరం స్థిరరాశి *filename)
{
  bool ok;

  ఒకవేళ (! streq (filename, "-") && freopen (filename, "r", stdin) == ఖాళీ)
    {
      error (0, errno, "%s", quotef (filename));
      తిరిగి false;
    }

  ok = dc_parse_ప్రవాహం (stdin, filename);

  ఒకవేళ (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      తిరిగి false;
    }

  తిరిగి ok;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool ok = true;
  పూర్ణాంకం optc;
  గణన Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, "bcp", long_options, ఖాళీ)) != -1)
    ఎంపిక (optc)
      {
      స్థితి 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        ఆపు;

      స్థితి 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        ఆపు;

      స్థితి 'p':
        print_database = true;
        ఆపు;

      స్థితి PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        ఆపు;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

      అప్రమేయ:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  ఒకవేళ ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      ఒకవేళ (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (print_database)
    {
      అక్షరం స్థిరరాశి *p = G_పంక్తి;
      అయితే (p - G_పంక్తి < పరిమాణం G_పంక్తి)
        {
          puts (p);
          p += అక్షరశ్రేణి_పొడవు (p) + 1;
        }
    }
  లేకపోతే
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      ఒకవేళ (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          ఒకవేళ (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      ఒకవేళ (argc == 0)
        ok = dc_parse_ప్రవాహం (ఖాళీ, ఖాళీ);
      లేకపోతే
        ok = dc_parse_file (argv[0]);

      ఒకవేళ (ok)
        {
          size_t len = obstack_object_కొలత (&lsc_obstack);
          అక్షరం *s = obstack_ముగింపు (&lsc_obstack);
          అక్షరం స్థిరరాశి *ఉపసర్గ;
          అక్షరం స్థిరరాశి *suffix;

          ఒకవేళ (syntax == SHELL_SYNTAX_BOURNE)
            {
              ఉపసర్గ = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          లేకపోతే
            {
              ఉపసర్గ = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          ఒకవేళ (! print_ls_colors)
            fputs (ఉపసర్గ, stdout);
          fwrite (s, 1, len, stdout);
          ఒకవేళ (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
