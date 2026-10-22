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
usage (পূর্ণাঙ্ক status)
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
  -n     কুরু not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -e     enable interpretation of backslash escapes (স্বভাব)\n\
")
           : N_("\
  -e     enable interpretation of backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -E     disable interpretation of backslash escapes\n\
")
           : N_("\
  -E     disable interpretation of backslash escapes (স্বভাব)\n\
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
  \\r      carriage প্রত্যাগম\n\
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
স্থির পূর্ণাঙ্ক
hextobin (unsigned বর্ণ c)
{
  বিকল্প (c)
    {
    স্বভাব: প্রত্যাগম c - '0';
    পক্ষ 'a': পক্ষ 'A': প্রত্যাগম 10;
    পক্ষ 'b': পক্ষ 'B': প্রত্যাগম 11;
    পক্ষ 'c': পক্ষ 'C': প্রত্যাগম 12;
    পক্ষ 'd': পক্ষ 'D': প্রত্যাগম 13;
    পক্ষ 'e': পক্ষ 'E': প্রত্যাগম 14;
    পক্ষ 'f': পক্ষ 'F': প্রত্যাগম 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
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
                       (বর্ণ *) রিক্ত);
          প্রত্যাগম EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  যদি (allow_options)
    যাবত্ (argc > 0 && *argv[0] == '-')
      {
        বর্ণ ধ্রুব *temp = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        প্রতি (i = 0; temp[i]; i++)
          বিকল্প (temp[i])
            {
            পক্ষ 'e': পক্ষ 'E': পক্ষ 'n':
              ভঙ্গ;
            স্বভাব:
              গচ্ছ just_echo;
            }

        যদি (i == 0)
          গচ্ছ just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        যাবত্ (*temp)
          বিকল্প (*temp++)
            {
            পক্ষ 'e':
              do_v9 = true;
              ভঙ্গ;

            পক্ষ 'E':
              do_v9 = false;
              ভঙ্গ;

            পক্ষ 'n':
              display_return = false;
              ভঙ্গ;
            }

        argc--;
        argv++;
      }

just_echo:

  যদি (do_v9 || posixly_correct)
    {
      যাবত্ (argc > 0)
        {
          বর্ণ ধ্রুব *s = argv[0];
          unsigned বর্ণ c;

          যাবত্ ((c = *s++))
            {
              যদি (c == '\\' && *s)
                {
                  বিকল্প (c = *s++)
                    {
                    পক্ষ 'a': c = '\a'; ভঙ্গ;
                    পক্ষ 'b': c = '\b'; ভঙ্গ;
                    পক্ষ 'c': প্রত্যাগম EXIT_SUCCESS;
                    পক্ষ 'e': c = '\x1B'; ভঙ্গ;
                    পক্ষ 'f': c = '\f'; ভঙ্গ;
                    পক্ষ 'n': c = '\n'; ভঙ্গ;
                    পক্ষ 'r': c = '\r'; ভঙ্গ;
                    পক্ষ 't': c = '\t'; ভঙ্গ;
                    পক্ষ 'v': c = '\v'; ভঙ্গ;
                    পক্ষ 'x':
                      {
                        unsigned বর্ণ ch = *s;
                        যদি (! c_isxdigit (ch))
                          গচ্ছ not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        যদি (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      ভঙ্গ;
                    পক্ষ '0':
                      c = 0;
                      যদি (! ('0' <= *s && *s <= '7'))
                        ভঙ্গ;
                      c = *s++;
                      FALLTHROUGH;
                    পক্ষ '1': পক্ষ '2': পক্ষ '3':
                    পক্ষ '4': পক্ষ '5': পক্ষ '6': পক্ষ '7':
                      c -= '0';
                      যদি ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      যদি ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      ভঙ্গ;
                    পক্ষ '\\': ভঙ্গ;

                    not_an_escape:
                    স্বভাব:  putchar ('\\'); ভঙ্গ;
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
      যাবত্ (argc > 0)
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
  প্রত্যাগম EXIT_SUCCESS;
}
