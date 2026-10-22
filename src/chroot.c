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

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <grp.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "ignore-value.h"
#अन्तर्भाव "mgetgroups.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "userspec.h"
#अन्तर्भाव "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chroot"

#define AUTHORS proper_name ("Roland McGrath")

#ifndef MAXGID
# define MAXGID GID_T_MAX
#endif

स्थिर अन्तःस्थ bool uid_unset (uid_t uid) { प्रत्यागम uid == (uid_t) -1; }
स्थिर अन्तःस्थ bool gid_unset (gid_t gid) { प्रत्यागम gid == (gid_t) -1; }
#define uid_set(x) (!uid_unset (x))
#define gid_set(x) (!gid_unset (x))

गणना
{
  GROUPS = अच_अक्षर_उच्च + 1,
  USERSPEC,
  SKIP_CHDIR
};

स्थिर संरचना option ध्रुव long_opts[] =
{
  {"groups", required_argument, रिक्त, GROUPS},
  {"userspec", required_argument, रिक्त, USERSPEC},
  {"skip-chdir", no_argument, रिक्त, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

#यदि ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
स्थिर पूर्णाङ्क
setgroups (माप_प्रकार size, MAYBE_UNUSED gid_t ध्रुव *list)
{
  यदि (size == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      प्रत्यागम 0;
    }
  अन्यथा
    {
      त्रुटि_सं = ENOTSUP;
      प्रत्यागम -1;
    }
}
#endif

/* Determine the group IDs for the specified supplementary GROUPS,
   which is a comma separated list of supplementary groups (names or numbers).
   Allocate an array for the parsed IDs and store it in PGIDS,
   which may be allocated even on parse failure.
   Update the number of parsed groups in PN_GIDS on success.
   Upon any failure return nonzero, and issue diagnostic if SHOW_ERRORS is true.
   Otherwise return zero.  */

स्थिर पूर्णाङ्क
parse_additional_groups (वर्ण ध्रुव *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = रिक्त;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  वर्ण *buffer = xstrdup (groups);
  पूर्णाङ्क ret = 0;

  प्रति (वर्ण ध्रुव *tmp = म_मोहर (buffer, ","); tmp;
       tmp = म_मोहर (रिक्त, ","))
    {
      संरचना group *g;
      uintmax_t मूल्य;

      यदि (xstrtoumax (tmp, रिक्त, 10, &मूल्य, "") == LONGINT_OK
          && मूल्य <= MAXGID)
        {
          यावत् (है_खाली (to_uchar (*tmp)))
            tmp++;
          यदि (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              यदि (g != रिक्त)
                मूल्य = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (संरचना group *) (intptr_t) ! रिक्त;
        }
      अन्यथा
        {
          g = getgrnam (tmp);
          यदि (g != रिक्त)
            मूल्य = g->gr_gid;
        }

      यदि (g == रिक्त)
        {
          ret = -1;

          यदि (show_errors)
            {
              error (0, त्रुटि_सं, _("invalid group %s"), quote (tmp));
              अनुवर्त;
            }

          भङ्ग;
        }

      यदि (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, परिमाण *gids);
      gids[n_gids++] = मूल्य;
    }

  यदि (ret == 0 && n_gids == 0)
    {
      यदि (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  यदि (ret == 0)
    *pn_gids = n_gids;

  मुक्त (buffer);
  प्रत्यागम ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

स्थिर bool
is_मूल (वर्ण ध्रुव *dir)
{
  वर्ण *resolved = canonicalize_file_name (dir);
  bool is_res_मूल = resolved && streq ("/", resolved);
  मुक्त (resolved);
  प्रत्यागम is_res_मूल;
}

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
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
         कुरु not change working directory to %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
If no command is given, run '\"$SHELL\" -i' (स्वभाव: '/bin/sh -i').\n\
"), मानक_निकास);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क c;

  /* Input user and groups spec.  */
  वर्ण *userspec = रिक्त;
  वर्ण ध्रुव *username = रिक्त;
  वर्ण ध्रुव *groups = रिक्त;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *out_gids = रिक्त;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  निकास_पर (close_stdout);

  यावत् ((c = getopt_long (argc, argv, "+", long_opts, रिक्त)) != -1)
    {
      विकल्प (c)
        {
        पक्ष USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = म_माप (userspec);
            यदि (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            भङ्ग;
          }

        पक्ष GROUPS:
          groups = optarg;
          भङ्ग;

        पक्ष SKIP_CHDIR:
          skip_chdir = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_CANCELED);
        }
    }

  यदि (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  वर्ण ध्रुव *newroot = argv[optind];
  bool is_oldroot = is_मूल (newroot);

  यदि (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  यदि (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      यदि (userspec)
        ignore_मूल्य (parse_user_spec (userspec, &uid, &gid, रिक्त, रिक्त));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      यदि (uid_set (uid) && (! groups || gid_unset (gid)))
        {
          ध्रुव संरचना passwd *pwd;
          यदि ((pwd = getpwuid (uid)))
            {
              यदि (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      यदि (groups && *groups)
        ignore_मूल्य (parse_additional_groups (groups, &out_gids, &n_gids,
                                               false));
#यदि HAVE_SETGROUPS
      अन्यथा यदि (! groups && gid_set (gid) && username)
        {
          पूर्णाङ्क ngroups = xgetgroups (username, gid, &out_gids);
          यदि (0 < ngroups)
            n_gids = ngroups;
        }
#endif
    }

  यदि (chroot (newroot) != 0)
    error (EXIT_CANCELED, त्रुटि_सं, _("cannot change root directory to %s"),
           quoteaf (newroot));

  यदि (! skip_chdir && स_बदलो ("/"))
    error (EXIT_CANCELED, त्रुटि_सं, _("cannot chdir to root directory"));

  यदि (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      वर्ण *shell = दो_पर्या ("SHELL");
      यदि (shell == रिक्त)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = रिक्त;
    }
  अन्यथा
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  यदि (userspec)
    {
      bool warn;
      वर्ण ध्रुव *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              रिक्त, रिक्त, &warn);
      यदि (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  यदि (uid_set (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      ध्रुव संरचना passwd *pwd;
      यदि ((pwd = getpwuid (uid)))
        {
          यदि (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      अन्यथा यदि (gid_unset (gid))
        {
          error (EXIT_CANCELED, त्रुटि_सं,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = out_gids;
  GETGROUPS_T *in_gids = रिक्त;
  यदि (groups && *groups)
    {
      यदि (parse_additional_groups (groups, &in_gids, &n_gids, !n_gids) != 0)
        {
          यदि (! n_gids)
            प्रत्यागम EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      अन्यथा
        gids = in_gids;
    }
#यदि HAVE_SETGROUPS
  अन्यथा यदि (! groups && gid_set (gid) && username)
    {
      पूर्णाङ्क ngroups = xgetgroups (username, gid, &in_gids);
      यदि (ngroups <= 0)
        {
          यदि (! n_gids)
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
#endif

  यदि ((uid_set (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, त्रुटि_सं, _("failed to set supplemental groups"));

  मुक्त (in_gids);
  मुक्त (out_gids);

  यदि (gid_set (gid) && setgid (gid))
    error (EXIT_CANCELED, त्रुटि_सं, _("failed to set group-ID"));

  यदि (uid_set (uid) && setuid (uid))
    error (EXIT_CANCELED, त्रुटि_सं, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  पूर्णाङ्क exit_status = त्रुटि_सं == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, त्रुटि_सं, _("failed to run command %s"), quote (argv[0]));
  प्रत्यागम exit_status;
}
