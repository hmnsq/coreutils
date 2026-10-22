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

#ضمن <config.h>

#ضمن <sys/types.h>
#ضمن <fnmatch.h>
#ضمن <getopt.h>

#ضمن "system.h"
#ضمن "dircolors.h"
#ضمن "c-ctype.h"
#ضمن "c-strcase.h"
#ضمن "obstack.h"
#ضمن "quote.h"
#ضمن "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dircolors"

#define AUTHORS proper_name ("H. Peter Anvin")

#define obstack_chunk_alloc malloc
#define obstack_chunk_free حرر

تعداد Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#define APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
ساكن بنية obstack lsc_obstack;

ساكن حرف ثابت *ثابت slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", عدم
};

ساكن حرف ثابت *ثابت ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  عدم
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
ساكن bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

ساكن بنية option ثابت long_options[] =
  {
    {"bourne-shell", no_argument, عدم, 'b'},
    {"sh", no_argument, عدم, 'b'},
    {"csh", no_argument, عدم, 'c'},
    {"c-shell", no_argument, عدم, 'c'},
    {"print-database", no_argument, عدم, 'p'},
    {"print-ls-colors", no_argument, عدم, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {عدم, 0, عدم, 0}
  };

فراغ
usage (صحيح status)
{
  𐤀𐤌 (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
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
         output fully escaped colors لكل display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, اقرأ it to determine which colors to use لكل which\n\
file types and extensions.  Otherwise, a precompiled database is used.\n\
For details on the format of these files, run 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

ساكن تعداد Shell_syntax
guess_shell_syntax (فراغ)
{
  حرف *shell;

  shell = getenv ("SHELL");
  𐤀𐤌 (shell == عدم || *shell == '\0')
    أرجع SHELL_SYNTAX_UNKNOWN;

  shell = last_component (shell);

  𐤀𐤌 (streq (shell, "csh") || streq (shell, "tcsh"))
    أرجع SHELL_SYNTAX_C;

  أرجع SHELL_SYNTAX_BOURNE;
}

ساكن فراغ
parse_line (حرف ثابت *line, حرف **keyword, حرف **arg)
{
  حرف ثابت *p;
  حرف ثابت *keyword_start;
  حرف ثابت *arg_start;

  *keyword = عدم;
  *arg = عدم;

  لكل (p = line; c_isspace (*p); ++p)
    استمر;

  /* Ignore blank lines and shell-style comments.  */
  𐤀𐤌 (*p == '\0' || *p == '#')
    أرجع;

  keyword_start = p;

  طالما (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  𐤀𐤌 (*p  == '\0')
    أرجع;

  𐤐𐤏𐤋
    {
      ++p;
    }
  طالما (c_isspace (*p));

  𐤀𐤌 (*p == '\0' || *p == '#')
    أرجع;

  arg_start = p;

  طالما (*p != '\0' && *p != '#')
    ++p;

  لكل (--p; c_isspace (*p); --p)
    استمر;
  ++p;

  *arg = ximemdup0 (arg_start, p - arg_start);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

ساكن فراغ
append_quoted (حرف ثابت *str)
{
  bool need_backslash = true;

  طالما (*str != '\0')
    {
      𐤀𐤌 (! print_ls_colors)
        بدل (*str)
          {
          حالة '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            اكسر;

          حالة '\\':
          حالة '^':
            need_backslash = !need_backslash;
            اكسر;

          حالة ':':
          حالة '=':
            𐤀𐤌 (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          افتراضي:
            need_backslash = true;
            اكسر;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

ساكن فراغ
append_entry (حرف prefix, حرف ثابت *item, حرف ثابت *arg)
{
  𐤀𐤌 (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  𐤀𐤌 (prefix)
    APPEND_CHAR (prefix);
  append_quoted (item);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  𐤀𐤌 (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

ساكن bool
dc_parse_stream (FILE *fp, حرف ثابت *filename)
{
  idx_t line_number = 0;
  حرف ثابت *next_G_line = G_line;
  حرف *input_line = عدم;
  size_t input_line_size = 0;
  حرف ثابت *line;
  حرف ثابت *term;
  حرف ثابت *colorterm;
  bool ok = true;

  /* State for the parser.  */
  تعداد { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  term = getenv ("TERM");
  𐤀𐤌 (term == عدم || *term == '\0')
    term = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  𐤀𐤌 (colorterm == عدم)
    colorterm = "";  /* Doesn't match default "?*"  */

  طالما (true)
    {
      حرف *keywd, *arg;
      bool unrecognized;

      ++line_number;

      𐤀𐤌 (fp)
        {
          𐤀𐤌 (getline (&input_line, &input_line_size, fp) <= 0)
            {
              𐤀𐤌 (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              حرر (input_line);
              اكسر;
            }
          line = input_line;
        }
      وإلا
        {
          𐤀𐤌 (next_G_line == G_line + حجم G_line)
            اكسر;
          line = next_G_line;
          next_G_line += strlen (next_G_line) + 1;
        }

      parse_line (line, &keywd, &arg);

      𐤀𐤌 (keywd == عدم)
        استمر;

      𐤀𐤌 (arg == عدم)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), line_number);
          ok = false;
          حرر (keywd);
          استمر;
        }

      unrecognized = false;
      𐤀𐤌 (c_strcasecmp (keywd, "TERM") == 0)
        {
          𐤀𐤌 (state != ST_TERMSURE)
            state = fnmatch (arg, term, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      وإلا 𐤀𐤌 (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          𐤀𐤌 (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      وإلا
        {
          𐤀𐤌 (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          𐤀𐤌 (state != ST_TERMNO)
            {
              𐤀𐤌 (keywd[0] == '.')
                append_entry ('*', keywd, arg);
              وإلا 𐤀𐤌 (keywd[0] == '*')
                append_entry (0, keywd, arg);
              وإلا 𐤀𐤌 (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              وإلا
                {
                  صحيح i;

                  لكل (i = 0; slack_codes[i] != عدم; ++i)
                    𐤀𐤌 (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      اكسر;

                  𐤀𐤌 (slack_codes[i] != عدم)
                    append_entry (0, ls_codes[i], arg);
                  وإلا
                    unrecognized = true;
                }
            }
          وإلا
            unrecognized = true;
        }

      𐤀𐤌 (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 line_number, keywd);
          ok = false;
        }

      حرر (keywd);
      حرر (arg);
    }

  أرجع ok;
}

ساكن bool
dc_parse_file (حرف ثابت *filename)
{
  bool ok;

  𐤀𐤌 (! streq (filename, "-") && freopen (filename, "r", stdin) == عدم)
    {
      error (0, errno, "%s", quotef (filename));
      أرجع false;
    }

  ok = dc_parse_stream (stdin, filename);

  𐤀𐤌 (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      أرجع false;
    }

  أرجع ok;
}

صحيح
main (صحيح argc, حرف **argv)
{
  bool ok = true;
  صحيح optc;
  تعداد Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "bcp", long_options, عدم)) != -1)
    بدل (optc)
      {
      حالة 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        اكسر;

      حالة 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        اكسر;

      حالة 'p':
        print_database = true;
        اكسر;

      حالة PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        اكسر;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      افتراضي:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  𐤀𐤌 ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  𐤀𐤌 (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  𐤀𐤌 ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      𐤀𐤌 (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  𐤀𐤌 (print_database)
    {
      حرف ثابت *p = G_line;
      طالما (p - G_line < حجم G_line)
        {
          puts (p);
          p += strlen (p) + 1;
        }
    }
  وإلا
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      𐤀𐤌 (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          𐤀𐤌 (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      𐤀𐤌 (argc == 0)
        ok = dc_parse_stream (عدم, عدم);
      وإلا
        ok = dc_parse_file (argv[0]);

      𐤀𐤌 (ok)
        {
          size_t len = obstack_object_size (&lsc_obstack);
          حرف *s = obstack_finish (&lsc_obstack);
          حرف ثابت *prefix;
          حرف ثابت *suffix;

          𐤀𐤌 (syntax == SHELL_SYNTAX_BOURNE)
            {
              prefix = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          وإلا
            {
              prefix = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          𐤀𐤌 (! print_ls_colors)
            fputs (prefix, stdout);
          fwrite (s, 1, len, stdout);
          𐤀𐤌 (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
