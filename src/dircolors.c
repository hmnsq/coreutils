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
ساكن bool اطبع_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  اطبع_LS_COLORS_OPTION = CHAR_MAX + 1,
};

ساكن بنية option ثابت long_options[] =
  {
    {"bourne-shell", no_argument, عدم, 'b'},
    {"sh", no_argument, عدم, 'b'},
    {"csh", no_argument, عدم, 'c'},
    {"c-shell", no_argument, عدم, 'c'},
    {"print-database", no_argument, عدم, 'p'},
    {"print-ls-colors", no_argument, عدم, اطبع_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {عدم, 0, عدم, 0}
  };

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
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
  -p, --اطبع_2-database\n\
         output defaults\n\
"));
      oputs (_("\
      --اطبع_2-ls-colors\n\
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
  إذا (shell == عدم || *shell == '\0')
    أرجع SHELL_SYNTAX_UNKNOWN;

  shell = last_component (shell);

  إذا (streq (shell, "csh") || streq (shell, "tcsh"))
    أرجع SHELL_SYNTAX_C;

  أرجع SHELL_SYNTAX_BOURNE;
}

ساكن فراغ
parse_سطر (حرف ثابت *سطر, حرف **keyword, حرف **arg)
{
  حرف ثابت *p;
  حرف ثابت *keyword_start;
  حرف ثابت *arg_start;

  *keyword = عدم;
  *arg = عدم;

  لكل (p = سطر; c_isspace (*p); ++p)
    استمر;

  /* Ignore blank lines and shell-style comments.  */
  إذا (*p == '\0' || *p == '#')
    أرجع;

  keyword_start = p;

  طالما (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  إذا (*p  == '\0')
    أرجع;

  افعل
    {
      ++p;
    }
  طالما (c_isspace (*p));

  إذا (*p == '\0' || *p == '#')
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
      إذا (! اطبع_ls_colors)
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
            إذا (need_backslash)
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
  إذا (اطبع_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  إذا (prefix)
    APPEND_CHAR (prefix);
  append_quoted (item);
  APPEND_CHAR (اطبع_ls_colors ? '\t' : '=');
  append_quoted (arg);
  إذا (اطبع_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (اطبع_ls_colors ? '\n' : ':');
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
  idx_t سطر_رقم = 0;
  حرف ثابت *next_G_سطر = G_سطر;
  حرف *مدخل_سطر = عدم;
  size_t مدخل_سطر_size = 0;
  حرف ثابت *سطر;
  حرف ثابت *term;
  حرف ثابت *colorterm;
  bool ok = true;

  /* State for the parser.  */
  تعداد { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  term = getenv ("TERM");
  إذا (term == عدم || *term == '\0')
    term = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  إذا (colorterm == عدم)
    colorterm = "";  /* Doesn't match default "?*"  */

  طالما (true)
    {
      حرف *keywd, *arg;
      bool unrecognized;

      ++سطر_رقم;

      إذا (fp)
        {
          إذا (getline (&مدخل_سطر, &مدخل_سطر_size, fp) <= 0)
            {
              إذا (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              حرر (مدخل_سطر);
              اكسر;
            }
          سطر = مدخل_سطر;
        }
      وإلا
        {
          إذا (next_G_سطر == G_سطر + حجم G_سطر)
            اكسر;
          سطر = next_G_سطر;
          next_G_سطر += strlen (next_G_سطر) + 1;
        }

      parse_سطر (سطر, &keywd, &arg);

      إذا (keywd == عدم)
        استمر;

      إذا (arg == عدم)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), سطر_رقم);
          ok = false;
          حرر (keywd);
          استمر;
        }

      unrecognized = false;
      إذا (c_strcasecmp (keywd, "TERM") == 0)
        {
          إذا (state != ST_TERMSURE)
            state = fnmatch (arg, term, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      وإلا إذا (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          إذا (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      وإلا
        {
          إذا (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          إذا (state != ST_TERMNO)
            {
              إذا (keywd[0] == '.')
                append_entry ('*', keywd, arg);
              وإلا إذا (keywd[0] == '*')
                append_entry (0, keywd, arg);
              وإلا إذا (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              وإلا
                {
                  صحيح i;

                  لكل (i = 0; slack_codes[i] != عدم; ++i)
                    إذا (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      اكسر;

                  إذا (slack_codes[i] != عدم)
                    append_entry (0, ls_codes[i], arg);
                  وإلا
                    unrecognized = true;
                }
            }
          وإلا
            unrecognized = true;
        }

      إذا (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 سطر_رقم, keywd);
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

  إذا (! streq (filename, "-") && freopen (filename, "r", stdin) == عدم)
    {
      error (0, errno, "%s", quotef (filename));
      أرجع false;
    }

  ok = dc_parse_stream (stdin, filename);

  إذا (fclose (stdin) != 0)
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
  bool اطبع_database = false;

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
        اطبع_database = true;
        اكسر;

      حالة اطبع_LS_COLORS_OPTION:
        اطبع_ls_colors = true;
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
  إذا ((اطبع_database | اطبع_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  إذا (اطبع_database && اطبع_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  إذا ((!اطبع_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!اطبع_database]));
      إذا (اطبع_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  إذا (اطبع_database)
    {
      حرف ثابت *p = G_سطر;
      طالما (p - G_سطر < حجم G_سطر)
        {
          puts (p);
          p += strlen (p) + 1;
        }
    }
  وإلا
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      إذا (syntax == SHELL_SYNTAX_UNKNOWN && ! اطبع_ls_colors)
        {
          syntax = guess_shell_syntax ();
          إذا (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      إذا (argc == 0)
        ok = dc_parse_stream (عدم, عدم);
      وإلا
        ok = dc_parse_file (argv[0]);

      إذا (ok)
        {
          size_t len = obstack_object_size (&lsc_obstack);
          حرف *s = obstack_finish (&lsc_obstack);
          حرف ثابت *prefix;
          حرف ثابت *suffix;

          إذا (syntax == SHELL_SYNTAX_BOURNE)
            {
              prefix = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          وإلا
            {
              prefix = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          إذا (! اطبع_ls_colors)
            fputs (prefix, stdout);
          fwrite (s, 1, len, stdout);
          إذا (! اطبع_ls_colors)
            fputs (suffix, stdout);
        }
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
