/* whoami -- print effective userid

   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Equivalent to 'id -un'. */
/* Written by Richard Mlynarik. */

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <pwd.h>

#ضمن "system.h"
#ضمن "long-options.h"
#ضمن "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "whoami"

#define AUTHORS proper_name ("Richard Mlynarik")

فراغ
usage (صحيح status)
{
  אם (status != EXIT_SUCCESS)
    emit_try_help ();
  אחרת
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);
      fputs (_("\
Print the user name associated with the current effective user ID.\n\
Same as id -un.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحيح
main (صحيح argc, حرف **argv)
{
  uid_t NO_UID = -1;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, PROGRAM_NAME, PACKAGE_NAME,
                                   Version, true, usage, AUTHORS,
                                   (حرف ثابت *) عدم);

  אם (optind != argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  errno = 0;
  uid_t uid = geteuid ();
  بنية passwd *pw = uid == NO_UID && errno ? عدم : getpwuid (uid);
  אם (!pw)
    error (EXIT_FAILURE, errno, _("cannot find name for user ID %ju"),
           (uintmax_t) uid);
  puts (pw->pw_name);
  החזר EXIT_SUCCESS;
}
