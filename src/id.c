/* id -- print real and effective UIDs and GIDs
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

/* Written by Arnold Robbins.
   Major rewrite by David MacKenzie, djm@gnu.ai.mit.edu. */

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <pwd.h>
#समावेश <grp.h>
#समावेश <getopt.h>
#समावेश <selinux/selinux.h>

#समावेश "system.h"
#समावेश "mgetgroups.h"
#समावेश "quote.h"
#समावेश "group-list.h"
#समावेश "smack.h"
#समावेश "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "id"

#घोषणा AUTHORS \
  proper_name ("Arnold Robbins"), \
  proper_name ("David MacKenzie")

/* If nonzero, output only the SELinux context.  */
अटल bool just_context = 0;
/* If true, delimit entries with NUL characters, not whitespace */
अटल bool opt_zero = false;
/* If true, output the list of all group IDs. -G */
अटल bool just_group_list = false;
/* If true, output only the group ID(s). -g */
अटल bool just_group = false;
/* If true, output real UID/GID instead of default effective UID/GID. -r */
अटल bool use_real = false;
/* If true, output only the user ID(s). -u */
अटल bool just_user = false;
/* True unless errors have been encountered.  */
अटल bool ok = true;
/* If true, we are using multiple users. Terminate -G with double NUL. */
अटल bool multiple_users = false;
/* If true, output user/group name instead of ID number. -n */
अटल bool use_name = false;

/* The real and effective IDs of the user to print. */
अटल uid_t ruid, euid;
अटल gid_t rgid, egid;

/* The SELinux context.  Start with a known invalid value so print_full_info
   knows when 'context' has not been set to a meaningful value.  */
अटल अक्षर *context = शून्य;

अटल व्योम print_user (uid_t uid);
अटल व्योम print_full_info (अक्षर स्थिर *username);
अटल व्योम print_stuff (अक्षर स्थिर *pw_name);

