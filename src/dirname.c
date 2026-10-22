/* dirname -- strip suffix from file name

   Copyright (C) 1990-2026 Free Software Foundation, Inc.

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

/* Written by David MacKenzie and Jim Meyering. */

#समावेश <config.h>
#समावेश <getopt.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>

#समावेश "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#समावेश "system.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "dirname"

#घोषणा AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

अटल bool quote_output;

अटल काष्ठा option स्थिर longopts[] =
{
  {"zero", no_argument, शून्य, 'z'},
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
Usage: %s [OPTION] NAME...\n\
"),
              program_name);
      ख_माला_दो (_("\
Output each NAME with its last non-slash component and trailing slashes\n\
removed; अगर NAME contains no /'s, output '.' (meaning the current directory).\n\
\n\
"), मानक_निकास);
      oputs (_("\
  -z, --zero\n\
         end each output पंक्ति with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      म_लिखो (_("\
\n\
Examples:\n\
  %s /usr/bin/          -> \"/usr\"\n\
  %s dir1/str dir2/str  -> \"dir1\" followed by \"dir2\"\n\
  %s stdio.h            -> \".\"\n\
"),
              program_name, program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक (true)
    {
      पूर्णांक c = getopt_long (argc, argv, "z", longopts, शून्य);

      अगर (c == -1)
        अवरोध;

      चयन (c)
        {
        हाल 'z':
          use_nuls = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }


  अगर (!use_nuls && isatty (STDOUT_FILENO))
    {
      पूर्णांक qs = getenv_quoting_style ();
      अगर (qs < 0)
        qs = shell_escape_quoting_style;
      अगर (qs != literal_quoting_style)
        {
          set_quoting_style (शून्य, qs);
          quote_output = true;
        }
    }

  क्रम (; optind < argc; optind++)
    {
      अक्षर स्थिर *परिणाम = argv[optind];
      idx_t len = dir_len (परिणाम);

      अगर (! len)
        {
          अटल अक्षर स्थिर dot = '.';
          परिणाम = &dot;
          len = 1;
        }
      अगर (quote_output)
        {
          परिणाम = quoteN_mem (परिणाम, len);
          len = म_माप (परिणाम);
        }
      ख_डालो (परिणाम, 1, len, मानक_निकास);
      अक्षर_दो (use_nuls ? '\0' :'\n');
    }

  वापस निकास_सफल;
}
