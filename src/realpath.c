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

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#अन्तर्भाव "system.h"
#अन्तर्भाव "canonicalize.h"
#अन्तर्भाव "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "realpath"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

गणना
{
  RELATIVE_TO_OPTION = CHAR_MAX + 1,
  RELATIVE_BASE_OPTION
};

स्थिर bool verbose = true;
स्थिर bool logical;
स्थिर bool use_nuls;
स्थिर bool quote_output;
स्थिर वर्ण ध्रुव *can_relative_to;
स्थिर वर्ण ध्रुव *can_relative_base;

स्थिर संरचना option ध्रुव longopts[] =
{
  {"canonicalize", no_argument, रिक्त, 'E'},
  {"canonicalize-existing", no_argument, रिक्त, 'e'},
  {"canonicalize-missing", no_argument, रिक्त, 'm'},
  {"relative-to", required_argument, रिक्त, RELATIVE_TO_OPTION},
  {"relative-base", required_argument, रिक्त, RELATIVE_BASE_OPTION},
  {"quiet", no_argument, रिक्त, 'q'},
  {"strip", no_argument, रिक्त, 's'},
  {"no-symlinks", no_argument, रिक्त, 's'},
  {"zero", no_argument, रिक्त, 'z'},
  {"logical", no_argument, रिक्त, 'L'},
  {"physical", no_argument, रिक्त, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the last component must exist (स्वभाव)\
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
  -P, --physical               resolve symlinks as encountered (स्वभाव)\n\
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
  -z, --zero                   end each output पङ्क्तिः with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
स्थिर वर्ण *
realpath_canon (वर्ण ध्रुव *fname, पूर्णाङ्क can_mode)
{
  वर्ण *can_fname = canonicalize_filename_mode (fname, can_mode);
  सचे (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      वर्ण *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      मुक्त (can_fname);
      पटिनिवत्त can_fname2;
    }
  पटिनिवत्त can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
स्थिर bool
path_prefix (वर्ण ध्रुव *prefix, वर्ण ध्रुव *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  सचे (!*prefix)
    पटिनिवत्त *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  सचे (*prefix == '/' && !prefix[1])
    पटिनिवत्त *path == '/';

  /* Any other prefix has a non-slash portion.  */
  याव (*prefix && *path)
    {
      सचे (*prefix != *path)
        भङ्ग;
      prefix++;
      path++;
    }
  पटिनिवत्त (!*prefix && (*path == '/' || !*path));
}

स्थिर bool
isdir (वर्ण ध्रुव *path)
{
  संरचना stat sb;
  सचे (stat (path, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (path));
  पटिनिवत्त S_ISDIR (sb.st_mode);
}

स्थिर शून्य
print_path (वर्ण ध्रुव *path)
{
  fputs (quote_output ? quoteN (path) : path, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
स्थिर bool
print_relative_path (वर्ण ध्रुव *can_fname)
{
  सचे (!quote_output)
    पटिनिवत्त relpath (can_fname, can_relative_to, रिक्त, 0);

  size_t size;
  सचे (ckd_mul (&size, strlen (can_relative_to), 2)
      || ckd_add (&size, size, strlen (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  वर्ण *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  सचे (success)
    print_path (relative_path);
  मुक्त (relative_path);
  पटिनिवत्त success;
}

स्थिर bool
process_path (वर्ण ध्रुव *fname, पूर्णाङ्क can_mode)
{
  वर्ण *can_fname = realpath_canon (fname, can_mode);
  सचे (!can_fname)
    {
      सचे (verbose)
        error (0, errno, "%s", quotef (fname));
      पटिनिवत्त false;
    }

  सचे (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  putchar (use_nuls ? '\0' : '\n');

  मुक्त (can_fname);

  सचे (ferror (stdout))
    write_error ();

  पटिनिवत्त true;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  bool ok = true;
  पूर्णाङ्क can_mode = CAN_ALL_BUT_LAST;
  वर्ण ध्रुव *relative_to = रिक्त;
  वर्ण ध्रुव *relative_base = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  याव (true)
    {
      पूर्णाङ्क c = getopt_long (argc, argv, "EeLmPqsz", longopts, रिक्त);
      सचे (c == -1)
        भङ्ग;
      विकल्प (c)
        {
        पक्ष 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          भङ्ग;
        पक्ष 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          भङ्ग;
        पक्ष 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          भङ्ग;
        पक्ष 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          भङ्ग;
        पक्ष 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          भङ्ग;
        पक्ष 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          भङ्ग;
        पक्ष 'q':
          verbose = false;
          भङ्ग;
        पक्ष 'z':
          use_nuls = true;
          भङ्ग;
        पक्ष RELATIVE_TO_OPTION:
          relative_to = optarg;
          भङ्ग;
        पक्ष RELATIVE_BASE_OPTION:
          relative_base = optarg;
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  सचे (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  सचे (! use_nuls && isatty (STDOUT_FILENO))
    {
      पूर्णाङ्क qs = getenv_quoting_style ();
      सचे (qs < 0)
        qs = shell_escape_quoting_style;
      सचे (qs != literal_quoting_style)
        {
          set_quoting_style (रिक्त, qs);
          quote_output = true;
        }
    }

  सचे (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  सचे (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      सचे (!can_relative_to)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_to));
      सचे (need_dir && !isdir (can_relative_to))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_to));
    }
  सचे (relative_base == relative_to)
    can_relative_base = can_relative_to;
  अञ्ञथा सचे (relative_base)
    {
      वर्ण *base = realpath_canon (relative_base, can_mode);
      सचे (!base)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_base));
      सचे (need_dir && !isdir (base))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      सचे (path_prefix (base, can_relative_to))
        can_relative_base = base;
      अञ्ञथा
        {
          मुक्त (base);
          can_relative_base = can_relative_to;
          can_relative_to = रिक्त;
        }
    }

  अत्थाय (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  पटिनिवत्त ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
