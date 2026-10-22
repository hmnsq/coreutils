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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "assure.h"
#અન્તર્ભાવ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "echo"

#define AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#ifndef DEFAULT_ECHO_TO_XPG
ગણના { DEFAULT_ECHO_TO_XPG = false };
#endif

શૂન્ય
usage (પૂર્ણાંક status)
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
  -n     કરો not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -e     enable interpretation of backslash escapes (મૂળભૂત)\n\
")
           : N_("\
  -e     enable interpretation of backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -E     disable interpretation of backslash escapes\n\
")
           : N_("\
  -E     disable interpretation of backslash escapes (મૂળભૂત)\n\
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
  \\n      new લીટી\n\
  \\r      carriage પાછા\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal કિંમત NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal કિંમત HH (1 to 2 digits)\n\
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
સ્થિર પૂર્ણાંક
hextobin (unsigned અક્ષર c)
{
  પસંદગી (c)
    {
    મૂળભૂત: પાછા c - '0';
    સ્થિતિ 'a': સ્થિતિ 'A': પાછા 10;
    સ્થિતિ 'b': સ્થિતિ 'B': પાછા 11;
    સ્થિતિ 'c': સ્થિતિ 'C': પાછા 12;
    સ્થિતિ 'd': સ્થિતિ 'D': પાછા 13;
    સ્થિતિ 'e': સ્થિતિ 'E': પાછા 14;
    સ્થિતિ 'f': સ્થિતિ 'F': પાછા 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
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
  જો (allow_options && argc == 2)
    {
      જો (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      જો (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (અક્ષર *) ખાલી);
          પાછા EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  જો (allow_options)
    જ્યારે (argc > 0 && *argv[0] == '-')
      {
        અક્ષર અચલ *temp = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        માટે (i = 0; temp[i]; i++)
          પસંદગી (temp[i])
            {
            સ્થિતિ 'e': સ્થિતિ 'E': સ્થિતિ 'n':
              તોડો;
            મૂળભૂત:
              જાઓ just_echo;
            }

        જો (i == 0)
          જાઓ just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        જ્યારે (*temp)
          પસંદગી (*temp++)
            {
            સ્થિતિ 'e':
              do_v9 = true;
              તોડો;

            સ્થિતિ 'E':
              do_v9 = false;
              તોડો;

            સ્થિતિ 'n':
              display_return = false;
              તોડો;
            }

        argc--;
        argv++;
      }

just_echo:

  જો (do_v9 || posixly_correct)
    {
      જ્યારે (argc > 0)
        {
          અક્ષર અચલ *s = argv[0];
          unsigned અક્ષર c;

          જ્યારે ((c = *s++))
            {
              જો (c == '\\' && *s)
                {
                  પસંદગી (c = *s++)
                    {
                    સ્થિતિ 'a': c = '\a'; તોડો;
                    સ્થિતિ 'b': c = '\b'; તોડો;
                    સ્થિતિ 'c': પાછા EXIT_SUCCESS;
                    સ્થિતિ 'e': c = '\x1B'; તોડો;
                    સ્થિતિ 'f': c = '\f'; તોડો;
                    સ્થિતિ 'n': c = '\n'; તોડો;
                    સ્થિતિ 'r': c = '\r'; તોડો;
                    સ્થિતિ 't': c = '\t'; તોડો;
                    સ્થિતિ 'v': c = '\v'; તોડો;
                    સ્થિતિ 'x':
                      {
                        unsigned અક્ષર ch = *s;
                        જો (! c_isxdigit (ch))
                          જાઓ not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        જો (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      તોડો;
                    સ્થિતિ '0':
                      c = 0;
                      જો (! ('0' <= *s && *s <= '7'))
                        તોડો;
                      c = *s++;
                      FALLTHROUGH;
                    સ્થિતિ '1': સ્થિતિ '2': સ્થિતિ '3':
                    સ્થિતિ '4': સ્થિતિ '5': સ્થિતિ '6': સ્થિતિ '7':
                      c -= '0';
                      જો ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      જો ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      તોડો;
                    સ્થિતિ '\\': તોડો;

                    not_an_escape:
                    મૂળભૂત:  putchar ('\\'); તોડો;
                    }
                }
              putchar (c);
            }
          argc--;
          argv++;
          જો (argc > 0)
            putchar (' ');
        }
    }
  નહીંતર
    {
      જ્યારે (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          જો (argc > 0)
            putchar (' ');
        }
    }

  જો (display_return)
    putchar ('\n');
  પાછા EXIT_SUCCESS;
}
