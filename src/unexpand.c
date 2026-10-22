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
#تعریف_کرو پروگرام_NAME "unexpand"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")



/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
شمار
{
  CONVERT_پہلا_ONLY_OPTION = CHAR_MAX + 1
};

ساکن ساخت option ثابت longopts[] =
{
  {"tabs", required_دلیل, تہی, 't'},
  {"all", no_دلیل, تہی, 'a'},
  {"first-only", no_دلیل, تہی, CONVERT_پہلا_ONLY_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Convert blanks اندر each FILE مقصد tabs, writing مقصد standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         convert all blanks, instead کا just initial blanks\n\
"));
      oputs (_("\
      --پہلا-only\n\
         convert only leading sequences کا blanks (overrides -a)\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart instead کا 8 (enables -a)\n\
"));
      emit_tab_list_info (پروگرام_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Change blanks to tabs, writing to stdout.
   Read each file in 'file_list', in order.  */

ساکن خالی
unexpand (خالی)
{
  /* Input stream.  */
  FILE *fp = اگلا_file (تہی);

  /* The array of pending blanks.  In non-POSIX locales, blanks can
     include characters other than spaces, so the blanks must be
     stored, not merely counted.  */
  حرف *pending_blank;

  اگر (!fp)
    واپس;

  ساکن حرف سطر_اندر[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, سطر_اندر, حجم سطر_اندر, fp);

  /* The worst case is a non-blank character, then one blank, then a
     tab stop, then MAX_COLUMN_WIDTH - 1 blanks, then a non-blank; so
     allocate MAX_COLUMN_WIDTH bytes to store the blanks.  */
  pending_blank = xinmalloc (max_عمود_چوڑائی, MB_CUR_MAX);

  جب (true)
    {
      /* Input character, or EOF.  */
      mcel_t g;

      /* If true, perform translations.  */
      bool convert = true;


      /* The following variables have valid values only when CONVERT
         is true:  */

      /* Column of next input character.  */
      colno عمود = 0;

      /* Column the next input tab stop is on.  */
      colno اگلا_tab_عمود = 0;

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_اشاریہ = 0;

      /* If true, the first pending blank came just before a tab stop.  */
      bool one_blank_before_tab_stop = false;

      /* If true, the previous input character was a blank.  This is
         initially true, since initial strings of blanks are treated
         as if the line was preceded by a blank.  */
      bool پچھلا_blank = true;

      /* Number of pending columns of blanks.  */
      idx_t pending = 0;


      /* Convert a line of text.  */

      کرو
        {
          جب ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = اگلا_file (fp)))
            mbbuf_init (&mbbuf, سطر_اندر, حجم سطر_اندر, fp);

          اگر (convert)
            {
              bool blank = c32issep (g.ch);

              اگر (blank)
                {
                  bool آخری_tab;

                  اگلا_tab_عمود = get_اگلا_tab_عمود (عمود, &tab_اشاریہ,
                                                         &آخری_tab);

                  اگر (آخری_tab)
                    convert = false;

                  اگر (convert)
                    {
                      اگر (g.ch == '\t')
                        {
                          عمود = اگلا_tab_عمود;

                          اگر (pending)
                            pending_blank[0] = '\t';
                        }
                      ورنہ
                        {
                          صحیح چوڑائی = c32width (g.ch);
                          عمود += چوڑائی < 0 ? 1 : چوڑائی;

                          اگر (! (پچھلا_blank && عمود >= اگلا_tab_عمود))
                            {
                              /* It is not yet known whether the pending blanks
                                 will be replaced by tabs.  */
                              اگر (عمود == اگلا_tab_عمود)
                                one_blank_before_tab_stop = true;
                              حافظہ_نقل (pending_blank + pending,
                                      mbbuf_char_انحراف (&mbbuf, g), g.len);
                              pending += g.len;
                              پچھلا_blank = true;
                              جاری;
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
              ورنہ اگر (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  عمود -= !!عمود;
                  اگلا_tab_عمود = عمود;
                  tab_اشاریہ -= !!tab_اشاریہ;
                }
              ورنہ
                {
                  صحیح چوڑائی = c32width (g.ch);
                  اگر (ckd_جوڑو (&عمود, عمود, چوڑائی < 0 ? 1 : چوڑائی))
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

              پچھلا_blank = blank;
              convert &= convert_entire_سطر || blank;
            }

          اگر (g.ch == MBBUF_EOF)
            {
              آزاد (pending_blank);
              واپس;
            }

          fwrite (mbbuf_char_انحراف (&mbbuf, g), حجم (حرف), g.len, stdout);
          اگر (ferror (stdout))
            write_error ();
        }
      جب (g.ch != '\n');
    }
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool have_tabval = false;
  colno tabval IF_LINT ( = 0);
  صحیح c;

  /* If true, cancel the effect of any -a (explicit or implicit in -t),
     so that only leading blanks will be considered.  */
  bool convert_پہلا_only = false;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((c = getopt_long (argc, argv, ",0123456789at:", longopts, تہی))
         != -1)
    {
      انتخاب (c)
        {
        حالت '?':
          usage (EXIT_FAILURE);
        حالت 'a':
          convert_entire_سطر = true;
          توڑو;
        حالت 't':
          convert_entire_سطر = true;
          parse_tab_stops (optarg);
          توڑو;
        حالت CONVERT_پہلا_ONLY_OPTION:
          convert_پہلا_only = true;
          توڑو;
        حالت ',':
          اگر (have_tabval)
            جوڑو_tab_stop (tabval);
          have_tabval = false;
          توڑو;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          اگر (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
            }
          اگر (!DECIMAL_DIGIT_ACCUMULATE (tabval, c - '0'))
            error (EXIT_FAILURE, 0, _("tab stop is too large"));
          توڑو;
        }
    }

  اگر (convert_پہلا_only)
    convert_entire_سطر = false;

  اگر (have_tabval)
    جوڑو_tab_stop (tabval);

  finalize_tab_stops ();

  سیٹ_file_list ((optind < argc) ? &argv[optind] : تہی);

  unexpand ();

  cleanup_file_list_stdin ();

  واپس exit_حالت;
}
