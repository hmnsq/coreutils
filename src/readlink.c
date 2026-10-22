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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "canonicalize.h"
#অন্তর্ভাব "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "readlink"

#define AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
স্থির bool no_newline;

/* If true, report error messages.  */
স্থির bool verbose;

/* If true, quote output according to the selected quoting style.  */
স্থির bool quote_output;

স্থির সংরচনা option ধ্রুব longopts[] =
{
  {"canonicalize", no_argument, রিক্ত, 'f'},
  {"canonicalize-existing", no_argument, রিক্ত, 'e'},
  {"canonicalize-missing", no_argument, রিক্ত, 'm'},
  {"no-newline", no_argument, রিক্ত, 'n'},
  {"quiet", no_argument, রিক্ত, 'q'},
  {"silent", no_argument, রিক্ত, 's'},
  {"verbose", no_argument, রিক্ত, 'v'},
  {"zero", no_argument, রিক্ত, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
         কুরু not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by স্বভাব যদি POSIXLY_CORRECT is not set)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by স্বভাব যদি POSIXLY_CORRECT is set)\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output সারি with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  /* If not -1, use this method to canonicalize.  */
  পূর্ণাঙ্ক can_mode = -1;
  পূর্ণাঙ্ক status = EXIT_SUCCESS;
  পূর্ণাঙ্ক optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যাবত্ ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, রিক্ত)) != -1)
    {
      বিকল্প (optc)
        {
        পক্ষ 'e':
          can_mode = CAN_EXISTING;
          ভঙ্গ;
        পক্ষ 'f':
          can_mode = CAN_ALL_BUT_LAST;
          ভঙ্গ;
        পক্ষ 'm':
          can_mode = CAN_MISSING;
          ভঙ্গ;
        পক্ষ 'n':
          no_newline = true;
          ভঙ্গ;
        পক্ষ 'q':
        পক্ষ 's':
          verbose = false;
          ভঙ্গ;
        পক্ষ 'v':
          verbose = true;
          ভঙ্গ;
        পক্ষ 'z':
          use_nuls = true;
          ভঙ্গ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্বভাব:
          usage (EXIT_FAILURE);
        }
    }

  যদি (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  যদি (argc - optind > 1)
    {
      যদি (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
    }

  যদি (!use_nuls && isatty (STDOUT_FILENO))
    {
      পূর্ণাঙ্ক qs = getenv_quoting_style ();
      যদি (qs < 0)
        qs = shell_escape_quoting_style;
      যদি (qs != literal_quoting_style)
        {
          set_quoting_style (রিক্ত, qs);
          quote_output = true;
        }
    }

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  যদি (getenv ("POSIXLY_CORRECT") != রিক্ত)
    verbose = true;

  প্রতি (; optind < argc; ++optind)
    {
      বর্ণ ধ্রুব *fname = argv[optind];
      বর্ণ *মান = (can_mode != -1
                     ? canonicalize_filename_mode (fname, can_mode)
                     : areadlink_with_size (fname, 63));
      যদি (মান)
        {
          fputs (quote_output ? quoteN (মান) : মান, stdout);
          যদি (! no_newline)
            putchar (use_nuls ? '\0' : '\n');
          মুক্ত (মান);
          যদি (ferror (stdout))
            write_error ();
        }
      অন্যথা
        {
          status = EXIT_FAILURE;
          যদি (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  প্রত্যাগম status;
}
