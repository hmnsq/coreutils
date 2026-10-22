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

#समावेश <config.h>

#समावेश <sys/types.h>
#समावेश <fnmatch.h>
#समावेश <getopt.h>

#समावेश "system.h"
#समावेश "dircolors.h"
#समावेश "c-ctype.h"
#समावेश "c-strcase.h"
#समावेश "obstack.h"
#समावेश "quote.h"
#समावेश "stdio--.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "dircolors"

#घोषणा AUTHORS proper_name ("H. Peter Anvin")

#घोषणा obstack_chunk_alloc दो_स्मृति
#घोषणा obstack_chunk_free मुक्त

क्रमागत Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#घोषणा APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
अटल काष्ठा obstack lsc_obstack;

अटल अक्षर स्थिर *स्थिर slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", शून्य
};

अटल अक्षर स्थिर *स्थिर ls_codes[] =
{
  "no", "no", "fi", "rs", "di", "ln", "ln", "ln", "or", "mi", "pi", "pi",
  "so", "bd", "bd", "cd", "cd", "do", "ex", "lc", "lc", "rc", "rc", "ec", "ec",
  "su", "su", "sg", "sg", "st", "ow", "ow", "tw", "tw", "ca", "mh", "cl",
  शून्य
};
static_assert (countof (slack_codes) == countof (ls_codes));

/* Whether to output escaped ls color codes for display.  */
अटल bool print_ls_colors;

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
क्रमागत
{
  PRINT_LS_COLORS_OPTION = अक्षर_उच्च + 1,
};

अटल काष्ठा option स्थिर long_options[] =
  {
    {"bourne-shell", no_argument, शून्य, 'b'},
    {"sh", no_argument, शून्य, 'b'},
    {"csh", no_argument, शून्य, 'c'},
    {"c-shell", no_argument, शून्य, 'c'},
    {"print-database", no_argument, शून्य, 'p'},
    {"print-ls-colors", no_argument, शून्य, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_VERSION_OPTION_DECL},
    {शून्य, 0, शून्य, 0}
  };

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
         output fully escaped colors क्रम display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
If खाता is specified, पढ़ो it to determine which colors to use क्रम which\n\
file types and extensions.  Otherwise, a precompiled database is used.\n\
For details on the format of these files, run 'dircolors --print-database'.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }

  निकास (status);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

अटल क्रमागत Shell_syntax
guess_shell_syntax (व्योम)
{
  अक्षर *shell;

  shell = दो_पर्या ("SHELL");
  अगर (shell == शून्य || *shell == '\0')
    वापस SHELL_SYNTAX_UNKNOWN;

  shell = last_component (shell);

  अगर (streq (shell, "csh") || streq (shell, "tcsh"))
    वापस SHELL_SYNTAX_C;

  वापस SHELL_SYNTAX_BOURNE;
}

