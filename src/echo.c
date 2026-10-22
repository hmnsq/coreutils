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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن "system.h"
#ضمن "assure.h"
#ضمن "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "echo"

#define AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#ifndef DEFAULT_ECHO_TO_XPG
تعداد { DEFAULT_ECHO_TO_XPG = false };
#endif

فراغ
usage (صحيح status)
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
  -n     افعل not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -e     enable interpretation of backslash escapes (افتراضي)\n\
")
           : N_("\
  -e     enable interpretation of backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -E     disable interpretation of backslash escapes\n\
")
           : N_("\
  -E     disable interpretation of backslash escapes (افتراضي)\n\
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
  \\n      new سطر\n\
  \\r      carriage إرجاع\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal قيمة NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal قيمة HH (1 to 2 digits)\n\
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
ساكن صحيح
hextobin (unsigned حرف c)
{
  بدل (c)
    {
    افتراضي: إرجاع c - '0';
    حالة 'a': حالة 'A': إرجاع 10;
    حالة 'b': حالة 'B': إرجاع 11;
    حالة 'c': حالة 'C': إرجاع 12;
    حالة 'd': حالة 'D': إرجاع 13;
    حالة 'e': حالة 'E': إرجاع 14;
    حالة 'f': حالة 'F': إرجاع 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

صحيح
الرئيسية (صحيح argc, حرف **argv)
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
  إذا (allow_options && argc == 2)
    {
      إذا (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      إذا (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (حرف *) عدم);
          إرجاع EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  إذا (allow_options)
    بينما (argc > 0 && *argv[0] == '-')
      {
        حرف ثابت *temp = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        لكل (i = 0; temp[i]; i++)
          بدل (temp[i])
            {
            حالة 'e': حالة 'E': حالة 'n':
              اكسر;
            افتراضي:
              اذهب just_echo;
            }

        إذا (i == 0)
          اذهب just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        بينما (*temp)
          بدل (*temp++)
            {
            حالة 'e':
              do_v9 = true;
              اكسر;

            حالة 'E':
              do_v9 = false;
              اكسر;

            حالة 'n':
              display_return = false;
              اكسر;
            }

        argc--;
        argv++;
      }

just_echo:

  إذا (do_v9 || posixly_correct)
    {
      بينما (argc > 0)
        {
          حرف ثابت *s = argv[0];
          unsigned حرف c;

          بينما ((c = *s++))
            {
              إذا (c == '\\' && *s)
                {
                  بدل (c = *s++)
                    {
                    حالة 'a': c = '\a'; اكسر;
                    حالة 'b': c = '\b'; اكسر;
                    حالة 'c': إرجاع EXIT_SUCCESS;
                    حالة 'e': c = '\x1B'; اكسر;
                    حالة 'f': c = '\f'; اكسر;
                    حالة 'n': c = '\n'; اكسر;
                    حالة 'r': c = '\r'; اكسر;
                    حالة 't': c = '\t'; اكسر;
                    حالة 'v': c = '\v'; اكسر;
                    حالة 'x':
                      {
                        unsigned حرف ch = *s;
                        إذا (! c_isxdigit (ch))
                          اذهب not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        إذا (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      اكسر;
                    حالة '0':
                      c = 0;
                      إذا (! ('0' <= *s && *s <= '7'))
                        اكسر;
                      c = *s++;
                      FALLTHROUGH;
                    حالة '1': حالة '2': حالة '3':
                    حالة '4': حالة '5': حالة '6': حالة '7':
                      c -= '0';
                      إذا ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      إذا ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      اكسر;
                    حالة '\\': اكسر;

                    not_an_escape:
                    افتراضي:  putchar ('\\'); اكسر;
                    }
                }
              putchar (c);
            }
          argc--;
          argv++;
          إذا (argc > 0)
            putchar (' ');
        }
    }
  وإلا
    {
      بينما (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          إذا (argc > 0)
            putchar (' ');
        }
    }

  إذا (display_return)
    putchar ('\n');
  إرجاع EXIT_SUCCESS;
}
