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
#تعریف_کن برنامه_NAME "echo"

#تعریف_کن AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#اگر_تعریف_نشده DEFAULT_ECHO_مقصد_XPG
تعداد { DEFAULT_ECHO_مقصد_XPG = false };
#پایان_اگر

فراغ
usage (صحيح وضعیت)
{
  /* STATUS should always be EXIT_SUCCESS (unlike in most other
     utilities which would call emit_try_help otherwise).  */
  affirm (وضعیت == EXIT_SUCCESS);

  چاپ_کن (_("\
Usage: %s [SHORT-OPTION]... [STRING]...\n\
  or:  %s LONG-OPTION\n\
"), برنامه_name, برنامه_name);
  fputs (_("\
Echo the STRING(s) مقصد standard output.\n\
\n\
"), stdout);
  oputs (_("\
  -n     افعل not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_مقصد_XPG
           ? N_("\
  -e     enable interpretation از backslash escapes (افتراضي)\n\
")
           : N_("\
  -e     enable interpretation از backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_مقصد_XPG
           ? N_("\
  -E     disable interpretation از backslash escapes\n\
")
           : N_("\
  -E     disable interpretation از backslash escapes (افتراضي)\n\
")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (نسخه_OPTION_DESCRIPTION);
  fputs (_("\
\n\
If -e is درون effect, the following sequences are recognized:\n\
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
  \\r      carriage أرجع\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal مقدار NNN (1 مقصد 3 digits)\n\
  \\xHH    byte with hexadecimal مقدار HH (1 مقصد 2 digits)\n\
"), stdout);
  چاپ_کن (USAGE_BUILTIN_WARNING, برنامه_NAME);
  fputs (_("\n\
Consider using the چاپ_کن(1) command instead,\n\
as it avoids problems when outputting option-like رشتهها.\n\
"), stdout);
  emit_ancillary_info (برنامه_NAME);
  خروج (وضعیت);
}

/* Convert C from hexadecimal character to integer.  */
ساكن صحيح
hextobin (بیعلامت حرف c)
{
  بدل (c)
    {
    افتراضي: أرجع c - '0';
    حالة 'a': حالة 'A': أرجع 10;
    حالة 'b': حالة 'B': أرجع 11;
    حالة 'c': حالة 'C': أرجع 12;
    حالة 'd': حالة 'D': أرجع 13;
    حالة 'e': حالة 'E': أرجع 14;
    حالة 'f': حالة 'F': أرجع 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

صحيح
اصلی (صحيح argc, حرف **argv)
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
  مجموعه_برنامه_name (argv[0]);
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
          نسخه_etc (stdout, برنامه_NAME, PACKAGE_NAME, نسخه, AUTHORS,
                       (حرف *) عدم);
          أرجع EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  إذا (allow_options)
    طالما (argc > 0 && *argv[0] == '-')
      {
        حرف ثابت *موقت = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        لكل (i = 0; موقت[i]; i++)
          بدل (موقت[i])
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
        طالما (*موقت)
          بدل (*موقت++)
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
      طالما (argc > 0)
        {
          حرف ثابت *s = argv[0];
          بیعلامت حرف c;

          طالما ((c = *s++))
            {
              إذا (c == '\\' && *s)
                {
                  بدل (c = *s++)
                    {
                    حالة 'a': c = '\a'; اكسر;
                    حالة 'b': c = '\b'; اكسر;
                    حالة 'c': أرجع EXIT_SUCCESS;
                    حالة 'e': c = '\x1B'; اكسر;
                    حالة 'f': c = '\f'; اكسر;
                    حالة 'n': c = '\n'; اكسر;
                    حالة 'r': c = '\r'; اكسر;
                    حالة 't': c = '\t'; اكسر;
                    حالة 'v': c = '\v'; اكسر;
                    حالة 'x':
                      {
                        بیعلامت حرف ch = *s;
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
                    افتراضي:  حرف_چاپ_کن ('\\'); اكسر;
                    }
                }
              حرف_چاپ_کن (c);
            }
          argc--;
          argv++;
          إذا (argc > 0)
            حرف_چاپ_کن (' ');
        }
    }
  وإلا
    {
      طالما (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          إذا (argc > 0)
            حرف_چاپ_کن (' ');
        }
    }

  إذا (display_return)
    حرف_چاپ_کن ('\n');
  أرجع EXIT_SUCCESS;
}
