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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "users"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
userid_compare (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *v_a, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *v_b)
{
  ಅಕ್ಷರ **a = (ಅಕ್ಷರ **) v_a;
  ಅಕ್ಷರ **b = (ಅಕ್ಷರ **) v_b;
  ಹಿಂತಿರುಗಿ strcmp (*a, *b);
}

ಸ್ಥಿರ ಶೂನ್ಯ
list_entries_users (idx_t n, STRUCT_UTMP ಸ್ಥಿರಾಂಕ *this)
{
  ಅಕ್ಷರ **u = xinmalloc (n, ಗಾತ್ರ *u);
  idx_t n_entries = 0;

  ಆಗ (n--)
    {
      ಆದರೆ (IS_USER_PROCESS (this))
        {
          ಅಕ್ಷರ *trimmed_name;

          trimmed_name = ಹೊರತೆಗೆ_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  qsort (u, n_entries, ಗಾತ್ರ (u[0]), userid_compare);

  ಗಾಗಿ (idx_t i = 0; i < n_entries; i++)
    {
      ಅಕ್ಷರ c = (i < n_entries - 1 ? ' ' : '\n');
      fputs (u[i], stdout);
      ಅಕ್ಷರ_ಬರೆ (c);
    }

  ಗಾಗಿ (idx_t i = 0; i < n_entries; i++)
    ಬಿಡುಗಡೆ (u[i]);
  ಬಿಡುಗಡೆ (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

ಸ್ಥಿರ ಶೂನ್ಯ
users (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *filename, ಪೂರ್ಣಾಂಕ options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  ಆದರೆ (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  ಬಿಡುಗಡೆ (utmp_buf);
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... [FILE]\n"), ಕಾರ್ಯಕ್ರಮ_name);
      ಬರೆ (_("\
Output who is currently logged ಒಳ according ಗಮ್ಯ FILE.\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n\
"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, ಕಾರ್ಯಕ್ರಮ_NAME, PACKAGE_NAME,
                                   ಆವೃತ್ತಿ, true, usage, AUTHORS,
                                   (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *) ಖಾಲಿ);

  ಆಯ್ಕೆ (argc - optind)
    {
    ಸ್ಥಿತಿ 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 1:			/* users <utmp file> */
      users (argv[optind], 0);
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
