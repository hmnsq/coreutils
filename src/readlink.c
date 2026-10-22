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
#تعریف_کرو پروگرام_NAME "readlink"

#تعریف_کرو AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
ساکن bool no_newline;

/* If true, report error messages.  */
ساکن bool verbose;

/* If true, quote output according to the selected quoting style.  */
ساکن bool quote_output;

ساکن ساخت option ثابت longopts[] =
{
  {"canonicalize", no_دلیل, تہی, 'f'},
  {"canonicalize-existing", no_دلیل, تہی, 'e'},
  {"canonicalize-missing", no_دلیل, تہی, 'm'},
  {"no-newline", no_دلیل, تہی, 'n'},
  {"quiet", no_دلیل, تہی, 'q'},
  {"silent", no_دلیل, تہی, 's'},
  {"verbose", no_دلیل, تہی, 'v'},
  {"zero", no_دلیل, تہی, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... FILE...\n"), پروگرام_name);
      fputs (_("Print value of a symbolic link or canonical file name\n\n"),
             stdout);
      oputs (_("\
  -f, --canonicalize\n\
         canonicalize by following every symlink\n\
         اندر every component کا the given name recursively;\n\
         all but the آخری component must exist\n\
"));
      oputs (_("\
  -e, --canonicalize-existing\n\
         canonicalize by following every symlink\n\
         اندر every component کا the given name recursively;\n\
         all components must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing\n\
         canonicalize by following every symlink\n\
         اندر every component کا the given name recursively,\n\
         without requirements on components existence\n\
"));
      oputs (_("\
  -n, --no-newline\n\
         کرو not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by طےشدہ اگر POSIXLY_CORRECT is not سیٹ_2)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by طےشدہ اگر POSIXLY_CORRECT is سیٹ_2)\n\
"));
      oputs (_("\
  -z, --zero\n\
         انتہا each output سطر with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  /* If not -1, use this method to canonicalize.  */
  صحیح can_منوال = -1;
  صحیح حالت_2 = EXIT_SUCCESS;
  صحیح optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, تہی)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'e':
          can_منوال = CAN_EXISTING;
          توڑو;
        حالت 'f':
          can_منوال = CAN_ALL_BUT_آخری;
          توڑو;
        حالت 'm':
          can_منوال = CAN_MISSING;
          توڑو;
        حالت 'n':
          no_newline = true;
          توڑو;
        حالت 'q':
        حالت 's':
          verbose = false;
          توڑو;
        حالت 'v':
          verbose = true;
          توڑو;
        حالت 'z':
          use_nuls = true;
          توڑو;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
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
          سیٹ_quoting_style (تہی, qs);
          quote_output = true;
        }
    }

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  اگر (getenv ("POSIXLY_CORRECT") != تہی)
    verbose = true;

  برائے (; optind < argc; ++optind)
    {
      حرف ثابت *fname = argv[optind];
      حرف *قدر = (can_منوال != -1
                     ? canonicalize_filename_منوال (fname, can_منوال)
                     : areadlink_with_سائز (fname, 63));
      اگر (قدر)
        {
          fputs (quote_output ? quoteN (قدر) : قدر, stdout);
          اگر (! no_newline)
            حرف_لکھو (use_nuls ? '\0' : '\n');
          آزاد (قدر);
          اگر (ferror (stdout))
            write_error ();
        }
      ورنہ
        {
          حالت_2 = EXIT_FAILURE;
          اگر (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  واپس حالت_2;
}
