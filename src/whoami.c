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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <pwd.h>

#شامل "system.h"
#شامل "long-options.h"
#شامل "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "whoami"

#تعریف_کرو AUTHORS proper_name ("Richard Mlynarik")

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]...\n"), پروگرام_name);
      fputs (_("\
Print the user name associated with the current effective user ID.\n\
Same as id -un.\n\
\n\
"), stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  uid_t NO_UID = -1;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  parse_gnu_standard_options_only (argc, argv, پروگرام_NAME, PACKAGE_NAME,
                                   نسخہ, true, usage, AUTHORS,
                                   (حرف ثابت *) تہی);

  اگر (optind != argc)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  errno = 0;
  uid_t uid = geteuid ();
  ساخت passwd *pw = uid == NO_UID && errno ? تہی : getpwuid (uid);
  اگر (!pw)
    error (EXIT_FAILURE, errno, _("cannot find name for user ID %ju"),
           (uintmax_t) uid);
  puts (pw->pw_name);
  واپس EXIT_SUCCESS;
}
