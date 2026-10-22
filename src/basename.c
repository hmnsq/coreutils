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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "basename"

#define AUTHORS proper_name ("David MacKenzie")

સ્થિર bool quote_output;

સ્થિર સંરચના option અચલ longopts[] =
{
  {"multiple", no_argument, ખાલી, 'a'},
  {"suffix", required_argument, ખાલી, 's'},
  {"zero", no_argument, ખાલી, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
         end each output લીટી with NUL, not newline\n\
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

સ્થિર શૂન્ય
remove_suffix (અક્ષર *name, અક્ષર અચલ *suffix, idx_t suffix_len)
{
  અક્ષર *np = name + strlen (name);
  અક્ષર અચલ *sp = suffix + suffix_len;

  જ્યારે (np > name && sp > suffix)
    જો (*--np != *--sp)
      પાછા;
  જો (np > name)
    *np = '\0';
}

/* Perform the basename operation on STRING.  If SUFFIX has a positive
   length of SUFFIX_LEN bytes, remove the trailing SUFFIX.
   Finally, output the result string.  */

સ્થિર શૂન્ય
perform_basename (અક્ષર અચલ *string, અક્ષર અચલ *suffix, idx_t suffix_len,
                  bool use_nuls)
{
  અક્ષર *name = base_name (string);
  strip_trailing_slashes (name);

  /* Per POSIX, 'basename // /' must return '//' on platforms with
     distinct //.  On platforms with drive letters, this generalizes
     to making 'basename c: :' return 'c:'.  This rule is captured by
     skipping suffix stripping if base_name returned an absolute path
     or a drive letter (only possible if name is a file-system
     root).  */
  જો (0 < suffix_len && IS_RELATIVE_FILE_NAME (name)
      && ! FILE_SYSTEM_PREFIX_LEN (name))
    remove_suffix (name, suffix, suffix_len);

  fputs (quote_output ? quoteN (name) : name, stdout);
  putchar (use_nuls ? '\0' : '\n');
  મુક્ત (name);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  bool multiple_names = false;
  bool use_nuls = false;
  અક્ષર અચલ *suffix = ખાલી;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે (true)
    {
      પૂર્ણાંક c = getopt_long (argc, argv, "+as:z", longopts, ખાલી);

      જો (c == -1)
        તોડો;

      પસંદગી (c)
        {
        સ્થિતિ 's':
          suffix = optarg;
          /* -s implies -a, so...  */
          FALLTHROUGH;

        સ્થિતિ 'a':
          multiple_names = true;
          તોડો;

        સ્થિતિ 'z':
          use_nuls = true;
          તોડો;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  જો (! multiple_names)
    {
      જો (optind + 2 == argc)
        suffix = argv[optind + 1];
      નહીંતર જો (optind + 2 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
          usage (EXIT_FAILURE);
        }
    }

  જો (!use_nuls && isatty (STDOUT_FILENO))
    {
      પૂર્ણાંક qs = getenv_quoting_style ();
      જો (qs < 0)
        qs = shell_escape_quoting_style;
      જો (qs != literal_quoting_style)
        {
          set_quoting_style (ખાલી, qs);
          quote_output = true;
        }
    }

  idx_t suffix_len = suffix ? strlen (suffix) : 0;

  અક્ષર **file = argv + optind;
  પૂર્ણાંક n_files = multiple_names ? argc - optind : 1;

  માટે (પૂર્ણાંક i = 0; i < n_files; ++i)
    perform_basename (file[i], suffix, suffix_len, use_nuls);

  પાછા EXIT_SUCCESS;
}
