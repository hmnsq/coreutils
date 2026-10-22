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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "ioblksize.h"
#ಅನ್ತರ್ಭಾವ "mbbuf.h"
#ಅನ್ತರ್ಭಾವ "mcel.h"
#ಅನ್ತರ್ಭಾವ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "unexpand"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")



/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  CONVERT_ಮೊದಲ_ONLY_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"tabs", required_ವಾದ, ಖಾಲಿ, 't'},
  {"all", no_ವಾದ, ಖಾಲಿ, 'a'},
  {"first-only", no_ವಾದ, ಖಾಲಿ, CONVERT_ಮೊದಲ_ONLY_OPTION},
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
Convert blanks ಒಳ each FILE ಗಮ್ಯ tabs, writing ಗಮ್ಯ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         convert all blanks, instead ದ just initial blanks\n\
"));
      oputs (_("\
      --ಮೊದಲ-only\n\
         convert only leading sequences ದ blanks (overrides -a)\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart instead ದ 8 (enables -a)\n\
"));
      emit_tab_list_info (ಕಾರ್ಯಕ್ರಮ_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Change blanks to tabs, writing to stdout.
   Read each file in 'file_list', in order.  */

ಸ್ಥಿರ ಶೂನ್ಯ
unexpand (ಶೂನ್ಯ)
{
  /* Input stream.  */
  FILE *fp = ಮುಂದಿನ_file (ಖಾಲಿ);

  /* The array of pending blanks.  In non-POSIX locales, blanks can
     include characters other than spaces, so the blanks must be
     stored, not merely counted.  */
  ಅಕ್ಷರ *pending_blank;

  ಆದರೆ (!fp)
    ಹಿಂತಿರುಗಿ;

  ಸ್ಥಿರ ಅಕ್ಷರ ಸಾಲು_ಒಳ[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, ಸಾಲು_ಒಳ, ಗಾತ್ರ ಸಾಲು_ಒಳ, fp);

  /* The worst case is a non-blank character, then one blank, then a
     tab stop, then MAX_COLUMN_WIDTH - 1 blanks, then a non-blank; so
     allocate MAX_COLUMN_WIDTH bytes to store the blanks.  */
  pending_blank = xinmalloc (max_ಸ್ತಂಭ_ಅಗಲ, MB_CUR_MAX);

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

      /* Column the next input tab stop is on.  */
      colno ಮುಂದಿನ_tab_ಸ್ತಂಭ = 0;

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_ಸೂಚ್ಯಂಕ = 0;

      /* If true, the first pending blank came just before a tab stop.  */
      bool one_blank_before_tab_stop = false;

      /* If true, the previous input character was a blank.  This is
         initially true, since initial strings of blanks are treated
         as if the line was preceded by a blank.  */
      bool prev_blank = true;

      /* Number of pending columns of blanks.  */
      idx_t pending = 0;


      /* Convert a line of text.  */

      ಮಾಡು
        {
          ಆಗ ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = ಮುಂದಿನ_file (fp)))
            mbbuf_init (&mbbuf, ಸಾಲು_ಒಳ, ಗಾತ್ರ ಸಾಲು_ಒಳ, fp);

          ಆದರೆ (convert)
            {
              bool blank = c32issep (g.ch);

              ಆದರೆ (blank)
                {
                  bool ಕೊನೆಯ_tab;

                  ಮುಂದಿನ_tab_ಸ್ತಂಭ = get_ಮುಂದಿನ_tab_ಸ್ತಂಭ (ಸ್ತಂಭ, &tab_ಸೂಚ್ಯಂಕ,
                                                         &ಕೊನೆಯ_tab);

                  ಆದರೆ (ಕೊನೆಯ_tab)
                    convert = false;

                  ಆದರೆ (convert)
                    {
                      ಆದರೆ (g.ch == '\t')
                        {
                          ಸ್ತಂಭ = ಮುಂದಿನ_tab_ಸ್ತಂಭ;

                          ಆದರೆ (pending)
                            pending_blank[0] = '\t';
                        }
                      ಇಲ್ಲದಿದ್ದರೆ
                        {
                          ಪೂರ್ಣಾಂಕ ಅಗಲ = c32width (g.ch);
                          ಸ್ತಂಭ += ಅಗಲ < 0 ? 1 : ಅಗಲ;

                          ಆದರೆ (! (prev_blank && ಸ್ತಂಭ >= ಮುಂದಿನ_tab_ಸ್ತಂಭ))
                            {
                              /* It is not yet known whether the pending blanks
                                 will be replaced by tabs.  */
                              ಆದರೆ (ಸ್ತಂಭ == ಮುಂದಿನ_tab_ಸ್ತಂಭ)
                                one_blank_before_tab_stop = true;
                              ಸ್ಮರಣೆ_ನಕಲು (pending_blank + pending,
                                      mbbuf_char_ಸ್ಥಾನಾಂತರ (&mbbuf, g), g.len);
                              pending += g.len;
                              prev_blank = true;
                              ಮುಂದುವರಿ;
                            }

                          /* Replace the pending blanks by a tab or two.  */
                          g.len = 0;
                          ಆದರೆ (putc ('\t', stdout) < 0)
                            write_error ();
                          pending_blank[0] = '\t';
                        }

                      /* Discard pending blanks, unless it was a single
                         blank just before the previous tab stop.  */
                      pending = one_blank_before_tab_stop;
                    }
                }
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  ಸ್ತಂಭ -= !!ಸ್ತಂಭ;
                  ಮುಂದಿನ_tab_ಸ್ತಂಭ = ಸ್ತಂಭ;
                  tab_ಸೂಚ್ಯಂಕ -= !!tab_ಸೂಚ್ಯಂಕ;
                }
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಪೂರ್ಣಾಂಕ ಅಗಲ = c32width (g.ch);
                  ಆದರೆ (ckd_ಕೂಡಿಸು (&ಸ್ತಂಭ, ಸ್ತಂಭ, ಅಗಲ < 0 ? 1 : ಅಗಲ))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

              ಆದರೆ (pending)
                {
                  ಆದರೆ (pending > 1 && one_blank_before_tab_stop)
                    pending_blank[0] = '\t';
                  ಆದರೆ (fwrite (pending_blank, 1, pending, stdout) != pending)
                    write_error ();
                  pending = 0;
                  one_blank_before_tab_stop = false;
                }

              prev_blank = blank;
              convert &= convert_entire_ಸಾಲು || blank;
            }

          ಆದರೆ (g.ch == MBBUF_EOF)
            {
              ಬಿಡುಗಡೆ (pending_blank);
              ಹಿಂತಿರುಗಿ;
            }

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
  bool have_tabval = false;
  colno tabval IF_LINT ( = 0);
  ಪೂರ್ಣಾಂಕ c;

  /* If true, cancel the effect of any -a (explicit or implicit in -t),
     so that only leading blanks will be considered.  */
  bool convert_ಮೊದಲ_only = false;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((c = getopt_long (argc, argv, ",0123456789at:", longopts, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ '?':
          usage (EXIT_FAILURE);
        ಸ್ಥಿತಿ 'a':
          convert_entire_ಸಾಲು = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 't':
          convert_entire_ಸಾಲು = true;
          parse_tab_stops (optarg);
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ CONVERT_ಮೊದಲ_ONLY_OPTION:
          convert_ಮೊದಲ_only = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ ',':
          ಆದರೆ (have_tabval)
            ಕೂಡಿಸು_tab_stop (tabval);
          have_tabval = false;
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          ಆದರೆ (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
            }
          ಆದರೆ (!DECIMAL_DIGIT_ACCUMULATE (tabval, c - '0'))
            error (EXIT_FAILURE, 0, _("tab stop is too large"));
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (convert_ಮೊದಲ_only)
    convert_entire_ಸಾಲು = false;

  ಆದರೆ (have_tabval)
    ಕೂಡಿಸು_tab_stop (tabval);

  finalize_tab_stops ();

  ಗಣ_file_list ((optind < argc) ? &argv[optind] : ಖಾಲಿ);

  unexpand ();

  cleanup_file_list_stdin ();

  ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}
