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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"
#ଅନ୍ତର୍ଭାଵ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "echo"

#define AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#ifndef DEFAULT_ECHO_TO_XPG
ଗଣନା { DEFAULT_ECHO_TO_XPG = false };
#endif

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
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
  -n     କର not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -e     enable interpretation of backslash escapes (ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\n\
")
           : N_("\
  -e     enable interpretation of backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_TO_XPG
           ? N_("\
  -E     disable interpretation of backslash escapes\n\
")
           : N_("\
  -E     disable interpretation of backslash escapes (ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\n\
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
  \\n      new ଧାଡ଼ି\n\
  \\r      carriage ଫେରନ୍ତୁ\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal ମୂଲ୍ୟ NNN (1 to 3 digits)\n\
  \\xHH    byte with hexadecimal ମୂଲ୍ୟ HH (1 to 2 digits)\n\
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
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
hextobin (unsigned ଅକ୍ଷର c)
{
  ବାଛ (c)
    {
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ c - '0';
    ସ୍ଥିତି 'a': ସ୍ଥିତି 'A': ଫେରନ୍ତୁ 10;
    ସ୍ଥିତି 'b': ସ୍ଥିତି 'B': ଫେରନ୍ତୁ 11;
    ସ୍ଥିତି 'c': ସ୍ଥିତି 'C': ଫେରନ୍ତୁ 12;
    ସ୍ଥିତି 'd': ସ୍ଥିତି 'D': ଫେରନ୍ତୁ 13;
    ସ୍ଥିତି 'e': ସ୍ଥିତି 'E': ଫେରନ୍ତୁ 14;
    ସ୍ଥିତି 'f': ସ୍ଥିତି 'F': ଫେରନ୍ତୁ 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
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
  ଯଦି (allow_options && argc == 2)
    {
      ଯଦି (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      ଯଦି (streq (argv[1], "--version"))
        {
          version_etc (stdout, PROGRAM_NAME, PACKAGE_NAME, Version, AUTHORS,
                       (ଅକ୍ଷର *) ଖାଲି);
          ଫେରନ୍ତୁ EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  ଯଦି (allow_options)
    ଯେତେବେଳେ (argc > 0 && *argv[0] == '-')
      {
        ଅକ୍ଷର ଅଚଳ *temp = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        ପାଇଁ (i = 0; temp[i]; i++)
          ବାଛ (temp[i])
            {
            ସ୍ଥିତି 'e': ସ୍ଥିତି 'E': ସ୍ଥିତି 'n':
              ଭାଙ୍ଗ;
            ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
              ଯାଅ just_echo;
            }

        ଯଦି (i == 0)
          ଯାଅ just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        ଯେତେବେଳେ (*temp)
          ବାଛ (*temp++)
            {
            ସ୍ଥିତି 'e':
              do_v9 = true;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'E':
              do_v9 = false;
              ଭାଙ୍ଗ;

            ସ୍ଥିତି 'n':
              display_return = false;
              ଭାଙ୍ଗ;
            }

        argc--;
        argv++;
      }

just_echo:

  ଯଦି (do_v9 || posixly_correct)
    {
      ଯେତେବେଳେ (argc > 0)
        {
          ଅକ୍ଷର ଅଚଳ *s = argv[0];
          unsigned ଅକ୍ଷର c;

          ଯେତେବେଳେ ((c = *s++))
            {
              ଯଦି (c == '\\' && *s)
                {
                  ବାଛ (c = *s++)
                    {
                    ସ୍ଥିତି 'a': c = '\a'; ଭାଙ୍ଗ;
                    ସ୍ଥିତି 'b': c = '\b'; ଭାଙ୍ଗ;
                    ସ୍ଥିତି 'c': ଫେରନ୍ତୁ EXIT_SUCCESS;
                    ସ୍ଥିତି 'e': c = '\x1B'; ଭାଙ୍ଗ;
                    ସ୍ଥିତି 'f': c = '\f'; ଭାଙ୍ଗ;
                    ସ୍ଥିତି 'n': c = '\n'; ଭାଙ୍ଗ;
                    ସ୍ଥିତି 'r': c = '\r'; ଭାଙ୍ଗ;
                    ସ୍ଥିତି 't': c = '\t'; ଭାଙ୍ଗ;
                    ସ୍ଥିତି 'v': c = '\v'; ଭାଙ୍ଗ;
                    ସ୍ଥିତି 'x':
                      {
                        unsigned ଅକ୍ଷର ch = *s;
                        ଯଦି (! c_isxdigit (ch))
                          ଯାଅ not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        ଯଦି (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      ଭାଙ୍ଗ;
                    ସ୍ଥିତି '0':
                      c = 0;
                      ଯଦି (! ('0' <= *s && *s <= '7'))
                        ଭାଙ୍ଗ;
                      c = *s++;
                      FALLTHROUGH;
                    ସ୍ଥିତି '1': ସ୍ଥିତି '2': ସ୍ଥିତି '3':
                    ସ୍ଥିତି '4': ସ୍ଥିତି '5': ସ୍ଥିତି '6': ସ୍ଥିତି '7':
                      c -= '0';
                      ଯଦି ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      ଯଦି ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      ଭାଙ୍ଗ;
                    ସ୍ଥିତି '\\': ଭାଙ୍ଗ;

                    not_an_escape:
                    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:  putchar ('\\'); ଭାଙ୍ଗ;
                    }
                }
              putchar (c);
            }
          argc--;
          argv++;
          ଯଦି (argc > 0)
            putchar (' ');
        }
    }
  ଅନ୍ୟଥା
    {
      ଯେତେବେଳେ (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          ଯଦି (argc > 0)
            putchar (' ');
        }
    }

  ଯଦି (display_return)
    putchar ('\n');
  ଫେରନ୍ତୁ EXIT_SUCCESS;
}
