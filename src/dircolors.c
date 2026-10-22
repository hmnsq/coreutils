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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "dircolors"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("H. Peter Anvin")

#ವ್ಯಾಖ್ಯಾನಿಸು obstack_chunk_alloc ಸ್ಮರಣೆ_ಕೊಡು
#ವ್ಯಾಖ್ಯಾನಿಸು obstack_chunk_free ಬಿಡುಗಡೆ

ಎಣಿಕೆ Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#ವ್ಯಾಖ್ಯಾನಿಸು APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
ಸ್ಥಿರ ರಚನೆ obstack lsc_obstack;

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", ಖಾಲಿ
};

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸ್ಥಿರಾಂಕ ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  ಖಾಲಿ
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
ಸ್ಥಿರ bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
  {
    {"bourne-shell", no_ವಾದ, ಖಾಲಿ, 'b'},
    {"sh", no_ವಾದ, ಖಾಲಿ, 'b'},
    {"csh", no_ವಾದ, ಖಾಲಿ, 'c'},
    {"c-shell", no_ವಾದ, ಖಾಲಿ, 'c'},
    {"print-database", no_ವಾದ, ಖಾಲಿ, 'p'},
    {"print-ls-colors", no_ವಾದ, ಖಾಲಿ, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
    {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
  };

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... [FILE]\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Output commands ಗಮ್ಯ ಗಣ_2 the LS_COLORS environment variable.\n\
\n\
Determine format ದ output:\n\
"), stdout);
      oputs (_("\
  -b, --sh, --bourne-shell\n\
         output Bourne shell ಸಂಕೇತ_2 ಗಮ್ಯ ಗಣ_2 LS_COLORS\n\
"));
      oputs (_("\
  -c, --csh, --c-shell\n\
         output C shell ಸಂಕೇತ_2 ಗಮ್ಯ ಗಣ_2 LS_COLORS\n\
"));
      oputs (_("\
  -p, --print-database\n\
         output defaults\n\
"));
      oputs (_("\
      --print-ls-colors\n\
         output fully escaped colors ಗಾಗಿ display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, ಪಠ it ಗಮ್ಯ determine which colors ಗಮ್ಯ use ಗಾಗಿ which\n\
file types and extensions.  Otherwise, a precompiled database is ಬಳಸಿದ.\n\
For details on the format ದ these files, ಸತತ_2 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }

  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

ಸ್ಥಿರ ಎಣಿಕೆ Shell_syntax
guess_shell_syntax (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ *shell;

  shell = getenv ("SHELL");
  ಆದರೆ (shell == ಖಾಲಿ || *shell == '\0')
    ಹಿಂತಿರುಗಿ SHELL_SYNTAX_UNKNOWN;

  shell = ಕೊನೆಯ_component (shell);

  ಆದರೆ (streq (shell, "csh") || streq (shell, "tcsh"))
    ಹಿಂತಿರುಗಿ SHELL_SYNTAX_C;

  ಹಿಂತಿರುಗಿ SHELL_SYNTAX_BOURNE;
}

ಸ್ಥಿರ ಶೂನ್ಯ
parse_ಸಾಲು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಾಲು, ಅಕ್ಷರ **keyword, ಅಕ್ಷರ **arg)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *keyword_ಆರಂಭ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg_ಆರಂಭ;

  *keyword = ಖಾಲಿ;
  *arg = ಖಾಲಿ;

  ಗಾಗಿ (p = ಸಾಲು; c_isspace (*p); ++p)
    ಮುಂದುವರಿ;

  /* Ignore blank lines and shell-style comments.  */
  ಆದರೆ (*p == '\0' || *p == '#')
    ಹಿಂತಿರುಗಿ;

  keyword_ಆರಂಭ = p;

  ಆಗ (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_ಆರಂಭ, p - keyword_ಆರಂಭ);
  ಆದರೆ (*p  == '\0')
    ಹಿಂತಿರುಗಿ;

  ಮಾಡು
    {
      ++p;
    }
  ಆಗ (c_isspace (*p));

  ಆದರೆ (*p == '\0' || *p == '#')
    ಹಿಂತಿರುಗಿ;

  arg_ಆರಂಭ = p;

  ಆಗ (*p != '\0' && *p != '#')
    ++p;

  ಗಾಗಿ (--p; c_isspace (*p); --p)
    ಮುಂದುವರಿ;
  ++p;

  *arg = ximemdup0 (arg_ಆರಂಭ, p - arg_ಆರಂಭ);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

ಸ್ಥಿರ ಶೂನ್ಯ
append_quoted (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str)
{
  bool need_backslash = true;

  ಆಗ (*str != '\0')
    {
      ಆದರೆ (! print_ls_colors)
        ಆಯ್ಕೆ (*str)
          {
          ಸ್ಥಿತಿ '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            ನಿಲ್ಲಿಸು;

          ಸ್ಥಿತಿ '\\':
          ಸ್ಥಿತಿ '^':
            need_backslash = !need_backslash;
            ನಿಲ್ಲಿಸು;

          ಸ್ಥಿತಿ ':':
          ಸ್ಥಿತಿ '=':
            ಆದರೆ (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          ಪೂರ್ವನಿಯೋಜಿತ:
            need_backslash = true;
            ನಿಲ್ಲಿಸು;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

ಸ್ಥಿರ ಶೂನ್ಯ
append_ನಮೂದು (ಅಕ್ಷರ ಪೂರ್ವಪ್ರತ್ಯಯ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಅಂಶ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *arg)
{
  ಆದರೆ (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  ಆದರೆ (ಪೂರ್ವಪ್ರತ್ಯಯ)
    APPEND_CHAR (ಪೂರ್ವಪ್ರತ್ಯಯ);
  append_quoted (ಅಂಶ);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  ಆದರೆ (print_ls_colors)
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
dc_parse_ಹರಿವು (FILE *fp, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
  idx_t ಸಾಲು_ಸಂಖ್ಯೆ = 0;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮುಂದಿನ_G_ಸಾಲು = G_ಸಾಲು;
  ಅಕ್ಷರ *ಒಳಹರಿವು_ಸಾಲು = ಖಾಲಿ;
  size_t ಒಳಹರಿವು_ಸಾಲು_ಗಾತ್ರ = 0;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಸಾಲು;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಪದ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *colorterm;
  bool ok = true;

  /* State for the parser.  */
  ಎಣಿಕೆ { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  ಪದ = getenv ("TERM");
  ಆದರೆ (ಪದ == ಖಾಲಿ || *ಪದ == '\0')
    ಪದ = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  ಆದರೆ (colorterm == ಖಾಲಿ)
    colorterm = "";  /* Doesn't match default "?*"  */

  ಆಗ (true)
    {
      ಅಕ್ಷರ *keywd, *arg;
      bool unrecognized;

      ++ಸಾಲು_ಸಂಖ್ಯೆ;

      ಆದರೆ (fp)
        {
          ಆದರೆ (getline (&ಒಳಹರಿವು_ಸಾಲು, &ಒಳಹರಿವು_ಸಾಲು_ಗಾತ್ರ, fp) <= 0)
            {
              ಆದರೆ (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              ಬಿಡುಗಡೆ (ಒಳಹರಿವು_ಸಾಲು);
              ನಿಲ್ಲಿಸು;
            }
          ಸಾಲು = ಒಳಹರಿವು_ಸಾಲು;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (ಮುಂದಿನ_G_ಸಾಲು == G_ಸಾಲು + ಗಾತ್ರ G_ಸಾಲು)
            ನಿಲ್ಲಿಸು;
          ಸಾಲು = ಮುಂದಿನ_G_ಸಾಲು;
          ಮುಂದಿನ_G_ಸಾಲು += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಮುಂದಿನ_G_ಸಾಲು) + 1;
        }

      parse_ಸಾಲು (ಸಾಲು, &keywd, &arg);

      ಆದರೆ (keywd == ಖಾಲಿ)
        ಮುಂದುವರಿ;

      ಆದರೆ (arg == ಖಾಲಿ)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), ಸಾಲು_ಸಂಖ್ಯೆ);
          ok = false;
          ಬಿಡುಗಡೆ (keywd);
          ಮುಂದುವರಿ;
        }

      unrecognized = false;
      ಆದರೆ (c_strcasecmp (keywd, "TERM") == 0)
        {
          ಆದರೆ (state != ST_TERMSURE)
            state = fnmatch (arg, ಪದ, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          ಆದರೆ (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          ಆದರೆ (state != ST_TERMNO)
            {
              ಆದರೆ (keywd[0] == '.')
                append_ನಮೂದು ('*', keywd, arg);
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (keywd[0] == '*')
                append_ನಮೂದು (0, keywd, arg);
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಪೂರ್ಣಾಂಕ i;

                  ಗಾಗಿ (i = 0; slack_codes[i] != ಖಾಲಿ; ++i)
                    ಆದರೆ (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      ನಿಲ್ಲಿಸು;

                  ಆದರೆ (slack_codes[i] != ಖಾಲಿ)
                    append_ನಮೂದು (0, ls_codes[i], arg);
                  ಇಲ್ಲದಿದ್ದರೆ
                    unrecognized = true;
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ
            unrecognized = true;
        }

      ಆದರೆ (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 ಸಾಲು_ಸಂಖ್ಯೆ, keywd);
          ok = false;
        }

      ಬಿಡುಗಡೆ (keywd);
      ಬಿಡುಗಡೆ (arg);
    }

  ಹಿಂತಿರುಗಿ ok;
}

ಸ್ಥಿರ bool
dc_parse_file (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename)
{
  bool ok;

  ಆದರೆ (! streq (filename, "-") && freopen (filename, "r", stdin) == ಖಾಲಿ)
    {
      error (0, errno, "%s", quotef (filename));
      ಹಿಂತಿರುಗಿ false;
    }

  ok = dc_parse_ಹರಿವು (stdin, filename);

  ಆದರೆ (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      ಹಿಂತಿರುಗಿ false;
    }

  ಹಿಂತಿರುಗಿ ok;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool ok = true;
  ಪೂರ್ಣಾಂಕ optc;
  ಎಣಿಕೆ Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((optc = getopt_long (argc, argv, "bcp", long_options, ಖಾಲಿ)) != -1)
    ಆಯ್ಕೆ (optc)
      {
      ಸ್ಥಿತಿ 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'p':
        print_database = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        ನಿಲ್ಲಿಸು;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

      ಪೂರ್ವನಿಯೋಜಿತ:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  ಆದರೆ ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      ಆದರೆ (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (print_database)
    {
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *p = G_ಸಾಲು;
      ಆಗ (p - G_ಸಾಲು < ಗಾತ್ರ G_ಸಾಲು)
        {
          puts (p);
          p += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (p) + 1;
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      ಆದರೆ (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          ಆದರೆ (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      ಆದರೆ (argc == 0)
        ok = dc_parse_ಹರಿವು (ಖಾಲಿ, ಖಾಲಿ);
      ಇಲ್ಲದಿದ್ದರೆ
        ok = dc_parse_file (argv[0]);

      ಆದರೆ (ok)
        {
          size_t len = obstack_object_ಗಾತ್ರ (&lsc_obstack);
          ಅಕ್ಷರ *s = obstack_ಮುಕ್ತಾಯ (&lsc_obstack);
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಪೂರ್ವಪ್ರತ್ಯಯ;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *suffix;

          ಆದರೆ (syntax == SHELL_SYNTAX_BOURNE)
            {
              ಪೂರ್ವಪ್ರತ್ಯಯ = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಪೂರ್ವಪ್ರತ್ಯಯ = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          ಆದರೆ (! print_ls_colors)
            fputs (ಪೂರ್ವಪ್ರತ್ಯಯ, stdout);
          fwrite (s, 1, len, stdout);
          ಆದರೆ (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
