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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <pwd.h>
#அந்தர்பாவ <grp.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "ignore-value.h"
#அந்தர்பாவ "mgetgroups.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "userspec.h"
#அந்தர்பாவ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chroot"

#define AUTHORS proper_name ("Roland McGrath")

#ifndef MAXGID
# define MAXGID GID_T_MAX
#endif

நிலையான உள்வரி bool uid_unset (uid_t uid) { திரும்பு uid == (uid_t) -1; }
நிலையான உள்வரி bool gid_unset (gid_t gid) { திரும்பு gid == (gid_t) -1; }
#define uid_set(x) (!uid_unset (x))
#define gid_set(x) (!gid_unset (x))

எண்ணிக்கை
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

நிலையான கட்டமைப்பு option மாறிலி long_opts[] =
{
  {"groups", required_argument, வெற்று, GROUPS},
  {"userspec", required_argument, வெற்று, USERSPEC},
  {"skip-chdir", no_argument, வெற்று, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

#எனில் ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
நிலையான முழு
setgroups (size_t size, MAYBE_UNUSED gid_t மாறிலி *list)
{
  எனில் (size == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      திரும்பு 0;
    }
  இல்லையெனில்
    {
      errno = ENOTSUP;
      திரும்பு -1;
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

நிலையான முழு
parse_additional_groups (எழுத்து மாறிலி *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = வெற்று;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  எழுத்து *buffer = xstrdup (groups);
  முழு ret = 0;

  க்கு (எழுத்து மாறிலி *tmp = strtok (buffer, ","); tmp;
       tmp = strtok (வெற்று, ","))
    {
      கட்டமைப்பு group *g;
      uintmax_t மதிப்பு;

      எனில் (xstrtoumax (tmp, வெற்று, 10, &மதிப்பு, "") == LONGINT_OK
          && மதிப்பு <= MAXGID)
        {
          போது (isspace (to_uchar (*tmp)))
            tmp++;
          எனில் (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              எனில் (g != வெற்று)
                மதிப்பு = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (கட்டமைப்பு group *) (intptr_t) ! வெற்று;
        }
      இல்லையெனில்
        {
          g = getgrnam (tmp);
          எனில் (g != வெற்று)
            மதிப்பு = g->gr_gid;
        }

      எனில் (g == வெற்று)
        {
          ret = -1;

          எனில் (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              தொடர்;
            }

          நிறுத்து;
        }

      எனில் (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, அளவு *gids);
      gids[n_gids++] = மதிப்பு;
    }

  எனில் (ret == 0 && n_gids == 0)
    {
      எனில் (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  எனில் (ret == 0)
    *pn_gids = n_gids;

  விடுவி (buffer);
  திரும்பு ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

நிலையான bool
is_வேர் (எழுத்து மாறிலி *dir)
{
  எழுத்து *resolved = canonicalize_file_name (dir);
  bool is_res_வேர் = resolved && streq ("/", resolved);
  விடுவி (resolved);
  திரும்பு is_res_வேர்;
}

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), program_name);

      fputs (_("\
Run COMMAND with வேர் directory set to NEWROOT.\n\
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
         செய் not change working directory to %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, run '\"$SHELL\" -i' (இயல்பு: '/bin/sh -i').\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு c;

  /* Input user and groups spec.  */
  எழுத்து *userspec = வெற்று;
  எழுத்து மாறிலி *username = வெற்று;
  எழுத்து மாறிலி *groups = வெற்று;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *out_gids = வெற்று;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  போது ((c = getopt_long (argc, argv, "+", long_opts, வெற்று)) != -1)
    {
      தேர்வு (c)
        {
        நிலை USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = strlen (userspec);
            எனில் (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            நிறுத்து;
          }

        நிலை GROUPS:
          groups = optarg;
          நிறுத்து;

        நிலை SKIP_CHDIR:
          skip_chdir = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_CANCELED);
        }
    }

  எனில் (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  எழுத்து மாறிலி *newroot = argv[optind];
  bool is_oldroot = is_வேர் (newroot);

  எனில் (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  எனில் (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      எனில் (userspec)
        ignore_மதிப்பு (parse_user_spec (userspec, &uid, &gid, வெற்று, வெற்று));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      எனில் (uid_set (uid) && (! groups || gid_unset (gid)))
        {
          மாறிலி கட்டமைப்பு passwd *pwd;
          எனில் ((pwd = getpwuid (uid)))
            {
              எனில் (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      எனில் (groups && *groups)
        ignore_மதிப்பு (parse_additional_groups (groups, &out_gids, &n_gids,
                                               false));
#எனில் HAVE_SETGROUPS
      இல்லையெனில் எனில் (! groups && gid_set (gid) && username)
        {
          முழு ngroups = xgetgroups (username, gid, &out_gids);
          எனில் (0 < ngroups)
            n_gids = ngroups;
        }
#endif
    }

  எனில் (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  எனில் (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  எனில் (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      எழுத்து *shell = getenv ("SHELL");
      எனில் (shell == வெற்று)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = வெற்று;
    }
  இல்லையெனில்
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  எனில் (userspec)
    {
      bool warn;
      எழுத்து மாறிலி *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              வெற்று, வெற்று, &warn);
      எனில் (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  எனில் (uid_set (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      மாறிலி கட்டமைப்பு passwd *pwd;
      எனில் ((pwd = getpwuid (uid)))
        {
          எனில் (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      இல்லையெனில் எனில் (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = out_gids;
  GETGROUPS_T *in_gids = வெற்று;
  எனில் (groups && *groups)
    {
      எனில் (parse_additional_groups (groups, &in_gids, &n_gids, !n_gids) != 0)
        {
          எனில் (! n_gids)
            திரும்பு EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      இல்லையெனில்
        gids = in_gids;
    }
#எனில் HAVE_SETGROUPS
  இல்லையெனில் எனில் (! groups && gid_set (gid) && username)
    {
      முழு ngroups = xgetgroups (username, gid, &in_gids);
      எனில் (ngroups <= 0)
        {
          எனில் (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      இல்லையெனில்
        {
          n_gids = ngroups;
          gids = in_gids;
        }
    }
#endif

  எனில் ((uid_set (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  விடுவி (in_gids);
  விடுவி (out_gids);

  எனில் (gid_set (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  எனில் (uid_set (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  முழு exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  திரும்பு exit_status;
}
