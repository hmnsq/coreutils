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

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#अन्तर्भाव "system.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dirname"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

स्थिर bool quote_output;

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"zero", no_argument, रिक्त, 'z'},
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
Usage: %s [OPTION] NAME...\n\
"),
              program_name);
      ख_माला_दो (_("\
Output each NAME with its last non-slash component and trailing slashes\n\
removed; जर NAME contains no /'s, output '.' (meaning the current directory).\n\
\n\
"), मानक_निकास);
      oputs (_("\
  -z, --zero\n\
         end each output ओळ with NUL, not newline\n\
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
main (पूर्णांक argc, अक्षर **argv)
{
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत (true)
    {
      पूर्णांक c = getopt_long (argc, argv, "z", longopts, रिक्त);

      जर (c == -1)
        थांबा;

      निवड (c)
        {
        स्थिती 'z':
          use_nuls = true;
          थांबा;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }


  जर (!use_nuls && isatty (STDOUT_FILENO))
    {
      पूर्णांक qs = getenv_quoting_style ();
      जर (qs < 0)
        qs = shell_escape_quoting_style;
      जर (qs != literal_quoting_style)
        {
          set_quoting_style (रिक्त, qs);
          quote_output = true;
        }
    }

  साठी (; optind < argc; optind++)
    {
      अक्षर स्थिरांक *result = argv[optind];
      idx_t len = dir_len (result);

      जर (! len)
        {
          स्थिर अक्षर स्थिरांक dot = '.';
          result = &dot;
          len = 1;
        }
      जर (quote_output)
        {
          result = quoteN_mem (result, len);
          len = म_माप (result);
        }
      ख_डालो (result, 1, len, मानक_निकास);
      अक्षर_दो (use_nuls ? '\0' :'\n');
    }

  परत निकास_सफल;
}
