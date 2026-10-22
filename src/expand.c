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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mcel.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mbbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "expand"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

ਸਥਿਰ ਅੱਖਰ ਅਚਲ shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"tabs", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"initial", no_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Convert tabs ਵਿੱਚ each FILE ਮੰਜ਼ਿਲ spaces, writing ਮੰਜ਼ਿਲ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --initial\n\
         ਕਰੋ not convert tabs after non blanks\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart, not 8\n\
"));
      emit_tab_list_info (ਪ੍ਰੋਗਰਾਮ_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}


/* Change tabs to spaces, writing to stdout.
   Read each file in 'file_list', in order.  */

ਸਥਿਰ ਖਾਲੀ
expand (ਖਾਲੀ)
{
  /* Input stream.  */
  FILE *fp = ਅਗਲਾ_file (ਨਿਰਾ);

  ਜੇ (!fp)
    ਵਾਪਸ;

  ਸਥਿਰ ਅੱਖਰ ਸਤਰ_ਵਿੱਚ[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, ਸਤਰ_ਵਿੱਚ, ਆਕਾਰ ਸਤਰ_ਵਿੱਚ, fp);

  ਜਦੋਂ (true)
    {
      /* Input character, or EOF.  */
      mcel_t g;

      /* If true, perform translations.  */
      bool convert = true;


      /* The following variables have valid values only when CONVERT
         is true:  */

      /* Column of next input character.  */
      colno ਕਾਲਮ = 0;

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_ਸੂਚਕਾਂਕ = 0;


      /* Convert a line of text.  */

      ਕਰੋ
        {
          ਜਦੋਂ ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = ਅਗਲਾ_file (fp)))
            mbbuf_init (&mbbuf, ਸਤਰ_ਵਿੱਚ, ਆਕਾਰ ਸਤਰ_ਵਿੱਚ, fp);

          ਜੇ (convert)
            {
              convert &= convert_entire_ਸਤਰ || c32issep (g.ch);

              ਜੇ (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool ਆਖਰੀ_tab;
                  colno ਅਗਲਾ_tab_ਕਾਲਮ
                    = get_ਅਗਲਾ_tab_ਕਾਲਮ (ਕਾਲਮ, &tab_ਸੂਚਕਾਂਕ, &ਆਖਰੀ_tab);

                  ਜਦੋਂ (++ਕਾਲਮ < ਅਗਲਾ_tab_ਕਾਲਮ)
                    ਜੇ (ਅੱਖਰ_ਲਿਖੋ (' ') < 0)
                      write_error ();

                  ਜੇ (ਅੱਖਰ_ਲਿਖੋ (' ') < 0)
                    write_error ();

                  ਜਾਰੀ;
                }
              ਨਹੀਂਤਾਂ ਜੇ (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  ਕਾਲਮ -= !!ਕਾਲਮ;
                  tab_ਸੂਚਕਾਂਕ -= !!tab_ਸੂਚਕਾਂਕ;
                }
              ਨਹੀਂਤਾਂ
                {
                  ਪੂਰਨ ਚੌੜਾਈ = c32width (g.ch);
                  ਜੇ (ckd_ਜੋੜੋ (&ਕਾਲਮ, ਕਾਲਮ, ਚੌੜਾਈ < 0 ? 1 : ਚੌੜਾਈ))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          ਜੇ (g.ch == MBBUF_EOF)
            ਵਾਪਸ;

          fwrite (mbbuf_char_ਵਿੱਥ (&mbbuf, g), ਆਕਾਰ (ਅੱਖਰ), g.len, stdout);
          ਜੇ (ferror (stdout))
            write_error ();
        }
      ਜਦੋਂ (g.ch != '\n');
    }
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ c;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_ਸਤਰ = true;

  ਜਦੋਂ ((c = getopt_long (argc, argv, shortopts, longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'i':
          convert_entire_ਸਤਰ = false;
          ਰੋਕੋ;

        ਹਾਲਤ 't':
          parse_tab_stops (optarg);
          ਰੋਕੋ;

        ਹਾਲਤ '0': ਹਾਲਤ '1': ਹਾਲਤ '2': ਹਾਲਤ '3': ਹਾਲਤ '4':
        ਹਾਲਤ '5': ਹਾਲਤ '6': ਹਾਲਤ '7': ਹਾਲਤ '8': ਹਾਲਤ '9':
          ਜੇ (optarg)
            parse_tab_stops (optarg - 1);
          ਨਹੀਂਤਾਂ
            {
              ਅੱਖਰ tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  ਸੈੱਟ_file_list (optind < argc ? &argv[optind] : ਨਿਰਾ);

  expand ();

  cleanup_file_list_stdin ();

  ਵਾਪਸ exit_ਸਥਿਤੀ;
}
