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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#அந்தர்பாவ "system.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "dirname"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

நிலையான bool quote_output;

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"zero", no_argument, வெற்று, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION] NAME...\n\
"),
              program_name);
      fputs (_("\
Output each NAME with its last non-slash component and trailing slashes\n\
removed; எனில் NAME contains no /'s, output '.' (meaning the current directory).\n\
\n\
"), stdout);
      oputs (_("\
  -z, --zero\n\
         end each output வரி with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
Examples:\n\
  %s /usr/bin/          -> \"/usr\"\n\
  %s dir1/str dir2/str  -> \"dir1\" followed by \"dir2\"\n\
  %s stdio.h            -> \".\"\n\
"),
              program_name, program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது (true)
    {
      முழு c = getopt_long (argc, argv, "z", longopts, வெற்று);

      எனில் (c == -1)
        நிறுத்து;

      தேர்வு (c)
        {
        நிலை 'z':
          use_nuls = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }


  எனில் (!use_nuls && isatty (STDOUT_FILENO))
    {
      முழு qs = getenv_quoting_style ();
      எனில் (qs < 0)
        qs = shell_escape_quoting_style;
      எனில் (qs != literal_quoting_style)
        {
          set_quoting_style (வெற்று, qs);
          quote_output = true;
        }
    }

  க்கு (; optind < argc; optind++)
    {
      எழுத்து மாறிலி *result = argv[optind];
      idx_t len = dir_len (result);

      எனில் (! len)
        {
          நிலையான எழுத்து மாறிலி dot = '.';
          result = &dot;
          len = 1;
        }
      எனில் (quote_output)
        {
          result = quoteN_mem (result, len);
          len = strlen (result);
        }
      fwrite (result, 1, len, stdout);
      putchar (use_nuls ? '\0' :'\n');
    }

  திரும்பு EXIT_SUCCESS;
}
