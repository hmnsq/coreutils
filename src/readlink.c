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
#تعریف_کن برنامه_NAME "readlink"

#تعریف_کن AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
ساكن bool no_newline;

/* If true, report error messages.  */
ساكن bool verbose;

/* If true, quote output according to the selected quoting style.  */
ساكن bool quote_output;

ساكن بنية option ثابت longopts[] =
{
  {"canonicalize", no_آرگومان, عدم, 'f'},
  {"canonicalize-existing", no_آرگومان, عدم, 'e'},
  {"canonicalize-missing", no_آرگومان, عدم, 'm'},
  {"no-newline", no_آرگومان, عدم, 'n'},
  {"quiet", no_آرگومان, عدم, 'q'},
  {"silent", no_آرگومان, عدم, 's'},
  {"verbose", no_آرگومان, عدم, 'v'},
  {"zero", no_آرگومان, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("Usage: %s [OPTION]... FILE...\n"), برنامه_name);
      fputs (_("Print value of a symbolic link or canonical file name\n\n"),
             stdout);
      oputs (_("\
  -f, --canonicalize\n\
         canonicalize by following every symlink\n\
         درون every component از the given name recursively;\n\
         all but the آخرین component must exist\n\
"));
      oputs (_("\
  -e, --canonicalize-existing\n\
         canonicalize by following every symlink\n\
         درون every component از the given name recursively;\n\
         all components must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing\n\
         canonicalize by following every symlink\n\
         درون every component از the given name recursively,\n\
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
         (on by افتراضي إذا POSIXLY_CORRECT is not مجموعه_2)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by افتراضي إذا POSIXLY_CORRECT is مجموعه_2)\n\
"));
      oputs (_("\
  -z, --zero\n\
         انتها each output سطر with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  /* If not -1, use this method to canonicalize.  */
  صحيح can_نما = -1;
  صحيح وضعیت = EXIT_SUCCESS;
  صحيح optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'e':
          can_نما = CAN_EXISTING;
          اكسر;
        حالة 'f':
          can_نما = CAN_ALL_BUT_آخرین;
          اكسر;
        حالة 'm':
          can_نما = CAN_MISSING;
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
        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);
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
          مجموعه_quoting_style (عدم, qs);
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
      حرف *مقدار = (can_نما != -1
                     ? canonicalize_filename_نما (fname, can_نما)
                     : areadlink_with_اندازه (fname, 63));
      إذا (مقدار)
        {
          fputs (quote_output ? quoteN (مقدار) : مقدار, stdout);
          إذا (! no_newline)
            حرف_چاپ_کن (use_nuls ? '\0' : '\n');
          حرر (مقدار);
          إذا (ferror (stdout))
            write_error ();
        }
      وإلا
        {
          وضعیت = EXIT_FAILURE;
          إذا (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  أرجع وضعیت;
}
