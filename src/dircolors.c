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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <fnmatch.h>
#અન્તર્ભાવ <getopt.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "dircolors.h"
#અન્તર્ભાવ "c-ctype.h"
#અન્તર્ભાવ "c-strcase.h"
#અન્તર્ભાવ "obstack.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dircolors"

#define AUTHORS proper_name ("H. Peter Anvin")

#define obstack_chunk_alloc malloc
#define obstack_chunk_free મુક્ત

ગણના Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#define APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
સ્થિર સંરચના obstack lsc_obstack;

સ્થિર અક્ષર અચલ *અચલ slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", ખાલી
};

સ્થિર અક્ષર અચલ *અચલ ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  ખાલી
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
સ્થિર bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

સ્થિર સંરચના option અચલ long_options[] =
  {
    {"bourne-shell", no_argument, ખાલી, 'b'},
    {"sh", no_argument, ખાલી, 'b'},
    {"csh", no_argument, ખાલી, 'c'},
    {"c-shell", no_argument, ખાલી, 'c'},
    {"print-database", no_argument, ખાલી, 'p'},
    {"print-ls-colors", no_argument, ખાલી, PRINT_LS_COLORS_OPTION},
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
         output fully escaped colors માટે display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, પઠ it to determine which colors to use માટે which\n\
file types and extensions.  Otherwise, a precompiled database is used.\n\
For details on the format of these files, run 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

સ્થિર ગણના Shell_syntax
guess_shell_syntax (શૂન્ય)
{
  અક્ષર *shell;

  shell = getenv ("SHELL");
  જો (shell == ખાલી || *shell == '\0')
    પાછા SHELL_SYNTAX_UNKNOWN;

  shell = last_component (shell);

  જો (streq (shell, "csh") || streq (shell, "tcsh"))
    પાછા SHELL_SYNTAX_C;

  પાછા SHELL_SYNTAX_BOURNE;
}

સ્થિર શૂન્ય
parse_લીટી (અક્ષર અચલ *લીટી, અક્ષર **keyword, અક્ષર **arg)
{
  અક્ષર અચલ *p;
  અક્ષર અચલ *keyword_start;
  અક્ષર અચલ *arg_start;

  *keyword = ખાલી;
  *arg = ખાલી;

  માટે (p = લીટી; c_isspace (*p); ++p)
    ચાલુ;

  /* Ignore blank lines and shell-style comments.  */
  જો (*p == '\0' || *p == '#')
    પાછા;

  keyword_start = p;

  જ્યારે (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  જો (*p  == '\0')
    પાછા;

  કરો
    {
      ++p;
    }
  જ્યારે (c_isspace (*p));

  જો (*p == '\0' || *p == '#')
    પાછા;

  arg_start = p;

  જ્યારે (*p != '\0' && *p != '#')
    ++p;

  માટે (--p; c_isspace (*p); --p)
    ચાલુ;
  ++p;

  *arg = ximemdup0 (arg_start, p - arg_start);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

સ્થિર શૂન્ય
append_quoted (અક્ષર અચલ *str)
{
  bool need_backslash = true;

  જ્યારે (*str != '\0')
    {
      જો (! print_ls_colors)
        પસંદગી (*str)
          {
          સ્થિતિ '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            તોડો;

          સ્થિતિ '\\':
          સ્થિતિ '^':
            need_backslash = !need_backslash;
            તોડો;

          સ્થિતિ ':':
          સ્થિતિ '=':
            જો (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          મૂળભૂત:
            need_backslash = true;
            તોડો;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

સ્થિર શૂન્ય
append_entry (અક્ષર prefix, અક્ષર અચલ *item, અક્ષર અચલ *arg)
{
  જો (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  જો (prefix)
    APPEND_CHAR (prefix);
  append_quoted (item);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  જો (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

સ્થિર bool
dc_parse_stream (FILE *fp, અક્ષર અચલ *filename)
{
  idx_t લીટી_સંખ્યા = 0;
  અક્ષર અચલ *next_G_લીટી = G_લીટી;
  અક્ષર *આગત_લીટી = ખાલી;
  size_t આગત_લીટી_size = 0;
  અક્ષર અચલ *લીટી;
  અક્ષર અચલ *term;
  અક્ષર અચલ *colorterm;
  bool ok = true;

  /* State for the parser.  */
  ગણના { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  term = getenv ("TERM");
  જો (term == ખાલી || *term == '\0')
    term = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  જો (colorterm == ખાલી)
    colorterm = "";  /* Doesn't match default "?*"  */

  જ્યારે (true)
    {
      અક્ષર *keywd, *arg;
      bool unrecognized;

      ++લીટી_સંખ્યા;

      જો (fp)
        {
          જો (getline (&આગત_લીટી, &આગત_લીટી_size, fp) <= 0)
            {
              જો (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              મુક્ત (આગત_લીટી);
              તોડો;
            }
          લીટી = આગત_લીટી;
        }
      નહીંતર
        {
          જો (next_G_લીટી == G_લીટી + કદ G_લીટી)
            તોડો;
          લીટી = next_G_લીટી;
          next_G_લીટી += strlen (next_G_લીટી) + 1;
        }

      parse_લીટી (લીટી, &keywd, &arg);

      જો (keywd == ખાલી)
        ચાલુ;

      જો (arg == ખાલી)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), લીટી_સંખ્યા);
          ok = false;
          મુક્ત (keywd);
          ચાલુ;
        }

      unrecognized = false;
      જો (c_strcasecmp (keywd, "TERM") == 0)
        {
          જો (state != ST_TERMSURE)
            state = fnmatch (arg, term, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      નહીંતર જો (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          જો (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      નહીંતર
        {
          જો (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          જો (state != ST_TERMNO)
            {
              જો (keywd[0] == '.')
                append_entry ('*', keywd, arg);
              નહીંતર જો (keywd[0] == '*')
                append_entry (0, keywd, arg);
              નહીંતર જો (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              નહીંતર
                {
                  પૂર્ણાંક i;

                  માટે (i = 0; slack_codes[i] != ખાલી; ++i)
                    જો (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      તોડો;

                  જો (slack_codes[i] != ખાલી)
                    append_entry (0, ls_codes[i], arg);
                  નહીંતર
                    unrecognized = true;
                }
            }
          નહીંતર
            unrecognized = true;
        }

      જો (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 લીટી_સંખ્યા, keywd);
          ok = false;
        }

      મુક્ત (keywd);
      મુક્ત (arg);
    }

  પાછા ok;
}

સ્થિર bool
dc_parse_file (અક્ષર અચલ *filename)
{
  bool ok;

  જો (! streq (filename, "-") && freopen (filename, "r", stdin) == ખાલી)
    {
      error (0, errno, "%s", quotef (filename));
      પાછા false;
    }

  ok = dc_parse_stream (stdin, filename);

  જો (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      પાછા false;
    }

  પાછા ok;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  bool ok = true;
  પૂર્ણાંક optc;
  ગણના Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((optc = getopt_long (argc, argv, "bcp", long_options, ખાલી)) != -1)
    પસંદગી (optc)
      {
      સ્થિતિ 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        તોડો;

      સ્થિતિ 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        તોડો;

      સ્થિતિ 'p':
        print_database = true;
        તોડો;

      સ્થિતિ PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        તોડો;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      મૂળભૂત:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  જો ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  જો (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  જો ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      જો (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  જો (print_database)
    {
      અક્ષર અચલ *p = G_લીટી;
      જ્યારે (p - G_લીટી < કદ G_લીટી)
        {
          puts (p);
          p += strlen (p) + 1;
        }
    }
  નહીંતર
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      જો (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          જો (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      જો (argc == 0)
        ok = dc_parse_stream (ખાલી, ખાલી);
      નહીંતર
        ok = dc_parse_file (argv[0]);

      જો (ok)
        {
          size_t len = obstack_object_size (&lsc_obstack);
          અક્ષર *s = obstack_finish (&lsc_obstack);
          અક્ષર અચલ *prefix;
          અક્ષર અચલ *suffix;

          જો (syntax == SHELL_SYNTAX_BOURNE)
            {
              prefix = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          નહીંતર
            {
              prefix = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          જો (! print_ls_colors)
            fputs (prefix, stdout);
          fwrite (s, 1, len, stdout);
          જો (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
