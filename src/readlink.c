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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>

#ضمن "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ضمن "system.h"
#ضمن "canonicalize.h"
#ضمن "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "readlink"

#define AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
ساكن bool no_newline;

/* If true, report error messages.  */
ساكن bool verbose;

/* If true, quote output according to the selected quoting style.  */
ساكن bool quote_output;

ساكن بنية option ثابت longopts[] =
{
  {"canonicalize", no_argument, عدم, 'f'},
  {"canonicalize-existing", no_argument, عدم, 'e'},
  {"canonicalize-missing", no_argument, عدم, 'm'},
  {"no-newline", no_argument, عدم, 'n'},
  {"quiet", no_argument, عدم, 'q'},
  {"silent", no_argument, عدم, 's'},
  {"verbose", no_argument, عدم, 'v'},
  {"zero", no_argument, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
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
         افعل not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by افتراضي إذا POSIXLY_CORRECT is not set)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by افتراضي إذا POSIXLY_CORRECT is set)\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output سطر with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحيح
الرئيسية (صحيح argc, حرف **argv)
{
  /* If not -1, use this method to canonicalize.  */
  صحيح can_mode = -1;
  صحيح status = EXIT_SUCCESS;
  صحيح optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  بينما ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'e':
          can_mode = CAN_EXISTING;
          اكسر;
        حالة 'f':
          can_mode = CAN_ALL_BUT_LAST;
          اكسر;
        حالة 'm':
          can_mode = CAN_MISSING;
          اكسر;
        حالة 'n':
          no_newline = true;
          اكسر;
        حالة 'q':
        حالة 's':
          verbose = false;
          اكسر;
        حالة 'v':
          verbose = true;
          اكسر;
        حالة 'z':
          use_nuls = true;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  إذا (argc - optind > 1)
    {
      إذا (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
    }

  إذا (!use_nuls && isatty (STDOUT_FILENO))
    {
      صحيح qs = getenv_quoting_style ();
      إذا (qs < 0)
        qs = shell_escape_quoting_style;
      إذا (qs != literal_quoting_style)
        {
          set_quoting_style (عدم, qs);
          quote_output = true;
        }
    }

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  إذا (getenv ("POSIXLY_CORRECT") != عدم)
    verbose = true;

  لكل (; optind < argc; ++optind)
    {
      حرف ثابت *fname = argv[optind];
      حرف *قيمة = (can_mode != -1
                     ? canonicalize_filename_mode (fname, can_mode)
                     : areadlink_with_size (fname, 63));
      إذا (قيمة)
        {
          fputs (quote_output ? quoteN (قيمة) : قيمة, stdout);
          إذا (! no_newline)
            putchar (use_nuls ? '\0' : '\n');
          حرر (قيمة);
          إذا (ferror (stdout))
            write_error ();
        }
      وإلا
        {
          status = EXIT_FAILURE;
          إذا (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  إرجاع status;
}
