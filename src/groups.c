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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <grp.h>
#अन्तर्भाव <getopt.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "group-list.h"
#अन्तर्भाव "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "groups"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("James Youngman")


स्थिर संरचना option स्थिरांक longopts[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... [USERNAME]...\n"), program_name);
      ख_माला_दो (_("\
Print group memberships साठी each USERNAME or, जर no USERNAME is specified, साठी\
\n\
the current process (which may differ जर the groups database has changed).\n"),
             मानक_निकास);
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

  /* Processing the arguments this way makes groups.c behave differently to
     groups.sh if one of the arguments is "--".  */
  पूर्णांक optc;
  पर्यंत ((optc = getopt_long (argc, argv, "", longopts, रिक्त)) != -1)
    {
      निवड (optc)
        {
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  bool ok = true;
  जर (optind == argc)
    {
      /* No arguments.  Divulge the details of the current process. */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      त्रुटि_सं = 0;
      uid_t ruid = getuid ();
      जर (ruid == NO_UID && त्रुटि_सं)
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot get real UID"));

      त्रुटि_सं = 0;
      gid_t egid = getegid ();
      जर (egid == NO_GID && त्रुटि_सं)
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot get effective GID"));

      त्रुटि_सं = 0;
      gid_t rgid = getgid ();
      जर (rgid == NO_GID && त्रुटि_सं)
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot get real GID"));

      जर (!print_group_list (रिक्त, ruid, rgid, egid, true, ' '))
        ok = false;
      अक्षर_दो ('\n');
    }
  अन्यथा
    {
      /* At least one argument.  Divulge the details of the specified users.  */
      साठी ( ; optind < argc; optind++)
        {
          संरचना passwd *pwd = getpwnam (argv[optind]);
          जर (pwd == रिक्त)
            {
              error (0, 0, _("%s: no such user"), quote (argv[optind]));
              ok = false;
              सुरूठेवा;
            }
          uid_t ruid = pwd->pw_uid;
          gid_t rgid = pwd->pw_gid;
          gid_t egid = rgid;

          म_लिखो ("%s : ", argv[optind]);
          जर (!print_group_list (argv[optind], ruid, rgid, egid, true, ' '))
            ok = false;
          अक्षर_दो ('\n');

          जर (ख_त्रुटि (मानक_निकास))
            write_error ();
        }
    }

  परत ok ? निकास_सफल : निकास_त्रुटि;
}
