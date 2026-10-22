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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "basename"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

ਸਥਿਰ bool quote_output;

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"multiple", no_ਦਲੀਲ, ਨਿਰਾ, 'a'},
  {"suffix", required_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"zero", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s NAME [SUFFIX]\n\
  or:  %s OPTION... NAME...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Print NAME with any leading directory components removed.\n\
If specified, also remove a trailing SUFFIX.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --ਗੁਣਜ\n\
         support ਗੁਣਜ arguments and treat each as a NAME\n\
"));
      oputs (_("\
  -s, --suffix=SUFFIX\n\
         remove a trailing SUFFIX; ਸਿੱਟਾ_ਦਿੰਦਾ -a\n\
"));
      oputs (_("\
  -z, --zero\n\
         ਸਿਰਾ each output ਸਤਰ with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      ਲਿਖੋ (_("\
\n\
Examples:\n\
  %s /usr/bin/ਛਾਂਟੋ          -> \"ਛਾਂਟੋ\"\n\
  %s include/stdio.h .h     -> \"stdio\"\n\
  %s -s .h include/stdio.h  -> \"stdio\"\n\
  %s -a any/str1 any/str2   -> \"str1\" followed by \"str2\"\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Remove SUFFIX from the end of NAME if it is there, unless NAME
   consists entirely of SUFFIX.  */

ਸਥਿਰ ਖਾਲੀ
remove_suffix (ਅੱਖਰ *name, ਅੱਖਰ ਅਚਲ *suffix, idx_t suffix_len)
{
  ਅੱਖਰ *np = name + ਲੜੀ_ਲੰਬਾਈ (name);
  ਅੱਖਰ ਅਚਲ *sp = suffix + suffix_len;

  ਜਦੋਂ (np > name && sp > suffix)
    ਜੇ (*--np != *--sp)
      ਵਾਪਸ;
  ਜੇ (np > name)
    *np = '\0';
}

/* Perform the basename operation on STRING.  If SUFFIX has a positive
   length of SUFFIX_LEN bytes, remove the trailing SUFFIX.
   Finally, output the result string.  */

ਸਥਿਰ ਖਾਲੀ
perform_basename (ਅੱਖਰ ਅਚਲ *string, ਅੱਖਰ ਅਚਲ *suffix, idx_t suffix_len,
                  bool use_nuls)
{
  ਅੱਖਰ *name = ਅਧਾਰ_name (string);
  strip_trailing_slashes (name);

  /* Per POSIX, 'basename // /' must return '//' on platforms with
     distinct //.  On platforms with drive letters, this generalizes
     to making 'basename c: :' return 'c:'.  This rule is captured by
     skipping suffix stripping if base_name returned an absolute path
     or a drive letter (only possible if name is a file-system
     root).  */
  ਜੇ (0 < suffix_len && IS_RELATIVE_FILE_NAME (name)
      && ! FILE_SYSTEM_ਅਗੇਤਰ_LEN (name))
    remove_suffix (name, suffix, suffix_len);

  fputs (quote_output ? quoteN (name) : name, stdout);
  ਅੱਖਰ_ਲਿਖੋ (use_nuls ? '\0' : '\n');
  ਮੁਕਤ (name);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool ਗੁਣਜ_ਨਾਂ = false;
  bool use_nuls = false;
  ਅੱਖਰ ਅਚਲ *suffix = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ (true)
    {
      ਪੂਰਨ c = getopt_long (argc, argv, "+as:z", longopts, ਨਿਰਾ);

      ਜੇ (c == -1)
        ਰੋਕੋ;

      ਚੋਣ (c)
        {
        ਹਾਲਤ 's':
          suffix = optarg;
          /* -s implies -a, so...  */
          FALLTHROUGH;

        ਹਾਲਤ 'a':
          ਗੁਣਜ_ਨਾਂ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'z':
          use_nuls = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (! ਗੁਣਜ_ਨਾਂ)
    {
      ਜੇ (optind + 2 == argc)
        suffix = argv[optind + 1];
      ਨਹੀਂਤਾਂ ਜੇ (optind + 2 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (!use_nuls && isatty (STDOUT_FILENO))
    {
      ਪੂਰਨ qs = getenv_quoting_style ();
      ਜੇ (qs < 0)
        qs = shell_escape_quoting_style;
      ਜੇ (qs != literal_quoting_style)
        {
          ਸੈੱਟ_quoting_style (ਨਿਰਾ, qs);
          quote_output = true;
        }
    }

  idx_t suffix_len = suffix ? ਲੜੀ_ਲੰਬਾਈ (suffix) : 0;

  ਅੱਖਰ **file = argv + optind;
  ਪੂਰਨ n_files = ਗੁਣਜ_ਨਾਂ ? argc - optind : 1;

  ਲਈ (ਪੂਰਨ i = 0; i < n_files; ++i)
    perform_basename (file[i], suffix, suffix_len, use_nuls);

  ਵਾਪਸ EXIT_SUCCESS;
}
