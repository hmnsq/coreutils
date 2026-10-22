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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "canonicalize.h"
#அந்தர்பாவ "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "readlink"

#define AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
நிலையான bool no_newline;

/* If true, report error messages.  */
நிலையான bool verbose;

/* If true, quote output according to the selected quoting style.  */
நிலையான bool quote_output;

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"canonicalize", no_argument, வெற்று, 'f'},
  {"canonicalize-existing", no_argument, வெற்று, 'e'},
  {"canonicalize-missing", no_argument, வெற்று, 'm'},
  {"no-newline", no_argument, வெற்று, 'n'},
  {"quiet", no_argument, வெற்று, 'q'},
  {"silent", no_argument, வெற்று, 's'},
  {"verbose", no_argument, வெற்று, 'v'},
  {"zero", no_argument, வெற்று, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
         செய் not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by இயல்பு எனில் POSIXLY_CORRECT is not set)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by இயல்பு எனில் POSIXLY_CORRECT is set)\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output வரி with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  /* If not -1, use this method to canonicalize.  */
  முழு can_mode = -1;
  முழு status = EXIT_SUCCESS;
  முழு optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, வெற்று)) != -1)
    {
      தேர்வு (optc)
        {
        நிலை 'e':
          can_mode = CAN_EXISTING;
          நிறுத்து;
        நிலை 'f':
          can_mode = CAN_ALL_BUT_LAST;
          நிறுத்து;
        நிலை 'm':
          can_mode = CAN_MISSING;
          நிறுத்து;
        நிலை 'n':
          no_newline = true;
          நிறுத்து;
        நிலை 'q':
        நிலை 's':
          verbose = false;
          நிறுத்து;
        நிலை 'v':
          verbose = true;
          நிறுத்து;
        நிலை 'z':
          use_nuls = true;
          நிறுத்து;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  எனில் (argc - optind > 1)
    {
      எனில் (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
    }

  எனில் (!use_nuls && isatty (STDOUT_FILENO))
    {
      முழு qs = getenv_quoting_style ();
      எனில் (qs < 0)
        qs = shell_escape_quoting_style;
      எனில் (qs != literal_quoting_style)
        {
          set_quoting_style (வெற்று, qs);
          quote_output = true;
        }
    }

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  எனில் (getenv ("POSIXLY_CORRECT") != வெற்று)
    verbose = true;

  க்கு (; optind < argc; ++optind)
    {
      எழுத்து மாறிலி *fname = argv[optind];
      எழுத்து *மதிப்பு = (can_mode != -1
                     ? canonicalize_filename_mode (fname, can_mode)
                     : areadlink_with_size (fname, 63));
      எனில் (மதிப்பு)
        {
          fputs (quote_output ? quoteN (மதிப்பு) : மதிப்பு, stdout);
          எனில் (! no_newline)
            putchar (use_nuls ? '\0' : '\n');
          விடுவி (மதிப்பு);
          எனில் (ferror (stdout))
            write_error ();
        }
      இல்லையெனில்
        {
          status = EXIT_FAILURE;
          எனில் (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  திரும்பு status;
}
