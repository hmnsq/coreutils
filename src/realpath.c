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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "canonicalize.h"
#ଅନ୍ତର୍ଭାଵ "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "realpath"

#define AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ଗଣନା
{
  RELATIVE_TO_OPTION = CHAR_MAX + 1,
  RELATIVE_BASE_OPTION
};

ସ୍ଥିର bool verbose = true;
ସ୍ଥିର bool logical;
ସ୍ଥିର bool use_nuls;
ସ୍ଥିର bool quote_output;
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *can_relative_to;
ସ୍ଥିର ଅକ୍ଷର ଅଚଳ *can_relative_base;

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"canonicalize", no_argument, ଖାଲି, 'E'},
  {"canonicalize-existing", no_argument, ଖାଲି, 'e'},
  {"canonicalize-missing", no_argument, ଖାଲି, 'm'},
  {"relative-to", required_argument, ଖାଲି, RELATIVE_TO_OPTION},
  {"relative-base", required_argument, ଖାଲି, RELATIVE_BASE_OPTION},
  {"quiet", no_argument, ଖାଲି, 'q'},
  {"strip", no_argument, ଖାଲି, 's'},
  {"no-symlinks", no_argument, ଖାଲି, 's'},
  {"zero", no_argument, ଖାଲି, 'z'},
  {"logical", no_argument, ଖାଲି, 'L'},
  {"physical", no_argument, ଖାଲି, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("Usage: %s [OPTION]... FILE...\n"), program_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the last component must exist (ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\
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
  -P, --physical               resolve symlinks as encountered (ପୂର୍ବନିର୍ଦ୍ଧାରିତ)\n\
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
  -z, --zero                   end each output ଧାଡ଼ି with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
ସ୍ଥିର ଅକ୍ଷର *
realpath_canon (ଅକ୍ଷର ଅଚଳ *fname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା can_mode)
{
  ଅକ୍ଷର *can_fname = canonicalize_filename_mode (fname, can_mode);
  ଯଦି (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_mode &= ~CAN_NOLINKS;
      ଅକ୍ଷର *can_fname2 = canonicalize_filename_mode (can_fname, can_mode);
      ମୁକ୍ତ (can_fname);
      ଫେରନ୍ତୁ can_fname2;
    }
  ଫେରନ୍ତୁ can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_PURE
ସ୍ଥିର bool
path_prefix (ଅକ୍ଷର ଅଚଳ *prefix, ଅକ୍ଷର ଅଚଳ *path)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  prefix++;
  path++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  ଯଦି (!*prefix)
    ଫେରନ୍ତୁ *path != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  ଯଦି (*prefix == '/' && !prefix[1])
    ଫେରନ୍ତୁ *path == '/';

  /* Any other prefix has a non-slash portion.  */
  ଯେତେବେଳେ (*prefix && *path)
    {
      ଯଦି (*prefix != *path)
        ଭାଙ୍ଗ;
      prefix++;
      path++;
    }
  ଫେରନ୍ତୁ (!*prefix && (*path == '/' || !*path));
}

ସ୍ଥିର bool
isdir (ଅକ୍ଷର ଅଚଳ *path)
{
  ସଂରଚନା stat sb;
  ଯଦି (stat (path, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (path));
  ଫେରନ୍ତୁ S_ISDIR (sb.st_mode);
}

ସ୍ଥିର ଶୂନ୍ୟ
print_path (ଅକ୍ଷର ଅଚଳ *path)
{
  fputs (quote_output ? quoteN (path) : path, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
ସ୍ଥିର bool
print_relative_path (ଅକ୍ଷର ଅଚଳ *can_fname)
{
  ଯଦି (!quote_output)
    ଫେରନ୍ତୁ relpath (can_fname, can_relative_to, ଖାଲି, 0);

  size_t size;
  ଯଦି (ckd_mul (&size, strlen (can_relative_to), 2)
      || ckd_add (&size, size, strlen (can_fname))
      || ckd_add (&size, size, 1))
    xalloc_die ();

  ଅକ୍ଷର *relative_path = xmalloc (size);
  bool success = relpath (can_fname, can_relative_to, relative_path, size);
  ଯଦି (success)
    print_path (relative_path);
  ମୁକ୍ତ (relative_path);
  ଫେରନ୍ତୁ success;
}

ସ୍ଥିର bool
process_path (ଅକ୍ଷର ଅଚଳ *fname, ପୂର୍ଣ୍ଣସଂଖ୍ୟା can_mode)
{
  ଅକ୍ଷର *can_fname = realpath_canon (fname, can_mode);
  ଯଦି (!can_fname)
    {
      ଯଦି (verbose)
        error (0, errno, "%s", quotef (fname));
      ଫେରନ୍ତୁ false;
    }

  ଯଦି (!can_relative_to
      || (can_relative_base && !path_prefix (can_relative_base, can_fname))
      || (can_relative_to && !print_relative_path (can_fname)))
    print_path (can_fname);

  putchar (use_nuls ? '\0' : '\n');

  ମୁକ୍ତ (can_fname);

  ଯଦି (ferror (stdout))
    write_error ();

  ଫେରନ୍ତୁ true;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  bool ok = true;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା can_mode = CAN_ALL_BUT_LAST;
  ଅକ୍ଷର ଅଚଳ *relative_to = ଖାଲି;
  ଅକ୍ଷର ଅଚଳ *relative_base = ଖାଲି;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ (true)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = getopt_long (argc, argv, "EeLmPqsz", longopts, ଖାଲି);
      ଯଦି (c == -1)
        ଭାଙ୍ଗ;
      ବାଛ (c)
        {
        ସ୍ଥିତି 'E':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_ALL_BUT_LAST;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'e':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_EXISTING;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'm':
          can_mode &= ~CAN_MODE_MASK;
          can_mode |= CAN_MISSING;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'L':
          can_mode |= CAN_NOLINKS;
          logical = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 's':
          can_mode |= CAN_NOLINKS;
          logical = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'P':
          can_mode &= ~CAN_NOLINKS;
          logical = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'q':
          verbose = false;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'z':
          use_nuls = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି RELATIVE_TO_OPTION:
          relative_to = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି RELATIVE_BASE_OPTION:
          relative_base = optarg;
          ଭାଙ୍ଗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (! use_nuls && isatty (STDOUT_FILENO))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା qs = getenv_quoting_style ();
      ଯଦି (qs < 0)
        qs = shell_escape_quoting_style;
      ଯଦି (qs != literal_quoting_style)
        {
          set_quoting_style (ଖାଲି, qs);
          quote_output = true;
        }
    }

  ଯଦି (relative_base && !relative_to)
    relative_to = relative_base;

  bool need_dir = (can_mode & CAN_MODE_MASK) == CAN_EXISTING;
  ଯଦି (relative_to)
    {
      can_relative_to = realpath_canon (relative_to, can_mode);
      ଯଦି (!can_relative_to)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_to));
      ଯଦି (need_dir && !isdir (can_relative_to))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_to));
    }
  ଯଦି (relative_base == relative_to)
    can_relative_base = can_relative_to;
  ଅନ୍ୟଥା ଯଦି (relative_base)
    {
      ଅକ୍ଷର *base = realpath_canon (relative_base, can_mode);
      ଯଦି (!base)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_base));
      ଯଦି (need_dir && !isdir (base))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_base));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      ଯଦି (path_prefix (base, can_relative_to))
        can_relative_base = base;
      ଅନ୍ୟଥା
        {
          ମୁକ୍ତ (base);
          can_relative_base = can_relative_to;
          can_relative_to = ଖାଲି;
        }
    }

  ପାଇଁ (; optind < argc; ++optind)
    ok &= process_path (argv[optind], can_mode);

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
