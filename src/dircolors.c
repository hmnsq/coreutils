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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <fnmatch.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "dircolors.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"
#ಅನ್ತರ್ಭಾವ "c-strcase.h"
#ಅನ್ತರ್ಭಾವ "obstack.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dircolors"

#define AUTHORS proper_name ("H. Peter Anvin")

#define obstack_chunk_alloc malloc
#define obstack_chunk_free ಮುಕ್ತ

ಗಣನಾ Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#define APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
ಸ್ಥಿರ ಸಂರಚನಾ obstack lsc_obstack;

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", ರಿಕ್ತ
};

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *ಧ್ರುವ ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  ರಿಕ್ತ
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
ಸ್ಥಿರ bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
  {
    {"bourne-shell", no_argument, ರಿಕ್ತ, 'b'},
    {"sh", no_argument, ರಿಕ್ತ, 'b'},
    {"csh", no_argument, ರಿಕ್ತ, 'c'},
    {"c-shell", no_argument, ರಿಕ್ತ, 'c'},
    {"print-database", no_argument, ರಿಕ್ತ, 'p'},
    {"print-ls-colors", no_argument, ರಿಕ್ತ, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
  };

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
         output fully escaped colors ಪ್ರತಿ display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, ಪಠ it to determine which colors to use ಪ್ರತಿ which\n\
file types and extensions.  Otherwise, a precompiled database is used.\n\
For details on the format of these files, run 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

ಸ್ಥಿರ ಗಣನಾ Shell_syntax
guess_shell_syntax (ಶೂನ್ಯ)
{
  ವರ್ಣ *shell;

  shell = getenv ("SHELL");
  ಯದಿ (shell == ರಿಕ್ತ || *shell == '\0')
    ಪ್ರತ್ಯಾಗಮ SHELL_SYNTAX_UNKNOWN;

  shell = last_component (shell);

  ಯದಿ (streq (shell, "csh") || streq (shell, "tcsh"))
    ಪ್ರತ್ಯಾಗಮ SHELL_SYNTAX_C;

  ಪ್ರತ್ಯಾಗಮ SHELL_SYNTAX_BOURNE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_ಸಾಲು (ವರ್ಣ ಧ್ರುವ *ಸಾಲು, ವರ್ಣ **keyword, ವರ್ಣ **arg)
{
  ವರ್ಣ ಧ್ರುವ *p;
  ವರ್ಣ ಧ್ರುವ *keyword_start;
  ವರ್ಣ ಧ್ರುವ *arg_start;

  *keyword = ರಿಕ್ತ;
  *arg = ರಿಕ್ತ;

  ಪ್ರತಿ (p = ಸಾಲು; c_isspace (*p); ++p)
    ಅನುವರ್ತ;

  /* Ignore blank lines and shell-style comments.  */
  ಯದಿ (*p == '\0' || *p == '#')
    ಪ್ರತ್ಯಾಗಮ;

  keyword_start = p;

  ಯಾವತ್ (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  ಯದಿ (*p  == '\0')
    ಪ್ರತ್ಯಾಗಮ;

  ಕುರು
    {
      ++p;
    }
  ಯಾವತ್ (c_isspace (*p));

  ಯದಿ (*p == '\0' || *p == '#')
    ಪ್ರತ್ಯಾಗಮ;

  arg_start = p;

  ಯಾವತ್ (*p != '\0' && *p != '#')
    ++p;

  ಪ್ರತಿ (--p; c_isspace (*p); --p)
    ಅನುವರ್ತ;
  ++p;

  *arg = ximemdup0 (arg_start, p - arg_start);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

ಸ್ಥಿರ ಶೂನ್ಯ
append_quoted (ವರ್ಣ ಧ್ರುವ *str)
{
  bool need_backslash = true;

  ಯಾವತ್ (*str != '\0')
    {
      ಯದಿ (! print_ls_colors)
        ವಿಕಲ್ಪ (*str)
          {
          ಪಕ್ಷ '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            ಭಙ್ಗ;

          ಪಕ್ಷ '\\':
          ಪಕ್ಷ '^':
            need_backslash = !need_backslash;
            ಭಙ್ಗ;

          ಪಕ್ಷ ':':
          ಪಕ್ಷ '=':
            ಯದಿ (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          ಸ್ವಭಾವ:
            need_backslash = true;
            ಭಙ್ಗ;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

ಸ್ಥಿರ ಶೂನ್ಯ
append_entry (ವರ್ಣ prefix, ವರ್ಣ ಧ್ರುವ *item, ವರ್ಣ ಧ್ರುವ *arg)
{
  ಯದಿ (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  ಯದಿ (prefix)
    APPEND_CHAR (prefix);
  append_quoted (item);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  ಯದಿ (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

ಸ್ಥಿರ bool
dc_parse_stream (FILE *fp, ವರ್ಣ ಧ್ರುವ *filename)
{
  idx_t ಸಾಲು_ಸಂಖ್ಯೆ = 0;
  ವರ್ಣ ಧ್ರುವ *next_G_ಸಾಲು = G_ಸಾಲು;
  ವರ್ಣ *ಒಳಹರಿವು_ಸಾಲು = ರಿಕ್ತ;
  size_t ಒಳಹರಿವು_ಸಾಲು_size = 0;
  ವರ್ಣ ಧ್ರುವ *ಸಾಲು;
  ವರ್ಣ ಧ್ರುವ *term;
  ವರ್ಣ ಧ್ರುವ *colorterm;
  bool ok = true;

  /* State for the parser.  */
  ಗಣನಾ { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  term = getenv ("TERM");
  ಯದಿ (term == ರಿಕ್ತ || *term == '\0')
    term = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  ಯದಿ (colorterm == ರಿಕ್ತ)
    colorterm = "";  /* Doesn't match default "?*"  */

  ಯಾವತ್ (true)
    {
      ವರ್ಣ *keywd, *arg;
      bool unrecognized;

      ++ಸಾಲು_ಸಂಖ್ಯೆ;

      ಯದಿ (fp)
        {
          ಯದಿ (getline (&ಒಳಹರಿವು_ಸಾಲು, &ಒಳಹರಿವು_ಸಾಲು_size, fp) <= 0)
            {
              ಯದಿ (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              ಮುಕ್ತ (ಒಳಹರಿವು_ಸಾಲು);
              ಭಙ್ಗ;
            }
          ಸಾಲು = ಒಳಹರಿವು_ಸಾಲು;
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (next_G_ಸಾಲು == G_ಸಾಲು + ಪರಿಮಾಣ G_ಸಾಲು)
            ಭಙ್ಗ;
          ಸಾಲು = next_G_ಸಾಲು;
          next_G_ಸಾಲು += strlen (next_G_ಸಾಲು) + 1;
        }

      parse_ಸಾಲು (ಸಾಲು, &keywd, &arg);

      ಯದಿ (keywd == ರಿಕ್ತ)
        ಅನುವರ್ತ;

      ಯದಿ (arg == ರಿಕ್ತ)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), ಸಾಲು_ಸಂಖ್ಯೆ);
          ok = false;
          ಮುಕ್ತ (keywd);
          ಅನುವರ್ತ;
        }

      unrecognized = false;
      ಯದಿ (c_strcasecmp (keywd, "TERM") == 0)
        {
          ಯದಿ (state != ST_TERMSURE)
            state = fnmatch (arg, term, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      ಅನ್ಯಥಾ ಯದಿ (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          ಯದಿ (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      ಅನ್ಯಥಾ
        {
          ಯದಿ (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          ಯದಿ (state != ST_TERMNO)
            {
              ಯದಿ (keywd[0] == '.')
                append_entry ('*', keywd, arg);
              ಅನ್ಯಥಾ ಯದಿ (keywd[0] == '*')
                append_entry (0, keywd, arg);
              ಅನ್ಯಥಾ ಯದಿ (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              ಅನ್ಯಥಾ
                {
                  ಪೂರ್ಣಾಙ್ಕ i;

                  ಪ್ರತಿ (i = 0; slack_codes[i] != ರಿಕ್ತ; ++i)
                    ಯದಿ (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      ಭಙ್ಗ;

                  ಯದಿ (slack_codes[i] != ರಿಕ್ತ)
                    append_entry (0, ls_codes[i], arg);
                  ಅನ್ಯಥಾ
                    unrecognized = true;
                }
            }
          ಅನ್ಯಥಾ
            unrecognized = true;
        }

      ಯದಿ (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 ಸಾಲು_ಸಂಖ್ಯೆ, keywd);
          ok = false;
        }

      ಮುಕ್ತ (keywd);
      ಮುಕ್ತ (arg);
    }

  ಪ್ರತ್ಯಾಗಮ ok;
}

ಸ್ಥಿರ bool
dc_parse_file (ವರ್ಣ ಧ್ರುವ *filename)
{
  bool ok;

  ಯದಿ (! streq (filename, "-") && freopen (filename, "r", stdin) == ರಿಕ್ತ)
    {
      error (0, errno, "%s", quotef (filename));
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ok = dc_parse_stream (stdin, filename);

  ಯದಿ (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ಪ್ರತ್ಯಾಗಮ ok;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  bool ok = true;
  ಪೂರ್ಣಾಙ್ಕ optc;
  ಗಣನಾ Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, "bcp", long_options, ರಿಕ್ತ)) != -1)
    ವಿಕಲ್ಪ (optc)
      {
      ಪಕ್ಷ 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'p':
        print_database = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        ಭಙ್ಗ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      ಸ್ವಭಾವ:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  ಯದಿ ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      ಯದಿ (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (print_database)
    {
      ವರ್ಣ ಧ್ರುವ *p = G_ಸಾಲು;
      ಯಾವತ್ (p - G_ಸಾಲು < ಪರಿಮಾಣ G_ಸಾಲು)
        {
          puts (p);
          p += strlen (p) + 1;
        }
    }
  ಅನ್ಯಥಾ
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      ಯದಿ (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          ಯದಿ (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      ಯದಿ (argc == 0)
        ok = dc_parse_stream (ರಿಕ್ತ, ರಿಕ್ತ);
      ಅನ್ಯಥಾ
        ok = dc_parse_file (argv[0]);

      ಯದಿ (ok)
        {
          size_t len = obstack_object_size (&lsc_obstack);
          ವರ್ಣ *s = obstack_finish (&lsc_obstack);
          ವರ್ಣ ಧ್ರುವ *prefix;
          ವರ್ಣ ಧ್ರುವ *suffix;

          ಯದಿ (syntax == SHELL_SYNTAX_BOURNE)
            {
              prefix = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          ಅನ್ಯಥಾ
            {
              prefix = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          ಯದಿ (! print_ls_colors)
            fputs (prefix, stdout);
          fwrite (s, 1, len, stdout);
          ಯದಿ (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
