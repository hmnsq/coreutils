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

#समावेश <config.h>

#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "ioblksize.h"
#समावेश "mcel.h"
#समावेश "mbbuf.h"
#समावेश "expand-common.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "expand"

#घोषणा AUTHORS proper_name ("David MacKenzie")

अटल अक्षर स्थिर shortopts[] = "it:0::1::2::3::4::5::6::7::8::9::";

अटल काष्ठा option स्थिर longopts[] =
{
  {"tabs", required_argument, शून्य, 't'},
  {"initial", no_argument, शून्य, 'i'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
         करो not convert tabs after non blanks\n\
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

अटल व्योम
expand (व्योम)
{
  /* Input stream.  */
  खाता *fp = अग्र_file (शून्य);

  अगर (!fp)
    वापस;

  अटल अक्षर पंक्ति_in[IO_BUFSIZE];
  mbbuf_t mbbuf;
  mbbuf_init (&mbbuf, पंक्ति_in, माप पंक्ति_in, fp);

  जबतक (true)
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
      idx_t tab_सूचकांक = 0;


      /* Convert a line of text.  */

      करो
        {
          जबतक ((g = mbbuf_get_char (&mbbuf)).ch == MBBUF_EOF
                 && (fp = अग्र_file (fp)))
            mbbuf_init (&mbbuf, पंक्ति_in, माप पंक्ति_in, fp);

          अगर (convert)
            {
              convert &= convert_entire_पंक्ति || c32issep (g.ch);

              अगर (g.ch == '\t')
                {
                  /* Column the next input tab stop is on.  */
                  bool last_tab;
                  colno अग्र_tab_column
                    = get_अग्र_tab_column (column, &tab_सूचकांक, &last_tab);

                  जबतक (++column < अग्र_tab_column)
                    अगर (अक्षर_दो (' ') < 0)
                      write_error ();

                  अगर (अक्षर_दो (' ') < 0)
                    write_error ();

                  जारी;
                }
              अन्यथा अगर (g.ch == '\b')
                {
                  /* Go back one column, and force recalculation of the
                     next tab stop.  */
                  column -= !!column;
                  tab_सूचकांक -= !!tab_सूचकांक;
                }
              अन्यथा
                {
                  पूर्णांक width = c32width (g.ch);
                  अगर (ckd_add (&column, column, width < 0 ? 1 : width))
                    error (निकास_त्रुटि, 0, _("input line is too long"));
                }

            }

          अगर (g.ch == MBBUF_EOF)
            वापस;

          ख_डालो (mbbuf_char_offset (&mbbuf, g), माप (अक्षर), g.len, मानक_निकास);
          अगर (ख_त्रुटि (मानक_निकास))
            write_error ();
        }
      जबतक (g.ch != '\n');
    }
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक c;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);
  convert_entire_पंक्ति = true;

  जबतक ((c = getopt_long (argc, argv, shortopts, longopts, शून्य)) != -1)
    {
      चयन (c)
        {
        हाल 'i':
          convert_entire_पंक्ति = false;
          अवरोध;

        हाल 't':
          parse_tab_stops (optarg);
          अवरोध;

        हाल '0': हाल '1': हाल '2': हाल '3': हाल '4':
        हाल '5': हाल '6': हाल '7': हाल '8': हाल '9':
          अगर (optarg)
            parse_tab_stops (optarg - 1);
          अन्यथा
            {
              अक्षर tab_stop[2];
              tab_stop[0] = c;
              tab_stop[1] = '\0';
              parse_tab_stops (tab_stop);
            }
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  finalize_tab_stops ();

  set_file_list (optind < argc ? &argv[optind] : शून्य);

  expand ();

  cleanup_file_list_stdin ();

  वापस exit_status;
}
