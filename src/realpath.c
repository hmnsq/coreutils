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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#অন্তর্ভাব "system.h"
#অন্তর্ভাব "canonicalize.h"
#অন্তর্ভাব "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "realpath"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

গণনা
{
  RELATIVE_TO_OPTION = CHAR_MAX + 1,
  RELATIVE_BASE_OPTION
};

স্থির bool verbose = true;
স্থির bool logical;
স্থির bool use_nuls;
স্থির bool quote_output;
স্থির অক্ষর ধ্রুবক *can_relative_to;
স্থির অক্ষর ধ্রুবক *can_relative_base;

স্থির কাঠামো option ধ্রুবক longopts[] =
{
  {"canonicalize", no_argument, ফাঁকা, 'E'},
  {"canonicalize-existing", no_argument, ফাঁকা, 'e'},
  {"canonicalize-missing", no_argument, ফাঁকা, 'm'},
  {"relative-to", required_argument, ফাঁকা, RELATIVE_TO_OPTION},
  {"relative-base", required_argument, ফাঁকা, RELATIVE_BASE_OPTION},
  {"quiet", no_argument, ফাঁকা, 'q'},
  {"strip", no_argument, ফাঁকা, 's'},
  {"no-symlinks", no_argument, ফাঁকা, 's'},
  {"zero", no_argument, ফাঁকা, 'z'},
  {"logical", no_argument, ফাঁকা, 'L'},
  {"physical", no_argument, ফাঁকা, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the last component must exist (পূর্বনির্ধারিত)\
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
  -P, --physical               resolve symlinks as encountered (পূর্বনির্ধারিত)\n\
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
  -z, --zero                   end each output সারি with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
স্থির অক্ষর *
realpath_canon (অক্ষর ধ্রুবক *fname, পূর্ণসংখ্যা can_mode)
{
  অক্ষর *can_fname = canonicalize_filename_mode (fname, can_mode);
  যদি (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      অক্ষর *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      মুক্ত (can_fname);
      ফেরত can_fname2;
    }
  ফেরত can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
স্থির bool
path_prefix (অক্ষর ধ্রুবক *prefix, অক্ষর ধ্রুবক *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  যদি (!*prefix)
    ফেরত *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  যদি (*prefix == '/' && !prefix[1])
    ফেরত *path == '/';

  /* Any other prefix has a non-slash portion.  */
  যখন (*prefix && *path)
    {
      যদি (*prefix != *path)
        বিরতি;
      prefix++;
      path++;
    }
  ফেরত (!*prefix && (*path == '/' || !*path));
}

স্থির bool
isdir (অক্ষর ধ্রুবক *path)
{
  কাঠামো stat sb;
  যদি (stat (path, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (path));
  ফেরত S_ISDIR (sb.st_mode);
}

স্থির শূন্য
print_path (অক্ষর ধ্রুবক *path)
{
  fputs (quote_output ? quoteN (path) : path, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
স্থির bool
print_relative_path (অক্ষর ধ্রুবক *can_fname)
{
  যদি (!quote_output)
    ফেরত relpath (can_fname, can_relative_to, ফাঁকা, 0);

  size_t আকার_2;
  যদি (ckd_mul (&আকার_2, strlen (can_relative_to), 2)
      || ckd_add (&আকার_2, আকার_2, strlen (can_fname))
      || ckd_add (&আকার_2, আকার_2, 1))
    xalloc_die ();

  অক্ষর *relative_path = xmalloc (আকার_2);
  bool success = relpath (can_fname, can_relative_to, relative_path, আকার_2);
  যদি (success)
    print_path (relative_path);
  মুক্ত (relative_path);
  ফেরত success;
}

স্থির bool
process_path (অক্ষর ধ্রুবক *fname, পূর্ণসংখ্যা can_mode)
{
  অক্ষর *can_fname = realpath_canon (fname, can_mode);
  যদি (!can_fname)
    {
      যদি (verbose)
        error (0, errno, "%s", quotef (fname));
      ফেরত false;
    }

  যদি (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  putchar (use_nuls ? '\0' : '\n');

  মুক্ত (can_fname);

  যদি (ferror (stdout))
    write_error ();

  ফেরত true;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool ok = true;
  পূর্ণসংখ্যা can_mode = CAN_ALL_BUT_LAST;
  অক্ষর ধ্রুবক *relative_to = ফাঁকা;
  অক্ষর ধ্রুবক *relative_base = ফাঁকা;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন (true)
    {
      পূর্ণসংখ্যা c = getopt_long (argc, argv, "EeLmPqsz", longopts, ফাঁকা);
      যদি (c == -1)
        বিরতি;
      নির্বাচন (c)
        {
        ক্ষেত্র 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          বিরতি;
        ক্ষেত্র 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          বিরতি;
        ক্ষেত্র 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          বিরতি;
        ক্ষেত্র 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          বিরতি;
        ক্ষেত্র 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          বিরতি;
        ক্ষেত্র 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          বিরতি;
        ক্ষেত্র 'q':
          verbose = false;
          বিরতি;
        ক্ষেত্র 'z':
          use_nuls = true;
          বিরতি;
        ক্ষেত্র RELATIVE_TO_OPTION:
          relative_to = optarg;
          বিরতি;
        ক্ষেত্র RELATIVE_BASE_OPTION:
          relative_base = optarg;
          বিরতি;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  যদি (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  যদি (! use_nuls && isatty (STDOUT_FILENO))
    {
      পূর্ণসংখ্যা qs = getenv_quoting_style ();
      যদি (qs < 0)
        qs = shell_escape_quoting_style;
      যদি (qs != literal_quoting_style)
        {
          set_quoting_style (ফাঁকা, qs);
          quote_output = true;
        }
    }

  যদি (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  যদি (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      যদি (!can_relative_to)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_to));
      যদি (need_dir && !isdir (can_relative_to))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_to));
    }
  যদি (relative_base == relative_to)
    can_relative_base = can_relative_to;
  অন্যথা যদি (relative_base)
    {
      অক্ষর *base = realpath_canon (relative_base, can_mode);
      যদি (!base)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_base));
      যদি (need_dir && !isdir (base))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      যদি (path_prefix (base, can_relative_to))
        can_relative_base = base;
      অন্যথা
        {
          মুক্ত (base);
          can_relative_base = can_relative_to;
          can_relative_to = ফাঁকা;
        }
    }

  জন্য (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
