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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "ioblksize.h"
#ଅନ୍ତର୍ଭାଵ "mcel.h"
#ଅନ୍ତର୍ଭାଵ "mbbuf.h"
#ଅନ୍ତର୍ଭାଵ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "expand"

#define AUTHORS proper_name ("David MacKenzie")

ସ୍ଥିର ଅକ୍ଷର ଅଚଳ shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"tabs", required_argument, ଖାଲି, 't'},
  {"initial", no_argument, ଖାଲି, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
         କର not convert tabs after non blanks\n\
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

ସ୍ଥିର ଶୂନ୍ୟ
expand (ଶୂନ୍ୟ)
{
  /* Input stream.  */
  FILE *fp = next_file (ଖାଲି);

  ଯଦି (!fp)
    ଫେରନ୍ତୁ;

  ସ୍ଥିର ଅକ୍ଷର ଧାଡ଼ି_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, ଧାଡ଼ି_in, ଆକାର ଧାଡ଼ି_in, fp);

  ଯେତେବେଳେ (true)
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

      କର
        {
          ଯେତେବେଳେ ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, ଧାଡ଼ି_in, ଆକାର ଧାଡ଼ି_in, fp);

          ଯଦି (convert)
            {
              convert &= convert_entire_ଧାଡ଼ି || c32issep (g.ch);

              ଯଦି (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool last_tab;
                  colno next_tab_column
                    = get_next_tab_column (column, &tab_index, &last_tab);

                  ଯେତେବେଳେ (++column < next_tab_column)
                    ଯଦି (putchar (' ') < 0)
                      write_error ();

                  ଯଦି (putchar (' ') < 0)
                    write_error ();

                  ଜାରି;
                }
              ଅନ୍ୟଥା ଯଦି (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  tab_index -= !!tab_index;
                }
              ଅନ୍ୟଥା
                {
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = c32width (g.ch);
                  ଯଦି (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          ଯଦି (g.ch == MBBUF_EOF)
            ଫେରନ୍ତୁ;

          fwrite (mbbuf_char_offset (&mbbuf, g), ଆକାର (ଅକ୍ଷର), g.len, stdout);
          ଯଦି (ferror (stdout))
            write_error ();
        }
      ଯେତେବେଳେ (g.ch != '\n');
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_ଧାଡ଼ି = true;

  ଯେତେବେଳେ ((c = getopt_long (argc, argv, shortopts, longopts, ଖାଲି)) != -1)
    {
      ବାଛ (c)
        {
        ସ୍ଥିତି 'i':
          convert_entire_ଧାଡ଼ି = false;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 't':
          parse_tab_stops (optarg);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି '0': ସ୍ଥିତି '1': ସ୍ଥିତି '2': ସ୍ଥିତି '3': ସ୍ଥିତି '4':
        ସ୍ଥିତି '5': ସ୍ଥିତି '6': ସ୍ଥିତି '7': ସ୍ଥିତି '8': ସ୍ଥିତି '9':
          ଯଦି (optarg)
            parse_tab_stops (optarg - 1);
          ଅନ୍ୟଥା
            {
              ଅକ୍ଷର tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  set_file_list (optind < argc ? &argv[optind] : ଖାଲି);

  expand ();

  cleanup_file_list_stdin ();

  ଫେରନ୍ତୁ exit_status;
}
