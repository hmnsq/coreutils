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

#समावेश <config.h>
#समावेश <stdio.h>

#समावेश <sys/types.h>
#समावेश "system.h"

#समावेश "long-options.h"
#समावेश "quote.h"
#समावेश "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "users"

#घोषणा AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

अटल पूर्णांक
userid_compare (स्थिर व्योम *v_a, स्थिर व्योम *v_b)
{
  अक्षर **a = (अक्षर **) v_a;
  अक्षर **b = (अक्षर **) v_b;
  वापस म_भेद (*a, *b);
}

अटल व्योम
list_entries_users (idx_t n, STRUCT_UTMP स्थिर *this)
{
  अक्षर **u = xinmalloc (n, माप *u);
  idx_t n_entries = 0;

  जबतक (n--)
    {
      अगर (IS_USER_PROCESS (this))
        {
          अक्षर *trimmed_name;

          trimmed_name = extract_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  क्विक (u, n_entries, माप (u[0]), userid_compare);

  क्रम (idx_t i = 0; i < n_entries; i++)
    {
      अक्षर c = (i < n_entries - 1 ? ' ' : '\n');
      ख_माला_दो (u[i], मानक_निकास);
      अक्षर_दो (c);
    }

  क्रम (idx_t i = 0; i < n_entries; i++)
    मुक्त (u[i]);
  मुक्त (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

अटल व्योम
users (अक्षर स्थिर *filename, पूर्णांक options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  अगर (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  मुक्त (utmp_buf);
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (अक्षर स्थिर *) शून्य);

  चयन (argc - optind)
    {
    हाल 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      अवरोध;

    हाल 1:			/* users <utmp file> */
      users (argv[optind], 0);
      अवरोध;

    शेष:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (निकास_त्रुटि);
    }

  वापस निकास_सफल;
}
