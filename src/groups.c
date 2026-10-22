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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <pwd.h>
#অন্তর্ভাব <grp.h>
#অন্তর্ভাব <getopt.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "group-list.h"
#অন্তর্ভাব "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "groups"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("James Youngman")


স্থির সংরচনা option ধ্রুব longopts[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... [USERNAME]...\n"), program_name);
      fputs (_("\
Print group memberships প্রতি each USERNAME or, যদি no USERNAME is specified, প্রতি\
\n\
the current process (which may differ যদি the groups database has changed).\n"),
             stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Processing the arguments this way makes groups.c behave differently to
     groups.sh if one of the arguments is "--".  */
  পূর্ণাঙ্ক optc;
  যাবত্ ((optc = getopt_long (argc, argv, "", longopts, রিক্ত)) != -1)
    {
      বিকল্প (optc)
        {
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্বভাব:
          usage (EXIT_FAILURE);
        }
    }

  bool ok = true;
  যদি (optind == argc)
    {
      /* No arguments.  Divulge the details of the current process. */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      errno = 0;
      uid_t ruid = getuid ();
      যদি (ruid == NO_UID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real UID"));

      errno = 0;
      gid_t egid = getegid ();
      যদি (egid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get effective GID"));

      errno = 0;
      gid_t rgid = getgid ();
      যদি (rgid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real GID"));

      যদি (!print_group_list (রিক্ত, ruid, rgid, egid, true, ' '))
        ok = false;
      putchar ('\n');
    }
  অন্যথা
    {
      /* At least one argument.  Divulge the details of the specified users.  */
      প্রতি ( ; optind < argc; optind++)
        {
          সংরচনা passwd *pwd = getpwnam (argv[optind]);
          যদি (pwd == রিক্ত)
            {
              error (0, 0, _("%s: no such user"), quote (argv[optind]));
              ok = false;
              অনুবর্ত;
            }
          uid_t ruid = pwd->pw_uid;
          gid_t rgid = pwd->pw_gid;
          gid_t egid = rgid;

          printf ("%s : ", argv[optind]);
          যদি (!print_group_list (argv[optind], ruid, rgid, egid, true, ' '))
            ok = false;
          putchar ('\n');

          যদি (ferror (stdout))
            write_error ();
        }
    }

  প্রত্যাগম ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
