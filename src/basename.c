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

#شامل <config.h>
#شامل <getopt.h>
#شامل <stdio.h>
#شامل <sys/types.h>

#شامل "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#شامل "system.h"
#شامل "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "basename"

#define AUTHORS proper_name ("David MacKenzie")

ایستا bool quote_output;

ایستا ساختار option ثابت longopts[] =
{
  {"multiple", no_argument, پوچ, 'a'},
  {"suffix", required_argument, پوچ, 's'},
  {"zero", no_argument, پوچ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
         end each output سطر with NUL, not newline\n\
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

ایستا تهی
remove_suffix (نویسه *name, نویسه ثابت *suffix, idx_t suffix_len)
{
  نویسه *np = name + strlen (name);
  نویسه ثابت *sp = suffix + suffix_len;

  وقتی (np > name && sp > suffix)
    اگر (*--np != *--sp)
      بازگشت;
  اگر (np > name)
    *np = '\0';
}

/* Perform the basename operation on STRING.  If SUFFIX has a positive
   length of SUFFIX_LEN bytes, remove the trailing SUFFIX.
   Finally, output the result string.  */

ایستا تهی
perform_basename (نویسه ثابت *string, نویسه ثابت *suffix, idx_t suffix_len,
                  bool use_nuls)
{
  نویسه *name = base_name (string);
  strip_trailing_slashes (name);

  /* Per POSIX, 'basename // /' must return '//' on platforms with
     distinct //.  On platforms with drive letters, this generalizes
     to making 'basename c: :' return 'c:'.  This rule is captured by
     skipping suffix stripping if base_name returned an absolute path
     or a drive letter (only possible if name is a file-system
     root).  */
  اگر (0 < suffix_len && IS_RELATIVE_FILE_NAME (name)
      && ! FILE_SYSTEM_PREFIX_LEN (name))
    remove_suffix (name, suffix, suffix_len);

  fputs (quote_output ? quoteN (name) : name, stdout);
  putchar (use_nuls ? '\0' : '\n');
  آزاد (name);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  bool multiple_names = false;
  bool use_nuls = false;
  نویسه ثابت *suffix = پوچ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی (true)
    {
      صحیح c = getopt_long (argc, argv, "+as:z", longopts, پوچ);

      اگر (c == -1)
        بشکن;

      انتخاب (c)
        {
        حالت 's':
          suffix = optarg;
          /* -s implies -a, so...  */
          FALLTHROUGH;

        حالت 'a':
          multiple_names = true;
          بشکن;

        حالت 'z':
          use_nuls = true;
          بشکن;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  اگر (! multiple_names)
    {
      اگر (optind + 2 == argc)
        suffix = argv[optind + 1];
      وگرنه اگر (optind + 2 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
          usage (EXIT_FAILURE);
        }
    }

  اگر (!use_nuls && isatty (STDOUT_FILENO))
    {
      صحیح qs = getenv_quoting_style ();
      اگر (qs < 0)
        qs = shell_escape_quoting_style;
      اگر (qs != literal_quoting_style)
        {
          set_quoting_style (پوچ, qs);
          quote_output = true;
        }
    }

  idx_t suffix_len = suffix ? strlen (suffix) : 0;

  نویسه **file = argv + optind;
  صحیح n_files = multiple_names ? argc - optind : 1;

  برای (صحیح i = 0; i < n_files; ++i)
    perform_basename (file[i], suffix, suffix_len, use_nuls);

  بازگشت EXIT_SUCCESS;
}
