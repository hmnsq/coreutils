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
#تعریف_کرو پروگرام_NAME "realpath"

#تعریف_کرو AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

تعداد
{
  RELATIVE_منزل_OPTION = CHAR_MAX + 1,
  RELATIVE_ادھار_OPTION
};

ساكن bool verbose = true;
ساكن bool logical;
ساكن bool use_nuls;
ساكن bool quote_output;
ساكن حرف ثابت *can_relative_منزل;
ساكن حرف ثابت *can_relative_ادھار;

ساكن بنية option ثابت longopts[] =
{
  {"canonicalize", no_دلیل, عدم, 'E'},
  {"canonicalize-existing", no_دلیل, عدم, 'e'},
  {"canonicalize-missing", no_دلیل, عدم, 'm'},
  {"relative-to", required_دلیل, عدم, RELATIVE_منزل_OPTION},
  {"relative-base", required_دلیل, عدم, RELATIVE_ادھار_OPTION},
  {"quiet", no_دلیل, عدم, 'q'},
  {"strip", no_دلیل, عدم, 's'},
  {"no-symlinks", no_دلیل, عدم, 's'},
  {"zero", no_دلیل, عدم, 'z'},
  {"logical", no_دلیل, عدم, 'L'},
  {"physical", no_دلیل, عدم, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [OPTION]... FILE...\n"), پروگرام_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the آخری component must exist (افتراضي)\
\n\
"));
      oputs (_("\
  -e, --canonicalize-existing  all components دا the رستہ must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing   no رستہ components need exist or be a directory\
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
      --relative-منزل=DIR        print the resolved رستہ relative منزل DIR\n\
"));
      oputs (_("\
      --relative-ادھار_2=DIR      print absolute paths unless paths ہیٹھاں DIR\n\
"));
      oputs (_("\
  -s, --strip, --no-symlinks   don't expand symlinks\n\
"));
      oputs (_("\
  -z, --zero                   سرا each output سطر with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
ساكن حرف *
realpath_canon (حرف ثابت *fname, صحيح can_منوال)
{
  حرف *can_fname = canonicalize_filename_منوال (fname, can_منوال);
  إذا (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_منوال &= ~CAN_NOLINKS;
      حرف *can_fname2 = canonicalize_filename_منوال (can_fname, can_منوال);
      حرر (can_fname);
      أرجع can_fname2;
    }
  أرجع can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_خالص
ساكن bool
رستہ_اگیتر (حرف ثابت *اگیتر, حرف ثابت *رستہ)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  اگیتر++;
  رستہ++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  إذا (!*اگیتر)
    أرجع *رستہ != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  إذا (*اگیتر == '/' && !اگیتر[1])
    أرجع *رستہ == '/';

  /* Any other prefix has a non-slash portion.  */
  طالما (*اگیتر && *رستہ)
    {
      إذا (*اگیتر != *رستہ)
        اكسر;
      اگیتر++;
      رستہ++;
    }
  أرجع (!*اگیتر && (*رستہ == '/' || !*رستہ));
}

ساكن bool
isdir (حرف ثابت *رستہ)
{
  بنية stat sb;
  إذا (stat (رستہ, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (رستہ));
  أرجع S_ISDIR (sb.st_منوال);
}

ساكن فراغ
print_رستہ (حرف ثابت *رستہ)
{
  fputs (quote_output ? quoteN (رستہ) : رستہ, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
ساكن bool
print_relative_رستہ (حرف ثابت *can_fname)
{
  إذا (!quote_output)
    أرجع relpath (can_fname, can_relative_منزل, عدم, 0);

  size_t ناپ;
  إذا (ckd_mul (&ناپ, لڑی_لمبائی (can_relative_منزل), 2)
      || ckd_جوڑو (&ناپ, ناپ, لڑی_لمبائی (can_fname))
      || ckd_جوڑو (&ناپ, ناپ, 1))
    xalloc_die ();

  حرف *relative_رستہ = xmalloc (ناپ);
  bool success = relpath (can_fname, can_relative_منزل, relative_رستہ, ناپ);
  إذا (success)
    print_رستہ (relative_رستہ);
  حرر (relative_رستہ);
  أرجع success;
}

ساكن bool
process_رستہ (حرف ثابت *fname, صحيح can_منوال)
{
  حرف *can_fname = realpath_canon (fname, can_منوال);
  إذا (!can_fname)
    {
      إذا (verbose)
        error (0, errno, "%s", quotef (fname));
      أرجع false;
    }

  إذا (!can_relative_منزل
      || (can_relative_ادھار && !رستہ_اگیتر (can_relative_ادھار, can_fname))
      || (can_relative_منزل && !print_relative_رستہ (can_fname)))
    print_رستہ (can_fname);

  حرف_لکھو (use_nuls ? '\0' : '\n');

  حرر (can_fname);

  إذا (ferror (stdout))
    write_error ();

  أرجع true;
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  bool ok = true;
  صحيح can_منوال = CAN_ALL_BUT_آخری;
  حرف ثابت *relative_منزل = عدم;
  حرف ثابت *relative_ادھار = عدم;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
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
          can_منوال &= ~CAN_منوال_نقاب;
          can_منوال |= CAN_ALL_BUT_آخری;
          اكسر;
        حالة 'e':
          can_منوال &= ~CAN_منوال_نقاب;
          can_منوال |= CAN_EXISTING;
          اكسر;
        حالة 'm':
          can_منوال &= ~CAN_منوال_نقاب;
          can_منوال |= CAN_MISSING;
          اكسر;
        حالة 'L':
          can_منوال |= CAN_NOLINKS;
          logical = true;
          اكسر;
        حالة 's':
          can_منوال |= CAN_NOLINKS;
          logical = false;
          اكسر;
        حالة 'P':
          can_منوال &= ~CAN_NOLINKS;
          logical = false;
          اكسر;
        حالة 'q':
          verbose = false;
          اكسر;
        حالة 'z':
          use_nuls = true;
          اكسر;
        حالة RELATIVE_منزل_OPTION:
          relative_منزل = optarg;
          اكسر;
        حالة RELATIVE_ادھار_OPTION:
          relative_ادھار = optarg;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
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
          سیٹ_quoting_style (عدم, qs);
          quote_output = true;
        }
    }

  إذا (relative_ادھار && !relative_منزل)
    relative_منزل = relative_ادھار;

  bool need_dir = (can_منوال & CAN_منوال_نقاب) == CAN_EXISTING;
  إذا (relative_منزل)
    {
      can_relative_منزل = realpath_canon (relative_منزل, can_منوال);
      إذا (!can_relative_منزل)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_منزل));
      إذا (need_dir && !isdir (can_relative_منزل))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_منزل));
    }
  إذا (relative_ادھار == relative_منزل)
    can_relative_ادھار = can_relative_منزل;
  وإلا إذا (relative_ادھار)
    {
      حرف *ادھار_2 = realpath_canon (relative_ادھار, can_منوال);
      إذا (!ادھار_2)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_ادھار));
      إذا (need_dir && !isdir (ادھار_2))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_ادھار));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      إذا (رستہ_اگیتر (ادھار_2, can_relative_منزل))
        can_relative_ادھار = ادھار_2;
      وإلا
        {
          حرر (ادھار_2);
          can_relative_ادھار = can_relative_منزل;
          can_relative_منزل = عدم;
        }
    }

  لكل (; optind < argc; ++optind)
    ok &= process_رستہ (argv[optind], can_منوال);

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
