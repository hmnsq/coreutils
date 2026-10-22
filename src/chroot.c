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

static anta_hstha bool uid_unset (uid_t uid) { ສົ່ງຄືນ uid == (uid_t) -1; }
static anta_hstha bool gid_unset (gid_t gid) { ສົ່ງຄືນ gid == (gid_t) -1; }
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

#ຖ້າ ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
static ຈຳນວນເຕັມ
setgroups (size_t size, MAYBE_UNUSED gid_t const *list)
{
  ຖ້າ (size == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      ສົ່ງຄືນ 0;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      errno = ENOTSUP;
      ສົ່ງຄືນ -1;
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

static ຈຳນວນເຕັມ
parse_additional_groups (var_na const *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = NULL;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  var_na *buffer = xstrdup (groups);
  ຈຳນວນເຕັມ ret = 0;

  ສຳລັບ (var_na const *tmp = strtok (buffer, ","); tmp;
       tmp = strtok (NULL, ","))
    {
      sa_nracanaa group *g;
      uintmax_t value;

      ຖ້າ (xstrtoumax (tmp, NULL, 10, &value, "") == LONGINT_OK
          && value <= MAXGID)
        {
          ໃນຂະນະທີ່ (isspace (to_uchar (*tmp)))
            tmp++;
          ຖ້າ (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              ຖ້າ (g != NULL)
                value = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (sa_nracanaa group *) (intptr_t) ! NULL;
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          g = getgrnam (tmp);
          ຖ້າ (g != NULL)
            value = g->gr_gid;
        }

      ຖ້າ (g == NULL)
        {
          ret = -1;

          ຖ້າ (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              continue;
            }

          bha_nga;
        }

      ຖ້າ (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, parimaa_na *gids);
      gids[n_gids++] = value;
    }

  ຖ້າ (ret == 0 && n_gids == 0)
    {
      ຖ້າ (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  ຖ້າ (ret == 0)
    *pn_gids = n_gids;

  free (buffer);
  ສົ່ງຄືນ ret;
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
  ສົ່ງຄືນ is_res_root;
}

void
usage (ຈຳນວນເຕັມ status)
{
  ຖ້າ (status != EXIT_SUCCESS)
    emit_try_help ();
  ບໍ່ດັ່ງນັ້ນ
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

ຈຳນວນເຕັມ
main (ຈຳນວນເຕັມ argc, var_na **argv)
{
  ຈຳນວນເຕັມ c;

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

  ໃນຂະນະທີ່ ((c = getopt_long (argc, argv, "+", long_opts, NULL)) != -1)
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
            ຖ້າ (userlen && userspec[userlen - 1] == ':')
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

  ຖ້າ (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  var_na const *newroot = argv[optind];
  bool is_oldroot = is_root (newroot);

  ຖ້າ (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  ຖ້າ (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      ຖ້າ (userspec)
        ignore_value (parse_user_spec (userspec, &uid, &gid, NULL, NULL));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      ຖ້າ (uid_set (uid) && (! groups || gid_unset (gid)))
        {
          const sa_nracanaa passwd *pwd;
          ຖ້າ ((pwd = getpwuid (uid)))
            {
              ຖ້າ (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      ຖ້າ (groups && *groups)
        ignore_value (parse_additional_groups (groups, &out_gids, &n_gids,
                                               false));
#ຖ້າ HAVE_SETGROUPS
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (! groups && gid_set (gid) && username)
        {
          ຈຳນວນເຕັມ ngroups = xgetgroups (username, gid, &out_gids);
          ຖ້າ (0 < ngroups)
            n_gids = ngroups;
        }
#endif
    }

  ຖ້າ (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  ຖ້າ (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  ຖ້າ (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      var_na *shell = getenv ("SHELL");
      ຖ້າ (shell == NULL)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = NULL;
    }
  ບໍ່ດັ່ງນັ້ນ
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  ຖ້າ (userspec)
    {
      bool warn;
      var_na const *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              NULL, NULL, &warn);
      ຖ້າ (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  ຖ້າ (uid_set (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      const sa_nracanaa passwd *pwd;
      ຖ້າ ((pwd = getpwuid (uid)))
        {
          ຖ້າ (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = out_gids;
  GETGROUPS_T *in_gids = NULL;
  ຖ້າ (groups && *groups)
    {
      ຖ້າ (parse_additional_groups (groups, &in_gids, &n_gids, !n_gids) != 0)
        {
          ຖ້າ (! n_gids)
            ສົ່ງຄືນ EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      ບໍ່ດັ່ງນັ້ນ
        gids = in_gids;
    }
#ຖ້າ HAVE_SETGROUPS
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (! groups && gid_set (gid) && username)
    {
      ຈຳນວນເຕັມ ngroups = xgetgroups (username, gid, &in_gids);
      ຖ້າ (ngroups <= 0)
        {
          ຖ້າ (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      ບໍ່ດັ່ງນັ້ນ
        {
          n_gids = ngroups;
          gids = in_gids;
        }
    }
#endif

  ຖ້າ ((uid_set (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  free (in_gids);
  free (out_gids);

  ຖ້າ (gid_set (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  ຖ້າ (uid_set (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  ຈຳນວນເຕັມ exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ສົ່ງຄືນ exit_status;
}
