/* readlink -- display value of a symbolic link.
   Copyright (C) 2002-2026 Free Software Foundation, Inc.

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

/* Written by Dmitry V. Levin */

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>

#समावेश "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#समावेश "system.h"
#समावेश "canonicalize.h"
#समावेश "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "readlink"

#घोषणा AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
अटल bool no_newline;

/* If true, report error messages.  */
अटल bool verbose;

/* If true, quote output according to the selected quoting style.  */
अटल bool quote_output;

अटल काष्ठा option स्थिर longopts[] =
{
  {"canonicalize", no_argument, शून्य, 'f'},
  {"canonicalize-existing", no_argument, शून्य, 'e'},
  {"canonicalize-missing", no_argument, शून्य, 'm'},
  {"no-newline", no_argument, शून्य, 'n'},
  {"quiet", no_argument, शून्य, 'q'},
  {"silent", no_argument, शून्य, 's'},
  {"verbose", no_argument, शून्य, 'v'},
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
      म_लिखो (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      ख_माला_दो (_("Print value of a symbolic link or canonical file name\n\n"),
             मानक_निकास);
      oputs (_("\
  -f, --canonicalize\n\
         canonicalize by following every symlink\n\
         in every component of the given name recursively;\n\
         all but the last component must exist\n\
"));
      oputs (_("\
  -e, --canonicalize-existing\n\
         canonicalize by following every symlink\n\
         in every component of the given name recursively;\n\
         all components must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing\n\
         canonicalize by following every symlink\n\
         in every component of the given name recursively,\n\
         without requirements on components existence\n\
"));
      oputs (_("\
  -n, --no-newline\n\
         करो not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by शेष अगर POSIXLY_CORRECT is not set)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by शेष अगर POSIXLY_CORRECT is set)\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output पंक्ति with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  /* If not -1, use this method to canonicalize.  */
  पूर्णांक can_mode = -1;
  पूर्णांक status = निकास_सफल;
  पूर्णांक optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, शून्य)) != -1)
    {
      चयन (optc)
        {
        हाल 'e':
          can_mode = CAN_EXISTING;
          अवरोध;
        हाल 'f':
          can_mode = CAN_ALL_BUT_LAST;
          अवरोध;
        हाल 'm':
          can_mode = CAN_MISSING;
          अवरोध;
        हाल 'n':
          no_newline = true;
          अवरोध;
        हाल 'q':
        हाल 's':
          verbose = false;
          अवरोध;
        हाल 'v':
          verbose = true;
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

  अगर (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  अगर (argc - optind > 1)
    {
      अगर (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
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

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  अगर (दो_पर्या ("POSIXLY_CORRECT") != शून्य)
    verbose = true;

  क्रम (; optind < argc; ++optind)
    {
      अक्षर स्थिर *fname = argv[optind];
      अक्षर *मूल्य = (can_mode != -1
                     ? canonicalize_filename_mode (fname, can_mode)
                     : areadlink_with_आकार (fname, 63));
      अगर (मूल्य)
        {
          ख_माला_दो (quote_output ? quoteN (मूल्य) : मूल्य, मानक_निकास);
          अगर (! no_newline)
            अक्षर_दो (use_nuls ? '\0' : '\n');
          मुक्त (मूल्य);
          अगर (ख_त्रुटि (मानक_निकास))
            write_error ();
        }
      अन्यथा
        {
          status = निकास_त्रुटि;
          अगर (verbose)
            error (0, त्रुटि_सं, "%s", quotef (fname));
        }
    }

  वापस status;
}
