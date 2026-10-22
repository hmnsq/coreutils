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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "ioblksize.h"
#अन्तर्भाव "mcel.h"
#अन्तर्भाव "mbbuf.h"
#अन्तर्भाव "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "expand"

#define AUTHORS proper_name ("David MacKenzie")

स्थिर वर्ण ध्रुव shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

स्थिर संरचना option ध्रुव longopts[] =
{
  {"tabs", required_argument, रिक्त, 't'},
  {"initial", no_argument, रिक्त, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
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
         करोहि not convert tabs after non blanks\n\
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

स्थिर शून्य
expand (शून्य)
{
  /* Input stream.  */
  FILE *fp = next_file (रिक्त);

  सचे (!fp)
    पटिनिवत्त;

  स्थिर वर्ण पङ्क्तिः_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, पङ्क्तिः_in, परिमाण पङ्क्तिः_in, fp);

  याव (true)
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

      करोहि
        {
          याव ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, पङ्क्तिः_in, परिमाण पङ्क्तिः_in, fp);

          सचे (convert)
            {
              convert &= convert_entire_पङ्क्तिः || c32issep (g.ch);

              सचे (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool last_tab;
                  colno next_tab_column
                    = get_next_tab_column (column, &tab_index, &last_tab);

                  याव (++column < next_tab_column)
                    सचे (putchar (' ') < 0)
                      write_error ();

                  सचे (putchar (' ') < 0)
                    write_error ();

                  अनुवर्त;
                }
              अञ्ञथा सचे (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  tab_index -= !!tab_index;
                }
              अञ्ञथा
                {
                  पूर्णाङ्क width = c32width (g.ch);
                  सचे (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          सचे (g.ch == MBBUF_EOF)
            पटिनिवत्त;

          fwrite (mbbuf_char_offset (&mbbuf, g), परिमाण (वर्ण), g.len, stdout);
          सचे (ferror (stdout))
            write_error ();
        }
      याव (g.ch != '\n');
    }
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_पङ्क्तिः = true;

  याव ((c = getopt_long (argc, argv, shortopts, longopts, रिक्त)) != -1)
    {
      विकल्प (c)
        {
        पक्ष 'i':
          convert_entire_पङ्क्तिः = false;
          भङ्ग;

        पक्ष 't':
          parse_tab_stops (optarg);
          भङ्ग;

        पक्ष '0': पक्ष '1': पक्ष '2': पक्ष '3': पक्ष '4':
        पक्ष '5': पक्ष '6': पक्ष '7': पक्ष '8': पक्ष '9':
          सचे (optarg)
            parse_tab_stops (optarg - 1);
          अञ्ञथा
            {
              वर्ण tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  set_file_list (optind < argc ? &argv[optind] : रिक्त);

  expand ();

  cleanup_file_list_stdin ();

  पटिनिवत्त exit_status;
}
