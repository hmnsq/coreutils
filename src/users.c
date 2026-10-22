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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>

#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"

#अन्तर्भाव "long-options.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "users"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

स्थिर पूर्णांक
userid_compare (स्थिरांक शून्य *v_a, स्थिरांक शून्य *v_b)
{
  अक्षर **a = (अक्षर **) v_a;
  अक्षर **b = (अक्षर **) v_b;
  परत म_भेद (*a, *b);
}

स्थिर शून्य
list_entries_users (idx_t n, STRUCT_UTMP स्थिरांक *this)
{
  अक्षर **u = xinmalloc (n, आकार *u);
  idx_t n_entries = 0;

  पर्यंत (n--)
    {
      जर (IS_USER_PROCESS (this))
        {
          अक्षर *trimmed_name;

          trimmed_name = extract_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  क्विक (u, n_entries, आकार (u[0]), userid_compare);

  साठी (idx_t i = 0; i < n_entries; i++)
    {
      अक्षर c = (i < n_entries - 1 ? ' ' : '\n');
      ख_माला_दो (u[i], मानक_निकास);
      अक्षर_दो (c);
    }

  साठी (idx_t i = 0; i < n_entries; i++)
    मुक्त (u[i]);
  मुक्त (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

स्थिर शून्य
users (अक्षर स्थिरांक *filename, पूर्णांक options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  जर (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  मुक्त (utmp_buf);
}

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... [FILE]\n"), program_name);
      म_लिखो (_("\
Output who is currently logged in according to खाता.\n\
If खाता is not specified, use %s.  %s as खाता is common.\n\
\n\
"),
              UTMP_FILE, WTMP_FILE);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (अक्षर स्थिरांक *) रिक्त);

  निवड (argc - optind)
    {
    स्थिती 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      थांबा;

    स्थिती 1:			/* users <utmp file> */
      users (argv[optind], 0);
      थांबा;

    पूर्वनिर्धारित:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (निकास_त्रुटि);
    }

  परत निकास_सफल;
}
