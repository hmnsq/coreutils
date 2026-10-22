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

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#అన్తర్భావ "system.h"
#అన్తర్భావ "canonicalize.h"
#అన్తర్భావ "relpath.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "realpath"

#నిర్వచించు AUTHORS proper_name_lite ("Padraig Brady", "P\303\241draig Brady")

గణన
{
  RELATIVE_గమ్యం_OPTION = CHAR_MAX + 1,
  RELATIVE_పునాది_OPTION
};

స్థిర bool verbose = true;
స్థిర bool logical;
స్థిర bool use_nuls;
స్థిర bool quote_output;
స్థిర అక్షరం స్థిరరాశి *can_relative_గమ్యం;
స్థిర అక్షరం స్థిరరాశి *can_relative_పునాది;

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"canonicalize", no_వాదం, ఖాళీ, 'E'},
  {"canonicalize-existing", no_వాదం, ఖాళీ, 'e'},
  {"canonicalize-missing", no_వాదం, ఖాళీ, 'm'},
  {"relative-to", required_వాదం, ఖాళీ, RELATIVE_గమ్యం_OPTION},
  {"relative-base", required_వాదం, ఖాళీ, RELATIVE_పునాది_OPTION},
  {"quiet", no_వాదం, ఖాళీ, 'q'},
  {"strip", no_వాదం, ఖాళీ, 's'},
  {"no-symlinks", no_వాదం, ఖాళీ, 's'},
  {"zero", no_వాదం, ఖాళీ, 'z'},
  {"logical", no_వాదం, ఖాళీ, 'L'},
  {"physical", no_వాదం, ఖాళీ, 'P'},
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
      fputs (_("\
Print the resolved absolute file name.\n\
"), stdout);
      oputs (_("\
  -E, --canonicalize           all but the చివరి component must exist (అప్రమేయ)\
\n\
"));
      oputs (_("\
  -e, --canonicalize-existing  all components యొక్క the మార్గం must exist\n\
"));
      oputs (_("\
  -m, --canonicalize-missing   no మార్గం components need exist or be a directory\
\n\
"));
      oputs (_("\
  -L, --logical                resolve '..' components before symlinks\n\
"));
      oputs (_("\
  -P, --physical               resolve symlinks as encountered (అప్రమేయ)\n\
"));
      oputs (_("\
  -q, --quiet                  suppress most error messages\n\
"));
      oputs (_("\
      --relative-గమ్యం=DIR        print the resolved మార్గం relative గమ్యం DIR\n\
"));
      oputs (_("\
      --relative-పునాది_2=DIR      print absolute paths unless paths క్రింద DIR\n\
"));
      oputs (_("\
  -s, --strip, --no-symlinks   don't expand symlinks\n\
"));
      oputs (_("\
  -z, --zero                   చివర each output పంక్తి with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* A wrapper around canonicalize_filename_mode(),
   to call it twice when in LOGICAL mode.  */
స్థిర అక్షరం *
realpath_canon (అక్షరం స్థిరరాశి *fname, పూర్ణాంకం can_బహుళకం)
{
  అక్షరం *can_fname = canonicalize_filename_బహుళకం (fname, can_బహుళకం);
  ఒకవేళ (logical && can_fname)  /* canonicalize again to resolve symlinks.  */
    {
      can_బహుళకం &= ~CAN_NOLINKS;
      అక్షరం *can_fname2 = canonicalize_filename_బహుళకం (can_fname, can_బహుళకం);
      విడుదల (can_fname);
      తిరిగి can_fname2;
    }
  తిరిగి can_fname;
}

/* Test whether canonical prefix is parent or match of path.  */
ATTRIBUTE_శుద్ధ
స్థిర bool
మార్గం_ఉపసర్గ (అక్షరం స్థిరరాశి *ఉపసర్గ, అక్షరం స్థిరరాశి *మార్గం)
{
  /* We already know prefix[0] and path[0] are '/'.  */
  ఉపసర్గ++;
  మార్గం++;

  /* '/' is the prefix of everything except '//' (since we know '//'
     is only present after canonicalization if it is distinct).  */
  ఒకవేళ (!*ఉపసర్గ)
    తిరిగి *మార్గం != '/';

  /* Likewise, '//' is a prefix of any double-slash path.  */
  ఒకవేళ (*ఉపసర్గ == '/' && !ఉపసర్గ[1])
    తిరిగి *మార్గం == '/';

  /* Any other prefix has a non-slash portion.  */
  అయితే (*ఉపసర్గ && *మార్గం)
    {
      ఒకవేళ (*ఉపసర్గ != *మార్గం)
        ఆపు;
      ఉపసర్గ++;
      మార్గం++;
    }
  తిరిగి (!*ఉపసర్గ && (*మార్గం == '/' || !*మార్గం));
}

స్థిర bool
isdir (అక్షరం స్థిరరాశి *మార్గం)
{
  నిర్మాణం stat sb;
  ఒకవేళ (stat (మార్గం, &sb) != 0)
    error (EXIT_FAILURE, errno, _("cannot stat %s"), quoteaf (మార్గం));
  తిరిగి S_ISDIR (sb.st_బహుళకం);
}

స్థిర శూన్య
print_మార్గం (అక్షరం స్థిరరాశి *మార్గం)
{
  fputs (quote_output ? quoteN (మార్గం) : మార్గం, stdout);
}

/* Print CAN_FNAME relative to can_relative_to, quoting the complete
   relative file name when appropriate.  */
స్థిర bool
print_relative_మార్గం (అక్షరం స్థిరరాశి *can_fname)
{
  ఒకవేళ (!quote_output)
    తిరిగి relpath (can_fname, can_relative_గమ్యం, ఖాళీ, 0);

  size_t కొలత;
  ఒకవేళ (ckd_mul (&కొలత, అక్షరశ్రేణి_పొడవు (can_relative_గమ్యం), 2)
      || ckd_కూడు (&కొలత, కొలత, అక్షరశ్రేణి_పొడవు (can_fname))
      || ckd_కూడు (&కొలత, కొలత, 1))
    xalloc_die ();

  అక్షరం *relative_మార్గం = xmalloc (కొలత);
  bool success = relpath (can_fname, can_relative_గమ్యం, relative_మార్గం, కొలత);
  ఒకవేళ (success)
    print_మార్గం (relative_మార్గం);
  విడుదల (relative_మార్గం);
  తిరిగి success;
}

స్థిర bool
process_మార్గం (అక్షరం స్థిరరాశి *fname, పూర్ణాంకం can_బహుళకం)
{
  అక్షరం *can_fname = realpath_canon (fname, can_బహుళకం);
  ఒకవేళ (!can_fname)
    {
      ఒకవేళ (verbose)
        error (0, errno, "%s", quotef (fname));
      తిరిగి false;
    }

  ఒకవేళ (!can_relative_గమ్యం
      || (can_relative_పునాది && !మార్గం_ఉపసర్గ (can_relative_పునాది, can_fname))
      || (can_relative_గమ్యం && !print_relative_మార్గం (can_fname)))
    print_మార్గం (can_fname);

  అక్షరం_రాయి (use_nuls ? '\0' : '\n');

  విడుదల (can_fname);

  ఒకవేళ (ferror (stdout))
    write_error ();

  తిరిగి true;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool ok = true;
  పూర్ణాంకం can_బహుళకం = CAN_ALL_BUT_చివరి;
  అక్షరం స్థిరరాశి *relative_గమ్యం = ఖాళీ;
  అక్షరం స్థిరరాశి *relative_పునాది = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే (true)
    {
      పూర్ణాంకం c = getopt_long (argc, argv, "EeLmPqsz", longopts, ఖాళీ);
      ఒకవేళ (c == -1)
        ఆపు;
      ఎంపిక (c)
        {
        స్థితి 'E':
          can_బహుళకం &= ~CAN_బహుళకం_ముసుగు;
          can_బహుళకం |= CAN_ALL_BUT_చివరి;
          ఆపు;
        స్థితి 'e':
          can_బహుళకం &= ~CAN_బహుళకం_ముసుగు;
          can_బహుళకం |= CAN_EXISTING;
          ఆపు;
        స్థితి 'm':
          can_బహుళకం &= ~CAN_బహుళకం_ముసుగు;
          can_బహుళకం |= CAN_MISSING;
          ఆపు;
        స్థితి 'L':
          can_బహుళకం |= CAN_NOLINKS;
          logical = true;
          ఆపు;
        స్థితి 's':
          can_బహుళకం |= CAN_NOLINKS;
          logical = false;
          ఆపు;
        స్థితి 'P':
          can_బహుళకం &= ~CAN_NOLINKS;
          logical = false;
          ఆపు;
        స్థితి 'q':
          verbose = false;
          ఆపు;
        స్థితి 'z':
          use_nuls = true;
          ఆపు;
        స్థితి RELATIVE_గమ్యం_OPTION:
          relative_గమ్యం = optarg;
          ఆపు;
        స్థితి RELATIVE_పునాది_OPTION:
          relative_పునాది = optarg;
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

  ఒకవేళ (! use_nuls && isatty (STDOUT_FILENO))
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

  ఒకవేళ (relative_పునాది && !relative_గమ్యం)
    relative_గమ్యం = relative_పునాది;

  bool need_dir = (can_బహుళకం & CAN_బహుళకం_ముసుగు) == CAN_EXISTING;
  ఒకవేళ (relative_గమ్యం)
    {
      can_relative_గమ్యం = realpath_canon (relative_గమ్యం, can_బహుళకం);
      ఒకవేళ (!can_relative_గమ్యం)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_గమ్యం));
      ఒకవేళ (need_dir && !isdir (can_relative_గమ్యం))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_గమ్యం));
    }
  ఒకవేళ (relative_పునాది == relative_గమ్యం)
    can_relative_పునాది = can_relative_గమ్యం;
  లేకపోతే ఒకవేళ (relative_పునాది)
    {
      అక్షరం *పునాది_2 = realpath_canon (relative_పునాది, can_బహుళకం);
      ఒకవేళ (!పునాది_2)
        error (EXIT_FAILURE, errno, "%s", quotef (relative_పునాది));
      ఒకవేళ (need_dir && !isdir (పునాది_2))
        error (EXIT_FAILURE, ENOTDIR, "%s", quotef (relative_పునాది));
      /* --relative-to is a no-op if it does not have --relative-base
           as a prefix */
      ఒకవేళ (మార్గం_ఉపసర్గ (పునాది_2, can_relative_గమ్యం))
        can_relative_పునాది = పునాది_2;
      లేకపోతే
        {
          విడుదల (పునాది_2);
          can_relative_పునాది = can_relative_గమ్యం;
          can_relative_గమ్యం = ఖాళీ;
        }
    }

  కొరకు (; optind < argc; ++optind)
    ok &= process_మార్గం (argv[optind], can_బహుళకం);

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
