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

#अन्तर्भाव <config.h>

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <fnmatch.h>
#अन्तर्भाव <getopt.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "dircolors.h"
#अन्तर्भाव "c-ctype.h"
#अन्तर्भाव "c-strcase.h"
#अन्तर्भाव "obstack.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dircolors"

#define AUTHORS proper_name ("H. Peter Anvin")

#define obstack_chunk_alloc दो_स्मृति
#define obstack_chunk_free मुक्त

गणना Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#define APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
स्थिर संरचना obstack lsc_obstack;

स्थिर अक्षर स्थिरांक *स्थिरांक slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", रिक्त
};

स्थिर अक्षर स्थिरांक *स्थिरांक ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  रिक्त
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
स्थिर bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  PRINT_LS_COLORS_OPTION = अक्षर_उच्च + 1,
};

स्थिर संरचना option स्थिरांक long_options[] =
  {
    {"bourne-shell", no_argument, रिक्त, 'b'},
    {"sh", no_argument, रिक्त, 'b'},
    {"csh", no_argument, रिक्त, 'c'},
    {"c-shell", no_argument, रिक्त, 'c'},
    {"print-database", no_argument, रिक्त, 'p'},
    {"print-ls-colors", no_argument, रिक्त, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {रिक्त, 0, रिक्त, 0}
  };

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... [FILE]\n"), program_name);
      ख_माला_दो (_("\
Output commands to set the LS_COLORS environment variable.\n\
\n\
Determine format of output:\n\
"), मानक_निकास);
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
         output fully escaped colors साठी display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
If खाता is specified, पठ it to determine which colors to use साठी which\n\
file types and extensions.  Otherwise, a precompiled database is used.\n\
For details on the format of these files, run 'dircolors --print-database'.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }

  निकास (status);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

स्थिर गणना Shell_syntax
guess_shell_syntax (शून्य)
{
  अक्षर *shell;

  shell = दो_पर्या ("SHELL");
  जर (shell == रिक्त || *shell == '\0')
    परत SHELL_SYNTAX_UNKNOWN;

  shell = last_component (shell);

  जर (streq (shell, "csh") || streq (shell, "tcsh"))
    परत SHELL_SYNTAX_C;

  परत SHELL_SYNTAX_BOURNE;
}

