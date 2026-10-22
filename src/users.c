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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "long-options.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "users"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
userid_compare (ಧ್ರುವ ಶೂನ್ಯ *v_a, ಧ್ರುವ ಶೂನ್ಯ *v_b)
{
  ವರ್ಣ **a = (ವರ್ಣ **) v_a;
  ವರ್ಣ **b = (ವರ್ಣ **) v_b;
  ಪ್ರತ್ಯಾಗಮ strcmp (*a, *b);
}

ಸ್ಥಿರ ಶೂನ್ಯ
list_entries_users (idx_t n, STRUCT_UTMP ಧ್ರುವ *this)
{
  ವರ್ಣ **u = xinmalloc (n, ಪರಿಮಾಣ *u);
  idx_t n_entries = 0;

  ಯಾವತ್ (n--)
    {
      ಯದಿ (IS_USER_PROCESS (this))
        {
          ವರ್ಣ *trimmed_name;

          trimmed_name = extract_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  qsort (u, n_entries, ಪರಿಮಾಣ (u[0]), userid_compare);

  ಪ್ರತಿ (idx_t i = 0; i < n_entries; i++)
    {
      ವರ್ಣ c = (i < n_entries - 1 ? ' ' : '\n');
      fputs (u[i], stdout);
      putchar (c);
    }

  ಪ್ರತಿ (idx_t i = 0; i < n_entries; i++)
    ಮುಕ್ತ (u[i]);
  ಮುಕ್ತ (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

ಸ್ಥಿರ ಶೂನ್ಯ
users (ವರ್ಣ ಧ್ರುವ *filename, ಪೂರ್ಣಾಙ್ಕ options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  ಯದಿ (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  ಮುಕ್ತ (utmp_buf);
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (ವರ್ಣ ಧ್ರುವ *) ರಿಕ್ತ);

  ವಿಕಲ್ಪ (argc - optind)
    {
    ಪಕ್ಷ 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ಭಙ್ಗ;

    ಪಕ್ಷ 1:			/* users <utmp file> */
      users (argv[optind], 0);
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
}
