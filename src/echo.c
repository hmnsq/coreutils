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
#تعریف_کړه پروګرام_NAME "echo"

#تعریف_کړه AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#که_نه_تعریف_شوی DEFAULT_ECHO_منزل_XPG
تعداد { DEFAULT_ECHO_منزل_XPG = false };
#د_که_پای

فراغ
usage (صحيح حالت)
{
  /* STATUS should always be EXIT_SUCCESS (unlike in most other
     utilities which would call emit_try_help otherwise).  */
  affirm (حالت == EXIT_SUCCESS);

  چاپ_کړه (_("\
Usage: %s [SHORT-OPTION]... [STRING]...\n\
  or:  %s LONG-OPTION\n\
"), پروګرام_name, پروګرام_name);
  fputs (_("\
Echo the STRING(s) منزل standard output.\n\
\n\
"), stdout);
  oputs (_("\
  -n     افعل not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_منزل_XPG
           ? N_("\
  -e     enable interpretation د backslash escapes (افتراضي)\n\
")
           : N_("\
  -e     enable interpretation د backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_منزل_XPG
           ? N_("\
  -E     disable interpretation د backslash escapes\n\
")
           : N_("\
  -E     disable interpretation د backslash escapes (افتراضي)\n\
")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (نسخه_OPTION_DESCRIPTION);
  fputs (_("\
\n\
If -e is دننه effect, the following sequences are recognized:\n\
\n\
"), stdout);
  fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new کرښه\n\
  \\r      carriage أرجع\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal ارزښت NNN (1 منزل 3 digits)\n\
  \\xHH    byte with hexadecimal ارزښت HH (1 منزل 2 digits)\n\
"), stdout);
  چاپ_کړه (USAGE_BUILTIN_WARNING, پروګرام_NAME);
  fputs (_("\n\
Consider using the چاپ_کړه(1) command instead,\n\
as it avoids problems when outputting option-like مزي_ګانې.\n\
"), stdout);
  emit_ancillary_info (پروګرام_NAME);
  وتل (حالت);
}

/* Convert C from hexadecimal character to integer.  */
ساكن صحيح
hextobin (بینښه حرف c)
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
اصلي (صحيح argc, حرف **argv)
{
  bool display_return = true;
  bool posixly_correct = !!getenv ("POSIXLY_CORRECT");
  bool allow_options =
    (! posixly_correct
     || (! DEFAULT_ECHO_منزل_XPG && 1 < argc && streq (argv[1], "-n")));

  /* System V machines already have a /bin/sh with a v9 behavior.
     Use the identical behavior for these machines so that the
     existing system shell scripts won't barf.  */
  bool do_v9 = DEFAULT_ECHO_منزل_XPG;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
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
          نسخه_etc (stdout, پروګرام_NAME, PACKAGE_NAME, نسخه, AUTHORS,
                       (حرف *) عدم);
          أرجع EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  إذا (allow_options)
    طالما (argc > 0 && *argv[0] == '-')
      {
        حرف ثابت *لنډمهاله = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        لكل (i = 0; لنډمهاله[i]; i++)
          بدل (لنډمهاله[i])
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
        طالما (*لنډمهاله)
          بدل (*لنډمهاله++)
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
          بینښه حرف c;

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
                        بینښه حرف ch = *s;
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
                    افتراضي:  توری_چاپ_کړه ('\\'); اكسر;
                    }
                }
              توری_چاپ_کړه (c);
            }
          argc--;
          argv++;
          إذا (argc > 0)
            توری_چاپ_کړه (' ');
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
            توری_چاپ_کړه (' ');
        }
    }

  إذا (display_return)
    توری_چاپ_کړه ('\n');
  أرجع EXIT_SUCCESS;
}
