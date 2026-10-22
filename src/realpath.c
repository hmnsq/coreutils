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
  RELATIVE_TO_OPTION = अक्षर_उच्च + 1,
  RELATIVE_BASE_OPTION
};

स्थिर bool verbose = true;
स्थिर bool logical;
स्थिर bool use_nuls;
स्थिर bool quote_output;
स्थिर वर्ण अचर *can_relative_to;
स्थिर वर्ण अचर *can_relative_base;

स्थिर संरचना option अचर longopts[] =
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
  यदि (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      ख_माला_दो (_("\
Print the resolved absolute file name.\n\
"), मानक_निकास);
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
      --relative-to=सूची        print the resolved path relative to सूची\n\
"));
      oputs (_("\
      --relative-base=सूची      print absolute paths unless paths below सूची\n\
"));
      oputs (_("\
  -s, --strip, --no-symlinks   don't expand symlinks\n\
"));
      oputs (_("\
  -z, --zero                   end each output पङ्क्ति with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
स्थिर वर्ण *
realpath_canon (वर्ण अचर *fname, पूर्णाङ्क can_mode)
{
  वर्ण *can_fname = canonicalize_filename_mode (fname, can_mode);
  यदि (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      वर्ण *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      मुक्त (can_fname);
      फर्काऊ can_fname2;
    }
  फर्काऊ can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
स्थिर bool
path_prefix (वर्ण अचर *prefix, वर्ण अचर *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  यदि (!*prefix)
    फर्काऊ *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  यदि (*prefix == '/' && !prefix[1])
    फर्काऊ *path == '/';

  /* Any other prefix has a non-slash portion.  */
  जबसम्म (*prefix && *path)
    {
      यदि (*prefix != *path)
        रोक;
      prefix++;
      path++;
    }
  फर्काऊ (!*prefix && (*path == '/' || !*path));
}

स्थिर bool
isdir (वर्ण अचर *path)
{
  संरचना stat sb;
  यदि (stat (path, &sb) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot stat %s"), quoteaf (path));
  फर्काऊ S_ISDIR (sb.st_mode);
}

स्थिर शून्य
print_path (वर्ण अचर *path)
{
  ख_माला_दो (quote_output ? quoteN (path) : path, मानक_निकास);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
स्थिर bool
print_relative_path (वर्ण अचर *can_fname)
{
  यदि (!quote_output)
    फर्काऊ relpath (can_fname, can_relative_to, रिक्त, 0);

  माप_प्रकार size;
  यदि (ckd_mul (&size, म_माप (can_relative_to), 2)
      || ckd_add (&size, size, म_माप (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  वर्ण *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  यदि (success)
    print_path (relative_path);
  मुक्त (relative_path);
  फर्काऊ success;
}

स्थिर bool
process_path (वर्ण अचर *fname, पूर्णाङ्क can_mode)
{
  वर्ण *can_fname = realpath_canon (fname, can_mode);
  यदि (!can_fname)
    {
      यदि (verbose)
        error (0, त्रुटि_सं, "%s", quotef (fname));
      फर्काऊ false;
    }

  यदि (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  अक्षर_दो (use_nuls ? '\0' : '\n');

  मुक्त (can_fname);

  यदि (ख_त्रुटि (मानक_निकास))
    write_error ();

  फर्काऊ true;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  bool ok = true;
  पूर्णाङ्क can_mode = CAN_ALL_BUT_LAST;
  वर्ण अचर *relative_to = रिक्त;
  वर्ण अचर *relative_base = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबसम्म (true)
    {
      पूर्णाङ्क c = getopt_long (argc, argv, "EeLmPqsz", longopts, रिक्त);
      यदि (c == -1)
        रोक;
      विकल्प (c)
        {
        पक्ष 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          रोक;
        पक्ष 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          रोक;
        पक्ष 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          रोक;
        पक्ष 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          रोक;
        पक्ष 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          रोक;
        पक्ष 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          रोक;
        पक्ष 'q':
          verbose = false;
          रोक;
        पक्ष 'z':
          use_nuls = true;
          रोक;
        पक्ष RELATIVE_TO_OPTION:
          relative_to = optarg;
          रोक;
        पक्ष RELATIVE_BASE_OPTION:
          relative_base = optarg;
          रोक;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (निकास_त्रुटि);
        }
    }

  यदि (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
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
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (relative_to));
      यदि (need_dir && !isdir (can_relative_to))
        error (निकास_त्रुटि, ENOTDIR, "%s", quotef (relative_to));
    }
  यदि (relative_base == relative_to)
    can_relative_base = can_relative_to;
  अन्यथा यदि (relative_base)
    {
      वर्ण *base = realpath_canon (relative_base, can_mode);
      यदि (!base)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (relative_base));
      यदि (need_dir && !isdir (base))
        error (निकास_त्रुटि, ENOTDIR, "%s", quotef (relative_base));
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

  लागि (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  फर्काऊ ok ? निकास_सफल : निकास_त्रुटि;
}
