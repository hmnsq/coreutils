/* basename -- strip directory and suffix from file names
   Copyright (C) 1990-2026 Free Software Foundation, Inc.

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

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#అన్తర్భావ "system.h"
#అన్తర్భావ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "basename"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

స్థిర bool quote_output;

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"multiple", no_వాదం, ఖాళీ, 'a'},
  {"suffix", required_వాదం, ఖాళీ, 's'},
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
      రాయి (_("\
Usage: %s NAME [SUFFIX]\n\
  or:  %s OPTION... NAME...\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Print NAME with any leading directory components removed.\n\
If specified, also remove a trailing SUFFIX.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -a, --గుణిజం\n\
         support గుణిజం arguments and treat each as a NAME\n\
"));
      oputs (_("\
  -s, --suffix=SUFFIX\n\
         remove a trailing SUFFIX; సూచిస్తుంది -a\n\
"));
      oputs (_("\
  -z, --zero\n\
         చివర each output పంక్తి with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      రాయి (_("\
\n\
Examples:\n\
  %s /usr/bin/క్రమీకరించు          -> \"క్రమీకరించు\"\n\
  %s include/stdio.h .h     -> \"stdio\"\n\
  %s -s .h include/stdio.h  -> \"stdio\"\n\
  %s -a any/str1 any/str2   -> \"str1\" followed by \"str2\"\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Remove SUFFIX from the end of NAME if it is there, unless NAME
   consists entirely of SUFFIX.  */

స్థిర శూన్య
remove_suffix (అక్షరం *name, అక్షరం స్థిరరాశి *suffix, idx_t suffix_len)
{
  అక్షరం *np = name + అక్షరశ్రేణి_పొడవు (name);
  అక్షరం స్థిరరాశి *sp = suffix + suffix_len;

  అయితే (np > name && sp > suffix)
    ఒకవేళ (*--np != *--sp)
      తిరిగి;
  ఒకవేళ (np > name)
    *np = '\0';
}

/* Perform the basename operation on STRING.  If SUFFIX has a positive
   length of SUFFIX_LEN bytes, remove the trailing SUFFIX.
   Finally, output the result string.  */

స్థిర శూన్య
perform_basename (అక్షరం స్థిరరాశి *string, అక్షరం స్థిరరాశి *suffix, idx_t suffix_len,
                  bool use_nuls)
{
  అక్షరం *name = పునాది_name (string);
  strip_trailing_slashes (name);

  /* Per POSIX, 'basename // /' must return '//' on platforms with
     distinct //.  On platforms with drive letters, this generalizes
     to making 'basename c: :' return 'c:'.  This rule is captured by
     skipping suffix stripping if base_name returned an absolute path
     or a drive letter (only possible if name is a file-system
     root).  */
  ఒకవేళ (0 < suffix_len && IS_RELATIVE_FILE_NAME (name)
      && ! FILE_SYSTEM_ఉపసర్గ_LEN (name))
    remove_suffix (name, suffix, suffix_len);

  fputs (quote_output ? quoteN (name) : name, stdout);
  అక్షరం_రాయి (use_nuls ? '\0' : '\n');
  విడుదల (name);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool గుణిజం_పేర్లు = false;
  bool use_nuls = false;
  అక్షరం స్థిరరాశి *suffix = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే (true)
    {
      పూర్ణాంకం c = getopt_long (argc, argv, "+as:z", longopts, ఖాళీ);

      ఒకవేళ (c == -1)
        ఆపు;

      ఎంపిక (c)
        {
        స్థితి 's':
          suffix = optarg;
          /* -s implies -a, so...  */
          FALLTHROUGH;

        స్థితి 'a':
          గుణిజం_పేర్లు = true;
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

  ఒకవేళ (argc < optind + 1)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (! గుణిజం_పేర్లు)
    {
      ఒకవేళ (optind + 2 == argc)
        suffix = argv[optind + 1];
      లేకపోతే ఒకవేళ (optind + 2 < argc)
        {
          error (0, 0, _("extra operand %s"), quote (argv[optind + 2]));
          usage (EXIT_FAILURE);
        }
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

  idx_t suffix_len = suffix ? అక్షరశ్రేణి_పొడవు (suffix) : 0;

  అక్షరం **file = argv + optind;
  పూర్ణాంకం n_files = గుణిజం_పేర్లు ? argc - optind : 1;

  కొరకు (పూర్ణాంకం i = 0; i < n_files; ++i)
    perform_basename (file[i], suffix, suffix_len, use_nuls);

  తిరిగి EXIT_SUCCESS;
}
