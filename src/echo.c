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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل "system.h"
#شامل "assure.h"
#شامل "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "echo"

#define AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#ifndef DEFAULT_ECHO_TO_XPG
شمارش { DEFAULT_ECHO_TO_XPG = false };
#endif

تهی
usage (صحیح status)
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
  -n     انجامبده not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -e     enable interpretation of backslash escapes (پیشفرض)\n\
")
           : N_("\
  -e     enable interpretation of backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -E     disable interpretation of backslash escapes\n\
")
           : N_("\
  -E     disable interpretation of backslash escapes (پیشفرض)\n\
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
  \\r      carriage بازگشت\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal مقدار NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal مقدار HH (1 to 2 digits)\n\
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
ایستا صحیح
hextobin (unsigned نویسه c)
{
  انتخاب (c)
    {
    پیشفرض: بازگشت c - '0';
    حالت 'a': حالت 'A': بازگشت 10;
    حالت 'b': حالت 'B': بازگشت 11;
    حالت 'c': حالت 'C': بازگشت 12;
    حالت 'd': حالت 'D': بازگشت 13;
    حالت 'e': حالت 'E': بازگشت 14;
    حالت 'f': حالت 'F': بازگشت 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

صحیح
اصلی (صحیح argc, نویسه **argv)
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
  اگر (allow_options && argc == 2)
    {
      اگر (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      اگر (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (نویسه *) پوچ);
          بازگشت EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  اگر (allow_options)
    وقتی (argc > 0 && *argv[0] == '-')
      {
        نویسه ثابت *temp = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        برای (i = 0; temp[i]; i++)
          انتخاب (temp[i])
            {
            حالت 'e': حالت 'E': حالت 'n':
              بشکن;
            پیشفرض:
              برو just_echo;
            }

        اگر (i == 0)
          برو just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        وقتی (*temp)
          انتخاب (*temp++)
            {
            حالت 'e':
              do_v9 = true;
              بشکن;

            حالت 'E':
              do_v9 = false;
              بشکن;

            حالت 'n':
              display_return = false;
              بشکن;
            }

        argc--;
        argv++;
      }

just_echo:

  اگر (do_v9 || posixly_correct)
    {
      وقتی (argc > 0)
        {
          نویسه ثابت *s = argv[0];
          unsigned نویسه c;

          وقتی ((c = *s++))
            {
              اگر (c == '\\' && *s)
                {
                  انتخاب (c = *s++)
                    {
                    حالت 'a': c = '\a'; بشکن;
                    حالت 'b': c = '\b'; بشکن;
                    حالت 'c': بازگشت EXIT_SUCCESS;
                    حالت 'e': c = '\x1B'; بشکن;
                    حالت 'f': c = '\f'; بشکن;
                    حالت 'n': c = '\n'; بشکن;
                    حالت 'r': c = '\r'; بشکن;
                    حالت 't': c = '\t'; بشکن;
                    حالت 'v': c = '\v'; بشکن;
                    حالت 'x':
                      {
                        unsigned نویسه ch = *s;
                        اگر (! c_isxdigit (ch))
                          برو not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        اگر (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      بشکن;
                    حالت '0':
                      c = 0;
                      اگر (! ('0' <= *s && *s <= '7'))
                        بشکن;
                      c = *s++;
                      FALLTHROUGH;
                    حالت '1': حالت '2': حالت '3':
                    حالت '4': حالت '5': حالت '6': حالت '7':
                      c -= '0';
                      اگر ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      اگر ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      بشکن;
                    حالت '\\': بشکن;

                    not_an_escape:
                    پیشفرض:  putchar ('\\'); بشکن;
                    }
                }
              putchar (c);
            }
          argc--;
          argv++;
          اگر (argc > 0)
            putchar (' ');
        }
    }
  وگرنه
    {
      وقتی (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          اگر (argc > 0)
            putchar (' ');
        }
    }

  اگر (display_return)
    putchar ('\n');
  بازگشت EXIT_SUCCESS;
}
