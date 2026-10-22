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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "canonicalize.h"
#അന്തര്ഭാവ "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "readlink"

#define AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
സ്ഥിതം bool no_newline;

/* If true, report error messages.  */
സ്ഥിതം bool verbose;

/* If true, quote output according to the selected quoting style.  */
സ്ഥിതം bool quote_output;

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"canonicalize", no_argument, ശൂന്യ, 'f'},
  {"canonicalize-existing", no_argument, ശൂന്യ, 'e'},
  {"canonicalize-missing", no_argument, ശൂന്യ, 'm'},
  {"no-newline", no_argument, ശൂന്യ, 'n'},
  {"quiet", no_argument, ശൂന്യ, 'q'},
  {"silent", no_argument, ശൂന്യ, 's'},
  {"verbose", no_argument, ശൂന്യ, 'v'},
  {"zero", no_argument, ശൂന്യ, 'z'},
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
      fputs (_("Print value of a symbolic link or canonical file name\n\n"),
             stdout);
      oputs (_("\
  -f, --canonicalize\n\
         canonicalize by following every symlink\n\
         in every component of the given name recursively;\n\
         all but the last component must exist\n\
"));
      oputs (_("\
  -e, --canonicalize-existing\n\
         canonicalize by following every symlink\n\
         in every component of the given name recursively;\n\
         all components must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing\n\
         canonicalize by following every symlink\n\
         in every component of the given name recursively,\n\
         without requirements on components existence\n\
"));
      oputs (_("\
  -n, --no-newline\n\
         ചെയ്യുക not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by സ്ഥിരസ്ഥിതി എങ്കിൽ POSIXLY_CORRECT is not set)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by സ്ഥിരസ്ഥിതി എങ്കിൽ POSIXLY_CORRECT is set)\n\
"));
      oputs (_("\
  -z, --zero\n\
         end each output വരി with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  /* If not -1, use this method to canonicalize.  */
  പൂർണ്ണസംഖ്യ can_mode = -1;
  പൂർണ്ണസംഖ്യ status = EXIT_SUCCESS;
  പൂർണ്ണസംഖ്യ optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'e':
          can_mode = CAN_EXISTING;
          നിർത്തുക;
        അവസ്ഥ 'f':
          can_mode = CAN_ALL_BUT_LAST;
          നിർത്തുക;
        അവസ്ഥ 'm':
          can_mode = CAN_MISSING;
          നിർത്തുക;
        അവസ്ഥ 'n':
          no_newline = true;
          നിർത്തുക;
        അവസ്ഥ 'q':
        അവസ്ഥ 's':
          verbose = false;
          നിർത്തുക;
        അവസ്ഥ 'v':
          verbose = true;
          നിർത്തുക;
        അവസ്ഥ 'z':
          use_nuls = true;
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

  എങ്കിൽ (argc - optind > 1)
    {
      എങ്കിൽ (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
    }

  എങ്കിൽ (!use_nuls && isatty (STDOUT_FILENO))
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

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  എങ്കിൽ (getenv ("POSIXLY_CORRECT") != ശൂന്യ)
    verbose = true;

  വേണ്ടി (; optind < argc; ++optind)
    {
      അക്ഷരം സ്ഥിരം *fname = argv[optind];
      അക്ഷരം *മൂല്യം = (can_mode != -1
                     ? canonicalize_filename_mode (fname, can_mode)
                     : areadlink_with_size (fname, 63));
      എങ്കിൽ (മൂല്യം)
        {
          fputs (quote_output ? quoteN (മൂല്യം) : മൂല്യം, stdout);
          എങ്കിൽ (! no_newline)
            putchar (use_nuls ? '\0' : '\n');
          സ്വതന്ത്രം (മൂല്യം);
          എങ്കിൽ (ferror (stdout))
            write_error ();
        }
      അല്ലെങ്കിൽ
        {
          status = EXIT_FAILURE;
          എങ്കിൽ (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  തിരികെ status;
}
