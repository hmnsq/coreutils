/* basename -- strip directory and suffix from file names
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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "basename"

#define AUTHORS proper_name ("David MacKenzie")

நிலையான bool quote_output;

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"multiple", no_argument, வெற்று, 'a'},
  {"suffix", required_argument, வெற்று, 's'},
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
Usage: %s NAME [SUFFIX]\n\
  or:  %s OPTION... NAME...\n\
"),
              program_name, program_name);
      fputs (_("\
Print NAME with any leading directory components removed.\n\
If specified, also remove a trailing SUFFIX.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --multiple\n\
         support multiple arguments and treat each as a NAME\n\
"));
      oputs (_("\
  -s, --suffix=SUFFIX\n\
         remove a trailing SUFFIX; implies -a\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output வரி with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      printf (_("\
\n\
Examples:\n\
  %s /usr/bin/sort          -> \"sort\"\n\
  %s include/stdio.h .h     -> \"stdio\"\n\
  %s -s .h include/stdio.h  -> \"stdio\"\n\
  %s -a any/str1 any/str2   -> \"str1\" followed by \"str2\"\n\
"),
              program_name, program_name, program_name, program_name);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Remove SUFFIX from the end of NAME if it is there, unless NAME
   consists entirely of SUFFIX.  */

நிலையான வெறுமை
remove_suffix (எழுத்து *name, எழுத்து மாறிலி *suffix, idx_t suffix_len)
{
  எழுத்து *np = name + strlen (name);
  எழுத்து மாறிலி *sp = suffix + suffix_len;

  போது (np > name && sp > suffix)
    எனில் (*--np != *--sp)
      திரும்பு;
  எனில் (np > name)
    *np = '\0';
}

/* Perform the basename operation on STRING.  If SUFFIX has a positive
   length of SUFFIX_LEN bytes, remove the trailing SUFFIX.
   Finally, output the result string.  */

நிலையான வெறுமை
perform_basename (எழுத்து மாறிலி *string, எழுத்து மாறிலி *suffix, idx_t suffix_len,
                  bool use_nuls)
{
  எழுத்து *name = base_name (string);
  strip_trailing_slashes (name);

  /* Per POSIX, 'basename // /' must return '//' on platforms with
     distinct //.  On platforms with drive letters, this generalizes
     to making 'basename c: :' return 'c:'.  This rule is captured by
     skipping suffix stripping if base_name returned an absolute path
     or a drive letter (only possible if name is a file-system
     root).  */
  எனில் (0 < suffix_len && IS_RELATIVE_FILE_NAME (name)
      && ! FILE_SYSTEM_PREFIX_LEN (name))
    remove_suffix (name, suffix, suffix_len);

  fputs (quote_output ? quoteN (name) : name, stdout);
  putchar (use_nuls ? '\0' : '\n');
  விடுவி (name);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool multiple_names = false;
  bool use_nuls = false;
  எழுத்து மாறிலி *suffix = வெற்று;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது (true)
    {
      முழு c = getopt_long (argc, argv, "+as:z", longopts, வெற்று);

      எனில் (c == -1)
        நிறுத்து;

      தேர்வு (c)
        {
        நிலை 's':
          suffix = optarg;
          /* -s implies -a, so...  */
          FALLTHROUGH;

        நிலை 'a':
          multiple_names = true;
          நிறுத்து;

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

  எனில் (! multiple_names)
    {
      எனில் (optind + 2 == argc)
        suffix = argv[optind + 1];
      இல்லையெனில் எனில் (optind + 2 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
          usage (EXIT_FAILURE);
        }
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

  idx_t suffix_len = suffix ? strlen (suffix) : 0;

  எழுத்து **file = argv + optind;
  முழு n_files = multiple_names ? argc - optind : 1;

  க்கு (முழு i = 0; i < n_files; ++i)
    perform_basename (file[i], suffix, suffix_len, use_nuls);

  திரும்பு EXIT_SUCCESS;
}
