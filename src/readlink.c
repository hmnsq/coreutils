/* readlink -- display value of a symbolic link.
   Copyright (C) 2002-2026 Free Software Foundation, Inc.

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

/* Written by Dmitry V. Levin */

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "canonicalize.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "readlink"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
ਸਥਿਰ bool no_newline;

/* If true, report error messages.  */
ਸਥਿਰ bool verbose;

/* If true, quote output according to the selected quoting style.  */
ਸਥਿਰ bool quote_output;

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"canonicalize", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {"canonicalize-existing", no_ਦਲੀਲ, ਨਿਰਾ, 'e'},
  {"canonicalize-missing", no_ਦਲੀਲ, ਨਿਰਾ, 'm'},
  {"no-newline", no_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"quiet", no_ਦਲੀਲ, ਨਿਰਾ, 'q'},
  {"silent", no_ਦਲੀਲ, ਨਿਰਾ, 's'},
  {"verbose", no_ਦਲੀਲ, ਨਿਰਾ, 'v'},
  {"zero", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
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
      fputs (_("Print value of a symbolic link or canonical file name\n\n"),
             stdout);
      oputs (_("\
  -f, --canonicalize\n\
         canonicalize by following every symlink\n\
         ਵਿੱਚ every component ਦਾ the given name recursively;\n\
         all but the ਆਖਰੀ component must exist\n\
"));
      oputs (_("\
  -e, --canonicalize-existing\n\
         canonicalize by following every symlink\n\
         ਵਿੱਚ every component ਦਾ the given name recursively;\n\
         all components must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing\n\
         canonicalize by following every symlink\n\
         ਵਿੱਚ every component ਦਾ the given name recursively,\n\
         without requirements on components existence\n\
"));
      oputs (_("\
  -n, --no-newline\n\
         ਕਰੋ not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by ਮੂਲ ਜੇ POSIXLY_CORRECT is not ਸੈੱਟ_2)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by ਮੂਲ ਜੇ POSIXLY_CORRECT is ਸੈੱਟ_2)\n\
"));
      oputs (_("\
  -z, --zero\n\
         ਸਿਰਾ each output ਸਤਰ with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  /* If not -1, use this method to canonicalize.  */
  ਪੂਰਨ can_ਬਹੁਲਕ = -1;
  ਪੂਰਨ ਸਥਿਤੀ = EXIT_SUCCESS;
  ਪੂਰਨ optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'e':
          can_ਬਹੁਲਕ = CAN_EXISTING;
          ਰੋਕੋ;
        ਹਾਲਤ 'f':
          can_ਬਹੁਲਕ = CAN_ALL_BUT_ਆਖਰੀ;
          ਰੋਕੋ;
        ਹਾਲਤ 'm':
          can_ਬਹੁਲਕ = CAN_MISSING;
          ਰੋਕੋ;
        ਹਾਲਤ 'n':
          no_newline = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'q':
        ਹਾਲਤ 's':
          verbose = false;
          ਰੋਕੋ;
        ਹਾਲਤ 'v':
          verbose = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'z':
          use_nuls = true;
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

  ਜੇ (argc - optind > 1)
    {
      ਜੇ (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
    }

  ਜੇ (!use_nuls && isatty (STDOUT_FILENO))
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

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  ਜੇ (getenv ("POSIXLY_CORRECT") != ਨਿਰਾ)
    verbose = true;

  ਲਈ (; optind < argc; ++optind)
    {
      ਅੱਖਰ ਅਚਲ *fname = argv[optind];
      ਅੱਖਰ *ਮੁੱਲ = (can_ਬਹੁਲਕ != -1
                     ? canonicalize_filename_ਬਹੁਲਕ (fname, can_ਬਹੁਲਕ)
                     : areadlink_with_ਮਾਪ (fname, 63));
      ਜੇ (ਮੁੱਲ)
        {
          fputs (quote_output ? quoteN (ਮੁੱਲ) : ਮੁੱਲ, stdout);
          ਜੇ (! no_newline)
            ਅੱਖਰ_ਲਿਖੋ (use_nuls ? '\0' : '\n');
          ਮੁਕਤ (ਮੁੱਲ);
          ਜੇ (ferror (stdout))
            write_error ();
        }
      ਨਹੀਂਤਾਂ
        {
          ਸਥਿਤੀ = EXIT_FAILURE;
          ਜੇ (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  ਵਾਪਸ ਸਥਿਤੀ;
}
