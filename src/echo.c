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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "echo"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ DEFAULT_ECHO_ಗಮ್ಯ_XPG
ಎಣಿಕೆ { DEFAULT_ECHO_ಗಮ್ಯ_XPG = false };
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  /* STATUS should always be EXIT_SUCCESS (unlike in most other
     utilities which would call emit_try_help otherwise).  */
  affirm (ಸ್ಥಿತಿ_2 == EXIT_SUCCESS);

  ಬರೆ (_("\
Usage: %s [SHORT-OPTION]... [STRING]...\n\
  or:  %s LONG-OPTION\n\
"), ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
  fputs (_("\
Echo the STRING(s) ಗಮ್ಯ standard output.\n\
\n\
"), stdout);
  oputs (_("\
  -n     ಮಾಡು not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_ಗಮ್ಯ_XPG
           ? N_("\
  -e     enable interpretation ದ backslash escapes (ಪೂರ್ವನಿಯೋಜಿತ)\n\
")
           : N_("\
  -e     enable interpretation ದ backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_ಗಮ್ಯ_XPG
           ? N_("\
  -E     disable interpretation ದ backslash escapes\n\
")
           : N_("\
  -E     disable interpretation ದ backslash escapes (ಪೂರ್ವನಿಯೋಜಿತ)\n\
")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
  fputs (_("\
\n\
If -e is ಒಳ effect, the following sequences are recognized:\n\
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
  \\r      carriage ಹಿಂತಿರುಗಿ\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal ಮೌಲ್ಯ NNN (1 ಗಮ್ಯ 3 digits)\n\
  \\xHH    byte with hexadecimal ಮೌಲ್ಯ HH (1 ಗಮ್ಯ 2 digits)\n\
"), stdout);
  ಬರೆ (USAGE_BUILTIN_WARNING, ಕಾರ್ಯಕ್ರಮ_NAME);
  fputs (_("\n\
Consider using the ಬರೆ(1) command instead,\n\
as it avoids problems when outputting option-like ಅಕ್ಷರಸರಣಿಗಳು.\n\
"), stdout);
  emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Convert C from hexadecimal character to integer.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
hextobin (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c)
{
  ಆಯ್ಕೆ (c)
    {
    ಪೂರ್ವನಿಯೋಜಿತ: ಹಿಂತಿರುಗಿ c - '0';
    ಸ್ಥಿತಿ 'a': ಸ್ಥಿತಿ 'A': ಹಿಂತಿರುಗಿ 10;
    ಸ್ಥಿತಿ 'b': ಸ್ಥಿತಿ 'B': ಹಿಂತಿರುಗಿ 11;
    ಸ್ಥಿತಿ 'c': ಸ್ಥಿತಿ 'C': ಹಿಂತಿರುಗಿ 12;
    ಸ್ಥಿತಿ 'd': ಸ್ಥಿತಿ 'D': ಹಿಂತಿರುಗಿ 13;
    ಸ್ಥಿತಿ 'e': ಸ್ಥಿತಿ 'E': ಹಿಂತಿರುಗಿ 14;
    ಸ್ಥಿತಿ 'f': ಸ್ಥಿತಿ 'F': ಹಿಂತಿರುಗಿ 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool display_return = true;
  bool posixly_correct = !!getenv ("POSIXLY_CORRECT");
  bool allow_options =
    (! posixly_correct
     || (! DEFAULT_ECHO_ಗಮ್ಯ_XPG && 1 < argc && streq (argv[1], "-n")));

  /* System V machines already have a /bin/sh with a v9 behavior.
     Use the identical behavior for these machines so that the
     existing system shell scripts won't barf.  */
  bool do_v9 = DEFAULT_ECHO_ಗಮ್ಯ_XPG;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  ಆದರೆ (allow_options && argc == 2)
    {
      ಆದರೆ (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      ಆದರೆ (streq (argv[1], "--version"))
        {
          ಆವೃತ್ತಿ_etc (stdout, ಕಾರ್ಯಕ್ರಮ_NAME, PACKAGE_NAME, ಆವೃತ್ತಿ, AUTHORS,
                       (ಅಕ್ಷರ *) ಖಾಲಿ);
          ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  ಆದರೆ (allow_options)
    ಆಗ (argc > 0 && *argv[0] == '-')
      {
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ತಾತ್ಕಾಲಿಕ = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        ಗಾಗಿ (i = 0; ತಾತ್ಕಾಲಿಕ[i]; i++)
          ಆಯ್ಕೆ (ತಾತ್ಕಾಲಿಕ[i])
            {
            ಸ್ಥಿತಿ 'e': ಸ್ಥಿತಿ 'E': ಸ್ಥಿತಿ 'n':
              ನಿಲ್ಲಿಸು;
            ಪೂರ್ವನಿಯೋಜಿತ:
              ಹೋಗು just_echo;
            }

        ಆದರೆ (i == 0)
          ಹೋಗು just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        ಆಗ (*ತಾತ್ಕಾಲಿಕ)
          ಆಯ್ಕೆ (*ತಾತ್ಕಾಲಿಕ++)
            {
            ಸ್ಥಿತಿ 'e':
              do_v9 = true;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'E':
              do_v9 = false;
              ನಿಲ್ಲಿಸು;

            ಸ್ಥಿತಿ 'n':
              display_return = false;
              ನಿಲ್ಲಿಸು;
            }

        argc--;
        argv++;
      }

just_echo:

  ಆದರೆ (do_v9 || posixly_correct)
    {
      ಆಗ (argc > 0)
        {
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s = argv[0];
          ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ c;

          ಆಗ ((c = *s++))
            {
              ಆದರೆ (c == '\\' && *s)
                {
                  ಆಯ್ಕೆ (c = *s++)
                    {
                    ಸ್ಥಿತಿ 'a': c = '\a'; ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ 'b': c = '\b'; ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ 'c': ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
                    ಸ್ಥಿತಿ 'e': c = '\x1B'; ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ 'f': c = '\f'; ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ 'n': c = '\n'; ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ 'r': c = '\r'; ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ 't': c = '\t'; ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ 'v': c = '\v'; ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ 'x':
                      {
                        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ch = *s;
                        ಆದರೆ (! c_isxdigit (ch))
                          ಹೋಗು not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        ಆದರೆ (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ '0':
                      c = 0;
                      ಆದರೆ (! ('0' <= *s && *s <= '7'))
                        ನಿಲ್ಲಿಸು;
                      c = *s++;
                      FALLTHROUGH;
                    ಸ್ಥಿತಿ '1': ಸ್ಥಿತಿ '2': ಸ್ಥಿತಿ '3':
                    ಸ್ಥಿತಿ '4': ಸ್ಥಿತಿ '5': ಸ್ಥಿತಿ '6': ಸ್ಥಿತಿ '7':
                      c -= '0';
                      ಆದರೆ ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      ಆದರೆ ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      ನಿಲ್ಲಿಸು;
                    ಸ್ಥಿತಿ '\\': ನಿಲ್ಲಿಸು;

                    not_an_escape:
                    ಪೂರ್ವನಿಯೋಜಿತ:  ಅಕ್ಷರ_ಬರೆ ('\\'); ನಿಲ್ಲಿಸು;
                    }
                }
              ಅಕ್ಷರ_ಬರೆ (c);
            }
          argc--;
          argv++;
          ಆದರೆ (argc > 0)
            ಅಕ್ಷರ_ಬರೆ (' ');
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆಗ (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          ಆದರೆ (argc > 0)
            ಅಕ್ಷರ_ಬರೆ (' ');
        }
    }

  ಆದರೆ (display_return)
    ಅಕ್ಷರ_ಬರೆ ('\n');
  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
