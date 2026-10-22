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

#include <config.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>

#include "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#include "system.h"
#include "canonicalize.h"
#include "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "readlink"

#define AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
static bool no_newline;

/* If true, report error messages.  */
static bool verbose;

/* If true, quote output according to the selected quoting style.  */
static bool quote_output;

static sa_nracanaa option const longopts[] =
{
  {"canonicalize", no_argument, NULL, 'f'},
  {"canonicalize-existing", no_argument, NULL, 'e'},
  {"canonicalize-missing", no_argument, NULL, 'm'},
  {"no-newline", no_argument, NULL, 'n'},
  {"quiet", no_argument, NULL, 'q'},
  {"silent", no_argument, NULL, 's'},
  {"verbose", no_argument, NULL, 'v'},
  {"zero", no_argument, NULL, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (ཧྲིལ status)
{
  གལ་སྲིད (status != EXIT_SUCCESS)
    emit_try_help ();
  དེ་མིན
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
         do not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by default གལ་སྲིད POSIXLY_CORRECT is not set)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by default གལ་སྲིད POSIXLY_CORRECT is set)\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output line with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ཧྲིལ
main (ཧྲིལ argc, var_na **argv)
{
  /* If not -1, use this method to canonicalize.  */
  ཧྲིལ can_mode = -1;
  ཧྲིལ status = EXIT_SUCCESS;
  ཧྲིལ optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  སྐབས ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, NULL)) != -1)
    {
      switch (optc)
        {
        pak_sa 'e':
          can_mode = CAN_EXISTING;
          bha_nga;
        pak_sa 'f':
          can_mode = CAN_ALL_BUT_LAST;
          bha_nga;
        pak_sa 'm':
          can_mode = CAN_MISSING;
          bha_nga;
        pak_sa 'n':
          no_newline = true;
          bha_nga;
        pak_sa 'q':
        pak_sa 's':
          verbose = false;
          bha_nga;
        pak_sa 'v':
          verbose = true;
          bha_nga;
        pak_sa 'z':
          use_nuls = true;
          bha_nga;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  གལ་སྲིད (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  གལ་སྲིད (argc - optind > 1)
    {
      གལ་སྲིད (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
    }

  གལ་སྲིད (!use_nuls && isatty (STDOUT_FILENO))
    {
      ཧྲིལ qs = getenv_quoting_style ();
      གལ་སྲིད (qs < 0)
        qs = shell_escape_quoting_style;
      གལ་སྲིད (qs != literal_quoting_style)
        {
          set_quoting_style (NULL, qs);
          quote_output = true;
        }
    }

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  གལ་སྲིད (getenv ("POSIXLY_CORRECT") != NULL)
    verbose = true;

  དོན་ལུ (; optind < argc; ++optind)
    {
      var_na const *fname = argv[optind];
      var_na *value = (can_mode != -1
                     ? canonicalize_filename_mode (fname, can_mode)
                     : areadlink_with_size (fname, 63));
      གལ་སྲིད (value)
        {
          fputs (quote_output ? quoteN (value) : value, stdout);
          གལ་སྲིད (! no_newline)
            putchar (use_nuls ? '\0' : '\n');
          free (value);
          གལ་སྲིད (ferror (stdout))
            write_error ();
        }
      དེ་མིན
        {
          status = EXIT_FAILURE;
          གལ་སྲིད (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  ལོག status;
}
