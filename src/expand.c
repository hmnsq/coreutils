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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"
#ಅನ್ತರ್ಭಾವ "mcel.h"
#ಅನ್ತರ್ಭಾವ "mbbuf.h"
#ಅನ್ತರ್ಭಾವ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "expand"

#define AUTHORS proper_name ("David MacKenzie")

ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"tabs", required_argument, ರಿಕ್ತ, 't'},
  {"initial", no_argument, ರಿಕ್ತ, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
         ಕುರು not convert tabs after non blanks\n\
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

ಸ್ಥಿರ ಶೂನ್ಯ
expand (ಶೂನ್ಯ)
{
  /* Input stream.  */
  FILE *fp = next_file (ರಿಕ್ತ);

  ಯದಿ (!fp)
    ಪ್ರತ್ಯಾಗಮ;

  ಸ್ಥಿರ ವರ್ಣ ಸಾಲು_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, ಸಾಲು_in, ಪರಿಮಾಣ ಸಾಲು_in, fp);

  ಯಾವತ್ (true)
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

      ಕುರು
        {
          ಯಾವತ್ ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, ಸಾಲು_in, ಪರಿಮಾಣ ಸಾಲು_in, fp);

          ಯದಿ (convert)
            {
              convert &= convert_entire_ಸಾಲು || c32issep (g.ch);

              ಯದಿ (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool last_tab;
                  colno next_tab_column
                    = get_next_tab_column (column, &tab_index, &last_tab);

                  ಯಾವತ್ (++column < next_tab_column)
                    ಯದಿ (putchar (' ') < 0)
                      write_error ();

                  ಯದಿ (putchar (' ') < 0)
                    write_error ();

                  ಅನುವರ್ತ;
                }
              ಅನ್ಯಥಾ ಯದಿ (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  tab_index -= !!tab_index;
                }
              ಅನ್ಯಥಾ
                {
                  ಪೂರ್ಣಾಙ್ಕ width = c32width (g.ch);
                  ಯದಿ (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          ಯದಿ (g.ch == MBBUF_EOF)
            ಪ್ರತ್ಯಾಗಮ;

          fwrite (mbbuf_char_offset (&mbbuf, g), ಪರಿಮಾಣ (ವರ್ಣ), g.len, stdout);
          ಯದಿ (ferror (stdout))
            write_error ();
        }
      ಯಾವತ್ (g.ch != '\n');
    }
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_ಸಾಲು = true;

  ಯಾವತ್ ((c = getopt_long (argc, argv, shortopts, longopts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'i':
          convert_entire_ಸಾಲು = false;
          ಭಙ್ಗ;

        ಪಕ್ಷ 't':
          parse_tab_stops (optarg);
          ಭಙ್ಗ;

        ಪಕ್ಷ '0': ಪಕ್ಷ '1': ಪಕ್ಷ '2': ಪಕ್ಷ '3': ಪಕ್ಷ '4':
        ಪಕ್ಷ '5': ಪಕ್ಷ '6': ಪಕ್ಷ '7': ಪಕ್ಷ '8': ಪಕ್ಷ '9':
          ಯದಿ (optarg)
            parse_tab_stops (optarg - 1);
          ಅನ್ಯಥಾ
            {
              ವರ್ಣ tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  set_file_list (optind < argc ? &argv[optind] : ರಿಕ್ತ);

  expand ();

  cleanup_file_list_stdin ();

  ಪ್ರತ್ಯಾಗಮ exit_status;
}
