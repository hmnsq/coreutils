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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "ioblksize.h"
#অন্তর্ভাব "mcel.h"
#অন্তর্ভাব "mbbuf.h"
#অন্তর্ভাব "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "expand"

#define AUTHORS proper_name ("David MacKenzie")

স্থির অক্ষর ধ্রুবক shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"tabs", required_argument, ফাঁকা, 't'},
  {"initial", no_argument, ফাঁকা, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
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
         করো not convert tabs after non blanks\n\
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
  FILE *fp = পরবর্তী_file (ফাঁকা);

  যদি (!fp)
    ফেরত;

  স্থির অক্ষর সারি_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, সারি_in, আকার সারি_in, fp);

  যখন (true)
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
      idx_t tab_সূচক = 0;


      /* Convert a line of text.  */

      করো
        {
          যখন ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = পরবর্তী_file (fp)))
            mbbuf_init (&mbbuf, সারি_in, আকার সারি_in, fp);

          যদি (convert)
            {
              convert &= convert_entire_সারি || c32issep (g.ch);

              যদি (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool last_tab;
                  colno পরবর্তী_tab_column
                    = get_পরবর্তী_tab_column (column, &tab_সূচক, &last_tab);

                  যখন (++column < পরবর্তী_tab_column)
                    যদি (putchar (' ') < 0)
                      write_error ();

                  যদি (putchar (' ') < 0)
                    write_error ();

                  চালু;
                }
              অন্যথা যদি (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  tab_সূচক -= !!tab_সূচক;
                }
              অন্যথা
                {
                  পূর্ণসংখ্যা width = c32width (g.ch);
                  যদি (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          যদি (g.ch == MBBUF_EOF)
            ফেরত;

          fwrite (mbbuf_char_offset (&mbbuf, g), আকার (অক্ষর), g.len, stdout);
          যদি (ferror (stdout))
            write_error ();
        }
      যখন (g.ch != '\n');
    }
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_সারি = true;

  যখন ((c = getopt_long (argc, argv, shortopts, longopts, ফাঁকা)) != -1)
    {
      নির্বাচন (c)
        {
        ক্ষেত্র 'i':
          convert_entire_সারি = false;
          বিরতি;

        ক্ষেত্র 't':
          parse_tab_stops (optarg);
          বিরতি;

        ক্ষেত্র '0': ক্ষেত্র '1': ক্ষেত্র '2': ক্ষেত্র '3': ক্ষেত্র '4':
        ক্ষেত্র '5': ক্ষেত্র '6': ক্ষেত্র '7': ক্ষেত্র '8': ক্ষেত্র '9':
          যদি (optarg)
            parse_tab_stops (optarg - 1);
          অন্যথা
            {
              অক্ষর tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  set_file_list (optind < argc ? &argv[optind] : ফাঁকা);

  expand ();

  cleanup_file_list_stdin ();

  ফেরত exit_status;
}
