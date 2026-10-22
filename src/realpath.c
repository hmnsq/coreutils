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
स्थिर अक्षर स्थिरांक *can_relative_to;
स्थिर अक्षर स्थिरांक *can_relative_base;

स्थिर संरचना option स्थिरांक longopts[] =
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
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      ख_माला_दो (_("\
Print the resolved absolute file name.\n\
"), मानक_निकास);
      oputs (_("\
  -E, --canonicalize           all but the last component must exist (पूर्वनिर्धारित)\
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
  -P, --physical               resolve symlinks as encountered (पूर्वनिर्धारित)\n\
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
  -z, --zero                   end each output ओळ with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
स्थिर अक्षर *
realpath_canon (अक्षर स्थिरांक *fname, पूर्णांक can_mode)
{
  अक्षर *can_fname = canonicalize_filename_mode (fname, can_mode);
  जर (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      अक्षर *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      मुक्त (can_fname);
      परत can_fname2;
    }
  परत can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
स्थिर bool
path_prefix (अक्षर स्थिरांक *prefix, अक्षर स्थिरांक *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  जर (!*prefix)
    परत *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  जर (*prefix == '/' && !prefix[1])
    परत *path == '/';

  /* Any other prefix has a non-slash portion.  */
  पर्यंत (*prefix && *path)
    {
      जर (*prefix != *path)
        थांबा;
      prefix++;
      path++;
    }
  परत (!*prefix && (*path == '/' || !*path));
}

स्थिर bool
isdir (अक्षर स्थिरांक *path)
{
  संरचना stat sb;
  जर (stat (path, &sb) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot stat %s"), quoteaf (path));
  परत S_ISDIR (sb.st_mode);
}

स्थिर शून्य
print_path (अक्षर स्थिरांक *path)
{
  ख_माला_दो (quote_output ? quoteN (path) : path, मानक_निकास);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
स्थिर bool
print_relative_path (अक्षर स्थिरांक *can_fname)
{
  जर (!quote_output)
    परत relpath (can_fname, can_relative_to, रिक्त, 0);

  माप_प्रकार size;
  जर (ckd_mul (&size, म_माप (can_relative_to), 2)
      || ckd_add (&size, size, म_माप (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  अक्षर *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  जर (success)
    print_path (relative_path);
  मुक्त (relative_path);
  परत success;
}

स्थिर bool
process_path (अक्षर स्थिरांक *fname, पूर्णांक can_mode)
{
  अक्षर *can_fname = realpath_canon (fname, can_mode);
  जर (!can_fname)
    {
      जर (verbose)
        error (0, त्रुटि_सं, "%s", quotef (fname));
      परत false;
    }

  जर (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  अक्षर_दो (use_nuls ? '\0' : '\n');

  मुक्त (can_fname);

  जर (ख_त्रुटि (मानक_निकास))
    write_error ();

  परत true;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  bool ok = true;
  पूर्णांक can_mode = CAN_ALL_BUT_LAST;
  अक्षर स्थिरांक *relative_to = रिक्त;
  अक्षर स्थिरांक *relative_base = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पर्यंत (true)
    {
      पूर्णांक c = getopt_long (argc, argv, "EeLmPqsz", longopts, रिक्त);
      जर (c == -1)
        थांबा;
      निवड (c)
        {
        स्थिती 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          थांबा;
        स्थिती 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          थांबा;
        स्थिती 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          थांबा;
        स्थिती 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          थांबा;
        स्थिती 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          थांबा;
        स्थिती 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          थांबा;
        स्थिती 'q':
          verbose = false;
          थांबा;
        स्थिती 'z':
          use_nuls = true;
          थांबा;
        स्थिती RELATIVE_TO_OPTION:
          relative_to = optarg;
          थांबा;
        स्थिती RELATIVE_BASE_OPTION:
          relative_base = optarg;
          थांबा;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  जर (! use_nuls && isatty (STDOUT_FILENO))
    {
      पूर्णांक qs = getenv_quoting_style ();
      जर (qs < 0)
        qs = shell_escape_quoting_style;
      जर (qs != literal_quoting_style)
        {
          set_quoting_style (रिक्त, qs);
          quote_output = true;
        }
    }

  जर (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  जर (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      जर (!can_relative_to)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (relative_to));
      जर (need_dir && !isdir (can_relative_to))
        error (निकास_त्रुटि, ENOTDIR, "%s", quotef (relative_to));
    }
  जर (relative_base == relative_to)
    can_relative_base = can_relative_to;
  अन्यथा जर (relative_base)
    {
      अक्षर *base = realpath_canon (relative_base, can_mode);
      जर (!base)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (relative_base));
      जर (need_dir && !isdir (base))
        error (निकास_त्रुटि, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      जर (path_prefix (base, can_relative_to))
        can_relative_base = base;
      अन्यथा
        {
          मुक्त (base);
          can_relative_base = can_relative_to;
          can_relative_to = रिक्त;
        }
    }

  साठी (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  परत ok ? निकास_सफल : निकास_त्रुटि;
}
