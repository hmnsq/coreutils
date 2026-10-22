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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "assure.h"
#অন্তর্ভাব "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "echo"

#define AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#ifndef DEFAULT_ECHO_TO_XPG
গণনা { DEFAULT_ECHO_TO_XPG = false };
#endif

শূন্য
usage (পূর্ণসংখ্যা status)
{
  /* STATUS should always be EXIT_SUCCESS (unlike in most other
     utilities which would call emit_try_help otherwise).  */
  affirm (status == EXIT_SUCCESS);

  printf (_("\
Usage: %s [SHORT-OPTION]... [STRING]...\n\
  or:  %s LONG-OPTION\n\
"), program_name, program_name);
  fputs (_("\
Echo the STRING(s) to standard output.\n\
\n\
"), stdout);
  oputs (_("\
  -n     করো not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -e     enable interpretation of backslash escapes (পূর্বনির্ধারিত)\n\
")
           : N_("\
  -e     enable interpretation of backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -E     disable interpretation of backslash escapes\n\
")
           : N_("\
  -E     disable interpretation of backslash escapes (পূর্বনির্ধারিত)\n\
")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (VERSION_OPTION_DESCRIPTION);
  fputs (_("\
\n\
If -e is in effect, the following sequences are recognized:\n\
\n\
"), stdout);
  fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new সারি\n\
  \\r      carriage ফেরত\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal মান NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal মান HH (1 to 2 digits)\n\
"), stdout);
  printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
  fputs (_("\n\
Consider using the printf(1) command instead,\n\
as it avoids problems when outputting option-like strings.\n\
"), stdout);
  emit_ancillary_info (PROGRAM_NAME);
  exit (status);
}

/* Convert C from hexadecimal character to integer.  */
স্থির পূর্ণসংখ্যা
hextobin (unsigned অক্ষর c)
{
  নির্বাচন (c)
    {
    পূর্বনির্ধারিত: ফেরত c - '0';
    ক্ষেত্র 'a': ক্ষেত্র 'A': ফেরত 10;
    ক্ষেত্র 'b': ক্ষেত্র 'B': ফেরত 11;
    ক্ষেত্র 'c': ক্ষেত্র 'C': ফেরত 12;
    ক্ষেত্র 'd': ক্ষেত্র 'D': ফেরত 13;
    ক্ষেত্র 'e': ক্ষেত্র 'E': ফেরত 14;
    ক্ষেত্র 'f': ক্ষেত্র 'F': ফেরত 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool display_return = true;
  bool posixly_correct = !!getenv ("POSIXLY_CORRECT");
  bool allow_options =
    (! posixly_correct
     || (! DEFAULT_ECHO_TO_XPG && 1 < argc && streq (argv[1], "-n")));

  /* System V machines already have a /bin/sh with a v9 behavior.
     Use the identical behavior for these machines so that the
     existing system shell scripts won't barf.  */
  bool do_v9 = DEFAULT_ECHO_TO_XPG;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  যদি (allow_options && argc == 2)
    {
      যদি (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      যদি (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (অক্ষর *) ফাঁকা);
          ফেরত EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  যদি (allow_options)
    যখন (argc > 0 && *argv[0] == '-')
      {
        অক্ষর ধ্রুবক *অস্থায়ী = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        জন্য (i = 0; অস্থায়ী[i]; i++)
          নির্বাচন (অস্থায়ী[i])
            {
            ক্ষেত্র 'e': ক্ষেত্র 'E': ক্ষেত্র 'n':
              বিরতি;
            পূর্বনির্ধারিত:
              যাও just_echo;
            }

        যদি (i == 0)
          যাও just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        যখন (*অস্থায়ী)
          নির্বাচন (*অস্থায়ী++)
            {
            ক্ষেত্র 'e':
              do_v9 = true;
              বিরতি;

            ক্ষেত্র 'E':
              do_v9 = false;
              বিরতি;

            ক্ষেত্র 'n':
              display_return = false;
              বিরতি;
            }

        argc--;
        argv++;
      }

just_echo:

  যদি (do_v9 || posixly_correct)
    {
      যখন (argc > 0)
        {
          অক্ষর ধ্রুবক *s = argv[0];
          unsigned অক্ষর c;

          যখন ((c = *s++))
            {
              যদি (c == '\\' && *s)
                {
                  নির্বাচন (c = *s++)
                    {
                    ক্ষেত্র 'a': c = '\a'; বিরতি;
                    ক্ষেত্র 'b': c = '\b'; বিরতি;
                    ক্ষেত্র 'c': ফেরত EXIT_SUCCESS;
                    ক্ষেত্র 'e': c = '\x1B'; বিরতি;
                    ক্ষেত্র 'f': c = '\f'; বিরতি;
                    ক্ষেত্র 'n': c = '\n'; বিরতি;
                    ক্ষেত্র 'r': c = '\r'; বিরতি;
                    ক্ষেত্র 't': c = '\t'; বিরতি;
                    ক্ষেত্র 'v': c = '\v'; বিরতি;
                    ক্ষেত্র 'x':
                      {
                        unsigned অক্ষর ch = *s;
                        যদি (! c_isxdigit (ch))
                          যাও not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        যদি (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      বিরতি;
                    ক্ষেত্র '0':
                      c = 0;
                      যদি (! ('0' <= *s && *s <= '7'))
                        বিরতি;
                      c = *s++;
                      FALLTHROUGH;
                    ক্ষেত্র '1': ক্ষেত্র '2': ক্ষেত্র '3':
                    ক্ষেত্র '4': ক্ষেত্র '5': ক্ষেত্র '6': ক্ষেত্র '7':
                      c -= '0';
                      যদি ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      যদি ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      বিরতি;
                    ক্ষেত্র '\\': বিরতি;

                    not_an_escape:
                    পূর্বনির্ধারিত:  putchar ('\\'); বিরতি;
                    }
                }
              putchar (c);
            }
          argc--;
          argv++;
          যদি (argc > 0)
            putchar (' ');
        }
    }
  অন্যথা
    {
      যখন (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          যদি (argc > 0)
            putchar (' ');
        }
    }

  যদি (display_return)
    putchar ('\n');
  ফেরত EXIT_SUCCESS;
}
