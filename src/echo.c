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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश "system.h"
#समावेश "assure.h"
#समावेश "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "echo"

#घोषणा AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#अगर_अघोषित DEFAULT_ECHO_TO_XPG
क्रमागत { DEFAULT_ECHO_TO_XPG = false };
#पूर्ण_अगर

व्योम
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
  -n     करो not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -e     enable interpretation of backslash escapes (शेष)\n\
")
           : N_("\
  -e     enable interpretation of backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -E     disable interpretation of backslash escapes\n\
")
           : N_("\
  -E     disable interpretation of backslash escapes (शेष)\n\
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
  \\n      new पंक्ति\n\
  \\r      carriage वापस\n\
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
अटल पूर्णांक
hextobin (अचिन्हित अक्षर c)
{
  चयन (c)
    {
    शेष: वापस c - '0';
    हाल 'a': हाल 'A': वापस 10;
    हाल 'b': हाल 'B': वापस 11;
    हाल 'c': हाल 'C': वापस 12;
    हाल 'd': हाल 'D': वापस 13;
    हाल 'e': हाल 'E': वापस 14;
    हाल 'f': हाल 'F': वापस 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
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
  अगर (allow_options && argc == 2)
    {
      अगर (streq (argv[1], "--help"))
        usage (निकास_सफल);

      अगर (streq (argv[1], "--version"))
        {
          version_etc (मानक_निकास, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (अक्षर *) शून्य);
          वापस निकास_सफल;
        }
    }

  --argc;
  ++argv;

  अगर (allow_options)
    जबतक (argc > 0 && *argv[0] == '-')
      {
        अक्षर स्थिर *अस्थायी = argv[0] + 1;
        माप_प्रकार i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        क्रम (i = 0; अस्थायी[i]; i++)
          चयन (अस्थायी[i])
            {
            हाल 'e': हाल 'E': हाल 'n':
              अवरोध;
            शेष:
              जाओ just_echo;
            }

        अगर (i == 0)
          जाओ just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        जबतक (*अस्थायी)
          चयन (*अस्थायी++)
            {
            हाल 'e':
              do_v9 = true;
              अवरोध;

            हाल 'E':
              do_v9 = false;
              अवरोध;

            हाल 'n':
              display_return = false;
              अवरोध;
            }

        argc--;
        argv++;
      }

just_echo:

  अगर (do_v9 || posixly_correct)
    {
      जबतक (argc > 0)
        {
          अक्षर स्थिर *s = argv[0];
          अचिन्हित अक्षर c;

          जबतक ((c = *s++))
            {
              अगर (c == '\\' && *s)
                {
                  चयन (c = *s++)
                    {
                    हाल 'a': c = '\a'; अवरोध;
                    हाल 'b': c = '\b'; अवरोध;
                    हाल 'c': वापस निकास_सफल;
                    हाल 'e': c = '\x1B'; अवरोध;
                    हाल 'f': c = '\f'; अवरोध;
                    हाल 'n': c = '\n'; अवरोध;
                    हाल 'r': c = '\r'; अवरोध;
                    हाल 't': c = '\t'; अवरोध;
                    हाल 'v': c = '\v'; अवरोध;
                    हाल 'x':
                      {
                        अचिन्हित अक्षर ch = *s;
                        अगर (! c_isxdigit (ch))
                          जाओ not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        अगर (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      अवरोध;
                    हाल '0':
                      c = 0;
                      अगर (! ('0' <= *s && *s <= '7'))
                        अवरोध;
                      c = *s++;
                      FALLTHROUGH;
                    हाल '1': हाल '2': हाल '3':
                    हाल '4': हाल '5': हाल '6': हाल '7':
                      c -= '0';
                      अगर ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      अगर ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      अवरोध;
                    हाल '\\': अवरोध;

                    not_an_escape:
                    शेष:  अक्षर_दो ('\\'); अवरोध;
                    }
                }
              अक्षर_दो (c);
            }
          argc--;
          argv++;
          अगर (argc > 0)
            अक्षर_दो (' ');
        }
    }
  अन्यथा
    {
      जबतक (argc > 0)
        {
          ख_माला_दो (argv[0], मानक_निकास);
          argc--;
          argv++;
          अगर (argc > 0)
            अक्षर_दो (' ');
        }
    }

  अगर (display_return)
    अक्षर_दो ('\n');
  वापस निकास_सफल;
}
