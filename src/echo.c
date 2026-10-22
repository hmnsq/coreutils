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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"
#அந்தர்பாவ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "echo"

#define AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#ifndef DEFAULT_ECHO_TO_XPG
எண்ணிக்கை { DEFAULT_ECHO_TO_XPG = false };
#endif

வெறுமை
usage (முழு status)
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
  -n     செய் not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -e     enable interpretation of backslash escapes (இயல்பு)\n\
")
           : N_("\
  -e     enable interpretation of backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -E     disable interpretation of backslash escapes\n\
")
           : N_("\
  -E     disable interpretation of backslash escapes (இயல்பு)\n\
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
  \\n      new வரி\n\
  \\r      carriage திரும்பு\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal மதிப்பு NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal மதிப்பு HH (1 to 2 digits)\n\
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
நிலையான முழு
hextobin (unsigned எழுத்து c)
{
  தேர்வு (c)
    {
    இயல்பு: திரும்பு c - '0';
    நிலை 'a': நிலை 'A': திரும்பு 10;
    நிலை 'b': நிலை 'B': திரும்பு 11;
    நிலை 'c': நிலை 'C': திரும்பு 12;
    நிலை 'd': நிலை 'D': திரும்பு 13;
    நிலை 'e': நிலை 'E': திரும்பு 14;
    நிலை 'f': நிலை 'F': திரும்பு 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

முழு
முதன்மை (முழு argc, எழுத்து **argv)
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
  எனில் (allow_options && argc == 2)
    {
      எனில் (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      எனில் (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (எழுத்து *) வெற்று);
          திரும்பு EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  எனில் (allow_options)
    போது (argc > 0 && *argv[0] == '-')
      {
        எழுத்து மாறிலி *temp = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        க்கு (i = 0; temp[i]; i++)
          தேர்வு (temp[i])
            {
            நிலை 'e': நிலை 'E': நிலை 'n':
              நிறுத்து;
            இயல்பு:
              செல் just_echo;
            }

        எனில் (i == 0)
          செல் just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        போது (*temp)
          தேர்வு (*temp++)
            {
            நிலை 'e':
              do_v9 = true;
              நிறுத்து;

            நிலை 'E':
              do_v9 = false;
              நிறுத்து;

            நிலை 'n':
              display_return = false;
              நிறுத்து;
            }

        argc--;
        argv++;
      }

just_echo:

  எனில் (do_v9 || posixly_correct)
    {
      போது (argc > 0)
        {
          எழுத்து மாறிலி *s = argv[0];
          unsigned எழுத்து c;

          போது ((c = *s++))
            {
              எனில் (c == '\\' && *s)
                {
                  தேர்வு (c = *s++)
                    {
                    நிலை 'a': c = '\a'; நிறுத்து;
                    நிலை 'b': c = '\b'; நிறுத்து;
                    நிலை 'c': திரும்பு EXIT_SUCCESS;
                    நிலை 'e': c = '\x1B'; நிறுத்து;
                    நிலை 'f': c = '\f'; நிறுத்து;
                    நிலை 'n': c = '\n'; நிறுத்து;
                    நிலை 'r': c = '\r'; நிறுத்து;
                    நிலை 't': c = '\t'; நிறுத்து;
                    நிலை 'v': c = '\v'; நிறுத்து;
                    நிலை 'x':
                      {
                        unsigned எழுத்து ch = *s;
                        எனில் (! c_isxdigit (ch))
                          செல் not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        எனில் (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      நிறுத்து;
                    நிலை '0':
                      c = 0;
                      எனில் (! ('0' <= *s && *s <= '7'))
                        நிறுத்து;
                      c = *s++;
                      FALLTHROUGH;
                    நிலை '1': நிலை '2': நிலை '3':
                    நிலை '4': நிலை '5': நிலை '6': நிலை '7':
                      c -= '0';
                      எனில் ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      எனில் ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      நிறுத்து;
                    நிலை '\\': நிறுத்து;

                    not_an_escape:
                    இயல்பு:  putchar ('\\'); நிறுத்து;
                    }
                }
              putchar (c);
            }
          argc--;
          argv++;
          எனில் (argc > 0)
            putchar (' ');
        }
    }
  இல்லையெனில்
    {
      போது (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          எனில் (argc > 0)
            putchar (' ');
        }
    }

  எனில் (display_return)
    putchar ('\n');
  திரும்பு EXIT_SUCCESS;
}
