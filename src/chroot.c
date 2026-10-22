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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <pwd.h>
#অন্তর্ভাৱ <grp.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "ignore-value.h"
#অন্তর্ভাৱ "mgetgroups.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "userspec.h"
#অন্তর্ভাৱ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chroot"

#define AUTHORS proper_name ("Roland McGrath")

#ifndef MAXGID
# define MAXGID GID_T_MAX
#endif

স্থির অন্তঃস্থ bool uid_unset (uid_t uid) { ঘূৰাই uid == (uid_t) -1; }
স্থির অন্তঃস্থ bool gid_unset (gid_t gid) { ঘূৰাই gid == (gid_t) -1; }
#define uid_set(x) (!uid_unset (x))
#define gid_set(x) (!gid_unset (x))

গণনা
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

স্থির সংরচনা option ধ্রুৱ long_opts[] =
{
  {"groups", required_argument, রিক্ত, GROUPS},
  {"userspec", required_argument, রিক্ত, USERSPEC},
  {"skip-chdir", no_argument, রিক্ত, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

#যদি ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
স্থির পূর্ণাঙ্ক
setgroups (size_t size, MAYBE_UNUSED gid_t ধ্রুৱ *list)
{
  যদি (size == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      ঘূৰাই 0;
    }
  অন্যথা
    {
      errno = ENOTSUP;
      ঘূৰাই -1;
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

স্থির পূর্ণাঙ্ক
parse_additional_groups (ৱর্ণ ধ্রুৱ *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = রিক্ত;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  ৱর্ণ *buffer = xstrdup (groups);
  পূর্ণাঙ্ক ret = 0;

  বাবে (ৱর্ণ ধ্রুৱ *tmp = strtok (buffer, ","); tmp;
       tmp = strtok (রিক্ত, ","))
    {
      সংরচনা group *g;
      uintmax_t মান;

      যদি (xstrtoumax (tmp, রিক্ত, 10, &মান, "") == LONGINT_OK
          && মান <= MAXGID)
        {
          যেতিয়ালৈ (isspace (to_uchar (*tmp)))
            tmp++;
          যদি (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              যদি (g != রিক্ত)
                মান = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (সংরচনা group *) (intptr_t) ! রিক্ত;
        }
      অন্যথা
        {
          g = getgrnam (tmp);
          যদি (g != রিক্ত)
            মান = g->gr_gid;
        }

      যদি (g == রিক্ত)
        {
          ret = -1;

          যদি (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              অনুৱর্ত;
            }

          ভঙ্গ;
        }

      যদি (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, পরিমাণ *gids);
      gids[n_gids++] = মান;
    }

  যদি (ret == 0 && n_gids == 0)
    {
      যদি (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  যদি (ret == 0)
    *pn_gids = n_gids;

  মুক্ত (buffer);
  ঘূৰাই ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

স্থির bool
is_মূল (ৱর্ণ ধ্রুৱ *dir)
{
  ৱর্ণ *resolved = canonicalize_file_name (dir);
  bool is_res_মূল = resolved && streq ("/", resolved);
  মুক্ত (resolved);
  ঘূৰাই is_res_মূল;
}

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), program_name);

      fputs (_("\
Run COMMAND with মূল directory set to NEWROOT.\n\
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
         কৰা not change working directory to %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, run '\"$SHELL\" -i' (স্ৱভাৱ: '/bin/sh -i').\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  পূর্ণাঙ্ক c;

  /* Input user and groups spec.  */
  ৱর্ণ *userspec = রিক্ত;
  ৱর্ণ ধ্রুৱ *username = রিক্ত;
  ৱর্ণ ধ্রুৱ *groups = রিক্ত;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *out_gids = রিক্ত;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  যেতিয়ালৈ ((c = getopt_long (argc, argv, "+", long_opts, রিক্ত)) != -1)
    {
      ৱিকল্প (c)
        {
        পক্ষ USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = strlen (userspec);
            যদি (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            ভঙ্গ;
          }

        পক্ষ GROUPS:
          groups = optarg;
          ভঙ্গ;

        পক্ষ SKIP_CHDIR:
          skip_chdir = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্ৱভাৱ:
          usage (EXIT_CANCELED);
        }
    }

  যদি (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  ৱর্ণ ধ্রুৱ *newroot = argv[optind];
  bool is_oldroot = is_মূল (newroot);

  যদি (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  যদি (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      যদি (userspec)
        ignore_মান (parse_user_spec (userspec, &uid, &gid, রিক্ত, রিক্ত));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      যদি (uid_set (uid) && (! groups || gid_unset (gid)))
        {
          ধ্রুৱ সংরচনা passwd *pwd;
          যদি ((pwd = getpwuid (uid)))
            {
              যদি (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      যদি (groups && *groups)
        ignore_মান (parse_additional_groups (groups, &out_gids, &n_gids,
                                               false));
#যদি HAVE_SETGROUPS
      অন্যথা যদি (! groups && gid_set (gid) && username)
        {
          পূর্ণাঙ্ক ngroups = xgetgroups (username, gid, &out_gids);
          যদি (0 < ngroups)
            n_gids = ngroups;
        }
#endif
    }

  যদি (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  যদি (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  যদি (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      ৱর্ণ *shell = getenv ("SHELL");
      যদি (shell == রিক্ত)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = রিক্ত;
    }
  অন্যথা
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  যদি (userspec)
    {
      bool warn;
      ৱর্ণ ধ্রুৱ *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              রিক্ত, রিক্ত, &warn);
      যদি (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  যদি (uid_set (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      ধ্রুৱ সংরচনা passwd *pwd;
      যদি ((pwd = getpwuid (uid)))
        {
          যদি (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      অন্যথা যদি (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = out_gids;
  GETGROUPS_T *in_gids = রিক্ত;
  যদি (groups && *groups)
    {
      যদি (parse_additional_groups (groups, &in_gids, &n_gids, !n_gids) != 0)
        {
          যদি (! n_gids)
            ঘূৰাই EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      অন্যথা
        gids = in_gids;
    }
#যদি HAVE_SETGROUPS
  অন্যথা যদি (! groups && gid_set (gid) && username)
    {
      পূর্ণাঙ্ক ngroups = xgetgroups (username, gid, &in_gids);
      যদি (ngroups <= 0)
        {
          যদি (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      অন্যথা
        {
          n_gids = ngroups;
          gids = in_gids;
        }
    }
#endif

  যদি ((uid_set (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  মুক্ত (in_gids);
  মুক্ত (out_gids);

  যদি (gid_set (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  যদি (uid_set (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  পূর্ণাঙ্ক exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ঘূৰাই exit_status;
}
