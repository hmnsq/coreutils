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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <sys/types.h>

#অন্তর্ভাৱ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "canonicalize.h"
#অন্তর্ভাৱ "relpath.h"

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
স্থির ৱর্ণ ধ্রুৱ *can_relative_to;
স্থির ৱর্ণ ধ্রুৱ *can_relative_base;

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {"canonicalize", no_argument, রিক্ত, 'E'},
  {"canonicalize-existing", no_argument, রিক্ত, 'e'},
  {"canonicalize-missing", no_argument, রিক্ত, 'm'},
  {"relative-to", required_argument, রিক্ত, RELATIVE_TO_OPTION},
  {"relative-base", required_argument, রিক্ত, RELATIVE_BASE_OPTION},
  {"quiet", no_argument, রিক্ত, 'q'},
  {"strip", no_argument, রিক্ত, 's'},
  {"no-symlinks", no_argument, রিক্ত, 's'},
  {"zero", no_argument, রিক্ত, 'z'},
  {"logical", no_argument, রিক্ত, 'L'},
  {"physical", no_argument, রিক্ত, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
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
  -E, --canonicalize           all but the last component must exist (স্ৱভাৱ)\
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
  -P, --physical               resolve symlinks as encountered (স্ৱভাৱ)\n\
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
  -z, --zero                   end each output সাৰি with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
স্থির ৱর্ণ *
realpath_canon (ৱর্ণ ধ্রুৱ *fname, পূর্ণাঙ্ক can_mode)
{
  ৱর্ণ *can_fname = canonicalize_filename_mode (fname, can_mode);
  যদি (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      ৱর্ণ *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      মুক্ত (can_fname);
      ঘূৰাই can_fname2;
    }
  ঘূৰাই can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
স্থির bool
path_prefix (ৱর্ণ ধ্রুৱ *prefix, ৱর্ণ ধ্রুৱ *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  যদি (!*prefix)
    ঘূৰাই *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  যদি (*prefix == '/' && !prefix[1])
    ঘূৰাই *path == '/';

  /* Any other prefix has a non-slash portion.  */
  যেতিয়ালৈ (*prefix && *path)
    {
      যদি (*prefix != *path)
        ভঙ্গ;
      prefix++;
      path++;
    }
  ঘূৰাই (!*prefix && (*path == '/' || !*path));
}

স্থির bool
isdir (ৱর্ণ ধ্রুৱ *path)
{
  সংরচনা stat sb;
  যদি (stat (path, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (path));
  ঘূৰাই S_ISDIR (sb.st_mode);
}

স্থির শূন্য
print_path (ৱর্ণ ধ্রুৱ *path)
{
  fputs (quote_output ? quoteN (path) : path, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
স্থির bool
print_relative_path (ৱর্ণ ধ্রুৱ *can_fname)
{
  যদি (!quote_output)
    ঘূৰাই relpath (can_fname, can_relative_to, রিক্ত, 0);

  size_t size;
  যদি (ckd_mul (&size, strlen (can_relative_to), 2)
      || ckd_add (&size, size, strlen (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  ৱর্ণ *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  যদি (success)
    print_path (relative_path);
  মুক্ত (relative_path);
  ঘূৰাই success;
}

স্থির bool
process_path (ৱর্ণ ধ্রুৱ *fname, পূর্ণাঙ্ক can_mode)
{
  ৱর্ণ *can_fname = realpath_canon (fname, can_mode);
  যদি (!can_fname)
    {
      যদি (verbose)
        error (0, errno, "%s", quotef (fname));
      ঘূৰাই false;
    }

  যদি (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  putchar (use_nuls ? '\0' : '\n');

  মুক্ত (can_fname);

  যদি (ferror (stdout))
    write_error ();

  ঘূৰাই true;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  bool ok = true;
  পূর্ণাঙ্ক can_mode = CAN_ALL_BUT_LAST;
  ৱর্ণ ধ্রুৱ *relative_to = রিক্ত;
  ৱর্ণ ধ্রুৱ *relative_base = রিক্ত;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ (true)
    {
      পূর্ণাঙ্ক c = getopt_long (argc, argv, "EeLmPqsz", longopts, রিক্ত);
      যদি (c == -1)
        ভঙ্গ;
      ৱিকল্প (c)
        {
        পক্ষ 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          ভঙ্গ;
        পক্ষ 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          ভঙ্গ;
        পক্ষ 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          ভঙ্গ;
        পক্ষ 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          ভঙ্গ;
        পক্ষ 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          ভঙ্গ;
        পক্ষ 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          ভঙ্গ;
        পক্ষ 'q':
          verbose = false;
          ভঙ্গ;
        পক্ষ 'z':
          use_nuls = true;
          ভঙ্গ;
        পক্ষ RELATIVE_TO_OPTION:
          relative_to = optarg;
          ভঙ্গ;
        পক্ষ RELATIVE_BASE_OPTION:
          relative_base = optarg;
          ভঙ্গ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
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
      পূর্ণাঙ্ক qs = getenv_quoting_style ();
      যদি (qs < 0)
        qs = shell_escape_quoting_style;
      যদি (qs != literal_quoting_style)
        {
          set_quoting_style (রিক্ত, qs);
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
      ৱর্ণ *base = realpath_canon (relative_base, can_mode);
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
          can_relative_to = রিক্ত;
        }
    }

  বাবে (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  ঘূৰাই ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
