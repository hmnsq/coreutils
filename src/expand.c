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

#شامل <config.h>

#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "ioblksize.h"
#شامل "mcel.h"
#شامل "mbbuf.h"
#شامل "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "expand"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

ساکن حرف ثابت shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

ساکن ساخت option ثابت longopts[] =
{
  {"tabs", required_دلیل, تہی, 't'},
  {"initial", no_دلیل, تہی, 'i'},
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
Convert tabs اندر each FILE مقصد spaces, writing مقصد standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --initial\n\
         کرو not convert tabs after non blanks\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart, not 8\n\
"));
      emit_tab_list_info (پروگرام_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}


/* Change tabs to spaces, writing to stdout.
   Read each file in 'file_list', in order.  */

ساکن خالی
expand (خالی)
{
  /* Input stream.  */
  FILE *fp = اگلا_file (تہی);

  اگر (!fp)
    واپس;

  ساکن حرف سطر_اندر[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, سطر_اندر, حجم سطر_اندر, fp);

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

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_اشاریہ = 0;


      /* Convert a line of text.  */

      کرو
        {
          جب ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = اگلا_file (fp)))
            mbbuf_init (&mbbuf, سطر_اندر, حجم سطر_اندر, fp);

          اگر (convert)
            {
              convert &= convert_entire_سطر || c32issep (g.ch);

              اگر (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool آخری_tab;
                  colno اگلا_tab_عمود
                    = get_اگلا_tab_عمود (عمود, &tab_اشاریہ, &آخری_tab);

                  جب (++عمود < اگلا_tab_عمود)
                    اگر (حرف_لکھو (' ') < 0)
                      write_error ();

                  اگر (حرف_لکھو (' ') < 0)
                    write_error ();

                  جاری;
                }
              ورنہ اگر (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  عمود -= !!عمود;
                  tab_اشاریہ -= !!tab_اشاریہ;
                }
              ورنہ
                {
                  صحیح چوڑائی = c32width (g.ch);
                  اگر (ckd_جوڑو (&عمود, عمود, چوڑائی < 0 ? 1 : چوڑائی))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          اگر (g.ch == MBBUF_EOF)
            واپس;

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
  صحیح c;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_سطر = true;

  جب ((c = getopt_long (argc, argv, shortopts, longopts, تہی)) != -1)
    {
      انتخاب (c)
        {
        حالت 'i':
          convert_entire_سطر = false;
          توڑو;

        حالت 't':
          parse_tab_stops (optarg);
          توڑو;

        حالت '0': حالت '1': حالت '2': حالت '3': حالت '4':
        حالت '5': حالت '6': حالت '7': حالت '8': حالت '9':
          اگر (optarg)
            parse_tab_stops (optarg - 1);
          ورنہ
            {
              حرف tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  سیٹ_file_list (optind < argc ? &argv[optind] : تہی);

  expand ();

  cleanup_file_list_stdin ();

  واپس exit_حالت;
}
