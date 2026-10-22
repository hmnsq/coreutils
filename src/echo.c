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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "assure.h"
#അന്തര്ഭാവ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "echo"

#define AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#ifndef DEFAULT_ECHO_TO_XPG
എണ്ണൽ { DEFAULT_ECHO_TO_XPG = false };
#endif

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
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
  -n     ചെയ്യുക not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -e     enable interpretation of backslash escapes (സ്ഥിരസ്ഥിതി)\n\
")
           : N_("\
  -e     enable interpretation of backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -E     disable interpretation of backslash escapes\n\
")
           : N_("\
  -E     disable interpretation of backslash escapes (സ്ഥിരസ്ഥിതി)\n\
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
  \\n      new വരി\n\
  \\r      carriage തിരികെ\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal മൂല്യം NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal മൂല്യം HH (1 to 2 digits)\n\
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
സ്ഥിതം പൂർണ്ണസംഖ്യ
hextobin (unsigned അക്ഷരം c)
{
  തിരഞ്ഞെടുപ്പ് (c)
    {
    സ്ഥിരസ്ഥിതി: തിരികെ c - '0';
    അവസ്ഥ 'a': അവസ്ഥ 'A': തിരികെ 10;
    അവസ്ഥ 'b': അവസ്ഥ 'B': തിരികെ 11;
    അവസ്ഥ 'c': അവസ്ഥ 'C': തിരികെ 12;
    അവസ്ഥ 'd': അവസ്ഥ 'D': തിരികെ 13;
    അവസ്ഥ 'e': അവസ്ഥ 'E': തിരികെ 14;
    അവസ്ഥ 'f': അവസ്ഥ 'F': തിരികെ 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
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
  എങ്കിൽ (allow_options && argc == 2)
    {
      എങ്കിൽ (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      എങ്കിൽ (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (അക്ഷരം *) ശൂന്യ);
          തിരികെ EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  എങ്കിൽ (allow_options)
    ആകുമ്പോൾ (argc > 0 && *argv[0] == '-')
      {
        അക്ഷരം സ്ഥിരം *temp = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        വേണ്ടി (i = 0; temp[i]; i++)
          തിരഞ്ഞെടുപ്പ് (temp[i])
            {
            അവസ്ഥ 'e': അവസ്ഥ 'E': അവസ്ഥ 'n':
              നിർത്തുക;
            സ്ഥിരസ്ഥിതി:
              പോകുക just_echo;
            }

        എങ്കിൽ (i == 0)
          പോകുക just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        ആകുമ്പോൾ (*temp)
          തിരഞ്ഞെടുപ്പ് (*temp++)
            {
            അവസ്ഥ 'e':
              do_v9 = true;
              നിർത്തുക;

            അവസ്ഥ 'E':
              do_v9 = false;
              നിർത്തുക;

            അവസ്ഥ 'n':
              display_return = false;
              നിർത്തുക;
            }

        argc--;
        argv++;
      }

just_echo:

  എങ്കിൽ (do_v9 || posixly_correct)
    {
      ആകുമ്പോൾ (argc > 0)
        {
          അക്ഷരം സ്ഥിരം *s = argv[0];
          unsigned അക്ഷരം c;

          ആകുമ്പോൾ ((c = *s++))
            {
              എങ്കിൽ (c == '\\' && *s)
                {
                  തിരഞ്ഞെടുപ്പ് (c = *s++)
                    {
                    അവസ്ഥ 'a': c = '\a'; നിർത്തുക;
                    അവസ്ഥ 'b': c = '\b'; നിർത്തുക;
                    അവസ്ഥ 'c': തിരികെ EXIT_SUCCESS;
                    അവസ്ഥ 'e': c = '\x1B'; നിർത്തുക;
                    അവസ്ഥ 'f': c = '\f'; നിർത്തുക;
                    അവസ്ഥ 'n': c = '\n'; നിർത്തുക;
                    അവസ്ഥ 'r': c = '\r'; നിർത്തുക;
                    അവസ്ഥ 't': c = '\t'; നിർത്തുക;
                    അവസ്ഥ 'v': c = '\v'; നിർത്തുക;
                    അവസ്ഥ 'x':
                      {
                        unsigned അക്ഷരം ch = *s;
                        എങ്കിൽ (! c_isxdigit (ch))
                          പോകുക not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        എങ്കിൽ (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      നിർത്തുക;
                    അവസ്ഥ '0':
                      c = 0;
                      എങ്കിൽ (! ('0' <= *s && *s <= '7'))
                        നിർത്തുക;
                      c = *s++;
                      FALLTHROUGH;
                    അവസ്ഥ '1': അവസ്ഥ '2': അവസ്ഥ '3':
                    അവസ്ഥ '4': അവസ്ഥ '5': അവസ്ഥ '6': അവസ്ഥ '7':
                      c -= '0';
                      എങ്കിൽ ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      എങ്കിൽ ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      നിർത്തുക;
                    അവസ്ഥ '\\': നിർത്തുക;

                    not_an_escape:
                    സ്ഥിരസ്ഥിതി:  putchar ('\\'); നിർത്തുക;
                    }
                }
              putchar (c);
            }
          argc--;
          argv++;
          എങ്കിൽ (argc > 0)
            putchar (' ');
        }
    }
  അല്ലെങ്കിൽ
    {
      ആകുമ്പോൾ (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          എങ്കിൽ (argc > 0)
            putchar (' ');
        }
    }

  എങ്കിൽ (display_return)
    putchar ('\n');
  തിരികെ EXIT_SUCCESS;
}
