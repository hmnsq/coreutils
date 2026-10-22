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
#परिभाषय क्रमादेशः_NAME "dircolors"

#परिभाषय AUTHORS proper_name ("H. Peter Anvin")

#परिभाषय obstack_chunk_alloc स्मृतिं_देहि
#परिभाषय obstack_chunk_free मुक्त

गणना Shell_syntax
{
  SHELL_SYNTAX_BOURNE,
  SHELL_SYNTAX_C,
  SHELL_SYNTAX_UNKNOWN
};

#परिभाषय APPEND_CHAR(C) obstack_1grow (&lsc_obstack, C)

/* Accumulate in this obstack the value for the LS_COLORS environment
   variable.  */
स्थिर संरचना obstack lsc_obstack;

स्थिर वर्ण ध्रुव *ध्रुव slack_codes[] =
{
  "NORMAL", "NORM", "FILE", "RESET", "DIR", "LNK", "LINK",
  "SYMLINK", "ORPHAN", "MISSING", "FIFO", "PIPE", "SOCK", "BLK", "BLOCK",
  "CHR", "CHAR", "DOOR", "EXEC", "LEFT", "LEFTCODE", "RIGHT", "RIGHTCODE",
  "END", "ENDCODE", "SUID", "SETUID", "SGID", "SETGID", "STICKY",
  "OTHER_WRITABLE", "OWR", "STICKY_OTHER_WRITABLE", "OWT", "CAPABILITY",
  "MULTIHARDLINK", "CLRTOEOL", रिक्त
};

स्थिर वर्ण ध्रुव *ध्रुव ls_codes[] =
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
  PRINT_LS_COLORS_OPTION = CHAR_MAX + 1,
};

स्थिर संरचना option ध्रुव long_options[] =
  {
    {"bourne-shell", no_तर्कः, रिक्त, 'b'},
    {"sh", no_तर्कः, रिक्त, 'b'},
    {"csh", no_तर्कः, रिक्त, 'c'},
    {"c-shell", no_तर्कः, रिक्त, 'c'},
    {"print-database", no_तर्कः, रिक्त, 'p'},
    {"print-ls-colors", no_तर्कः, रिक्त, PRINT_LS_COLORS_OPTION},
    {GETOPT_HELP_OPTION_DECL},
    {GETOPT_संस्करणम्_OPTION_DECL},
    {रिक्त, 0, रिक्त, 0}
  };

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION]... [FILE]\n"), क्रमादेशः_name);
      fputs (_("\
Output commands गन्तव्यम् समुच्चयः_2 the LS_COLORS environment variable.\n\
\n\
Determine format सम्बन्धि output:\n\
"), stdout);
      oputs (_("\
  -b, --sh, --bourne-shell\n\
         output Bourne shell संहिता गन्तव्यम् समुच्चयः_2 LS_COLORS\n\
"));
      oputs (_("\
  -c, --csh, --c-shell\n\
         output C shell संहिता गन्तव्यम् समुच्चयः_2 LS_COLORS\n\
"));
      oputs (_("\
  -p, --print-database\n\
         output defaults\n\
"));
      oputs (_("\
      --print-ls-colors\n\
         output fully escaped colors प्रति display\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If FILE is specified, पठ it गन्तव्यम् determine which colors गन्तव्यम् use प्रति which\n\
file types and extensions.  Otherwise, a precompiled database is प्रयुक्तम्.\n\
For details on the format सम्बन्धि these files, सततम्_2 'dircolors --print-database'.\n\
"), stdout);
      emit_ancillary_info (क्रमादेशः_NAME);
    }

  निर्गच्छ (स्थितिः);
}

/* If the SHELL environment variable is set to 'csh' or 'tcsh,'
   assume C shell.  Else Bourne shell.  */

