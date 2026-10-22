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
#وصف_ڪريو پروگرام_NAME "realpath"

#وصف_ڪريو AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

تعداد
{
  RELATIVE_منزل_OPTION = CHAR_MAX + 1,
  RELATIVE_بنياد_OPTION
};

ساكن bool verbose = true;
ساكن bool logical;
ساكن bool use_nuls;
ساكن bool quote_output;
ساكن حرف ثابت *can_relative_منزل;
ساكن حرف ثابت *can_relative_بنياد;

ساكن بنية option ثابت longopts[] =
{
  {"canonicalize", no_دليل, عدم, 'E'},
  {"canonicalize-existing", no_دليل, عدم, 'e'},
  {"canonicalize-missing", no_دليل, عدم, 'm'},
  {"relative-to", required_دليل, عدم, RELATIVE_منزل_OPTION},
  {"relative-base", required_دليل, عدم, RELATIVE_بنياد_OPTION},
  {"quiet", no_دليل, عدم, 'q'},
  {"strip", no_دليل, عدم, 's'},
  {"no-symlinks", no_دليل, عدم, 's'},
  {"zero", no_دليل, عدم, 'z'},
  {"logical", no_دليل, عدم, 'L'},
  {"physical", no_دليل, عدم, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("Usage: %s [OPTION]... FILE...\n"), پروگرام_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the آخري component must exist (افتراضي)\
\n\
"));
      oputs (_("\
  -e, --canonicalize-existing  all components جو the رستو must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing   no رستو components need exist or be a directory\
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
      --relative-منزل=DIR        print the resolved رستو relative منزل DIR\n\
"));
      oputs (_("\
      --relative-بنياد_2=DIR      print absolute paths unless paths هيٺ DIR\n\
"));
      oputs (_("\
  -s, --strip, --no-symlinks   don't expand symlinks\n\
"));
      oputs (_("\
  -z, --zero                   پڇاڙي each output سٽ with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
ساكن حرف *
realpath_canon (حرف ثابت *fname, صحيح can_گهڻائي)
{
  حرف *can_fname = canonicalize_filename_گهڻائي (fname, can_گهڻائي);
  إذا (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_گهڻائي &= ~CAN_NOLINKS;
      حرف *can_fname2 = canonicalize_filename_گهڻائي (can_fname, can_گهڻائي);
      حرر (can_fname);
      أرجع can_fname2;
    }
  أرجع can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_خالص
ساكن bool
رستو_اڳياڙي (حرف ثابت *اڳياڙي, حرف ثابت *رستو)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  اڳياڙي++;
  رستو++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  إذا (!*اڳياڙي)
    أرجع *رستو != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  إذا (*اڳياڙي == '/' && !اڳياڙي[1])
    أرجع *رستو == '/';

  /* Any other prefix has a non-slash portion.  */
  طالما (*اڳياڙي && *رستو)
    {
      إذا (*اڳياڙي != *رستو)
        اكسر;
      اڳياڙي++;
      رستو++;
    }
  أرجع (!*اڳياڙي && (*رستو == '/' || !*رستو));
}

ساكن bool
isdir (حرف ثابت *رستو)
{
  بنية stat sb;
  إذا (stat (رستو, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (رستو));
  أرجع S_ISDIR (sb.st_گهڻائي);
}

ساكن فراغ
print_رستو (حرف ثابت *رستو)
{
  fputs (quote_output ? quoteN (رستو) : رستو, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
ساكن bool
print_relative_رستو (حرف ثابت *can_fname)
{
  إذا (!quote_output)
    أرجع relpath (can_fname, can_relative_منزل, عدم, 0);

  size_t سائيز;
  إذا (ckd_mul (&سائيز, لڙي_ڊيگهه (can_relative_منزل), 2)
      || ckd_جوڙيو (&سائيز, سائيز, لڙي_ڊيگهه (can_fname))
      || ckd_جوڙيو (&سائيز, سائيز, 1))
    xalloc_die ();

  حرف *relative_رستو = xmalloc (سائيز);
  bool success = relpath (can_fname, can_relative_منزل, relative_رستو, سائيز);
  إذا (success)
    print_رستو (relative_رستو);
  حرر (relative_رستو);
  أرجع success;
}

ساكن bool
process_رستو (حرف ثابت *fname, صحيح can_گهڻائي)
{
  حرف *can_fname = realpath_canon (fname, can_گهڻائي);
  إذا (!can_fname)
    {
      إذا (verbose)
        error (0, errno, "%s", quotef (fname));
      أرجع false;
    }

  إذا (!can_relative_منزل
      || (can_relative_بنياد && !رستو_اڳياڙي (can_relative_بنياد, can_fname))
      || (can_relative_منزل && !print_relative_رستو (can_fname)))
    print_رستو (can_fname);

  اکر_لکو (use_nuls ? '\0' : '\n');

  حرر (can_fname);

  إذا (ferror (stdout))
    write_error ();

  أرجع true;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  bool ok = true;
  صحيح can_گهڻائي = CAN_ALL_BUT_آخري;
  حرف ثابت *relative_منزل = عدم;
  حرف ثابت *relative_بنياد = عدم;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
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
          can_گهڻائي &= ~CAN_گهڻائي_نقاب;
          can_گهڻائي |= CAN_ALL_BUT_آخري;
          اكسر;
        حالة 'e':
          can_گهڻائي &= ~CAN_گهڻائي_نقاب;
          can_گهڻائي |= CAN_EXISTING;
          اكسر;
        حالة 'm':
          can_گهڻائي &= ~CAN_گهڻائي_نقاب;
          can_گهڻائي |= CAN_MISSING;
          اكسر;
        حالة 'L':
          can_گهڻائي |= CAN_NOLINKS;
          logical = true;
          اكسر;
        حالة 's':
          can_گهڻائي |= CAN_NOLINKS;
          logical = false;
          اكسر;
        حالة 'P':
          can_گهڻائي &= ~CAN_NOLINKS;
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
        حالة RELATIVE_بنياد_OPTION:
          relative_بنياد = optarg;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);
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
          سيٽ_quoting_style (عدم, qs);
          quote_output = true;
        }
    }

  إذا (relative_بنياد && !relative_منزل)
    relative_منزل = relative_بنياد;

  bool need_dir = (can_گهڻائي & CAN_گهڻائي_نقاب) == CAN_EXISTING;
  إذا (relative_منزل)
    {
      can_relative_منزل = realpath_canon (relative_منزل, can_گهڻائي);
      إذا (!can_relative_منزل)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_منزل));
      إذا (need_dir && !isdir (can_relative_منزل))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_منزل));
    }
  إذا (relative_بنياد == relative_منزل)
    can_relative_بنياد = can_relative_منزل;
  وإلا إذا (relative_بنياد)
    {
      حرف *بنياد_2 = realpath_canon (relative_بنياد, can_گهڻائي);
      إذا (!بنياد_2)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_بنياد));
      إذا (need_dir && !isdir (بنياد_2))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_بنياد));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      إذا (رستو_اڳياڙي (بنياد_2, can_relative_منزل))
        can_relative_بنياد = بنياد_2;
      وإلا
        {
          حرر (بنياد_2);
          can_relative_بنياد = can_relative_منزل;
          can_relative_منزل = عدم;
        }
    }

  لكل (; optind < argc; ++optind)
    ok &= process_رستو (argv[optind], can_گهڻائي);

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
