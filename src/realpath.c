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
#تعریف_کړه پروګرام_NAME "realpath"

#تعریف_کړه AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

تعداد
{
  RELATIVE_منزل_OPTION = CHAR_MAX + 1,
  RELATIVE_بنسټ_OPTION
};

ساكن bool verbose = true;
ساكن bool logical;
ساكن bool use_nuls;
ساكن bool quote_output;
ساكن حرف ثابت *can_relative_منزل;
ساكن حرف ثابت *can_relative_بنسټ;

ساكن بنية option ثابت longopts[] =
{
  {"canonicalize", no_دلیل, عدم, 'E'},
  {"canonicalize-existing", no_دلیل, عدم, 'e'},
  {"canonicalize-missing", no_دلیل, عدم, 'm'},
  {"relative-to", required_دلیل, عدم, RELATIVE_منزل_OPTION},
  {"relative-base", required_دلیل, عدم, RELATIVE_بنسټ_OPTION},
  {"quiet", no_دلیل, عدم, 'q'},
  {"strip", no_دلیل, عدم, 's'},
  {"no-symlinks", no_دلیل, عدم, 's'},
  {"zero", no_دلیل, عدم, 'z'},
  {"logical", no_دلیل, عدم, 'L'},
  {"physical", no_دلیل, عدم, 'P'},
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
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the وروستی component must exist (افتراضي)\
\n\
"));
      oputs (_("\
  -e, --canonicalize-existing  all components د the لار must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing   no لار components need exist or be a directory\
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
      --relative-منزل=DIR        print the resolved لار relative منزل DIR\n\
"));
      oputs (_("\
      --relative-بنسټ_2=DIR      print absolute paths unless paths لاندې DIR\n\
"));
      oputs (_("\
  -s, --strip, --no-symlinks   don't expand symlinks\n\
"));
      oputs (_("\
  -z, --zero                   پای_ټکی each output کرښه with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
ساكن حرف *
realpath_canon (حرف ثابت *fname, صحيح can_ډېرپېښ)
{
  حرف *can_fname = canonicalize_filename_ډېرپېښ (fname, can_ډېرپېښ);
  إذا (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_ډېرپېښ &= ~CAN_NOLINKS;
      حرف *can_fname2 = canonicalize_filename_ډېرپېښ (can_fname, can_ډېرپېښ);
      حرر (can_fname);
      أرجع can_fname2;
    }
  أرجع can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_خالص
ساكن bool
لار_مختاړی (حرف ثابت *مختاړی, حرف ثابت *لار)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  مختاړی++;
  لار++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  إذا (!*مختاړی)
    أرجع *لار != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  إذا (*مختاړی == '/' && !مختاړی[1])
    أرجع *لار == '/';

  /* Any other prefix has a non-slash portion.  */
  طالما (*مختاړی && *لار)
    {
      إذا (*مختاړی != *لار)
        اكسر;
      مختاړی++;
      لار++;
    }
  أرجع (!*مختاړی && (*لار == '/' || !*لار));
}

ساكن bool
isdir (حرف ثابت *لار)
{
  بنية stat sb;
  إذا (stat (لار, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (لار));
  أرجع S_ISDIR (sb.st_ډېرپېښ);
}

ساكن فراغ
print_لار (حرف ثابت *لار)
{
  fputs (quote_output ? quoteN (لار) : لار, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
ساكن bool
print_relative_لار (حرف ثابت *can_fname)
{
  إذا (!quote_output)
    أرجع relpath (can_fname, can_relative_منزل, عدم, 0);

  size_t اندازه;
  إذا (ckd_mul (&اندازه, مزي_اوږدوالی (can_relative_منزل), 2)
      || ckd_جمع_کړه (&اندازه, اندازه, مزي_اوږدوالی (can_fname))
      || ckd_جمع_کړه (&اندازه, اندازه, 1))
    xalloc_die ();

  حرف *relative_لار = xmalloc (اندازه);
  bool success = relpath (can_fname, can_relative_منزل, relative_لار, اندازه);
  إذا (success)
    print_لار (relative_لار);
  حرر (relative_لار);
  أرجع success;
}

ساكن bool
process_لار (حرف ثابت *fname, صحيح can_ډېرپېښ)
{
  حرف *can_fname = realpath_canon (fname, can_ډېرپېښ);
  إذا (!can_fname)
    {
      إذا (verbose)
        error (0, errno, "%s", quotef (fname));
      أرجع false;
    }

  إذا (!can_relative_منزل
      || (can_relative_بنسټ && !لار_مختاړی (can_relative_بنسټ, can_fname))
      || (can_relative_منزل && !print_relative_لار (can_fname)))
    print_لار (can_fname);

  توری_چاپ_کړه (use_nuls ? '\0' : '\n');

  حرر (can_fname);

  إذا (ferror (stdout))
    write_error ();

  أرجع true;
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  bool ok = true;
  صحيح can_ډېرپېښ = CAN_ALL_BUT_وروستی;
  حرف ثابت *relative_منزل = عدم;
  حرف ثابت *relative_بنسټ = عدم;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
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
          can_ډېرپېښ &= ~CAN_ډېرپېښ_ماسک;
          can_ډېرپېښ |= CAN_ALL_BUT_وروستی;
          اكسر;
        حالة 'e':
          can_ډېرپېښ &= ~CAN_ډېرپېښ_ماسک;
          can_ډېرپېښ |= CAN_EXISTING;
          اكسر;
        حالة 'm':
          can_ډېرپېښ &= ~CAN_ډېرپېښ_ماسک;
          can_ډېرپېښ |= CAN_MISSING;
          اكسر;
        حالة 'L':
          can_ډېرپېښ |= CAN_NOLINKS;
          logical = true;
          اكسر;
        حالة 's':
          can_ډېرپېښ |= CAN_NOLINKS;
          logical = false;
          اكسر;
        حالة 'P':
          can_ډېرپېښ &= ~CAN_NOLINKS;
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
        حالة RELATIVE_بنسټ_OPTION:
          relative_بنسټ = optarg;
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

  إذا (! use_nuls && isatty (STDOUT_FILENO))
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

  إذا (relative_بنسټ && !relative_منزل)
    relative_منزل = relative_بنسټ;

  bool need_dir = (can_ډېرپېښ & CAN_ډېرپېښ_ماسک) == CAN_EXISTING;
  إذا (relative_منزل)
    {
      can_relative_منزل = realpath_canon (relative_منزل, can_ډېرپېښ);
      إذا (!can_relative_منزل)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_منزل));
      إذا (need_dir && !isdir (can_relative_منزل))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_منزل));
    }
  إذا (relative_بنسټ == relative_منزل)
    can_relative_بنسټ = can_relative_منزل;
  وإلا إذا (relative_بنسټ)
    {
      حرف *بنسټ_2 = realpath_canon (relative_بنسټ, can_ډېرپېښ);
      إذا (!بنسټ_2)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_بنسټ));
      إذا (need_dir && !isdir (بنسټ_2))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_بنسټ));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      إذا (لار_مختاړی (بنسټ_2, can_relative_منزل))
        can_relative_بنسټ = بنسټ_2;
      وإلا
        {
          حرر (بنسټ_2);
          can_relative_بنسټ = can_relative_منزل;
          can_relative_منزل = عدم;
        }
    }

  لكل (; optind < argc; ++optind)
    ok &= process_لار (argv[optind], can_ډېرپېښ);

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
