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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "echo"

#define AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#ifndef DEFAULT_ECHO_TO_XPG
ಗಣನಾ { DEFAULT_ECHO_TO_XPG = false };
#endif

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
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
  -n     ಕುರು not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -e     enable interpretation of backslash escapes (ಸ್ವಭಾವ)\n\
")
           : N_("\
  -e     enable interpretation of backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -E     disable interpretation of backslash escapes\n\
")
           : N_("\
  -E     disable interpretation of backslash escapes (ಸ್ವಭಾವ)\n\
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
  \\n      new ಸಾಲು\n\
  \\r      carriage ಪ್ರತ್ಯಾಗಮ\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal ಮೌಲ್ಯ NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal ಮೌಲ್ಯ HH (1 to 2 digits)\n\
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
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
hextobin (unsigned ವರ್ಣ c)
{
  ವಿಕಲ್ಪ (c)
    {
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ c - '0';
    ಪಕ್ಷ 'a': ಪಕ್ಷ 'A': ಪ್ರತ್ಯಾಗಮ 10;
    ಪಕ್ಷ 'b': ಪಕ್ಷ 'B': ಪ್ರತ್ಯಾಗಮ 11;
    ಪಕ್ಷ 'c': ಪಕ್ಷ 'C': ಪ್ರತ್ಯಾಗಮ 12;
    ಪಕ್ಷ 'd': ಪಕ್ಷ 'D': ಪ್ರತ್ಯಾಗಮ 13;
    ಪಕ್ಷ 'e': ಪಕ್ಷ 'E': ಪ್ರತ್ಯಾಗಮ 14;
    ಪಕ್ಷ 'f': ಪಕ್ಷ 'F': ಪ್ರತ್ಯಾಗಮ 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
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
  ಯದಿ (allow_options && argc == 2)
    {
      ಯದಿ (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      ಯದಿ (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (ವರ್ಣ *) ರಿಕ್ತ);
          ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  ಯದಿ (allow_options)
    ಯಾವತ್ (argc > 0 && *argv[0] == '-')
      {
        ವರ್ಣ ಧ್ರುವ *temp = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        ಪ್ರತಿ (i = 0; temp[i]; i++)
          ವಿಕಲ್ಪ (temp[i])
            {
            ಪಕ್ಷ 'e': ಪಕ್ಷ 'E': ಪಕ್ಷ 'n':
              ಭಙ್ಗ;
            ಸ್ವಭಾವ:
              ಗಚ್ಛ just_echo;
            }

        ಯದಿ (i == 0)
          ಗಚ್ಛ just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        ಯಾವತ್ (*temp)
          ವಿಕಲ್ಪ (*temp++)
            {
            ಪಕ್ಷ 'e':
              do_v9 = true;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'E':
              do_v9 = false;
              ಭಙ್ಗ;

            ಪಕ್ಷ 'n':
              display_return = false;
              ಭಙ್ಗ;
            }

        argc--;
        argv++;
      }

just_echo:

  ಯದಿ (do_v9 || posixly_correct)
    {
      ಯಾವತ್ (argc > 0)
        {
          ವರ್ಣ ಧ್ರುವ *s = argv[0];
          unsigned ವರ್ಣ c;

          ಯಾವತ್ ((c = *s++))
            {
              ಯದಿ (c == '\\' && *s)
                {
                  ವಿಕಲ್ಪ (c = *s++)
                    {
                    ಪಕ್ಷ 'a': c = '\a'; ಭಙ್ಗ;
                    ಪಕ್ಷ 'b': c = '\b'; ಭಙ್ಗ;
                    ಪಕ್ಷ 'c': ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
                    ಪಕ್ಷ 'e': c = '\x1B'; ಭಙ್ಗ;
                    ಪಕ್ಷ 'f': c = '\f'; ಭಙ್ಗ;
                    ಪಕ್ಷ 'n': c = '\n'; ಭಙ್ಗ;
                    ಪಕ್ಷ 'r': c = '\r'; ಭಙ್ಗ;
                    ಪಕ್ಷ 't': c = '\t'; ಭಙ್ಗ;
                    ಪಕ್ಷ 'v': c = '\v'; ಭಙ್ಗ;
                    ಪಕ್ಷ 'x':
                      {
                        unsigned ವರ್ಣ ch = *s;
                        ಯದಿ (! c_isxdigit (ch))
                          ಗಚ್ಛ not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        ಯದಿ (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      ಭಙ್ಗ;
                    ಪಕ್ಷ '0':
                      c = 0;
                      ಯದಿ (! ('0' <= *s && *s <= '7'))
                        ಭಙ್ಗ;
                      c = *s++;
                      FALLTHROUGH;
                    ಪಕ್ಷ '1': ಪಕ್ಷ '2': ಪಕ್ಷ '3':
                    ಪಕ್ಷ '4': ಪಕ್ಷ '5': ಪಕ್ಷ '6': ಪಕ್ಷ '7':
                      c -= '0';
                      ಯದಿ ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      ಯದಿ ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      ಭಙ್ಗ;
                    ಪಕ್ಷ '\\': ಭಙ್ಗ;

                    not_an_escape:
                    ಸ್ವಭಾವ:  putchar ('\\'); ಭಙ್ಗ;
                    }
                }
              putchar (c);
            }
          argc--;
          argv++;
          ಯದಿ (argc > 0)
            putchar (' ');
        }
    }
  ಅನ್ಯಥಾ
    {
      ಯಾವತ್ (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          ಯದಿ (argc > 0)
            putchar (' ');
        }
    }

  ಯದಿ (display_return)
    putchar ('\n');
  ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
}
