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
#تعریف_کرو پروگرام_NAME "echo"

#تعریف_کرو AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#اگر_غیر_تعریف_شدہ DEFAULT_ECHO_مقصد_XPG
شمار { DEFAULT_ECHO_مقصد_XPG = false };
#اگر_ختم

خالی
usage (صحیح حالت_2)
{
  /* STATUS should always be EXIT_SUCCESS (unlike in most other
     utilities which would call emit_try_help otherwise).  */
  affirm (حالت_2 == EXIT_SUCCESS);

  لکھو (_("\
Usage: %s [SHORT-OPTION]... [STRING]...\n\
  or:  %s LONG-OPTION\n\
"), پروگرام_name, پروگرام_name);
  fputs (_("\
Echo the STRING(s) مقصد standard output.\n\
\n\
"), stdout);
  oputs (_("\
  -n     کرو not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_مقصد_XPG
           ? N_("\
  -e     enable interpretation کا backslash escapes (طےشدہ)\n\
")
           : N_("\
  -e     enable interpretation کا backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_مقصد_XPG
           ? N_("\
  -E     disable interpretation کا backslash escapes\n\
")
           : N_("\
  -E     disable interpretation کا backslash escapes (طےشدہ)\n\
")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (نسخہ_OPTION_DESCRIPTION);
  fputs (_("\
\n\
If -e is اندر effect, the following sequences are recognized:\n\
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
  \\r      carriage واپس\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal قدر NNN (1 مقصد 3 digits)\n\
  \\xHH    byte with hexadecimal قدر HH (1 مقصد 2 digits)\n\
"), stdout);
  لکھو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
  fputs (_("\n\
Consider using the لکھو(1) command instead,\n\
as it avoids problems when outputting option-like لڑیاں.\n\
"), stdout);
  emit_ancillary_info (پروگرام_NAME);
  باہر_نکلو (حالت_2);
}

/* Convert C from hexadecimal character to integer.  */
ساکن صحیح
hextobin (غیرعلامتی حرف c)
{
  انتخاب (c)
    {
    طےشدہ: واپس c - '0';
    حالت 'a': حالت 'A': واپس 10;
    حالت 'b': حالت 'B': واپس 11;
    حالت 'c': حالت 'C': واپس 12;
    حالت 'd': حالت 'D': واپس 13;
    حالت 'e': حالت 'E': واپس 14;
    حالت 'f': حالت 'F': واپس 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

صحیح
مین (صحیح argc, حرف **argv)
{
  bool display_return = true;
  bool posixly_correct = !!getenv ("POSIXLY_CORRECT");
  bool allow_options =
    (! posixly_correct
     || (! DEFAULT_ECHO_مقصد_XPG && 1 < argc && streq (argv[1], "-n")));

  /* System V machines already have a /bin/sh with a v9 behavior.
     Use the identical behavior for these machines so that the
     existing system shell scripts won't barf.  */
  bool do_v9 = DEFAULT_ECHO_مقصد_XPG;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
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
          نسخہ_etc (stdout, پروگرام_NAME, PACKAGE_NAME, نسخہ, AUTHORS,
                       (حرف *) تہی);
          واپس EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  اگر (allow_options)
    جب (argc > 0 && *argv[0] == '-')
      {
        حرف ثابت *عارضی = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        برائے (i = 0; عارضی[i]; i++)
          انتخاب (عارضی[i])
            {
            حالت 'e': حالت 'E': حالت 'n':
              توڑو;
            طےشدہ:
              جاؤ just_echo;
            }

        اگر (i == 0)
          جاؤ just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        جب (*عارضی)
          انتخاب (*عارضی++)
            {
            حالت 'e':
              do_v9 = true;
              توڑو;

            حالت 'E':
              do_v9 = false;
              توڑو;

            حالت 'n':
              display_return = false;
              توڑو;
            }

        argc--;
        argv++;
      }

just_echo:

  اگر (do_v9 || posixly_correct)
    {
      جب (argc > 0)
        {
          حرف ثابت *s = argv[0];
          غیرعلامتی حرف c;

          جب ((c = *s++))
            {
              اگر (c == '\\' && *s)
                {
                  انتخاب (c = *s++)
                    {
                    حالت 'a': c = '\a'; توڑو;
                    حالت 'b': c = '\b'; توڑو;
                    حالت 'c': واپس EXIT_SUCCESS;
                    حالت 'e': c = '\x1B'; توڑو;
                    حالت 'f': c = '\f'; توڑو;
                    حالت 'n': c = '\n'; توڑو;
                    حالت 'r': c = '\r'; توڑو;
                    حالت 't': c = '\t'; توڑو;
                    حالت 'v': c = '\v'; توڑو;
                    حالت 'x':
                      {
                        غیرعلامتی حرف ch = *s;
                        اگر (! c_isxdigit (ch))
                          جاؤ not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        اگر (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      توڑو;
                    حالت '0':
                      c = 0;
                      اگر (! ('0' <= *s && *s <= '7'))
                        توڑو;
                      c = *s++;
                      FALLTHROUGH;
                    حالت '1': حالت '2': حالت '3':
                    حالت '4': حالت '5': حالت '6': حالت '7':
                      c -= '0';
                      اگر ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      اگر ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      توڑو;
                    حالت '\\': توڑو;

                    not_an_escape:
                    طےشدہ:  حرف_لکھو ('\\'); توڑو;
                    }
                }
              حرف_لکھو (c);
            }
          argc--;
          argv++;
          اگر (argc > 0)
            حرف_لکھو (' ');
        }
    }
  ورنہ
    {
      جب (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          اگر (argc > 0)
            حرف_لکھو (' ');
        }
    }

  اگر (display_return)
    حرف_لکھو ('\n');
  واپس EXIT_SUCCESS;
}
