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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <pwd.h>
#అన్తర్భావ <grp.h>
#అన్తర్భావ <getopt.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "group-list.h"
#అన్తర్భావ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "groups"

#నిర్వచించు AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("James Youngman")


స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... [USERNAME]...\n"), కార్యక్రమం_name);
      fputs (_("\
Print సమూహం memberships కొరకు each USERNAME or, ఒకవేళ no USERNAME is specified, కొరకు\
\n\
the current process (which may తేడా ఒకవేళ the groups database has మారింది).\n"),
             stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Processing the arguments this way makes groups.c behave differently to
     groups.sh if one of the arguments is "--".  */
  పూర్ణాంకం optc;
  అయితే ((optc = getopt_long (argc, argv, "", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  bool ok = true;
  ఒకవేళ (optind == argc)
    {
      /* No arguments.  Divulge the details of the current process. */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      errno = 0;
      uid_t ruid = getuid ();
      ఒకవేళ (ruid == NO_UID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real UID"));

      errno = 0;
      gid_t egid = getegid ();
      ఒకవేళ (egid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get effective GID"));

      errno = 0;
      gid_t rgid = getgid ();
      ఒకవేళ (rgid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real GID"));

      ఒకవేళ (!print_సమూహం_list (ఖాళీ, ruid, rgid, egid, true, ' '))
        ok = false;
      అక్షరం_రాయి ('\n');
    }
  లేకపోతే
    {
      /* At least one argument.  Divulge the details of the specified users.  */
      కొరకు ( ; optind < argc; optind++)
        {
          నిర్మాణం passwd *pwd = getpwnam (argv[optind]);
          ఒకవేళ (pwd == ఖాళీ)
            {
              error (0, 0, _("%s: no such user"), quote (argv[optind]));
              ok = false;
              కొనసాగు;
            }
          uid_t ruid = pwd->pw_uid;
          gid_t rgid = pwd->pw_gid;
          gid_t egid = rgid;

          రాయి ("%s : ", argv[optind]);
          ఒకవేళ (!print_సమూహం_list (argv[optind], ruid, rgid, egid, true, ' '))
            ok = false;
          అక్షరం_రాయి ('\n');

          ఒకవేళ (ferror (stdout))
            write_error ();
        }
    }

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
