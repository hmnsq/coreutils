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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "canonicalize.h"
#અન્તર્ભાવ "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "realpath"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ગણના
{
  RELATIVE_TO_OPTION = CHAR_MAX + 1,
  RELATIVE_BASE_OPTION
};

સ્થિર bool verbose = true;
સ્થિર bool logical;
સ્થિર bool use_nuls;
સ્થિર bool quote_output;
સ્થિર અક્ષર અચલ *can_relative_to;
સ્થિર અક્ષર અચલ *can_relative_base;

સ્થિર સંરચના option અચલ longopts[] =
{
  {"canonicalize", no_argument, ખાલી, 'E'},
  {"canonicalize-existing", no_argument, ખાલી, 'e'},
  {"canonicalize-missing", no_argument, ખાલી, 'm'},
  {"relative-to", required_argument, ખાલી, RELATIVE_TO_OPTION},
  {"relative-base", required_argument, ખાલી, RELATIVE_BASE_OPTION},
  {"quiet", no_argument, ખાલી, 'q'},
  {"strip", no_argument, ખાલી, 's'},
  {"no-symlinks", no_argument, ખાલી, 's'},
  {"zero", no_argument, ખાલી, 'z'},
  {"logical", no_argument, ખાલી, 'L'},
  {"physical", no_argument, ખાલી, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the last component must exist (મૂળભૂત)\
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
  -P, --physical               resolve symlinks as encountered (મૂળભૂત)\n\
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
  -z, --zero                   end each output લીટી with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
સ્થિર અક્ષર *
realpath_canon (અક્ષર અચલ *fname, પૂર્ણાંક can_mode)
{
  અક્ષર *can_fname = canonicalize_filename_mode (fname, can_mode);
  જો (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      અક્ષર *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      મુક્ત (can_fname);
      પાછા can_fname2;
    }
  પાછા can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
સ્થિર bool
path_prefix (અક્ષર અચલ *prefix, અક્ષર અચલ *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  જો (!*prefix)
    પાછા *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  જો (*prefix == '/' && !prefix[1])
    પાછા *path == '/';

  /* Any other prefix has a non-slash portion.  */
  જ્યારે (*prefix && *path)
    {
      જો (*prefix != *path)
        તોડો;
      prefix++;
      path++;
    }
  પાછા (!*prefix && (*path == '/' || !*path));
}

સ્થિર bool
isdir (અક્ષર અચલ *path)
{
  સંરચના stat sb;
  જો (stat (path, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (path));
  પાછા S_ISDIR (sb.st_mode);
}

સ્થિર શૂન્ય
print_path (અક્ષર અચલ *path)
{
  fputs (quote_output ? quoteN (path) : path, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
સ્થિર bool
print_relative_path (અક્ષર અચલ *can_fname)
{
  જો (!quote_output)
    પાછા relpath (can_fname, can_relative_to, ખાલી, 0);

  size_t size;
  જો (ckd_mul (&size, strlen (can_relative_to), 2)
      || ckd_add (&size, size, strlen (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  અક્ષર *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  જો (success)
    print_path (relative_path);
  મુક્ત (relative_path);
  પાછા success;
}

સ્થિર bool
process_path (અક્ષર અચલ *fname, પૂર્ણાંક can_mode)
{
  અક્ષર *can_fname = realpath_canon (fname, can_mode);
  જો (!can_fname)
    {
      જો (verbose)
        error (0, errno, "%s", quotef (fname));
      પાછા false;
    }

  જો (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  putchar (use_nuls ? '\0' : '\n');

  મુક્ત (can_fname);

  જો (ferror (stdout))
    write_error ();

  પાછા true;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  bool ok = true;
  પૂર્ણાંક can_mode = CAN_ALL_BUT_LAST;
  અક્ષર અચલ *relative_to = ખાલી;
  અક્ષર અચલ *relative_base = ખાલી;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે (true)
    {
      પૂર્ણાંક c = getopt_long (argc, argv, "EeLmPqsz", longopts, ખાલી);
      જો (c == -1)
        તોડો;
      પસંદગી (c)
        {
        સ્થિતિ 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          તોડો;
        સ્થિતિ 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          તોડો;
        સ્થિતિ 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          તોડો;
        સ્થિતિ 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          તોડો;
        સ્થિતિ 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          તોડો;
        સ્થિતિ 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          તોડો;
        સ્થિતિ 'q':
          verbose = false;
          તોડો;
        સ્થિતિ 'z':
          use_nuls = true;
          તોડો;
        સ્થિતિ RELATIVE_TO_OPTION:
          relative_to = optarg;
          તોડો;
        સ્થિતિ RELATIVE_BASE_OPTION:
          relative_base = optarg;
          તોડો;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  જો (! use_nuls && isatty (STDOUT_FILENO))
    {
      પૂર્ણાંક qs = getenv_quoting_style ();
      જો (qs < 0)
        qs = shell_escape_quoting_style;
      જો (qs != literal_quoting_style)
        {
          set_quoting_style (ખાલી, qs);
          quote_output = true;
        }
    }

  જો (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  જો (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      જો (!can_relative_to)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_to));
      જો (need_dir && !isdir (can_relative_to))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_to));
    }
  જો (relative_base == relative_to)
    can_relative_base = can_relative_to;
  નહીંતર જો (relative_base)
    {
      અક્ષર *base = realpath_canon (relative_base, can_mode);
      જો (!base)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_base));
      જો (need_dir && !isdir (base))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      જો (path_prefix (base, can_relative_to))
        can_relative_base = base;
      નહીંતર
        {
          મુક્ત (base);
          can_relative_base = can_relative_to;
          can_relative_to = ખાલી;
        }
    }

  માટે (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
