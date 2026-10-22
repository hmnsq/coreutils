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

#ضمن <config.h>
#ضمن <getopt.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ضمن "system.h"
#ضمن "canonicalize.h"
#ضمن "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "realpath"

#تعریف_کن AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

تعداد
{
  RELATIVE_مقصد_OPTION = CHAR_MAX + 1,
  RELATIVE_پایه_OPTION
};

ساكن bool verbose = true;
ساكن bool logical;
ساكن bool use_nuls;
ساكن bool quote_output;
ساكن حرف ثابت *can_relative_مقصد;
ساكن حرف ثابت *can_relative_پایه;

ساكن بنية option ثابت longopts[] =
{
  {"canonicalize", no_آرگومان, عدم, 'E'},
  {"canonicalize-existing", no_آرگومان, عدم, 'e'},
  {"canonicalize-missing", no_آرگومان, عدم, 'm'},
  {"relative-to", required_آرگومان, عدم, RELATIVE_مقصد_OPTION},
  {"relative-base", required_آرگومان, عدم, RELATIVE_پایه_OPTION},
  {"quiet", no_آرگومان, عدم, 'q'},
  {"strip", no_آرگومان, عدم, 's'},
  {"no-symlinks", no_آرگومان, عدم, 's'},
  {"zero", no_آرگومان, عدم, 'z'},
  {"logical", no_آرگومان, عدم, 'L'},
  {"physical", no_آرگومان, عدم, 'P'},
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
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the آخرین component must exist (افتراضي)\
\n\
"));
      oputs (_("\
  -e, --canonicalize-existing  all components از the مسیر must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing   no مسیر components need exist or be a directory\
\n\
"));
      oputs (_("\
  -L, --logical                resolve '..' components before symlinks\n\
"));
      oputs (_("\
  -P, --physical               resolve symlinks as encountered (افتراضي)\n\
"));
      oputs (_("\
  -q, --quiet                  suppress most error messages\n\
"));
      oputs (_("\
      --relative-مقصد=DIR        print the resolved مسیر relative مقصد DIR\n\
"));
      oputs (_("\
      --relative-پایه_2=DIR      print absolute paths unless paths زیر DIR\n\
"));
      oputs (_("\
  -s, --strip, --no-symlinks   don't expand symlinks\n\
"));
      oputs (_("\
  -z, --zero                   انتها each output سطر with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
ساكن حرف *
realpath_canon (حرف ثابت *fname, صحيح can_نما)
{
  حرف *can_fname = canonicalize_filename_نما (fname, can_نما);
  إذا (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_نما &= ~CAN_NOLINKS;
      حرف *can_fname2 = canonicalize_filename_نما (can_fname, can_نما);
      حرر (can_fname);
      أرجع can_fname2;
    }
  أرجع can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_خالص
ساكن bool
مسیر_پیشوند (حرف ثابت *پیشوند, حرف ثابت *مسیر)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  پیشوند++;
  مسیر++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  إذا (!*پیشوند)
    أرجع *مسیر != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  إذا (*پیشوند == '/' && !پیشوند[1])
    أرجع *مسیر == '/';

  /* Any other prefix has a non-slash portion.  */
  طالما (*پیشوند && *مسیر)
    {
      إذا (*پیشوند != *مسیر)
        اكسر;
      پیشوند++;
      مسیر++;
    }
  أرجع (!*پیشوند && (*مسیر == '/' || !*مسیر));
}

ساكن bool
isdir (حرف ثابت *مسیر)
{
  بنية stat sb;
  إذا (stat (مسیر, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (مسیر));
  أرجع S_ISDIR (sb.st_نما);
}

ساكن فراغ
print_مسیر (حرف ثابت *مسیر)
{
  fputs (quote_output ? quoteN (مسیر) : مسیر, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
ساكن bool
print_relative_مسیر (حرف ثابت *can_fname)
{
  إذا (!quote_output)
    أرجع relpath (can_fname, can_relative_مقصد, عدم, 0);

  size_t اندازه;
  إذا (ckd_mul (&اندازه, طول_رشته (can_relative_مقصد), 2)
      || ckd_جمع_کن (&اندازه, اندازه, طول_رشته (can_fname))
      || ckd_جمع_کن (&اندازه, اندازه, 1))
    xalloc_die ();

  حرف *relative_مسیر = xmalloc (اندازه);
  bool success = relpath (can_fname, can_relative_مقصد, relative_مسیر, اندازه);
  إذا (success)
    print_مسیر (relative_مسیر);
  حرر (relative_مسیر);
  أرجع success;
}

ساكن bool
process_مسیر (حرف ثابت *fname, صحيح can_نما)
{
  حرف *can_fname = realpath_canon (fname, can_نما);
  إذا (!can_fname)
    {
      إذا (verbose)
        error (0, errno, "%s", quotef (fname));
      أرجع false;
    }

  إذا (!can_relative_مقصد
      || (can_relative_پایه && !مسیر_پیشوند (can_relative_پایه, can_fname))
      || (can_relative_مقصد && !print_relative_مسیر (can_fname)))
    print_مسیر (can_fname);

  حرف_چاپ_کن (use_nuls ? '\0' : '\n');

  حرر (can_fname);

  إذا (ferror (stdout))
    write_error ();

  أرجع true;
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  bool ok = true;
  صحيح can_نما = CAN_ALL_BUT_آخرین;
  حرف ثابت *relative_مقصد = عدم;
  حرف ثابت *relative_پایه = عدم;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما (true)
    {
      صحيح c = getopt_long (argc, argv, "EeLmPqsz", longopts, عدم);
      إذا (c == -1)
        اكسر;
      بدل (c)
        {
        حالة 'E':
          can_نما &= ~CAN_نما_ماسک;
          can_نما |= CAN_ALL_BUT_آخرین;
          اكسر;
        حالة 'e':
          can_نما &= ~CAN_نما_ماسک;
          can_نما |= CAN_EXISTING;
          اكسر;
        حالة 'm':
          can_نما &= ~CAN_نما_ماسک;
          can_نما |= CAN_MISSING;
          اكسر;
        حالة 'L':
          can_نما |= CAN_NOLINKS;
          logical = true;
          اكسر;
        حالة 's':
          can_نما |= CAN_NOLINKS;
          logical = false;
          اكسر;
        حالة 'P':
          can_نما &= ~CAN_NOLINKS;
          logical = false;
          اكسر;
        حالة 'q':
          verbose = false;
          اكسر;
        حالة 'z':
          use_nuls = true;
          اكسر;
        حالة RELATIVE_مقصد_OPTION:
          relative_مقصد = optarg;
          اكسر;
        حالة RELATIVE_پایه_OPTION:
          relative_پایه = optarg;
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

  إذا (! use_nuls && isatty (STDOUT_FILENO))
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

  إذا (relative_پایه && !relative_مقصد)
    relative_مقصد = relative_پایه;

  bool need_dir = (can_نما & CAN_نما_ماسک) == CAN_EXISTING;
  إذا (relative_مقصد)
    {
      can_relative_مقصد = realpath_canon (relative_مقصد, can_نما);
      إذا (!can_relative_مقصد)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_مقصد));
      إذا (need_dir && !isdir (can_relative_مقصد))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_مقصد));
    }
  إذا (relative_پایه == relative_مقصد)
    can_relative_پایه = can_relative_مقصد;
  وإلا إذا (relative_پایه)
    {
      حرف *پایه_2 = realpath_canon (relative_پایه, can_نما);
      إذا (!پایه_2)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_پایه));
      إذا (need_dir && !isdir (پایه_2))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_پایه));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      إذا (مسیر_پیشوند (پایه_2, can_relative_مقصد))
        can_relative_پایه = پایه_2;
      وإلا
        {
          حرر (پایه_2);
          can_relative_پایه = can_relative_مقصد;
          can_relative_مقصد = عدم;
        }
    }

  لكل (; optind < argc; ++optind)
    ok &= process_مسیر (argv[optind], can_نما);

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
