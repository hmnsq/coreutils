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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "ioblksize.h"
#અન્તર્ભાવ "mcel.h"
#અન્તર્ભાવ "mbbuf.h"
#અન્તર્ભાવ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "expand"

#define AUTHORS proper_name ("David MacKenzie")

સ્થિર અક્ષર અચલ shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

સ્થિર સંરચના option અચલ longopts[] =
{
  {"tabs", required_argument, ખાલી, 't'},
  {"initial", no_argument, ખાલી, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
         કરો not convert tabs after non blanks\n\
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

સ્થિર શૂન્ય
expand (શૂન્ય)
{
  /* Input stream.  */
  FILE *fp = next_file (ખાલી);

  જો (!fp)
    પાછા;

  સ્થિર અક્ષર લીટી_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, લીટી_in, કદ લીટી_in, fp);

  જ્યારે (true)
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

      કરો
        {
          જ્યારે ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, લીટી_in, કદ લીટી_in, fp);

          જો (convert)
            {
              convert &= convert_entire_લીટી || c32issep (g.ch);

              જો (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool last_tab;
                  colno next_tab_column
                    = get_next_tab_column (column, &tab_index, &last_tab);

                  જ્યારે (++column < next_tab_column)
                    જો (putchar (' ') < 0)
                      write_error ();

                  જો (putchar (' ') < 0)
                    write_error ();

                  ચાલુ;
                }
              નહીંતર જો (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  tab_index -= !!tab_index;
                }
              નહીંતર
                {
                  પૂર્ણાંક width = c32width (g.ch);
                  જો (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          જો (g.ch == MBBUF_EOF)
            પાછા;

          fwrite (mbbuf_char_offset (&mbbuf, g), કદ (અક્ષર), g.len, stdout);
          જો (ferror (stdout))
            write_error ();
        }
      જ્યારે (g.ch != '\n');
    }
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_લીટી = true;

  જ્યારે ((c = getopt_long (argc, argv, shortopts, longopts, ખાલી)) != -1)
    {
      પસંદગી (c)
        {
        સ્થિતિ 'i':
          convert_entire_લીટી = false;
          તોડો;

        સ્થિતિ 't':
          parse_tab_stops (optarg);
          તોડો;

        સ્થિતિ '0': સ્થિતિ '1': સ્થિતિ '2': સ્થિતિ '3': સ્થિતિ '4':
        સ્થિતિ '5': સ્થિતિ '6': સ્થિતિ '7': સ્થિતિ '8': સ્થિતિ '9':
          જો (optarg)
            parse_tab_stops (optarg - 1);
          નહીંતર
            {
              અક્ષર tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  set_file_list (optind < argc ? &argv[optind] : ખાલી);

  expand ();

  cleanup_file_list_stdin ();

  પાછા exit_status;
}
