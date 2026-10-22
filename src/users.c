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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>

#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "long-options.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "users"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

நிலையான முழு
userid_compare (மாறிலி வெறுமை *v_a, மாறிலி வெறுமை *v_b)
{
  எழுத்து **a = (எழுத்து **) v_a;
  எழுத்து **b = (எழுத்து **) v_b;
  திரும்பு strcmp (*a, *b);
}

நிலையான வெறுமை
list_entries_users (idx_t n, STRUCT_UTMP மாறிலி *this)
{
  எழுத்து **u = xinmalloc (n, அளவு *u);
  idx_t n_entries = 0;

  போது (n--)
    {
      எனில் (IS_USER_PROCESS (this))
        {
          எழுத்து *trimmed_name;

          trimmed_name = extract_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  qsort (u, n_entries, அளவு (u[0]), userid_compare);

  க்கு (idx_t i = 0; i < n_entries; i++)
    {
      எழுத்து c = (i < n_entries - 1 ? ' ' : '\n');
      fputs (u[i], stdout);
      putchar (c);
    }

  க்கு (idx_t i = 0; i < n_entries; i++)
    விடுவி (u[i]);
  விடுவி (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

நிலையான வெறுமை
users (எழுத்து மாறிலி *filename, முழு options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  எனில் (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  விடுவி (utmp_buf);
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (எழுத்து மாறிலி *) வெற்று);

  தேர்வு (argc - optind)
    {
    நிலை 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      நிறுத்து;

    நிலை 1:			/* users <utmp file> */
      users (argv[optind], 0);
      நிறுத்து;

    இயல்பு:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  திரும்பு EXIT_SUCCESS;
}
