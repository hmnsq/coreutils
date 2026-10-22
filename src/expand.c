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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"
#ಅನ್ತರ್ಭಾವ "mcel.h"
#ಅನ್ತರ್ಭಾವ "mbbuf.h"
#ಅನ್ತರ್ಭಾವ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "expand"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"tabs", required_ವಾದ, ಖಾಲಿ, 't'},
  {"initial", no_ವಾದ, ಖಾಲಿ, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Convert tabs ಒಳ each FILE ಗಮ್ಯ spaces, writing ಗಮ್ಯ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --initial\n\
         ಮಾಡು not convert tabs after non blanks\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart, not 8\n\
"));
      emit_tab_list_info (ಕಾರ್ಯಕ್ರಮ_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}


/* Change tabs to spaces, writing to stdout.
   Read each file in 'file_list', in order.  */

ಸ್ಥಿರ ಶೂನ್ಯ
expand (ಶೂನ್ಯ)
{
  /* Input stream.  */
  FILE *fp = ಮುಂದಿನ_file (ಖಾಲಿ);

  ಆದರೆ (!fp)
    ಹಿಂತಿರುಗಿ;

  ಸ್ಥಿರ ಅಕ್ಷರ ಸಾಲು_ಒಳ[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, ಸಾಲು_ಒಳ, ಗಾತ್ರ ಸಾಲು_ಒಳ, fp);

  ಆಗ (true)
    {
      /* Input character, or EOF.  */
      mcel_t g;

      /* If true, perform translations.  */
      bool convert = true;


      /* The following variables have valid values only when CONVERT
         is true:  */

      /* Column of next input character.  */
      colno ಸ್ತಂಭ = 0;

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_ಸೂಚ್ಯಂಕ = 0;


      /* Convert a line of text.  */

      ಮಾಡು
        {
          ಆಗ ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = ಮುಂದಿನ_file (fp)))
            mbbuf_init (&mbbuf, ಸಾಲು_ಒಳ, ಗಾತ್ರ ಸಾಲು_ಒಳ, fp);

          ಆದರೆ (convert)
            {
              convert &= convert_entire_ಸಾಲು || c32issep (g.ch);

              ಆದರೆ (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool ಕೊನೆಯ_tab;
                  colno ಮುಂದಿನ_tab_ಸ್ತಂಭ
                    = get_ಮುಂದಿನ_tab_ಸ್ತಂಭ (ಸ್ತಂಭ, &tab_ಸೂಚ್ಯಂಕ, &ಕೊನೆಯ_tab);

                  ಆಗ (++ಸ್ತಂಭ < ಮುಂದಿನ_tab_ಸ್ತಂಭ)
                    ಆದರೆ (ಅಕ್ಷರ_ಬರೆ (' ') < 0)
                      write_error ();

                  ಆದರೆ (ಅಕ್ಷರ_ಬರೆ (' ') < 0)
                    write_error ();

                  ಮುಂದುವರಿ;
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  ಸ್ತಂಭ -= !!ಸ್ತಂಭ;
                  tab_ಸೂಚ್ಯಂಕ -= !!tab_ಸೂಚ್ಯಂಕ;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಪೂರ್ಣಾಂಕ ಅಗಲ = c32width (g.ch);
                  ಆದರೆ (ckd_ಕೂಡಿಸು (&ಸ್ತಂಭ, ಸ್ತಂಭ, ಅಗಲ < 0 ? 1 : ಅಗಲ))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

            }

          ಆದರೆ (g.ch == MBBUF_EOF)
            ಹಿಂತಿರುಗಿ;

          fwrite (mbbuf_char_ಸ್ಥಾನಾಂತರ (&mbbuf, g), ಗಾತ್ರ (ಅಕ್ಷರ), g.len, stdout);
          ಆದರೆ (ferror (stdout))
            write_error ();
        }
      ಆಗ (g.ch != '\n');
    }
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ c;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);
  convert_entire_ಸಾಲು = true;

  ಆಗ ((c = getopt_long (argc, argv, shortopts, longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'i':
          convert_entire_ಸಾಲು = false;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 't':
          parse_tab_stops (optarg);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ '0': ಸ್ಥಿತಿ '1': ಸ್ಥಿತಿ '2': ಸ್ಥಿತಿ '3': ಸ್ಥಿತಿ '4':
        ಸ್ಥಿತಿ '5': ಸ್ಥಿತಿ '6': ಸ್ಥಿತಿ '7': ಸ್ಥಿತಿ '8': ಸ್ಥಿತಿ '9':
          ಆದರೆ (optarg)
            parse_tab_stops (optarg - 1);
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಅಕ್ಷರ tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  finalize_tab_stops ();

  ಗಣ_file_list (optind < argc ? &argv[optind] : ಖಾಲಿ);

  expand ();

  cleanup_file_list_stdin ();

  ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}
