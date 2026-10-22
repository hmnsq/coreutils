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

#अन्तर्भाव <config.h>

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "ioblksize.h"
#अन्तर्भाव "mcel.h"
#अन्तर्भाव "mbbuf.h"
#अन्तर्भाव "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "expand"

#define AUTHORS proper_name ("David MacKenzie")

स्थिर अक्षर स्थिरांक shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"tabs", required_argument, रिक्त, 't'},
  {"initial", no_argument, रिक्त, 'i'},
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
Convert tabs in each खाता to spaces, writing to standard output.\n\
"), मानक_निकास);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -i, --initial\n\
         करा not convert tabs after non blanks\n\
"));
      oputs (_("\
  -t, --tabs=N\n\
         have tabs N characters apart, not 8\n\
"));
      emit_tab_list_info (PROGRAM_NAME);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}


/* Change tabs to spaces, writing to stdout.
   Read each file in 'file_list', in order.  */

स्थिर शून्य
expand (शून्य)
{
  /* Input stream.  */
  खाता *fp = next_file (रिक्त);

  जर (!fp)
    परत;

  स्थिर अक्षर ओळ_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, ओळ_in, आकार ओळ_in, fp);

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

      /* Index in TAB_LIST of next tab stop to examine.  */
      idx_t tab_index = 0;


      /* Convert a line of text.  */

      करा
        {
          पर्यंत ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = next_file (fp)))
            mbbuf_init (&mbbuf, ओळ_in, आकार ओळ_in, fp);

          जर (convert)
            {
              convert &= convert_entire_ओळ || c32issep (g.ch);

              जर (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool last_tab;
                  colno next_tab_column
                    = get_next_tab_column (column, &tab_index, &last_tab);

                  पर्यंत (++column < next_tab_column)
                    जर (अक्षर_दो (' ') < 0)
                      write_error ();

                  जर (अक्षर_दो (' ') < 0)
                    write_error ();

                  सुरूठेवा;
                }
              अन्यथा जर (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  tab_index -= !!tab_index;
                }
              अन्यथा
                {
                  पूर्णांक width = c32width (g.ch);
                  जर (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (निकास_त्रुटि, 0, _("input line is too long"));
                }

            }

          जर (g.ch == MBBUF_EOF)
            परत;

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
  पूर्णांक c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);
  convert_entire_ओळ = true;

  पर्यंत ((c = getopt_long (argc, argv, shortopts, longopts, रिक्त)) != -1)
    {
      निवड (c)
        {
        स्थिती 'i':
          convert_entire_ओळ = false;
          थांबा;

        स्थिती 't':
          parse_tab_stops (optarg);
          थांबा;

        स्थिती '0': स्थिती '1': स्थिती '2': स्थिती '3': स्थिती '4':
        स्थिती '5': स्थिती '6': स्थिती '7': स्थिती '8': स्थिती '9':
          जर (optarg)
            parse_tab_stops (optarg - 1);
          अन्यथा
            {
              अक्षर tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  finalize_tab_stops ();

  set_file_list (optind < argc ? &argv[optind] : रिक्त);

  expand ();

  cleanup_file_list_stdin ();

  परत exit_status;
}
