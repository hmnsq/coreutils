/* GNU's users.
   Copyright (C) 1992-2026 Free Software Foundation, Inc.

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

/* Written by jla; revised by djm */

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>

#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"

#అన్తర్భావ "long-options.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "users"

#నిర్వచించు AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

స్థిర పూర్ణాంకం
userid_compare (స్థిరరాశి శూన్య *v_a, స్థిరరాశి శూన్య *v_b)
{
  అక్షరం **a = (అక్షరం **) v_a;
  అక్షరం **b = (అక్షరం **) v_b;
  తిరిగి strcmp (*a, *b);
}

స్థిర శూన్య
list_entries_users (idx_t n, STRUCT_UTMP స్థిరరాశి *this)
{
  అక్షరం **u = xinmalloc (n, పరిమాణం *u);
  idx_t n_entries = 0;

  అయితే (n--)
    {
      ఒకవేళ (IS_USER_PROCESS (this))
        {
          అక్షరం *trimmed_name;

          trimmed_name = వెలికితీయి_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  qsort (u, n_entries, పరిమాణం (u[0]), userid_compare);

  కొరకు (idx_t i = 0; i < n_entries; i++)
    {
      అక్షరం c = (i < n_entries - 1 ? ' ' : '\n');
      fputs (u[i], stdout);
      అక్షరం_రాయి (c);
    }

  కొరకు (idx_t i = 0; i < n_entries; i++)
    విడుదల (u[i]);
  విడుదల (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

స్థిర శూన్య
users (అక్షరం స్థిరరాశి *filename, పూర్ణాంకం options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  ఒకవేళ (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  విడుదల (utmp_buf);
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... [FILE]\n"), కార్యక్రమం_name);
      రాయి (_("\
Output who is currently logged లోపల according గమ్యం FILE.\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n\
"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, కార్యక్రమం_NAME, PACKAGE_NAME,
                                   సంస్కరణ, true, usage, AUTHORS,
                                   (అక్షరం స్థిరరాశి *) ఖాళీ);

  ఎంపిక (argc - optind)
    {
    స్థితి 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ఆపు;

    స్థితి 1:			/* users <utmp file> */
      users (argv[optind], 0);
      ఆపు;

    అప్రమేయ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  తిరిగి EXIT_SUCCESS;
}
