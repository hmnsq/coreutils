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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>

#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"

#അന്തര്ഭാവ "long-options.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "users"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

സ്ഥിതം പൂർണ്ണസംഖ്യ
userid_compare (സ്ഥിരം ശൂന്യം *v_a, സ്ഥിരം ശൂന്യം *v_b)
{
  അക്ഷരം **a = (അക്ഷരം **) v_a;
  അക്ഷരം **b = (അക്ഷരം **) v_b;
  തിരികെ strcmp (*a, *b);
}

സ്ഥിതം ശൂന്യം
list_entries_users (idx_t n, STRUCT_UTMP സ്ഥിരം *this)
{
  അക്ഷരം **u = xinmalloc (n, വലുപ്പം *u);
  idx_t n_entries = 0;

  ആകുമ്പോൾ (n--)
    {
      എങ്കിൽ (IS_USER_PROCESS (this))
        {
          അക്ഷരം *trimmed_name;

          trimmed_name = extract_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  qsort (u, n_entries, വലുപ്പം (u[0]), userid_compare);

  വേണ്ടി (idx_t i = 0; i < n_entries; i++)
    {
      അക്ഷരം c = (i < n_entries - 1 ? ' ' : '\n');
      fputs (u[i], stdout);
      putchar (c);
    }

  വേണ്ടി (idx_t i = 0; i < n_entries; i++)
    സ്വതന്ത്രം (u[i]);
  സ്വതന്ത്രം (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

സ്ഥിതം ശൂന്യം
users (അക്ഷരം സ്ഥിരം *filename, പൂർണ്ണസംഖ്യ options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  എങ്കിൽ (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  സ്വതന്ത്രം (utmp_buf);
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (അക്ഷരം സ്ഥിരം *) ശൂന്യ);

  തിരഞ്ഞെടുപ്പ് (argc - optind)
    {
    അവസ്ഥ 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      നിർത്തുക;

    അവസ്ഥ 1:			/* users <utmp file> */
      users (argv[optind], 0);
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  തിരികെ EXIT_SUCCESS;
}
