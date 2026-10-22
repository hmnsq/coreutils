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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <fnmatch.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "dircolors.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"
#ଅନ୍ତର୍ଭାଵ "c-strcase.h"
#ଅନ୍ତର୍ଭାଵ "obstack.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dircolors"

#define AUTHORS proper_name ("H. Peter Anvin")

#define obstack_chunk_alloc malloc
#define obstack_chunk_free ମୁକ୍ତ

ଗଣନା Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#define APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
ସ୍ଥିର ସଂରଚନା obstack lsc_obstack;

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", ଖାଲି
};

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *ଅଚଳ ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  ଖାଲି
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
ସ୍ଥିର bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
  {
    {"bourne-shell", no_argument, ଖାଲି, 'b'},
    {"sh", no_argument, ଖାଲି, 'b'},
    {"csh", no_argument, ଖାଲି, 'c'},
    {"c-shell", no_argument, ଖାଲି, 'c'},
    {"print-database", no_argument, ଖାଲି, 'p'},
    {"print-ls-colors", no_argument, ଖାଲି, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {ଖାଲି, 0, ଖାଲି, 0}
  };

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
         output fully escaped colors ପାଇଁ display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, ପଠ it to determine which colors to use ପାଇଁ which\n\
file types and extensions.  Otherwise, a precompiled database is used.\n\
For details on the format of these files, run 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

ସ୍ଥିର ଗଣନା Shell_syntax
guess_shell_syntax (ଶୂନ୍ୟ)
{
  ଅକ୍ଷର *shell;

  shell = getenv ("SHELL");
  ଯଦି (shell == ଖାଲି || *shell == '\0')
    ଫେରନ୍ତୁ SHELL_SYNTAX_UNKNOWN;

  shell = last_component (shell);

  ଯଦି (streq (shell, "csh") || streq (shell, "tcsh"))
    ଫେରନ୍ତୁ SHELL_SYNTAX_C;

  ଫେରନ୍ତୁ SHELL_SYNTAX_BOURNE;
}

ସ୍ଥିର ଶୂନ୍ୟ
parse_ଧାଡ଼ି (ଅକ୍ଷର ଅଚଳ *ଧାଡ଼ି, ଅକ୍ଷର **keyword, ଅକ୍ଷର **arg)
{
  ଅକ୍ଷର ଅଚଳ *p;
  ଅକ୍ଷର ଅଚଳ *keyword_start;
  ଅକ୍ଷର ଅଚଳ *arg_start;

  *keyword = ଖାଲି;
  *arg = ଖାଲି;

  ପାଇଁ (p = ଧାଡ଼ି; c_isspace (*p); ++p)
    ଜାରି;

  /* Ignore blank lines and shell-style comments.  */
  ଯଦି (*p == '\0' || *p == '#')
    ଫେରନ୍ତୁ;

  keyword_start = p;

  ଯେତେବେଳେ (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  ଯଦି (*p  == '\0')
    ଫେରନ୍ତୁ;

  କର
    {
      ++p;
    }
  ଯେତେବେଳେ (c_isspace (*p));

  ଯଦି (*p == '\0' || *p == '#')
    ଫେରନ୍ତୁ;

  arg_start = p;

  ଯେତେବେଳେ (*p != '\0' && *p != '#')
    ++p;

  ପାଇଁ (--p; c_isspace (*p); --p)
    ଜାରି;
  ++p;

  *arg = ximemdup0 (arg_start, p - arg_start);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

ସ୍ଥିର ଶୂନ୍ୟ
append_quoted (ଅକ୍ଷର ଅଚଳ *str)
{
  bool need_backslash = true;

  ଯେତେବେଳେ (*str != '\0')
    {
      ଯଦି (! print_ls_colors)
        ବାଛ (*str)
          {
          ସ୍ଥିତି '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            ଭାଙ୍ଗ;

          ସ୍ଥିତି '\\':
          ସ୍ଥିତି '^':
            need_backslash = !need_backslash;
            ଭାଙ୍ଗ;

          ସ୍ଥିତି ':':
          ସ୍ଥିତି '=':
            ଯଦି (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            need_backslash = true;
            ଭାଙ୍ଗ;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

ସ୍ଥିର ଶୂନ୍ୟ
append_entry (ଅକ୍ଷର prefix, ଅକ୍ଷର ଅଚଳ *item, ଅକ୍ଷର ଅଚଳ *arg)
{
  ଯଦି (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  ଯଦି (prefix)
    APPEND_CHAR (prefix);
  append_quoted (item);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  ଯଦି (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

ସ୍ଥିର bool
dc_parse_stream (FILE *fp, ଅକ୍ଷର ଅଚଳ *filename)
{
  idx_t ଧାଡ଼ି_ସଂଖ୍ୟା = 0;
  ଅକ୍ଷର ଅଚଳ *next_G_ଧାଡ଼ି = G_ଧାଡ଼ି;
  ଅକ୍ଷର *ନିବେଶ_ଧାଡ଼ି = ଖାଲି;
  size_t ନିବେଶ_ଧାଡ଼ି_size = 0;
  ଅକ୍ଷର ଅଚଳ *ଧାଡ଼ି;
  ଅକ୍ଷର ଅଚଳ *term;
  ଅକ୍ଷର ଅଚଳ *colorterm;
  bool ok = true;

  /* State for the parser.  */
  ଗଣନା { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  term = getenv ("TERM");
  ଯଦି (term == ଖାଲି || *term == '\0')
    term = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  ଯଦି (colorterm == ଖାଲି)
    colorterm = "";  /* Doesn't match default "?*"  */

  ଯେତେବେଳେ (true)
    {
      ଅକ୍ଷର *keywd, *arg;
      bool unrecognized;

      ++ଧାଡ଼ି_ସଂଖ୍ୟା;

      ଯଦି (fp)
        {
          ଯଦି (getline (&ନିବେଶ_ଧାଡ଼ି, &ନିବେଶ_ଧାଡ଼ି_size, fp) <= 0)
            {
              ଯଦି (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              ମୁକ୍ତ (ନିବେଶ_ଧାଡ଼ି);
              ଭାଙ୍ଗ;
            }
          ଧାଡ଼ି = ନିବେଶ_ଧାଡ଼ି;
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (next_G_ଧାଡ଼ି == G_ଧାଡ଼ି + ଆକାର G_ଧାଡ଼ି)
            ଭାଙ୍ଗ;
          ଧାଡ଼ି = next_G_ଧାଡ଼ି;
          next_G_ଧାଡ଼ି += strlen (next_G_ଧାଡ଼ି) + 1;
        }

      parse_ଧାଡ଼ି (ଧାଡ଼ି, &keywd, &arg);

      ଯଦି (keywd == ଖାଲି)
        ଜାରି;

      ଯଦି (arg == ଖାଲି)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), ଧାଡ଼ି_ସଂଖ୍ୟା);
          ok = false;
          ମୁକ୍ତ (keywd);
          ଜାରି;
        }

      unrecognized = false;
      ଯଦି (c_strcasecmp (keywd, "TERM") == 0)
        {
          ଯଦି (state != ST_TERMSURE)
            state = fnmatch (arg, term, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      ଅନ୍ୟଥା ଯଦି (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          ଯଦି (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      ଅନ୍ୟଥା
        {
          ଯଦି (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          ଯଦି (state != ST_TERMNO)
            {
              ଯଦି (keywd[0] == '.')
                append_entry ('*', keywd, arg);
              ଅନ୍ୟଥା ଯଦି (keywd[0] == '*')
                append_entry (0, keywd, arg);
              ଅନ୍ୟଥା ଯଦି (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              ଅନ୍ୟଥା
                {
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

                  ପାଇଁ (i = 0; slack_codes[i] != ଖାଲି; ++i)
                    ଯଦି (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      ଭାଙ୍ଗ;

                  ଯଦି (slack_codes[i] != ଖାଲି)
                    append_entry (0, ls_codes[i], arg);
                  ଅନ୍ୟଥା
                    unrecognized = true;
                }
            }
          ଅନ୍ୟଥା
            unrecognized = true;
        }

      ଯଦି (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 ଧାଡ଼ି_ସଂଖ୍ୟା, keywd);
          ok = false;
        }

      ମୁକ୍ତ (keywd);
      ମୁକ୍ତ (arg);
    }

  ଫେରନ୍ତୁ ok;
}

ସ୍ଥିର bool
dc_parse_file (ଅକ୍ଷର ଅଚଳ *filename)
{
  bool ok;

  ଯଦି (! streq (filename, "-") && freopen (filename, "r", stdin) == ଖାଲି)
    {
      error (0, errno, "%s", quotef (filename));
      ଫେରନ୍ତୁ false;
    }

  ok = dc_parse_stream (stdin, filename);

  ଯଦି (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      ଫେରନ୍ତୁ false;
    }

  ଫେରନ୍ତୁ ok;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  bool ok = true;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ଗଣନା Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "bcp", long_options, ଖାଲି)) != -1)
    ବାଛ (optc)
      {
      ସ୍ଥିତି 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'p':
        print_database = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        ଭାଙ୍ଗ;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  ଯଦି ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  ଯଦି ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      ଯଦି (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (print_database)
    {
      ଅକ୍ଷର ଅଚଳ *p = G_ଧାଡ଼ି;
      ଯେତେବେଳେ (p - G_ଧାଡ଼ି < ଆକାର G_ଧାଡ଼ି)
        {
          puts (p);
          p += strlen (p) + 1;
        }
    }
  ଅନ୍ୟଥା
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      ଯଦି (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          ଯଦି (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      ଯଦି (argc == 0)
        ok = dc_parse_stream (ଖାଲି, ଖାଲି);
      ଅନ୍ୟଥା
        ok = dc_parse_file (argv[0]);

      ଯଦି (ok)
        {
          size_t len = obstack_object_size (&lsc_obstack);
          ଅକ୍ଷର *s = obstack_finish (&lsc_obstack);
          ଅକ୍ଷର ଅଚଳ *prefix;
          ଅକ୍ଷର ଅଚଳ *suffix;

          ଯଦି (syntax == SHELL_SYNTAX_BOURNE)
            {
              prefix = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          ଅନ୍ୟଥା
            {
              prefix = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          ଯଦି (! print_ls_colors)
            fputs (prefix, stdout);
          fwrite (s, 1, len, stdout);
          ଯଦି (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
