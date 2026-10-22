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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fnmatch.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "dircolors.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-strcase.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "obstack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "dircolors"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("H. Peter Anvin")

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ obstack_chunk_alloc ਸਮ੍ਰਿਤੀ_ਦਿਓ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ obstack_chunk_free ਮੁਕਤ

ਗਿਣਤੀ Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
ਸਥਿਰ ਬਣਤਰ obstack lsc_obstack;

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", ਨਿਰਾ
};

ਸਥਿਰ ਅੱਖਰ ਅਚਲ *ਅਚਲ ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  ਨਿਰਾ
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
ਸਥਿਰ bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
  {
    {"bourne-shell", no_ਦਲੀਲ, ਨਿਰਾ, 'b'},
    {"sh", no_ਦਲੀਲ, ਨਿਰਾ, 'b'},
    {"csh", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
    {"c-shell", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
    {"print-database", no_ਦਲੀਲ, ਨਿਰਾ, 'p'},
    {"print-ls-colors", no_ਦਲੀਲ, ਨਿਰਾ, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
    {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
  };

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... [FILE]\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Output commands ਮੰਜ਼ਿਲ ਸੈੱਟ_2 the LS_COLORS environment variable.\n\
\n\
Determine format ਦਾ output:\n\
"), stdout);
      oputs (_("\
  -b, --sh, --bourne-shell\n\
         output Bourne shell ਕੋਡ ਮੰਜ਼ਿਲ ਸੈੱਟ_2 LS_COLORS\n\
"));
      oputs (_("\
  -c, --csh, --c-shell\n\
         output C shell ਕੋਡ ਮੰਜ਼ਿਲ ਸੈੱਟ_2 LS_COLORS\n\
"));
      oputs (_("\
  -p, --print-database\n\
         output defaults\n\
"));
      oputs (_("\
      --print-ls-colors\n\
         output fully escaped colors ਲਈ display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, ਪਠ it ਮੰਜ਼ਿਲ determine which colors ਮੰਜ਼ਿਲ use ਲਈ which\n\
file types and extensions.  Otherwise, a precompiled database is ਵਰਤਿਆ.\n\
For details on the format ਦਾ these files, ਲੜੀਵਾਰ_2 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }

  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

ਸਥਿਰ ਗਿਣਤੀ Shell_syntax
guess_shell_syntax (ਖਾਲੀ)
{
  ਅੱਖਰ *shell;

  shell = getenv ("SHELL");
  ਜੇ (shell == ਨਿਰਾ || *shell == '\0')
    ਵਾਪਸ SHELL_SYNTAX_UNKNOWN;

  shell = ਆਖਰੀ_component (shell);

  ਜੇ (streq (shell, "csh") || streq (shell, "tcsh"))
    ਵਾਪਸ SHELL_SYNTAX_C;

  ਵਾਪਸ SHELL_SYNTAX_BOURNE;
}

ਸਥਿਰ ਖਾਲੀ
parse_ਸਤਰ (ਅੱਖਰ ਅਚਲ *ਸਤਰ, ਅੱਖਰ **keyword, ਅੱਖਰ **arg)
{
  ਅੱਖਰ ਅਚਲ *p;
  ਅੱਖਰ ਅਚਲ *keyword_ਸ਼ੁਰੂ;
  ਅੱਖਰ ਅਚਲ *arg_ਸ਼ੁਰੂ;

  *keyword = ਨਿਰਾ;
  *arg = ਨਿਰਾ;

  ਲਈ (p = ਸਤਰ; c_isspace (*p); ++p)
    ਜਾਰੀ;

  /* Ignore blank lines and shell-style comments.  */
  ਜੇ (*p == '\0' || *p == '#')
    ਵਾਪਸ;

  keyword_ਸ਼ੁਰੂ = p;

  ਜਦੋਂ (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_ਸ਼ੁਰੂ, p - keyword_ਸ਼ੁਰੂ);
  ਜੇ (*p  == '\0')
    ਵਾਪਸ;

  ਕਰੋ
    {
      ++p;
    }
  ਜਦੋਂ (c_isspace (*p));

  ਜੇ (*p == '\0' || *p == '#')
    ਵਾਪਸ;

  arg_ਸ਼ੁਰੂ = p;

  ਜਦੋਂ (*p != '\0' && *p != '#')
    ++p;

  ਲਈ (--p; c_isspace (*p); --p)
    ਜਾਰੀ;
  ++p;

  *arg = ximemdup0 (arg_ਸ਼ੁਰੂ, p - arg_ਸ਼ੁਰੂ);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

ਸਥਿਰ ਖਾਲੀ
append_quoted (ਅੱਖਰ ਅਚਲ *str)
{
  bool need_backslash = true;

  ਜਦੋਂ (*str != '\0')
    {
      ਜੇ (! print_ls_colors)
        ਚੋਣ (*str)
          {
          ਹਾਲਤ '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            ਰੋਕੋ;

          ਹਾਲਤ '\\':
          ਹਾਲਤ '^':
            need_backslash = !need_backslash;
            ਰੋਕੋ;

          ਹਾਲਤ ':':
          ਹਾਲਤ '=':
            ਜੇ (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          ਮੂਲ:
            need_backslash = true;
            ਰੋਕੋ;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

ਸਥਿਰ ਖਾਲੀ
append_ਇੰਦਰਾਜ (ਅੱਖਰ ਅਗੇਤਰ, ਅੱਖਰ ਅਚਲ *ਵਸਤੂ, ਅੱਖਰ ਅਚਲ *arg)
{
  ਜੇ (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  ਜੇ (ਅਗੇਤਰ)
    APPEND_CHAR (ਅਗੇਤਰ);
  append_quoted (ਵਸਤੂ);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  ਜੇ (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

ਸਥਿਰ bool
dc_parse_ਧਾਰਾ (FILE *fp, ਅੱਖਰ ਅਚਲ *filename)
{
  idx_t ਸਤਰ_ਸੰਖਿਆ = 0;
  ਅੱਖਰ ਅਚਲ *ਅਗਲਾ_G_ਸਤਰ = G_ਸਤਰ;
  ਅੱਖਰ *ਨਿਵੇਸ਼_ਸਤਰ = ਨਿਰਾ;
  size_t ਨਿਵੇਸ਼_ਸਤਰ_ਮਾਪ = 0;
  ਅੱਖਰ ਅਚਲ *ਸਤਰ;
  ਅੱਖਰ ਅਚਲ *ਪਦ;
  ਅੱਖਰ ਅਚਲ *colorterm;
  bool ok = true;

  /* State for the parser.  */
  ਗਿਣਤੀ { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  ਪਦ = getenv ("TERM");
  ਜੇ (ਪਦ == ਨਿਰਾ || *ਪਦ == '\0')
    ਪਦ = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  ਜੇ (colorterm == ਨਿਰਾ)
    colorterm = "";  /* Doesn't match default "?*"  */

  ਜਦੋਂ (true)
    {
      ਅੱਖਰ *keywd, *arg;
      bool unrecognized;

      ++ਸਤਰ_ਸੰਖਿਆ;

      ਜੇ (fp)
        {
          ਜੇ (getline (&ਨਿਵੇਸ਼_ਸਤਰ, &ਨਿਵੇਸ਼_ਸਤਰ_ਮਾਪ, fp) <= 0)
            {
              ਜੇ (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              ਮੁਕਤ (ਨਿਵੇਸ਼_ਸਤਰ);
              ਰੋਕੋ;
            }
          ਸਤਰ = ਨਿਵੇਸ਼_ਸਤਰ;
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (ਅਗਲਾ_G_ਸਤਰ == G_ਸਤਰ + ਆਕਾਰ G_ਸਤਰ)
            ਰੋਕੋ;
          ਸਤਰ = ਅਗਲਾ_G_ਸਤਰ;
          ਅਗਲਾ_G_ਸਤਰ += ਲੜੀ_ਲੰਬਾਈ (ਅਗਲਾ_G_ਸਤਰ) + 1;
        }

      parse_ਸਤਰ (ਸਤਰ, &keywd, &arg);

      ਜੇ (keywd == ਨਿਰਾ)
        ਜਾਰੀ;

      ਜੇ (arg == ਨਿਰਾ)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), ਸਤਰ_ਸੰਖਿਆ);
          ok = false;
          ਮੁਕਤ (keywd);
          ਜਾਰੀ;
        }

      unrecognized = false;
      ਜੇ (c_strcasecmp (keywd, "TERM") == 0)
        {
          ਜੇ (state != ST_TERMSURE)
            state = fnmatch (arg, ਪਦ, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      ਨਹੀਂਤਾਂ ਜੇ (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          ਜੇ (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          ਜੇ (state != ST_TERMNO)
            {
              ਜੇ (keywd[0] == '.')
                append_ਇੰਦਰਾਜ ('*', keywd, arg);
              ਨਹੀਂਤਾਂ ਜੇ (keywd[0] == '*')
                append_ਇੰਦਰਾਜ (0, keywd, arg);
              ਨਹੀਂਤਾਂ ਜੇ (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              ਨਹੀਂਤਾਂ
                {
                  ਪੂਰਨ i;

                  ਲਈ (i = 0; slack_codes[i] != ਨਿਰਾ; ++i)
                    ਜੇ (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      ਰੋਕੋ;

                  ਜੇ (slack_codes[i] != ਨਿਰਾ)
                    append_ਇੰਦਰਾਜ (0, ls_codes[i], arg);
                  ਨਹੀਂਤਾਂ
                    unrecognized = true;
                }
            }
          ਨਹੀਂਤਾਂ
            unrecognized = true;
        }

      ਜੇ (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 ਸਤਰ_ਸੰਖਿਆ, keywd);
          ok = false;
        }

      ਮੁਕਤ (keywd);
      ਮੁਕਤ (arg);
    }

  ਵਾਪਸ ok;
}

ਸਥਿਰ bool
dc_parse_file (ਅੱਖਰ ਅਚਲ *filename)
{
  bool ok;

  ਜੇ (! streq (filename, "-") && freopen (filename, "r", stdin) == ਨਿਰਾ)
    {
      error (0, errno, "%s", quotef (filename));
      ਵਾਪਸ false;
    }

  ok = dc_parse_ਧਾਰਾ (stdin, filename);

  ਜੇ (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      ਵਾਪਸ false;
    }

  ਵਾਪਸ ok;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool ok = true;
  ਪੂਰਨ optc;
  ਗਿਣਤੀ Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "bcp", long_options, ਨਿਰਾ)) != -1)
    ਚੋਣ (optc)
      {
      ਹਾਲਤ 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        ਰੋਕੋ;

      ਹਾਲਤ 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        ਰੋਕੋ;

      ਹਾਲਤ 'p':
        print_database = true;
        ਰੋਕੋ;

      ਹਾਲਤ PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        ਰੋਕੋ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

      ਮੂਲ:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  ਜੇ ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ਜੇ ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      ਜੇ (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (print_database)
    {
      ਅੱਖਰ ਅਚਲ *p = G_ਸਤਰ;
      ਜਦੋਂ (p - G_ਸਤਰ < ਆਕਾਰ G_ਸਤਰ)
        {
          puts (p);
          p += ਲੜੀ_ਲੰਬਾਈ (p) + 1;
        }
    }
  ਨਹੀਂਤਾਂ
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      ਜੇ (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          ਜੇ (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      ਜੇ (argc == 0)
        ok = dc_parse_ਧਾਰਾ (ਨਿਰਾ, ਨਿਰਾ);
      ਨਹੀਂਤਾਂ
        ok = dc_parse_file (argv[0]);

      ਜੇ (ok)
        {
          size_t len = obstack_object_ਮਾਪ (&lsc_obstack);
          ਅੱਖਰ *s = obstack_ਅੰਤ (&lsc_obstack);
          ਅੱਖਰ ਅਚਲ *ਅਗੇਤਰ;
          ਅੱਖਰ ਅਚਲ *suffix;

          ਜੇ (syntax == SHELL_SYNTAX_BOURNE)
            {
              ਅਗੇਤਰ = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          ਨਹੀਂਤਾਂ
            {
              ਅਗੇਤਰ = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          ਜੇ (! print_ls_colors)
            fputs (ਅਗੇਤਰ, stdout);
          fwrite (s, 1, len, stdout);
          ਜੇ (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
