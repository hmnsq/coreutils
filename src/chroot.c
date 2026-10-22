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
#تعریف_کرو پروگرام_NAME "chroot"

#تعریف_کرو AUTHORS proper_name ("Roland McGrath")

#اگر_غیر_تعریف_شدہ MAXGID
# define MAXGID GID_T_MAX
#اگر_ختم

ساکن اندرونی bool uid_unset (uid_t uid) { واپس uid == (uid_t) -1; }
ساکن اندرونی bool gid_unset (gid_t gid) { واپس gid == (gid_t) -1; }
#تعریف_کرو uid_سیٹ(x) (!uid_unset (x))
#تعریف_کرو gid_سیٹ(x) (!gid_unset (x))

شمار
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

ساکن ساخت option ثابت long_opts[] =
{
  {"groups", required_دلیل, تہی, GROUPS},
  {"userspec", required_دلیل, تہی, USERSPEC},
  {"skip-chdir", no_دلیل, تہی, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

#اگر ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
ساکن صحیح
setgroups (size_t سائز, MAYBE_غیر_مستعمل gid_t ثابت *list)
{
  اگر (سائز == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      واپس 0;
    }
  ورنہ
    {
      errno = ENOTSUP;
      واپس -1;
    }
}
#اگر_ختم

/* Determine the group IDs for the specified supplementary GROUPS,
   which is a comma separated list of supplementary groups (names or numbers).
   Allocate an array for the parsed IDs and store it in PGIDS,
   which may be allocated even on parse failure.
   Update the number of parsed groups in PN_GIDS on success.
   Upon any failure return nonzero, and issue diagnostic if SHOW_ERRORS is true.
   Otherwise return zero.  */

ساکن صحیح
parse_additional_groups (حرف ثابت *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = تہی;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  حرف *بفر = xstrdup (groups);
  صحیح ret = 0;

  برائے (حرف ثابت *tmp = strtok (بفر, ","); tmp;
       tmp = strtok (تہی, ","))
    {
      ساخت گروہ *g;
      uintmax_t قدر;

      اگر (xstrtoumax (tmp, تہی, 10, &قدر, "") == LONGINT_OK
          && قدر <= MAXGID)
        {
          جب (خلا_ہے (مقصد_uchar (*tmp)))
            tmp++;
          اگر (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              اگر (g != تہی)
                قدر = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (ساخت گروہ *) (intptr_t) ! تہی;
        }
      ورنہ
        {
          g = getgrnam (tmp);
          اگر (g != تہی)
            قدر = g->gr_gid;
        }

      اگر (g == تہی)
        {
          ret = -1;

          اگر (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              جاری;
            }

          توڑو;
        }

      اگر (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, حجم *gids);
      gids[n_gids++] = قدر;
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

  آزاد (بفر);
  واپس ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

ساکن bool
is_جڑ (حرف ثابت *dir)
{
  حرف *resolved = canonicalize_file_name (dir);
  bool is_res_جڑ = resolved && streq ("/", resolved);
  آزاد (resolved);
  واپس is_res_جڑ;
}

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), پروگرام_name);

      fputs (_("\
تسلسل COMMAND with جڑ directory سیٹ_2 مقصد NEWROOT.\n\
\n\
"), stdout);

      oputs (_("\
      --groups=G_LIST\n\
         specify supplementary groups as g1,g2,..,gN\n\
"));
      oputs (_("\
      --userspec=USER:گروہ_2\n\
         specify user and گروہ (ID or name) مقصد use\n\
"));
      oprintf (_("\
      --skip-chdir\n\
         کرو not change working directory مقصد %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, تسلسل_2 '\"$SHELL\" -i' (طےشدہ: '/bin/sh -i').\n\
"), stdout);
      emit_exec_حالت (پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح c;

  /* Input user and groups spec.  */
  حرف *userspec = تہی;
  حرف ثابت *username = تہی;
  حرف ثابت *groups = تہی;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *برآمد_gids = تہی;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  جب ((c = getopt_long (argc, argv, "+", long_opts, تہی)) != -1)
    {
      انتخاب (c)
        {
        حالت USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = لڑی_لمبائی (userspec);
            اگر (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            توڑو;
          }

        حالت GROUPS:
          groups = optarg;
          توڑو;

        حالت SKIP_CHDIR:
          skip_chdir = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_CANCELED);
        }
    }

  اگر (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  حرف ثابت *newroot = argv[optind];
  bool is_oldroot = is_جڑ (newroot);

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
        ignore_قدر (parse_user_spec (userspec, &uid, &gid, تہی, تہی));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      اگر (uid_سیٹ (uid) && (! groups || gid_unset (gid)))
        {
          ثابت ساخت passwd *pwd;
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
        ignore_قدر (parse_additional_groups (groups, &برآمد_gids, &n_gids,
                                               false));
#اگر HAVE_SETGROUPS
      ورنہ اگر (! groups && gid_سیٹ (gid) && username)
        {
          صحیح ngroups = xgetgroups (username, gid, &برآمد_gids);
          اگر (0 < ngroups)
            n_gids = ngroups;
        }
#اگر_ختم
    }

  اگر (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  اگر (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  اگر (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      حرف *shell = getenv ("SHELL");
      اگر (shell == تہی)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = تہی;
    }
  ورنہ
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  اگر (userspec)
    {
      bool warn;
      حرف ثابت *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              تہی, تہی, &warn);
      اگر (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  اگر (uid_سیٹ (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      ثابت ساخت passwd *pwd;
      اگر ((pwd = getpwuid (uid)))
        {
          اگر (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      ورنہ اگر (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = برآمد_gids;
  GETGROUPS_T *اندر_gids = تہی;
  اگر (groups && *groups)
    {
      اگر (parse_additional_groups (groups, &اندر_gids, &n_gids, !n_gids) != 0)
        {
          اگر (! n_gids)
            واپس EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      ورنہ
        gids = اندر_gids;
    }
#اگر HAVE_SETGROUPS
  ورنہ اگر (! groups && gid_سیٹ (gid) && username)
    {
      صحیح ngroups = xgetgroups (username, gid, &اندر_gids);
      اگر (ngroups <= 0)
        {
          اگر (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      ورنہ
        {
          n_gids = ngroups;
          gids = اندر_gids;
        }
    }
#اگر_ختم

  اگر ((uid_سیٹ (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  آزاد (اندر_gids);
  آزاد (برآمد_gids);

  اگر (gid_سیٹ (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  اگر (uid_سیٹ (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  صحیح exit_حالت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  واپس exit_حالت;
}
