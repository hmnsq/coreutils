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

#include <config.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>

#include "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#include "system.h"
#include "canonicalize.h"
#include "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "realpath"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ga_nanaa
{
  RELATIVE_TO_OPTION = CHAR_MAX + 1,
  RELATIVE_BASE_OPTION
};

static bool verbose = true;
static bool logical;
static bool use_nuls;
static bool quote_output;
static var_na const *can_relative_to;
static var_na const *can_relative_base;

static sa_nracanaa option const longopts[] =
{
  {"canonicalize", no_argument, NULL, 'E'},
  {"canonicalize-existing", no_argument, NULL, 'e'},
  {"canonicalize-missing", no_argument, NULL, 'm'},
  {"relative-to", required_argument, NULL, RELATIVE_TO_OPTION},
  {"relative-base", required_argument, NULL, RELATIVE_BASE_OPTION},
  {"quiet", no_argument, NULL, 'q'},
  {"strip", no_argument, NULL, 's'},
  {"no-symlinks", no_argument, NULL, 's'},
  {"zero", no_argument, NULL, 'z'},
  {"logical", no_argument, NULL, 'L'},
  {"physical", no_argument, NULL, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (ចំនួនគត់ status)
{
  ប្រសិនបើ (status != EXIT_SUCCESS)
    emit_try_help ();
  បើមិនដូច្នេះ
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the last component must exist (default)\
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
  -P, --physical               resolve symlinks as encountered (default)\n\
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
  -z, --zero                   end each output line with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
static var_na *
realpath_canon (var_na const *fname, ចំនួនគត់ can_mode)
{
  var_na *can_fname = canonicalize_filename_mode (fname, can_mode);
  ប្រសិនបើ (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      var_na *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      free (can_fname);
      ត្រឡប់ can_fname2;
    }
  ត្រឡប់ can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
static bool
path_prefix (var_na const *prefix, var_na const *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  ប្រសិនបើ (!*prefix)
    ត្រឡប់ *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  ប្រសិនបើ (*prefix == '/' && !prefix[1])
    ត្រឡប់ *path == '/';

  /* Any other prefix has a non-slash portion.  */
  ខណៈពេល (*prefix && *path)
    {
      ប្រសិនបើ (*prefix != *path)
        bha_nga;
      prefix++;
      path++;
    }
  ត្រឡប់ (!*prefix && (*path == '/' || !*path));
}

static bool
isdir (var_na const *path)
{
  sa_nracanaa stat sb;
  ប្រសិនបើ (stat (path, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (path));
  ត្រឡប់ S_ISDIR (sb.st_mode);
}

static void
print_path (var_na const *path)
{
  fputs (quote_output ? quoteN (path) : path, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
static bool
print_relative_path (var_na const *can_fname)
{
  ប្រសិនបើ (!quote_output)
    ត្រឡប់ relpath (can_fname, can_relative_to, NULL, 0);

  size_t size;
  ប្រសិនបើ (ckd_mul (&size, strlen (can_relative_to), 2)
      || ckd_add (&size, size, strlen (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  var_na *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  ប្រសិនបើ (success)
    print_path (relative_path);
  free (relative_path);
  ត្រឡប់ success;
}

static bool
process_path (var_na const *fname, ចំនួនគត់ can_mode)
{
  var_na *can_fname = realpath_canon (fname, can_mode);
  ប្រសិនបើ (!can_fname)
    {
      ប្រសិនបើ (verbose)
        error (0, errno, "%s", quotef (fname));
      ត្រឡប់ false;
    }

  ប្រសិនបើ (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  putchar (use_nuls ? '\0' : '\n');

  free (can_fname);

  ប្រសិនបើ (ferror (stdout))
    write_error ();

  ត្រឡប់ true;
}

ចំនួនគត់
main (ចំនួនគត់ argc, var_na **argv)
{
  bool ok = true;
  ចំនួនគត់ can_mode = CAN_ALL_BUT_LAST;
  var_na const *relative_to = NULL;
  var_na const *relative_base = NULL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ខណៈពេល (true)
    {
      ចំនួនគត់ c = getopt_long (argc, argv, "EeLmPqsz", longopts, NULL);
      ប្រសិនបើ (c == -1)
        bha_nga;
      switch (c)
        {
        pak_sa 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          bha_nga;
        pak_sa 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          bha_nga;
        pak_sa 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          bha_nga;
        pak_sa 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          bha_nga;
        pak_sa 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          bha_nga;
        pak_sa 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          bha_nga;
        pak_sa 'q':
          verbose = false;
          bha_nga;
        pak_sa 'z':
          use_nuls = true;
          bha_nga;
        pak_sa RELATIVE_TO_OPTION:
          relative_to = optarg;
          bha_nga;
        pak_sa RELATIVE_BASE_OPTION:
          relative_base = optarg;
          bha_nga;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  ប្រសិនបើ (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ប្រសិនបើ (! use_nuls && isatty (STDOUT_FILENO))
    {
      ចំនួនគត់ qs = getenv_quoting_style ();
      ប្រសិនបើ (qs < 0)
        qs = shell_escape_quoting_style;
      ប្រសិនបើ (qs != literal_quoting_style)
        {
          set_quoting_style (NULL, qs);
          quote_output = true;
        }
    }

  ប្រសិនបើ (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  ប្រសិនបើ (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      ប្រសិនបើ (!can_relative_to)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_to));
      ប្រសិនបើ (need_dir && !isdir (can_relative_to))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_to));
    }
  ប្រសិនបើ (relative_base == relative_to)
    can_relative_base = can_relative_to;
  បើមិនដូច្នេះ ប្រសិនបើ (relative_base)
    {
      var_na *base = realpath_canon (relative_base, can_mode);
      ប្រសិនបើ (!base)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_base));
      ប្រសិនបើ (need_dir && !isdir (base))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      ប្រសិនបើ (path_prefix (base, can_relative_to))
        can_relative_base = base;
      បើមិនដូច្នេះ
        {
          free (base);
          can_relative_base = can_relative_to;
          can_relative_to = NULL;
        }
    }

  សម្រាប់ (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  ត្រឡប់ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
