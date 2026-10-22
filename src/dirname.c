/* dirname -- strip suffix from file name

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

/* Written by David MacKenzie and Jim Meyering. */

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "argmatch.h"  /* argmatch($QUOTING_STYLE).  */
#అన్తర్భావ "system.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "dirname"

#నిర్వచించు AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("Jim Meyering")

స్థిర bool quote_output;

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
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
Usage: %s [OPTION] NAME...\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Output each NAME with its చివరి non-slash component and trailing slashes\n\
removed; ఒకవేళ NAME కలిగియుంది no /'s, output '.' (meaning the current directory).\n\
\n\
"), stdout);
      oputs (_("\
  -z, --zero\n\
         చివర each output పంక్తి with NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      రాయి (_("\
\n\
Examples:\n\
  %s /usr/bin/          -> \"/usr\"\n\
  %s dir1/str dir2/str  -> \"dir1\" followed by \"dir2\"\n\
  %s stdio.h            -> \".\"\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool use_nuls = false;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే (true)
    {
      పూర్ణాంకం c = getopt_long (argc, argv, "z", longopts, ఖాళీ);

      ఒకవేళ (c == -1)
        ఆపు;

      ఎంపిక (c)
        {
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

  కొరకు (; optind < argc; optind++)
    {
      అక్షరం స్థిరరాశి *ఫలితం = argv[optind];
      idx_t len = dir_len (ఫలితం);

      ఒకవేళ (! len)
        {
          స్థిర అక్షరం స్థిరరాశి dot = '.';
          ఫలితం = &dot;
          len = 1;
        }
      ఒకవేళ (quote_output)
        {
          ఫలితం = quoteN_mem (ఫలితం, len);
          len = అక్షరశ్రేణి_పొడవు (ఫలితం);
        }
      fwrite (ఫలితం, 1, len, stdout);
      అక్షరం_రాయి (use_nuls ? '\0' :'\n');
    }

  తిరిగి EXIT_SUCCESS;
}
