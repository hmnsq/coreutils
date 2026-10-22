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

#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "ioblksize.h"
#شامل "mbbuf.h"
#شامل "mcel.h"
#شامل "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "unexpand"

#define AUTHORS proper_name ("David MacKenzie")



/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمارش
{
  CONVERT_FIRST_ONLY_OPTION = CHAR_MAX + 1
};

ایستا ساختار option ثابت longopts[] =
{
  {"tabs", required_argument, پوچ, 't'},
  {"all", no_argument, پوچ, 'a'},
  {"first-only", no_argument, پوچ, CONVERT_FIRST_ONLY_OPTION},
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

ایستا تهی
unexpand (تهی)
{
  /* Input stream.  */
  FILE *fp = next_file (پوچ);

  /* The array of pending blanks.  In non-POSIX locales, blanks can
     include characters other than spaces, so the blanks must be
     stored, not merely counted.  */
  نویسه *pending_blank;

  اگر (!fp)
    بازگشت;

  ایستا نویسه سطر_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, سطر_in, اندازه سطر_in, fp);

  /* The worst case is a non-blank character, then one blank, then a
     tab stop, then MAX_COLUMN_WIDTH - 1 blanks, then a non-blank; so
     allocate MAX_COLUMN_WIDTH bytes to store the blanks.  */
  pending_blank = xinmalloc (max_column_width, MB_CUR_MAX);

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

      انجامبده
        {
          وقتی ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, سطر_in, اندازه سطر_in, fp);

          اگر (convert)
            {
              bool blank = c32issep (g.ch);

              اگر (blank)
                {
                  bool last_tab;

                  next_tab_column = get_next_tab_column (column, &tab_index,
                                                         &last_tab);

                  اگر (last_tab)
                    convert = false;

                  اگر (convert)
                    {
                      اگر (g.ch == '\t')
                        {
                          column = next_tab_column;

                          اگر (pending)
                            pending_blank[0] = '\t';
                        }
                      وگرنه
                        {
                          صحیح width = c32width (g.ch);
                          column += width < 0 ? 1 : width;

                          اگر (! (prev_blank && column >= next_tab_column))
                            {
                              /* It is not yet known whether the pending blanks
                                 will be replaced by tabs.  */
                              اگر (column == next_tab_column)
                                one_blank_before_tab_stop = true;
                              memcpy (pending_blank + pending,
                                      mbbuf_char_offset (&mbbuf, g), g.len);
                              pending += g.len;
                              prev_blank = true;
                              ادامه;
                            }

                          /* Replace the pending blanks by a tab or two.  */
                          g.len = 0;
                          اگر (putc ('\t', stdout) < 0)
                            write_error ();
                          pending_blank[0] = '\t';
                        }

                      /* Discard pending blanks, unless it was a single
                         blank just before the previous tab stop.  */
                      pending = one_blank_before_tab_stop;
                    }
                }
              وگرنه اگر (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  next_tab_column = column;
                  tab_index -= !!tab_index;
                }
              وگرنه
                {
                  صحیح width = c32width (g.ch);
                  اگر (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

              اگر (pending)
                {
                  اگر (pending > 1 && one_blank_before_tab_stop)
                    pending_blank[0] = '\t';
                  اگر (fwrite (pending_blank, 1, pending, stdout) != pending)
                    write_error ();
                  pending = 0;
                  one_blank_before_tab_stop = false;
                }

              prev_blank = blank;
              convert &= convert_entire_سطر || blank;
            }

          اگر (g.ch == MBBUF_EOF)
            {
              آزاد (pending_blank);
              بازگشت;
            }

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
  bool have_tabval = false;
  colno tabval IF_LINT ( = 0);
  صحیح c;

  /* If true, cancel the effect of any -a (explicit or implicit in -t),
     so that only leading blanks will be considered.  */
  bool convert_first_only = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی ((c = getopt_long (argc, argv, ",0123456789at:", longopts, پوچ))
         != -1)
    {
      انتخاب (c)
        {
        حالت '?':
          usage (EXIT_FAILURE);
        حالت 'a':
          convert_entire_سطر = true;
          بشکن;
        حالت 't':
          convert_entire_سطر = true;
          parse_tab_stops (optarg);
          بشکن;
        حالت CONVERT_FIRST_ONLY_OPTION:
          convert_first_only = true;
          بشکن;
        حالت ',':
          اگر (have_tabval)
            add_tab_stop (tabval);
          have_tabval = false;
          بشکن;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          اگر (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
            }
          اگر (!DECIMAL_DIGIT_ACCUMULATE (tabval, c - '0'))
            error (EXIT_FAILURE, 0, _("tab stop is too large"));
          بشکن;
        }
    }

  اگر (convert_first_only)
    convert_entire_سطر = false;

  اگر (have_tabval)
    add_tab_stop (tabval);

  finalize_tab_stops ();

  set_file_list ((optind < argc) ? &argv[optind] : پوچ);

  unexpand ();

  cleanup_file_list_stdin ();

  بازگشت exit_status;
}
