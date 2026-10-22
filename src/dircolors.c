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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <fnmatch.h>
#অন্তর্ভাব <getopt.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "dircolors.h"
#অন্তর্ভাব "c-ctype.h"
#অন্তর্ভাব "c-strcase.h"
#অন্তর্ভাব "obstack.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dircolors"

#define AUTHORS proper_name ("H. Peter Anvin")

#define obstack_chunk_alloc malloc
#define obstack_chunk_free মুক্ত

গণনা Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#define APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
স্থির কাঠামো obstack lsc_obstack;

স্থির অক্ষর ধ্রুবক *ধ্রুবক slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", ফাঁকা
};

স্থির অক্ষর ধ্রুবক *ধ্রুবক ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  ফাঁকা
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
স্থির bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

স্থির কাঠামো option ধ্রুবক long_options[] =
  {
    {"bourne-shell", no_argument, ফাঁকা, 'b'},
    {"sh", no_argument, ফাঁকা, 'b'},
    {"csh", no_argument, ফাঁকা, 'c'},
    {"c-shell", no_argument, ফাঁকা, 'c'},
    {"print-database", no_argument, ফাঁকা, 'p'},
    {"print-ls-colors", no_argument, ফাঁকা, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {ফাঁকা, 0, ফাঁকা, 0}
  };

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
         output fully escaped colors জন্য display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, পঠ it to determine which colors to use জন্য which\n\
file types and extensions.  Otherwise, a precompiled database is used.\n\
For details on the format of these files, run 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

স্থির গণনা Shell_syntax
guess_shell_syntax (শূন্য)
{
  অক্ষর *shell;

  shell = getenv ("SHELL");
  যদি (shell == ফাঁকা || *shell == '\0')
    ফেরত SHELL_SYNTAX_UNKNOWN;

  shell = last_component (shell);

  যদি (streq (shell, "csh") || streq (shell, "tcsh"))
    ফেরত SHELL_SYNTAX_C;

  ফেরত SHELL_SYNTAX_BOURNE;
}

স্থির শূন্য
parse_সারি (অক্ষর ধ্রুবক *সারি, অক্ষর **keyword, অক্ষর **arg)
{
  অক্ষর ধ্রুবক *p;
  অক্ষর ধ্রুবক *keyword_start;
  অক্ষর ধ্রুবক *arg_start;

  *keyword = ফাঁকা;
  *arg = ফাঁকা;

  জন্য (p = সারি; c_isspace (*p); ++p)
    চালু;

  /* Ignore blank lines and shell-style comments.  */
  যদি (*p == '\0' || *p == '#')
    ফেরত;

  keyword_start = p;

  যখন (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  যদি (*p  == '\0')
    ফেরত;

  করো
    {
      ++p;
    }
  যখন (c_isspace (*p));

  যদি (*p == '\0' || *p == '#')
    ফেরত;

  arg_start = p;

  যখন (*p != '\0' && *p != '#')
    ++p;

  জন্য (--p; c_isspace (*p); --p)
    চালু;
  ++p;

  *arg = ximemdup0 (arg_start, p - arg_start);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

স্থির শূন্য
append_quoted (অক্ষর ধ্রুবক *str)
{
  bool need_backslash = true;

  যখন (*str != '\0')
    {
      যদি (! print_ls_colors)
        নির্বাচন (*str)
          {
          ক্ষেত্র '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            বিরতি;

          ক্ষেত্র '\\':
          ক্ষেত্র '^':
            need_backslash = !need_backslash;
            বিরতি;

          ক্ষেত্র ':':
          ক্ষেত্র '=':
            যদি (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          পূর্বনির্ধারিত:
            need_backslash = true;
            বিরতি;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

স্থির শূন্য
append_entry (অক্ষর prefix, অক্ষর ধ্রুবক *item, অক্ষর ধ্রুবক *arg)
{
  যদি (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  যদি (prefix)
    APPEND_CHAR (prefix);
  append_quoted (item);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  যদি (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

স্থির bool
dc_parse_stream (FILE *fp, অক্ষর ধ্রুবক *filename)
{
  idx_t সারি_সংখ্যা = 0;
  অক্ষর ধ্রুবক *পরবর্তী_G_সারি = G_সারি;
  অক্ষর *প্রবেশ_সারি = ফাঁকা;
  size_t প্রবেশ_সারি_আকার = 0;
  অক্ষর ধ্রুবক *সারি;
  অক্ষর ধ্রুবক *term;
  অক্ষর ধ্রুবক *colorterm;
  bool ok = true;

  /* State for the parser.  */
  গণনা { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  term = getenv ("TERM");
  যদি (term == ফাঁকা || *term == '\0')
    term = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  যদি (colorterm == ফাঁকা)
    colorterm = "";  /* Doesn't match default "?*"  */

  যখন (true)
    {
      অক্ষর *keywd, *arg;
      bool unrecognized;

      ++সারি_সংখ্যা;

      যদি (fp)
        {
          যদি (getline (&প্রবেশ_সারি, &প্রবেশ_সারি_আকার, fp) <= 0)
            {
              যদি (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              মুক্ত (প্রবেশ_সারি);
              বিরতি;
            }
          সারি = প্রবেশ_সারি;
        }
      অন্যথা
        {
          যদি (পরবর্তী_G_সারি == G_সারি + আকার G_সারি)
            বিরতি;
          সারি = পরবর্তী_G_সারি;
          পরবর্তী_G_সারি += strlen (পরবর্তী_G_সারি) + 1;
        }

      parse_সারি (সারি, &keywd, &arg);

      যদি (keywd == ফাঁকা)
        চালু;

      যদি (arg == ফাঁকা)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), সারি_সংখ্যা);
          ok = false;
          মুক্ত (keywd);
          চালু;
        }

      unrecognized = false;
      যদি (c_strcasecmp (keywd, "TERM") == 0)
        {
          যদি (state != ST_TERMSURE)
            state = fnmatch (arg, term, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      অন্যথা যদি (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          যদি (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      অন্যথা
        {
          যদি (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          যদি (state != ST_TERMNO)
            {
              যদি (keywd[0] == '.')
                append_entry ('*', keywd, arg);
              অন্যথা যদি (keywd[0] == '*')
                append_entry (0, keywd, arg);
              অন্যথা যদি (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              অন্যথা
                {
                  পূর্ণসংখ্যা i;

                  জন্য (i = 0; slack_codes[i] != ফাঁকা; ++i)
                    যদি (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      বিরতি;

                  যদি (slack_codes[i] != ফাঁকা)
                    append_entry (0, ls_codes[i], arg);
                  অন্যথা
                    unrecognized = true;
                }
            }
          অন্যথা
            unrecognized = true;
        }

      যদি (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 সারি_সংখ্যা, keywd);
          ok = false;
        }

      মুক্ত (keywd);
      মুক্ত (arg);
    }

  ফেরত ok;
}

স্থির bool
dc_parse_file (অক্ষর ধ্রুবক *filename)
{
  bool ok;

  যদি (! streq (filename, "-") && freopen (filename, "r", stdin) == ফাঁকা)
    {
      error (0, errno, "%s", quotef (filename));
      ফেরত false;
    }

  ok = dc_parse_stream (stdin, filename);

  যদি (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      ফেরত false;
    }

  ফেরত ok;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool ok = true;
  পূর্ণসংখ্যা optc;
  গণনা Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন ((optc = getopt_long (argc, argv, "bcp", long_options, ফাঁকা)) != -1)
    নির্বাচন (optc)
      {
      ক্ষেত্র 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        বিরতি;

      ক্ষেত্র 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        বিরতি;

      ক্ষেত্র 'p':
        print_database = true;
        বিরতি;

      ক্ষেত্র PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        বিরতি;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      পূর্বনির্ধারিত:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  যদি ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  যদি (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  যদি ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      যদি (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  যদি (print_database)
    {
      অক্ষর ধ্রুবক *p = G_সারি;
      যখন (p - G_সারি < আকার G_সারি)
        {
          puts (p);
          p += strlen (p) + 1;
        }
    }
  অন্যথা
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      যদি (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          যদি (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      যদি (argc == 0)
        ok = dc_parse_stream (ফাঁকা, ফাঁকা);
      অন্যথা
        ok = dc_parse_file (argv[0]);

      যদি (ok)
        {
          size_t len = obstack_object_আকার (&lsc_obstack);
          অক্ষর *s = obstack_finish (&lsc_obstack);
          অক্ষর ধ্রুবক *prefix;
          অক্ষর ধ্রুবক *suffix;

          যদি (syntax == SHELL_SYNTAX_BOURNE)
            {
              prefix = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          অন্যথা
            {
              prefix = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          যদি (! print_ls_colors)
            fputs (prefix, stdout);
          fwrite (s, 1, len, stdout);
          যদি (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
