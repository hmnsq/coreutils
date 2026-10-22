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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <pwd.h>
#അന്തര്ഭാവ <grp.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "ignore-value.h"
#അന്തര്ഭാവ "mgetgroups.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "userspec.h"
#അന്തര്ഭാവ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chroot"

#define AUTHORS proper_name ("Roland McGrath")

#ifndef MAXGID
# define MAXGID GID_T_MAX
#endif

സ്ഥിതം വരിയിൽ bool uid_unset (uid_t uid) { തിരികെ uid == (uid_t) -1; }
സ്ഥിതം വരിയിൽ bool gid_unset (gid_t gid) { തിരികെ gid == (gid_t) -1; }
#define uid_set(x) (!uid_unset (x))
#define gid_set(x) (!gid_unset (x))

എണ്ണൽ
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

സ്ഥിതം ഘടന option സ്ഥിരം long_opts[] =
{
  {"groups", required_argument, ശൂന്യ, GROUPS},
  {"userspec", required_argument, ശൂന്യ, USERSPEC},
  {"skip-chdir", no_argument, ശൂന്യ, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

#എങ്കിൽ ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
setgroups (size_t size, MAYBE_UNUSED gid_t സ്ഥിരം *list)
{
  എങ്കിൽ (size == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      തിരികെ 0;
    }
  അല്ലെങ്കിൽ
    {
      errno = ENOTSUP;
      തിരികെ -1;
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

സ്ഥിതം പൂർണ്ണസംഖ്യ
parse_additional_groups (അക്ഷരം സ്ഥിരം *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = ശൂന്യ;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  അക്ഷരം *buffer = xstrdup (groups);
  പൂർണ്ണസംഖ്യ ret = 0;

  വേണ്ടി (അക്ഷരം സ്ഥിരം *tmp = strtok (buffer, ","); tmp;
       tmp = strtok (ശൂന്യ, ","))
    {
      ഘടന group *g;
      uintmax_t മൂല്യം;

      എങ്കിൽ (xstrtoumax (tmp, ശൂന്യ, 10, &മൂല്യം, "") == LONGINT_OK
          && മൂല്യം <= MAXGID)
        {
          ആകുമ്പോൾ (isspace (to_uchar (*tmp)))
            tmp++;
          എങ്കിൽ (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              എങ്കിൽ (g != ശൂന്യ)
                മൂല്യം = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (ഘടന group *) (intptr_t) ! ശൂന്യ;
        }
      അല്ലെങ്കിൽ
        {
          g = getgrnam (tmp);
          എങ്കിൽ (g != ശൂന്യ)
            മൂല്യം = g->gr_gid;
        }

      എങ്കിൽ (g == ശൂന്യ)
        {
          ret = -1;

          എങ്കിൽ (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              തുടരുക;
            }

          നിർത്തുക;
        }

      എങ്കിൽ (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, വലുപ്പം *gids);
      gids[n_gids++] = മൂല്യം;
    }

  എങ്കിൽ (ret == 0 && n_gids == 0)
    {
      എങ്കിൽ (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  എങ്കിൽ (ret == 0)
    *pn_gids = n_gids;

  സ്വതന്ത്രം (buffer);
  തിരികെ ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

സ്ഥിതം bool
is_വേര് (അക്ഷരം സ്ഥിരം *dir)
{
  അക്ഷരം *resolved = canonicalize_file_name (dir);
  bool is_res_വേര് = resolved && streq ("/", resolved);
  സ്വതന്ത്രം (resolved);
  തിരികെ is_res_വേര്;
}

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), program_name);

      fputs (_("\
Run COMMAND with വേര് directory set to NEWROOT.\n\
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
         ചെയ്യുക not change working directory to %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, run '\"$SHELL\" -i' (സ്ഥിരസ്ഥിതി: '/bin/sh -i').\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ c;

  /* Input user and groups spec.  */
  അക്ഷരം *userspec = ശൂന്യ;
  അക്ഷരം സ്ഥിരം *username = ശൂന്യ;
  അക്ഷരം സ്ഥിരം *groups = ശൂന്യ;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *out_gids = ശൂന്യ;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "+", long_opts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = strlen (userspec);
            എങ്കിൽ (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            നിർത്തുക;
          }

        അവസ്ഥ GROUPS:
          groups = optarg;
          നിർത്തുക;

        അവസ്ഥ SKIP_CHDIR:
          skip_chdir = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_CANCELED);
        }
    }

  എങ്കിൽ (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  അക്ഷരം സ്ഥിരം *newroot = argv[optind];
  bool is_oldroot = is_വേര് (newroot);

  എങ്കിൽ (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  എങ്കിൽ (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      എങ്കിൽ (userspec)
        ignore_മൂല്യം (parse_user_spec (userspec, &uid, &gid, ശൂന്യ, ശൂന്യ));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      എങ്കിൽ (uid_set (uid) && (! groups || gid_unset (gid)))
        {
          സ്ഥിരം ഘടന passwd *pwd;
          എങ്കിൽ ((pwd = getpwuid (uid)))
            {
              എങ്കിൽ (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      എങ്കിൽ (groups && *groups)
        ignore_മൂല്യം (parse_additional_groups (groups, &out_gids, &n_gids,
                                               false));
#എങ്കിൽ HAVE_SETGROUPS
      അല്ലെങ്കിൽ എങ്കിൽ (! groups && gid_set (gid) && username)
        {
          പൂർണ്ണസംഖ്യ ngroups = xgetgroups (username, gid, &out_gids);
          എങ്കിൽ (0 < ngroups)
            n_gids = ngroups;
        }
#endif
    }

  എങ്കിൽ (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  എങ്കിൽ (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  എങ്കിൽ (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      അക്ഷരം *shell = getenv ("SHELL");
      എങ്കിൽ (shell == ശൂന്യ)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = ശൂന്യ;
    }
  അല്ലെങ്കിൽ
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  എങ്കിൽ (userspec)
    {
      bool warn;
      അക്ഷരം സ്ഥിരം *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              ശൂന്യ, ശൂന്യ, &warn);
      എങ്കിൽ (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  എങ്കിൽ (uid_set (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      സ്ഥിരം ഘടന passwd *pwd;
      എങ്കിൽ ((pwd = getpwuid (uid)))
        {
          എങ്കിൽ (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = out_gids;
  GETGROUPS_T *in_gids = ശൂന്യ;
  എങ്കിൽ (groups && *groups)
    {
      എങ്കിൽ (parse_additional_groups (groups, &in_gids, &n_gids, !n_gids) != 0)
        {
          എങ്കിൽ (! n_gids)
            തിരികെ EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      അല്ലെങ്കിൽ
        gids = in_gids;
    }
#എങ്കിൽ HAVE_SETGROUPS
  അല്ലെങ്കിൽ എങ്കിൽ (! groups && gid_set (gid) && username)
    {
      പൂർണ്ണസംഖ്യ ngroups = xgetgroups (username, gid, &in_gids);
      എങ്കിൽ (ngroups <= 0)
        {
          എങ്കിൽ (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      അല്ലെങ്കിൽ
        {
          n_gids = ngroups;
          gids = in_gids;
        }
    }
#endif

  എങ്കിൽ ((uid_set (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  സ്വതന്ത്രം (in_gids);
  സ്വതന്ത്രം (out_gids);

  എങ്കിൽ (gid_set (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  എങ്കിൽ (uid_set (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  പൂർണ്ണസംഖ്യ exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  തിരികെ exit_status;
}
