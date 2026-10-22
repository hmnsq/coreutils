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

#شامل <config.h>
#شامل <getopt.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <pwd.h>
#شامل <grp.h>

#شامل "system.h"
#شامل "ignore-value.h"
#شامل "mgetgroups.h"
#شامل "quote.h"
#شامل "userspec.h"
#شامل "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chroot"

#define AUTHORS proper_name ("Roland McGrath")

#ifndef MAXGID
# define MAXGID GID_T_MAX
#endif

ایستا درونخطی bool uid_unset (uid_t uid) { بازگشت uid == (uid_t) -1; }
ایستا درونخطی bool gid_unset (gid_t gid) { بازگشت gid == (gid_t) -1; }
#define uid_set(x) (!uid_unset (x))
#define gid_set(x) (!gid_unset (x))

شمارش
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

ایستا ساختار option ثابت long_opts[] =
{
  {"groups", required_argument, پوچ, GROUPS},
  {"userspec", required_argument, پوچ, USERSPEC},
  {"skip-chdir", no_argument, پوچ, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

#اگر ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
ایستا صحیح
setgroups (size_t size, MAYBE_UNUSED gid_t ثابت *list)
{
  اگر (size == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      بازگشت 0;
    }
  وگرنه
    {
      errno = ENOTSUP;
      بازگشت -1;
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

ایستا صحیح
parse_additional_groups (نویسه ثابت *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = پوچ;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  نویسه *buffer = xstrdup (groups);
  صحیح ret = 0;

  برای (نویسه ثابت *tmp = strtok (buffer, ","); tmp;
       tmp = strtok (پوچ, ","))
    {
      ساختار group *g;
      uintmax_t مقدار;

      اگر (xstrtoumax (tmp, پوچ, 10, &مقدار, "") == LONGINT_OK
          && مقدار <= MAXGID)
        {
          وقتی (isspace (to_uchar (*tmp)))
            tmp++;
          اگر (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              اگر (g != پوچ)
                مقدار = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (ساختار group *) (intptr_t) ! پوچ;
        }
      وگرنه
        {
          g = getgrnam (tmp);
          اگر (g != پوچ)
            مقدار = g->gr_gid;
        }

      اگر (g == پوچ)
        {
          ret = -1;

          اگر (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              ادامه;
            }

          بشکن;
        }

      اگر (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, اندازه *gids);
      gids[n_gids++] = مقدار;
    }

  اگر (ret == 0 && n_gids == 0)
    {
      اگر (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  اگر (ret == 0)
    *pn_gids = n_gids;

  آزاد (buffer);
  بازگشت ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

ایستا bool
is_ریشه (نویسه ثابت *dir)
{
  نویسه *resolved = canonicalize_file_name (dir);
  bool is_res_ریشه = resolved && streq ("/", resolved);
  آزاد (resolved);
  بازگشت is_res_ریشه;
}

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), program_name);

      fputs (_("\
Run COMMAND with ریشه directory set to NEWROOT.\n\
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
         انجامبده not change working directory to %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, run '\"$SHELL\" -i' (پیشفرض: '/bin/sh -i').\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح c;

  /* Input user and groups spec.  */
  نویسه *userspec = پوچ;
  نویسه ثابت *username = پوچ;
  نویسه ثابت *groups = پوچ;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *out_gids = پوچ;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  وقتی ((c = getopt_long (argc, argv, "+", long_opts, پوچ)) != -1)
    {
      انتخاب (c)
        {
        حالت USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = strlen (userspec);
            اگر (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            بشکن;
          }

        حالت GROUPS:
          groups = optarg;
          بشکن;

        حالت SKIP_CHDIR:
          skip_chdir = true;
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_CANCELED);
        }
    }

  اگر (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  نویسه ثابت *newroot = argv[optind];
  bool is_oldroot = is_ریشه (newroot);

  اگر (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  اگر (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      اگر (userspec)
        ignore_مقدار (parse_user_spec (userspec, &uid, &gid, پوچ, پوچ));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      اگر (uid_set (uid) && (! groups || gid_unset (gid)))
        {
          ثابت ساختار passwd *pwd;
          اگر ((pwd = getpwuid (uid)))
            {
              اگر (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      اگر (groups && *groups)
        ignore_مقدار (parse_additional_groups (groups, &out_gids, &n_gids,
                                               false));
#اگر HAVE_SETGROUPS
      وگرنه اگر (! groups && gid_set (gid) && username)
        {
          صحیح ngroups = xgetgroups (username, gid, &out_gids);
          اگر (0 < ngroups)
            n_gids = ngroups;
        }
#endif
    }

  اگر (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  اگر (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  اگر (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      نویسه *shell = getenv ("SHELL");
      اگر (shell == پوچ)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = پوچ;
    }
  وگرنه
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  اگر (userspec)
    {
      bool warn;
      نویسه ثابت *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              پوچ, پوچ, &warn);
      اگر (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  اگر (uid_set (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      ثابت ساختار passwd *pwd;
      اگر ((pwd = getpwuid (uid)))
        {
          اگر (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      وگرنه اگر (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = out_gids;
  GETGROUPS_T *in_gids = پوچ;
  اگر (groups && *groups)
    {
      اگر (parse_additional_groups (groups, &in_gids, &n_gids, !n_gids) != 0)
        {
          اگر (! n_gids)
            بازگشت EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      وگرنه
        gids = in_gids;
    }
#اگر HAVE_SETGROUPS
  وگرنه اگر (! groups && gid_set (gid) && username)
    {
      صحیح ngroups = xgetgroups (username, gid, &in_gids);
      اگر (ngroups <= 0)
        {
          اگر (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      وگرنه
        {
          n_gids = ngroups;
          gids = in_gids;
        }
    }
#endif

  اگر ((uid_set (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  آزاد (in_gids);
  آزاد (out_gids);

  اگر (gid_set (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  اگر (uid_set (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  صحیح exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  بازگشت exit_status;
}
