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

#অন্তর্ভাৱ <config.h>

#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "ioblksize.h"
#অন্তর্ভাৱ "mbbuf.h"
#অন্তর্ভাৱ "mcel.h"
#অন্তর্ভাৱ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "unexpand"

#define AUTHORS proper_name ("David MacKenzie")



/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  CONVERT_FIRST_ONLY_OPTION = CHAR_MAX + 1
};

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {"tabs", required_argument, রিক্ত, 't'},
  {"all", no_argument, রিক্ত, 'a'},
  {"first-only", no_argument, রিক্ত, CONVERT_FIRST_ONLY_OPTION},
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
Convert blanks in each FILE to tabs, writing to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         convert all blanks, instead of just initial blanks\n\
"));
      oputs (_("\
      --first-only\n\
         convert only leading sequences of blanks (overrides -a)\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart instead of 8 (enables -a)\n\
"));
      emit_tab_list_info (PROGRAM_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Change blanks to tabs, writing to stdout.
   Read each file in 'file_list', in order.  */

স্থির শূন্য
unexpand (শূন্য)
{
  /* Input stream.  */
  FILE *fp = next_file (রিক্ত);

  /* The array of pending blanks.  In non-POSIX locales, blanks can
     include characters other than spaces, so the blanks must be
     stored, not merely counted.  */
  ৱর্ণ *pending_blank;

  যদি (!fp)
    ঘূৰাই;

  স্থির ৱর্ণ সাৰি_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, সাৰি_in, পরিমাণ সাৰি_in, fp);

  /* The worst case is a non-blank character, then one blank, then a
     tab stop, then MAX_COLUMN_WIDTH - 1 blanks, then a non-blank; so
     allocate MAX_COLUMN_WIDTH bytes to store the blanks.  */
  pending_blank = xinmalloc (max_column_width, MB_CUR_MAX);

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

      /* Column the next input tab stop is on.  */
      colno next_tab_column = 0;

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_index = 0;

      /* If true, the first pending blank came just before a tab stop.  */
      bool one_blank_before_tab_stop = false;

      /* If true, the previous input character was a blank.  This is
         initially true, since initial strings of blanks are treated
         as if the line was preceded by a blank.  */
      bool prev_blank = true;

      /* Number of pending columns of blanks.  */
      idx_t pending = 0;


      /* Convert a line of text.  */

      কৰা
        {
          যেতিয়ালৈ ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, সাৰি_in, পরিমাণ সাৰি_in, fp);

          যদি (convert)
            {
              bool blank = c32issep (g.ch);

              যদি (blank)
                {
                  bool last_tab;

                  next_tab_column = get_next_tab_column (column, &tab_index,
                                                         &last_tab);

                  যদি (last_tab)
                    convert = false;

                  যদি (convert)
                    {
                      যদি (g.ch == '\t')
                        {
                          column = next_tab_column;

                          যদি (pending)
                            pending_blank[0] = '\t';
                        }
                      অন্যথা
                        {
                          পূর্ণাঙ্ক width = c32width (g.ch);
                          column += width < 0 ? 1 : width;

                          যদি (! (prev_blank && column >= next_tab_column))
                            {
                              /* It is not yet known whether the pending blanks
                                 will be replaced by tabs.  */
                              যদি (column == next_tab_column)
                                one_blank_before_tab_stop = true;
                              memcpy (pending_blank + pending,
                                      mbbuf_char_offset (&mbbuf, g), g.len);
                              pending += g.len;
                              prev_blank = true;
                              অনুৱর্ত;
                            }

                          /* Replace the pending blanks by a tab or two.  */
                          g.len = 0;
                          যদি (putc ('\t', stdout) < 0)
                            write_error ();
                          pending_blank[0] = '\t';
                        }

                      /* Discard pending blanks, unless it was a single
                         blank just before the previous tab stop.  */
                      pending = one_blank_before_tab_stop;
                    }
                }
              অন্যথা যদি (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  next_tab_column = column;
                  tab_index -= !!tab_index;
                }
              অন্যথা
                {
                  পূর্ণাঙ্ক width = c32width (g.ch);
                  যদি (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

              যদি (pending)
                {
                  যদি (pending > 1 && one_blank_before_tab_stop)
                    pending_blank[0] = '\t';
                  যদি (fwrite (pending_blank, 1, pending, stdout) != pending)
                    write_error ();
                  pending = 0;
                  one_blank_before_tab_stop = false;
                }

              prev_blank = blank;
              convert &= convert_entire_সাৰি || blank;
            }

          যদি (g.ch == MBBUF_EOF)
            {
              মুক্ত (pending_blank);
              ঘূৰাই;
            }

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
  bool have_tabval = false;
  colno tabval IF_LINT ( = 0);
  পূর্ণাঙ্ক c;

  /* If true, cancel the effect of any -a (explicit or implicit in -t),
     so that only leading blanks will be considered.  */
  bool convert_first_only = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ ((c = getopt_long (argc, argv, ",0123456789at:", longopts, রিক্ত))
         != -1)
    {
      ৱিকল্প (c)
        {
        পক্ষ '?':
          usage (EXIT_FAILURE);
        পক্ষ 'a':
          convert_entire_সাৰি = true;
          ভঙ্গ;
        পক্ষ 't':
          convert_entire_সাৰি = true;
          parse_tab_stops (optarg);
          ভঙ্গ;
        পক্ষ CONVERT_FIRST_ONLY_OPTION:
          convert_first_only = true;
          ভঙ্গ;
        পক্ষ ',':
          যদি (have_tabval)
            add_tab_stop (tabval);
          have_tabval = false;
          ভঙ্গ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
          যদি (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
            }
          যদি (!DECIMAL_DIGIT_ACCUMULATE (tabval, c - '0'))
            error (EXIT_FAILURE, 0, _("tab stop is too large"));
          ভঙ্গ;
        }
    }

  যদি (convert_first_only)
    convert_entire_সাৰি = false;

  যদি (have_tabval)
    add_tab_stop (tabval);

  finalize_tab_stops ();

  set_file_list ((optind < argc) ? &argv[optind] : রিক্ত);

  unexpand ();

  cleanup_file_list_stdin ();

  ঘূৰাই exit_status;
}
