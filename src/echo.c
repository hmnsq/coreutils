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
usage (पूर्णाङ्क status)
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
  -n     गर not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -e     enable interpretation of backslash escapes (स्वभाव)\n\
")
           : N_("\
  -e     enable interpretation of backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -E     disable interpretation of backslash escapes\n\
")
           : N_("\
  -E     disable interpretation of backslash escapes (स्वभाव)\n\
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
  \\n      new पङ्क्ति\n\
  \\r      carriage फर्काऊ\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), मानक_निकास);
  ख_माला_दो (_("\
  \\0NNN   byte with octal मान NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal मान HH (1 to 2 digits)\n\
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
स्थिर पूर्णाङ्क
hextobin (अचिन्हित वर्ण c)
{
  विकल्प (c)
    {
    स्वभाव: फर्काऊ c - '0';
    पक्ष 'a': पक्ष 'A': फर्काऊ 10;
    पक्ष 'b': पक्ष 'B': फर्काऊ 11;
    पक्ष 'c': पक्ष 'C': फर्काऊ 12;
    पक्ष 'd': पक्ष 'D': फर्काऊ 13;
    पक्ष 'e': पक्ष 'E': फर्काऊ 14;
    पक्ष 'f': पक्ष 'F': फर्काऊ 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
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
  यदि (allow_options && argc == 2)
    {
      यदि (streq (argv[1], "--help"))
        usage (निकास_सफल);

      यदि (streq (argv[1], "--version"))
        {
          version_etc (मानक_निकास, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (वर्ण *) रिक्त);
          फर्काऊ निकास_सफल;
        }
    }

  --argc;
  ++argv;

  यदि (allow_options)
    जबसम्म (argc > 0 && *argv[0] == '-')
      {
        वर्ण अचर *temp = argv[0] + 1;
        माप_प्रकार i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        लागि (i = 0; temp[i]; i++)
          विकल्प (temp[i])
            {
            पक्ष 'e': पक्ष 'E': पक्ष 'n':
              रोक;
            स्वभाव:
              गच्छ just_echo;
            }

        यदि (i == 0)
          गच्छ just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        जबसम्म (*temp)
          विकल्प (*temp++)
            {
            पक्ष 'e':
              do_v9 = true;
              रोक;

            पक्ष 'E':
              do_v9 = false;
              रोक;

            पक्ष 'n':
              display_return = false;
              रोक;
            }

        argc--;
        argv++;
      }

just_echo:

  यदि (do_v9 || posixly_correct)
    {
      जबसम्म (argc > 0)
        {
          वर्ण अचर *s = argv[0];
          अचिन्हित वर्ण c;

          जबसम्म ((c = *s++))
            {
              यदि (c == '\\' && *s)
                {
                  विकल्प (c = *s++)
                    {
                    पक्ष 'a': c = '\a'; रोक;
                    पक्ष 'b': c = '\b'; रोक;
                    पक्ष 'c': फर्काऊ निकास_सफल;
                    पक्ष 'e': c = '\x1B'; रोक;
                    पक्ष 'f': c = '\f'; रोक;
                    पक्ष 'n': c = '\n'; रोक;
                    पक्ष 'r': c = '\r'; रोक;
                    पक्ष 't': c = '\t'; रोक;
                    पक्ष 'v': c = '\v'; रोक;
                    पक्ष 'x':
                      {
                        अचिन्हित वर्ण ch = *s;
                        यदि (! c_isxdigit (ch))
                          गच्छ not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        यदि (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      रोक;
                    पक्ष '0':
                      c = 0;
                      यदि (! ('0' <= *s && *s <= '7'))
                        रोक;
                      c = *s++;
                      FALLTHROUGH;
                    पक्ष '1': पक्ष '2': पक्ष '3':
                    पक्ष '4': पक्ष '5': पक्ष '6': पक्ष '7':
                      c -= '0';
                      यदि ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      यदि ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      रोक;
                    पक्ष '\\': रोक;

                    not_an_escape:
                    स्वभाव:  अक्षर_दो ('\\'); रोक;
                    }
                }
              अक्षर_दो (c);
            }
          argc--;
          argv++;
          यदि (argc > 0)
            अक्षर_दो (' ');
        }
    }
  अन्यथा
    {
      जबसम्म (argc > 0)
        {
          ख_माला_दो (argv[0], मानक_निकास);
          argc--;
          argv++;
          यदि (argc > 0)
            अक्षर_दो (' ');
        }
    }

  यदि (display_return)
    अक्षर_दो ('\n');
  फर्काऊ निकास_सफल;
}
