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

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "ioblksize.h"
#అన్తర్భావ "mcel.h"
#అన్తర్భావ "mbbuf.h"
#అన్తర్భావ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "expand"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

స్థిర అక్షరం స్థిరరాశి shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"tabs", required_వాదం, ఖాళీ, 't'},
  {"initial", no_వాదం, ఖాళీ, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Convert tabs లోపల each FILE గమ్యం spaces, writing గమ్యం standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --initial\n\
         చేయి not convert tabs after non blanks\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart, not 8\n\
"));
      emit_tab_list_info (కార్యక్రమం_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}


/* Change tabs to spaces, writing to stdout.
   Read each file in 'file_list', in order.  */

స్థిర శూన్య
expand (శూన్య)
{
  /* Input stream.  */
  FILE *fp = తదుపరి_file (ఖాళీ);

  ఒకవేళ (!fp)
    తిరిగి;

  స్థిర అక్షరం పంక్తి_లోపల[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, పంక్తి_లోపల, పరిమాణం పంక్తి_లోపల, fp);

  అయితే (true)
    {
      /* Input character, or EOF.  */
      mcel_t g;

      /* If true, perform translations.  */
      bool convert = true;


      /* The following variables have valid values only when CONVERT
         is true:  */

      /* Column of next input character.  */
      colno స్తంభం = 0;

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_సూచీ = 0;


      /* Convert a line of text.  */

      చేయి
        {
          అయితే ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = తదుపరి_file (fp)))
            mbbuf_init (&mbbuf, పంక్తి_లోపల, పరిమాణం పంక్తి_లోపల, fp);

          ఒకవేళ (convert)
            {
              convert &= convert_entire_పంక్తి || c32issep (g.ch);

              ఒకవేళ (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool చివరి_tab;
                  colno తదుపరి_tab_స్తంభం
                    = get_తదుపరి_tab_స్తంభం (స్తంభం, &tab_సూచీ, &చివరి_tab);

                  అయితే (++స్తంభం < తదుపరి_tab_స్తంభం)
                    ఒకవేళ (అక్షరం_రాయి (' ') < 0)
                      write_error ();

                  ఒకవేళ (అక్షరం_రాయి (' ') < 0)
                    write_error ();

                  కొనసాగు;
                }
              లేకపోతే ఒకవేళ (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  స్తంభం -= !!స్తంభం;
                  tab_సూచీ -= !!tab_సూచీ;
                }
              లేకపోతే
                {
                  పూర్ణాంకం వెడల్పు = c32width (g.ch);
                  ఒకవేళ (ckd_కూడు (&స్తంభం, స్తంభం, వెడల్పు < 0 ? 1 : వెడల్పు))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          ఒకవేళ (g.ch == MBBUF_EOF)
            తిరిగి;

          fwrite (mbbuf_char_స్థానభ్రంశం (&mbbuf, g), పరిమాణం (అక్షరం), g.len, stdout);
          ఒకవేళ (ferror (stdout))
            write_error ();
        }
      అయితే (g.ch != '\n');
    }
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం c;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_పంక్తి = true;

  అయితే ((c = getopt_long (argc, argv, shortopts, longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'i':
          convert_entire_పంక్తి = false;
          ఆపు;

        స్థితి 't':
          parse_tab_stops (optarg);
          ఆపు;

        స్థితి '0': స్థితి '1': స్థితి '2': స్థితి '3': స్థితి '4':
        స్థితి '5': స్థితి '6': స్థితి '7': స్థితి '8': స్థితి '9':
          ఒకవేళ (optarg)
            parse_tab_stops (optarg - 1);
          లేకపోతే
            {
              అక్షరం tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  సమితి_file_list (optind < argc ? &argv[optind] : ఖాళీ);

  expand ();

  cleanup_file_list_stdin ();

  తిరిగి exit_స్థితి;
}
