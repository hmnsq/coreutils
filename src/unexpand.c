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

#അന്തര്ഭാവ <config.h>

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "ioblksize.h"
#അന്തര്ഭാവ "mbbuf.h"
#അന്തര്ഭാവ "mcel.h"
#അന്തര്ഭാവ "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "unexpand"

#define AUTHORS proper_name ("David MacKenzie")



/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
എണ്ണൽ
{
  CONVERT_FIRST_ONLY_OPTION = CHAR_MAX + 1
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"tabs", required_argument, ശൂന്യ, 't'},
  {"all", no_argument, ശൂന്യ, 'a'},
  {"first-only", no_argument, ശൂന്യ, CONVERT_FIRST_ONLY_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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

സ്ഥിതം ശൂന്യം
unexpand (ശൂന്യം)
{
  /* Input stream.  */
  FILE *fp = next_file (ശൂന്യ);

  /* The array of pending blanks.  In non-POSIX locales, blanks can
     include characters other than spaces, so the blanks must be
     stored, not merely counted.  */
  അക്ഷരം *pending_blank;

  എങ്കിൽ (!fp)
    തിരികെ;

  സ്ഥിതം അക്ഷരം വരി_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, വരി_in, വലുപ്പം വരി_in, fp);

  /* The worst case is a non-blank character, then one blank, then a
     tab stop, then MAX_COLUMN_WIDTH - 1 blanks, then a non-blank; so
     allocate MAX_COLUMN_WIDTH bytes to store the blanks.  */
  pending_blank = xinmalloc (max_column_width, MB_CUR_MAX);

  ആകുമ്പോൾ (true)
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

      ചെയ്യുക
        {
          ആകുമ്പോൾ ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, വരി_in, വലുപ്പം വരി_in, fp);

          എങ്കിൽ (convert)
            {
              bool blank = c32issep (g.ch);

              എങ്കിൽ (blank)
                {
                  bool last_tab;

                  next_tab_column = get_next_tab_column (column, &tab_index,
                                                         &last_tab);

                  എങ്കിൽ (last_tab)
                    convert = false;

                  എങ്കിൽ (convert)
                    {
                      എങ്കിൽ (g.ch == '\t')
                        {
                          column = next_tab_column;

                          എങ്കിൽ (pending)
                            pending_blank[0] = '\t';
                        }
                      അല്ലെങ്കിൽ
                        {
                          പൂർണ്ണസംഖ്യ width = c32width (g.ch);
                          column += width < 0 ? 1 : width;

                          എങ്കിൽ (! (prev_blank && column >= next_tab_column))
                            {
                              /* It is not yet known whether the pending blanks
                                 will be replaced by tabs.  */
                              എങ്കിൽ (column == next_tab_column)
                                one_blank_before_tab_stop = true;
                              memcpy (pending_blank + pending,
                                      mbbuf_char_offset (&mbbuf, g), g.len);
                              pending += g.len;
                              prev_blank = true;
                              തുടരുക;
                            }

                          /* Replace the pending blanks by a tab or two.  */
                          g.len = 0;
                          എങ്കിൽ (putc ('\t', stdout) < 0)
                            write_error ();
                          pending_blank[0] = '\t';
                        }

                      /* Discard pending blanks, unless it was a single
                         blank just before the previous tab stop.  */
                      pending = one_blank_before_tab_stop;
                    }
                }
              അല്ലെങ്കിൽ എങ്കിൽ (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  next_tab_column = column;
                  tab_index -= !!tab_index;
                }
              അല്ലെങ്കിൽ
                {
                  പൂർണ്ണസംഖ്യ width = c32width (g.ch);
                  എങ്കിൽ (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (EXIT_FAILURE, 0, _("input line is too long"));
                }

              എങ്കിൽ (pending)
                {
                  എങ്കിൽ (pending > 1 && one_blank_before_tab_stop)
                    pending_blank[0] = '\t';
                  എങ്കിൽ (fwrite (pending_blank, 1, pending, stdout) != pending)
                    write_error ();
                  pending = 0;
                  one_blank_before_tab_stop = false;
                }

              prev_blank = blank;
              convert &= convert_entire_വരി || blank;
            }

          എങ്കിൽ (g.ch == MBBUF_EOF)
            {
              സ്വതന്ത്രം (pending_blank);
              തിരികെ;
            }

          fwrite (mbbuf_char_offset (&mbbuf, g), വലുപ്പം (അക്ഷരം), g.len, stdout);
          എങ്കിൽ (ferror (stdout))
            write_error ();
        }
      ആകുമ്പോൾ (g.ch != '\n');
    }
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  bool have_tabval = false;
  colno tabval IF_LINT ( = 0);
  പൂർണ്ണസംഖ്യ c;

  /* If true, cancel the effect of any -a (explicit or implicit in -t),
     so that only leading blanks will be considered.  */
  bool convert_first_only = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, ",0123456789at:", longopts, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ '?':
          usage (EXIT_FAILURE);
        അവസ്ഥ 'a':
          convert_entire_വരി = true;
          നിർത്തുക;
        അവസ്ഥ 't':
          convert_entire_വരി = true;
          parse_tab_stops (optarg);
          നിർത്തുക;
        അവസ്ഥ CONVERT_FIRST_ONLY_OPTION:
          convert_first_only = true;
          നിർത്തുക;
        അവസ്ഥ ',':
          എങ്കിൽ (have_tabval)
            add_tab_stop (tabval);
          have_tabval = false;
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          എങ്കിൽ (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
            }
          എങ്കിൽ (!DECIMAL_DIGIT_ACCUMULATE (tabval, c - '0'))
            error (EXIT_FAILURE, 0, _("tab stop is too large"));
          നിർത്തുക;
        }
    }

  എങ്കിൽ (convert_first_only)
    convert_entire_വരി = false;

  എങ്കിൽ (have_tabval)
    add_tab_stop (tabval);

  finalize_tab_stops ();

  set_file_list ((optind < argc) ? &argv[optind] : ശൂന്യ);

  unexpand ();

  cleanup_file_list_stdin ();

  തിരികെ exit_status;
}
