/* chroot -- run command or shell with special root directory
   Copyright (C) 1995-2026 Free Software Foundation, Inc.

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

/* Written by Roland McGrath.  */

#समावेश <config.h>
#समावेश <getopt.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#समावेश <pwd.h>
#समावेश <grp.h>

#समावेश "system.h"
#समावेश "ignore-value.h"
#समावेश "mgetgroups.h"
#समावेश "quote.h"
#समावेश "userspec.h"
#समावेश "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "chroot"

#घोषणा AUTHORS proper_name ("Roland McGrath")

#अगर_अघोषित MAXGID
# define MAXGID GID_T_MAX
#पूर्ण_अगर

अटल अंतरभूत bool uid_unset (uid_t uid) { वापस uid == (uid_t) -1; }
अटल अंतरभूत bool gid_unset (gid_t gid) { वापस gid == (gid_t) -1; }
#घोषणा uid_set(x) (!uid_unset (x))
#घोषणा gid_set(x) (!gid_unset (x))

क्रमागत
{
  GROUPS = अच_अक्षर_उच्च + 1,
  USERSPEC,
  SKIP_CHDIR
};

अटल काष्ठा option स्थिर long_opts[] =
{
  {"groups", required_argument, शून्य, GROUPS},
  {"userspec", required_argument, शून्य, USERSPEC},
  {"skip-chdir", no_argument, शून्य, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

#अगर ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
अटल पूर्णांक
setgroups (माप_प्रकार आकार, MAYBE_UNUSED gid_t स्थिर *list)
{
  अगर (आकार == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      वापस 0;
    }
  अन्यथा
    {
      त्रुटि_सं = ENOTSUP;
      वापस -1;
    }
}
#पूर्ण_अगर

/* Determine the group IDs for the specified supplementary GROUPS,
   which is a comma separated list of supplementary groups (names or numbers).
   Allocate an array for the parsed IDs and store it in PGIDS,
   which may be allocated even on parse failure.
   Update the number of parsed groups in PN_GIDS on success.
   Upon any failure return nonzero, and issue diagnostic if SHOW_ERRORS is true.
   Otherwise return zero.  */

अटल पूर्णांक
parse_additional_groups (अक्षर स्थिर *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = शून्य;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  अक्षर *बफर = xstrdup (groups);
  पूर्णांक ret = 0;

  क्रम (अक्षर स्थिर *tmp = म_मोहर (बफर, ","); tmp;
       tmp = म_मोहर (शून्य, ","))
    {
      काष्ठा group *g;
      uintmax_t मूल्य;

      अगर (xstrtoumax (tmp, शून्य, 10, &मूल्य, "") == LONGINT_OK
          && मूल्य <= MAXGID)
        {
          जबतक (है_खाली (to_uchar (*tmp)))
            tmp++;
          अगर (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              अगर (g != शून्य)
                मूल्य = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (काष्ठा group *) (intptr_t) ! शून्य;
        }
      अन्यथा
        {
          g = getgrnam (tmp);
          अगर (g != शून्य)
            मूल्य = g->gr_gid;
        }

      अगर (g == शून्य)
        {
          ret = -1;

          अगर (show_errors)
            {
              error (0, त्रुटि_सं, _("invalid group %s"), quote (tmp));
              जारी;
            }

          अवरोध;
        }

      अगर (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, माप *gids);
      gids[n_gids++] = मूल्य;
    }

  अगर (ret == 0 && n_gids == 0)
    {
      अगर (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  अगर (ret == 0)
    *pn_gids = n_gids;

  मुक्त (बफर);
  वापस ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

अटल bool
is_मूल (अक्षर स्थिर *dir)
{
  अक्षर *resolved = canonicalize_file_name (dir);
  bool is_res_मूल = resolved && streq ("/", resolved);
  मुक्त (resolved);
  वापस is_res_मूल;
}

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), program_name);

      ख_माला_दो (_("\
Run COMMAND with मूल directory set to NEWROOT.\n\
\n\
"), मानक_निकास);

      oputs (_("\
      --groups=G_LIST\n\
         specify supplementary groups as g1,g2,..,gN\n\
"));
      oputs (_("\
      --userspec=USER:GROUP\n\
         specify user and group (ID or name) to use\n\
"));
      oprintf (_("\
      --skip-स_बदलो\n\
         करो not change working directory to %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
If no command is given, run '\"$SHELL\" -i' (शेष: '/bin/sh -i').\n\
"), मानक_निकास);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक c;

  /* Input user and groups spec.  */
  अक्षर *userspec = शून्य;
  अक्षर स्थिर *username = शून्य;
  अक्षर स्थिर *groups = शून्य;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *out_gids = शून्य;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  निकास_पर (close_stdout);

  जबतक ((c = getopt_long (argc, argv, "+", long_opts, शून्य)) != -1)
    {
      चयन (c)
        {
        हाल USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = म_माप (userspec);
            अगर (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            अवरोध;
          }

        हाल GROUPS:
          groups = optarg;
          अवरोध;

        हाल SKIP_CHDIR:
          skip_chdir = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (EXIT_CANCELED);
        }
    }

  अगर (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  अक्षर स्थिर *newroot = argv[optind];
  bool is_oldroot = is_मूल (newroot);

  अगर (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  अगर (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      अगर (userspec)
        ignore_मूल्य (parse_user_spec (userspec, &uid, &gid, शून्य, शून्य));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      अगर (uid_set (uid) && (! groups || gid_unset (gid)))
        {
          स्थिर काष्ठा passwd *pwd;
          अगर ((pwd = getpwuid (uid)))
            {
              अगर (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      अगर (groups && *groups)
        ignore_मूल्य (parse_additional_groups (groups, &out_gids, &n_gids,
                                               false));
#अगर HAVE_SETGROUPS
      अन्यथा अगर (! groups && gid_set (gid) && username)
        {
          पूर्णांक ngroups = xgetgroups (username, gid, &out_gids);
          अगर (0 < ngroups)
            n_gids = ngroups;
        }
#पूर्ण_अगर
    }

  अगर (chroot (newroot) != 0)
    error (EXIT_CANCELED, त्रुटि_सं, _("cannot change root directory to %s"),
           quoteaf (newroot));

  अगर (! skip_chdir && स_बदलो ("/"))
    error (EXIT_CANCELED, त्रुटि_सं, _("cannot chdir to root directory"));

  अगर (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      अक्षर *shell = दो_पर्या ("SHELL");
      अगर (shell == शून्य)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = शून्य;
    }
  अन्यथा
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  अगर (userspec)
    {
      bool warn;
      अक्षर स्थिर *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              शून्य, शून्य, &warn);
      अगर (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  अगर (uid_set (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      स्थिर काष्ठा passwd *pwd;
      अगर ((pwd = getpwuid (uid)))
        {
          अगर (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      अन्यथा अगर (gid_unset (gid))
        {
          error (EXIT_CANCELED, त्रुटि_सं,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = out_gids;
  GETGROUPS_T *in_gids = शून्य;
  अगर (groups && *groups)
    {
      अगर (parse_additional_groups (groups, &in_gids, &n_gids, !n_gids) != 0)
        {
          अगर (! n_gids)
            वापस EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      अन्यथा
        gids = in_gids;
    }
#अगर HAVE_SETGROUPS
  अन्यथा अगर (! groups && gid_set (gid) && username)
    {
      पूर्णांक ngroups = xgetgroups (username, gid, &in_gids);
      अगर (ngroups <= 0)
        {
          अगर (! n_gids)
            error (EXIT_CANCELED, त्रुटि_सं,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      अन्यथा
        {
          n_gids = ngroups;
          gids = in_gids;
        }
    }
#पूर्ण_अगर

  अगर ((uid_set (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, त्रुटि_सं, _("failed to set supplemental groups"));

  मुक्त (in_gids);
  मुक्त (out_gids);

  अगर (gid_set (gid) && setgid (gid))
    error (EXIT_CANCELED, त्रुटि_सं, _("failed to set group-ID"));

  अगर (uid_set (uid) && setuid (uid))
    error (EXIT_CANCELED, त्रुटि_सं, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  पूर्णांक exit_status = त्रुटि_सं == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, त्रुटि_सं, _("failed to run command %s"), quote (argv[0]));
  वापस exit_status;
}