स्थिर शून्य
parse_ओळ (अक्षर स्थिरांक *ओळ, अक्षर **keyword, अक्षर **arg)
{
  अक्षर स्थिरांक *p;
  अक्षर स्थिरांक *keyword_start;
  अक्षर स्थिरांक *arg_start;

  *keyword = रिक्त;
  *arg = रिक्त;

  साठी (p = ओळ; c_isspace (*p); ++p)
    सुरूठेवा;

  /* Ignore blank lines and shell-style comments.  */
  जर (*p == '\0' || *p == '#')
    परत;

  keyword_start = p;

  पर्यंत (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  जर (*p  == '\0')
    परत;

  करा
    {
      ++p;
    }
  पर्यंत (c_isspace (*p));

  जर (*p == '\0' || *p == '#')
    परत;

  arg_start = p;

  पर्यंत (*p != '\0' && *p != '#')
    ++p;

  साठी (--p; c_isspace (*p); --p)
    सुरूठेवा;
  ++p;

  *arg = ximemdup0 (arg_start, p - arg_start);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

स्थिर शून्य
append_quoted (अक्षर स्थिरांक *str)
{
  bool need_backslash = true;

  पर्यंत (*str != '\0')
    {
      जर (! print_ls_colors)
        निवड (*str)
          {
          स्थिती '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            थांबा;

          स्थिती '\\':
          स्थिती '^':
            need_backslash = !need_backslash;
            थांबा;

          स्थिती ':':
          स्थिती '=':
            जर (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          पूर्वनिर्धारित:
            need_backslash = true;
            थांबा;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

स्थिर शून्य
append_entry (अक्षर prefix, अक्षर स्थिरांक *item, अक्षर स्थिरांक *arg)
{
  जर (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  जर (prefix)
    APPEND_CHAR (prefix);
  append_quoted (item);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  जर (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

स्थिर bool
dc_parse_stream (खाता *fp, अक्षर स्थिरांक *filename)
{
  idx_t ओळ_संख्या = 0;
  अक्षर स्थिरांक *next_G_ओळ = G_ओळ;
  अक्षर *आदान_ओळ = रिक्त;
  माप_प्रकार आदान_ओळ_size = 0;
  अक्षर स्थिरांक *ओळ;
  अक्षर स्थिरांक *term;
  अक्षर स्थिरांक *colorterm;
  bool ok = true;

  /* State for the parser.  */
  गणना { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  term = दो_पर्या ("TERM");
  जर (term == रिक्त || *term == '\0')
    term = "none";

  /* Also match $COLORTERM.  */
  colorterm = दो_पर्या ("COLORTERM");
  जर (colorterm == रिक्त)
    colorterm = "";  /* Doesn't match default "?*"  */

  पर्यंत (true)
    {
      अक्षर *keywd, *arg;
      bool unrecognized;

      ++ओळ_संख्या;

      जर (fp)
        {
          जर (getline (&आदान_ओळ, &आदान_ओळ_size, fp) <= 0)
            {
              जर (ख_त्रुटि (fp))
                {
                  error (0, त्रुटि_सं, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              मुक्त (आदान_ओळ);
              थांबा;
            }
          ओळ = आदान_ओळ;
        }
      अन्यथा
        {
          जर (next_G_ओळ == G_ओळ + आकार G_ओळ)
            थांबा;
          ओळ = next_G_ओळ;
          next_G_ओळ += म_माप (next_G_ओळ) + 1;
        }

      parse_ओळ (ओळ, &keywd, &arg);

      जर (keywd == रिक्त)
        सुरूठेवा;

      जर (arg == रिक्त)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), ओळ_संख्या);
          ok = false;
          मुक्त (keywd);
          सुरूठेवा;
        }

      unrecognized = false;
      जर (c_strcasecmp (keywd, "TERM") == 0)
        {
          जर (state != ST_TERMSURE)
            state = fnmatch (arg, term, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      अन्यथा जर (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          जर (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      अन्यथा
        {
          जर (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          जर (state != ST_TERMNO)
            {
              जर (keywd[0] == '.')
                append_entry ('*', keywd, arg);
              अन्यथा जर (keywd[0] == '*')
                append_entry (0, keywd, arg);
              अन्यथा जर (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              अन्यथा
                {
                  पूर्णांक i;

                  साठी (i = 0; slack_codes[i] != रिक्त; ++i)
                    जर (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      थांबा;

                  जर (slack_codes[i] != रिक्त)
                    append_entry (0, ls_codes[i], arg);
                  अन्यथा
                    unrecognized = true;
                }
            }
          अन्यथा
            unrecognized = true;
        }

      जर (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 ओळ_संख्या, keywd);
          ok = false;
        }

      मुक्त (keywd);
      मुक्त (arg);
    }

  परत ok;
}

स्थिर bool
dc_parse_file (अक्षर स्थिरांक *filename)
{
  bool ok;

  जर (! streq (filename, "-") && ख_व_खोलो (filename, "r", मानक_निवेश) == रिक्त)
    {
      error (0, त्रुटि_सं, "%s", quotef (filename));
      परत false;
    }

  ok = dc_parse_stream (मानक_निवेश, filename);

  जर (ख_बंद (मानक_निवेश) != 0)
    {
      error (0, त्रुटि_सं, "%s", quotef (filename));
      परत false;
    }

  परत ok;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  bool ok = true;
  पूर्णांक optc;
  गणना Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत ((optc = getopt_long (argc, argv, "bcp", long_options, रिक्त)) != -1)
    निवड (optc)
      {
      स्थिती 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        थांबा;

      स्थिती 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        थांबा;

      स्थिती 'p':
        print_database = true;
        थांबा;

      स्थिती PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        थांबा;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      पूर्वनिर्धारित:
        usage (निकास_त्रुटि);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  जर ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (निकास_त्रुटि);
    }

  जर (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (निकास_त्रुटि);
    }

  जर ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      जर (print_database)
        ख_लिखो (मानक_त्रुटि, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (निकास_त्रुटि);
    }

  जर (print_database)
    {
      अक्षर स्थिरांक *p = G_ओळ;
      पर्यंत (p - G_ओळ < आकार G_ओळ)
        {
          माला_दो (p);
          p += म_माप (p) + 1;
        }
    }
  अन्यथा
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      जर (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          जर (syntax == SHELL_SYNTAX_UNKNOWN)
            error (निकास_त्रुटि, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      जर (argc == 0)
        ok = dc_parse_stream (रिक्त, रिक्त);
      अन्यथा
        ok = dc_parse_file (argv[0]);

      जर (ok)
        {
          माप_प्रकार len = obstack_object_size (&lsc_obstack);
          अक्षर *s = obstack_finish (&lsc_obstack);
          अक्षर स्थिरांक *prefix;
          अक्षर स्थिरांक *suffix;

          जर (syntax == SHELL_SYNTAX_BOURNE)
            {
              prefix = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          अन्यथा
            {
              prefix = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          जर (! print_ls_colors)
            ख_माला_दो (prefix, मानक_निकास);
          ख_डालो (s, 1, len, मानक_निकास);
          जर (! print_ls_colors)
            ख_माला_दो (suffix, मानक_निकास);
        }
    }

  परत ok ? निकास_सफल : निकास_त्रुटि;
}
