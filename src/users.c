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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>

#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "long-options.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "users"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

স্থির পূর্ণাঙ্ক
userid_compare (ধ্রুব শূন্য *v_a, ধ্রুব শূন্য *v_b)
{
  বর্ণ **a = (বর্ণ **) v_a;
  বর্ণ **b = (বর্ণ **) v_b;
  প্রত্যাগম strcmp (*a, *b);
}

স্থির শূন্য
list_entries_users (idx_t n, STRUCT_UTMP ধ্রুব *this)
{
  বর্ণ **u = xinmalloc (n, পরিমাণ *u);
  idx_t n_entries = 0;

  যাবত্ (n--)
    {
      যদি (IS_USER_PROCESS (this))
        {
          বর্ণ *trimmed_name;

          trimmed_name = extract_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  qsort (u, n_entries, পরিমাণ (u[0]), userid_compare);

  প্রতি (idx_t i = 0; i < n_entries; i++)
    {
      বর্ণ c = (i < n_entries - 1 ? ' ' : '\n');
      fputs (u[i], stdout);
      putchar (c);
    }

  প্রতি (idx_t i = 0; i < n_entries; i++)
    মুক্ত (u[i]);
  মুক্ত (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

স্থির শূন্য
users (বর্ণ ধ্রুব *filename, পূর্ণাঙ্ক options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  যদি (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  মুক্ত (utmp_buf);
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (বর্ণ ধ্রুব *) রিক্ত);

  বিকল্প (argc - optind)
    {
    পক্ষ 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ভঙ্গ;

    পক্ষ 1:			/* users <utmp file> */
      users (argv[optind], 0);
      ভঙ্গ;

    স্বভাব:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  প্রত্যাগম EXIT_SUCCESS;
}
