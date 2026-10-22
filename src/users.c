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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>

#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "long-options.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "readutmp.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "users"

#define AUTHORS \
  proper_name ("Joseph Arceneaux"), \
  proper_name ("David MacKenzie")

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
userid_compare (ଅଚଳ ଶୂନ୍ୟ *v_a, ଅଚଳ ଶୂନ୍ୟ *v_b)
{
  ଅକ୍ଷର **a = (ଅକ୍ଷର **) v_a;
  ଅକ୍ଷର **b = (ଅକ୍ଷର **) v_b;
  ଫେରନ୍ତୁ strcmp (*a, *b);
}

ସ୍ଥିର ଶୂନ୍ୟ
list_entries_users (idx_t n, STRUCT_UTMP ଅଚଳ *this)
{
  ଅକ୍ଷର **u = xinmalloc (n, ଆକାର *u);
  idx_t n_entries = 0;

  ଯେତେବେଳେ (n--)
    {
      ଯଦି (IS_USER_PROCESS (this))
        {
          ଅକ୍ଷର *trimmed_name;

          trimmed_name = extract_trimmed_name (this);

          u[n_entries] = trimmed_name;
          ++n_entries;
        }
      this++;
    }

  qsort (u, n_entries, ଆକାର (u[0]), userid_compare);

  ପାଇଁ (idx_t i = 0; i < n_entries; i++)
    {
      ଅକ୍ଷର c = (i < n_entries - 1 ? ' ' : '\n');
      fputs (u[i], stdout);
      putchar (c);
    }

  ପାଇଁ (idx_t i = 0; i < n_entries; i++)
    ମୁକ୍ତ (u[i]);
  ମୁକ୍ତ (u);
}

/* Display a list of users on the system, according to utmp file FILENAME.
   Use read_utmp OPTIONS to read FILENAME.  */

ସ୍ଥିର ଶୂନ୍ୟ
users (ଅକ୍ଷର ଅଚଳ *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା options)
{
  idx_t n_users;
  STRUCT_UTMP *utmp_buf;
  options |= READ_UTMP_USER_PROCESS;
  ଯଦି (read_utmp (filename, &n_users, &utmp_buf, options) != 0)
    error (EXIT_FAILURE, errno, "%s", quotef (filename));

  list_entries_users (n_users, utmp_buf);

  ମୁକ୍ତ (utmp_buf);
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (ଅକ୍ଷର ଅଚଳ *) ଖାଲି);

  ବାଛ (argc - optind)
    {
    ସ୍ଥିତି 0:			/* users */
      users (UTMP_FILE, READ_UTMP_CHECK_PIDS);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି 1:			/* users <utmp file> */
      users (argv[optind], 0);
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:			/* lose */
      error (0, 0, _("extra operand %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ଫେରନ୍ତୁ EXIT_SUCCESS;
}
