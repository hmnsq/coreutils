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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>

#شامل "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#شامل "system.h"
#شامل "canonicalize.h"
#شامل "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "readlink"

#define AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
ایستا bool no_newline;

/* If true, report error messages.  */
ایستا bool verbose;

/* If true, quote output according to the selected quoting style.  */
ایستا bool quote_output;

ایستا ساختار option ثابت longopts[] =
{
  {"canonicalize", no_argument, پوچ, 'f'},
  {"canonicalize-existing", no_argument, پوچ, 'e'},
  {"canonicalize-missing", no_argument, پوچ, 'm'},
  {"no-newline", no_argument, پوچ, 'n'},
  {"quiet", no_argument, پوچ, 'q'},
  {"silent", no_argument, پوچ, 's'},
  {"verbose", no_argument, پوچ, 'v'},
  {"zero", no_argument, پوچ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
         انجامبده not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by پیشفرض اگر POSIXLY_CORRECT is not set)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by پیشفرض اگر POSIXLY_CORRECT is set)\n\
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

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  /* If not -1, use this method to canonicalize.  */
  صحیح can_mode = -1;
  صحیح status = EXIT_SUCCESS;
  صحیح optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, پوچ)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'e':
          can_mode = CAN_EXISTING;
          بشکن;
        حالت 'f':
          can_mode = CAN_ALL_BUT_LAST;
          بشکن;
        حالت 'm':
          can_mode = CAN_MISSING;
          بشکن;
        حالت 'n':
          no_newline = true;
          بشکن;
        حالت 'q':
        حالت 's':
          verbose = false;
          بشکن;
        حالت 'v':
          verbose = true;
          بشکن;
        حالت 'z':
          use_nuls = true;
          بشکن;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  اگر (argc - optind > 1)
    {
      اگر (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
    }

  اگر (!use_nuls && isatty (STDOUT_FILENO))
    {
      صحیح qs = getenv_quoting_style ();
      اگر (qs < 0)
        qs = shell_escape_quoting_style;
      اگر (qs != literal_quoting_style)
        {
          set_quoting_style (پوچ, qs);
          quote_output = true;
        }
    }

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  اگر (getenv ("POSIXLY_CORRECT") != پوچ)
    verbose = true;

  برای (; optind < argc; ++optind)
    {
      نویسه ثابت *fname = argv[optind];
      نویسه *مقدار = (can_mode != -1
                     ? canonicalize_filename_mode (fname, can_mode)
                     : areadlink_with_size (fname, 63));
      اگر (مقدار)
        {
          fputs (quote_output ? quoteN (مقدار) : مقدار, stdout);
          اگر (! no_newline)
            putchar (use_nuls ? '\0' : '\n');
          آزاد (مقدار);
          اگر (ferror (stdout))
            write_error ();
        }
      وگرنه
        {
          status = EXIT_FAILURE;
          اگر (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  بازگشت status;
}
