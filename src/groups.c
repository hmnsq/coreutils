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
#परिभाषय क्रमादेशः_NAME "groups"

#परिभाषय AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("James Youngman")


स्थिर संरचना option ध्रुव longopts[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION]... [USERNAME]...\n"), क्रमादेशः_name);
      fputs (_("\
Print गणः memberships प्रति each USERNAME or, यदि no USERNAME is specified, प्रति\
\n\
the current process (which may भेदः यदि the groups database has परिवर्तितम्).\n"),
             stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Processing the arguments this way makes groups.c behave differently to
     groups.sh if one of the arguments is "--".  */
  पूर्णाङ्क optc;
  यावत् ((optc = getopt_long (argc, argv, "", longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  bool ok = true;
  यदि (optind == argc)
    {
      /* No arguments.  Divulge the details of the current process. */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      errno = 0;
      uid_t ruid = getuid ();
      यदि (ruid == NO_UID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real UID"));

      errno = 0;
      gid_t egid = getegid ();
      यदि (egid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get effective GID"));

      errno = 0;
      gid_t rgid = getgid ();
      यदि (rgid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real GID"));

      यदि (!print_गणः_list (रिक्त, ruid, rgid, egid, true, ' '))
        ok = false;
      अक्षरं_लिख ('\n');
    }
  अन्यथा
    {
      /* At least one argument.  Divulge the details of the specified users.  */
      प्रति ( ; optind < argc; optind++)
        {
          संरचना passwd *pwd = getpwnam (argv[optind]);
          यदि (pwd == रिक्त)
            {
              error (0, 0, _("%s: no such user"), quote (argv[optind]));
              ok = false;
              अनुवर्त;
            }
          uid_t ruid = pwd->pw_uid;
          gid_t rgid = pwd->pw_gid;
          gid_t egid = rgid;

          प्रारूप_लिख ("%s : ", argv[optind]);
          यदि (!print_गणः_list (argv[optind], ruid, rgid, egid, true, ' '))
            ok = false;
          अक्षरं_लिख ('\n');

          यदि (ferror (stdout))
            write_error ();
        }
    }

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
