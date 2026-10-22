/* expand - convert tabs to spaces
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* By default, convert all tabs to spaces.
   Preserves backspace characters in the output; they decrement the
   column count for tab calculations.
   The default action is equivalent to -8.

   Options:
   --tabs=tab1[,tab2[,...]]
   -t tab1[,tab2[,...]]
   -tab1[,tab2[,...]]	If only one tab stop is given, set the tabs tab1
                        columns apart instead of the default 8.  Otherwise,
                        set the tabs at columns tab1, tab2, etc. (numbered from
                        0); replace any tabs beyond the tab stops given with
                        single spaces.
   --initial
   -i			Only convert initial tabs on each line to spaces.

   David MacKenzie <djm@gnu.ai.mit.edu> */

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "ioblksize.h"
#അന്തര്ഭാവ "mcel.h"
#അന്തര്ഭാവ "mbbuf.h"
#അന്തര്ഭാവ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "expand"

#define AUTHORS proper_name ("David MacKenzie")

സ്ഥിതം അക്ഷരം സ്ഥിരം shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"tabs", required_argument, ശൂന്യ, 't'},
  {"initial", no_argument, ശൂന്യ, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              program_name);
      fputs (_("\
Convert tabs in each FILE to spaces, writing to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --initial\n\
         ചെയ്യുക not convert tabs after non blanks\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart, not 8\n\
"));
      emit_tab_list_info (PROGRAM_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}


/* Change tabs to spaces, writing to stdout.
   Read each file in 'file_list', in order.  */

സ്ഥിതം ശൂന്യം
expand (ശൂന്യം)
{
  /* Input stream.  */
  FILE *fp = next_file (ശൂന്യ);

  എങ്കിൽ (!fp)
    തിരികെ;

  സ്ഥിതം അക്ഷരം വരി_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, വരി_in, വലുപ്പം വരി_in, fp);

  ആകുമ്പോൾ (true)
    {
      /* Input character, or EOF.  */
      mcel_t g;

      /* If true, perform translations.  */
      bool convert = true;


      /* The following variables have valid values only when CONVERT
         is true:  */

      /* Column of next input character.  */
      colno column = 0;

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_index = 0;


      /* Convert a line of text.  */

      ചെയ്യുക
        {
          ആകുമ്പോൾ ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, വരി_in, വലുപ്പം വരി_in, fp);

          എങ്കിൽ (convert)
            {
              convert &= convert_entire_വരി || c32issep (g.ch);

              എങ്കിൽ (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool last_tab;
                  colno next_tab_column
                    = get_next_tab_column (column, &tab_index, &last_tab);

                  ആകുമ്പോൾ (++column < next_tab_column)
                    എങ്കിൽ (putchar (' ') < 0)
                      write_error ();

                  എങ്കിൽ (putchar (' ') < 0)
                    write_error ();

                  തുടരുക;
                }
              അല്ലെങ്കിൽ എങ്കിൽ (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  tab_index -= !!tab_index;
                }
              അല്ലെങ്കിൽ
                {
                  പൂർണ്ണസംഖ്യ width = c32width (g.ch);
                  എങ്കിൽ (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          എങ്കിൽ (g.ch == MBBUF_EOF)
            തിരികെ;

          fwrite (mbbuf_char_offset (&mbbuf, g), വലുപ്പം (അക്ഷരം), g.len, stdout);
          എങ്കിൽ (ferror (stdout))
            write_error ();
        }
      ആകുമ്പോൾ (g.ch != '\n');
    }
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_വരി = true;

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, shortopts, longopts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'i':
          convert_entire_വരി = false;
          നിർത്തുക;

        അവസ്ഥ 't':
          parse_tab_stops (optarg);
          നിർത്തുക;

        അവസ്ഥ '0': അവസ്ഥ '1': അവസ്ഥ '2': അവസ്ഥ '3': അവസ്ഥ '4':
        അവസ്ഥ '5': അവസ്ഥ '6': അവസ്ഥ '7': അവസ്ഥ '8': അവസ്ഥ '9':
          എങ്കിൽ (optarg)
            parse_tab_stops (optarg - 1);
          അല്ലെങ്കിൽ
            {
              അക്ഷരം tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  set_file_list (optind < argc ? &argv[optind] : ശൂന്യ);

  expand ();

  cleanup_file_list_stdin ();

  തിരികെ exit_status;
}