अटल काष्ठा option स्थिर longopts[] =
{
  {"context", no_argument, शून्य, 'Z'},
  {"group", no_argument, शून्य, 'g'},
  {"groups", no_argument, शून्य, 'G'},
  {"name", no_argument, शून्य, 'n'},
  {"real", no_argument, शून्य, 'r'},
  {"user", no_argument, शून्य, 'u'},
  {"zero", no_argument, शून्य, 'z'},
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
      म_लिखो (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      ख_माला_दो (_("\
Print user and group information क्रम each specified USER,\n\
or (when USER omitted) क्रम the current process.\n\
\n"),
             मानक_निकास);
      oputs (_("\
  -a\n\
         ignore, क्रम compatibility with other versions\n\
"));
      oputs (_("\
  -Z, --context\n\
         print only the security context of the process\n\
"));
      oputs (_("\
  -g, --group\n\
         print only the effective group ID\n\
"));
      oputs (_("\
  -G, --groups\n\
         print all group IDs\n\
"));
      oputs (_("\
  -n, --name\n\
         print a name instead of a संख्या, क्रम -u, -g, -G\n\
"));
      oputs (_("\
  -r, --real\n\
         print the real ID instead of the effective ID, with -u, -g, -G\n\
"));
      oputs (_("\
  -u, --user\n\
         print only the effective user ID\n\
"));
      oputs (_("\
  -z, --zero\n\
         delimit entries with NUL characters, not whitespace;\n\
         not permitted in शेष format\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Without any OPTION, print some useful set of identified information.\n\
"), मानक_निकास);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक optc;
  पूर्णांक selinux_enabled = (is_selinux_enabled () > 0);
  bool smack_enabled = is_smack_enabled ();

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  जबतक ((optc = getopt_long (argc, argv, "agnruzGZ", longopts, शून्य)) != -1)
    {
      चयन (optc)
        {
        हाल 'a':
          /* Ignore -a, for compatibility with SVR4.  */
          अवरोध;

        हाल 'Z':
          /* politely decline if we're not on a SELinux/SMACK-enabled kernel.
             Keep require_selinux_ in tests in sync with these messages.  */
#अगर_घोषित HAVE_SMACK
          अगर (!selinux_enabled && !smack_enabled)
            error (निकास_त्रुटि, 0,
                   _("--context (-Z) works only on "
                     "an SELinux/SMACK-enabled kernel"));
#अन्यथा
          अगर (!selinux_enabled)
            error (निकास_त्रुटि, 0,
                   _("--context (-Z) works only on an SELinux-enabled kernel"));
#पूर्ण_अगर
          just_context = true;
          अवरोध;

        हाल 'g':
          just_group = true;
          अवरोध;
        हाल 'n':
          use_name = true;
          अवरोध;
        हाल 'r':
          use_real = true;
          अवरोध;
        हाल 'u':
          just_user = true;
          अवरोध;
        हाल 'z':
          opt_zero = true;
          अवरोध;
        हाल 'G':
          just_group_list = true;
          अवरोध;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (निकास_त्रुटि);
        }
    }

  माप_प्रकार n_ids = argc - optind;

  अगर (n_ids && just_context)
    error (निकास_त्रुटि, 0,
           _("cannot print security context when user specified"));

  अगर (just_user + just_group + just_group_list + just_context > 1)
    error (निकास_त्रुटि, 0, _("cannot print \"only\" of more than one choice"));

  bool default_format = ! (just_user
                           || just_group
                           || just_group_list
                           || just_context);

  अगर (default_format && (use_real || use_name))
    error (निकास_त्रुटि, 0,
           _("printing only names or real IDs requires -u, -g, or -G"));

  अगर (default_format && opt_zero)
    error (निकास_त्रुटि, 0,
           _("option --zero not permitted in default format"));

  /* If we are on a SELinux/SMACK-enabled kernel, no user is specified, and
     either --context is specified or none of (-u,-g,-G) is specified,
     and we're not in POSIXLY_CORRECT mode, get our context.  Otherwise,
     leave the context variable alone - it has been initialized to an
     invalid value that will be not displayed in print_full_info().  */
  अगर (n_ids == 0
      && (just_context
          || (default_format && ! दो_पर्या ("POSIXLY_CORRECT"))))
    {
      /* Report failure only if --context (-Z) was explicitly requested.  */
      अगर ((selinux_enabled && getcon (&context) && just_context)
          || (smack_enabled
              && smack_new_label_from_self (&context) < 0
              && just_context))
        error (निकास_त्रुटि, 0, _("can't get process context"));
    }

  अगर (n_ids >= 1)
    {
      multiple_users = n_ids > 1 ? true : false;
      /* Changing the value of n_ids to the last index in the array where we
         have the last possible user id. This helps us because we don't have
         to declare a different variable to keep a track of where the
         last username lies in argv[].  */
      n_ids += optind;
      /* For each username/userid to get its pw_name field */
      क्रम (; optind < n_ids; optind++)
        {
          अक्षर *pw_name = शून्य;
          काष्ठा passwd *pwd = शून्य;
          अक्षर स्थिर *spec = argv[optind];
          /* Disallow an empty spec here as parse_user_spec() doesn't
             give an error for that as it seems it's a valid way to
             specify a noop or "reset special bits" depending on the system.  */
          अगर (*spec)
            {
              अगर (! parse_user_spec (spec, &euid, शून्य, &pw_name, शून्य))
                pwd = pw_name ? getpwnam (pw_name) : getpwuid (euid);
            }
          अगर (pwd == शून्य)
            {
              error (0, त्रुटि_सं, _("%s: no such user"), quote (spec));
              ok &= false;
            }
          अन्यथा
            {
              अगर (!pw_name)
                pw_name = xstrdup (pwd->pw_name);
              ruid = euid = pwd->pw_uid;
              rgid = egid = pwd->pw_gid;
              print_stuff (pw_name);
              अगर (ख_त्रुटि (मानक_निकास))
                write_error ();
            }
          मुक्त (pw_name);
        }
    }
  अन्यथा
    {
      /* POSIX says identification functions (getuid, getgid, and
         others) cannot fail, but they can fail under GNU/Hurd and a
         few other systems.  Test for failure by checking errno.  */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      अगर (just_user ? !use_real
          : !just_group && !just_group_list && !just_context)
        {
          त्रुटि_सं = 0;
          euid = geteuid ();
          अगर (euid == NO_UID && त्रुटि_सं)
            error (निकास_त्रुटि, त्रुटि_सं, _("cannot get effective UID"));
        }

      अगर (just_user ? use_real
          : !just_group && (just_group_list || !just_context))
        {
          त्रुटि_सं = 0;
          ruid = getuid ();
          अगर (ruid == NO_UID && त्रुटि_सं)
            error (निकास_त्रुटि, त्रुटि_सं, _("cannot get real UID"));
        }

      अगर (!just_user && (just_group || just_group_list || !just_context))
        {
          त्रुटि_सं = 0;
          egid = getegid ();
          अगर (egid == NO_GID && त्रुटि_सं)
            error (निकास_त्रुटि, त्रुटि_सं, _("cannot get effective GID"));

          त्रुटि_सं = 0;
          rgid = getgid ();
          अगर (rgid == NO_GID && त्रुटि_सं)
            error (निकास_त्रुटि, त्रुटि_सं, _("cannot get real GID"));
        }
        print_stuff (शून्य);
    }

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}

/* Print the name or value of user ID UID. */

अटल व्योम
print_user (uid_t uid)
{
  काष्ठा passwd *pwd = शून्य;

  अगर (use_name)
    {
      pwd = getpwuid (uid);
      अगर (pwd == शून्य)
        {
          error (0, 0, _("cannot find name for user ID %ju"), (uintmax_t) uid);
          ok &= false;
        }
    }

  अगर (pwd)
    म_लिखो ("%s", pwd->pw_name);
  अन्यथा
    म_लिखो ("%ju", (uintmax_t) uid);
}

/* Print all of the info about the user's user and group IDs. */

अटल व्योम
print_full_info (अक्षर स्थिर *username)
{
  काष्ठा passwd *pwd;
  काष्ठा group *grp;

  म_लिखो (_("uid=%ju"), (uintmax_t) ruid);
  pwd = getpwuid (ruid);
  अगर (pwd)
    म_लिखो ("(%s)", pwd->pw_name);

  म_लिखो (_(" gid=%ju"), (uintmax_t) rgid);
  grp = getgrgid (rgid);
  अगर (grp)
    म_लिखो ("(%s)", grp->gr_name);

  अगर (euid != ruid)
    {
      म_लिखो (_(" euid=%ju"), (uintmax_t) euid);
      pwd = getpwuid (euid);
      अगर (pwd)
        म_लिखो ("(%s)", pwd->pw_name);
    }

  अगर (egid != rgid)
    {
      म_लिखो (_(" egid=%ju"), (uintmax_t) egid);
      grp = getgrgid (egid);
      अगर (grp)
        म_लिखो ("(%s)", grp->gr_name);
    }

  {
    gid_t *groups;

    gid_t primary_group;
    अगर (username)
      primary_group = pwd ? pwd->pw_gid : -1;
    अन्यथा
      primary_group = egid;

    पूर्णांक n_groups = xgetgroups (username, primary_group, &groups);
    अगर (n_groups < 0)
      {
        अगर (username)
          error (0, त्रुटि_सं, _("failed to get groups for user %s"),
                 quote (username));
        अन्यथा
          error (0, त्रुटि_सं, _("failed to get groups for the current process"));
        ok &= false;
        वापस;
      }

    अगर (n_groups > 0)
      ख_माला_दो (_(" groups="), मानक_निकास);
    क्रम (पूर्णांक i = 0; i < n_groups; i++)
      {
        अगर (i > 0)
          अक्षर_दो (',');
        म_लिखो ("%ju", (uintmax_t) groups[i]);
        grp = getgrgid (groups[i]);
        अगर (grp)
          म_लिखो ("(%s)", grp->gr_name);
      }
    मुक्त (groups);
  }

  /* POSIX mandates the precise output format, and that it not include
     any context=... part, so skip that if POSIXLY_CORRECT is set.  */
  अगर (context)
    म_लिखो (_(" context=%s"), context);
}

/* Print information about the user based on the arguments passed. */

अटल व्योम
print_stuff (अक्षर स्थिर *pw_name)
{
  अगर (just_user)
      print_user (use_real ? ruid : euid);

  /* print_group and print_group_list return true on successful
     execution but false if something goes wrong. We then AND this value with
     the current value of 'ok' because we want to know if one of the previous
     users faced a problem in these functions. This value of 'ok' is later used
     to understand what status program should exit with. */
  अन्यथा अगर (just_group)
    ok &= print_group (use_real ? rgid : egid, use_name);
  अन्यथा अगर (just_group_list)
    ok &= print_group_list (pw_name, ruid, rgid, egid,
                            use_name, opt_zero ? '\0' : ' ');
  अन्यथा अगर (just_context)
    ख_माला_दो (context, मानक_निकास);
  अन्यथा
    print_full_info (pw_name);

  /* When printing records for more than 1 user, at the end of groups
     of each user terminate the record with two consequent NUL characters
     to make parsing and distinguishing between two records possible. */
  अगर (opt_zero && just_group_list && multiple_users)
    {
      अक्षर_दो ('\0');
      अक्षर_दो ('\0');
    }
  अन्यथा
    {
      अक्षर_दो (opt_zero ? '\0' : '\n');
    }
}
