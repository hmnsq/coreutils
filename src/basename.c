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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "basename"

#define AUTHORS proper_name ("David MacKenzie")

স্থির bool quote_output;

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"multiple", no_argument, ফাঁকা, 'a'},
  {"suffix", required_argument, ফাঁকা, 's'},
  {"zero", no_argument, ফাঁকা, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
         end each output সারি with NUL, not newline\n\
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

স্থির শূন্য
remove_suffix (অক্ষর *name, অক্ষর ধ্রুবক *suffix, idx_t suffix_len)
{
  অক্ষর *np = name + strlen (name);
  অক্ষর ধ্রুবক *sp = suffix + suffix_len;

  যখন (np > name && sp > suffix)
    যদি (*--np != *--sp)
      ফেরত;
  যদি (np > name)
    *np = '\0';
}

/* Perform the basename operation on STRING.  If SUFFIX has a positive
   length of SUFFIX_LEN bytes, remove the trailing SUFFIX.
   Finally, output the result string.  */

স্থির শূন্য
perform_basename (অক্ষর ধ্রুবক *string, অক্ষর ধ্রুবক *suffix, idx_t suffix_len,
                  bool use_nuls)
{
  অক্ষর *name = base_name (string);
  strip_trailing_slashes (name);

  /* Per POSIX, 'basename // /' must return '//' on platforms with
     distinct //.  On platforms with drive letters, this generalizes
     to making 'basename c: :' return 'c:'.  This rule is captured by
     skipping suffix stripping if base_name returned an absolute path
     or a drive letter (only possible if name is a file-system
     root).  */
  যদি (0 < suffix_len && IS_RELATIVE_FILE_NAME (name)
      && ! FILE_SYSTEM_PREFIX_LEN (name))
    remove_suffix (name, suffix, suffix_len);

  fputs (quote_output ? quoteN (name) : name, stdout);
  putchar (use_nuls ? '\0' : '\n');
  মুক্ত (name);
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool multiple_names = false;
  bool use_nuls = false;
  অক্ষর ধ্রুবক *suffix = ফাঁকা;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন (true)
    {
      পূর্ণসংখ্যা c = getopt_long (argc, argv, "+as:z", longopts, ফাঁকা);

      যদি (c == -1)
        বিরতি;

      নির্বাচন (c)
        {
        ক্ষেত্র 's':
          suffix = optarg;
          /* -s implies -a, so...  */
          FALLTHROUGH;

        ক্ষেত্র 'a':
          multiple_names = true;
          বিরতি;

        ক্ষেত্র 'z':
          use_nuls = true;
          বিরতি;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  যদি (! multiple_names)
    {
      যদি (optind + 2 == argc)
        suffix = argv[optind + 1];
      অন্যথা যদি (optind + 2 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
          usage (EXIT_FAILURE);
        }
    }

  যদি (!use_nuls && isatty (STDOUT_FILENO))
    {
      পূর্ণসংখ্যা qs = getenv_quoting_style ();
      যদি (qs < 0)
        qs = shell_escape_quoting_style;
      যদি (qs != literal_quoting_style)
        {
          set_quoting_style (ফাঁকা, qs);
          quote_output = true;
        }
    }

  idx_t suffix_len = suffix ? strlen (suffix) : 0;

  অক্ষর **file = argv + optind;
  পূর্ণসংখ্যা n_files = multiple_names ? argc - optind : 1;

  জন্য (পূর্ণসংখ্যা i = 0; i < n_files; ++i)
    perform_basename (file[i], suffix, suffix_len, use_nuls);

  ফেরত EXIT_SUCCESS;
}
