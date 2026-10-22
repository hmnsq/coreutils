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

#include <config.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

#include "system.h"
#include "ignore-value.h"
#include "mgetgroups.h"
#include "quote.h"
#include "userspec.h"
#include "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chroot"

#define AUTHORS proper_name ("Roland McGrath")

#ifndef MAXGID
# define MAXGID GID_T_MAX
#endif

static anta_hstha bool uid_unset (uid_t uid) { ආපසු uid == (uid_t) -1; }
static anta_hstha bool gid_unset (gid_t gid) { ආපසු gid == (gid_t) -1; }
#define uid_set(x) (!uid_unset (x))
#define gid_set(x) (!gid_unset (x))

ga_nanaa
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

static sa_nracanaa option const long_opts[] =
{
  {"groups", required_argument, NULL, GROUPS},
  {"userspec", required_argument, NULL, USERSPEC},
  {"skip-chdir", no_argument, NULL, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

#නම් ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
static පූර්ණ_සංඛ්‍යා
setgroups (size_t size, MAYBE_UNUSED gid_t const *list)
{
  නම් (size == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      ආපසු 0;
    }
  නැතහොත්
    {
      errno = ENOTSUP;
      ආපසු -1;
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

static පූර්ණ_සංඛ්‍යා
parse_additional_groups (var_na const *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = NULL;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  var_na *buffer = xstrdup (groups);
  පූර්ණ_සංඛ්‍යා ret = 0;

  සඳහා (var_na const *tmp = strtok (buffer, ","); tmp;
       tmp = strtok (NULL, ","))
    {
      sa_nracanaa group *g;
      uintmax_t value;

      නම් (xstrtoumax (tmp, NULL, 10, &value, "") == LONGINT_OK
          && value <= MAXGID)
        {
          අතරතුර (isspace (to_uchar (*tmp)))
            tmp++;
          නම් (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              නම් (g != NULL)
                value = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (sa_nracanaa group *) (intptr_t) ! NULL;
        }
      නැතහොත්
        {
          g = getgrnam (tmp);
          නම් (g != NULL)
            value = g->gr_gid;
        }

      නම් (g == NULL)
        {
          ret = -1;

          නම් (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              continue;
            }

          bha_nga;
        }

      නම් (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, parimaa_na *gids);
      gids[n_gids++] = value;
    }

  නම් (ret == 0 && n_gids == 0)
    {
      නම් (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  නම් (ret == 0)
    *pn_gids = n_gids;

  free (buffer);
  ආපසු ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

static bool
is_root (var_na const *dir)
{
  var_na *resolved = canonicalize_file_name (dir);
  bool is_res_root = resolved && streq ("/", resolved);
  free (resolved);
  ආපසු is_res_root;
}

void
usage (පූර්ණ_සංඛ්‍යා status)
{
  නම් (status != EXIT_SUCCESS)
    emit_try_help ();
  නැතහොත්
    {
      printf (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), program_name);

      fputs (_("\
Run COMMAND with root directory set to NEWROOT.\n\
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
         do not change working directory to %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, run '\"$SHELL\" -i' (default: '/bin/sh -i').\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

පූර්ණ_සංඛ්‍යා
main (පූර්ණ_සංඛ්‍යා argc, var_na **argv)
{
  පූර්ණ_සංඛ්‍යා c;

  /* Input user and groups spec.  */
  var_na *userspec = NULL;
  var_na const *username = NULL;
  var_na const *groups = NULL;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *out_gids = NULL;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  අතරතුර ((c = getopt_long (argc, argv, "+", long_opts, NULL)) != -1)
    {
      switch (c)
        {
        pak_sa USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = strlen (userspec);
            නම් (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            bha_nga;
          }

        pak_sa GROUPS:
          groups = optarg;
          bha_nga;

        pak_sa SKIP_CHDIR:
          skip_chdir = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_CANCELED);
        }
    }

  නම් (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  var_na const *newroot = argv[optind];
  bool is_oldroot = is_root (newroot);

  නම් (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  නම් (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      නම් (userspec)
        ignore_value (parse_user_spec (userspec, &uid, &gid, NULL, NULL));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      නම් (uid_set (uid) && (! groups || gid_unset (gid)))
        {
          const sa_nracanaa passwd *pwd;
          නම් ((pwd = getpwuid (uid)))
            {
              නම් (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      නම් (groups && *groups)
        ignore_value (parse_additional_groups (groups, &out_gids, &n_gids,
                                               false));
#නම් HAVE_SETGROUPS
      නැතහොත් නම් (! groups && gid_set (gid) && username)
        {
          පූර්ණ_සංඛ්‍යා ngroups = xgetgroups (username, gid, &out_gids);
          නම් (0 < ngroups)
            n_gids = ngroups;
        }
#endif
    }

  නම් (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  නම් (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  නම් (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      var_na *shell = getenv ("SHELL");
      නම් (shell == NULL)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = NULL;
    }
  නැතහොත්
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  නම් (userspec)
    {
      bool warn;
      var_na const *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              NULL, NULL, &warn);
      නම් (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  නම් (uid_set (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      const sa_nracanaa passwd *pwd;
      නම් ((pwd = getpwuid (uid)))
        {
          නම් (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      නැතහොත් නම් (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = out_gids;
  GETGROUPS_T *in_gids = NULL;
  නම් (groups && *groups)
    {
      නම් (parse_additional_groups (groups, &in_gids, &n_gids, !n_gids) != 0)
        {
          නම් (! n_gids)
            ආපසු EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      නැතහොත්
        gids = in_gids;
    }
#නම් HAVE_SETGROUPS
  නැතහොත් නම් (! groups && gid_set (gid) && username)
    {
      පූර්ණ_සංඛ්‍යා ngroups = xgetgroups (username, gid, &in_gids);
      නම් (ngroups <= 0)
        {
          නම් (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      නැතහොත්
        {
          n_gids = ngroups;
          gids = in_gids;
        }
    }
#endif

  නම් ((uid_set (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  free (in_gids);
  free (out_gids);

  නම් (gid_set (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  නම් (uid_set (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  පූර්ණ_සංඛ්‍යා exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ආපසු exit_status;
}
