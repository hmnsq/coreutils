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

स्थिर संरचना option ध्रुव longopts[] =
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
usage (पूर्णाङ्क status)
{
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("Print value of a symbolic link or canonical file name\n\n"),
             stdout);
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
         कुरु not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by स्वभाव यदि POSIXLY_CORRECT is not set)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by स्वभाव यदि POSIXLY_CORRECT is set)\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output ओळ with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  /* If not -1, use this method to canonicalize.  */
  पूर्णाङ्क can_mode = -1;
  पूर्णाङ्क status = EXIT_SUCCESS;
  पूर्णाङ्क optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'e':
          can_mode = CAN_EXISTING;
          भङ्ग;
        पक्ष 'f':
          can_mode = CAN_ALL_BUT_LAST;
          भङ्ग;
        पक्ष 'm':
          can_mode = CAN_MISSING;
          भङ्ग;
        पक्ष 'n':
          no_newline = true;
          भङ्ग;
        पक्ष 'q':
        पक्ष 's':
          verbose = false;
          भङ्ग;
        पक्ष 'v':
          verbose = true;
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

  यदि (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  यदि (argc - optind > 1)
    {
      यदि (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
    }

  यदि (!use_nuls && isatty (STDOUT_FILENO))
    {
      पूर्णाङ्क qs = getenv_quoting_style ();
      यदि (qs < 0)
        qs = shell_escape_quoting_style;
      यदि (qs != literal_quoting_style)
        {
          set_quoting_style (रिक्त, qs);
          quote_output = true;
        }
    }

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  यदि (getenv ("POSIXLY_CORRECT") != रिक्त)
    verbose = true;

  प्रति (; optind < argc; ++optind)
    {
      वर्ण ध्रुव *fname = argv[optind];
      वर्ण *मूल्य = (can_mode != -1
                     ? canonicalize_filename_mode (fname, can_mode)
                     : areadlink_with_size (fname, 63));
      यदि (मूल्य)
        {
          fputs (quote_output ? quoteN (मूल्य) : मूल्य, stdout);
          यदि (! no_newline)
            putchar (use_nuls ? '\0' : '\n');
          मुक्त (मूल्य);
          यदि (ferror (stdout))
            write_error ();
        }
      अन्यथा
        {
          status = EXIT_FAILURE;
          यदि (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  प्रत्यागम status;
}
