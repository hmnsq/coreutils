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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "long-options.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "users"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

ਸਥਿਰ ਪੂਰਨ
userid_compare (ਅਚਲ ਖਾਲੀ *v_a, ਅਚਲ ਖਾਲੀ *v_b)
{
  ਅੱਖਰ **a = (ਅੱਖਰ **) v_a;
  ਅੱਖਰ **b = (ਅੱਖਰ **) v_b;
  ਵਾਪਸ strcmp (*a, *b);
}

ਸਥਿਰ ਖਾਲੀ
list_entries_users (idx_t n, STRUCT_UTMP ਅਚਲ *this)
{
  ਅੱਖਰ **u = xinmalloc (n, ਆਕਾਰ *u);
  idx_t n_entries = 0;

  ਜਦੋਂ (n--)
    {
      ਜੇ (IS_USER_PROCESS (this))
        {
          ਅੱਖਰ *trimmed_name;

          trimmed_name = ਕੱਢੋ_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  qsort (u, n_entries, ਆਕਾਰ (u[0]), userid_compare);

  ਲਈ (idx_t i = 0; i < n_entries; i++)
    {
      ਅੱਖਰ c = (i < n_entries - 1 ? ' ' : '\n');
      fputs (u[i], stdout);
      ਅੱਖਰ_ਲਿਖੋ (c);
    }

  ਲਈ (idx_t i = 0; i < n_entries; i++)
    ਮੁਕਤ (u[i]);
  ਮੁਕਤ (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

ਸਥਿਰ ਖਾਲੀ
users (ਅੱਖਰ ਅਚਲ *filename, ਪੂਰਨ options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  ਜੇ (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  ਮੁਕਤ (utmp_buf);
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... [FILE]\n"), ਪ੍ਰੋਗਰਾਮ_name);
      ਲਿਖੋ (_("\
Output who is currently logged ਵਿੱਚ according ਮੰਜ਼ਿਲ FILE.\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n\
"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, ਪ੍ਰੋਗਰਾਮ_NAME, PACKAGE_NAME,
                                   ਸੰਸਕਰਣ, true, usage, AUTHORS,
                                   (ਅੱਖਰ ਅਚਲ *) ਨਿਰਾ);

  ਚੋਣ (argc - optind)
    {
    ਹਾਲਤ 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ਰੋਕੋ;

    ਹਾਲਤ 1:			/* users <utmp file> */
      users (argv[optind], 0);
      ਰੋਕੋ;

    ਮੂਲ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ਵਾਪਸ EXIT_SUCCESS;
}
