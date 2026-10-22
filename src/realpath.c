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
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
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
  -z, --zero                   end each output ओळ with NUL, not newline\n\
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
  यदि (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      वर्ण *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      मुक्त (can_fname);
      प्रत्यागम can_fname2;
    }
  प्रत्यागम can_fname;
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
  यदि (!*prefix)
    प्रत्यागम *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  यदि (*prefix == '/' && !prefix[1])
    प्रत्यागम *path == '/';

  /* Any other prefix has a non-slash portion.  */
  यावत् (*prefix && *path)
    {
      यदि (*prefix != *path)
        भङ्ग;
      prefix++;
      path++;
    }
  प्रत्यागम (!*prefix && (*path == '/' || !*path));
}

स्थिर bool
isdir (वर्ण ध्रुव *path)
{
  संरचना stat sb;
  यदि (stat (path, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (path));
  प्रत्यागम S_ISDIR (sb.st_mode);
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
  यदि (!quote_output)
    प्रत्यागम relpath (can_fname, can_relative_to, रिक्त, 0);

  size_t size;
  यदि (ckd_mul (&size, strlen (can_relative_to), 2)
      || ckd_add (&size, size, strlen (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  वर्ण *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  यदि (success)
    print_path (relative_path);
  मुक्त (relative_path);
  प्रत्यागम success;
}

स्थिर bool
process_path (वर्ण ध्रुव *fname, पूर्णाङ्क can_mode)
{
  वर्ण *can_fname = realpath_canon (fname, can_mode);
  यदि (!can_fname)
    {
      यदि (verbose)
        error (0, errno, "%s", quotef (fname));
      प्रत्यागम false;
    }

  यदि (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  putchar (use_nuls ? '\0' : '\n');

  मुक्त (can_fname);

  यदि (ferror (stdout))
    write_error ();

  प्रत्यागम true;
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

  यावत् (true)
    {
      पूर्णाङ्क c = getopt_long (argc, argv, "EeLmPqsz", longopts, रिक्त);
      यदि (c == -1)
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

  यदि (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  यदि (! use_nuls && isatty (STDOUT_FILENO))
    {
      पूर्णाङ्क qs = getenv_quoting_style ();
      यदि (qs < 0)
        qs = shell_escape_quoting_style;
      यदि (qs != literal_quoting_style)
        {
          set_quoting_style (रिक्त, qs);
          quote_output = true;
        }
    }

  यदि (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  यदि (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      यदि (!can_relative_to)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_to));
      यदि (need_dir && !isdir (can_relative_to))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_to));
    }
  यदि (relative_base == relative_to)
    can_relative_base = can_relative_to;
  अन्यथा यदि (relative_base)
    {
      वर्ण *base = realpath_canon (relative_base, can_mode);
      यदि (!base)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_base));
      यदि (need_dir && !isdir (base))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      यदि (path_prefix (base, can_relative_to))
        can_relative_base = base;
      अन्यथा
        {
          मुक्त (base);
          can_relative_base = can_relative_to;
          can_relative_to = रिक्त;
        }
    }

  प्रति (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