स्थिर गणना Shell_syntax
guess_shell_syntax (शून्य)
{
  वर्ण *shell;

  shell = getenv ("SHELL");
  यदि (shell == रिक्त || *shell == '\0')
    प्रत्यागम SHELL_SYNTAX_UNKNOWN;

  shell = चरमः_component (shell);

  यदि (streq (shell, "csh") || streq (shell, "tcsh"))
    प्रत्यागम SHELL_SYNTAX_C;

  प्रत्यागम SHELL_SYNTAX_BOURNE;
}

स्थिर शून्य
parse_पङ्क्तिः (वर्ण ध्रुव *पङ्क्तिः, वर्ण **keyword, वर्ण **arg)
{
  वर्ण ध्रुव *p;
  वर्ण ध्रुव *keyword_start;
  वर्ण ध्रुव *arg_start;

  *keyword = रिक्त;
  *arg = रिक्त;

  प्रति (p = पङ्क्तिः; c_isspace (*p); ++p)
    अनुवर्त;

  /* Ignore blank lines and shell-style comments.  */
  यदि (*p == '\0' || *p == '#')
    प्रत्यागम;

  keyword_start = p;

  यावत् (!c_isspace (*p) && *p != '\0')
    {
      ++p;
    }

  *keyword = ximemdup0 (keyword_start, p - keyword_start);
  यदि (*p  == '\0')
    प्रत्यागम;

  कुरु
    {
      ++p;
    }
  यावत् (c_isspace (*p));

  यदि (*p == '\0' || *p == '#')
    प्रत्यागम;

  arg_start = p;

  यावत् (*p != '\0' && *p != '#')
    ++p;

  प्रति (--p; c_isspace (*p); --p)
    अनुवर्त;
  ++p;

  *arg = ximemdup0 (arg_start, p - arg_start);
}

/* Accumulate STR to LS_COLORS data.
   If outputting shell syntax, then escape appropriately.  */

स्थिर शून्य
append_quoted (वर्ण ध्रुव *str)
{
  bool need_backslash = true;

  यावत् (*str != '\0')
    {
      यदि (! print_ls_colors)
        विकल्प (*str)
          {
          पक्ष '\'':
            APPEND_CHAR ('\'');
            APPEND_CHAR ('\\');
            APPEND_CHAR ('\'');
            need_backslash = true;
            भङ्ग;

          पक्ष '\\':
          पक्ष '^':
            need_backslash = !need_backslash;
            भङ्ग;

          पक्ष ':':
          पक्ष '=':
            यदि (need_backslash)
              APPEND_CHAR ('\\');
            FALLTHROUGH;

          स्वभाव:
            need_backslash = true;
            भङ्ग;
          }

      APPEND_CHAR (*str);
      ++str;
    }
}

/* Accumulate entry to LS_COLORS data.
   Use shell syntax unless PRINT_LS_COLORS is set.  */

