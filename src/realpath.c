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
#define PROGRAM_NAME "realpath"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

تعداد
{
  RELATIVE_TO_OPTION = CHAR_MAX + 1,
  RELATIVE_BASE_OPTION
};

ساكن bool verbose = true;
ساكن bool logical;
ساكن bool use_nuls;
ساكن bool quote_output;
ساكن حرف ثابت *can_relative_to;
ساكن حرف ثابت *can_relative_base;

ساكن بنية option ثابت longopts[] =
{
  {"canonicalize", no_argument, عدم, 'E'},
  {"canonicalize-existing", no_argument, عدم, 'e'},
  {"canonicalize-missing", no_argument, عدم, 'm'},
  {"relative-to", required_argument, عدم, RELATIVE_TO_OPTION},
  {"relative-base", required_argument, عدم, RELATIVE_BASE_OPTION},
  {"quiet", no_argument, عدم, 'q'},
  {"strip", no_argument, عدم, 's'},
  {"no-symlinks", no_argument, عدم, 's'},
  {"zero", no_argument, عدم, 'z'},
  {"logical", no_argument, عدم, 'L'},
  {"physical", no_argument, عدم, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the last component must exist (افتراضي)\
\n\
"));
      oputs (_("\
  -e, --canonicalize-existing  all components of the path must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing   no path components need exist or be a directory\
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
      --relative-to=DIR        print the resolved path relative to DIR\n\
"));
      oputs (_("\
      --relative-base=DIR      print absolute paths unless paths below DIR\n\
"));
      oputs (_("\
  -s, --strip, --no-symlinks   don't expand symlinks\n\
"));
      oputs (_("\
  -z, --zero                   end each output سطر with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
ساكن حرف *
realpath_canon (حرف ثابت *fname, صحيح can_mode)
{
  حرف *can_fname = canonicalize_filename_mode (fname, can_mode);
  إذا (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      حرف *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      حرر (can_fname);
      إرجاع can_fname2;
    }
  إرجاع can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
ساكن bool
path_prefix (حرف ثابت *prefix, حرف ثابت *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  إذا (!*prefix)
    إرجاع *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  إذا (*prefix == '/' && !prefix[1])
    إرجاع *path == '/';

  /* Any other prefix has a non-slash portion.  */
  بينما (*prefix && *path)
    {
      إذا (*prefix != *path)
        اكسر;
      prefix++;
      path++;
    }
  إرجاع (!*prefix && (*path == '/' || !*path));
}

ساكن bool
isdir (حرف ثابت *path)
{
  بنية stat sb;
  إذا (stat (path, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (path));
  إرجاع S_ISDIR (sb.st_mode);
}

ساكن فراغ
print_path (حرف ثابت *path)
{
  fputs (quote_output ? quoteN (path) : path, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
ساكن bool
print_relative_path (حرف ثابت *can_fname)
{
  إذا (!quote_output)
    إرجاع relpath (can_fname, can_relative_to, عدم, 0);

  size_t size;
  إذا (ckd_mul (&size, strlen (can_relative_to), 2)
      || ckd_add (&size, size, strlen (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  حرف *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  إذا (success)
    print_path (relative_path);
  حرر (relative_path);
  إرجاع success;
}

ساكن bool
process_path (حرف ثابت *fname, صحيح can_mode)
{
  حرف *can_fname = realpath_canon (fname, can_mode);
  إذا (!can_fname)
    {
      إذا (verbose)
        error (0, errno, "%s", quotef (fname));
      إرجاع false;
    }

  إذا (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  putchar (use_nuls ? '\0' : '\n');

  حرر (can_fname);

  إذا (ferror (stdout))
    write_error ();

  إرجاع true;
}

صحيح
الرئيسية (صحيح argc, حرف **argv)
{
  bool ok = true;
  صحيح can_mode = CAN_ALL_BUT_LAST;
  حرف ثابت *relative_to = عدم;
  حرف ثابت *relative_base = عدم;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  بينما (true)
    {
      صحيح c = getopt_long (argc, argv, "EeLmPqsz", longopts, عدم);
      إذا (c == -1)
        اكسر;
      بدل (c)
        {
        حالة 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          اكسر;
        حالة 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          اكسر;
        حالة 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          اكسر;
        حالة 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          اكسر;
        حالة 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          اكسر;
        حالة 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          اكسر;
        حالة 'q':
          verbose = false;
          اكسر;
        حالة 'z':
          use_nuls = true;
          اكسر;
        حالة RELATIVE_TO_OPTION:
          relative_to = optarg;
          اكسر;
        حالة RELATIVE_BASE_OPTION:
          relative_base = optarg;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
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
          set_quoting_style (عدم, qs);
          quote_output = true;
        }
    }

  إذا (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  إذا (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      إذا (!can_relative_to)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_to));
      إذا (need_dir && !isdir (can_relative_to))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_to));
    }
  إذا (relative_base == relative_to)
    can_relative_base = can_relative_to;
  وإلا إذا (relative_base)
    {
      حرف *base = realpath_canon (relative_base, can_mode);
      إذا (!base)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_base));
      إذا (need_dir && !isdir (base))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      إذا (path_prefix (base, can_relative_to))
        can_relative_base = base;
      وإلا
        {
          حرر (base);
          can_relative_base = can_relative_to;
          can_relative_to = عدم;
        }
    }

  لكل (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  إرجاع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
