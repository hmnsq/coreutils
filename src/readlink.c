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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#अन्तर्भाव "system.h"
#अन्तर्भाव "canonicalize.h"
#अन्तर्भाव "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "readlink"

#define AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
स्थिर bool no_newline;

/* If true, report error messages.  */
स्थिर bool verbose;

/* If true, quote output according to the selected quoting style.  */
स्थिर bool quote_output;

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"canonicalize", no_argument, रिक्त, 'f'},
  {"canonicalize-existing", no_argument, रिक्त, 'e'},
  {"canonicalize-missing", no_argument, रिक्त, 'm'},
  {"no-newline", no_argument, रिक्त, 'n'},
  {"quiet", no_argument, रिक्त, 'q'},
  {"silent", no_argument, रिक्त, 's'},
  {"verbose", no_argument, रिक्त, 'v'},
  {"zero", no_argument, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
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
         करा not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by पूर्वनिर्धारित जर POSIXLY_CORRECT is not set)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by पूर्वनिर्धारित जर POSIXLY_CORRECT is set)\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output ओळ with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
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

  पर्यंत ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, रिक्त)) != -1)
    {
      निवड (optc)
        {
        स्थिती 'e':
          can_mode = CAN_EXISTING;
          थांबा;
        स्थिती 'f':
          can_mode = CAN_ALL_BUT_LAST;
          थांबा;
        स्थिती 'm':
          can_mode = CAN_MISSING;
          थांबा;
        स्थिती 'n':
          no_newline = true;
          थांबा;
        स्थिती 'q':
        स्थिती 's':
          verbose = false;
          थांबा;
        स्थिती 'v':
          verbose = true;
          थांबा;
        स्थिती 'z':
          use_nuls = true;
          थांबा;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  जर (argc - optind > 1)
    {
      जर (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
    }

  जर (!use_nuls && isatty (STDOUT_FILENO))
    {
      पूर्णांक qs = getenv_quoting_style ();
      जर (qs < 0)
        qs = shell_escape_quoting_style;
      जर (qs != literal_quoting_style)
        {
          set_quoting_style (रिक्त, qs);
          quote_output = true;
        }
    }

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  जर (दो_पर्या ("POSIXLY_CORRECT") != रिक्त)
    verbose = true;

  साठी (; optind < argc; ++optind)
    {
      अक्षर स्थिरांक *fname = argv[optind];
      अक्षर *मूल्य = (can_mode != -1
                     ? canonicalize_filename_mode (fname, can_mode)
                     : areadlink_with_size (fname, 63));
      जर (मूल्य)
        {
          ख_माला_दो (quote_output ? quoteN (मूल्य) : मूल्य, मानक_निकास);
          जर (! no_newline)
            अक्षर_दो (use_nuls ? '\0' : '\n');
          मुक्त (मूल्य);
          जर (ख_त्रुटि (मानक_निकास))
            write_error ();
        }
      अन्यथा
        {
          status = निकास_त्रुटि;
          जर (verbose)
            error (0, त्रुटि_सं, "%s", quotef (fname));
        }
    }

  परत status;
}
