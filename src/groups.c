/* groups -- print the groups a user is in
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

/* Written by James Youngman based on id.c and groups.sh,
   which were written by Arnold Robbins and David MacKenzie. */

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <pwd.h>
#ضمن <grp.h>
#ضمن <getopt.h>

#ضمن "system.h"
#ضمن "group-list.h"
#ضمن "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "groups"

#تعریف_کرو AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("James Youngman")


ساكن بنية option ثابت longopts[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [OPTION]... [USERNAME]...\n"), پروگرام_name);
      fputs (_("\
Print گروہ memberships لكل each USERNAME or, إذا no USERNAME is specified, لكل\
\n\
the current process (which may فرق إذا the groups database has بدلیا).\n"),
             stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Processing the arguments this way makes groups.c behave differently to
     groups.sh if one of the arguments is "--".  */
  صحيح optc;
  طالما ((optc = getopt_long (argc, argv, "", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  bool ok = true;
  إذا (optind == argc)
    {
      /* No arguments.  Divulge the details of the current process. */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      errno = 0;
      uid_t ruid = getuid ();
      إذا (ruid == NO_UID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real UID"));

      errno = 0;
      gid_t egid = getegid ();
      إذا (egid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get effective GID"));

      errno = 0;
      gid_t rgid = getgid ();
      إذا (rgid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real GID"));

      إذا (!print_گروہ_list (عدم, ruid, rgid, egid, true, ' '))
        ok = false;
      حرف_لکھو ('\n');
    }
  وإلا
    {
      /* At least one argument.  Divulge the details of the specified users.  */
      لكل ( ; optind < argc; optind++)
        {
          بنية passwd *pwd = getpwnam (argv[optind]);
          إذا (pwd == عدم)
            {
              error (0, 0, _("%s: no such user"), quote (argv[optind]));
              ok = false;
              استمر;
            }
          uid_t ruid = pwd->pw_uid;
          gid_t rgid = pwd->pw_gid;
          gid_t egid = rgid;

          لکھو ("%s : ", argv[optind]);
          إذا (!print_گروہ_list (argv[optind], ruid, rgid, egid, true, ' '))
            ok = false;
          حرف_لکھو ('\n');

          إذا (ferror (stdout))
            write_error ();
        }
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