अटल व्योम
parse_पंक्ति (अक्षर स्थिर *पंक्ति, अक्षर **keyword, अक्षर **arg)
{
  अक्षर स्थिर *p;
  अक्षर स्थिर *keyword_start;
  अक्षर स्थिर *arg_start;

  *keyword = शून्य;
  *arg = शून्य;

  क्रम (p = पंक्ति; c_isspace (*p); ++p)
    जारी;

  /* Ignore blank lines and shell-style comments.  */
  अगर (*p == '\0' || *p == '#')
    वापस;

  keyword_start = p;

  जबतक (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  अगर (*p  == '\0')
    वापस;

  करो
    {
      ++p;
    }
  जबतक (c_isspace (*p));

  अगर (*p == '\0' || *p == '#')
    वापस;

  arg_start = p;

  जबतक (*p != '\0' && *p != '#')
    ++p;

  क्रम (--p; c_isspace (*p); --p)
    जारी;
  ++p;

  *arg = ximemdup0 (arg_start, p - arg_start);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

अटल व्योम
append_quoted (अक्षर स्थिर *str)
{
  bool need_backslash = true;

  जबतक (*str != '\0')
    {
      अगर (! print_ls_colors)
        चयन (*str)
          {
          हाल '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            अवरोध;

          हाल '\\':
          हाल '^':
            need_backslash = !need_backslash;
            अवरोध;

          हाल ':':
          हाल '=':
            अगर (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          शेष:
            need_backslash = true;
            अवरोध;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

अटल व्योम
append_entry (अक्षर prefix, अक्षर स्थिर *item, अक्षर स्थिर *arg)
{
  अगर (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  अगर (prefix)
    APPEND_CHAR (prefix);
  append_quoted (item);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  अगर (print_ls_colors)
    append_quoted ("\x1B[0m");
  APPEND_CHAR (print_ls_colors ? '\n' : ':');
}

/* Read the file open on FP (with name FILENAME).  First, look for a
   'TERM name' directive where name matches the current terminal type.
   Once found, translate and accumulate the associated directives onto
   the global obstack LSC_OBSTACK.  Give a diagnostic
   upon failure (unrecognized keyword is the only way to fail here).
   Return true if successful.  */

अटल bool
dc_parse_stream (खाता *fp, अक्षर स्थिर *filename)
{
  idx_t पंक्ति_संख्या = 0;
  अक्षर स्थिर *अग्र_G_पंक्ति = G_पंक्ति;
  अक्षर *निवेश_पंक्ति = शून्य;
  माप_प्रकार निवेश_पंक्ति_आकार = 0;
  अक्षर स्थिर *पंक्ति;
  अक्षर स्थिर *term;
  अक्षर स्थिर *colorterm;
  bool ok = true;

  /* State for the parser.  */
  क्रमागत { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  term = दो_पर्या ("TERM");
  अगर (term == शून्य || *term == '\0')
    term = "none";

  /* Also match $COLORTERM.  */
  colorterm = दो_पर्या ("COLORTERM");
  अगर (colorterm == शून्य)
    colorterm = "";  /* Doesn't match default "?*"  */

  जबतक (true)
    {
      अक्षर *keywd, *arg;
      bool unrecognized;

      ++पंक्ति_संख्या;

      अगर (fp)
        {
          अगर (getline (&निवेश_पंक्ति, &निवेश_पंक्ति_आकार, fp) <= 0)
            {
              अगर (ख_त्रुटि (fp))
                {
                  error (0, त्रुटि_सं, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              मुक्त (निवेश_पंक्ति);
              अवरोध;
            }
          पंक्ति = निवेश_पंक्ति;
        }
      अन्यथा
        {
          अगर (अग्र_G_पंक्ति == G_पंक्ति + माप G_पंक्ति)
            अवरोध;
          पंक्ति = अग्र_G_पंक्ति;
          अग्र_G_पंक्ति += म_माप (अग्र_G_पंक्ति) + 1;
        }

      parse_पंक्ति (पंक्ति, &keywd, &arg);

      अगर (keywd == शून्य)
        जारी;

      अगर (arg == शून्य)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), पंक्ति_संख्या);
          ok = false;
          मुक्त (keywd);
          जारी;
        }

      unrecognized = false;
      अगर (c_strcasecmp (keywd, "TERM") == 0)
        {
          अगर (state != ST_TERMSURE)
            state = fnmatch (arg, term, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      अन्यथा अगर (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          अगर (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      अन्यथा
        {
          अगर (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          अगर (state != ST_TERMNO)
            {
              अगर (keywd[0] == '.')
                append_entry ('*', keywd, arg);
              अन्यथा अगर (keywd[0] == '*')
                append_entry (0, keywd, arg);
              अन्यथा अगर (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              अन्यथा
                {
                  पूर्णांक i;

                  क्रम (i = 0; slack_codes[i] != शून्य; ++i)
                    अगर (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      अवरोध;

                  अगर (slack_codes[i] != शून्य)
                    append_entry (0, ls_codes[i], arg);
                  अन्यथा
                    unrecognized = true;
                }
            }
          अन्यथा
            unrecognized = true;
        }

      अगर (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 पंक्ति_संख्या, keywd);
          ok = false;
        }

      मुक्त (keywd);
      मुक्त (arg);
    }

  वापस ok;
}

अटल bool
dc_parse_file (अक्षर स्थिर *filename)
{
  bool ok;

  अगर (! streq (filename, "-") && ख_व_खोलो (filename, "r", मानक_निवेश) == शून्य)
    {
      error (0, त्रुटि_सं, "%s", quotef (filename));
      वापस false;
    }

  ok = dc_parse_stream (मानक_निवेश, filename);

  अगर (ख_बंद (मानक_निवेश) != 0)
    {
      error (0, त्रुटि_सं, "%s", quotef (filename));
      वापस false;
    }

  वापस ok;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  bool ok = true;
  पूर्णांक optc;
  क्रमागत Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((optc = getopt_long (argc, argv, "bcp", long_options, शून्य)) != -1)
    चयन (optc)
      {
      हाल 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        अवरोध;

      हाल 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        अवरोध;

      हाल 'p':
        print_database = true;
        अवरोध;

      हाल PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        अवरोध;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

      शेष:
        usage (निकास_त्रुटि);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  अगर ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (निकास_त्रुटि);
    }

  अगर (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (निकास_त्रुटि);
    }

  अगर ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      अगर (print_database)
        ख_लिखो (मानक_त्रुटि, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (निकास_त्रुटि);
    }

  अगर (print_database)
    {
      अक्षर स्थिर *p = G_पंक्ति;
      जबतक (p - G_पंक्ति < माप G_पंक्ति)
        {
          माला_दो (p);
          p += म_माप (p) + 1;
        }
    }
  अन्यथा
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      अगर (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          अगर (syntax == SHELL_SYNTAX_UNKNOWN)
            error (निकास_त्रुटि, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      अगर (argc == 0)
        ok = dc_parse_stream (शून्य, शून्य);
      अन्यथा
        ok = dc_parse_file (argv[0]);

      अगर (ok)
        {
          माप_प्रकार len = obstack_object_आकार (&lsc_obstack);
          अक्षर *s = obstack_finish (&lsc_obstack);
          अक्षर स्थिर *prefix;
          अक्षर स्थिर *suffix;

          अगर (syntax == SHELL_SYNTAX_BOURNE)
            {
              prefix = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          अन्यथा
            {
              prefix = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          अगर (! print_ls_colors)
            ख_माला_दो (prefix, मानक_निकास);
          ख_डालो (s, 1, len, मानक_निकास);
          अगर (! print_ls_colors)
            ख_माला_दो (suffix, मानक_निकास);
        }
    }

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
