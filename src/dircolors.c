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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <fnmatch.h>
#അന്തര്ഭാവ <getopt.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "dircolors.h"
#അന്തര്ഭാവ "c-ctype.h"
#അന്തര്ഭാവ "c-strcase.h"
#അന്തര്ഭാവ "obstack.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dircolors"

#define AUTHORS proper_name ("H. Peter Anvin")

#define obstack_chunk_alloc malloc
#define obstack_chunk_free സ്വതന്ത്രം

എണ്ണൽ Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#define APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
സ്ഥിതം ഘടന obstack lsc_obstack;

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", ശൂന്യ
};

സ്ഥിതം അക്ഷരം സ്ഥിരം *സ്ഥിരം ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  ശൂന്യ
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
സ്ഥിതം bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
  {
    {"bourne-shell", no_argument, ശൂന്യ, 'b'},
    {"sh", no_argument, ശൂന്യ, 'b'},
    {"csh", no_argument, ശൂന്യ, 'c'},
    {"c-shell", no_argument, ശൂന്യ, 'c'},
    {"print-database", no_argument, ശൂന്യ, 'p'},
    {"print-ls-colors", no_argument, ശൂന്യ, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {ശൂന്യ, 0, ശൂന്യ, 0}
  };

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]... [FILE]\n"), program_name);
      fputs (_("\
Output commands to set the LS_COLORS environment variable.\n\
\n\
Determine format of output:\n\
"), stdout);
      oputs (_("\
  -b, --sh, --bourne-shell\n\
         output Bourne shell code to set LS_COLORS\n\
"));
      oputs (_("\
  -c, --csh, --c-shell\n\
         output C shell code to set LS_COLORS\n\
"));
      oputs (_("\
  -p, --print-database\n\
         output defaults\n\
"));
      oputs (_("\
      --print-ls-colors\n\
         output fully escaped colors വേണ്ടി display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, പഠ it to determine which colors to use വേണ്ടി which\n\
file types and extensions.  Otherwise, a precompiled database is used.\n\
For details on the format of these files, run 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

സ്ഥിതം എണ്ണൽ Shell_syntax
guess_shell_syntax (ശൂന്യം)
{
  അക്ഷരം *shell;

  shell = getenv ("SHELL");
  എങ്കിൽ (shell == ശൂന്യ || *shell == '\0')
    തിരികെ SHELL_SYNTAX_UNKNOWN;

  shell = last_component (shell);

  എങ്കിൽ (streq (shell, "csh") || streq (shell, "tcsh"))
    തിരികെ SHELL_SYNTAX_C;

  തിരികെ SHELL_SYNTAX_BOURNE;
}

സ്ഥിതം ശൂന്യം
parse_വരി (അക്ഷരം സ്ഥിരം *വരി, അക്ഷരം **keyword, അക്ഷരം **arg)
{
  അക്ഷരം സ്ഥിരം *p;
  അക്ഷരം സ്ഥിരം *keyword_start;
  അക്ഷരം സ്ഥിരം *arg_start;

  *keyword = ശൂന്യ;
  *arg = ശൂന്യ;

  വേണ്ടി (p = വരി; c_isspace (*p); ++p)
    തുടരുക;

  /* Ignore blank lines and shell-style comments.  */
  എങ്കിൽ (*p == '\0' || *p == '#')
    തിരികെ;

  keyword_start = p;

  ആകുമ്പോൾ (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  എങ്കിൽ (*p  == '\0')
    തിരികെ;

  ചെയ്യുക
    {
      ++p;
    }
  ആകുമ്പോൾ (c_isspace (*p));

  എങ്കിൽ (*p == '\0' || *p == '#')
    തിരികെ;

  arg_start = p;

  ആകുമ്പോൾ (*p != '\0' && *p != '#')
    ++p;

  വേണ്ടി (--p; c_isspace (*p); --p)
    തുടരുക;
  ++p;

  *arg = ximemdup0 (arg_start, p - arg_start);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

സ്ഥിതം ശൂന്യം
append_quoted (അക്ഷരം സ്ഥിരം *str)
{
  bool need_backslash = true;

  ആകുമ്പോൾ (*str != '\0')
    {
      എങ്കിൽ (! print_ls_colors)
        തിരഞ്ഞെടുപ്പ് (*str)
          {
          അവസ്ഥ '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            നിർത്തുക;

          അവസ്ഥ '\\':
          അവസ്ഥ '^':
            need_backslash = !need_backslash;
            നിർത്തുക;

          അവസ്ഥ ':':
          അവസ്ഥ '=':
            എങ്കിൽ (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          സ്ഥിരസ്ഥിതി:
            need_backslash = true;
            നിർത്തുക;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

സ്ഥിതം ശൂന്യം
append_entry (അക്ഷരം prefix, അക്ഷരം സ്ഥിരം *item, അക്ഷരം സ്ഥിരം *arg)
{
  എങ്കിൽ (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  എങ്കിൽ (prefix)
    APPEND_CHAR (prefix);
  append_quoted (item);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  എങ്കിൽ (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

സ്ഥിതം bool
dc_parse_stream (FILE *fp, അക്ഷരം സ്ഥിരം *filename)
{
  idx_t വരി_സംഖ്യ = 0;
  അക്ഷരം സ്ഥിരം *next_G_വരി = G_വരി;
  അക്ഷരം *നിവേശം_വരി = ശൂന്യ;
  size_t നിവേശം_വരി_size = 0;
  അക്ഷരം സ്ഥിരം *വരി;
  അക്ഷരം സ്ഥിരം *term;
  അക്ഷരം സ്ഥിരം *colorterm;
  bool ok = true;

  /* State for the parser.  */
  എണ്ണൽ { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  term = getenv ("TERM");
  എങ്കിൽ (term == ശൂന്യ || *term == '\0')
    term = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  എങ്കിൽ (colorterm == ശൂന്യ)
    colorterm = "";  /* Doesn't match default "?*"  */

  ആകുമ്പോൾ (true)
    {
      അക്ഷരം *keywd, *arg;
      bool unrecognized;

      ++വരി_സംഖ്യ;

      എങ്കിൽ (fp)
        {
          എങ്കിൽ (getline (&നിവേശം_വരി, &നിവേശം_വരി_size, fp) <= 0)
            {
              എങ്കിൽ (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              സ്വതന്ത്രം (നിവേശം_വരി);
              നിർത്തുക;
            }
          വരി = നിവേശം_വരി;
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (next_G_വരി == G_വരി + വലുപ്പം G_വരി)
            നിർത്തുക;
          വരി = next_G_വരി;
          next_G_വരി += strlen (next_G_വരി) + 1;
        }

      parse_വരി (വരി, &keywd, &arg);

      എങ്കിൽ (keywd == ശൂന്യ)
        തുടരുക;

      എങ്കിൽ (arg == ശൂന്യ)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), വരി_സംഖ്യ);
          ok = false;
          സ്വതന്ത്രം (keywd);
          തുടരുക;
        }

      unrecognized = false;
      എങ്കിൽ (c_strcasecmp (keywd, "TERM") == 0)
        {
          എങ്കിൽ (state != ST_TERMSURE)
            state = fnmatch (arg, term, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          എങ്കിൽ (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      അല്ലെങ്കിൽ
        {
          എങ്കിൽ (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          എങ്കിൽ (state != ST_TERMNO)
            {
              എങ്കിൽ (keywd[0] == '.')
                append_entry ('*', keywd, arg);
              അല്ലെങ്കിൽ എങ്കിൽ (keywd[0] == '*')
                append_entry (0, keywd, arg);
              അല്ലെങ്കിൽ എങ്കിൽ (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              അല്ലെങ്കിൽ
                {
                  പൂർണ്ണസംഖ്യ i;

                  വേണ്ടി (i = 0; slack_codes[i] != ശൂന്യ; ++i)
                    എങ്കിൽ (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      നിർത്തുക;

                  എങ്കിൽ (slack_codes[i] != ശൂന്യ)
                    append_entry (0, ls_codes[i], arg);
                  അല്ലെങ്കിൽ
                    unrecognized = true;
                }
            }
          അല്ലെങ്കിൽ
            unrecognized = true;
        }

      എങ്കിൽ (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 വരി_സംഖ്യ, keywd);
          ok = false;
        }

      സ്വതന്ത്രം (keywd);
      സ്വതന്ത്രം (arg);
    }

  തിരികെ ok;
}

സ്ഥിതം bool
dc_parse_file (അക്ഷരം സ്ഥിരം *filename)
{
  bool ok;

  എങ്കിൽ (! streq (filename, "-") && freopen (filename, "r", stdin) == ശൂന്യ)
    {
      error (0, errno, "%s", quotef (filename));
      തിരികെ false;
    }

  ok = dc_parse_stream (stdin, filename);

  എങ്കിൽ (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      തിരികെ false;
    }

  തിരികെ ok;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  bool ok = true;
  പൂർണ്ണസംഖ്യ optc;
  എണ്ണൽ Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "bcp", long_options, ശൂന്യ)) != -1)
    തിരഞ്ഞെടുപ്പ് (optc)
      {
      അവസ്ഥ 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        നിർത്തുക;

      അവസ്ഥ 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        നിർത്തുക;

      അവസ്ഥ 'p':
        print_database = true;
        നിർത്തുക;

      അവസ്ഥ PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        നിർത്തുക;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      സ്ഥിരസ്ഥിതി:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  എങ്കിൽ ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      എങ്കിൽ (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (print_database)
    {
      അക്ഷരം സ്ഥിരം *p = G_വരി;
      ആകുമ്പോൾ (p - G_വരി < വലുപ്പം G_വരി)
        {
          puts (p);
          p += strlen (p) + 1;
        }
    }
  അല്ലെങ്കിൽ
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      എങ്കിൽ (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          എങ്കിൽ (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      എങ്കിൽ (argc == 0)
        ok = dc_parse_stream (ശൂന്യ, ശൂന്യ);
      അല്ലെങ്കിൽ
        ok = dc_parse_file (argv[0]);

      എങ്കിൽ (ok)
        {
          size_t len = obstack_object_size (&lsc_obstack);
          അക്ഷരം *s = obstack_finish (&lsc_obstack);
          അക്ഷരം സ്ഥിരം *prefix;
          അക്ഷരം സ്ഥിരം *suffix;

          എങ്കിൽ (syntax == SHELL_SYNTAX_BOURNE)
            {
              prefix = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          അല്ലെങ്കിൽ
            {
              prefix = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          എങ്കിൽ (! print_ls_colors)
            fputs (prefix, stdout);
          fwrite (s, 1, len, stdout);
          എങ്കിൽ (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
