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

#समावेश <config.h>
#समावेश <getopt.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>

#समावेश "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#समावेश "system.h"
#समावेश "canonicalize.h"
#समावेश "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "realpath"

#घोषणा AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

क्रमागत
{
  RELATIVE_TO_OPTION = अक्षर_उच्च + 1,
  RELATIVE_BASE_OPTION
};

अटल bool verbose = true;
अटल bool logical;
अटल bool use_nuls;
अटल bool quote_output;
अटल अक्षर स्थिर *can_relative_to;
अटल अक्षर स्थिर *can_relative_base;

अटल काष्ठा option स्थिर longopts[] =
{
  {"canonicalize", no_argument, शून्य, 'E'},
  {"canonicalize-existing", no_argument, शून्य, 'e'},
  {"canonicalize-missing", no_argument, शून्य, 'm'},
  {"relative-to", required_argument, शून्य, RELATIVE_TO_OPTION},
  {"relative-base", required_argument, शून्य, RELATIVE_BASE_OPTION},
  {"quiet", no_argument, शून्य, 'q'},
  {"strip", no_argument, शून्य, 's'},
  {"no-symlinks", no_argument, शून्य, 's'},
  {"zero", no_argument, शून्य, 'z'},
  {"logical", no_argument, शून्य, 'L'},
  {"physical", no_argument, शून्य, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      ख_माला_दो (_("\
Print the resolved absolute file name.\n\
"), मानक_निकास);
      oputs (_("\
  -E, --canonicalize           all but the last component must exist (शेष)\
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
  -P, --physical               resolve symlinks as encountered (शेष)\n\
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
  -z, --zero                   end each output पंक्ति with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
अटल अक्षर *
realpath_canon (अक्षर स्थिर *fname, पूर्णांक can_mode)
{
  अक्षर *can_fname = canonicalize_filename_mode (fname, can_mode);
  अगर (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      अक्षर *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      मुक्त (can_fname);
      वापस can_fname2;
    }
  वापस can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
अटल bool
path_prefix (अक्षर स्थिर *prefix, अक्षर स्थिर *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  अगर (!*prefix)
    वापस *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  अगर (*prefix == '/' && !prefix[1])
    वापस *path == '/';

  /* Any other prefix has a non-slash portion.  */
  जबतक (*prefix && *path)
    {
      अगर (*prefix != *path)
        अवरोध;
      prefix++;
      path++;
    }
  वापस (!*prefix && (*path == '/' || !*path));
}

अटल bool
isdir (अक्षर स्थिर *path)
{
  काष्ठा stat sb;
  अगर (stat (path, &sb) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot stat %s"), quoteaf (path));
  वापस S_ISDIR (sb.st_mode);
}

अटल व्योम
print_path (अक्षर स्थिर *path)
{
  ख_माला_दो (quote_output ? quoteN (path) : path, मानक_निकास);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
अटल bool
print_relative_path (अक्षर स्थिर *can_fname)
{
  अगर (!quote_output)
    वापस relpath (can_fname, can_relative_to, शून्य, 0);

  माप_प्रकार आकार;
  अगर (ckd_mul (&आकार, म_माप (can_relative_to), 2)
      || ckd_add (&आकार, आकार, म_माप (can_fname))
      || ckd_add (&आकार, आकार, 1))
    xalloc_die ();

  अक्षर *relative_path = xmalloc (आकार);
  bool success = relpath (can_fname, can_relative_to, relative_path, आकार);
  अगर (success)
    print_path (relative_path);
  मुक्त (relative_path);
  वापस success;
}

अटल bool
process_path (अक्षर स्थिर *fname, पूर्णांक can_mode)
{
  अक्षर *can_fname = realpath_canon (fname, can_mode);
  अगर (!can_fname)
    {
      अगर (verbose)
        error (0, त्रुटि_सं, "%s", quotef (fname));
      वापस false;
    }

  अगर (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  अक्षर_दो (use_nuls ? '\0' : '\n');

  मुक्त (can_fname);

  अगर (ख_त्रुटि (मानक_निकास))
    write_error ();

  वापस true;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  bool ok = true;
  पूर्णांक can_mode = CAN_ALL_BUT_LAST;
  अक्षर स्थिर *relative_to = शून्य;
  अक्षर स्थिर *relative_base = शून्य;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक (true)
    {
      पूर्णांक c = getopt_long (argc, argv, "EeLmPqsz", longopts, शून्य);
      अगर (c == -1)
        अवरोध;
      चयन (c)
        {
        हाल 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          अवरोध;
        हाल 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          अवरोध;
        हाल 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          अवरोध;
        हाल 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          अवरोध;
        हाल 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          अवरोध;
        हाल 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          अवरोध;
        हाल 'q':
          verbose = false;
          अवरोध;
        हाल 'z':
          use_nuls = true;
          अवरोध;
        हाल RELATIVE_TO_OPTION:
          relative_to = optarg;
          अवरोध;
        हाल RELATIVE_BASE_OPTION:
          relative_base = optarg;
          अवरोध;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  अगर (! use_nuls && isatty (STDOUT_FILENO))
    {
      पूर्णांक qs = getenv_quoting_style ();
      अगर (qs < 0)
        qs = shell_escape_quoting_style;
      अगर (qs != literal_quoting_style)
        {
          set_quoting_style (शून्य, qs);
          quote_output = true;
        }
    }

  अगर (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  अगर (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      अगर (!can_relative_to)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (relative_to));
      अगर (need_dir && !isdir (can_relative_to))
        error (निकास_त्रुटि, ENOTDIR, "%s", quotef (relative_to));
    }
  अगर (relative_base == relative_to)
    can_relative_base = can_relative_to;
  अन्यथा अगर (relative_base)
    {
      अक्षर *base = realpath_canon (relative_base, can_mode);
      अगर (!base)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (relative_base));
      अगर (need_dir && !isdir (base))
        error (निकास_त्रुटि, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      अगर (path_prefix (base, can_relative_to))
        can_relative_base = base;
      अन्यथा
        {
          मुक्त (base);
          can_relative_base = can_relative_to;
          can_relative_to = शून्य;
        }
    }

  क्रम (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
