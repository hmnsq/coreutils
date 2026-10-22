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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pwd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <grp.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ignore-value.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mgetgroups.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "userspec.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "chroot"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Roland McGrath")

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MAXGID
# define MAXGID GID_T_MAX
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਸਤਰਅੰਦਰ bool uid_unset (uid_t uid) { ਵਾਪਸ uid == (uid_t) -1; }
ਸਥਿਰ ਸਤਰਅੰਦਰ bool gid_unset (gid_t gid) { ਵਾਪਸ gid == (gid_t) -1; }
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ uid_ਸੈੱਟ(x) (!uid_unset (x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ gid_ਸੈੱਟ(x) (!gid_unset (x))

ਗਿਣਤੀ
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_opts[] =
{
  {"groups", required_ਦਲੀਲ, ਨਿਰਾ, GROUPS},
  {"userspec", required_ਦਲੀਲ, ਨਿਰਾ, USERSPEC},
  {"skip-chdir", no_ਦਲੀਲ, ਨਿਰਾ, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

#ਜੇ ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
ਸਥਿਰ ਪੂਰਨ
setgroups (size_t ਮਾਪ, MAYBE_ਅਣਵਰਤਿਆ gid_t ਅਚਲ *list)
{
  ਜੇ (ਮਾਪ == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      ਵਾਪਸ 0;
    }
  ਨਹੀਂਤਾਂ
    {
      errno = ENOTSUP;
      ਵਾਪਸ -1;
    }
}
#ਜੇ_ਅੰਤ

/* Determine the group IDs for the specified supplementary GROUPS,
   which is a comma separated list of supplementary groups (names or numbers).
   Allocate an array for the parsed IDs and store it in PGIDS,
   which may be allocated even on parse failure.
   Update the number of parsed groups in PN_GIDS on success.
   Upon any failure return nonzero, and issue diagnostic if SHOW_ERRORS is true.
   Otherwise return zero.  */

ਸਥਿਰ ਪੂਰਨ
parse_additional_groups (ਅੱਖਰ ਅਚਲ *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = ਨਿਰਾ;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  ਅੱਖਰ *ਬਫਰ = xstrdup (groups);
  ਪੂਰਨ ret = 0;

  ਲਈ (ਅੱਖਰ ਅਚਲ *tmp = strtok (ਬਫਰ, ","); tmp;
       tmp = strtok (ਨਿਰਾ, ","))
    {
      ਬਣਤਰ ਸਮੂਹ *g;
      uintmax_t ਮੁੱਲ;

      ਜੇ (xstrtoumax (tmp, ਨਿਰਾ, 10, &ਮੁੱਲ, "") == LONGINT_OK
          && ਮੁੱਲ <= MAXGID)
        {
          ਜਦੋਂ (ਖਾਲੀ_ਥਾਂ_ਹੈ (ਮੰਜ਼ਿਲ_uchar (*tmp)))
            tmp++;
          ਜੇ (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              ਜੇ (g != ਨਿਰਾ)
                ਮੁੱਲ = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (ਬਣਤਰ ਸਮੂਹ *) (intptr_t) ! ਨਿਰਾ;
        }
      ਨਹੀਂਤਾਂ
        {
          g = getgrnam (tmp);
          ਜੇ (g != ਨਿਰਾ)
            ਮੁੱਲ = g->gr_gid;
        }

      ਜੇ (g == ਨਿਰਾ)
        {
          ret = -1;

          ਜੇ (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              ਜਾਰੀ;
            }

          ਰੋਕੋ;
        }

      ਜੇ (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, ਆਕਾਰ *gids);
      gids[n_gids++] = ਮੁੱਲ;
    }

  ਜੇ (ret == 0 && n_gids == 0)
    {
      ਜੇ (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  ਜੇ (ret == 0)
    *pn_gids = n_gids;

  ਮੁਕਤ (ਬਫਰ);
  ਵਾਪਸ ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

ਸਥਿਰ bool
is_ਜੜ੍ਹ (ਅੱਖਰ ਅਚਲ *dir)
{
  ਅੱਖਰ *resolved = canonicalize_file_name (dir);
  bool is_res_ਜੜ੍ਹ = resolved && streq ("/", resolved);
  ਮੁਕਤ (resolved);
  ਵਾਪਸ is_res_ਜੜ੍ਹ;
}

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), ਪ੍ਰੋਗਰਾਮ_name);

      fputs (_("\
ਲੜੀਵਾਰ COMMAND with ਜੜ੍ਹ directory ਸੈੱਟ_2 ਮੰਜ਼ਿਲ NEWROOT.\n\
\n\
"), stdout);

      oputs (_("\
      --groups=G_LIST\n\
         specify supplementary groups as g1,g2,..,gN\n\
"));
      oputs (_("\
      --userspec=USER:ਸਮੂਹ_2\n\
         specify user and ਸਮੂਹ (ID or name) ਮੰਜ਼ਿਲ use\n\
"));
      oprintf (_("\
      --skip-chdir\n\
         ਕਰੋ not change working directory ਮੰਜ਼ਿਲ %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, ਲੜੀਵਾਰ_2 '\"$SHELL\" -i' (ਮੂਲ: '/bin/sh -i').\n\
"), stdout);
      emit_exec_ਸਥਿਤੀ (ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ c;

  /* Input user and groups spec.  */
  ਅੱਖਰ *userspec = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *username = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *groups = ਨਿਰਾ;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *ਬਾਹਰਲਾ_gids = ਨਿਰਾ;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ਜਦੋਂ ((c = getopt_long (argc, argv, "+", long_opts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = ਲੜੀ_ਲੰਬਾਈ (userspec);
            ਜੇ (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            ਰੋਕੋ;
          }

        ਹਾਲਤ GROUPS:
          groups = optarg;
          ਰੋਕੋ;

        ਹਾਲਤ SKIP_CHDIR:
          skip_chdir = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_CANCELED);
        }
    }

  ਜੇ (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  ਅੱਖਰ ਅਚਲ *newroot = argv[optind];
  bool is_oldroot = is_ਜੜ੍ਹ (newroot);

  ਜੇ (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  ਜੇ (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      ਜੇ (userspec)
        ignore_ਮੁੱਲ (parse_user_spec (userspec, &uid, &gid, ਨਿਰਾ, ਨਿਰਾ));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      ਜੇ (uid_ਸੈੱਟ (uid) && (! groups || gid_unset (gid)))
        {
          ਅਚਲ ਬਣਤਰ passwd *pwd;
          ਜੇ ((pwd = getpwuid (uid)))
            {
              ਜੇ (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      ਜੇ (groups && *groups)
        ignore_ਮੁੱਲ (parse_additional_groups (groups, &ਬਾਹਰਲਾ_gids, &n_gids,
                                               false));
#ਜੇ HAVE_SETGROUPS
      ਨਹੀਂਤਾਂ ਜੇ (! groups && gid_ਸੈੱਟ (gid) && username)
        {
          ਪੂਰਨ ngroups = xgetgroups (username, gid, &ਬਾਹਰਲਾ_gids);
          ਜੇ (0 < ngroups)
            n_gids = ngroups;
        }
#ਜੇ_ਅੰਤ
    }

  ਜੇ (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  ਜੇ (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  ਜੇ (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      ਅੱਖਰ *shell = getenv ("SHELL");
      ਜੇ (shell == ਨਿਰਾ)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = ਨਿਰਾ;
    }
  ਨਹੀਂਤਾਂ
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  ਜੇ (userspec)
    {
      bool warn;
      ਅੱਖਰ ਅਚਲ *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              ਨਿਰਾ, ਨਿਰਾ, &warn);
      ਜੇ (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  ਜੇ (uid_ਸੈੱਟ (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      ਅਚਲ ਬਣਤਰ passwd *pwd;
      ਜੇ ((pwd = getpwuid (uid)))
        {
          ਜੇ (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      ਨਹੀਂਤਾਂ ਜੇ (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = ਬਾਹਰਲਾ_gids;
  GETGROUPS_T *ਵਿੱਚ_gids = ਨਿਰਾ;
  ਜੇ (groups && *groups)
    {
      ਜੇ (parse_additional_groups (groups, &ਵਿੱਚ_gids, &n_gids, !n_gids) != 0)
        {
          ਜੇ (! n_gids)
            ਵਾਪਸ EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      ਨਹੀਂਤਾਂ
        gids = ਵਿੱਚ_gids;
    }
#ਜੇ HAVE_SETGROUPS
  ਨਹੀਂਤਾਂ ਜੇ (! groups && gid_ਸੈੱਟ (gid) && username)
    {
      ਪੂਰਨ ngroups = xgetgroups (username, gid, &ਵਿੱਚ_gids);
      ਜੇ (ngroups <= 0)
        {
          ਜੇ (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      ਨਹੀਂਤਾਂ
        {
          n_gids = ngroups;
          gids = ਵਿੱਚ_gids;
        }
    }
#ਜੇ_ਅੰਤ

  ਜੇ ((uid_ਸੈੱਟ (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  ਮੁਕਤ (ਵਿੱਚ_gids);
  ਮੁਕਤ (ਬਾਹਰਲਾ_gids);

  ਜੇ (gid_ਸੈੱਟ (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  ਜੇ (uid_ਸੈੱਟ (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  ਪੂਰਨ exit_ਸਥਿਤੀ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ਵਾਪਸ exit_ਸਥਿਤੀ;
}