स्थिर शून्य
append_प्रविष्टिः (वर्ण उपसर्गः, वर्ण ध्रुव *item, वर्ण ध्रुव *arg)
{
  यदि (print_ls_colors)
    {
      append_quoted ("\x1B[");
      append_quoted (arg);
      APPEND_CHAR ('m');
    }
  यदि (उपसर्गः)
    APPEND_CHAR (उपसर्गः);
  append_quoted (item);
  APPEND_CHAR (print_ls_colors ? '\t' : '=');
  append_quoted (arg);
  यदि (print_ls_colors)
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
dc_parse_प्रवाहः (FILE *fp, वर्ण ध्रुव *filename)
{
  idx_t पङ्क्तिः_सङ्ख्या = 0;
  वर्ण ध्रुव *अग्र_G_पङ्क्तिः = G_पङ्क्तिः;
  वर्ण *निवेशः_पङ्क्तिः = रिक्त;
  size_t निवेशः_पङ्क्तिः_आकार = 0;
  वर्ण ध्रुव *पङ्क्तिः;
  वर्ण ध्रुव *पदांशः;
  वर्ण ध्रुव *colorterm;
  bool ok = true;

  /* State for the parser.  */
  गणना { ST_TERMNO, ST_TERMYES, ST_TERMSURE, ST_GLOBAL } state = ST_GLOBAL;

  /* Get terminal type */
  पदांशः = getenv ("TERM");
  यदि (पदांशः == रिक्त || *पदांशः == '\0')
    पदांशः = "none";

  /* Also match $COLORTERM.  */
  colorterm = getenv ("COLORTERM");
  यदि (colorterm == रिक्त)
    colorterm = "";  /* Doesn't match default "?*"  */

  यावत् (true)
    {
      वर्ण *keywd, *arg;
      bool unrecognized;

      ++पङ्क्तिः_सङ्ख्या;

      यदि (fp)
        {
          यदि (getline (&निवेशः_पङ्क्तिः, &निवेशः_पङ्क्तिः_आकार, fp) <= 0)
            {
              यदि (ferror (fp))
                {
                  error (0, errno, _("%s: read error"), quotef (filename));
                  ok = false;
                }
              मुक्त (निवेशः_पङ्क्तिः);
              भङ्ग;
            }
          पङ्क्तिः = निवेशः_पङ्क्तिः;
        }
      अन्यथा
        {
          यदि (अग्र_G_पङ्क्तिः == G_पङ्क्तिः + परिमाण G_पङ्क्तिः)
            भङ्ग;
          पङ्क्तिः = अग्र_G_पङ्क्तिः;
          अग्र_G_पङ्क्तिः += वाक्यदैर्घ्यम् (अग्र_G_पङ्क्तिः) + 1;
        }

      parse_पङ्क्तिः (पङ्क्तिः, &keywd, &arg);

      यदि (keywd == रिक्त)
        अनुवर्त;

      यदि (arg == रिक्त)
        {
          error (0, 0, _("%s:%td: invalid line;  missing second token"),
                 quotef (filename), पङ्क्तिः_सङ्ख्या);
          ok = false;
          मुक्त (keywd);
          अनुवर्त;
        }

      unrecognized = false;
      यदि (c_strcasecmp (keywd, "TERM") == 0)
        {
          यदि (state != ST_TERMSURE)
            state = fnmatch (arg, पदांशः, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      अन्यथा यदि (c_strcasecmp (keywd, "COLORTERM") == 0)
        {
          यदि (state != ST_TERMSURE)
            state = fnmatch (arg, colorterm, 0) == 0 ? ST_TERMSURE : ST_TERMNO;
        }
      अन्यथा
        {
          यदि (state == ST_TERMSURE)
            state = ST_TERMYES;  /* Another {COLOR,}TERM can cancel.  */

          यदि (state != ST_TERMNO)
            {
              यदि (keywd[0] == '.')
                append_प्रविष्टिः ('*', keywd, arg);
              अन्यथा यदि (keywd[0] == '*')
                append_प्रविष्टिः (0, keywd, arg);
              अन्यथा यदि (c_strcasecmp (keywd, "OPTIONS") == 0
                       || c_strcasecmp (keywd, "COLOR") == 0
                       || c_strcasecmp (keywd, "EIGHTBIT") == 0)
                {
                  /* Ignore.  */
                }
              अन्यथा
                {
                  पूर्णाङ्क i;

                  प्रति (i = 0; slack_codes[i] != रिक्त; ++i)
                    यदि (c_strcasecmp (keywd, slack_codes[i]) == 0)
                      भङ्ग;

                  यदि (slack_codes[i] != रिक्त)
                    append_प्रविष्टिः (0, ls_codes[i], arg);
                  अन्यथा
                    unrecognized = true;
                }
            }
          अन्यथा
            unrecognized = true;
        }

      यदि (unrecognized && (state == ST_TERMSURE || state == ST_TERMYES))
        {
          error (0, 0, _("%s:%td: unrecognized keyword %s"),
                 (filename ? quotef (filename) : _("<internal>")),
                 पङ्क्तिः_सङ्ख्या, keywd);
          ok = false;
        }

      मुक्त (keywd);
      मुक्त (arg);
    }

  प्रत्यागम ok;
}

स्थिर bool
dc_parse_file (वर्ण ध्रुव *filename)
{
  bool ok;

  यदि (! streq (filename, "-") && freopen (filename, "r", stdin) == रिक्त)
    {
      error (0, errno, "%s", quotef (filename));
      प्रत्यागम false;
    }

  ok = dc_parse_प्रवाहः (stdin, filename);

  यदि (fclose (stdin) != 0)
    {
      error (0, errno, "%s", quotef (filename));
      प्रत्यागम false;
    }

  प्रत्यागम ok;
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool ok = true;
  पूर्णाङ्क optc;
  गणना Shell_syntax syntax = SHELL_SYNTAX_UNKNOWN;
  bool print_database = false;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् ((optc = getopt_long (argc, argv, "bcp", long_options, रिक्त)) != -1)
    विकल्प (optc)
      {
      पक्ष 'b':	/* Bourne shell syntax.  */
        syntax = SHELL_SYNTAX_BOURNE;
        भङ्ग;

      पक्ष 'c':	/* C shell syntax.  */
        syntax = SHELL_SYNTAX_C;
        भङ्ग;

      पक्ष 'p':
        print_database = true;
        भङ्ग;

      पक्ष PRINT_LS_COLORS_OPTION:
        print_ls_colors = true;
        भङ्ग;

      case_GETOPT_HELP_CHAR;

      case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

      स्वभाव:
        usage (EXIT_FAILURE);
      }

  argc -= optind;
  argv += optind;

  /* It doesn't make sense to use --print with either of
     --bourne or --c-shell.  */
  यदि ((print_database | print_ls_colors) && syntax != SHELL_SYNTAX_UNKNOWN)
    {
      error (0, 0,
             _("the options to output non shell syntax,\n"
               "and to select a shell syntax are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  यदि (print_database && print_ls_colors)
    {
      error (0, 0,
             _("options --print-database and --print-ls-colors "
               "are mutually exclusive"));
      usage (EXIT_FAILURE);
    }

  यदि ((!print_database) < argc)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[!print_database]));
      यदि (print_database)
        fprintf (stderr, "%s\n",
                 _("file operands cannot be combined with "
                   "--print-database (-p)"));
      usage (EXIT_FAILURE);
    }

  यदि (print_database)
    {
      वर्ण ध्रुव *p = G_पङ्क्तिः;
      यावत् (p - G_पङ्क्तिः < परिमाण G_पङ्क्तिः)
        {
          पङ्क्तिं_लिख (p);
          p += वाक्यदैर्घ्यम् (p) + 1;
        }
    }
  अन्यथा
    {
      /* If shell syntax was not explicitly specified, try to guess it. */
      यदि (syntax == SHELL_SYNTAX_UNKNOWN && ! print_ls_colors)
        {
          syntax = guess_shell_syntax ();
          यदि (syntax == SHELL_SYNTAX_UNKNOWN)
            error (EXIT_FAILURE, 0,
                   _("no SHELL environment variable,"
                     " and no shell type option given"));
        }

      obstack_init (&lsc_obstack);
      यदि (argc == 0)
        ok = dc_parse_प्रवाहः (रिक्त, रिक्त);
      अन्यथा
        ok = dc_parse_file (argv[0]);

      यदि (ok)
        {
          size_t len = obstack_object_आकार (&lsc_obstack);
          वर्ण *s = obstack_finish (&lsc_obstack);
          वर्ण ध्रुव *उपसर्गः;
          वर्ण ध्रुव *suffix;

          यदि (syntax == SHELL_SYNTAX_BOURNE)
            {
              उपसर्गः = "LS_COLORS='";
              suffix = "';\nexport LS_COLORS\n";
            }
          अन्यथा
            {
              उपसर्गः = "setenv LS_COLORS '";
              suffix = "'\n";
            }
          यदि (! print_ls_colors)
            fputs (उपसर्गः, stdout);
          fwrite (s, 1, len, stdout);
          यदि (! print_ls_colors)
            fputs (suffix, stdout);
        }
    }

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
