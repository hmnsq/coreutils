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

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#अन्तर्भाव "system.h"
#अन्तर्भाव "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "basename"

#define AUTHORS proper_name ("David MacKenzie")

स्थिर bool quote_output;

स्थिर संरचना option ध्रुव longopts[] =
{
  {"multiple", no_argument, रिक्त, 'a'},
  {"suffix", required_argument, रिक्त, 's'},
  {"zero", no_argument, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  जइ (status != EXIT_SUCCESS)
    emit_try_help ();
  अण्णहा
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
         end each output पङ्क्तिः with NUL, not newline\n\
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

स्थिर शून्य
remove_suffix (वर्ण *name, वर्ण ध्रुव *suffix, idx_t suffix_len)
{
  वर्ण *np = name + strlen (name);
  वर्ण ध्रुव *sp = suffix + suffix_len;

  जाव (np > name && sp > suffix)
    जइ (*--np != *--sp)
      प्रत्यागम;
  जइ (np > name)
    *np = '\0';
}

/* Perform the basename operation on STRING.  If SUFFIX has a positive
   length of SUFFIX_LEN bytes, remove the trailing SUFFIX.
   Finally, output the result string.  */

स्थिर शून्य
perform_basename (वर्ण ध्रुव *string, वर्ण ध्रुव *suffix, idx_t suffix_len,
                  bool use_nuls)
{
  वर्ण *name = base_name (string);
  strip_trailing_slashes (name);

  /* Per POSIX, 'basename // /' must return '//' on platforms with
     distinct //.  On platforms with drive letters, this generalizes
     to making 'basename c: :' return 'c:'.  This rule is captured by
     skipping suffix stripping if base_name returned an absolute path
     or a drive letter (only possible if name is a file-system
     root).  */
  जइ (0 < suffix_len && IS_RELATIVE_FILE_NAME (name)
      && ! FILE_SYSTEM_PREFIX_LEN (name))
    remove_suffix (name, suffix, suffix_len);

  fputs (quote_output ? quoteN (name) : name, stdout);
  putchar (use_nuls ? '\0' : '\n');
  मुक्त (name);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  bool multiple_names = false;
  bool use_nuls = false;
  वर्ण ध्रुव *suffix = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  जाव (true)
    {
      पूर्णाङ्क c = getopt_long (argc, argv, "+as:z", longopts, रिक्त);

      जइ (c == -1)
        भङ्ग;

      विकल्प (c)
        {
        पक्ष 's':
          suffix = optarg;
          /* -s implies -a, so...  */
          FALLTHROUGH;

        पक्ष 'a':
          multiple_names = true;
          भङ्ग;

        पक्ष 'z':
          use_nuls = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  जइ (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  जइ (! multiple_names)
    {
      जइ (optind + 2 == argc)
        suffix = argv[optind + 1];
      अण्णहा जइ (optind + 2 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
          usage (EXIT_FAILURE);
        }
    }

  जइ (!use_nuls && isatty (STDOUT_FILENO))
    {
      पूर्णाङ्क qs = getenv_quoting_style ();
      जइ (qs < 0)
        qs = shell_escape_quoting_style;
      जइ (qs != literal_quoting_style)
        {
          set_quoting_style (रिक्त, qs);
          quote_output = true;
        }
    }

  idx_t suffix_len = suffix ? strlen (suffix) : 0;

  वर्ण **file = argv + optind;
  पूर्णाङ्क n_files = multiple_names ? argc - optind : 1;

  प्रति (पूर्णाङ्क i = 0; i < n_files; ++i)
    perform_basename (file[i], suffix, suffix_len, use_nuls);

  प्रत्यागम EXIT_SUCCESS;
}
