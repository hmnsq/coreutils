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
#परिभाषय क्रमादेशः_NAME "dirname"

#परिभाषय AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

स्थिर bool quote_output;

स्थिर संरचना option ध्रुव longopts[] =
{
  {"zero", no_तर्कः, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("\
Usage: %s [OPTION] NAME...\n\
"),
              क्रमादेशः_name);
      fputs (_("\
Output each NAME with its चरमः non-slash component and trailing slashes\n\
removed; यदि NAME अन्तर्भवति no /'s, output '.' (meaning the current directory).\n\
\n\
"), stdout);
      oputs (_("\
  -z, --zero\n\
         अन्तः each output पङ्क्तिः with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      प्रारूप_लिख (_("\
\n\
Examples:\n\
  %s /usr/bin/          -> \"/usr\"\n\
  %s dir1/str dir2/str  -> \"dir1\" followed by \"dir2\"\n\
  %s stdio.h            -> \".\"\n\
"),
              क्रमादेशः_name, क्रमादेशः_name, क्रमादेशः_name);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् (true)
    {
      पूर्णाङ्क c = getopt_long (argc, argv, "z", longopts, रिक्त);

      यदि (c == -1)
        भङ्ग;

      विकल्प (c)
        {
        पक्ष 'z':
          use_nuls = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }


  यदि (!use_nuls && isatty (STDOUT_FILENO))
    {
      पूर्णाङ्क qs = getenv_quoting_style ();
      यदि (qs < 0)
        qs = shell_escape_quoting_style;
      यदि (qs != literal_quoting_style)
        {
          समुच्चयः_quoting_style (रिक्त, qs);
          quote_output = true;
        }
    }

  प्रति (; optind < argc; optind++)
    {
      वर्ण ध्रुव *परिणाम = argv[optind];
      idx_t len = dir_len (परिणाम);

      यदि (! len)
        {
          स्थिर वर्ण ध्रुव dot = '.';
          परिणाम = &dot;
          len = 1;
        }
      यदि (quote_output)
        {
          परिणाम = quoteN_mem (परिणाम, len);
          len = वाक्यदैर्घ्यम् (परिणाम);
        }
      fwrite (परिणाम, 1, len, stdout);
      अक्षरं_लिख (use_nuls ? '\0' :'\n');
    }

  प्रत्यागम EXIT_SUCCESS;
}
