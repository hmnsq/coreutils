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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "ioblksize.h"
#ضمن "mbbuf.h"
#ضمن "mcel.h"
#ضمن "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کړه پروګرام_NAME "unexpand"

#تعریف_کړه AUTHORS proper_name ("David MacKenzie")



/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
تعداد
{
  CONVERT_لومړی_ONLY_OPTION = CHAR_MAX + 1
};

ساكن بنية option ثابت longopts[] =
{
  {"tabs", required_دلیل, عدم, 't'},
  {"all", no_دلیل, عدم, 'a'},
  {"first-only", no_دلیل, عدم, CONVERT_لومړی_ONLY_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروګرام_name);
      fputs (_("\
Convert blanks دننه each FILE منزل tabs, writing منزل standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         convert all blanks, instead د just initial blanks\n\
"));
      oputs (_("\
      --لومړی-only\n\
         convert only leading sequences د blanks (overrides -a)\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart instead د 8 (enables -a)\n\
"));
      emit_tab_list_info (پروګرام_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

/* Change blanks to tabs, writing to stdout.
   Read each file in 'file_list', in order.  */

ساكن فراغ
unexpand (فراغ)
{
  /* Input stream.  */
  FILE *fp = بل_file (عدم);

  /* The array of pending blanks.  In non-POSIX locales, blanks can
     include characters other than spaces, so the blanks must be
     stored, not merely counted.  */
  حرف *pending_blank;

  إذا (!fp)
    أرجع;

  ساكن حرف کرښه_دننه[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, کرښه_دننه, حجم کرښه_دننه, fp);

  /* The worst case is a non-blank character, then one blank, then a
     tab stop, then MAX_COLUMN_WIDTH - 1 blanks, then a non-blank; so
     allocate MAX_COLUMN_WIDTH bytes to store the blanks.  */
  pending_blank = xinmalloc (max_ستنه_پلنوالی, MB_CUR_MAX);

  طالما (true)
    {
      /* Input character, or EOF.  */
      mcel_t g;

      /* If true, perform translations.  */
      bool convert = true;


      /* The following variables have valid values only when CONVERT
         is true:  */

      /* Column of next input character.  */
      colno ستنه = 0;

      /* Column the next input tab stop is on.  */
      colno بل_tab_ستنه = 0;

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_شاخص = 0;

      /* If true, the first pending blank came just before a tab stop.  */
      bool one_blank_before_tab_stop = false;

      /* If true, the previous input character was a blank.  This is
         initially true, since initial strings of blanks are treated
         as if the line was preceded by a blank.  */
      bool prev_blank = true;

      /* Number of pending columns of blanks.  */
      idx_t pending = 0;


      /* Convert a line of text.  */

      افعل
        {
          طالما ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = بل_file (fp)))
            mbbuf_init (&mbbuf, کرښه_دننه, حجم کرښه_دننه, fp);

          إذا (convert)
            {
              bool blank = c32issep (g.ch);

              إذا (blank)
                {
                  bool وروستی_tab;

                  بل_tab_ستنه = get_بل_tab_ستنه (ستنه, &tab_شاخص,
                                                         &وروستی_tab);

                  إذا (وروستی_tab)
                    convert = false;

                  إذا (convert)
                    {
                      إذا (g.ch == '\t')
                        {
                          ستنه = بل_tab_ستنه;

                          إذا (pending)
                            pending_blank[0] = '\t';
                        }
                      وإلا
                        {
                          صحيح پلنوالی = c32width (g.ch);
                          ستنه += پلنوالی < 0 ? 1 : پلنوالی;

                          إذا (! (prev_blank && ستنه >= بل_tab_ستنه))
                            {
                              /* It is not yet known whether the pending blanks
                                 will be replaced by tabs.  */
                              إذا (ستنه == بل_tab_ستنه)
                                one_blank_before_tab_stop = true;
                              حافظه_نقل (pending_blank + pending,
                                      mbbuf_char_دننه_واټن (&mbbuf, g), g.len);
                              pending += g.len;
                              prev_blank = true;
                              استمر;
                            }

                          /* Replace the pending blanks by a tab or two.  */
                          g.len = 0;
                          إذا (putc ('\t', stdout) < 0)
                            write_error ();
                          pending_blank[0] = '\t';
                        }

                      /* Discard pending blanks, unless it was a single
                         blank just before the previous tab stop.  */
                      pending = one_blank_before_tab_stop;
                    }
                }
              وإلا إذا (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  ستنه -= !!ستنه;
                  بل_tab_ستنه = ستنه;
                  tab_شاخص -= !!tab_شاخص;
                }
              وإلا
                {
                  صحيح پلنوالی = c32width (g.ch);
                  إذا (ckd_جمع_کړه (&ستنه, ستنه, پلنوالی < 0 ? 1 : پلنوالی))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

              إذا (pending)
                {
                  إذا (pending > 1 && one_blank_before_tab_stop)
                    pending_blank[0] = '\t';
                  إذا (fwrite (pending_blank, 1, pending, stdout) != pending)
                    write_error ();
                  pending = 0;
                  one_blank_before_tab_stop = false;
                }

              prev_blank = blank;
              convert &= convert_entire_کرښه || blank;
            }

          إذا (g.ch == MBBUF_EOF)
            {
              حرر (pending_blank);
              أرجع;
            }

          fwrite (mbbuf_char_دننه_واټن (&mbbuf, g), حجم (حرف), g.len, stdout);
          إذا (ferror (stdout))
            write_error ();
        }
      طالما (g.ch != '\n');
    }
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  bool have_tabval = false;
  colno tabval IF_LINT ( = 0);
  صحيح c;

  /* If true, cancel the effect of any -a (explicit or implicit in -t),
     so that only leading blanks will be considered.  */
  bool convert_لومړی_only = false;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((c = getopt_long (argc, argv, ",0123456789at:", longopts, عدم))
         != -1)
    {
      بدل (c)
        {
        حالة '?':
          usage (EXIT_FAILURE);
        حالة 'a':
          convert_entire_کرښه = true;
          اكسر;
        حالة 't':
          convert_entire_کرښه = true;
          parse_tab_stops (optarg);
          اكسر;
        حالة CONVERT_لومړی_ONLY_OPTION:
          convert_لومړی_only = true;
          اكسر;
        حالة ',':
          إذا (have_tabval)
            جمع_کړه_tab_stop (tabval);
          have_tabval = false;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);
        افتراضي:
          إذا (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
            }
          إذا (!DECIMAL_DIGIT_ACCUMULATE (tabval, c - '0'))
            error (EXIT_FAILURE, 0, _("tab stop is too large"));
          اكسر;
        }
    }

  إذا (convert_لومړی_only)
    convert_entire_کرښه = false;

  إذا (have_tabval)
    جمع_کړه_tab_stop (tabval);

  finalize_tab_stops ();

  سېټ_file_list ((optind < argc) ? &argv[optind] : عدم);

  unexpand ();

  cleanup_file_list_stdin ();

  أرجع exit_حالت;
}
