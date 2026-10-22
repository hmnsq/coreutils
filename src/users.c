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

স্থির পূর্ণসংখ্যা
userid_compare (ধ্রুবক শূন্য *v_a, ধ্রুবক শূন্য *v_b)
{
  অক্ষর **a = (অক্ষর **) v_a;
  অক্ষর **b = (অক্ষর **) v_b;
  ফেরত strcmp (*a, *b);
}

স্থির শূন্য
list_entries_users (idx_t n, STRUCT_UTMP ধ্রুবক *this)
{
  অক্ষর **u = xinmalloc (n, আকার *u);
  idx_t n_entries = 0;

  যখন (n--)
    {
      যদি (IS_USER_PROCESS (this))
        {
          অক্ষর *trimmed_name;

          trimmed_name = extract_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  qsort (u, n_entries, আকার (u[0]), userid_compare);

  জন্য (idx_t i = 0; i < n_entries; i++)
    {
      অক্ষর c = (i < n_entries - 1 ? ' ' : '\n');
      fputs (u[i], stdout);
      putchar (c);
    }

  জন্য (idx_t i = 0; i < n_entries; i++)
    মুক্ত (u[i]);
  মুক্ত (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

স্থির শূন্য
users (অক্ষর ধ্রুবক *filename, পূর্ণসংখ্যা options)
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
usage (পূর্ণসংখ্যা status)
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

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (অক্ষর ধ্রুবক *) ফাঁকা);

  নির্বাচন (argc - optind)
    {
    ক্ষেত্র 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      বিরতি;

    ক্ষেত্র 1:			/* users <utmp file> */
      users (argv[optind], 0);
      বিরতি;

    পূর্বনির্ধারিত:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ফেরত EXIT_SUCCESS;
}
