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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "canonicalize.h"
#અન્તર્ભાવ "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "readlink"

#define AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
સ્થિર bool no_newline;

/* If true, report error messages.  */
સ્થિર bool verbose;

/* If true, quote output according to the selected quoting style.  */
સ્થિર bool quote_output;

સ્થિર સંરચના option અચલ longopts[] =
{
  {"canonicalize", no_argument, ખાલી, 'f'},
  {"canonicalize-existing", no_argument, ખાલી, 'e'},
  {"canonicalize-missing", no_argument, ખાલી, 'm'},
  {"no-newline", no_argument, ખાલી, 'n'},
  {"quiet", no_argument, ખાલી, 'q'},
  {"silent", no_argument, ખાલી, 's'},
  {"verbose", no_argument, ખાલી, 'v'},
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
         કરો not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by મૂળભૂત જો POSIXLY_CORRECT is not set)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by મૂળભૂત જો POSIXLY_CORRECT is set)\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output લીટી with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  /* If not -1, use this method to canonicalize.  */
  પૂર્ણાંક can_mode = -1;
  પૂર્ણાંક status = EXIT_SUCCESS;
  પૂર્ણાંક optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, ખાલી)) != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'e':
          can_mode = CAN_EXISTING;
          તોડો;
        સ્થિતિ 'f':
          can_mode = CAN_ALL_BUT_LAST;
          તોડો;
        સ્થિતિ 'm':
          can_mode = CAN_MISSING;
          તોડો;
        સ્થિતિ 'n':
          no_newline = true;
          તોડો;
        સ્થિતિ 'q':
        સ્થિતિ 's':
          verbose = false;
          તોડો;
        સ્થિતિ 'v':
          verbose = true;
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

  જો (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  જો (argc - optind > 1)
    {
      જો (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
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

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  જો (getenv ("POSIXLY_CORRECT") != ખાલી)
    verbose = true;

  માટે (; optind < argc; ++optind)
    {
      અક્ષર અચલ *fname = argv[optind];
      અક્ષર *કિંમત = (can_mode != -1
                     ? canonicalize_filename_mode (fname, can_mode)
                     : areadlink_with_size (fname, 63));
      જો (કિંમત)
        {
          fputs (quote_output ? quoteN (કિંમત) : કિંમત, stdout);
          જો (! no_newline)
            putchar (use_nuls ? '\0' : '\n');
          મુક્ત (કિંમત);
          જો (ferror (stdout))
            write_error ();
        }
      નહીંતર
        {
          status = EXIT_FAILURE;
          જો (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  પાછા status;
}
