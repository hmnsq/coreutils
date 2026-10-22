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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "canonicalize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "realpath"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

ਗਿਣਤੀ
{
  RELATIVE_ਮੰਜ਼ਿਲ_OPTION = CHAR_MAX + 1,
  RELATIVE_ਅਧਾਰ_OPTION
};

ਸਥਿਰ bool verbose = true;
ਸਥਿਰ bool logical;
ਸਥਿਰ bool use_nuls;
ਸਥਿਰ bool quote_output;
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *can_relative_ਮੰਜ਼ਿਲ;
ਸਥਿਰ ਅੱਖਰ ਅਚਲ *can_relative_ਅਧਾਰ;

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"canonicalize", no_ਦਲੀਲ, ਨਿਰਾ, 'E'},
  {"canonicalize-existing", no_ਦਲੀਲ, ਨਿਰਾ, 'e'},
  {"canonicalize-missing", no_ਦਲੀਲ, ਨਿਰਾ, 'm'},
  {"relative-to", required_ਦਲੀਲ, ਨਿਰਾ, RELATIVE_ਮੰਜ਼ਿਲ_OPTION},
  {"relative-base", required_ਦਲੀਲ, ਨਿਰਾ, RELATIVE_ਅਧਾਰ_OPTION},
  {"quiet", no_ਦਲੀਲ, ਨਿਰਾ, 'q'},
  {"strip", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"no-symlinks", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"zero", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {"logical", no_ਦਲੀਲ, ਨਿਰਾ, 'L'},
  {"physical", no_ਦਲੀਲ, ਨਿਰਾ, 'P'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... FILE...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the ਆਖਰੀ component must exist (ਮੂਲ)\
\n\
"));
      oputs (_("\
  -e, --canonicalize-existing  all components ਦਾ the ਰਸਤਾ must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing   no ਰਸਤਾ components need exist or be a directory\
\n\
"));
      oputs (_("\
  -L, --logical                resolve '..' components before symlinks\n\
"));
      oputs (_("\
  -P, --physical               resolve symlinks as encountered (ਮੂਲ)\n\
"));
      oputs (_("\
  -q, --quiet                  suppress most error messages\n\
"));
      oputs (_("\
      --relative-ਮੰਜ਼ਿਲ=DIR        print the resolved ਰਸਤਾ relative ਮੰਜ਼ਿਲ DIR\n\
"));
      oputs (_("\
      --relative-ਅਧਾਰ_2=DIR      print absolute paths unless paths ਹੇਠਾਂ DIR\n\
"));
      oputs (_("\
  -s, --strip, --no-symlinks   don't expand symlinks\n\
"));
      oputs (_("\
  -z, --zero                   ਸਿਰਾ each output ਸਤਰ with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
ਸਥਿਰ ਅੱਖਰ *
realpath_canon (ਅੱਖਰ ਅਚਲ *fname, ਪੂਰਨ can_ਬਹੁਲਕ)
{
  ਅੱਖਰ *can_fname = canonicalize_filename_ਬਹੁਲਕ (fname, can_ਬਹੁਲਕ);
  ਜੇ (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_ਬਹੁਲਕ &= ~CAN_NOLINKS;
      ਅੱਖਰ *can_fname2 = canonicalize_filename_ਬਹੁਲਕ (can_fname, can_ਬਹੁਲਕ);
      ਮੁਕਤ (can_fname);
      ਵਾਪਸ can_fname2;
    }
  ਵਾਪਸ can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_ਸ਼ੁੱਧ
ਸਥਿਰ bool
ਰਸਤਾ_ਅਗੇਤਰ (ਅੱਖਰ ਅਚਲ *ਅਗੇਤਰ, ਅੱਖਰ ਅਚਲ *ਰਸਤਾ)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  ਅਗੇਤਰ++;
  ਰਸਤਾ++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  ਜੇ (!*ਅਗੇਤਰ)
    ਵਾਪਸ *ਰਸਤਾ != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  ਜੇ (*ਅਗੇਤਰ == '/' && !ਅਗੇਤਰ[1])
    ਵਾਪਸ *ਰਸਤਾ == '/';

  /* Any other prefix has a non-slash portion.  */
  ਜਦੋਂ (*ਅਗੇਤਰ && *ਰਸਤਾ)
    {
      ਜੇ (*ਅਗੇਤਰ != *ਰਸਤਾ)
        ਰੋਕੋ;
      ਅਗੇਤਰ++;
      ਰਸਤਾ++;
    }
  ਵਾਪਸ (!*ਅਗੇਤਰ && (*ਰਸਤਾ == '/' || !*ਰਸਤਾ));
}

ਸਥਿਰ bool
isdir (ਅੱਖਰ ਅਚਲ *ਰਸਤਾ)
{
  ਬਣਤਰ stat sb;
  ਜੇ (stat (ਰਸਤਾ, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (ਰਸਤਾ));
  ਵਾਪਸ S_ISDIR (sb.st_ਬਹੁਲਕ);
}

ਸਥਿਰ ਖਾਲੀ
print_ਰਸਤਾ (ਅੱਖਰ ਅਚਲ *ਰਸਤਾ)
{
  fputs (quote_output ? quoteN (ਰਸਤਾ) : ਰਸਤਾ, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
ਸਥਿਰ bool
print_relative_ਰਸਤਾ (ਅੱਖਰ ਅਚਲ *can_fname)
{
  ਜੇ (!quote_output)
    ਵਾਪਸ relpath (can_fname, can_relative_ਮੰਜ਼ਿਲ, ਨਿਰਾ, 0);

  size_t ਮਾਪ;
  ਜੇ (ckd_mul (&ਮਾਪ, ਲੜੀ_ਲੰਬਾਈ (can_relative_ਮੰਜ਼ਿਲ), 2)
      || ckd_ਜੋੜੋ (&ਮਾਪ, ਮਾਪ, ਲੜੀ_ਲੰਬਾਈ (can_fname))
      || ckd_ਜੋੜੋ (&ਮਾਪ, ਮਾਪ, 1))
    xalloc_die ();

  ਅੱਖਰ *relative_ਰਸਤਾ = xmalloc (ਮਾਪ);
  bool success = relpath (can_fname, can_relative_ਮੰਜ਼ਿਲ, relative_ਰਸਤਾ, ਮਾਪ);
  ਜੇ (success)
    print_ਰਸਤਾ (relative_ਰਸਤਾ);
  ਮੁਕਤ (relative_ਰਸਤਾ);
  ਵਾਪਸ success;
}

ਸਥਿਰ bool
process_ਰਸਤਾ (ਅੱਖਰ ਅਚਲ *fname, ਪੂਰਨ can_ਬਹੁਲਕ)
{
  ਅੱਖਰ *can_fname = realpath_canon (fname, can_ਬਹੁਲਕ);
  ਜੇ (!can_fname)
    {
      ਜੇ (verbose)
        error (0, errno, "%s", quotef (fname));
      ਵਾਪਸ false;
    }

  ਜੇ (!can_relative_ਮੰਜ਼ਿਲ
      || (can_relative_ਅਧਾਰ && !ਰਸਤਾ_ਅਗੇਤਰ (can_relative_ਅਧਾਰ, can_fname))
      || (can_relative_ਮੰਜ਼ਿਲ && !print_relative_ਰਸਤਾ (can_fname)))
    print_ਰਸਤਾ (can_fname);

  ਅੱਖਰ_ਲਿਖੋ (use_nuls ? '\0' : '\n');

  ਮੁਕਤ (can_fname);

  ਜੇ (ferror (stdout))
    write_error ();

  ਵਾਪਸ true;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool ok = true;
  ਪੂਰਨ can_ਬਹੁਲਕ = CAN_ALL_BUT_ਆਖਰੀ;
  ਅੱਖਰ ਅਚਲ *relative_ਮੰਜ਼ਿਲ = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *relative_ਅਧਾਰ = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ (true)
    {
      ਪੂਰਨ c = getopt_long (argc, argv, "EeLmPqsz", longopts, ਨਿਰਾ);
      ਜੇ (c == -1)
        ਰੋਕੋ;
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'E':
          can_ਬਹੁਲਕ &= ~CAN_ਬਹੁਲਕ_ਨਕਾਬ;
          can_ਬਹੁਲਕ |= CAN_ALL_BUT_ਆਖਰੀ;
          ਰੋਕੋ;
        ਹਾਲਤ 'e':
          can_ਬਹੁਲਕ &= ~CAN_ਬਹੁਲਕ_ਨਕਾਬ;
          can_ਬਹੁਲਕ |= CAN_EXISTING;
          ਰੋਕੋ;
        ਹਾਲਤ 'm':
          can_ਬਹੁਲਕ &= ~CAN_ਬਹੁਲਕ_ਨਕਾਬ;
          can_ਬਹੁਲਕ |= CAN_MISSING;
          ਰੋਕੋ;
        ਹਾਲਤ 'L':
          can_ਬਹੁਲਕ |= CAN_NOLINKS;
          logical = true;
          ਰੋਕੋ;
        ਹਾਲਤ 's':
          can_ਬਹੁਲਕ |= CAN_NOLINKS;
          logical = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'P':
          can_ਬਹੁਲਕ &= ~CAN_NOLINKS;
          logical = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'q':
          verbose = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'z':
          use_nuls = true;
          ਰੋਕੋ;
        ਹਾਲਤ RELATIVE_ਮੰਜ਼ਿਲ_OPTION:
          relative_ਮੰਜ਼ਿਲ = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ RELATIVE_ਅਧਾਰ_OPTION:
          relative_ਅਧਾਰ = optarg;
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (! use_nuls && isatty (STDOUT_FILENO))
    {
      ਪੂਰਨ qs = getenv_quoting_style ();
      ਜੇ (qs < 0)
        qs = shell_escape_quoting_style;
      ਜੇ (qs != literal_quoting_style)
        {
          ਸੈੱਟ_quoting_style (ਨਿਰਾ, qs);
          quote_output = true;
        }
    }

  ਜੇ (relative_ਅਧਾਰ && !relative_ਮੰਜ਼ਿਲ)
    relative_ਮੰਜ਼ਿਲ = relative_ਅਧਾਰ;

  bool need_dir = (can_ਬਹੁਲਕ & CAN_ਬਹੁਲਕ_ਨਕਾਬ) == CAN_EXISTING;
  ਜੇ (relative_ਮੰਜ਼ਿਲ)
    {
      can_relative_ਮੰਜ਼ਿਲ = realpath_canon (relative_ਮੰਜ਼ਿਲ, can_ਬਹੁਲਕ);
      ਜੇ (!can_relative_ਮੰਜ਼ਿਲ)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_ਮੰਜ਼ਿਲ));
      ਜੇ (need_dir && !isdir (can_relative_ਮੰਜ਼ਿਲ))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_ਮੰਜ਼ਿਲ));
    }
  ਜੇ (relative_ਅਧਾਰ == relative_ਮੰਜ਼ਿਲ)
    can_relative_ਅਧਾਰ = can_relative_ਮੰਜ਼ਿਲ;
  ਨਹੀਂਤਾਂ ਜੇ (relative_ਅਧਾਰ)
    {
      ਅੱਖਰ *ਅਧਾਰ_2 = realpath_canon (relative_ਅਧਾਰ, can_ਬਹੁਲਕ);
      ਜੇ (!ਅਧਾਰ_2)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_ਅਧਾਰ));
      ਜੇ (need_dir && !isdir (ਅਧਾਰ_2))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_ਅਧਾਰ));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      ਜੇ (ਰਸਤਾ_ਅਗੇਤਰ (ਅਧਾਰ_2, can_relative_ਮੰਜ਼ਿਲ))
        can_relative_ਅਧਾਰ = ਅਧਾਰ_2;
      ਨਹੀਂਤਾਂ
        {
          ਮੁਕਤ (ਅਧਾਰ_2);
          can_relative_ਅਧਾਰ = can_relative_ਮੰਜ਼ਿਲ;
          can_relative_ਮੰਜ਼ਿਲ = ਨਿਰਾ;
        }
    }

  ਲਈ (; optind < argc; ++optind)
    ok &= process_ਰਸਤਾ (argv[optind], can_ਬਹੁਲਕ);

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
