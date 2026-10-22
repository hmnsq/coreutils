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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "basename"

#define AUTHORS proper_name ("David MacKenzie")

ସ୍ଥିର bool quote_output;

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"multiple", no_argument, ଖାଲି, 'a'},
  {"suffix", required_argument, ଖାଲି, 's'},
  {"zero", no_argument, ଖାଲି, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
         end each output ଧାଡ଼ି with NUL, not newline\n\
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

ସ୍ଥିର ଶୂନ୍ୟ
remove_suffix (ଅକ୍ଷର *name, ଅକ୍ଷର ଅଚଳ *suffix, idx_t suffix_len)
{
  ଅକ୍ଷର *np = name + strlen (name);
  ଅକ୍ଷର ଅଚଳ *sp = suffix + suffix_len;

  ଯେତେବେଳେ (np > name && sp > suffix)
    ଯଦି (*--np != *--sp)
      ଫେରନ୍ତୁ;
  ଯଦି (np > name)
    *np = '\0';
}

/* Perform the basename operation on STRING.  If SUFFIX has a positive
   length of SUFFIX_LEN bytes, remove the trailing SUFFIX.
   Finally, output the result string.  */

ସ୍ଥିର ଶୂନ୍ୟ
perform_basename (ଅକ୍ଷର ଅଚଳ *string, ଅକ୍ଷର ଅଚଳ *suffix, idx_t suffix_len,
                  bool use_nuls)
{
  ଅକ୍ଷର *name = base_name (string);
  strip_trailing_slashes (name);

  /* Per POSIX, 'basename // /' must return '//' on platforms with
     distinct //.  On platforms with drive letters, this generalizes
     to making 'basename c: :' return 'c:'.  This rule is captured by
     skipping suffix stripping if base_name returned an absolute path
     or a drive letter (only possible if name is a file-system
     root).  */
  ଯଦି (0 < suffix_len && IS_RELATIVE_FILE_NAME (name)
      && ! FILE_SYSTEM_PREFIX_LEN (name))
    remove_suffix (name, suffix, suffix_len);

  fputs (quote_output ? quoteN (name) : name, stdout);
  putchar (use_nuls ? '\0' : '\n');
  ମୁକ୍ତ (name);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  bool multiple_names = false;
  bool use_nuls = false;
  ଅକ୍ଷର ଅଚଳ *suffix = ଖାଲି;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ (true)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getopt_long (argc, argv, "+as:z", longopts, ଖାଲି);

      ଯଦି (c == -1)
        ଭାଙ୍ଗ;

      ବାଛ (c)
        {
        ସ୍ଥିତି 's':
          suffix = optarg;
          /* -s implies -a, so...  */
          FALLTHROUGH;

        ସ୍ଥିତି 'a':
          multiple_names = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'z':
          use_nuls = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (! multiple_names)
    {
      ଯଦି (optind + 2 == argc)
        suffix = argv[optind + 1];
      ଅନ୍ୟଥା ଯଦି (optind + 2 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (!use_nuls && isatty (STDOUT_FILENO))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା qs = getenv_quoting_style ();
      ଯଦି (qs < 0)
        qs = shell_escape_quoting_style;
      ଯଦି (qs != literal_quoting_style)
        {
          set_quoting_style (ଖାଲି, qs);
          quote_output = true;
        }
    }

  idx_t suffix_len = suffix ? strlen (suffix) : 0;

  ଅକ୍ଷର **file = argv + optind;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_files = multiple_names ? argc - optind : 1;

  ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_files; ++i)
    perform_basename (file[i], suffix, suffix_len, use_nuls);

  ଫେରନ୍ତୁ EXIT_SUCCESS;
}
