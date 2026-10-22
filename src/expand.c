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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "ioblksize.h"
#அந்தர்பாவ "mcel.h"
#அந்தர்பாவ "mbbuf.h"
#அந்தர்பாவ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "expand"

#define AUTHORS proper_name ("David MacKenzie")

நிலையான எழுத்து மாறிலி shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"tabs", required_argument, வெற்று, 't'},
  {"initial", no_argument, வெற்று, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
         செய் not convert tabs after non blanks\n\
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

நிலையான வெறுமை
expand (வெறுமை)
{
  /* Input stream.  */
  FILE *fp = next_file (வெற்று);

  எனில் (!fp)
    திரும்பு;

  நிலையான எழுத்து வரி_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, வரி_in, அளவு வரி_in, fp);

  போது (true)
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

      செய்
        {
          போது ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, வரி_in, அளவு வரி_in, fp);

          எனில் (convert)
            {
              convert &= convert_entire_வரி || c32issep (g.ch);

              எனில் (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool last_tab;
                  colno next_tab_column
                    = get_next_tab_column (column, &tab_index, &last_tab);

                  போது (++column < next_tab_column)
                    எனில் (putchar (' ') < 0)
                      write_error ();

                  எனில் (putchar (' ') < 0)
                    write_error ();

                  தொடர்;
                }
              இல்லையெனில் எனில் (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  tab_index -= !!tab_index;
                }
              இல்லையெனில்
                {
                  முழு width = c32width (g.ch);
                  எனில் (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          எனில் (g.ch == MBBUF_EOF)
            திரும்பு;

          fwrite (mbbuf_char_offset (&mbbuf, g), அளவு (எழுத்து), g.len, stdout);
          எனில் (ferror (stdout))
            write_error ();
        }
      போது (g.ch != '\n');
    }
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_வரி = true;

  போது ((c = getopt_long (argc, argv, shortopts, longopts, வெற்று)) != -1)
    {
      தேர்வு (c)
        {
        நிலை 'i':
          convert_entire_வரி = false;
          நிறுத்து;

        நிலை 't':
          parse_tab_stops (optarg);
          நிறுத்து;

        நிலை '0': நிலை '1': நிலை '2': நிலை '3': நிலை '4':
        நிலை '5': நிலை '6': நிலை '7': நிலை '8': நிலை '9':
          எனில் (optarg)
            parse_tab_stops (optarg - 1);
          இல்லையெனில்
            {
              எழுத்து tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  set_file_list (optind < argc ? &argv[optind] : வெற்று);

  expand ();

  cleanup_file_list_stdin ();

  திரும்பு exit_status;
}
