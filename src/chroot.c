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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <grp.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "ignore-value.h"
#ಅನ್ತರ್ಭಾವ "mgetgroups.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "userspec.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "chroot"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Roland McGrath")

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MAXGID
# define MAXGID GID_T_MAX
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool uid_unset (uid_t uid) { ಹಿಂತಿರುಗಿ uid == (uid_t) -1; }
ಸ್ಥಿರ ಸಾಲೊಳಗೆ bool gid_unset (gid_t gid) { ಹಿಂತಿರುಗಿ gid == (gid_t) -1; }
#ವ್ಯಾಖ್ಯಾನಿಸು uid_ಗಣ(x) (!uid_unset (x))
#ವ್ಯಾಖ್ಯಾನಿಸು gid_ಗಣ(x) (!gid_unset (x))

ಎಣಿಕೆ
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_opts[] =
{
  {"groups", required_ವಾದ, ಖಾಲಿ, GROUPS},
  {"userspec", required_ವಾದ, ಖಾಲಿ, USERSPEC},
  {"skip-chdir", no_ವಾದ, ಖಾಲಿ, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

#ಆದರೆ ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
setgroups (size_t ಗಾತ್ರ_2, MAYBE_ಬಳಸದ gid_t ಸ್ಥಿರಾಂಕ *list)
{
  ಆದರೆ (ಗಾತ್ರ_2 == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      ಹಿಂತಿರುಗಿ 0;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      errno = ENOTSUP;
      ಹಿಂತಿರುಗಿ -1;
    }
}
#ಆದರೆ_ಅಂತ್ಯ

/* Determine the group IDs for the specified supplementary GROUPS,
   which is a comma separated list of supplementary groups (names or numbers).
   Allocate an array for the parsed IDs and store it in PGIDS,
   which may be allocated even on parse failure.
   Update the number of parsed groups in PN_GIDS on success.
   Upon any failure return nonzero, and issue diagnostic if SHOW_ERRORS is true.
   Otherwise return zero.  */

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
parse_additional_groups (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = ಖಾಲಿ;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  ಅಕ್ಷರ *ಬಫರ್ = xstrdup (groups);
  ಪೂರ್ಣಾಂಕ ret = 0;

  ಗಾಗಿ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *tmp = strtok (ಬಫರ್, ","); tmp;
       tmp = strtok (ಖಾಲಿ, ","))
    {
      ರಚನೆ ಗುಂಪು *g;
      uintmax_t ಮೌಲ್ಯ;

      ಆದರೆ (xstrtoumax (tmp, ಖಾಲಿ, 10, &ಮೌಲ್ಯ, "") == LONGINT_OK
          && ಮೌಲ್ಯ <= MAXGID)
        {
          ಆಗ (ಖಾಲಿಯೇ (ಗಮ್ಯ_uchar (*tmp)))
            tmp++;
          ಆದರೆ (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              ಆದರೆ (g != ಖಾಲಿ)
                ಮೌಲ್ಯ = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (ರಚನೆ ಗುಂಪು *) (intptr_t) ! ಖಾಲಿ;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          g = getgrnam (tmp);
          ಆದರೆ (g != ಖಾಲಿ)
            ಮೌಲ್ಯ = g->gr_gid;
        }

      ಆದರೆ (g == ಖಾಲಿ)
        {
          ret = -1;

          ಆದರೆ (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              ಮುಂದುವರಿ;
            }

          ನಿಲ್ಲಿಸು;
        }

      ಆದರೆ (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, ಗಾತ್ರ *gids);
      gids[n_gids++] = ಮೌಲ್ಯ;
    }

  ಆದರೆ (ret == 0 && n_gids == 0)
    {
      ಆದರೆ (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  ಆದರೆ (ret == 0)
    *pn_gids = n_gids;

  ಬಿಡುಗಡೆ (ಬಫರ್);
  ಹಿಂತಿರುಗಿ ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

ಸ್ಥಿರ bool
is_ಬೇರು (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *dir)
{
  ಅಕ್ಷರ *resolved = canonicalize_file_name (dir);
  bool is_res_ಬೇರು = resolved && streq ("/", resolved);
  ಬಿಡುಗಡೆ (resolved);
  ಹಿಂತಿರುಗಿ is_res_ಬೇರು;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), ಕಾರ್ಯಕ್ರಮ_name);

      fputs (_("\
ಸತತ COMMAND with ಬೇರು directory ಗಣ_2 ಗಮ್ಯ NEWROOT.\n\
\n\
"), stdout);

      oputs (_("\
      --groups=G_LIST\n\
         specify supplementary groups as g1,g2,..,gN\n\
"));
      oputs (_("\
      --userspec=USER:ಗುಂಪು_2\n\
         specify user and ಗುಂಪು (ID or name) ಗಮ್ಯ use\n\
"));
      oprintf (_("\
      --skip-chdir\n\
         ಮಾಡು not change working directory ಗಮ್ಯ %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, ಸತತ_2 '\"$SHELL\" -i' (ಪೂರ್ವನಿಯೋಜಿತ: '/bin/sh -i').\n\
"), stdout);
      emit_exec_ಸ್ಥಿತಿ (ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ c;

  /* Input user and groups spec.  */
  ಅಕ್ಷರ *userspec = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *username = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *groups = ಖಾಲಿ;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *ಹೊರಹೋಗು_gids = ಖಾಲಿ;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ಆಗ ((c = getopt_long (argc, argv, "+", long_opts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (userspec);
            ಆದರೆ (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            ನಿಲ್ಲಿಸು;
          }

        ಸ್ಥಿತಿ GROUPS:
          groups = optarg;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ SKIP_CHDIR:
          skip_chdir = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_CANCELED);
        }
    }

  ಆದರೆ (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *newroot = argv[optind];
  bool is_oldroot = is_ಬೇರು (newroot);

  ಆದರೆ (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  ಆದರೆ (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      ಆದರೆ (userspec)
        ignore_ಮೌಲ್ಯ (parse_user_spec (userspec, &uid, &gid, ಖಾಲಿ, ಖಾಲಿ));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      ಆದರೆ (uid_ಗಣ (uid) && (! groups || gid_unset (gid)))
        {
          ಸ್ಥಿರಾಂಕ ರಚನೆ passwd *pwd;
          ಆದರೆ ((pwd = getpwuid (uid)))
            {
              ಆದರೆ (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      ಆದರೆ (groups && *groups)
        ignore_ಮೌಲ್ಯ (parse_additional_groups (groups, &ಹೊರಹೋಗು_gids, &n_gids,
                                               false));
#ಆದರೆ HAVE_SETGROUPS
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! groups && gid_ಗಣ (gid) && username)
        {
          ಪೂರ್ಣಾಂಕ ngroups = xgetgroups (username, gid, &ಹೊರಹೋಗು_gids);
          ಆದರೆ (0 < ngroups)
            n_gids = ngroups;
        }
#ಆದರೆ_ಅಂತ್ಯ
    }

  ಆದರೆ (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  ಆದರೆ (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  ಆದರೆ (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      ಅಕ್ಷರ *shell = getenv ("SHELL");
      ಆದರೆ (shell == ಖಾಲಿ)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = ಖಾಲಿ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  ಆದರೆ (userspec)
    {
      bool warn;
      ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              ಖಾಲಿ, ಖಾಲಿ, &warn);
      ಆದರೆ (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  ಆದರೆ (uid_ಗಣ (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      ಸ್ಥಿರಾಂಕ ರಚನೆ passwd *pwd;
      ಆದರೆ ((pwd = getpwuid (uid)))
        {
          ಆದರೆ (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = ಹೊರಹೋಗು_gids;
  GETGROUPS_T *ಒಳ_gids = ಖಾಲಿ;
  ಆದರೆ (groups && *groups)
    {
      ಆದರೆ (parse_additional_groups (groups, &ಒಳ_gids, &n_gids, !n_gids) != 0)
        {
          ಆದರೆ (! n_gids)
            ಹಿಂತಿರುಗಿ EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      ಇಲ್ಲದಿದ್ದರೆ
        gids = ಒಳ_gids;
    }
#ಆದರೆ HAVE_SETGROUPS
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! groups && gid_ಗಣ (gid) && username)
    {
      ಪೂರ್ಣಾಂಕ ngroups = xgetgroups (username, gid, &ಒಳ_gids);
      ಆದರೆ (ngroups <= 0)
        {
          ಆದರೆ (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          n_gids = ngroups;
          gids = ಒಳ_gids;
        }
    }
#ಆದರೆ_ಅಂತ್ಯ

  ಆದರೆ ((uid_ಗಣ (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  ಬಿಡುಗಡೆ (ಒಳ_gids);
  ಬಿಡುಗಡೆ (ಹೊರಹೋಗು_gids);

  ಆದರೆ (gid_ಗಣ (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  ಆದರೆ (uid_ಗಣ (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}
