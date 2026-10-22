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

#ضمن <config.h>
#ضمن <getopt.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ضمن "system.h"
#ضمن "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "basename"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

ساكن bool quote_output;

ساكن بنية option ثابت longopts[] =
{
  {"multiple", no_دلیل, عدم, 'a'},
  {"suffix", required_دلیل, عدم, 's'},
  {"zero", no_دلیل, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("\
Usage: %s NAME [SUFFIX]\n\
  or:  %s OPTION... NAME...\n\
"),
              پروگرام_name, پروگرام_name);
      fputs (_("\
Print NAME with any leading directory components removed.\n\
If specified, also remove a trailing SUFFIX.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --مضرب\n\
         support مضرب arguments and treat each as a NAME\n\
"));
      oputs (_("\
  -s, --suffix=SUFFIX\n\
         remove a trailing SUFFIX; سٹا_دیندا -a\n\
"));
      oputs (_("\
  -z, --zero\n\
         سرا each output سطر with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      لکھو (_("\
\n\
Examples:\n\
  %s /usr/bin/ترتیب_دیو          -> \"ترتیب_دیو\"\n\
  %s include/stdio.h .h     -> \"stdio\"\n\
  %s -s .h include/stdio.h  -> \"stdio\"\n\
  %s -a any/str1 any/str2   -> \"str1\" followed by \"str2\"\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name, پروگرام_name);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* Remove SUFFIX from the end of NAME if it is there, unless NAME
   consists entirely of SUFFIX.  */

ساكن فراغ
remove_suffix (حرف *name, حرف ثابت *suffix, idx_t suffix_len)
{
  حرف *np = name + لڑی_لمبائی (name);
  حرف ثابت *sp = suffix + suffix_len;

  طالما (np > name && sp > suffix)
    إذا (*--np != *--sp)
      أرجع;
  إذا (np > name)
    *np = '\0';
}

/* Perform the basename operation on STRING.  If SUFFIX has a positive
   length of SUFFIX_LEN bytes, remove the trailing SUFFIX.
   Finally, output the result string.  */

ساكن فراغ
perform_basename (حرف ثابت *string, حرف ثابت *suffix, idx_t suffix_len,
                  bool use_nuls)
{
  حرف *name = ادھار_name (string);
  strip_trailing_slashes (name);

  /* Per POSIX, 'basename // /' must return '//' on platforms with
     distinct //.  On platforms with drive letters, this generalizes
     to making 'basename c: :' return 'c:'.  This rule is captured by
     skipping suffix stripping if base_name returned an absolute path
     or a drive letter (only possible if name is a file-system
     root).  */
  إذا (0 < suffix_len && IS_RELATIVE_FILE_NAME (name)
      && ! FILE_SYSTEM_اگیتر_LEN (name))
    remove_suffix (name, suffix, suffix_len);

  fputs (quote_output ? quoteN (name) : name, stdout);
  حرف_لکھو (use_nuls ? '\0' : '\n');
  حرر (name);
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  bool مضرب_ناں = false;
  bool use_nuls = false;
  حرف ثابت *suffix = عدم;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما (true)
    {
      صحيح c = getopt_long (argc, argv, "+as:z", longopts, عدم);

      إذا (c == -1)
        اكسر;

      بدل (c)
        {
        حالة 's':
          suffix = optarg;
          /* -s implies -a, so...  */
          FALLTHROUGH;

        حالة 'a':
          مضرب_ناں = true;
          اكسر;

        حالة 'z':
          use_nuls = true;
          اكسر;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  إذا (! مضرب_ناں)
    {
      إذا (optind + 2 == argc)
        suffix = argv[optind + 1];
      وإلا إذا (optind + 2 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
          usage (EXIT_FAILURE);
        }
    }

  إذا (!use_nuls && isatty (STDOUT_FILENO))
    {
      صحيح qs = getenv_quoting_style ();
      إذا (qs < 0)
        qs = shell_escape_quoting_style;
      إذا (qs != literal_quoting_style)
        {
          سیٹ_quoting_style (عدم, qs);
          quote_output = true;
        }
    }

  idx_t suffix_len = suffix ? لڑی_لمبائی (suffix) : 0;

  حرف **file = argv + optind;
  صحيح n_files = مضرب_ناں ? argc - optind : 1;

  لكل (صحيح i = 0; i < n_files; ++i)
    perform_basename (file[i], suffix, suffix_len, use_nuls);

  أرجع EXIT_SUCCESS;
}
