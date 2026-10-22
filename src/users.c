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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>

#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "long-options.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "users"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

સ્થિર પૂર્ણાંક
userid_compare (અચલ શૂન્ય *v_a, અચલ શૂન્ય *v_b)
{
  અક્ષર **a = (અક્ષર **) v_a;
  અક્ષર **b = (અક્ષર **) v_b;
  પાછા strcmp (*a, *b);
}

સ્થિર શૂન્ય
list_entries_users (idx_t n, STRUCT_UTMP અચલ *this)
{
  અક્ષર **u = xinmalloc (n, કદ *u);
  idx_t n_entries = 0;

  જ્યારે (n--)
    {
      જો (IS_USER_PROCESS (this))
        {
          અક્ષર *trimmed_name;

          trimmed_name = extract_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  qsort (u, n_entries, કદ (u[0]), userid_compare);

  માટે (idx_t i = 0; i < n_entries; i++)
    {
      અક્ષર c = (i < n_entries - 1 ? ' ' : '\n');
      fputs (u[i], stdout);
      putchar (c);
    }

  માટે (idx_t i = 0; i < n_entries; i++)
    મુક્ત (u[i]);
  મુક્ત (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

સ્થિર શૂન્ય
users (અક્ષર અચલ *filename, પૂર્ણાંક options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  જો (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  મુક્ત (utmp_buf);
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]... [FILE]\n"), program_name);
      printf (_("\
Output who is currently logged in according to FILE.\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n\
"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (અક્ષર અચલ *) ખાલી);

  પસંદગી (argc - optind)
    {
    સ્થિતિ 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      તોડો;

    સ્થિતિ 1:			/* users <utmp file> */
      users (argv[optind], 0);
      તોડો;

    મૂળભૂત:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  પાછા EXIT_SUCCESS;
}
