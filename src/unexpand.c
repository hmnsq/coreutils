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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "ioblksize.h"
#अन्तर्भाव "mbbuf.h"
#अन्तर्भाव "mcel.h"
#अन्तर्भाव "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "unexpand"

#define AUTHORS proper_name ("David MacKenzie")



/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
गणना
{
  CONVERT_FIRST_ONLY_OPTION = अक्षर_उच्च + 1
};

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"tabs", required_argument, रिक्त, 't'},
  {"all", no_argument, रिक्त, 'a'},
  {"first-only", no_argument, रिक्त, CONVERT_FIRST_ONLY_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... [खाता]...\n\
"),
              program_name);
      ख_माला_दो (_("\
Convert blanks in each खाता to tabs, writing to standard output.\n\
"), मानक_निकास);

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
  निकास (status);
}

/* Change blanks to tabs, writing to stdout.
   Read each file in 'file_list', in order.  */

स्थिर शून्य
unexpand (शून्य)
{
  /* Input stream.  */
  खाता *fp = next_file (रिक्त);

  /* The array of pending blanks.  In non-POSIX locales, blanks can
     include characters other than spaces, so the blanks must be
     stored, not merely counted.  */
  अक्षर *pending_blank;

  जर (!fp)
    परत;

  स्थिर अक्षर ओळ_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, ओळ_in, आकार ओळ_in, fp);

  /* The worst case is a non-blank character, then one blank, then a
     tab stop, then MAX_COLUMN_WIDTH - 1 blanks, then a non-blank; so
     allocate MAX_COLUMN_WIDTH bytes to store the blanks.  */
  pending_blank = xinmalloc (max_column_width, MB_CUR_MAX);

  पर्यंत (true)
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

      करा
        {
          पर्यंत ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, ओळ_in, आकार ओळ_in, fp);

          जर (convert)
            {
              bool blank = c32issep (g.ch);

              जर (blank)
                {
                  bool last_tab;

                  next_tab_column = get_next_tab_column (column, &tab_index,
                                                         &last_tab);

                  जर (last_tab)
                    convert = false;

                  जर (convert)
                    {
                      जर (g.ch == '\t')
                        {
                          column = next_tab_column;

                          जर (pending)
                            pending_blank[0] = '\t';
                        }
                      अन्यथा
                        {
                          पूर्णांक width = c32width (g.ch);
                          column += width < 0 ? 1 : width;

                          जर (! (prev_blank && column >= next_tab_column))
                            {
                              /* It is not yet known whether the pending blanks
                                 will be replaced by tabs.  */
                              जर (column == next_tab_column)
                                one_blank_before_tab_stop = true;
                              स_नकल (pending_blank + pending,
                                      mbbuf_char_offset (&mbbuf, g), g.len);
                              pending += g.len;
                              prev_blank = true;
                              सुरूठेवा;
                            }

                          /* Replace the pending blanks by a tab or two.  */
                          g.len = 0;
                          जर (अ_दो ('\t', मानक_निकास) < 0)
                            write_error ();
                          pending_blank[0] = '\t';
                        }

                      /* Discard pending blanks, unless it was a single
                         blank just before the previous tab stop.  */
                      pending = one_blank_before_tab_stop;
                    }
                }
              अन्यथा जर (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  next_tab_column = column;
                  tab_index -= !!tab_index;
                }
              अन्यथा
                {
                  पूर्णांक width = c32width (g.ch);
                  जर (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (निकास_त्रुटि, 0, _("input line is too long"));
                }

              जर (pending)
                {
                  जर (pending > 1 && one_blank_before_tab_stop)
                    pending_blank[0] = '\t';
                  जर (ख_डालो (pending_blank, 1, pending, मानक_निकास) != pending)
                    write_error ();
                  pending = 0;
                  one_blank_before_tab_stop = false;
                }

              prev_blank = blank;
              convert &= convert_entire_ओळ || blank;
            }

          जर (g.ch == MBBUF_EOF)
            {
              मुक्त (pending_blank);
              परत;
            }

          ख_डालो (mbbuf_char_offset (&mbbuf, g), आकार (अक्षर), g.len, मानक_निकास);
          जर (ख_त्रुटि (मानक_निकास))
            write_error ();
        }
      पर्यंत (g.ch != '\n');
    }
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  bool have_tabval = false;
  colno tabval IF_LINT ( = 0);
  पूर्णांक c;

  /* If true, cancel the effect of any -a (explicit or implicit in -t),
     so that only leading blanks will be considered.  */
  bool convert_first_only = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत ((c = getopt_long (argc, argv, ",0123456789at:", longopts, रिक्त))
         != -1)
    {
      निवड (c)
        {
        स्थिती '?':
          usage (निकास_त्रुटि);
        स्थिती 'a':
          convert_entire_ओळ = true;
          थांबा;
        स्थिती 't':
          convert_entire_ओळ = true;
          parse_tab_stops (optarg);
          थांबा;
        स्थिती CONVERT_FIRST_ONLY_OPTION:
          convert_first_only = true;
          थांबा;
        स्थिती ',':
          जर (have_tabval)
            add_tab_stop (tabval);
          have_tabval = false;
          थांबा;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        पूर्वनिर्धारित:
          जर (!have_tabval)
            {
              tabval = 0;
              have_tabval = true;
            }
          जर (!DECIMAL_DIGIT_ACCUMULATE (tabval, c - '0'))
            error (निकास_त्रुटि, 0, _("tab stop is too large"));
          थांबा;
        }
    }

  जर (convert_first_only)
    convert_entire_ओळ = false;

  जर (have_tabval)
    add_tab_stop (tabval);

  finalize_tab_stops ();

  set_file_list ((optind < argc) ? &argv[optind] : रिक्त);

  unexpand ();

  cleanup_file_list_stdin ();

  परत exit_status;
}
