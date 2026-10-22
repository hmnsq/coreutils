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

#ضمن <config.h>
#ضمن <stdio.h>

#ضمن <sys/types.h>
#ضمن "system.h"

#ضمن "long-options.h"
#ضمن "quote.h"
#ضمن "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#وصف_ڪريو پروگرام_NAME "users"

#وصف_ڪريو AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

ساكن صحيح
userid_compare (ثابت فراغ *v_a, ثابت فراغ *v_b)
{
  حرف **a = (حرف **) v_a;
  حرف **b = (حرف **) v_b;
  أرجع strcmp (*a, *b);
}

ساكن فراغ
list_entries_users (idx_t n, STRUCT_UTMP ثابت *this)
{
  حرف **u = xinmalloc (n, حجم *u);
  idx_t n_entries = 0;

  طالما (n--)
    {
      إذا (IS_USER_PROCESS (this))
        {
          حرف *trimmed_name;

          trimmed_name = ڪڍو_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  qsort (u, n_entries, حجم (u[0]), userid_compare);

  لكل (idx_t i = 0; i < n_entries; i++)
    {
      حرف c = (i < n_entries - 1 ? ' ' : '\n');
      fputs (u[i], stdout);
      اکر_لکو (c);
    }

  لكل (idx_t i = 0; i < n_entries; i++)
    حرر (u[i]);
  حرر (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

ساكن فراغ
users (حرف ثابت *filename, صحيح options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  إذا (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  حرر (utmp_buf);
}

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("Usage: %s [OPTION]... [FILE]\n"), پروگرام_name);
      لکو (_("\
Output who is currently logged اندريون according منزل FILE.\n\
If FILE is not specified, use %s.  %s as FILE is common.\n\
\n\
"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE_NAME,
                                   نسخو, true, usage, AUTHORS,
                                   (حرف ثابت *) عدم);

  بدل (argc - optind)
    {
    حالة 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      اكسر;

    حالة 1:			/* users <utmp file> */
      users (argv[optind], 0);
      اكسر;

    افتراضي:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  أرجع EXIT_SUCCESS;
}
