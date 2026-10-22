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

#include <config.h>

#include <sys/types.h>
#include <fnmatch.h>
#include <getopt.h>

#include "system.h"
#include "dircolors.h"
#include "c-ctype.h"
#include "c-strcase.h"
#include "obstack.h"
#include "quote.h"
#include "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dircolors"

#define AUTHORS proper_name ("H. Peter Anvin")

#define obstack_chunk_alloc malloc
#define obstack_chunk_free free

ga_nanaa Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#define APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
static sa_nracanaa obstack lsc_obstack;

static var_na const *const slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", NULL
};

static var_na const *const ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  NULL
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
static bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ga_nanaa
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

static sa_nracanaa option const long_options[] =
  {
    {"bourne-shell", no_argument, NULL, 'b'},
    {"sh", no_argument, NULL, 'b'},
    {"csh", no_argument, NULL, 'c'},
    {"c-shell", no_argument, NULL, 'c'},
    {"print-database", no_argument, NULL, 'p'},
    {"print-ls-colors", no_argument, NULL, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {NULL, 0, NULL, 0}
  };

void
usage (puur_naa_nka status)
{
  ᰀᰧ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᰀᰧᰀ
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
         output fully escaped colors for display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, read it to determine which colors to use for which\n\
file types and extensions.  Otherwise, a precompiled database is used.\n\
For details on the format of these files, run 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

static ga_nanaa Shell_syntax
guess_shell_syntax (void)
{
  var_na *shell;

  shell = getenv ("SHELL");
  ᰀᰧ (shell == NULL || *shell == '\0')
    ᰀᰧᰌ SHELL_SYNTAX_UNKNOWN;

  shell = last_component (shell);

  ᰀᰧ (streq (shell, "csh") || streq (shell, "tcsh"))
    ᰀᰧᰌ SHELL_SYNTAX_C;

  ᰀᰧᰌ SHELL_SYNTAX_BOURNE;
}

static void
parse_line (var_na const *line, var_na **keyword, var_na **arg)
{
  var_na const *p;
  var_na const *keyword_start;
  var_na const *arg_start;

  *keyword = NULL;
  *arg = NULL;

  for (p = line; c_isspace (*p); ++p)
    continue;

  /* Ignore blank lines and shell-style comments.  */
  ᰀᰧ (*p == '\0' || *p == '#')
    ᰀᰧᰌ;

  keyword_start = p;

  ᰀᰬ (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  ᰀᰧ (*p  == '\0')
    ᰀᰧᰌ;

  do
    {
      ++p;
    }
  ᰀᰬ (c_isspace (*p));

  ᰀᰧ (*p == '\0' || *p == '#')
    ᰀᰧᰌ;

  arg_start = p;

  ᰀᰬ (*p != '\0' && *p != '#')
    ++p;

  for (--p; c_isspace (*p); --p)
    continue;
  ++p;

  *arg = ximemdup0 (arg_start, p - arg_start);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

static void
append_quoted (var_na const *str)
{
  bool need_backslash = true;

  ᰀᰬ (*str != '\0')
    {
      ᰀᰧ (! print_ls_colors)
        switch (*str)
          {
          pak_sa '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            bha_nga;

          pak_sa '\\':
          pak_sa '^':
            need_backslash = !need_backslash;
            bha_nga;

          pak_sa ':':
          pak_sa '=':
            ᰀᰧ (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          default:
            need_backslash = true;
            bha_nga;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

static void
append_entry (var_na prefix, var_na const *item, var_na const *arg)
{
  ᰀᰧ (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  ᰀᰧ (prefix)
    APPEND_CHAR (prefix);
  append_quoted (item);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  ᰀᰧ (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

static bool
dc_parse_stream (FILE *fp, var_na const *filename)
{
  idx_t line_number = 0;
  var_na const *next_G_line = G_line;
  var_na *input_line = NULL;
  size_t input_line_size = 0;
  var_na const *line;
  var_na const *term;
  var_na const *colorterm;
  bool ok = true;

  /* State for the parser.  */
  ga_nanaa { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  term = getenv ("TERM");
  ᰀᰧ (term == NULL || *term == '\0')
    term = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  ᰀᰧ (colorterm == NULL)
    colorterm = "";  /* Doesn't match default "?*"  */

  ᰀᰬ (true)
    {
      var_na *keywd, *arg;
      bool unrecognized;

      ++line_number;

      ᰀᰧ (fp)
        {
          ᰀᰧ (getline (&input_line, &input_line_size, fp) <= 0)
            {
              ᰀᰧ (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              free (input_line);
              bha_nga;
            }
          line = input_line;
        }
      ᰀᰧᰀ
        {
          ᰀᰧ (next_G_line == G_line + parimaa_na G_line)
            bha_nga;
          line = next_G_line;
          next_G_line += strlen (next_G_line) + 1;
        }

      parse_line (line, &keywd, &arg);

      ᰀᰧ (keywd == NULL)
        continue;

      ᰀᰧ (arg == NULL)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), line_number);
          ok = false;
          free (keywd);
          continue;
        }

      unrecognized = false;
      ᰀᰧ (c_strcasecmp (keywd, "TERM") == 0)
        {
          ᰀᰧ (state != ST_TERMSURE)
            state = fnmatch (arg, term, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      ᰀᰧᰀ ᰀᰧ (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          ᰀᰧ (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      ᰀᰧᰀ
        {
          ᰀᰧ (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          ᰀᰧ (state != ST_TERMNO)
            {
              ᰀᰧ (keywd[0] == '.')
                append_entry ('*', keywd, arg);
              ᰀᰧᰀ ᰀᰧ (keywd[0] == '*')
                append_entry (0, keywd, arg);
              ᰀᰧᰀ ᰀᰧ (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              ᰀᰧᰀ
                {
                  puur_naa_nka i;

                  for (i = 0; slack_codes[i] != NULL; ++i)
                    ᰀᰧ (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      bha_nga;

                  ᰀᰧ (slack_codes[i] != NULL)
                    append_entry (0, ls_codes[i], arg);
                  ᰀᰧᰀ
                    unrecognized = true;
                }
            }
          ᰀᰧᰀ
            unrecognized = true;
        }

      ᰀᰧ (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 line_number, keywd);
          ok = false;
        }

      free (keywd);
      free (arg);
    }

  ᰀᰧᰌ ok;
}

static bool
dc_parse_file (var_na const *filename)
{
  bool ok;

  ᰀᰧ (! streq (filename, "-") && freopen (filename, "r", stdin) == NULL)
    {
      error (0, errno, "%s", quotef (filename));
      ᰀᰧᰌ false;
    }

  ok = dc_parse_stream (stdin, filename);

  ᰀᰧ (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      ᰀᰧᰌ false;
    }

  ᰀᰧᰌ ok;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  bool ok = true;
  puur_naa_nka optc;
  ga_nanaa Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ᰀᰬ ((optc = getopt_long (argc, argv, "bcp", long_options, NULL)) != -1)
    switch (optc)
      {
      pak_sa 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        bha_nga;

      pak_sa 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        bha_nga;

      pak_sa 'p':
        print_database = true;
        bha_nga;

      pak_sa PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        bha_nga;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      default:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  ᰀᰧ ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ᰀᰧ (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ᰀᰧ ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      ᰀᰧ (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  ᰀᰧ (print_database)
    {
      var_na const *p = G_line;
      ᰀᰬ (p - G_line < parimaa_na G_line)
        {
          puts (p);
          p += strlen (p) + 1;
        }
    }
  ᰀᰧᰀ
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      ᰀᰧ (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          ᰀᰧ (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      ᰀᰧ (argc == 0)
        ok = dc_parse_stream (NULL, NULL);
      ᰀᰧᰀ
        ok = dc_parse_file (argv[0]);

      ᰀᰧ (ok)
        {
          size_t len = obstack_object_size (&lsc_obstack);
          var_na *s = obstack_finish (&lsc_obstack);
          var_na const *prefix;
          var_na const *suffix;

          ᰀᰧ (syntax == SHELL_SYNTAX_BOURNE)
            {
              prefix = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          ᰀᰧᰀ
            {
              prefix = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          ᰀᰧ (! print_ls_colors)
            fputs (prefix, stdout);
          fwrite (s, 1, len, stdout);
          ᰀᰧ (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  ᰀᰧᰌ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
