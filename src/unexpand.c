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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ioblksize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mbbuf.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mcel.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "unexpand"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")



/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  CONVERT_ਪਹਿਲਾ_ONLY_OPTION = CHAR_MAX + 1
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"tabs", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"all", no_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"first-only", no_ਦਲੀਲ, ਨਿਰਾ, CONVERT_ਪਹਿਲਾ_ONLY_OPTION},
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
Convert blanks ਵਿੱਚ each FILE ਮੰਜ਼ਿਲ tabs, writing ਮੰਜ਼ਿਲ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --all\n\
         convert all blanks, instead ਦਾ just initial blanks\n\
"));
      oputs (_("\
      --ਪਹਿਲਾ-only\n\
         convert only leading sequences ਦਾ blanks (overrides -a)\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart instead ਦਾ 8 (enables -a)\n\
"));
      emit_tab_list_info (ਪ੍ਰੋਗਰਾਮ_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Change blanks to tabs, writing to stdout.
   Read each file in 'file_list', in order.  */

ਸਥਿਰ ਖਾਲੀ
unexpand (ਖਾਲੀ)
{
  /* Input stream.  */
  FILE *fp = ਅਗਲਾ_file (ਨਿਰਾ);

  /* The array of pending blanks.  In non-POSIX locales, blanks can
     include characters other than spaces, so the blanks must be
     stored, not merely counted.  */
  ਅੱਖਰ *pending_blank;

  ਜੇ (!fp)
    ਵਾਪਸ;

  ਸਥਿਰ ਅੱਖਰ ਸਤਰ_ਵਿੱਚ[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, ਸਤਰ_ਵਿੱਚ, ਆਕਾਰ ਸਤਰ_ਵਿੱਚ, fp);

  /* The worst case is a non-blank character, then one blank, then a
     tab stop, then MAX_COLUMN_WIDTH - 1 blanks, then a non-blank; so
     allocate MAX_COLUMN_WIDTH bytes to store the blanks.  */
  pending_blank = xinmalloc (max_ਕਾਲਮ_ਚੌੜਾਈ, MB_CUR_MAX);

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

      /* Column the next input tab stop is on.  */
      colno ਅਗਲਾ_tab_ਕਾਲਮ = 0;

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_ਸੂਚਕਾਂਕ = 0;

      /* If true, the first pending blank came just before a tab stop.  */
      bool one_blank_before_tab_stop = false;

      /* If true, the previous input character was a blank.  This is
         initially true, since initial strings of blanks are treated
         as if the line was preceded by a blank.  */
      bool prev_blank = true;

      /* Number of pending columns of blanks.  */
      idx_t pending = 0;


      /* Convert a line of text.  */

      ਕਰੋ
        {
          ਜਦੋਂ ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = ਅਗਲਾ_file (fp)))
            mbbuf_init (&mbbuf, ਸਤਰ_ਵਿੱਚ, ਆਕਾਰ ਸਤਰ_ਵਿੱਚ, fp);

          ਜੇ (convert)
            {
              bool blank = c32issep (g.ch);

              ਜੇ (blank)
                {
                  bool ਆਖਰੀ_tab;

                  ਅਗਲਾ_tab_ਕਾਲਮ = get_ਅਗਲਾ_tab_ਕਾਲਮ (ਕਾਲਮ, &tab_ਸੂਚਕਾਂਕ,
                                                         &ਆਖਰੀ_tab);

                  ਜੇ (ਆਖਰੀ_tab)
                    convert = false;

                  ਜੇ (convert)
                    {
                      ਜੇ (g.ch == '\t')
                        {
                          ਕਾਲਮ = ਅਗਲਾ_tab_ਕਾਲਮ;

                          ਜੇ (pending)
                            pending_blank[0] = '\t';
                        }
                      ਨਹੀਂਤਾਂ
                        {
                          ਪੂਰਨ ਚੌੜਾਈ = c32width (g.ch);
                          ਕਾਲਮ += ਚੌੜਾਈ < 0 ? 1 : ਚੌੜਾਈ;

                          ਜੇ (! (prev_blank && ਕਾਲਮ >= ਅਗਲਾ_tab_ਕਾਲਮ))
                            {
                              /* It is not yet known whether the pending blanks
                                 will be replaced by tabs.  */
                              ਜੇ (ਕਾਲਮ == ਅਗਲਾ_tab_ਕਾਲਮ)
                                one_blank_before_tab_stop = true;
                              ਸਮ੍ਰਿਤੀ_ਨਕਲ (pending_blank + pending,
                                      mbbuf_char_ਵਿੱਥ (&mbbuf, g), g.len);
                              pending += g.len;
                              prev_blank = true;
                              ਜਾਰੀ;
                            }

                          /* Replace the pending blanks by a tab or two.  */
                          g.len = 0;
                          ਜੇ (putc ('\t', stdout) < 0)
                            write_error ();
                          pending_blank[0] = '\t';
                        }

                      /* Discard pending blanks, unless it was a single
                         blank just before the previous tab stop.  */
                      pending = one_blank_before_tab_stop;
                    }
                }
              ਨਹੀਂਤਾਂ ਜੇ (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  ਕਾਲਮ -= !!ਕਾਲਮ;
                  ਅਗਲਾ_tab_ਕਾਲਮ = ਕਾਲਮ;
                  tab_ਸੂਚਕਾਂਕ -= !!tab_ਸੂਚਕਾਂਕ;
                }
              ਨਹੀਂਤਾਂ
                {
                  ਪੂਰਨ ਚੌੜਾਈ = c32width (g.ch);
                  ਜੇ (ckd_ਜੋੜੋ (&ਕਾਲਮ, ਕਾਲਮ, ਚੌੜਾਈ < 0 ? 1 : ਚੌੜਾਈ))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

              ਜੇ (pending)
                {
                  ਜੇ (pending > 1 && one_blank_before_tab_stop)
                    pending_blank[0] = '\t';
                  ਜੇ (fwrite (pending_blank, 1, pending, stdout) != pending)
                    write_error ();
                  pending = 0;
                  one_blank_before_tab_stop = false;
                }

              prev_blank = blank;
              convert &= convert_entire_ਸਤਰ || blank;
            }

          ਜੇ (g.ch == MBBUF_EOF)
            {
              ਮੁਕਤ (pending_blank);
              ਵਾਪਸ;
            }

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
  bool have_tabval = false;
  colno tabval IF_LINT ( = 0);
  ਪੂਰਨ c;

  /* If true, cancel the effect of any -a (explicit or implicit in -t),
     so that only leading blanks will be considered.  */
  bool convert_ਪਹਿਲਾ_only = false;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((c = getopt_long (argc, argv, ",0123456789at:", longopts, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ '?':
          usage (EXIT_FAILURE);
        ਹਾਲਤ 'a':
          convert_entire_ਸਤਰ = true;
          ਰੋਕੋ;
        ਹਾਲਤ 't':
          convert_entire_ਸਤਰ = true;
          parse_tab_stops (optarg);
          ਰੋਕੋ;
        ਹਾਲਤ CONVERT_ਪਹਿਲਾ_ONLY_OPTION:
          convert_ਪਹਿਲਾ_only = true;
          ਰੋਕੋ;
        ਹਾਲਤ ',':
          ਜੇ (have_tabval)
            ਜੋੜੋ_tab_stop (tabval);
          have_tabval = false;
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          ਜੇ (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
            }
          ਜੇ (!DECIMAL_DIGIT_ACCUMULATE (tabval, c - '0'))
            error (EXIT_FAILURE, 0, _("tab stop is too large"));
          ਰੋਕੋ;
        }
    }

  ਜੇ (convert_ਪਹਿਲਾ_only)
    convert_entire_ਸਤਰ = false;

  ਜੇ (have_tabval)
    ਜੋੜੋ_tab_stop (tabval);

  finalize_tab_stops ();

  ਸੈੱਟ_file_list ((optind < argc) ? &argv[optind] : ਨਿਰਾ);

  unexpand ();

  cleanup_file_list_stdin ();

  ਵਾਪਸ exit_ਸਥਿਤੀ;
}
