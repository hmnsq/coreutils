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
#define PROGRAM_NAME "dircolors"

#define AUTHORS proper_name ("H. Peter Anvin")

#define obstack_chunk_alloc malloc
#define obstack_chunk_free آزاد

شمارش Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#define APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
ایستا ساختار obstack lsc_obstack;

ایستا نویسه ثابت *ثابت slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", پوچ
};

ایستا نویسه ثابت *ثابت ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  پوچ
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
ایستا bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

ایستا ساختار option ثابت long_options[] =
  {
    {"bourne-shell", no_argument, پوچ, 'b'},
    {"sh", no_argument, پوچ, 'b'},
    {"csh", no_argument, پوچ, 'c'},
    {"c-shell", no_argument, پوچ, 'c'},
    {"print-database", no_argument, پوچ, 'p'},
    {"print-ls-colors", no_argument, پوچ, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {پوچ, 0, پوچ, 0}
  };

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
         output fully escaped colors برای display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, اقرأ it to determine which colors to use برای which\n\
file types and extensions.  Otherwise, a precompiled database is used.\n\
For details on the format of these files, run 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

ایستا شمارش Shell_syntax
guess_shell_syntax (تهی)
{
  نویسه *shell;

  shell = getenv ("SHELL");
  اگر (shell == پوچ || *shell == '\0')
    بازگشت SHELL_SYNTAX_UNKNOWN;

  shell = last_component (shell);

  اگر (streq (shell, "csh") || streq (shell, "tcsh"))
    بازگشت SHELL_SYNTAX_C;

  بازگشت SHELL_SYNTAX_BOURNE;
}

ایستا تهی
parse_سطر (نویسه ثابت *سطر, نویسه **keyword, نویسه **arg)
{
  نویسه ثابت *p;
  نویسه ثابت *keyword_start;
  نویسه ثابت *arg_start;

  *keyword = پوچ;
  *arg = پوچ;

  برای (p = سطر; c_isspace (*p); ++p)
    ادامه;

  /* Ignore blank lines and shell-style comments.  */
  اگر (*p == '\0' || *p == '#')
    بازگشت;

  keyword_start = p;

  وقتی (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  اگر (*p  == '\0')
    بازگشت;

  انجامبده
    {
      ++p;
    }
  وقتی (c_isspace (*p));

  اگر (*p == '\0' || *p == '#')
    بازگشت;

  arg_start = p;

  وقتی (*p != '\0' && *p != '#')
    ++p;

  برای (--p; c_isspace (*p); --p)
    ادامه;
  ++p;

  *arg = ximemdup0 (arg_start, p - arg_start);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

ایستا تهی
append_quoted (نویسه ثابت *str)
{
  bool need_backslash = true;

  وقتی (*str != '\0')
    {
      اگر (! print_ls_colors)
        انتخاب (*str)
          {
          حالت '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            بشکن;

          حالت '\\':
          حالت '^':
            need_backslash = !need_backslash;
            بشکن;

          حالت ':':
          حالت '=':
            اگر (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          پیشفرض:
            need_backslash = true;
            بشکن;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

ایستا تهی
append_entry (نویسه prefix, نویسه ثابت *item, نویسه ثابت *arg)
{
  اگر (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  اگر (prefix)
    APPEND_CHAR (prefix);
  append_quoted (item);
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

ایستا bool
dc_parse_stream (FILE *fp, نویسه ثابت *filename)
{
  idx_t سطر_عدد = 0;
  نویسه ثابت *next_G_سطر = G_سطر;
  نویسه *درون_داد_سطر = پوچ;
  size_t درون_داد_سطر_size = 0;
  نویسه ثابت *سطر;
  نویسه ثابت *term;
  نویسه ثابت *colorterm;
  bool ok = true;

  /* State for the parser.  */
  شمارش { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  term = getenv ("TERM");
  اگر (term == پوچ || *term == '\0')
    term = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  اگر (colorterm == پوچ)
    colorterm = "";  /* Doesn't match default "?*"  */

  وقتی (true)
    {
      نویسه *keywd, *arg;
      bool unrecognized;

      ++سطر_عدد;

      اگر (fp)
        {
          اگر (getline (&درون_داد_سطر, &درون_داد_سطر_size, fp) <= 0)
            {
              اگر (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              آزاد (درون_داد_سطر);
              بشکن;
            }
          سطر = درون_داد_سطر;
        }
      وگرنه
        {
          اگر (next_G_سطر == G_سطر + اندازه G_سطر)
            بشکن;
          سطر = next_G_سطر;
          next_G_سطر += strlen (next_G_سطر) + 1;
        }

      parse_سطر (سطر, &keywd, &arg);

      اگر (keywd == پوچ)
        ادامه;

      اگر (arg == پوچ)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), سطر_عدد);
          ok = false;
          آزاد (keywd);
          ادامه;
        }

      unrecognized = false;
      اگر (c_strcasecmp (keywd, "TERM") == 0)
        {
          اگر (state != ST_TERMSURE)
            state = fnmatch (arg, term, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      وگرنه اگر (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          اگر (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      وگرنه
        {
          اگر (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          اگر (state != ST_TERMNO)
            {
              اگر (keywd[0] == '.')
                append_entry ('*', keywd, arg);
              وگرنه اگر (keywd[0] == '*')
                append_entry (0, keywd, arg);
              وگرنه اگر (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              وگرنه
                {
                  صحیح i;

                  برای (i = 0; slack_codes[i] != پوچ; ++i)
                    اگر (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      بشکن;

                  اگر (slack_codes[i] != پوچ)
                    append_entry (0, ls_codes[i], arg);
                  وگرنه
                    unrecognized = true;
                }
            }
          وگرنه
            unrecognized = true;
        }

      اگر (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 سطر_عدد, keywd);
          ok = false;
        }

      آزاد (keywd);
      آزاد (arg);
    }

  بازگشت ok;
}

ایستا bool
dc_parse_file (نویسه ثابت *filename)
{
  bool ok;

  اگر (! streq (filename, "-") && freopen (filename, "r", stdin) == پوچ)
    {
      error (0, errno, "%s", quotef (filename));
      بازگشت false;
    }

  ok = dc_parse_stream (stdin, filename);

  اگر (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      بازگشت false;
    }

  بازگشت ok;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  bool ok = true;
  صحیح optc;
  شمارش Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی ((optc = getopt_long (argc, argv, "bcp", long_options, پوچ)) != -1)
    انتخاب (optc)
      {
      حالت 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        بشکن;

      حالت 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        بشکن;

      حالت 'p':
        print_database = true;
        بشکن;

      حالت PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        بشکن;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      پیشفرض:
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
      نویسه ثابت *p = G_سطر;
      وقتی (p - G_سطر < اندازه G_سطر)
        {
          puts (p);
          p += strlen (p) + 1;
        }
    }
  وگرنه
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
        ok = dc_parse_stream (پوچ, پوچ);
      وگرنه
        ok = dc_parse_file (argv[0]);

      اگر (ok)
        {
          size_t len = obstack_object_size (&lsc_obstack);
          نویسه *s = obstack_finish (&lsc_obstack);
          نویسه ثابت *prefix;
          نویسه ثابت *suffix;

          اگر (syntax == SHELL_SYNTAX_BOURNE)
            {
              prefix = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          وگرنه
            {
              prefix = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          اگر (! print_ls_colors)
            fputs (prefix, stdout);
          fwrite (s, 1, len, stdout);
          اگر (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
