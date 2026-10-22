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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "canonicalize.h"
#അന്തര്ഭാവ "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "realpath"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

എണ്ണൽ
{
  RELATIVE_TO_OPTION = CHAR_MAX + 1,
  RELATIVE_BASE_OPTION
};

സ്ഥിതം bool verbose = true;
സ്ഥിതം bool logical;
സ്ഥിതം bool use_nuls;
സ്ഥിതം bool quote_output;
സ്ഥിതം അക്ഷരം സ്ഥിരം *can_relative_to;
സ്ഥിതം അക്ഷരം സ്ഥിരം *can_relative_base;

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"canonicalize", no_argument, ശൂന്യ, 'E'},
  {"canonicalize-existing", no_argument, ശൂന്യ, 'e'},
  {"canonicalize-missing", no_argument, ശൂന്യ, 'm'},
  {"relative-to", required_argument, ശൂന്യ, RELATIVE_TO_OPTION},
  {"relative-base", required_argument, ശൂന്യ, RELATIVE_BASE_OPTION},
  {"quiet", no_argument, ശൂന്യ, 'q'},
  {"strip", no_argument, ശൂന്യ, 's'},
  {"no-symlinks", no_argument, ശൂന്യ, 's'},
  {"zero", no_argument, ശൂന്യ, 'z'},
  {"logical", no_argument, ശൂന്യ, 'L'},
  {"physical", no_argument, ശൂന്യ, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the last component must exist (സ്ഥിരസ്ഥിതി)\
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
  -P, --physical               resolve symlinks as encountered (സ്ഥിരസ്ഥിതി)\n\
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
  -z, --zero                   end each output വരി with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
സ്ഥിതം അക്ഷരം *
realpath_canon (അക്ഷരം സ്ഥിരം *fname, പൂർണ്ണസംഖ്യ can_mode)
{
  അക്ഷരം *can_fname = canonicalize_filename_mode (fname, can_mode);
  എങ്കിൽ (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      അക്ഷരം *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      സ്വതന്ത്രം (can_fname);
      തിരികെ can_fname2;
    }
  തിരികെ can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
സ്ഥിതം bool
path_prefix (അക്ഷരം സ്ഥിരം *prefix, അക്ഷരം സ്ഥിരം *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  എങ്കിൽ (!*prefix)
    തിരികെ *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  എങ്കിൽ (*prefix == '/' && !prefix[1])
    തിരികെ *path == '/';

  /* Any other prefix has a non-slash portion.  */
  ആകുമ്പോൾ (*prefix && *path)
    {
      എങ്കിൽ (*prefix != *path)
        നിർത്തുക;
      prefix++;
      path++;
    }
  തിരികെ (!*prefix && (*path == '/' || !*path));
}

സ്ഥിതം bool
isdir (അക്ഷരം സ്ഥിരം *path)
{
  ഘടന stat sb;
  എങ്കിൽ (stat (path, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (path));
  തിരികെ S_ISDIR (sb.st_mode);
}

സ്ഥിതം ശൂന്യം
print_path (അക്ഷരം സ്ഥിരം *path)
{
  fputs (quote_output ? quoteN (path) : path, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
സ്ഥിതം bool
print_relative_path (അക്ഷരം സ്ഥിരം *can_fname)
{
  എങ്കിൽ (!quote_output)
    തിരികെ relpath (can_fname, can_relative_to, ശൂന്യ, 0);

  size_t size;
  എങ്കിൽ (ckd_mul (&size, strlen (can_relative_to), 2)
      || ckd_add (&size, size, strlen (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  അക്ഷരം *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  എങ്കിൽ (success)
    print_path (relative_path);
  സ്വതന്ത്രം (relative_path);
  തിരികെ success;
}

സ്ഥിതം bool
process_path (അക്ഷരം സ്ഥിരം *fname, പൂർണ്ണസംഖ്യ can_mode)
{
  അക്ഷരം *can_fname = realpath_canon (fname, can_mode);
  എങ്കിൽ (!can_fname)
    {
      എങ്കിൽ (verbose)
        error (0, errno, "%s", quotef (fname));
      തിരികെ false;
    }

  എങ്കിൽ (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  putchar (use_nuls ? '\0' : '\n');

  സ്വതന്ത്രം (can_fname);

  എങ്കിൽ (ferror (stdout))
    write_error ();

  തിരികെ true;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  bool ok = true;
  പൂർണ്ണസംഖ്യ can_mode = CAN_ALL_BUT_LAST;
  അക്ഷരം സ്ഥിരം *relative_to = ശൂന്യ;
  അക്ഷരം സ്ഥിരം *relative_base = ശൂന്യ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ (true)
    {
      പൂർണ്ണസംഖ്യ c = getopt_long (argc, argv, "EeLmPqsz", longopts, ശൂന്യ);
      എങ്കിൽ (c == -1)
        നിർത്തുക;
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          നിർത്തുക;
        അവസ്ഥ 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          നിർത്തുക;
        അവസ്ഥ 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          നിർത്തുക;
        അവസ്ഥ 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          നിർത്തുക;
        അവസ്ഥ 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          നിർത്തുക;
        അവസ്ഥ 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          നിർത്തുക;
        അവസ്ഥ 'q':
          verbose = false;
          നിർത്തുക;
        അവസ്ഥ 'z':
          use_nuls = true;
          നിർത്തുക;
        അവസ്ഥ RELATIVE_TO_OPTION:
          relative_to = optarg;
          നിർത്തുക;
        അവസ്ഥ RELATIVE_BASE_OPTION:
          relative_base = optarg;
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (! use_nuls && isatty (STDOUT_FILENO))
    {
      പൂർണ്ണസംഖ്യ qs = getenv_quoting_style ();
      എങ്കിൽ (qs < 0)
        qs = shell_escape_quoting_style;
      എങ്കിൽ (qs != literal_quoting_style)
        {
          set_quoting_style (ശൂന്യ, qs);
          quote_output = true;
        }
    }

  എങ്കിൽ (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  എങ്കിൽ (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      എങ്കിൽ (!can_relative_to)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_to));
      എങ്കിൽ (need_dir && !isdir (can_relative_to))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_to));
    }
  എങ്കിൽ (relative_base == relative_to)
    can_relative_base = can_relative_to;
  അല്ലെങ്കിൽ എങ്കിൽ (relative_base)
    {
      അക്ഷരം *base = realpath_canon (relative_base, can_mode);
      എങ്കിൽ (!base)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_base));
      എങ്കിൽ (need_dir && !isdir (base))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      എങ്കിൽ (path_prefix (base, can_relative_to))
        can_relative_base = base;
      അല്ലെങ്കിൽ
        {
          സ്വതന്ത്രം (base);
          can_relative_base = can_relative_to;
          can_relative_to = ശൂന്യ;
        }
    }

  വേണ്ടി (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
