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

#ضمن <config.h>
#ضمن <getopt.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <pwd.h>
#ضمن <grp.h>

#ضمن "system.h"
#ضمن "ignore-value.h"
#ضمن "mgetgroups.h"
#ضمن "quote.h"
#ضمن "userspec.h"
#ضمن "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کن برنامه_NAME "chroot"

#تعریف_کن AUTHORS proper_name ("Roland McGrath")

#اگر_تعریف_نشده MAXGID
# define MAXGID GID_T_MAX
#پایان_اگر

ساكن مضمن bool uid_unset (uid_t uid) { أرجع uid == (uid_t) -1; }
ساكن مضمن bool gid_unset (gid_t gid) { أرجع gid == (gid_t) -1; }
#تعریف_کن uid_مجموعه(x) (!uid_unset (x))
#تعریف_کن gid_مجموعه(x) (!gid_unset (x))

تعداد
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

ساكن بنية option ثابت long_opts[] =
{
  {"groups", required_آرگومان, عدم, GROUPS},
  {"userspec", required_آرگومان, عدم, USERSPEC},
  {"skip-chdir", no_آرگومان, عدم, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

#إذا ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
ساكن صحيح
setgroups (size_t اندازه, MAYBE_استفادهنشده gid_t ثابت *list)
{
  إذا (اندازه == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      أرجع 0;
    }
  وإلا
    {
      errno = ENOTSUP;
      أرجع -1;
    }
}
#پایان_اگر

/* Determine the group IDs for the specified supplementary GROUPS,
   which is a comma separated list of supplementary groups (names or numbers).
   Allocate an array for the parsed IDs and store it in PGIDS,
   which may be allocated even on parse failure.
   Update the number of parsed groups in PN_GIDS on success.
   Upon any failure return nonzero, and issue diagnostic if SHOW_ERRORS is true.
   Otherwise return zero.  */

ساكن صحيح
parse_additional_groups (حرف ثابت *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = عدم;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  حرف *میانگیر = xstrdup (groups);
  صحيح ret = 0;

  لكل (حرف ثابت *tmp = strtok (میانگیر, ","); tmp;
       tmp = strtok (عدم, ","))
    {
      بنية گروه *g;
      uintmax_t مقدار;

      إذا (xstrtoumax (tmp, عدم, 10, &مقدار, "") == LONGINT_OK
          && مقدار <= MAXGID)
        {
          طالما (آیا_فاصله (مقصد_uchar (*tmp)))
            tmp++;
          إذا (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              إذا (g != عدم)
                مقدار = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (بنية گروه *) (intptr_t) ! عدم;
        }
      وإلا
        {
          g = getgrnam (tmp);
          إذا (g != عدم)
            مقدار = g->gr_gid;
        }

      إذا (g == عدم)
        {
          ret = -1;

          إذا (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              استمر;
            }

          اكسر;
        }

      إذا (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, حجم *gids);
      gids[n_gids++] = مقدار;
    }

  إذا (ret == 0 && n_gids == 0)
    {
      إذا (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  إذا (ret == 0)
    *pn_gids = n_gids;

  حرر (میانگیر);
  أرجع ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

ساكن bool
is_ریشه (حرف ثابت *dir)
{
  حرف *resolved = canonicalize_file_name (dir);
  bool is_res_ریشه = resolved && streq ("/", resolved);
  حرر (resolved);
  أرجع is_res_ریشه;
}

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), برنامه_name);

      fputs (_("\
پیاپی COMMAND with ریشه directory مجموعه_2 مقصد NEWROOT.\n\
\n\
"), stdout);

      oputs (_("\
      --groups=G_LIST\n\
         specify supplementary groups as g1,g2,..,gN\n\
"));
      oputs (_("\
      --userspec=USER:گروه_2\n\
         specify user and گروه (ID or name) مقصد use\n\
"));
      oprintf (_("\
      --skip-chdir\n\
         افعل not change working directory مقصد %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, پیاپی_2 '\"$SHELL\" -i' (افتراضي: '/bin/sh -i').\n\
"), stdout);
      emit_exec_وضعیت (برنامه_NAME);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح c;

  /* Input user and groups spec.  */
  حرف *userspec = عدم;
  حرف ثابت *username = عدم;
  حرف ثابت *groups = عدم;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *بیرون_gids = عدم;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  طالما ((c = getopt_long (argc, argv, "+", long_opts, عدم)) != -1)
    {
      بدل (c)
        {
        حالة USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = طول_رشته (userspec);
            إذا (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            اكسر;
          }

        حالة GROUPS:
          groups = optarg;
          اكسر;

        حالة SKIP_CHDIR:
          skip_chdir = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_CANCELED);
        }
    }

  إذا (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  حرف ثابت *newroot = argv[optind];
  bool is_oldroot = is_ریشه (newroot);

  إذا (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  إذا (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      إذا (userspec)
        ignore_مقدار (parse_user_spec (userspec, &uid, &gid, عدم, عدم));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      إذا (uid_مجموعه (uid) && (! groups || gid_unset (gid)))
        {
          ثابت بنية passwd *pwd;
          إذا ((pwd = getpwuid (uid)))
            {
              إذا (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      إذا (groups && *groups)
        ignore_مقدار (parse_additional_groups (groups, &بیرون_gids, &n_gids,
                                               false));
#إذا HAVE_SETGROUPS
      وإلا إذا (! groups && gid_مجموعه (gid) && username)
        {
          صحيح ngroups = xgetgroups (username, gid, &بیرون_gids);
          إذا (0 < ngroups)
            n_gids = ngroups;
        }
#پایان_اگر
    }

  إذا (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  إذا (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  إذا (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      حرف *shell = getenv ("SHELL");
      إذا (shell == عدم)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = عدم;
    }
  وإلا
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  إذا (userspec)
    {
      bool warn;
      حرف ثابت *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              عدم, عدم, &warn);
      إذا (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  إذا (uid_مجموعه (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      ثابت بنية passwd *pwd;
      إذا ((pwd = getpwuid (uid)))
        {
          إذا (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      وإلا إذا (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = بیرون_gids;
  GETGROUPS_T *درون_gids = عدم;
  إذا (groups && *groups)
    {
      إذا (parse_additional_groups (groups, &درون_gids, &n_gids, !n_gids) != 0)
        {
          إذا (! n_gids)
            أرجع EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      وإلا
        gids = درون_gids;
    }
#إذا HAVE_SETGROUPS
  وإلا إذا (! groups && gid_مجموعه (gid) && username)
    {
      صحيح ngroups = xgetgroups (username, gid, &درون_gids);
      إذا (ngroups <= 0)
        {
          إذا (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      وإلا
        {
          n_gids = ngroups;
          gids = درون_gids;
        }
    }
#پایان_اگر

  إذا ((uid_مجموعه (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  حرر (درون_gids);
  حرر (بیرون_gids);

  إذا (gid_مجموعه (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  إذا (uid_مجموعه (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  صحيح exit_وضعیت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  أرجع exit_وضعیت;
}
