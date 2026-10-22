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
#define PROGRAM_NAME "realpath"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

شمارش
{
  RELATIVE_TO_OPTION = CHAR_MAX + 1,
  RELATIVE_BASE_OPTION
};

ایستا bool verbose = true;
ایستا bool logical;
ایستا bool use_nuls;
ایستا bool quote_output;
ایستا نویسه ثابت *can_relative_to;
ایستا نویسه ثابت *can_relative_base;

ایستا ساختار option ثابت longopts[] =
{
  {"canonicalize", no_argument, پوچ, 'E'},
  {"canonicalize-existing", no_argument, پوچ, 'e'},
  {"canonicalize-missing", no_argument, پوچ, 'm'},
  {"relative-to", required_argument, پوچ, RELATIVE_TO_OPTION},
  {"relative-base", required_argument, پوچ, RELATIVE_BASE_OPTION},
  {"quiet", no_argument, پوچ, 'q'},
  {"strip", no_argument, پوچ, 's'},
  {"no-symlinks", no_argument, پوچ, 's'},
  {"zero", no_argument, پوچ, 'z'},
  {"logical", no_argument, پوچ, 'L'},
  {"physical", no_argument, پوچ, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the last component must exist (پیشفرض)\
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
  -P, --physical               resolve symlinks as encountered (پیشفرض)\n\
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
ایستا نویسه *
realpath_canon (نویسه ثابت *fname, صحیح can_mode)
{
  نویسه *can_fname = canonicalize_filename_mode (fname, can_mode);
  اگر (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      نویسه *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      آزاد (can_fname);
      بازگشت can_fname2;
    }
  بازگشت can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
ایستا bool
path_prefix (نویسه ثابت *prefix, نویسه ثابت *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  اگر (!*prefix)
    بازگشت *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  اگر (*prefix == '/' && !prefix[1])
    بازگشت *path == '/';

  /* Any other prefix has a non-slash portion.  */
  وقتی (*prefix && *path)
    {
      اگر (*prefix != *path)
        بشکن;
      prefix++;
      path++;
    }
  بازگشت (!*prefix && (*path == '/' || !*path));
}

ایستا bool
isdir (نویسه ثابت *path)
{
  ساختار stat sb;
  اگر (stat (path, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (path));
  بازگشت S_ISDIR (sb.st_mode);
}

ایستا تهی
print_path (نویسه ثابت *path)
{
  fputs (quote_output ? quoteN (path) : path, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
ایستا bool
print_relative_path (نویسه ثابت *can_fname)
{
  اگر (!quote_output)
    بازگشت relpath (can_fname, can_relative_to, پوچ, 0);

  size_t size;
  اگر (ckd_mul (&size, strlen (can_relative_to), 2)
      || ckd_add (&size, size, strlen (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  نویسه *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  اگر (success)
    print_path (relative_path);
  آزاد (relative_path);
  بازگشت success;
}

ایستا bool
process_path (نویسه ثابت *fname, صحیح can_mode)
{
  نویسه *can_fname = realpath_canon (fname, can_mode);
  اگر (!can_fname)
    {
      اگر (verbose)
        error (0, errno, "%s", quotef (fname));
      بازگشت false;
    }

  اگر (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  putchar (use_nuls ? '\0' : '\n');

  آزاد (can_fname);

  اگر (ferror (stdout))
    write_error ();

  بازگشت true;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  bool ok = true;
  صحیح can_mode = CAN_ALL_BUT_LAST;
  نویسه ثابت *relative_to = پوچ;
  نویسه ثابت *relative_base = پوچ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  وقتی (true)
    {
      صحیح c = getopt_long (argc, argv, "EeLmPqsz", longopts, پوچ);
      اگر (c == -1)
        بشکن;
      انتخاب (c)
        {
        حالت 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          بشکن;
        حالت 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          بشکن;
        حالت 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          بشکن;
        حالت 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          بشکن;
        حالت 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          بشکن;
        حالت 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          بشکن;
        حالت 'q':
          verbose = false;
          بشکن;
        حالت 'z':
          use_nuls = true;
          بشکن;
        حالت RELATIVE_TO_OPTION:
          relative_to = optarg;
          بشکن;
        حالت RELATIVE_BASE_OPTION:
          relative_base = optarg;
          بشکن;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
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
          set_quoting_style (پوچ, qs);
          quote_output = true;
        }
    }

  اگر (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  اگر (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      اگر (!can_relative_to)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_to));
      اگر (need_dir && !isdir (can_relative_to))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_to));
    }
  اگر (relative_base == relative_to)
    can_relative_base = can_relative_to;
  وگرنه اگر (relative_base)
    {
      نویسه *base = realpath_canon (relative_base, can_mode);
      اگر (!base)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_base));
      اگر (need_dir && !isdir (base))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      اگر (path_prefix (base, can_relative_to))
        can_relative_base = base;
      وگرنه
        {
          آزاد (base);
          can_relative_base = can_relative_to;
          can_relative_to = پوچ;
        }
    }

  برای (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
