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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "canonicalize.h"
#ଅନ୍ତର୍ଭାଵ "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "readlink"

#define AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
ସ୍ଥିର bool no_newline;

/* If true, report error messages.  */
ସ୍ଥିର bool verbose;

/* If true, quote output according to the selected quoting style.  */
ସ୍ଥିର bool quote_output;

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"canonicalize", no_argument, ଖାଲି, 'f'},
  {"canonicalize-existing", no_argument, ଖାଲି, 'e'},
  {"canonicalize-missing", no_argument, ଖାଲି, 'm'},
  {"no-newline", no_argument, ଖାଲି, 'n'},
  {"quiet", no_argument, ଖାଲି, 'q'},
  {"silent", no_argument, ଖାଲି, 's'},
  {"verbose", no_argument, ଖାଲି, 'v'},
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
         କର not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by ପୂର୍ବନିର୍ଦ୍ଧାରିତ ଯଦି POSIXLY_CORRECT is not set)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by ପୂର୍ବନିର୍ଦ୍ଧାରିତ ଯଦି POSIXLY_CORRECT is set)\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output ଧାଡ଼ି with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  /* If not -1, use this method to canonicalize.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା can_mode = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା status = EXIT_SUCCESS;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, ଖାଲି)) != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'e':
          can_mode = CAN_EXISTING;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'f':
          can_mode = CAN_ALL_BUT_LAST;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'm':
          can_mode = CAN_MISSING;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'n':
          no_newline = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'q':
        ସ୍ଥିତି 's':
          verbose = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'v':
          verbose = true;
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

  ଯଦି (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (argc - optind > 1)
    {
      ଯଦି (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
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

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  ଯଦି (getenv ("POSIXLY_CORRECT") != ଖାଲି)
    verbose = true;

  ପାଇଁ (; optind < argc; ++optind)
    {
      ଅକ୍ଷର ଅଚଳ *fname = argv[optind];
      ଅକ୍ଷର *ମୂଲ୍ୟ = (can_mode != -1
                     ? canonicalize_filename_mode (fname, can_mode)
                     : areadlink_with_size (fname, 63));
      ଯଦି (ମୂଲ୍ୟ)
        {
          fputs (quote_output ? quoteN (ମୂଲ୍ୟ) : ମୂଲ୍ୟ, stdout);
          ଯଦି (! no_newline)
            putchar (use_nuls ? '\0' : '\n');
          ମୁକ୍ତ (ମୂଲ୍ୟ);
          ଯଦି (ferror (stdout))
            write_error ();
        }
      ଅନ୍ୟଥା
        {
          status = EXIT_FAILURE;
          ଯଦି (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  ଫେରନ୍ତୁ status;
}
