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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "ioblksize.h"
#অন্তর্ভাৱ "mcel.h"
#অন্তর্ভাৱ "mbbuf.h"
#অন্তর্ভাৱ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "expand"

#define AUTHORS proper_name ("David MacKenzie")

স্থির ৱর্ণ ধ্রুৱ shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {"tabs", required_argument, রিক্ত, 't'},
  {"initial", no_argument, রিক্ত, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
         কৰা not convert tabs after non blanks\n\
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

স্থির শূন্য
expand (শূন্য)
{
  /* Input stream.  */
  FILE *fp = next_file (রিক্ত);

  যদি (!fp)
    ঘূৰাই;

  স্থির ৱর্ণ সাৰি_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, সাৰি_in, পরিমাণ সাৰি_in, fp);

  যেতিয়ালৈ (true)
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

      কৰা
        {
          যেতিয়ালৈ ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, সাৰি_in, পরিমাণ সাৰি_in, fp);

          যদি (convert)
            {
              convert &= convert_entire_সাৰি || c32issep (g.ch);

              যদি (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool last_tab;
                  colno next_tab_column
                    = get_next_tab_column (column, &tab_index, &last_tab);

                  যেতিয়ালৈ (++column < next_tab_column)
                    যদি (putchar (' ') < 0)
                      write_error ();

                  যদি (putchar (' ') < 0)
                    write_error ();

                  অনুৱর্ত;
                }
              অন্যথা যদি (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  tab_index -= !!tab_index;
                }
              অন্যথা
                {
                  পূর্ণাঙ্ক width = c32width (g.ch);
                  যদি (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          যদি (g.ch == MBBUF_EOF)
            ঘূৰাই;

          fwrite (mbbuf_char_offset (&mbbuf, g), পরিমাণ (ৱর্ণ), g.len, stdout);
          যদি (ferror (stdout))
            write_error ();
        }
      যেতিয়ালৈ (g.ch != '\n');
    }
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  পূর্ণাঙ্ক c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_সাৰি = true;

  যেতিয়ালৈ ((c = getopt_long (argc, argv, shortopts, longopts, রিক্ত)) != -1)
    {
      ৱিকল্প (c)
        {
        পক্ষ 'i':
          convert_entire_সাৰি = false;
          ভঙ্গ;

        পক্ষ 't':
          parse_tab_stops (optarg);
          ভঙ্গ;

        পক্ষ '0': পক্ষ '1': পক্ষ '2': পক্ষ '3': পক্ষ '4':
        পক্ষ '5': পক্ষ '6': পক্ষ '7': পক্ষ '8': পক্ষ '9':
          যদি (optarg)
            parse_tab_stops (optarg - 1);
          অন্যথা
            {
              ৱর্ণ tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  set_file_list (optind < argc ? &argv[optind] : রিক্ত);

  expand ();

  cleanup_file_list_stdin ();

  ঘূৰাই exit_status;
}
