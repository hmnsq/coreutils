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

#شامل <config.h>

#شامل <sys/types.h>
#شامل <fnmatch.h>
#شامل <getopt.h>

#شامل "system.h"
#شامل "dircolors.h"
#شامل "c-ctype.h"
#شامل "c-strcase.h"
#شامل "obstack.h"
#شامل "quote.h"
#شامل "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "dircolors"

#تعریف_کرو AUTHORS proper_name ("H. Peter Anvin")

#تعریف_کرو obstack_chunk_alloc حافظہ_دو
#تعریف_کرو obstack_chunk_free آزاد

شمار Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#تعریف_کرو APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
ساکن ساخت obstack lsc_obstack;

ساکن حرف ثابت *ثابت slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", تہی
};

ساکن حرف ثابت *ثابت ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  تہی
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
ساکن bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

ساکن ساخت option ثابت long_options[] =
  {
    {"bourne-shell", no_دلیل, تہی, 'b'},
    {"sh", no_دلیل, تہی, 'b'},
    {"csh", no_دلیل, تہی, 'c'},
    {"c-shell", no_دلیل, تہی, 'c'},
    {"print-database", no_دلیل, تہی, 'p'},
    {"print-ls-colors", no_دلیل, تہی, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_نسخہ_OPTION_DECL},
    {تہی, 0, تہی, 0}
  };

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... [FILE]\n"), پروگرام_name);
      fputs (_("\
Output commands مقصد سیٹ_2 the LS_COLORS environment variable.\n\
\n\
Determine format کا output:\n\
"), stdout);
      oputs (_("\
  -b, --sh, --bourne-shell\n\
         output Bourne shell رمز مقصد سیٹ_2 LS_COLORS\n\
"));
      oputs (_("\
  -c, --csh, --c-shell\n\
         output C shell رمز مقصد سیٹ_2 LS_COLORS\n\
"));
      oputs (_("\
  -p, --print-database\n\
         output defaults\n\
"));
      oputs (_("\
      --print-ls-colors\n\
         output fully escaped colors برائے display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, اقرأ it مقصد determine which colors مقصد use برائے which\n\
file types and extensions.  Otherwise, a precompiled database is مستعمل.\n\
For details on the format کا these files, تسلسل_2 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }

  باہر_نکلو (حالت_2);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

ساکن شمار Shell_syntax
guess_shell_syntax (خالی)
{
  حرف *shell;

  shell = getenv ("SHELL");
  اگر (shell == تہی || *shell == '\0')
    واپس SHELL_SYNTAX_UNKNOWN;

  shell = آخری_component (shell);

  اگر (streq (shell, "csh") || streq (shell, "tcsh"))
    واپس SHELL_SYNTAX_C;

  واپس SHELL_SYNTAX_BOURNE;
}

ساکن خالی
parse_سطر (حرف ثابت *سطر, حرف **keyword, حرف **arg)
{
  حرف ثابت *p;
  حرف ثابت *keyword_آغاز;
  حرف ثابت *arg_آغاز;

  *keyword = تہی;
  *arg = تہی;

  برائے (p = سطر; c_isspace (*p); ++p)
    جاری;

  /* Ignore blank lines and shell-style comments.  */
  اگر (*p == '\0' || *p == '#')
    واپس;

  keyword_آغاز = p;

  جب (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_آغاز, p - keyword_آغاز);
  اگر (*p  == '\0')
    واپس;

  کرو
    {
      ++p;
    }
  جب (c_isspace (*p));

  اگر (*p == '\0' || *p == '#')
    واپس;

  arg_آغاز = p;

  جب (*p != '\0' && *p != '#')
    ++p;

  برائے (--p; c_isspace (*p); --p)
    جاری;
  ++p;

  *arg = ximemdup0 (arg_آغاز, p - arg_آغاز);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

ساکن خالی
append_quoted (حرف ثابت *str)
{
  bool need_backslash = true;

  جب (*str != '\0')
    {
      اگر (! print_ls_colors)
        انتخاب (*str)
          {
          حالت '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            توڑو;

          حالت '\\':
          حالت '^':
            need_backslash = !need_backslash;
            توڑو;

          حالت ':':
          حالت '=':
            اگر (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          طےشدہ:
            need_backslash = true;
            توڑو;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

ساکن خالی
append_اندراج (حرف سابقہ, حرف ثابت *شے, حرف ثابت *arg)
{
  اگر (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  اگر (سابقہ)
    APPEND_CHAR (سابقہ);
  append_quoted (شے);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  اگر (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

ساکن bool
dc_parse_دھارا (FILE *fp, حرف ثابت *filename)
{
  idx_t سطر_نمبر = 0;
  حرف ثابت *اگلا_G_سطر = G_سطر;
  حرف *داخلہ_سطر = تہی;
  size_t داخلہ_سطر_سائز = 0;
  حرف ثابت *سطر;
  حرف ثابت *رکن;
  حرف ثابت *colorterm;
  bool ok = true;

  /* State for the parser.  */
  شمار { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  رکن = getenv ("TERM");
  اگر (رکن == تہی || *رکن == '\0')
    رکن = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  اگر (colorterm == تہی)
    colorterm = "";  /* Doesn't match default "?*"  */

  جب (true)
    {
      حرف *keywd, *arg;
      bool unrecognized;

      ++سطر_نمبر;

      اگر (fp)
        {
          اگر (getline (&داخلہ_سطر, &داخلہ_سطر_سائز, fp) <= 0)
            {
              اگر (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              آزاد (داخلہ_سطر);
              توڑو;
            }
          سطر = داخلہ_سطر;
        }
      ورنہ
        {
          اگر (اگلا_G_سطر == G_سطر + حجم G_سطر)
            توڑو;
          سطر = اگلا_G_سطر;
          اگلا_G_سطر += لڑی_لمبائی (اگلا_G_سطر) + 1;
        }

      parse_سطر (سطر, &keywd, &arg);

      اگر (keywd == تہی)
        جاری;

      اگر (arg == تہی)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), سطر_نمبر);
          ok = false;
          آزاد (keywd);
          جاری;
        }

      unrecognized = false;
      اگر (c_strcasecmp (keywd, "TERM") == 0)
        {
          اگر (state != ST_TERMSURE)
            state = fnmatch (arg, رکن, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      ورنہ اگر (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          اگر (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      ورنہ
        {
          اگر (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          اگر (state != ST_TERMNO)
            {
              اگر (keywd[0] == '.')
                append_اندراج ('*', keywd, arg);
              ورنہ اگر (keywd[0] == '*')
                append_اندراج (0, keywd, arg);
              ورنہ اگر (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              ورنہ
                {
                  صحیح i;

                  برائے (i = 0; slack_codes[i] != تہی; ++i)
                    اگر (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      توڑو;

                  اگر (slack_codes[i] != تہی)
                    append_اندراج (0, ls_codes[i], arg);
                  ورنہ
                    unrecognized = true;
                }
            }
          ورنہ
            unrecognized = true;
        }

      اگر (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 سطر_نمبر, keywd);
          ok = false;
        }

      آزاد (keywd);
      آزاد (arg);
    }

  واپس ok;
}

ساکن bool
dc_parse_file (حرف ثابت *filename)
{
  bool ok;

  اگر (! streq (filename, "-") && freopen (filename, "r", stdin) == تہی)
    {
      error (0, errno, "%s", quotef (filename));
      واپس false;
    }

  ok = dc_parse_دھارا (stdin, filename);

  اگر (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      واپس false;
    }

  واپس ok;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool ok = true;
  صحیح optc;
  شمار Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, "bcp", long_options, تہی)) != -1)
    انتخاب (optc)
      {
      حالت 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        توڑو;

      حالت 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        توڑو;

      حالت 'p':
        print_database = true;
        توڑو;

      حالت PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        توڑو;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

      طےشدہ:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  اگر ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  اگر (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  اگر ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      اگر (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  اگر (print_database)
    {
      حرف ثابت *p = G_سطر;
      جب (p - G_سطر < حجم G_سطر)
        {
          puts (p);
          p += لڑی_لمبائی (p) + 1;
        }
    }
  ورنہ
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      اگر (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          اگر (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      اگر (argc == 0)
        ok = dc_parse_دھارا (تہی, تہی);
      ورنہ
        ok = dc_parse_file (argv[0]);

      اگر (ok)
        {
          size_t len = obstack_object_سائز (&lsc_obstack);
          حرف *s = obstack_اختتام (&lsc_obstack);
          حرف ثابت *سابقہ;
          حرف ثابت *suffix;

          اگر (syntax == SHELL_SYNTAX_BOURNE)
            {
              سابقہ = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          ورنہ
            {
              سابقہ = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          اگر (! print_ls_colors)
            fputs (سابقہ, stdout);
          fwrite (s, 1, len, stdout);
          اگر (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
