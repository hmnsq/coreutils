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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "echo"

#నిర్వచించు AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#నిర్వచితం_కాకపోతే DEFAULT_ECHO_గమ్యం_XPG
గణన { DEFAULT_ECHO_గమ్యం_XPG = false };
#అయితే_ముగింపు

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  /* STATUS should always be EXIT_SUCCESS (unlike in most other
     utilities which would call emit_try_help otherwise).  */
  affirm (స్థితి_2 == EXIT_SUCCESS);

  రాయి (_("\
Usage: %s [SHORT-OPTION]... [STRING]...\n\
  or:  %s LONG-OPTION\n\
"), కార్యక్రమం_name, కార్యక్రమం_name);
  fputs (_("\
Echo the STRING(s) గమ్యం standard output.\n\
\n\
"), stdout);
  oputs (_("\
  -n     చేయి not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_గమ్యం_XPG
           ? N_("\
  -e     enable interpretation యొక్క backslash escapes (అప్రమేయ)\n\
")
           : N_("\
  -e     enable interpretation యొక్క backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_గమ్యం_XPG
           ? N_("\
  -E     disable interpretation యొక్క backslash escapes\n\
")
           : N_("\
  -E     disable interpretation యొక్క backslash escapes (అప్రమేయ)\n\
")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (సంస్కరణ_OPTION_DESCRIPTION);
  fputs (_("\
\n\
If -e is లోపల effect, the following sequences are recognized:\n\
\n\
"), stdout);
  fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new పంక్తి\n\
  \\r      carriage తిరిగి\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal విలువ NNN (1 గమ్యం 3 digits)\n\
  \\xHH    byte with hexadecimal విలువ HH (1 గమ్యం 2 digits)\n\
"), stdout);
  రాయి (USAGE_BUILTIN_WARNING, కార్యక్రమం_NAME);
  fputs (_("\n\
Consider using the రాయి(1) command instead,\n\
as it avoids problems when outputting option-like అక్షరశ్రేణులు.\n\
"), stdout);
  emit_ancillary_info (కార్యక్రమం_NAME);
  నిష్క్రమించు (స్థితి_2);
}

/* Convert C from hexadecimal character to integer.  */
స్థిర పూర్ణాంకం
hextobin (గుర్తులేని అక్షరం c)
{
  ఎంపిక (c)
    {
    అప్రమేయ: తిరిగి c - '0';
    స్థితి 'a': స్థితి 'A': తిరిగి 10;
    స్థితి 'b': స్థితి 'B': తిరిగి 11;
    స్థితి 'c': స్థితి 'C': తిరిగి 12;
    స్థితి 'd': స్థితి 'D': తిరిగి 13;
    స్థితి 'e': స్థితి 'E': తిరిగి 14;
    స్థితి 'f': స్థితి 'F': తిరిగి 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool display_return = true;
  bool posixly_correct = !!getenv ("POSIXLY_CORRECT");
  bool allow_options =
    (! posixly_correct
     || (! DEFAULT_ECHO_గమ్యం_XPG && 1 < argc && streq (argv[1], "-n")));

  /* System V machines already have a /bin/sh with a v9 behavior.
     Use the identical behavior for these machines so that the
     existing system shell scripts won't barf.  */
  bool do_v9 = DEFAULT_ECHO_గమ్యం_XPG;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  ఒకవేళ (allow_options && argc == 2)
    {
      ఒకవేళ (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      ఒకవేళ (streq (argv[1], "--version"))
        {
          సంస్కరణ_etc (stdout, కార్యక్రమం_NAME, PACKAGE_NAME, సంస్కరణ, AUTHORS,
                       (అక్షరం *) ఖాళీ);
          తిరిగి EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  ఒకవేళ (allow_options)
    అయితే (argc > 0 && *argv[0] == '-')
      {
        అక్షరం స్థిరరాశి *తాత్కాలిక = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        కొరకు (i = 0; తాత్కాలిక[i]; i++)
          ఎంపిక (తాత్కాలిక[i])
            {
            స్థితి 'e': స్థితి 'E': స్థితి 'n':
              ఆపు;
            అప్రమేయ:
              వెళ్ళు just_echo;
            }

        ఒకవేళ (i == 0)
          వెళ్ళు just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        అయితే (*తాత్కాలిక)
          ఎంపిక (*తాత్కాలిక++)
            {
            స్థితి 'e':
              do_v9 = true;
              ఆపు;

            స్థితి 'E':
              do_v9 = false;
              ఆపు;

            స్థితి 'n':
              display_return = false;
              ఆపు;
            }

        argc--;
        argv++;
      }

just_echo:

  ఒకవేళ (do_v9 || posixly_correct)
    {
      అయితే (argc > 0)
        {
          అక్షరం స్థిరరాశి *s = argv[0];
          గుర్తులేని అక్షరం c;

          అయితే ((c = *s++))
            {
              ఒకవేళ (c == '\\' && *s)
                {
                  ఎంపిక (c = *s++)
                    {
                    స్థితి 'a': c = '\a'; ఆపు;
                    స్థితి 'b': c = '\b'; ఆపు;
                    స్థితి 'c': తిరిగి EXIT_SUCCESS;
                    స్థితి 'e': c = '\x1B'; ఆపు;
                    స్థితి 'f': c = '\f'; ఆపు;
                    స్థితి 'n': c = '\n'; ఆపు;
                    స్థితి 'r': c = '\r'; ఆపు;
                    స్థితి 't': c = '\t'; ఆపు;
                    స్థితి 'v': c = '\v'; ఆపు;
                    స్థితి 'x':
                      {
                        గుర్తులేని అక్షరం ch = *s;
                        ఒకవేళ (! c_isxdigit (ch))
                          వెళ్ళు not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        ఒకవేళ (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      ఆపు;
                    స్థితి '0':
                      c = 0;
                      ఒకవేళ (! ('0' <= *s && *s <= '7'))
                        ఆపు;
                      c = *s++;
                      FALLTHROUGH;
                    స్థితి '1': స్థితి '2': స్థితి '3':
                    స్థితి '4': స్థితి '5': స్థితి '6': స్థితి '7':
                      c -= '0';
                      ఒకవేళ ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      ఒకవేళ ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      ఆపు;
                    స్థితి '\\': ఆపు;

                    not_an_escape:
                    అప్రమేయ:  అక్షరం_రాయి ('\\'); ఆపు;
                    }
                }
              అక్షరం_రాయి (c);
            }
          argc--;
          argv++;
          ఒకవేళ (argc > 0)
            అక్షరం_రాయి (' ');
        }
    }
  లేకపోతే
    {
      అయితే (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          ఒకవేళ (argc > 0)
            అక్షరం_రాయి (' ');
        }
    }

  ఒకవేళ (display_return)
    అక్షరం_రాయి ('\n');
  తిరిగి EXIT_SUCCESS;
}
