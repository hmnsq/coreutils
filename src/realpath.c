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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "realpath"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ಎಣಿಕೆ
{
  RELATIVE_ಗಮ್ಯ_OPTION = CHAR_MAX + 1,
  RELATIVE_ತಳ_OPTION
};

ಸ್ಥಿರ bool verbose = true;
ಸ್ಥಿರ bool logical;
ಸ್ಥಿರ bool use_nuls;
ಸ್ಥಿರ bool quote_output;
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *can_relative_ಗಮ್ಯ;
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *can_relative_ತಳ;

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"canonicalize", no_ವಾದ, ಖಾಲಿ, 'E'},
  {"canonicalize-existing", no_ವಾದ, ಖಾಲಿ, 'e'},
  {"canonicalize-missing", no_ವಾದ, ಖಾಲಿ, 'm'},
  {"relative-to", required_ವಾದ, ಖಾಲಿ, RELATIVE_ಗಮ್ಯ_OPTION},
  {"relative-base", required_ವಾದ, ಖಾಲಿ, RELATIVE_ತಳ_OPTION},
  {"quiet", no_ವಾದ, ಖಾಲಿ, 'q'},
  {"strip", no_ವಾದ, ಖಾಲಿ, 's'},
  {"no-symlinks", no_ವಾದ, ಖಾಲಿ, 's'},
  {"zero", no_ವಾದ, ಖಾಲಿ, 'z'},
  {"logical", no_ವಾದ, ಖಾಲಿ, 'L'},
  {"physical", no_ವಾದ, ಖಾಲಿ, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... FILE...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the ಕೊನೆಯ component must exist (ಪೂರ್ವನಿಯೋಜಿತ)\
\n\
"));
      oputs (_("\
  -e, --canonicalize-existing  all components ದ the ಮಾರ್ಗ must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing   no ಮಾರ್ಗ components need exist or be a directory\
\n\
"));
      oputs (_("\
  -L, --logical                resolve '..' components before symlinks\n\
"));
      oputs (_("\
  -P, --physical               resolve symlinks as encountered (ಪೂರ್ವನಿಯೋಜಿತ)\n\
"));
      oputs (_("\
  -q, --quiet                  suppress most error messages\n\
"));
      oputs (_("\
      --relative-ಗಮ್ಯ=DIR        print the resolved ಮಾರ್ಗ relative ಗಮ್ಯ DIR\n\
"));
      oputs (_("\
      --relative-ತಳ_2=DIR      print absolute paths unless paths ಕೆಳಗೆ DIR\n\
"));
      oputs (_("\
  -s, --strip, --no-symlinks   don't expand symlinks\n\
"));
      oputs (_("\
  -z, --zero                   ಕೊನೆ each output ಸಾಲು with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
realpath_canon (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fname, ಪೂರ್ಣಾಂಕ can_ಬಹುಲಕ)
{
  ಅಕ್ಷರ *can_fname = canonicalize_filename_ಬಹುಲಕ (fname, can_ಬಹುಲಕ);
  ಆದರೆ (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_ಬಹುಲಕ &= ~CAN_NOLINKS;
      ಅಕ್ಷರ *can_fname2 = canonicalize_filename_ಬಹುಲಕ (can_fname, can_ಬಹುಲಕ);
      ಬಿಡುಗಡೆ (can_fname);
      ಹಿಂತಿರುಗಿ can_fname2;
    }
  ಹಿಂತಿರುಗಿ can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_ಶುದ್ಧ
ಸ್ಥಿರ bool
ಮಾರ್ಗ_ಪೂರ್ವಪ್ರತ್ಯಯ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಪೂರ್ವಪ್ರತ್ಯಯ, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮಾರ್ಗ)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  ಪೂರ್ವಪ್ರತ್ಯಯ++;
  ಮಾರ್ಗ++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  ಆದರೆ (!*ಪೂರ್ವಪ್ರತ್ಯಯ)
    ಹಿಂತಿರುಗಿ *ಮಾರ್ಗ != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  ಆದರೆ (*ಪೂರ್ವಪ್ರತ್ಯಯ == '/' && !ಪೂರ್ವಪ್ರತ್ಯಯ[1])
    ಹಿಂತಿರುಗಿ *ಮಾರ್ಗ == '/';

  /* Any other prefix has a non-slash portion.  */
  ಆಗ (*ಪೂರ್ವಪ್ರತ್ಯಯ && *ಮಾರ್ಗ)
    {
      ಆದರೆ (*ಪೂರ್ವಪ್ರತ್ಯಯ != *ಮಾರ್ಗ)
        ನಿಲ್ಲಿಸು;
      ಪೂರ್ವಪ್ರತ್ಯಯ++;
      ಮಾರ್ಗ++;
    }
  ಹಿಂತಿರುಗಿ (!*ಪೂರ್ವಪ್ರತ್ಯಯ && (*ಮಾರ್ಗ == '/' || !*ಮಾರ್ಗ));
}

ಸ್ಥಿರ bool
isdir (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮಾರ್ಗ)
{
  ರಚನೆ stat sb;
  ಆದರೆ (stat (ಮಾರ್ಗ, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (ಮಾರ್ಗ));
  ಹಿಂತಿರುಗಿ S_ISDIR (sb.st_ಬಹುಲಕ);
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_ಮಾರ್ಗ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಮಾರ್ಗ)
{
  fputs (quote_output ? quoteN (ಮಾರ್ಗ) : ಮಾರ್ಗ, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
ಸ್ಥಿರ bool
print_relative_ಮಾರ್ಗ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *can_fname)
{
  ಆದರೆ (!quote_output)
    ಹಿಂತಿರುಗಿ relpath (can_fname, can_relative_ಗಮ್ಯ, ಖಾಲಿ, 0);

  size_t ಗಾತ್ರ_2;
  ಆದರೆ (ckd_mul (&ಗಾತ್ರ_2, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (can_relative_ಗಮ್ಯ), 2)
      || ckd_ಕೂಡಿಸು (&ಗಾತ್ರ_2, ಗಾತ್ರ_2, ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (can_fname))
      || ckd_ಕೂಡಿಸು (&ಗಾತ್ರ_2, ಗಾತ್ರ_2, 1))
    xalloc_die ();

  ಅಕ್ಷರ *relative_ಮಾರ್ಗ = xmalloc (ಗಾತ್ರ_2);
  bool success = relpath (can_fname, can_relative_ಗಮ್ಯ, relative_ಮಾರ್ಗ, ಗಾತ್ರ_2);
  ಆದರೆ (success)
    print_ಮಾರ್ಗ (relative_ಮಾರ್ಗ);
  ಬಿಡುಗಡೆ (relative_ಮಾರ್ಗ);
  ಹಿಂತಿರುಗಿ success;
}

ಸ್ಥಿರ bool
process_ಮಾರ್ಗ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *fname, ಪೂರ್ಣಾಂಕ can_ಬಹುಲಕ)
{
  ಅಕ್ಷರ *can_fname = realpath_canon (fname, can_ಬಹುಲಕ);
  ಆದರೆ (!can_fname)
    {
      ಆದರೆ (verbose)
        error (0, errno, "%s", quotef (fname));
      ಹಿಂತಿರುಗಿ false;
    }

  ಆದರೆ (!can_relative_ಗಮ್ಯ
      || (can_relative_ತಳ && !ಮಾರ್ಗ_ಪೂರ್ವಪ್ರತ್ಯಯ (can_relative_ತಳ, can_fname))
      || (can_relative_ಗಮ್ಯ && !print_relative_ಮಾರ್ಗ (can_fname)))
    print_ಮಾರ್ಗ (can_fname);

  ಅಕ್ಷರ_ಬರೆ (use_nuls ? '\0' : '\n');

  ಬಿಡುಗಡೆ (can_fname);

  ಆದರೆ (ferror (stdout))
    write_error ();

  ಹಿಂತಿರುಗಿ true;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool ok = true;
  ಪೂರ್ಣಾಂಕ can_ಬಹುಲಕ = CAN_ALL_BUT_ಕೊನೆಯ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *relative_ಗಮ್ಯ = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *relative_ತಳ = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv, "EeLmPqsz", longopts, ಖಾಲಿ);
      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'E':
          can_ಬಹುಲಕ &= ~CAN_ಬಹುಲಕ_ಮುಖವಾಡ;
          can_ಬಹುಲಕ |= CAN_ALL_BUT_ಕೊನೆಯ;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'e':
          can_ಬಹುಲಕ &= ~CAN_ಬಹುಲಕ_ಮುಖವಾಡ;
          can_ಬಹುಲಕ |= CAN_EXISTING;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'm':
          can_ಬಹುಲಕ &= ~CAN_ಬಹುಲಕ_ಮುಖವಾಡ;
          can_ಬಹುಲಕ |= CAN_MISSING;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'L':
          can_ಬಹುಲಕ |= CAN_NOLINKS;
          logical = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 's':
          can_ಬಹುಲಕ |= CAN_NOLINKS;
          logical = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'P':
          can_ಬಹುಲಕ &= ~CAN_NOLINKS;
          logical = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'q':
          verbose = false;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'z':
          use_nuls = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ RELATIVE_ಗಮ್ಯ_OPTION:
          relative_ಗಮ್ಯ = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ RELATIVE_ತಳ_OPTION:
          relative_ತಳ = optarg;
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (! use_nuls && isatty (STDOUT_FILENO))
    {
      ಪೂರ್ಣಾಂಕ qs = getenv_quoting_style ();
      ಆದರೆ (qs < 0)
        qs = shell_escape_quoting_style;
      ಆದರೆ (qs != literal_quoting_style)
        {
          ಗಣ_quoting_style (ಖಾಲಿ, qs);
          quote_output = true;
        }
    }

  ಆದರೆ (relative_ತಳ && !relative_ಗಮ್ಯ)
    relative_ಗಮ್ಯ = relative_ತಳ;

  bool need_dir = (can_ಬಹುಲಕ & CAN_ಬಹುಲಕ_ಮುಖವಾಡ) == CAN_EXISTING;
  ಆದರೆ (relative_ಗಮ್ಯ)
    {
      can_relative_ಗಮ್ಯ = realpath_canon (relative_ಗಮ್ಯ, can_ಬಹುಲಕ);
      ಆದರೆ (!can_relative_ಗಮ್ಯ)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_ಗಮ್ಯ));
      ಆದರೆ (need_dir && !isdir (can_relative_ಗಮ್ಯ))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_ಗಮ್ಯ));
    }
  ಆದರೆ (relative_ತಳ == relative_ಗಮ್ಯ)
    can_relative_ತಳ = can_relative_ಗಮ್ಯ;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (relative_ತಳ)
    {
      ಅಕ್ಷರ *ತಳ_2 = realpath_canon (relative_ತಳ, can_ಬಹುಲಕ);
      ಆದರೆ (!ತಳ_2)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_ತಳ));
      ಆದರೆ (need_dir && !isdir (ತಳ_2))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_ತಳ));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      ಆದರೆ (ಮಾರ್ಗ_ಪೂರ್ವಪ್ರತ್ಯಯ (ತಳ_2, can_relative_ಗಮ್ಯ))
        can_relative_ತಳ = ತಳ_2;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಬಿಡುಗಡೆ (ತಳ_2);
          can_relative_ತಳ = can_relative_ಗಮ್ಯ;
          can_relative_ಗಮ್ಯ = ಖಾಲಿ;
        }
    }

  ಗಾಗಿ (; optind < argc; ++optind)
    ok &= process_ಮಾರ್ಗ (argv[optind], can_ಬಹುಲಕ);

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
