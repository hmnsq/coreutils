/* realpath - print the resolved path
   Copyright (C) 2011-2026 Free Software Foundation, Inc.

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

/* Written by Pádraig Brady.  */

#شامل <config.h>
#شامل <getopt.h>
#شامل <stdio.h>
#شامل <sys/types.h>

#شامل "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#شامل "system.h"
#شامل "canonicalize.h"
#شامل "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "realpath"

#تعریف_کرو AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

شمار
{
  RELATIVE_مقصد_OPTION = CHAR_MAX + 1,
  RELATIVE_بنیاد_OPTION
};

ساکن bool verbose = true;
ساکن bool logical;
ساکن bool use_nuls;
ساکن bool quote_output;
ساکن حرف ثابت *can_relative_مقصد;
ساکن حرف ثابت *can_relative_بنیاد;

ساکن ساخت option ثابت longopts[] =
{
  {"canonicalize", no_دلیل, تہی, 'E'},
  {"canonicalize-existing", no_دلیل, تہی, 'e'},
  {"canonicalize-missing", no_دلیل, تہی, 'm'},
  {"relative-to", required_دلیل, تہی, RELATIVE_مقصد_OPTION},
  {"relative-base", required_دلیل, تہی, RELATIVE_بنیاد_OPTION},
  {"quiet", no_دلیل, تہی, 'q'},
  {"strip", no_دلیل, تہی, 's'},
  {"no-symlinks", no_دلیل, تہی, 's'},
  {"zero", no_دلیل, تہی, 'z'},
  {"logical", no_دلیل, تہی, 'L'},
  {"physical", no_دلیل, تہی, 'P'},
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
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the آخری component must exist (طےشدہ)\
\n\
"));
      oputs (_("\
  -e, --canonicalize-existing  all components کا the راستہ must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing   no راستہ components need exist or be a directory\
\n\
"));
      oputs (_("\
  -L, --logical                resolve '..' components before symlinks\n\
"));
      oputs (_("\
  -P, --physical               resolve symlinks as encountered (طےشدہ)\n\
"));
      oputs (_("\
  -q, --quiet                  suppress most error messages\n\
"));
      oputs (_("\
      --relative-مقصد=DIR        print the resolved راستہ relative مقصد DIR\n\
"));
      oputs (_("\
      --relative-بنیاد_2=DIR      print absolute paths unless paths نیچے DIR\n\
"));
      oputs (_("\
  -s, --strip, --no-symlinks   don't expand symlinks\n\
"));
      oputs (_("\
  -z, --zero                   انتہا each output سطر with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
ساکن حرف *
realpath_canon (حرف ثابت *fname, صحیح can_منوال)
{
  حرف *can_fname = canonicalize_filename_منوال (fname, can_منوال);
  اگر (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_منوال &= ~CAN_NOLINKS;
      حرف *can_fname2 = canonicalize_filename_منوال (can_fname, can_منوال);
      آزاد (can_fname);
      واپس can_fname2;
    }
  واپس can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_خالص
ساکن bool
راستہ_سابقہ (حرف ثابت *سابقہ, حرف ثابت *راستہ)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  سابقہ++;
  راستہ++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  اگر (!*سابقہ)
    واپس *راستہ != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  اگر (*سابقہ == '/' && !سابقہ[1])
    واپس *راستہ == '/';

  /* Any other prefix has a non-slash portion.  */
  جب (*سابقہ && *راستہ)
    {
      اگر (*سابقہ != *راستہ)
        توڑو;
      سابقہ++;
      راستہ++;
    }
  واپس (!*سابقہ && (*راستہ == '/' || !*راستہ));
}

ساکن bool
isdir (حرف ثابت *راستہ)
{
  ساخت stat sb;
  اگر (stat (راستہ, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (راستہ));
  واپس S_ISDIR (sb.st_منوال);
}

ساکن خالی
print_راستہ (حرف ثابت *راستہ)
{
  fputs (quote_output ? quoteN (راستہ) : راستہ, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
ساکن bool
print_relative_راستہ (حرف ثابت *can_fname)
{
  اگر (!quote_output)
    واپس relpath (can_fname, can_relative_مقصد, تہی, 0);

  size_t سائز;
  اگر (ckd_mul (&سائز, لڑی_لمبائی (can_relative_مقصد), 2)
      || ckd_جوڑو (&سائز, سائز, لڑی_لمبائی (can_fname))
      || ckd_جوڑو (&سائز, سائز, 1))
    xalloc_die ();

  حرف *relative_راستہ = xmalloc (سائز);
  bool success = relpath (can_fname, can_relative_مقصد, relative_راستہ, سائز);
  اگر (success)
    print_راستہ (relative_راستہ);
  آزاد (relative_راستہ);
  واپس success;
}

ساکن bool
process_راستہ (حرف ثابت *fname, صحیح can_منوال)
{
  حرف *can_fname = realpath_canon (fname, can_منوال);
  اگر (!can_fname)
    {
      اگر (verbose)
        error (0, errno, "%s", quotef (fname));
      واپس false;
    }

  اگر (!can_relative_مقصد
      || (can_relative_بنیاد && !راستہ_سابقہ (can_relative_بنیاد, can_fname))
      || (can_relative_مقصد && !print_relative_راستہ (can_fname)))
    print_راستہ (can_fname);

  حرف_لکھو (use_nuls ? '\0' : '\n');

  آزاد (can_fname);

  اگر (ferror (stdout))
    write_error ();

  واپس true;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool ok = true;
  صحیح can_منوال = CAN_ALL_BUT_آخری;
  حرف ثابت *relative_مقصد = تہی;
  حرف ثابت *relative_بنیاد = تہی;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب (true)
    {
      صحیح c = getopt_long (argc, argv, "EeLmPqsz", longopts, تہی);
      اگر (c == -1)
        توڑو;
      انتخاب (c)
        {
        حالت 'E':
          can_منوال &= ~CAN_منوال_نقاب;
          can_منوال |= CAN_ALL_BUT_آخری;
          توڑو;
        حالت 'e':
          can_منوال &= ~CAN_منوال_نقاب;
          can_منوال |= CAN_EXISTING;
          توڑو;
        حالت 'm':
          can_منوال &= ~CAN_منوال_نقاب;
          can_منوال |= CAN_MISSING;
          توڑو;
        حالت 'L':
          can_منوال |= CAN_NOLINKS;
          logical = true;
          توڑو;
        حالت 's':
          can_منوال |= CAN_NOLINKS;
          logical = false;
          توڑو;
        حالت 'P':
          can_منوال &= ~CAN_NOLINKS;
          logical = false;
          توڑو;
        حالت 'q':
          verbose = false;
          توڑو;
        حالت 'z':
          use_nuls = true;
          توڑو;
        حالت RELATIVE_مقصد_OPTION:
          relative_مقصد = optarg;
          توڑو;
        حالت RELATIVE_بنیاد_OPTION:
          relative_بنیاد = optarg;
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

  اگر (! use_nuls && isatty (STDOUT_FILENO))
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

  اگر (relative_بنیاد && !relative_مقصد)
    relative_مقصد = relative_بنیاد;

  bool need_dir = (can_منوال & CAN_منوال_نقاب) == CAN_EXISTING;
  اگر (relative_مقصد)
    {
      can_relative_مقصد = realpath_canon (relative_مقصد, can_منوال);
      اگر (!can_relative_مقصد)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_مقصد));
      اگر (need_dir && !isdir (can_relative_مقصد))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_مقصد));
    }
  اگر (relative_بنیاد == relative_مقصد)
    can_relative_بنیاد = can_relative_مقصد;
  ورنہ اگر (relative_بنیاد)
    {
      حرف *بنیاد_2 = realpath_canon (relative_بنیاد, can_منوال);
      اگر (!بنیاد_2)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_بنیاد));
      اگر (need_dir && !isdir (بنیاد_2))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_بنیاد));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      اگر (راستہ_سابقہ (بنیاد_2, can_relative_مقصد))
        can_relative_بنیاد = بنیاد_2;
      ورنہ
        {
          آزاد (بنیاد_2);
          can_relative_بنیاد = can_relative_مقصد;
          can_relative_مقصد = تہی;
        }
    }

  برائے (; optind < argc; ++optind)
    ok &= process_راستہ (argv[optind], can_منوال);

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
