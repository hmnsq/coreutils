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

#समावेश <config.h>
#समावेश <getopt.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>

#समावेश "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#समावेश "system.h"
#समावेश "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "basename"

#घोषणा AUTHORS proper_name ("David MacKenzie")

अटल bool quote_output;

अटल काष्ठा option स्थिर longopts[] =
{
  {"multiple", no_argument, शून्य, 'a'},
  {"suffix", required_argument, शून्य, 's'},
  {"zero", no_argument, शून्य, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s NAME [SUFFIX]\n\
  or:  %s OPTION... NAME...\n\
"),
              program_name, program_name);
      ख_माला_दो (_("\
Print NAME with any leading directory components removed.\n\
If specified, also हटाओ a trailing SUFFIX.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --multiple\n\
         support multiple arguments and treat each as a NAME\n\
"));
      oputs (_("\
  -s, --suffix=SUFFIX\n\
         हटाओ a trailing SUFFIX; implies -a\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output पंक्ति with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      म_लिखो (_("\
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
  निकास (status);
}

/* Remove SUFFIX from the end of NAME if it is there, unless NAME
   consists entirely of SUFFIX.  */

अटल व्योम
remove_suffix (अक्षर *name, अक्षर स्थिर *suffix, idx_t suffix_len)
{
  अक्षर *np = name + म_माप (name);
  अक्षर स्थिर *sp = suffix + suffix_len;

  जबतक (np > name && sp > suffix)
    अगर (*--np != *--sp)
      वापस;
  अगर (np > name)
    *np = '\0';
}

/* Perform the basename operation on STRING.  If SUFFIX has a positive
   length of SUFFIX_LEN bytes, remove the trailing SUFFIX.
   Finally, output the result string.  */

अटल व्योम
perform_basename (अक्षर स्थिर *string, अक्षर स्थिर *suffix, idx_t suffix_len,
                  bool use_nuls)
{
  अक्षर *name = base_name (string);
  strip_trailing_slashes (name);

  /* Per POSIX, 'basename // /' must return '//' on platforms with
     distinct //.  On platforms with drive letters, this generalizes
     to making 'basename c: :' return 'c:'.  This rule is captured by
     skipping suffix stripping if base_name returned an absolute path
     or a drive letter (only possible if name is a file-system
     root).  */
  अगर (0 < suffix_len && IS_RELATIVE_FILE_NAME (name)
      && ! FILE_SYSTEM_PREFIX_LEN (name))
    remove_suffix (name, suffix, suffix_len);

  ख_माला_दो (quote_output ? quoteN (name) : name, मानक_निकास);
  अक्षर_दो (use_nuls ? '\0' : '\n');
  मुक्त (name);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  bool multiple_names = false;
  bool use_nuls = false;
  अक्षर स्थिर *suffix = शून्य;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक (true)
    {
      पूर्णांक c = getopt_long (argc, argv, "+as:z", longopts, शून्य);

      अगर (c == -1)
        अवरोध;

      चयन (c)
        {
        हाल 's':
          suffix = optarg;
          /* -s implies -a, so...  */
          FALLTHROUGH;

        हाल 'a':
          multiple_names = true;
          अवरोध;

        हाल 'z':
          use_nuls = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  अगर (! multiple_names)
    {
      अगर (optind + 2 == argc)
        suffix = argv[optind + 1];
      अन्यथा अगर (optind + 2 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
          usage (निकास_त्रुटि);
        }
    }

  अगर (!use_nuls && isatty (STDOUT_FILENO))
    {
      पूर्णांक qs = getenv_quoting_style ();
      अगर (qs < 0)
        qs = shell_escape_quoting_style;
      अगर (qs != literal_quoting_style)
        {
          set_quoting_style (शून्य, qs);
          quote_output = true;
        }
    }

  idx_t suffix_len = suffix ? म_माप (suffix) : 0;

  अक्षर **file = argv + optind;
  पूर्णांक n_files = multiple_names ? argc - optind : 1;

  क्रम (पूर्णांक i = 0; i < n_files; ++i)
    perform_basename (file[i], suffix, suffix_len, use_nuls);

  वापस निकास_सफल;
}
