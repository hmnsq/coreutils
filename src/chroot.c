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

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <pwd.h>
#అన్తర్భావ <grp.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "ignore-value.h"
#అన్తర్భావ "mgetgroups.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "userspec.h"
#అన్తర్భావ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "chroot"

#నిర్వచించు AUTHORS proper_name ("Roland McGrath")

#నిర్వచితం_కాకపోతే MAXGID
# define MAXGID GID_T_MAX
#అయితే_ముగింపు

స్థిర అంతఃవరుస bool uid_unset (uid_t uid) { తిరిగి uid == (uid_t) -1; }
స్థిర అంతఃవరుస bool gid_unset (gid_t gid) { తిరిగి gid == (gid_t) -1; }
#నిర్వచించు uid_సమితి(x) (!uid_unset (x))
#నిర్వచించు gid_సమితి(x) (!gid_unset (x))

గణన
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

స్థిర నిర్మాణం option స్థిరరాశి long_opts[] =
{
  {"groups", required_వాదం, ఖాళీ, GROUPS},
  {"userspec", required_వాదం, ఖాళీ, USERSPEC},
  {"skip-chdir", no_వాదం, ఖాళీ, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

#ఒకవేళ ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
స్థిర పూర్ణాంకం
setgroups (size_t కొలత, MAYBE_వాడని gid_t స్థిరరాశి *list)
{
  ఒకవేళ (కొలత == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      తిరిగి 0;
    }
  లేకపోతే
    {
      errno = ENOTSUP;
      తిరిగి -1;
    }
}
#అయితే_ముగింపు

/* Determine the group IDs for the specified supplementary GROUPS,
   which is a comma separated list of supplementary groups (names or numbers).
   Allocate an array for the parsed IDs and store it in PGIDS,
   which may be allocated even on parse failure.
   Update the number of parsed groups in PN_GIDS on success.
   Upon any failure return nonzero, and issue diagnostic if SHOW_ERRORS is true.
   Otherwise return zero.  */

స్థిర పూర్ణాంకం
parse_additional_groups (అక్షరం స్థిరరాశి *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = ఖాళీ;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  అక్షరం *బఫరు = xstrdup (groups);
  పూర్ణాంకం ret = 0;

  కొరకు (అక్షరం స్థిరరాశి *tmp = strtok (బఫరు, ","); tmp;
       tmp = strtok (ఖాళీ, ","))
    {
      నిర్మాణం సమూహం *g;
      uintmax_t విలువ;

      ఒకవేళ (xstrtoumax (tmp, ఖాళీ, 10, &విలువ, "") == LONGINT_OK
          && విలువ <= MAXGID)
        {
          అయితే (ఖాళీనా (గమ్యం_uchar (*tmp)))
            tmp++;
          ఒకవేళ (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              ఒకవేళ (g != ఖాళీ)
                విలువ = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (నిర్మాణం సమూహం *) (intptr_t) ! ఖాళీ;
        }
      లేకపోతే
        {
          g = getgrnam (tmp);
          ఒకవేళ (g != ఖాళీ)
            విలువ = g->gr_gid;
        }

      ఒకవేళ (g == ఖాళీ)
        {
          ret = -1;

          ఒకవేళ (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              కొనసాగు;
            }

          ఆపు;
        }

      ఒకవేళ (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, పరిమాణం *gids);
      gids[n_gids++] = విలువ;
    }

  ఒకవేళ (ret == 0 && n_gids == 0)
    {
      ఒకవేళ (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  ఒకవేళ (ret == 0)
    *pn_gids = n_gids;

  విడుదల (బఫరు);
  తిరిగి ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

స్థిర bool
is_మూలం (అక్షరం స్థిరరాశి *dir)
{
  అక్షరం *resolved = canonicalize_file_name (dir);
  bool is_res_మూలం = resolved && streq ("/", resolved);
  విడుదల (resolved);
  తిరిగి is_res_మూలం;
}

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), కార్యక్రమం_name);

      fputs (_("\
నిరంతరం COMMAND with మూలం directory సమితి_2 గమ్యం NEWROOT.\n\
\n\
"), stdout);

      oputs (_("\
      --groups=G_LIST\n\
         specify supplementary groups as g1,g2,..,gN\n\
"));
      oputs (_("\
      --userspec=USER:సమూహం_2\n\
         specify user and సమూహం (ID or name) గమ్యం use\n\
"));
      oprintf (_("\
      --skip-chdir\n\
         చేయి not change working directory గమ్యం %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, నిరంతరం_2 '\"$SHELL\" -i' (అప్రమేయ: '/bin/sh -i').\n\
"), stdout);
      emit_exec_స్థితి (కార్యక్రమం_NAME);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం c;

  /* Input user and groups spec.  */
  అక్షరం *userspec = ఖాళీ;
  అక్షరం స్థిరరాశి *username = ఖాళీ;
  అక్షరం స్థిరరాశి *groups = ఖాళీ;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *బహిర్గతం_gids = ఖాళీ;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  అయితే ((c = getopt_long (argc, argv, "+", long_opts, ఖాళీ)) != -1)
    {
      ఎంపిక (c)
        {
        స్థితి USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = అక్షరశ్రేణి_పొడవు (userspec);
            ఒకవేళ (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            ఆపు;
          }

        స్థితి GROUPS:
          groups = optarg;
          ఆపు;

        స్థితి SKIP_CHDIR:
          skip_chdir = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_CANCELED);
        }
    }

  ఒకవేళ (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  అక్షరం స్థిరరాశి *newroot = argv[optind];
  bool is_oldroot = is_మూలం (newroot);

  ఒకవేళ (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  ఒకవేళ (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      ఒకవేళ (userspec)
        ignore_విలువ (parse_user_spec (userspec, &uid, &gid, ఖాళీ, ఖాళీ));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      ఒకవేళ (uid_సమితి (uid) && (! groups || gid_unset (gid)))
        {
          స్థిరరాశి నిర్మాణం passwd *pwd;
          ఒకవేళ ((pwd = getpwuid (uid)))
            {
              ఒకవేళ (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      ఒకవేళ (groups && *groups)
        ignore_విలువ (parse_additional_groups (groups, &బహిర్గతం_gids, &n_gids,
                                               false));
#ఒకవేళ HAVE_SETGROUPS
      లేకపోతే ఒకవేళ (! groups && gid_సమితి (gid) && username)
        {
          పూర్ణాంకం ngroups = xgetgroups (username, gid, &బహిర్గతం_gids);
          ఒకవేళ (0 < ngroups)
            n_gids = ngroups;
        }
#అయితే_ముగింపు
    }

  ఒకవేళ (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  ఒకవేళ (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  ఒకవేళ (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      అక్షరం *shell = getenv ("SHELL");
      ఒకవేళ (shell == ఖాళీ)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = ఖాళీ;
    }
  లేకపోతే
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  ఒకవేళ (userspec)
    {
      bool warn;
      అక్షరం స్థిరరాశి *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              ఖాళీ, ఖాళీ, &warn);
      ఒకవేళ (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  ఒకవేళ (uid_సమితి (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      స్థిరరాశి నిర్మాణం passwd *pwd;
      ఒకవేళ ((pwd = getpwuid (uid)))
        {
          ఒకవేళ (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      లేకపోతే ఒకవేళ (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = బహిర్గతం_gids;
  GETGROUPS_T *లోపల_gids = ఖాళీ;
  ఒకవేళ (groups && *groups)
    {
      ఒకవేళ (parse_additional_groups (groups, &లోపల_gids, &n_gids, !n_gids) != 0)
        {
          ఒకవేళ (! n_gids)
            తిరిగి EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      లేకపోతే
        gids = లోపల_gids;
    }
#ఒకవేళ HAVE_SETGROUPS
  లేకపోతే ఒకవేళ (! groups && gid_సమితి (gid) && username)
    {
      పూర్ణాంకం ngroups = xgetgroups (username, gid, &లోపల_gids);
      ఒకవేళ (ngroups <= 0)
        {
          ఒకవేళ (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      లేకపోతే
        {
          n_gids = ngroups;
          gids = లోపల_gids;
        }
    }
#అయితే_ముగింపు

  ఒకవేళ ((uid_సమితి (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  విడుదల (లోపల_gids);
  విడుదల (బహిర్గతం_gids);

  ఒకవేళ (gid_సమితి (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  ఒకవేళ (uid_సమితి (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  పూర్ణాంకం exit_స్థితి = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  తిరిగి exit_స్థితి;
}
