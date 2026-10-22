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
#تعریف_کړه پروګرام_NAME "readlink"

#تعریف_کړه AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
ساكن bool no_newline;

/* If true, report error messages.  */
ساكن bool verbose;

/* If true, quote output according to the selected quoting style.  */
ساكن bool quote_output;

ساكن بنية option ثابت longopts[] =
{
  {"canonicalize", no_دلیل, عدم, 'f'},
  {"canonicalize-existing", no_دلیل, عدم, 'e'},
  {"canonicalize-missing", no_دلیل, عدم, 'm'},
  {"no-newline", no_دلیل, عدم, 'n'},
  {"quiet", no_دلیل, عدم, 'q'},
  {"silent", no_دلیل, عدم, 's'},
  {"verbose", no_دلیل, عدم, 'v'},
  {"zero", no_دلیل, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("Usage: %s [OPTION]... FILE...\n"), پروګرام_name);
      fputs (_("Print value of a symbolic link or canonical file name\n\n"),
             stdout);
      oputs (_("\
  -f, --canonicalize\n\
         canonicalize by following every symlink\n\
         دننه every component د the given name recursively;\n\
         all but the وروستی component must exist\n\
"));
      oputs (_("\
  -e, --canonicalize-existing\n\
         canonicalize by following every symlink\n\
         دننه every component د the given name recursively;\n\
         all components must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing\n\
         canonicalize by following every symlink\n\
         دننه every component د the given name recursively,\n\
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
         (on by افتراضي إذا POSIXLY_CORRECT is not سېټ_2)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by افتراضي إذا POSIXLY_CORRECT is سېټ_2)\n\
"));
      oputs (_("\
  -z, --zero\n\
         پای_ټکی each output کرښه with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  /* If not -1, use this method to canonicalize.  */
  صحيح can_ډېرپېښ = -1;
  صحيح حالت = EXIT_SUCCESS;
  صحيح optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'e':
          can_ډېرپېښ = CAN_EXISTING;
          اكسر;
        حالة 'f':
          can_ډېرپېښ = CAN_ALL_BUT_وروستی;
          اكسر;
        حالة 'm':
          can_ډېرپېښ = CAN_MISSING;
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
        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);
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
          سېټ_quoting_style (عدم, qs);
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
      حرف *ارزښت = (can_ډېرپېښ != -1
                     ? canonicalize_filename_ډېرپېښ (fname, can_ډېرپېښ)
                     : areadlink_with_اندازه (fname, 63));
      إذا (ارزښت)
        {
          fputs (quote_output ? quoteN (ارزښت) : ارزښت, stdout);
          إذا (! no_newline)
            توری_چاپ_کړه (use_nuls ? '\0' : '\n');
          حرر (ارزښت);
          إذا (ferror (stdout))
            write_error ();
        }
      وإلا
        {
          حالت = EXIT_FAILURE;
          إذا (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  أرجع حالت;
}
