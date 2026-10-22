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
#define PROGRAM_NAME "chroot"

#define AUTHORS proper_name ("Roland McGrath")

#ifndef MAXGID
# define MAXGID GID_T_MAX
#endif

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool uid_unset (uid_t uid) { ಪ್ರತ್ಯಾಗಮ uid == (uid_t) -1; }
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ bool gid_unset (gid_t gid) { ಪ್ರತ್ಯಾಗಮ gid == (gid_t) -1; }
#define uid_set(x) (!uid_unset (x))
#define gid_set(x) (!gid_unset (x))

ಗಣನಾ
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_opts[] =
{
  {"groups", required_argument, ರಿಕ್ತ, GROUPS},
  {"userspec", required_argument, ರಿಕ್ತ, USERSPEC},
  {"skip-chdir", no_argument, ರಿಕ್ತ, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

#ಯದಿ ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
setgroups (size_t size, MAYBE_UNUSED gid_t ಧ್ರುವ *list)
{
  ಯದಿ (size == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      ಪ್ರತ್ಯಾಗಮ 0;
    }
  ಅನ್ಯಥಾ
    {
      errno = ENOTSUP;
      ಪ್ರತ್ಯಾಗಮ -1;
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

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
parse_additional_groups (ವರ್ಣ ಧ್ರುವ *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = ರಿಕ್ತ;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  ವರ್ಣ *buffer = xstrdup (groups);
  ಪೂರ್ಣಾಙ್ಕ ret = 0;

  ಪ್ರತಿ (ವರ್ಣ ಧ್ರುವ *tmp = strtok (buffer, ","); tmp;
       tmp = strtok (ರಿಕ್ತ, ","))
    {
      ಸಂರಚನಾ group *g;
      uintmax_t ಮೌಲ್ಯ;

      ಯದಿ (xstrtoumax (tmp, ರಿಕ್ತ, 10, &ಮೌಲ್ಯ, "") == LONGINT_OK
          && ಮೌಲ್ಯ <= MAXGID)
        {
          ಯಾವತ್ (isspace (to_uchar (*tmp)))
            tmp++;
          ಯದಿ (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              ಯದಿ (g != ರಿಕ್ತ)
                ಮೌಲ್ಯ = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (ಸಂರಚನಾ group *) (intptr_t) ! ರಿಕ್ತ;
        }
      ಅನ್ಯಥಾ
        {
          g = getgrnam (tmp);
          ಯದಿ (g != ರಿಕ್ತ)
            ಮೌಲ್ಯ = g->gr_gid;
        }

      ಯದಿ (g == ರಿಕ್ತ)
        {
          ret = -1;

          ಯದಿ (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              ಅನುವರ್ತ;
            }

          ಭಙ್ಗ;
        }

      ಯದಿ (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, ಪರಿಮಾಣ *gids);
      gids[n_gids++] = ಮೌಲ್ಯ;
    }

  ಯದಿ (ret == 0 && n_gids == 0)
    {
      ಯದಿ (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  ಯದಿ (ret == 0)
    *pn_gids = n_gids;

  ಮುಕ್ತ (buffer);
  ಪ್ರತ್ಯಾಗಮ ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

ಸ್ಥಿರ bool
is_ಬೇರು (ವರ್ಣ ಧ್ರುವ *dir)
{
  ವರ್ಣ *resolved = canonicalize_file_name (dir);
  bool is_res_ಬೇರು = resolved && streq ("/", resolved);
  ಮುಕ್ತ (resolved);
  ಪ್ರತ್ಯಾಗಮ is_res_ಬೇರು;
}

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), program_name);

      fputs (_("\
Run COMMAND with ಬೇರು directory set to NEWROOT.\n\
\n\
"), stdout);

      oputs (_("\
      --groups=G_LIST\n\
         specify supplementary groups as g1,g2,..,gN\n\
"));
      oputs (_("\
      --userspec=USER:GROUP\n\
         specify user and group (ID or name) to use\n\
"));
      oprintf (_("\
      --skip-chdir\n\
         ಕುರು not change working directory to %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, run '\"$SHELL\" -i' (ಸ್ವಭಾವ: '/bin/sh -i').\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ c;

  /* Input user and groups spec.  */
  ವರ್ಣ *userspec = ರಿಕ್ತ;
  ವರ್ಣ ಧ್ರುವ *username = ರಿಕ್ತ;
  ವರ್ಣ ಧ್ರುವ *groups = ರಿಕ್ತ;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *out_gids = ರಿಕ್ತ;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ಯಾವತ್ ((c = getopt_long (argc, argv, "+", long_opts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = strlen (userspec);
            ಯದಿ (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            ಭಙ್ಗ;
          }

        ಪಕ್ಷ GROUPS:
          groups = optarg;
          ಭಙ್ಗ;

        ಪಕ್ಷ SKIP_CHDIR:
          skip_chdir = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_CANCELED);
        }
    }

  ಯದಿ (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  ವರ್ಣ ಧ್ರುವ *newroot = argv[optind];
  bool is_oldroot = is_ಬೇರು (newroot);

  ಯದಿ (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  ಯದಿ (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      ಯದಿ (userspec)
        ignore_ಮೌಲ್ಯ (parse_user_spec (userspec, &uid, &gid, ರಿಕ್ತ, ರಿಕ್ತ));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      ಯದಿ (uid_set (uid) && (! groups || gid_unset (gid)))
        {
          ಧ್ರುವ ಸಂರಚನಾ passwd *pwd;
          ಯದಿ ((pwd = getpwuid (uid)))
            {
              ಯದಿ (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      ಯದಿ (groups && *groups)
        ignore_ಮೌಲ್ಯ (parse_additional_groups (groups, &out_gids, &n_gids,
                                               false));
#ಯದಿ HAVE_SETGROUPS
      ಅನ್ಯಥಾ ಯದಿ (! groups && gid_set (gid) && username)
        {
          ಪೂರ್ಣಾಙ್ಕ ngroups = xgetgroups (username, gid, &out_gids);
          ಯದಿ (0 < ngroups)
            n_gids = ngroups;
        }
#endif
    }

  ಯದಿ (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  ಯದಿ (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  ಯದಿ (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      ವರ್ಣ *shell = getenv ("SHELL");
      ಯದಿ (shell == ರಿಕ್ತ)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = ರಿಕ್ತ;
    }
  ಅನ್ಯಥಾ
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  ಯದಿ (userspec)
    {
      bool warn;
      ವರ್ಣ ಧ್ರುವ *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              ರಿಕ್ತ, ರಿಕ್ತ, &warn);
      ಯದಿ (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  ಯದಿ (uid_set (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      ಧ್ರುವ ಸಂರಚನಾ passwd *pwd;
      ಯದಿ ((pwd = getpwuid (uid)))
        {
          ಯದಿ (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      ಅನ್ಯಥಾ ಯದಿ (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = out_gids;
  GETGROUPS_T *in_gids = ರಿಕ್ತ;
  ಯದಿ (groups && *groups)
    {
      ಯದಿ (parse_additional_groups (groups, &in_gids, &n_gids, !n_gids) != 0)
        {
          ಯದಿ (! n_gids)
            ಪ್ರತ್ಯಾಗಮ EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      ಅನ್ಯಥಾ
        gids = in_gids;
    }
#ಯದಿ HAVE_SETGROUPS
  ಅನ್ಯಥಾ ಯದಿ (! groups && gid_set (gid) && username)
    {
      ಪೂರ್ಣಾಙ್ಕ ngroups = xgetgroups (username, gid, &in_gids);
      ಯದಿ (ngroups <= 0)
        {
          ಯದಿ (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      ಅನ್ಯಥಾ
        {
          n_gids = ngroups;
          gids = in_gids;
        }
    }
#endif

  ಯದಿ ((uid_set (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  ಮುಕ್ತ (in_gids);
  ಮುಕ್ತ (out_gids);

  ಯದಿ (gid_set (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  ಯದಿ (uid_set (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  ಪೂರ್ಣಾಙ್ಕ exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ಪ್ರತ್ಯಾಗಮ exit_status;
}
