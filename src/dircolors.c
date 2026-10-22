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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <fnmatch.h>
#அந்தர்பாவ <getopt.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "dircolors.h"
#அந்தர்பாவ "c-ctype.h"
#அந்தர்பாவ "c-strcase.h"
#அந்தர்பாவ "obstack.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dircolors"

#define AUTHORS proper_name ("H. Peter Anvin")

#define obstack_chunk_alloc malloc
#define obstack_chunk_free விடுவி

எண்ணிக்கை Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#define APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
நிலையான கட்டமைப்பு obstack lsc_obstack;

நிலையான எழுத்து மாறிலி *மாறிலி slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", வெற்று
};

நிலையான எழுத்து மாறிலி *மாறிலி ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  வெற்று
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
நிலையான bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
  {
    {"bourne-shell", no_argument, வெற்று, 'b'},
    {"sh", no_argument, வெற்று, 'b'},
    {"csh", no_argument, வெற்று, 'c'},
    {"c-shell", no_argument, வெற்று, 'c'},
    {"print-database", no_argument, வெற்று, 'p'},
    {"print-ls-colors", no_argument, வெற்று, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {வெற்று, 0, வெற்று, 0}
  };

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
         output fully escaped colors க்கு display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, பட it to determine which colors to use க்கு which\n\
file types and extensions.  Otherwise, a precompiled database is used.\n\
For details on the format of these files, run 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

நிலையான எண்ணிக்கை Shell_syntax
guess_shell_syntax (வெறுமை)
{
  எழுத்து *shell;

  shell = getenv ("SHELL");
  எனில் (shell == வெற்று || *shell == '\0')
    திரும்பு SHELL_SYNTAX_UNKNOWN;

  shell = last_component (shell);

  எனில் (streq (shell, "csh") || streq (shell, "tcsh"))
    திரும்பு SHELL_SYNTAX_C;

  திரும்பு SHELL_SYNTAX_BOURNE;
}

நிலையான வெறுமை
parse_வரி (எழுத்து மாறிலி *வரி, எழுத்து **keyword, எழுத்து **arg)
{
  எழுத்து மாறிலி *p;
  எழுத்து மாறிலி *keyword_start;
  எழுத்து மாறிலி *arg_start;

  *keyword = வெற்று;
  *arg = வெற்று;

  க்கு (p = வரி; c_isspace (*p); ++p)
    தொடர்;

  /* Ignore blank lines and shell-style comments.  */
  எனில் (*p == '\0' || *p == '#')
    திரும்பு;

  keyword_start = p;

  போது (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  எனில் (*p  == '\0')
    திரும்பு;

  செய்
    {
      ++p;
    }
  போது (c_isspace (*p));

  எனில் (*p == '\0' || *p == '#')
    திரும்பு;

  arg_start = p;

  போது (*p != '\0' && *p != '#')
    ++p;

  க்கு (--p; c_isspace (*p); --p)
    தொடர்;
  ++p;

  *arg = ximemdup0 (arg_start, p - arg_start);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

நிலையான வெறுமை
append_quoted (எழுத்து மாறிலி *str)
{
  bool need_backslash = true;

  போது (*str != '\0')
    {
      எனில் (! print_ls_colors)
        தேர்வு (*str)
          {
          நிலை '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            நிறுத்து;

          நிலை '\\':
          நிலை '^':
            need_backslash = !need_backslash;
            நிறுத்து;

          நிலை ':':
          நிலை '=':
            எனில் (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          இயல்பு:
            need_backslash = true;
            நிறுத்து;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

நிலையான வெறுமை
append_entry (எழுத்து prefix, எழுத்து மாறிலி *item, எழுத்து மாறிலி *arg)
{
  எனில் (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  எனில் (prefix)
    APPEND_CHAR (prefix);
  append_quoted (item);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  எனில் (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

நிலையான bool
dc_parse_stream (FILE *fp, எழுத்து மாறிலி *filename)
{
  idx_t வரி_எண் = 0;
  எழுத்து மாறிலி *next_G_வரி = G_வரி;
  எழுத்து *உள்ளீடு_வரி = வெற்று;
  size_t உள்ளீடு_வரி_size = 0;
  எழுத்து மாறிலி *வரி;
  எழுத்து மாறிலி *term;
  எழுத்து மாறிலி *colorterm;
  bool ok = true;

  /* State for the parser.  */
  எண்ணிக்கை { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  term = getenv ("TERM");
  எனில் (term == வெற்று || *term == '\0')
    term = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  எனில் (colorterm == வெற்று)
    colorterm = "";  /* Doesn't match default "?*"  */

  போது (true)
    {
      எழுத்து *keywd, *arg;
      bool unrecognized;

      ++வரி_எண்;

      எனில் (fp)
        {
          எனில் (getline (&உள்ளீடு_வரி, &உள்ளீடு_வரி_size, fp) <= 0)
            {
              எனில் (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              விடுவி (உள்ளீடு_வரி);
              நிறுத்து;
            }
          வரி = உள்ளீடு_வரி;
        }
      இல்லையெனில்
        {
          எனில் (next_G_வரி == G_வரி + அளவு G_வரி)
            நிறுத்து;
          வரி = next_G_வரி;
          next_G_வரி += strlen (next_G_வரி) + 1;
        }

      parse_வரி (வரி, &keywd, &arg);

      எனில் (keywd == வெற்று)
        தொடர்;

      எனில் (arg == வெற்று)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), வரி_எண்);
          ok = false;
          விடுவி (keywd);
          தொடர்;
        }

      unrecognized = false;
      எனில் (c_strcasecmp (keywd, "TERM") == 0)
        {
          எனில் (state != ST_TERMSURE)
            state = fnmatch (arg, term, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      இல்லையெனில் எனில் (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          எனில் (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      இல்லையெனில்
        {
          எனில் (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          எனில் (state != ST_TERMNO)
            {
              எனில் (keywd[0] == '.')
                append_entry ('*', keywd, arg);
              இல்லையெனில் எனில் (keywd[0] == '*')
                append_entry (0, keywd, arg);
              இல்லையெனில் எனில் (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              இல்லையெனில்
                {
                  முழு i;

                  க்கு (i = 0; slack_codes[i] != வெற்று; ++i)
                    எனில் (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      நிறுத்து;

                  எனில் (slack_codes[i] != வெற்று)
                    append_entry (0, ls_codes[i], arg);
                  இல்லையெனில்
                    unrecognized = true;
                }
            }
          இல்லையெனில்
            unrecognized = true;
        }

      எனில் (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 வரி_எண், keywd);
          ok = false;
        }

      விடுவி (keywd);
      விடுவி (arg);
    }

  திரும்பு ok;
}

நிலையான bool
dc_parse_file (எழுத்து மாறிலி *filename)
{
  bool ok;

  எனில் (! streq (filename, "-") && freopen (filename, "r", stdin) == வெற்று)
    {
      error (0, errno, "%s", quotef (filename));
      திரும்பு false;
    }

  ok = dc_parse_stream (stdin, filename);

  எனில் (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      திரும்பு false;
    }

  திரும்பு ok;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool ok = true;
  முழு optc;
  எண்ணிக்கை Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((optc = getopt_long (argc, argv, "bcp", long_options, வெற்று)) != -1)
    தேர்வு (optc)
      {
      நிலை 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        நிறுத்து;

      நிலை 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        நிறுத்து;

      நிலை 'p':
        print_database = true;
        நிறுத்து;

      நிலை PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        நிறுத்து;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      இயல்பு:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  எனில் ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  எனில் (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  எனில் ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      எனில் (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  எனில் (print_database)
    {
      எழுத்து மாறிலி *p = G_வரி;
      போது (p - G_வரி < அளவு G_வரி)
        {
          puts (p);
          p += strlen (p) + 1;
        }
    }
  இல்லையெனில்
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      எனில் (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          எனில் (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      எனில் (argc == 0)
        ok = dc_parse_stream (வெற்று, வெற்று);
      இல்லையெனில்
        ok = dc_parse_file (argv[0]);

      எனில் (ok)
        {
          size_t len = obstack_object_size (&lsc_obstack);
          எழுத்து *s = obstack_finish (&lsc_obstack);
          எழுத்து மாறிலி *prefix;
          எழுத்து மாறிலி *suffix;

          எனில் (syntax == SHELL_SYNTAX_BOURNE)
            {
              prefix = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          இல்லையெனில்
            {
              prefix = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          எனில் (! print_ls_colors)
            fputs (prefix, stdout);
          fwrite (s, 1, len, stdout);
          எனில் (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
