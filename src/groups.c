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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <pwd.h>
#समावेश <grp.h>
#समावेश <getopt.h>

#समावेश "system.h"
#समावेश "group-list.h"
#समावेश "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "groups"

#घोषणा AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("James Youngman")


अटल काष्ठा option स्थिर longopts[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... [USERNAME]...\n"), program_name);
      ख_माला_दो (_("\
Print group memberships क्रम each USERNAME or, अगर no USERNAME is specified, क्रम\
\n\
the current process (which may differ अगर the groups database has changed).\n"),
             मानक_निकास);
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

  /* Processing the arguments this way makes groups.c behave differently to
     groups.sh if one of the arguments is "--".  */
  पूर्णांक optc;
  जबतक ((optc = getopt_long (argc, argv, "", longopts, शून्य)) != -1)
    {
      चयन (optc)
        {
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (निकास_त्रुटि);
        }
    }

  bool ok = true;
  अगर (optind == argc)
    {
      /* No arguments.  Divulge the details of the current process. */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      त्रुटि_सं = 0;
      uid_t ruid = getuid ();
      अगर (ruid == NO_UID && त्रुटि_सं)
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot get real UID"));

      त्रुटि_सं = 0;
      gid_t egid = getegid ();
      अगर (egid == NO_GID && त्रुटि_सं)
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot get effective GID"));

      त्रुटि_सं = 0;
      gid_t rgid = getgid ();
      अगर (rgid == NO_GID && त्रुटि_सं)
        error (निकास_त्रुटि, त्रुटि_सं, _("cannot get real GID"));

      अगर (!print_group_list (शून्य, ruid, rgid, egid, true, ' '))
        ok = false;
      अक्षर_दो ('\n');
    }
  अन्यथा
    {
      /* At least one argument.  Divulge the details of the specified users.  */
      क्रम ( ; optind < argc; optind++)
        {
          काष्ठा passwd *pwd = getpwnam (argv[optind]);
          अगर (pwd == शून्य)
            {
              error (0, 0, _("%s: no such user"), quote (argv[optind]));
              ok = false;
              जारी;
            }
          uid_t ruid = pwd->pw_uid;
          gid_t rgid = pwd->pw_gid;
          gid_t egid = rgid;

          म_लिखो ("%s : ", argv[optind]);
          अगर (!print_group_list (argv[optind], ruid, rgid, egid, true, ' '))
            ok = false;
          अक्षर_दो ('\n');

          अगर (ख_त्रुटि (मानक_निकास))
            write_error ();
        }
    }

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
