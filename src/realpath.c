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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "canonicalize.h"
#ಅನ್ತರ್ಭಾವ "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "realpath"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ಗಣನಾ
{
  RELATIVE_TO_OPTION = CHAR_MAX + 1,
  RELATIVE_BASE_OPTION
};

ಸ್ಥಿರ bool verbose = true;
ಸ್ಥಿರ bool logical;
ಸ್ಥಿರ bool use_nuls;
ಸ್ಥಿರ bool quote_output;
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *can_relative_to;
ಸ್ಥಿರ ವರ್ಣ ಧ್ರುವ *can_relative_base;

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"canonicalize", no_argument, ರಿಕ್ತ, 'E'},
  {"canonicalize-existing", no_argument, ರಿಕ್ತ, 'e'},
  {"canonicalize-missing", no_argument, ರಿಕ್ತ, 'm'},
  {"relative-to", required_argument, ರಿಕ್ತ, RELATIVE_TO_OPTION},
  {"relative-base", required_argument, ರಿಕ್ತ, RELATIVE_BASE_OPTION},
  {"quiet", no_argument, ರಿಕ್ತ, 'q'},
  {"strip", no_argument, ರಿಕ್ತ, 's'},
  {"no-symlinks", no_argument, ರಿಕ್ತ, 's'},
  {"zero", no_argument, ರಿಕ್ತ, 'z'},
  {"logical", no_argument, ರಿಕ್ತ, 'L'},
  {"physical", no_argument, ರಿಕ್ತ, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the last component must exist (ಸ್ವಭಾವ)\
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
  -P, --physical               resolve symlinks as encountered (ಸ್ವಭಾವ)\n\
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
  -z, --zero                   end each output ಸಾಲು with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
ಸ್ಥಿರ ವರ್ಣ *
realpath_canon (ವರ್ಣ ಧ್ರುವ *fname, ಪೂರ್ಣಾಙ್ಕ can_mode)
{
  ವರ್ಣ *can_fname = canonicalize_filename_mode (fname, can_mode);
  ಯದಿ (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      ವರ್ಣ *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      ಮುಕ್ತ (can_fname);
      ಪ್ರತ್ಯಾಗಮ can_fname2;
    }
  ಪ್ರತ್ಯಾಗಮ can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
ಸ್ಥಿರ bool
path_prefix (ವರ್ಣ ಧ್ರುವ *prefix, ವರ್ಣ ಧ್ರುವ *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  ಯದಿ (!*prefix)
    ಪ್ರತ್ಯಾಗಮ *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  ಯದಿ (*prefix == '/' && !prefix[1])
    ಪ್ರತ್ಯಾಗಮ *path == '/';

  /* Any other prefix has a non-slash portion.  */
  ಯಾವತ್ (*prefix && *path)
    {
      ಯದಿ (*prefix != *path)
        ಭಙ್ಗ;
      prefix++;
      path++;
    }
  ಪ್ರತ್ಯಾಗಮ (!*prefix && (*path == '/' || !*path));
}

ಸ್ಥಿರ bool
isdir (ವರ್ಣ ಧ್ರುವ *path)
{
  ಸಂರಚನಾ stat sb;
  ಯದಿ (stat (path, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (path));
  ಪ್ರತ್ಯಾಗಮ S_ISDIR (sb.st_mode);
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_path (ವರ್ಣ ಧ್ರುವ *path)
{
  fputs (quote_output ? quoteN (path) : path, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
ಸ್ಥಿರ bool
print_relative_path (ವರ್ಣ ಧ್ರುವ *can_fname)
{
  ಯದಿ (!quote_output)
    ಪ್ರತ್ಯಾಗಮ relpath (can_fname, can_relative_to, ರಿಕ್ತ, 0);

  size_t size;
  ಯದಿ (ckd_mul (&size, strlen (can_relative_to), 2)
      || ckd_add (&size, size, strlen (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  ವರ್ಣ *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  ಯದಿ (success)
    print_path (relative_path);
  ಮುಕ್ತ (relative_path);
  ಪ್ರತ್ಯಾಗಮ success;
}

ಸ್ಥಿರ bool
process_path (ವರ್ಣ ಧ್ರುವ *fname, ಪೂರ್ಣಾಙ್ಕ can_mode)
{
  ವರ್ಣ *can_fname = realpath_canon (fname, can_mode);
  ಯದಿ (!can_fname)
    {
      ಯದಿ (verbose)
        error (0, errno, "%s", quotef (fname));
      ಪ್ರತ್ಯಾಗಮ false;
    }

  ಯದಿ (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  putchar (use_nuls ? '\0' : '\n');

  ಮುಕ್ತ (can_fname);

  ಯದಿ (ferror (stdout))
    write_error ();

  ಪ್ರತ್ಯಾಗಮ true;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  bool ok = true;
  ಪೂರ್ಣಾಙ್ಕ can_mode = CAN_ALL_BUT_LAST;
  ವರ್ಣ ಧ್ರುವ *relative_to = ರಿಕ್ತ;
  ವರ್ಣ ಧ್ರುವ *relative_base = ರಿಕ್ತ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ (true)
    {
      ಪೂರ್ಣಾಙ್ಕ c = getopt_long (argc, argv, "EeLmPqsz", longopts, ರಿಕ್ತ);
      ಯದಿ (c == -1)
        ಭಙ್ಗ;
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'q':
          verbose = false;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'z':
          use_nuls = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ RELATIVE_TO_OPTION:
          relative_to = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ RELATIVE_BASE_OPTION:
          relative_base = optarg;
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (! use_nuls && isatty (STDOUT_FILENO))
    {
      ಪೂರ್ಣಾಙ್ಕ qs = getenv_quoting_style ();
      ಯದಿ (qs < 0)
        qs = shell_escape_quoting_style;
      ಯದಿ (qs != literal_quoting_style)
        {
          set_quoting_style (ರಿಕ್ತ, qs);
          quote_output = true;
        }
    }

  ಯದಿ (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  ಯದಿ (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      ಯದಿ (!can_relative_to)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_to));
      ಯದಿ (need_dir && !isdir (can_relative_to))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_to));
    }
  ಯದಿ (relative_base == relative_to)
    can_relative_base = can_relative_to;
  ಅನ್ಯಥಾ ಯದಿ (relative_base)
    {
      ವರ್ಣ *base = realpath_canon (relative_base, can_mode);
      ಯದಿ (!base)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_base));
      ಯದಿ (need_dir && !isdir (base))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      ಯದಿ (path_prefix (base, can_relative_to))
        can_relative_base = base;
      ಅನ್ಯಥಾ
        {
          ಮುಕ್ತ (base);
          can_relative_base = can_relative_to;
          can_relative_to = ರಿಕ್ತ;
        }
    }

  ಪ್ರತಿ (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
