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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#అన్తర్భావ "system.h"
#అన్తర్భావ "canonicalize.h"
#అన్తర్భావ "areadlink.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "readlink"

#నిర్వచించు AUTHORS proper_name ("Dmitry V. Levin")

/* If true, do not output the trailing newline.  */
స్థిర bool no_newline;

/* If true, report error messages.  */
స్థిర bool verbose;

/* If true, quote output according to the selected quoting style.  */
స్థిర bool quote_output;

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"canonicalize", no_వాదం, ఖాళీ, 'f'},
  {"canonicalize-existing", no_వాదం, ఖాళీ, 'e'},
  {"canonicalize-missing", no_వాదం, ఖాళీ, 'm'},
  {"no-newline", no_వాదం, ఖాళీ, 'n'},
  {"quiet", no_వాదం, ఖాళీ, 'q'},
  {"silent", no_వాదం, ఖాళీ, 's'},
  {"verbose", no_వాదం, ఖాళీ, 'v'},
  {"zero", no_వాదం, ఖాళీ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... FILE...\n"), కార్యక్రమం_name);
      fputs (_("Print value of a symbolic link or canonical file name\n\n"),
             stdout);
      oputs (_("\
  -f, --canonicalize\n\
         canonicalize by following every symlink\n\
         లోపల every component యొక్క the given name recursively;\n\
         all but the చివరి component must exist\n\
"));
      oputs (_("\
  -e, --canonicalize-existing\n\
         canonicalize by following every symlink\n\
         లోపల every component యొక్క the given name recursively;\n\
         all components must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing\n\
         canonicalize by following every symlink\n\
         లోపల every component యొక్క the given name recursively,\n\
         without requirements on components existence\n\
"));
      oputs (_("\
  -n, --no-newline\n\
         చేయి not output the trailing delimiter\n\
"));
      oputs (_("\
  -q, --quiet\n\
"));
      oputs (_("\
  -s, --silent\n\
         suppress most error messages\n\
         (on by అప్రమేయ ఒకవేళ POSIXLY_CORRECT is not సమితి_2)\n\
"));
      oputs (_("\
  -v, --verbose\n\
         report error messages\n\
         (on by అప్రమేయ ఒకవేళ POSIXLY_CORRECT is సమితి_2)\n\
"));
      oputs (_("\
  -z, --zero\n\
         చివర each output పంక్తి with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  /* If not -1, use this method to canonicalize.  */
  పూర్ణాంకం can_బహుళకం = -1;
  పూర్ణాంకం స్థితి_2 = EXIT_SUCCESS;
  పూర్ణాంకం optc;
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, "efmnqsvz", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'e':
          can_బహుళకం = CAN_EXISTING;
          ఆపు;
        స్థితి 'f':
          can_బహుళకం = CAN_ALL_BUT_చివరి;
          ఆపు;
        స్థితి 'm':
          can_బహుళకం = CAN_MISSING;
          ఆపు;
        స్థితి 'n':
          no_newline = true;
          ఆపు;
        స్థితి 'q':
        స్థితి 's':
          verbose = false;
          ఆపు;
        స్థితి 'v':
          verbose = true;
          ఆపు;
        స్థితి 'z':
          use_nuls = true;
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (optind >= argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (argc - optind > 1)
    {
      ఒకవేళ (no_newline)
        error (0, 0, _("ignoring --no-newline with multiple arguments"));
      no_newline = false;
    }

  ఒకవేళ (!use_nuls && isatty (STDOUT_FILENO))
    {
      పూర్ణాంకం qs = getenv_quoting_style ();
      ఒకవేళ (qs < 0)
        qs = shell_escape_quoting_style;
      ఒకవేళ (qs != literal_quoting_style)
        {
          సమితి_quoting_style (ఖాళీ, qs);
          quote_output = true;
        }
    }

  /* POSIX requires a diagnostic message written to standard error and a
     non-zero exit status when given a file that is not a symbolic link.  */
  ఒకవేళ (getenv ("POSIXLY_CORRECT") != ఖాళీ)
    verbose = true;

  కొరకు (; optind < argc; ++optind)
    {
      అక్షరం స్థిరరాశి *fname = argv[optind];
      అక్షరం *విలువ = (can_బహుళకం != -1
                     ? canonicalize_filename_బహుళకం (fname, can_బహుళకం)
                     : areadlink_with_కొలత (fname, 63));
      ఒకవేళ (విలువ)
        {
          fputs (quote_output ? quoteN (విలువ) : విలువ, stdout);
          ఒకవేళ (! no_newline)
            అక్షరం_రాయి (use_nuls ? '\0' : '\n');
          విడుదల (విలువ);
          ఒకవేళ (ferror (stdout))
            write_error ();
        }
      లేకపోతే
        {
          స్థితి_2 = EXIT_FAILURE;
          ఒకవేళ (verbose)
            error (0, errno, "%s", quotef (fname));
        }
    }

  తిరిగి స్థితి_2;
}
