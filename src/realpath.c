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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#அந்தர்பாவ "system.h"
#அந்தர்பாவ "canonicalize.h"
#அந்தர்பாவ "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "realpath"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

எண்ணிக்கை
{
  RELATIVE_TO_OPTION = CHAR_MAX + 1,
  RELATIVE_BASE_OPTION
};

நிலையான bool verbose = true;
நிலையான bool logical;
நிலையான bool use_nuls;
நிலையான bool quote_output;
நிலையான எழுத்து மாறிலி *can_relative_to;
நிலையான எழுத்து மாறிலி *can_relative_base;

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"canonicalize", no_argument, வெற்று, 'E'},
  {"canonicalize-existing", no_argument, வெற்று, 'e'},
  {"canonicalize-missing", no_argument, வெற்று, 'm'},
  {"relative-to", required_argument, வெற்று, RELATIVE_TO_OPTION},
  {"relative-base", required_argument, வெற்று, RELATIVE_BASE_OPTION},
  {"quiet", no_argument, வெற்று, 'q'},
  {"strip", no_argument, வெற்று, 's'},
  {"no-symlinks", no_argument, வெற்று, 's'},
  {"zero", no_argument, வெற்று, 'z'},
  {"logical", no_argument, வெற்று, 'L'},
  {"physical", no_argument, வெற்று, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the last component must exist (இயல்பு)\
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
  -P, --physical               resolve symlinks as encountered (இயல்பு)\n\
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
  -z, --zero                   end each output வரி with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
நிலையான எழுத்து *
realpath_canon (எழுத்து மாறிலி *fname, முழு can_mode)
{
  எழுத்து *can_fname = canonicalize_filename_mode (fname, can_mode);
  எனில் (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      எழுத்து *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      விடுவி (can_fname);
      திரும்பு can_fname2;
    }
  திரும்பு can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
நிலையான bool
path_prefix (எழுத்து மாறிலி *prefix, எழுத்து மாறிலி *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  எனில் (!*prefix)
    திரும்பு *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  எனில் (*prefix == '/' && !prefix[1])
    திரும்பு *path == '/';

  /* Any other prefix has a non-slash portion.  */
  போது (*prefix && *path)
    {
      எனில் (*prefix != *path)
        நிறுத்து;
      prefix++;
      path++;
    }
  திரும்பு (!*prefix && (*path == '/' || !*path));
}

நிலையான bool
isdir (எழுத்து மாறிலி *path)
{
  கட்டமைப்பு stat sb;
  எனில் (stat (path, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (path));
  திரும்பு S_ISDIR (sb.st_mode);
}

நிலையான வெறுமை
print_path (எழுத்து மாறிலி *path)
{
  fputs (quote_output ? quoteN (path) : path, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
நிலையான bool
print_relative_path (எழுத்து மாறிலி *can_fname)
{
  எனில் (!quote_output)
    திரும்பு relpath (can_fname, can_relative_to, வெற்று, 0);

  size_t size;
  எனில் (ckd_mul (&size, strlen (can_relative_to), 2)
      || ckd_add (&size, size, strlen (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  எழுத்து *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  எனில் (success)
    print_path (relative_path);
  விடுவி (relative_path);
  திரும்பு success;
}

நிலையான bool
process_path (எழுத்து மாறிலி *fname, முழு can_mode)
{
  எழுத்து *can_fname = realpath_canon (fname, can_mode);
  எனில் (!can_fname)
    {
      எனில் (verbose)
        error (0, errno, "%s", quotef (fname));
      திரும்பு false;
    }

  எனில் (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  putchar (use_nuls ? '\0' : '\n');

  விடுவி (can_fname);

  எனில் (ferror (stdout))
    write_error ();

  திரும்பு true;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool ok = true;
  முழு can_mode = CAN_ALL_BUT_LAST;
  எழுத்து மாறிலி *relative_to = வெற்று;
  எழுத்து மாறிலி *relative_base = வெற்று;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது (true)
    {
      முழு c = getopt_long (argc, argv, "EeLmPqsz", longopts, வெற்று);
      எனில் (c == -1)
        நிறுத்து;
      தேர்வு (c)
        {
        நிலை 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          நிறுத்து;
        நிலை 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          நிறுத்து;
        நிலை 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          நிறுத்து;
        நிலை 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          நிறுத்து;
        நிலை 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          நிறுத்து;
        நிலை 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          நிறுத்து;
        நிலை 'q':
          verbose = false;
          நிறுத்து;
        நிலை 'z':
          use_nuls = true;
          நிறுத்து;
        நிலை RELATIVE_TO_OPTION:
          relative_to = optarg;
          நிறுத்து;
        நிலை RELATIVE_BASE_OPTION:
          relative_base = optarg;
          நிறுத்து;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  எனில் (! use_nuls && isatty (STDOUT_FILENO))
    {
      முழு qs = getenv_quoting_style ();
      எனில் (qs < 0)
        qs = shell_escape_quoting_style;
      எனில் (qs != literal_quoting_style)
        {
          set_quoting_style (வெற்று, qs);
          quote_output = true;
        }
    }

  எனில் (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  எனில் (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      எனில் (!can_relative_to)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_to));
      எனில் (need_dir && !isdir (can_relative_to))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_to));
    }
  எனில் (relative_base == relative_to)
    can_relative_base = can_relative_to;
  இல்லையெனில் எனில் (relative_base)
    {
      எழுத்து *base = realpath_canon (relative_base, can_mode);
      எனில் (!base)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_base));
      எனில் (need_dir && !isdir (base))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      எனில் (path_prefix (base, can_relative_to))
        can_relative_base = base;
      இல்லையெனில்
        {
          விடுவி (base);
          can_relative_base = can_relative_to;
          can_relative_to = வெற்று;
        }
    }

  க்கு (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
