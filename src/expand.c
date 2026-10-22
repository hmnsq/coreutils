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

#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "ioblksize.h"
#شامل "mcel.h"
#شامل "mbbuf.h"
#شامل "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "expand"

#define AUTHORS proper_name ("David MacKenzie")

ایستا نویسه ثابت shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

ایستا ساختار option ثابت longopts[] =
{
  {"tabs", required_argument, پوچ, 't'},
  {"initial", no_argument, پوچ, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
         انجامبده not convert tabs after non blanks\n\
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

ایستا تهی
expand (تهی)
{
  /* Input stream.  */
  FILE *fp = next_file (پوچ);

  اگر (!fp)
    بازگشت;

  ایستا نویسه سطر_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, سطر_in, اندازه سطر_in, fp);

  وقتی (true)
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

      انجامبده
        {
          وقتی ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, سطر_in, اندازه سطر_in, fp);

          اگر (convert)
            {
              convert &= convert_entire_سطر || c32issep (g.ch);

              اگر (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool last_tab;
                  colno next_tab_column
                    = get_next_tab_column (column, &tab_index, &last_tab);

                  وقتی (++column < next_tab_column)
                    اگر (putchar (' ') < 0)
                      write_error ();

                  اگر (putchar (' ') < 0)
                    write_error ();

                  ادامه;
                }
              وگرنه اگر (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  tab_index -= !!tab_index;
                }
              وگرنه
                {
                  صحیح width = c32width (g.ch);
                  اگر (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          اگر (g.ch == MBBUF_EOF)
            بازگشت;

          fwrite (mbbuf_char_offset (&mbbuf, g), اندازه (نویسه), g.len, stdout);
          اگر (ferror (stdout))
            write_error ();
        }
      وقتی (g.ch != '\n');
    }
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_سطر = true;

  وقتی ((c = getopt_long (argc, argv, shortopts, longopts, پوچ)) != -1)
    {
      انتخاب (c)
        {
        حالت 'i':
          convert_entire_سطر = false;
          بشکن;

        حالت 't':
          parse_tab_stops (optarg);
          بشکن;

        حالت '0': حالت '1': حالت '2': حالت '3': حالت '4':
        حالت '5': حالت '6': حالت '7': حالت '8': حالت '9':
          اگر (optarg)
            parse_tab_stops (optarg - 1);
          وگرنه
            {
              نویسه tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  set_file_list (optind < argc ? &argv[optind] : پوچ);

  expand ();

  cleanup_file_list_stdin ();

  بازگشت exit_status;
}
