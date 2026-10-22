/* dirname -- strip suffix from file name

   Copyright (C) 1990-2026 Free Software Foundation, Inc.

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

/* Written by David MacKenzie and Jim Meyering. */

#شامل <config.h>
#شامل <getopt.h>
#شامل <stdio.h>
#شامل <sys/types.h>

#شامل "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#شامل "system.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "dirname"

#تعریف_کرو AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

ساکن bool quote_output;

ساکن ساخت option ثابت longopts[] =
{
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
      لکھو (_("\
Usage: %s [OPTION] NAME...\n\
"),
              پروگرام_name);
      fputs (_("\
Output each NAME with its آخری non-slash component and trailing slashes\n\
removed; اگر NAME شامل_ہے no /'s, output '.' (meaning the current directory).\n\
\n\
"), stdout);
      oputs (_("\
  -z, --zero\n\
         انتہا each output سطر with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      لکھو (_("\
\n\
Examples:\n\
  %s /usr/bin/          -> \"/usr\"\n\
  %s dir1/str dir2/str  -> \"dir1\" followed by \"dir2\"\n\
  %s stdio.h            -> \".\"\n\
"),
              پروگرام_name, پروگرام_name, پروگرام_name);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب (true)
    {
      صحیح c = getopt_long (argc, argv, "z", longopts, تہی);

      اگر (c == -1)
        توڑو;

      انتخاب (c)
        {
        حالت 'z':
          use_nuls = true;
          توڑو;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
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

  برائے (; optind < argc; optind++)
    {
      حرف ثابت *نتیجہ = argv[optind];
      idx_t len = dir_len (نتیجہ);

      اگر (! len)
        {
          ساکن حرف ثابت dot = '.';
          نتیجہ = &dot;
          len = 1;
        }
      اگر (quote_output)
        {
          نتیجہ = quoteN_mem (نتیجہ, len);
          len = لڑی_لمبائی (نتیجہ);
        }
      fwrite (نتیجہ, 1, len, stdout);
      حرف_لکھو (use_nuls ? '\0' :'\n');
    }

  واپس EXIT_SUCCESS;
}
