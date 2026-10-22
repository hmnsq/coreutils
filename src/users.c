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

#شامل <config.h>
#شامل <stdio.h>

#شامل <sys/types.h>
#شامل "system.h"

#شامل "long-options.h"
#شامل "quote.h"
#شامل "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "users"

#تعریف_کرو AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

ساکن صحیح
userid_compare (ثابت خالی *v_a, ثابت خالی *v_b)
{
  حرف **a = (حرف **) v_a;
  حرف **b = (حرف **) v_b;
  واپس strcmp (*a, *b);
}

ساکن خالی
list_entries_users (idx_t n, STRUCT_UTMP ثابت *this)
{
  حرف **u = xinmalloc (n, حجم *u);
  idx_t n_entries = 0;

  جب (n--)
    {
      اگر (IS_USER_PROCESS (this))
        {
          حرف *trimmed_name;

          trimmed_name = نکالو_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  qsort (u, n_entries, حجم (u[0]), userid_compare);

  برائے (idx_t i = 0; i < n_entries; i++)
    {
      حرف c = (i < n_entries - 1 ? ' ' : '\n');
      fputs (u[i], stdout);
      حرف_لکھو (c);
    }

  برائے (idx_t i = 0; i < n_entries; i++)
    آزاد (u[i]);
  آزاد (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

ساکن خالی
users (حرف ثابت *filename, صحیح options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  اگر (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  آزاد (utmp_buf);
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... [FILE]\n"), پروگرام_name);
      لکھو (_("\
Output who is currently logged اندر according مقصد FILE.\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n\
"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE_NAME,
                                   نسخہ, true, usage, AUTHORS,
                                   (حرف ثابت *) تہی);

  انتخاب (argc - optind)
    {
    حالت 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      توڑو;

    حالت 1:			/* users <utmp file> */
      users (argv[optind], 0);
      توڑو;

    طےشدہ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  واپس EXIT_SUCCESS;
}
