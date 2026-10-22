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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "c-ctype.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "echo"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Brian Fox"), \
  proper_name ("Chet Ramey")

/* If true, interpret backslash escapes by default.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ DEFAULT_ECHO_ਮੰਜ਼ਿਲ_XPG
ਗਿਣਤੀ { DEFAULT_ECHO_ਮੰਜ਼ਿਲ_XPG = false };
#ਜੇ_ਅੰਤ

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  /* STATUS should always be EXIT_SUCCESS (unlike in most other
     utilities which would call emit_try_help otherwise).  */
  affirm (ਸਥਿਤੀ == EXIT_SUCCESS);

  ਲਿਖੋ (_("\
Usage: %s [SHORT-OPTION]... [STRING]...\n\
  or:  %s LONG-OPTION\n\
"), ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
  fputs (_("\
Echo the STRING(s) ਮੰਜ਼ਿਲ standard output.\n\
\n\
"), stdout);
  oputs (_("\
  -n     ਕਰੋ not output the trailing newline\n\
"));
  oputs (_(DEFAULT_ECHO_ਮੰਜ਼ਿਲ_XPG
           ? N_("\
  -e     enable interpretation ਦਾ backslash escapes (ਮੂਲ)\n\
")
           : N_("\
  -e     enable interpretation ਦਾ backslash escapes\n\
")));
  oputs (_(DEFAULT_ECHO_ਮੰਜ਼ਿਲ_XPG
           ? N_("\
  -E     disable interpretation ਦਾ backslash escapes\n\
")
           : N_("\
  -E     disable interpretation ਦਾ backslash escapes (ਮੂਲ)\n\
")));
  oputs (HELP_OPTION_DESCRIPTION);
  oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
  fputs (_("\
\n\
If -e is ਵਿੱਚ effect, the following sequences are recognized:\n\
\n\
"), stdout);
  fputs (_("\
  \\\\      backslash\n\
  \\a      alert (bell)\n\
  \\b      backspace\n\
  \\c      produce no further output\n\
  \\e      escape\n\
  \\f      form feed\n\
  \\n      new ਸਤਰ\n\
  \\r      carriage ਵਾਪਸ\n\
  \\t      horizontal tab\n\
  \\v      vertical tab\n\
"), stdout);
  fputs (_("\
  \\0NNN   byte with octal ਮੁੱਲ NNN (1 ਮੰਜ਼ਿਲ 3 digits)\n\
  \\xHH    byte with hexadecimal ਮੁੱਲ HH (1 ਮੰਜ਼ਿਲ 2 digits)\n\
"), stdout);
  ਲਿਖੋ (USAGE_BUILTIN_WARNING, ਪ੍ਰੋਗਰਾਮ_NAME);
  fputs (_("\n\
Consider using the ਲਿਖੋ(1) command instead,\n\
as it avoids problems when outputting option-like ਲੜੀਆਂ.\n\
"), stdout);
  emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Convert C from hexadecimal character to integer.  */
ਸਥਿਰ ਪੂਰਨ
hextobin (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c)
{
  ਚੋਣ (c)
    {
    ਮੂਲ: ਵਾਪਸ c - '0';
    ਹਾਲਤ 'a': ਹਾਲਤ 'A': ਵਾਪਸ 10;
    ਹਾਲਤ 'b': ਹਾਲਤ 'B': ਵਾਪਸ 11;
    ਹਾਲਤ 'c': ਹਾਲਤ 'C': ਵਾਪਸ 12;
    ਹਾਲਤ 'd': ਹਾਲਤ 'D': ਵਾਪਸ 13;
    ਹਾਲਤ 'e': ਹਾਲਤ 'E': ਵਾਪਸ 14;
    ਹਾਲਤ 'f': ਹਾਲਤ 'F': ਵਾਪਸ 15;
    }
}

/* Print the words in LIST to standard output.  If the first word is
   '-n', then don't print a trailing newline.  We also support the
   echo syntax from Version 9 unix systems. */

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool display_return = true;
  bool posixly_correct = !!getenv ("POSIXLY_CORRECT");
  bool allow_options =
    (! posixly_correct
     || (! DEFAULT_ECHO_ਮੰਜ਼ਿਲ_XPG && 1 < argc && streq (argv[1], "-n")));

  /* System V machines already have a /bin/sh with a v9 behavior.
     Use the identical behavior for these machines so that the
     existing system shell scripts won't barf.  */
  bool do_v9 = DEFAULT_ECHO_ਮੰਜ਼ਿਲ_XPG;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* We directly parse options, rather than use parse_long_options, in
     order to avoid accepting abbreviations.  */
  ਜੇ (allow_options && argc == 2)
    {
      ਜੇ (streq (argv[1], "--help"))
        usage (EXIT_SUCCESS);

      ਜੇ (streq (argv[1], "--version"))
        {
          ਸੰਸਕਰਣ_etc (stdout, ਪ੍ਰੋਗਰਾਮ_NAME, PACKAGE_NAME, ਸੰਸਕਰਣ, AUTHORS,
                       (ਅੱਖਰ *) ਨਿਰਾ);
          ਵਾਪਸ EXIT_SUCCESS;
        }
    }

  --argc;
  ++argv;

  ਜੇ (allow_options)
    ਜਦੋਂ (argc > 0 && *argv[0] == '-')
      {
        ਅੱਖਰ ਅਚਲ *ਆਰਜ਼ੀ = argv[0] + 1;
        size_t i;

        /* If it appears that we are handling options, then make sure that
           all of the options specified are actually valid.  Otherwise, the
           string should just be echoed.  */

        ਲਈ (i = 0; ਆਰਜ਼ੀ[i]; i++)
          ਚੋਣ (ਆਰਜ਼ੀ[i])
            {
            ਹਾਲਤ 'e': ਹਾਲਤ 'E': ਹਾਲਤ 'n':
              ਰੋਕੋ;
            ਮੂਲ:
              ਜਾਓ just_echo;
            }

        ਜੇ (i == 0)
          ਜਾਓ just_echo;

        /* All of the options in TEMP are valid options to ECHO.
           Handle them. */
        ਜਦੋਂ (*ਆਰਜ਼ੀ)
          ਚੋਣ (*ਆਰਜ਼ੀ++)
            {
            ਹਾਲਤ 'e':
              do_v9 = true;
              ਰੋਕੋ;

            ਹਾਲਤ 'E':
              do_v9 = false;
              ਰੋਕੋ;

            ਹਾਲਤ 'n':
              display_return = false;
              ਰੋਕੋ;
            }

        argc--;
        argv++;
      }

just_echo:

  ਜੇ (do_v9 || posixly_correct)
    {
      ਜਦੋਂ (argc > 0)
        {
          ਅੱਖਰ ਅਚਲ *s = argv[0];
          ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ c;

          ਜਦੋਂ ((c = *s++))
            {
              ਜੇ (c == '\\' && *s)
                {
                  ਚੋਣ (c = *s++)
                    {
                    ਹਾਲਤ 'a': c = '\a'; ਰੋਕੋ;
                    ਹਾਲਤ 'b': c = '\b'; ਰੋਕੋ;
                    ਹਾਲਤ 'c': ਵਾਪਸ EXIT_SUCCESS;
                    ਹਾਲਤ 'e': c = '\x1B'; ਰੋਕੋ;
                    ਹਾਲਤ 'f': c = '\f'; ਰੋਕੋ;
                    ਹਾਲਤ 'n': c = '\n'; ਰੋਕੋ;
                    ਹਾਲਤ 'r': c = '\r'; ਰੋਕੋ;
                    ਹਾਲਤ 't': c = '\t'; ਰੋਕੋ;
                    ਹਾਲਤ 'v': c = '\v'; ਰੋਕੋ;
                    ਹਾਲਤ 'x':
                      {
                        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ch = *s;
                        ਜੇ (! c_isxdigit (ch))
                          ਜਾਓ not_an_escape;
                        s++;
                        c = hextobin (ch);
                        ch = *s;
                        ਜੇ (c_isxdigit (ch))
                          {
                            s++;
                            c = c * 16 + hextobin (ch);
                          }
                      }
                      ਰੋਕੋ;
                    ਹਾਲਤ '0':
                      c = 0;
                      ਜੇ (! ('0' <= *s && *s <= '7'))
                        ਰੋਕੋ;
                      c = *s++;
                      FALLTHROUGH;
                    ਹਾਲਤ '1': ਹਾਲਤ '2': ਹਾਲਤ '3':
                    ਹਾਲਤ '4': ਹਾਲਤ '5': ਹਾਲਤ '6': ਹਾਲਤ '7':
                      c -= '0';
                      ਜੇ ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      ਜੇ ('0' <= *s && *s <= '7')
                        c = c * 8 + (*s++ - '0');
                      ਰੋਕੋ;
                    ਹਾਲਤ '\\': ਰੋਕੋ;

                    not_an_escape:
                    ਮੂਲ:  ਅੱਖਰ_ਲਿਖੋ ('\\'); ਰੋਕੋ;
                    }
                }
              ਅੱਖਰ_ਲਿਖੋ (c);
            }
          argc--;
          argv++;
          ਜੇ (argc > 0)
            ਅੱਖਰ_ਲਿਖੋ (' ');
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਜਦੋਂ (argc > 0)
        {
          fputs (argv[0], stdout);
          argc--;
          argv++;
          ਜੇ (argc > 0)
            ਅੱਖਰ_ਲਿਖੋ (' ');
        }
    }

  ਜੇ (display_return)
    ਅੱਖਰ_ਲਿਖੋ ('\n');
  ਵਾਪਸ EXIT_SUCCESS;
}
