/* echo.c, derived from code echo.c in Bash.
   Copyright (C) 1987-2026 Free Software Foundation, Inc.

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
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"
#अन्तर्भाव "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "echo"

#define AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#ifndef DEFAULT_ECHO_TO_XPG
गणना { DEFAULT_ECHO_TO_XPG = false };
#endif

शून्य
usage (पूर्णांक status)
{
  /* STATUS should always be EXIT_SUCCESS (unlike in most other
     utilities which would call emit_try_help otherwise).  */
  affirm (status == निकास_सफल);

  म_लिखो (_("\
Usage: %s [SHORT-OPTION]... [STRING]...\n\
  or:  %s LONG-OPTION\n\
"), program_name, program_name);
  ख_माला_दो (_("\
Echo the STRING(s) to standard output.\n\
\n\
"), मानक_निकास);
  oputs (_("\
  -n     करा not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -e     enable interpretation of backslash escapes (पूर्वनिर्धारित)\n\
")
           : N_("\
  -e     enable interpretation of backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -E     disable interpretation of backslash escapes\n\
")
           : N_("\
  -E     disable interpretation of backslash escapes (पूर्वनिर्धारित)\n\
")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (VERSION_OPTION_DESCRIPTION);
  ख_माला_दो (_("\
\n\
If -e is in effect, the following sequences are recognized:\n\
\n\
"), मानक_निकास);
  ख_माला_दो (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new ओळ\n\
  \\r      carriage परत\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), मानक_निकास);
  ख_माला_दो (_("\
  \\0NNN   byte with octal मूल्य NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal मूल्य HH (1 to 2 digits)\n\
"), मानक_निकास);
  म_लिखो (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
  ख_माला_दो (_("\n\
Consider using the म_लिखो(1) command instead,\n\
as it avoids problems when outputting option-like strings.\n\
"), मानक_निकास);
  emit_ancillary_info (PROGRAM_NAME);
  निकास (status);
}

/* Convert C from hexadecimal character to integer.  */
स्थिर पूर्णांक
hextobin (अचिन्हित अक्षर c)
{
  निवड (c)
    {
    पूर्वनिर्धारित: परत c - '0';
    स्थिती 'a': स्थिती 'A': परत 10;
    स्थिती 'b': स्थिती 'B': परत 11;
    स्थिती 'c': स्थिती 'C': परत 12;
    स्थिती 'd': स्थिती 'D': परत 13;
    स्थिती 'e': स्थिती 'E': परत 14;
    स्थिती 'f': स्थिती 'F': परत 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  bool display_return = true;
  bool posixly_correct = !!दो_पर्या ("POSIXLY_CORRECT");
  bool allow_options =
    (! posixly_correct
     || (! DEFAULT_ECHO_TO_XPG && 1 < argc && streq (argv[1], "-n")));

  /* System V machines already have a /bin/sh with a v9 behavior.
     Use the identical behavior for these machines so that the
     existing system shell scripts won't barf.  */
  bool do_v9 = DEFAULT_ECHO_TO_XPG;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  जर (allow_options && argc == 2)
    {
      जर (streq (argv[1], "--help"))
        usage (निकास_सफल);

      जर (streq (argv[1], "--version"))
        {
          version_etc (मानक_निकास, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (अक्षर *) रिक्त);
          परत निकास_सफल;
        }
    }

  --argc;
  ++argv;

  जर (allow_options)
    पर्यंत (argc > 0 && *argv[0] == '-')
      {
        अक्षर स्थिरांक *temp = argv[0] + 1;
        माप_प्रकार i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        साठी (i = 0; temp[i]; i++)
          निवड (temp[i])
            {
            स्थिती 'e': स्थिती 'E': स्थिती 'n':
              थांबा;
            पूर्वनिर्धारित:
              जा just_echo;
            }

        जर (i == 0)
          जा just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        पर्यंत (*temp)
          निवड (*temp++)
            {
            स्थिती 'e':
              do_v9 = true;
              थांबा;

            स्थिती 'E':
              do_v9 = false;
              थांबा;

            स्थिती 'n':
              display_return = false;
              थांबा;
            }

        argc--;
        argv++;
      }

just_echo:

  जर (do_v9 || posixly_correct)
    {
      पर्यंत (argc > 0)
        {
          अक्षर स्थिरांक *s = argv[0];
          अचिन्हित अक्षर c;

          पर्यंत ((c = *s++))
            {
              जर (c == '\\' && *s)
                {
                  निवड (c = *s++)
                    {
                    स्थिती 'a': c = '\a'; थांबा;
                    स्थिती 'b': c = '\b'; थांबा;
                    स्थिती 'c': परत निकास_सफल;
                    स्थिती 'e': c = '\x1B'; थांबा;
                    स्थिती 'f': c = '\f'; थांबा;
                    स्थिती 'n': c = '\n'; थांबा;
                    स्थिती 'r': c = '\r'; थांबा;
                    स्थिती 't': c = '\t'; थांबा;
                    स्थिती 'v': c = '\v'; थांबा;
                    स्थिती 'x':
                      {
                        अचिन्हित अक्षर ch = *s;
                        जर (! c_isxdigit (ch))
                          जा not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        जर (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      थांबा;
                    स्थिती '0':
                      c = 0;
                      जर (! ('0' <= *s && *s <= '7'))
                        थांबा;
                      c = *s++;
                      FALLTHROUGH;
                    स्थिती '1': स्थिती '2': स्थिती '3':
                    स्थिती '4': स्थिती '5': स्थिती '6': स्थिती '7':
                      c -= '0';
                      जर ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      जर ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      थांबा;
                    स्थिती '\\': थांबा;

                    not_an_escape:
                    पूर्वनिर्धारित:  अक्षर_दो ('\\'); थांबा;
                    }
                }
              अक्षर_दो (c);
            }
          argc--;
          argv++;
          जर (argc > 0)
            अक्षर_दो (' ');
        }
    }
  अन्यथा
    {
      पर्यंत (argc > 0)
        {
          ख_माला_दो (argv[0], मानक_निकास);
          argc--;
          argv++;
          जर (argc > 0)
            अक्षर_दो (' ');
        }
    }

  जर (display_return)
    अक्षर_दो ('\n');
  परत निकास_सफल;
}
