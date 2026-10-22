/* unexpand - convert blanks to tabs
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

/* By default, convert only maximal strings of initial blanks and tabs
   into tabs.
   Preserves backspace characters in the output; they decrement the
   column count for tab calculations.
   The default action is equivalent to -8.

   Options:
   --tabs=tab1[,tab2[,...]]
   -t tab1[,tab2[,...]]
   -tab1[,tab2[,...]]	If only one tab stop is given, set the tabs tab1
                        columns apart instead of the default 8.  Otherwise,
                        set the tabs at columns tab1, tab2, etc. (numbered from
                        0); preserve any blanks beyond the tab stops given.
   --all
   -a			Use tabs wherever they would replace 2 or more blanks,
                        not just at the beginnings of lines.

   David MacKenzie <djm@gnu.ai.mit.edu> */

#అన్తర్భావ <config.h>

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "ioblksize.h"
#అన్తర్భావ "mbbuf.h"
#అన్తర్భావ "mcel.h"
#అన్తర్భావ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "unexpand"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")



/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  CONVERT_మొదటి_ONLY_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"tabs", required_వాదం, ఖాళీ, 't'},
  {"all", no_వాదం, ఖాళీ, 'a'},
  {"first-only", no_వాదం, ఖాళీ, CONVERT_మొదటి_ONLY_OPTION},
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
Convert blanks లోపల each FILE గమ్యం tabs, writing గమ్యం standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         convert all blanks, instead యొక్క just initial blanks\n\
"));
      oputs (_("\
      --మొదటి-only\n\
         convert only leading sequences యొక్క blanks (overrides -a)\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart instead యొక్క 8 (enables -a)\n\
"));
      emit_tab_list_info (కార్యక్రమం_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Change blanks to tabs, writing to stdout.
   Read each file in 'file_list', in order.  */

స్థిర శూన్య
unexpand (శూన్య)
{
  /* Input stream.  */
  FILE *fp = తదుపరి_file (ఖాళీ);

  /* The array of pending blanks.  In non-POSIX locales, blanks can
     include characters other than spaces, so the blanks must be
     stored, not merely counted.  */
  అక్షరం *pending_blank;

  ఒకవేళ (!fp)
    తిరిగి;

  స్థిర అక్షరం పంక్తి_లోపల[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, పంక్తి_లోపల, పరిమాణం పంక్తి_లోపల, fp);

  /* The worst case is a non-blank character, then one blank, then a
     tab stop, then MAX_COLUMN_WIDTH - 1 blanks, then a non-blank; so
     allocate MAX_COLUMN_WIDTH bytes to store the blanks.  */
  pending_blank = xinmalloc (max_స్తంభం_వెడల్పు, MB_CUR_MAX);

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

      /* Column the next input tab stop is on.  */
      colno తదుపరి_tab_స్తంభం = 0;

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_సూచీ = 0;

      /* If true, the first pending blank came just before a tab stop.  */
      bool one_blank_before_tab_stop = false;

      /* If true, the previous input character was a blank.  This is
         initially true, since initial strings of blanks are treated
         as if the line was preceded by a blank.  */
      bool prev_blank = true;

      /* Number of pending columns of blanks.  */
      idx_t pending = 0;


      /* Convert a line of text.  */

      చేయి
        {
          అయితే ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = తదుపరి_file (fp)))
            mbbuf_init (&mbbuf, పంక్తి_లోపల, పరిమాణం పంక్తి_లోపల, fp);

          ఒకవేళ (convert)
            {
              bool blank = c32issep (g.ch);

              ఒకవేళ (blank)
                {
                  bool చివరి_tab;

                  తదుపరి_tab_స్తంభం = get_తదుపరి_tab_స్తంభం (స్తంభం, &tab_సూచీ,
                                                         &చివరి_tab);

                  ఒకవేళ (చివరి_tab)
                    convert = false;

                  ఒకవేళ (convert)
                    {
                      ఒకవేళ (g.ch == '\t')
                        {
                          స్తంభం = తదుపరి_tab_స్తంభం;

                          ఒకవేళ (pending)
                            pending_blank[0] = '\t';
                        }
                      లేకపోతే
                        {
                          పూర్ణాంకం వెడల్పు = c32width (g.ch);
                          స్తంభం += వెడల్పు < 0 ? 1 : వెడల్పు;

                          ఒకవేళ (! (prev_blank && స్తంభం >= తదుపరి_tab_స్తంభం))
                            {
                              /* It is not yet known whether the pending blanks
                                 will be replaced by tabs.  */
                              ఒకవేళ (స్తంభం == తదుపరి_tab_స్తంభం)
                                one_blank_before_tab_stop = true;
                              జ్ఞాపకం_నకలు (pending_blank + pending,
                                      mbbuf_char_స్థానభ్రంశం (&mbbuf, g), g.len);
                              pending += g.len;
                              prev_blank = true;
                              కొనసాగు;
                            }

                          /* Replace the pending blanks by a tab or two.  */
                          g.len = 0;
                          ఒకవేళ (putc ('\t', stdout) < 0)
                            write_error ();
                          pending_blank[0] = '\t';
                        }

                      /* Discard pending blanks, unless it was a single
                         blank just before the previous tab stop.  */
                      pending = one_blank_before_tab_stop;
                    }
                }
              లేకపోతే ఒకవేళ (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  స్తంభం -= !!స్తంభం;
                  తదుపరి_tab_స్తంభం = స్తంభం;
                  tab_సూచీ -= !!tab_సూచీ;
                }
              లేకపోతే
                {
                  పూర్ణాంకం వెడల్పు = c32width (g.ch);
                  ఒకవేళ (ckd_కూడు (&స్తంభం, స్తంభం, వెడల్పు < 0 ? 1 : వెడల్పు))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

              ఒకవేళ (pending)
                {
                  ఒకవేళ (pending > 1 && one_blank_before_tab_stop)
                    pending_blank[0] = '\t';
                  ఒకవేళ (fwrite (pending_blank, 1, pending, stdout) != pending)
                    write_error ();
                  pending = 0;
                  one_blank_before_tab_stop = false;
                }

              prev_blank = blank;
              convert &= convert_entire_పంక్తి || blank;
            }

          ఒకవేళ (g.ch == MBBUF_EOF)
            {
              విడుదల (pending_blank);
              తిరిగి;
            }

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
  bool have_tabval = false;
  colno tabval IF_LINT ( = 0);
  పూర్ణాంకం c;

  /* If true, cancel the effect of any -a (explicit or implicit in -t),
     so that only leading blanks will be considered.  */
  bool convert_మొదటి_only = false;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((c = getopt_long (argc, argv, ",0123456789at:", longopts, ఖాళీ))
         != -1)
    {
      ఎంపిక (c)
        {
        స్థితి '?':
          usage (EXIT_FAILURE);
        స్థితి 'a':
          convert_entire_పంక్తి = true;
          ఆపు;
        స్థితి 't':
          convert_entire_పంక్తి = true;
          parse_tab_stops (optarg);
          ఆపు;
        స్థితి CONVERT_మొదటి_ONLY_OPTION:
          convert_మొదటి_only = true;
          ఆపు;
        స్థితి ',':
          ఒకవేళ (have_tabval)
            కూడు_tab_stop (tabval);
          have_tabval = false;
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          ఒకవేళ (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
            }
          ఒకవేళ (!DECIMAL_DIGIT_ACCUMULATE (tabval, c - '0'))
            error (EXIT_FAILURE, 0, _("tab stop is too large"));
          ఆపు;
        }
    }

  ఒకవేళ (convert_మొదటి_only)
    convert_entire_పంక్తి = false;

  ఒకవేళ (have_tabval)
    కూడు_tab_stop (tabval);

  finalize_tab_stops ();

  సమితి_file_list ((optind < argc) ? &argv[optind] : ఖాళీ);

  unexpand ();

  cleanup_file_list_stdin ();

  తిరిగి exit_స్థితి;
}
