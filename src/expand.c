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

#ضمن <config.h>

#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "ioblksize.h"
#ضمن "mcel.h"
#ضمن "mbbuf.h"
#ضمن "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "expand"

#وصف_ڪريو AUTHORS proper_name ("David MacKenzie")

ساكن حرف ثابت shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

ساكن بنية option ثابت longopts[] =
{
  {"tabs", required_دليل, عدم, 't'},
  {"initial", no_دليل, عدم, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              پروگرام_name);
      fputs (_("\
Convert tabs اندريون each FILE منزل spaces, writing منزل standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --initial\n\
         افعل not convert tabs after non blanks\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart, not 8\n\
"));
      emit_tab_list_info (پروگرام_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}


/* Change tabs to spaces, writing to stdout.
   Read each file in 'file_list', in order.  */

ساكن فراغ
expand (فراغ)
{
  /* Input stream.  */
  FILE *fp = ايندڙ_file (عدم);

  إذا (!fp)
    أرجع;

  ساكن حرف سٽ_اندريون[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, سٽ_اندريون, حجم سٽ_اندريون, fp);

  طالما (true)
    {
      /* Input character, or EOF.  */
      mcel_t g;

      /* If true, perform translations.  */
      bool convert = true;


      /* The following variables have valid values only when CONVERT
         is true:  */

      /* Column of next input character.  */
      colno ڪالم = 0;

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_اشاريو = 0;


      /* Convert a line of text.  */

      افعل
        {
          طالما ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = ايندڙ_file (fp)))
            mbbuf_init (&mbbuf, سٽ_اندريون, حجم سٽ_اندريون, fp);

          إذا (convert)
            {
              convert &= convert_entire_سٽ || c32issep (g.ch);

              إذا (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool آخري_tab;
                  colno ايندڙ_tab_ڪالم
                    = get_ايندڙ_tab_ڪالم (ڪالم, &tab_اشاريو, &آخري_tab);

                  طالما (++ڪالم < ايندڙ_tab_ڪالم)
                    إذا (اکر_لکو (' ') < 0)
                      write_error ();

                  إذا (اکر_لکو (' ') < 0)
                    write_error ();

                  استمر;
                }
              وإلا إذا (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  ڪالم -= !!ڪالم;
                  tab_اشاريو -= !!tab_اشاريو;
                }
              وإلا
                {
                  صحيح ويڪر = c32width (g.ch);
                  إذا (ckd_جوڙيو (&ڪالم, ڪالم, ويڪر < 0 ? 1 : ويڪر))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          إذا (g.ch == MBBUF_EOF)
            أرجع;

          fwrite (mbbuf_char_وٿي (&mbbuf, g), حجم (حرف), g.len, stdout);
          إذا (ferror (stdout))
            write_error ();
        }
      طالما (g.ch != '\n');
    }
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح c;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_سٽ = true;

  طالما ((c = getopt_long (argc, argv, shortopts, longopts, عدم)) != -1)
    {
      بدل (c)
        {
        حالة 'i':
          convert_entire_سٽ = false;
          اكسر;

        حالة 't':
          parse_tab_stops (optarg);
          اكسر;

        حالة '0': حالة '1': حالة '2': حالة '3': حالة '4':
        حالة '5': حالة '6': حالة '7': حالة '8': حالة '9':
          إذا (optarg)
            parse_tab_stops (optarg - 1);
          وإلا
            {
              حرف tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  سيٽ_file_list (optind < argc ? &argv[optind] : عدم);

  expand ();

  cleanup_file_list_stdin ();

  أرجع exit_حالت;
}
