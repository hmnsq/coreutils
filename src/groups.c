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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <pwd.h>
#شامل <grp.h>
#شامل <getopt.h>

#شامل "system.h"
#شامل "group-list.h"
#شامل "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "groups"

#تعریف_کرو AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("James Youngman")


ساکن ساخت option ثابت longopts[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... [USERNAME]...\n"), پروگرام_name);
      fputs (_("\
Print گروہ memberships برائے each USERNAME or, اگر no USERNAME is specified, برائے\
\n\
the current process (which may فرق اگر the groups database has بدلا).\n"),
             stdout);
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

  /* Processing the arguments this way makes groups.c behave differently to
     groups.sh if one of the arguments is "--".  */
  صحیح optc;
  جب ((optc = getopt_long (argc, argv, "", longopts, تہی)) != -1)
    {
      انتخاب (optc)
        {
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  bool ok = true;
  اگر (optind == argc)
    {
      /* No arguments.  Divulge the details of the current process. */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      errno = 0;
      uid_t ruid = getuid ();
      اگر (ruid == NO_UID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real UID"));

      errno = 0;
      gid_t egid = getegid ();
      اگر (egid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get effective GID"));

      errno = 0;
      gid_t rgid = getgid ();
      اگر (rgid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real GID"));

      اگر (!print_گروہ_list (تہی, ruid, rgid, egid, true, ' '))
        ok = false;
      حرف_لکھو ('\n');
    }
  ورنہ
    {
      /* At least one argument.  Divulge the details of the specified users.  */
      برائے ( ; optind < argc; optind++)
        {
          ساخت passwd *pwd = getpwnam (argv[optind]);
          اگر (pwd == تہی)
            {
              error (0, 0, _("%s: no such user"), quote (argv[optind]));
              ok = false;
              جاری;
            }
          uid_t ruid = pwd->pw_uid;
          gid_t rgid = pwd->pw_gid;
          gid_t egid = rgid;

          لکھو ("%s : ", argv[optind]);
          اگر (!print_گروہ_list (argv[optind], ruid, rgid, egid, true, ' '))
            ok = false;
          حرف_لکھو ('\n');

          اگر (ferror (stdout))
            write_error ();
        }
    }

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
