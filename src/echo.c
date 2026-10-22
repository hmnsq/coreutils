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
#परिभाषय क्रमादेशः_NAME "echo"

#परिभाषय AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#यदि_अपरिभाषितम् DEFAULT_ECHO_गन्तव्यम्_XPG
गणना { DEFAULT_ECHO_गन्तव्यम्_XPG = false };
#यदि_अन्तः

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  /* STATUS should always be EXIT_SUCCESS (unlike in most other
     utilities which would call emit_try_help otherwise).  */
  affirm (स्थितिः == EXIT_SUCCESS);

  प्रारूप_लिख (_("\
Usage: %s [SHORT-OPTION]... [STRING]...\n\
  or:  %s LONG-OPTION\n\
"), क्रमादेशः_name, क्रमादेशः_name);
  fputs (_("\
Echo the STRING(s) गन्तव्यम् standard output.\n\
\n\
"), stdout);
  oputs (_("\
  -n     कुरु not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_गन्तव्यम्_XPG
           ? N_("\
  -e     enable interpretation सम्बन्धि backslash escapes (स्वभाव)\n\
")
           : N_("\
  -e     enable interpretation सम्बन्धि backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_गन्तव्यम्_XPG
           ? N_("\
  -E     disable interpretation सम्बन्धि backslash escapes\n\
")
           : N_("\
  -E     disable interpretation सम्बन्धि backslash escapes (स्वभाव)\n\
")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (संस्करणम्_OPTION_DESCRIPTION);
  fputs (_("\
\n\
If -e is अन्तर् effect, the following sequences are recognized:\n\
\n\
"), stdout);
  fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new पङ्क्तिः\n\
  \\r      carriage प्रत्यागम\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal मानम् NNN (1 गन्तव्यम् 3 digits)\n\
  \\xHH    byte with hexadecimal मानम् HH (1 गन्तव्यम् 2 digits)\n\
"), stdout);
  प्रारूप_लिख (USAGE_BUILTIN_WARNING, क्रमादेशः_NAME);
  fputs (_("\n\
Consider using the प्रारूप_लिख(1) command instead,\n\
as it avoids problems when outputting option-like वाक्यानि.\n\
"), stdout);
  emit_ancillary_info (क्रमादेशः_NAME);
  निर्गच्छ (स्थितिः);
}

/* Convert C from hexadecimal character to integer.  */
स्थिर पूर्णाङ्क
hextobin (अचिह्नित वर्ण c)
{
  विकल्प (c)
    {
    स्वभाव: प्रत्यागम c - '0';
    पक्ष 'a': पक्ष 'A': प्रत्यागम 10;
    पक्ष 'b': पक्ष 'B': प्रत्यागम 11;
    पक्ष 'c': पक्ष 'C': प्रत्यागम 12;
    पक्ष 'd': पक्ष 'D': प्रत्यागम 13;
    पक्ष 'e': पक्ष 'E': प्रत्यागम 14;
    पक्ष 'f': पक्ष 'F': प्रत्यागम 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool display_return = true;
  bool posixly_correct = !!getenv ("POSIXLY_CORRECT");
  bool allow_options =
    (! posixly_correct
     || (! DEFAULT_ECHO_गन्तव्यम्_XPG && 1 < argc && streq (argv[1], "-n")));

  /* System V machines already have a /bin/sh with a v9 behavior.
     Use the identical behavior for these machines so that the
     existing system shell scripts won't barf.  */
  bool do_v9 = DEFAULT_ECHO_गन्तव्यम्_XPG;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  यदि (allow_options && argc == 2)
    {
      यदि (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      यदि (streq (argv[1], "--version"))
        {
          संस्करणम्_etc (stdout, क्रमादेशः_NAME, PACKAGE_NAME, संस्करणम्, AUTHORS,
                       (वर्ण *) रिक्त);
          प्रत्यागम EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  यदि (allow_options)
    यावत् (argc > 0 && *argv[0] == '-')
      {
        वर्ण ध्रुव *अस्थायी = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        प्रति (i = 0; अस्थायी[i]; i++)
          विकल्प (अस्थायी[i])
            {
            पक्ष 'e': पक्ष 'E': पक्ष 'n':
              भङ्ग;
            स्वभाव:
              गच्छ just_echo;
            }

        यदि (i == 0)
          गच्छ just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        यावत् (*अस्थायी)
          विकल्प (*अस्थायी++)
            {
            पक्ष 'e':
              do_v9 = true;
              भङ्ग;

            पक्ष 'E':
              do_v9 = false;
              भङ्ग;

            पक्ष 'n':
              display_return = false;
              भङ्ग;
            }

        argc--;
        argv++;
      }

just_echo:

  यदि (do_v9 || posixly_correct)
    {
      यावत् (argc > 0)
        {
          वर्ण ध्रुव *s = argv[0];
          अचिह्नित वर्ण c;

          यावत् ((c = *s++))
            {
              यदि (c == '\\' && *s)
                {
                  विकल्प (c = *s++)
                    {
                    पक्ष 'a': c = '\a'; भङ्ग;
                    पक्ष 'b': c = '\b'; भङ्ग;
                    पक्ष 'c': प्रत्यागम EXIT_SUCCESS;
                    पक्ष 'e': c = '\x1B'; भङ्ग;
                    पक्ष 'f': c = '\f'; भङ्ग;
                    पक्ष 'n': c = '\n'; भङ्ग;
                    पक्ष 'r': c = '\r'; भङ्ग;
                    पक्ष 't': c = '\t'; भङ्ग;
                    पक्ष 'v': c = '\v'; भङ्ग;
                    पक्ष 'x':
                      {
                        अचिह्नित वर्ण ch = *s;
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
                      भङ्ग;
                    पक्ष '0':
                      c = 0;
                      यदि (! ('0' <= *s && *s <= '7'))
                        भङ्ग;
                      c = *s++;
                      FALLTHROUGH;
                    पक्ष '1': पक्ष '2': पक्ष '3':
                    पक्ष '4': पक्ष '5': पक्ष '6': पक्ष '7':
                      c -= '0';
                      यदि ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      यदि ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      भङ्ग;
                    पक्ष '\\': भङ्ग;

                    not_an_escape:
                    स्वभाव:  अक्षरं_लिख ('\\'); भङ्ग;
                    }
                }
              अक्षरं_लिख (c);
            }
          argc--;
          argv++;
          यदि (argc > 0)
            अक्षरं_लिख (' ');
        }
    }
  अन्यथा
    {
      यावत् (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          यदि (argc > 0)
            अक्षरं_लिख (' ');
        }
    }

  यदि (display_return)
    अक्षरं_लिख ('\n');
  प्रत्यागम EXIT_SUCCESS;
}
