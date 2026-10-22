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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <pwd.h>
#અન્તર્ભાવ <grp.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "ignore-value.h"
#અન્તર્ભાવ "mgetgroups.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "userspec.h"
#અન્તર્ભાવ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chroot"

#define AUTHORS proper_name ("Roland McGrath")

#ifndef MAXGID
# define MAXGID GID_T_MAX
#endif

સ્થિર અંતઃરેખ bool uid_unset (uid_t uid) { પાછા uid == (uid_t) -1; }
સ્થિર અંતઃરેખ bool gid_unset (gid_t gid) { પાછા gid == (gid_t) -1; }
#define uid_set(x) (!uid_unset (x))
#define gid_set(x) (!gid_unset (x))

ગણના
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

સ્થિર સંરચના option અચલ long_opts[] =
{
  {"groups", required_argument, ખાલી, GROUPS},
  {"userspec", required_argument, ખાલી, USERSPEC},
  {"skip-chdir", no_argument, ખાલી, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

#જો ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
સ્થિર પૂર્ણાંક
setgroups (size_t size, MAYBE_UNUSED gid_t અચલ *list)
{
  જો (size == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      પાછા 0;
    }
  નહીંતર
    {
      errno = ENOTSUP;
      પાછા -1;
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

સ્થિર પૂર્ણાંક
parse_additional_groups (અક્ષર અચલ *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = ખાલી;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  અક્ષર *buffer = xstrdup (groups);
  પૂર્ણાંક ret = 0;

  માટે (અક્ષર અચલ *tmp = strtok (buffer, ","); tmp;
       tmp = strtok (ખાલી, ","))
    {
      સંરચના group *g;
      uintmax_t કિંમત;

      જો (xstrtoumax (tmp, ખાલી, 10, &કિંમત, "") == LONGINT_OK
          && કિંમત <= MAXGID)
        {
          જ્યારે (isspace (to_uchar (*tmp)))
            tmp++;
          જો (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              જો (g != ખાલી)
                કિંમત = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (સંરચના group *) (intptr_t) ! ખાલી;
        }
      નહીંતર
        {
          g = getgrnam (tmp);
          જો (g != ખાલી)
            કિંમત = g->gr_gid;
        }

      જો (g == ખાલી)
        {
          ret = -1;

          જો (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              ચાલુ;
            }

          તોડો;
        }

      જો (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, કદ *gids);
      gids[n_gids++] = કિંમત;
    }

  જો (ret == 0 && n_gids == 0)
    {
      જો (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  જો (ret == 0)
    *pn_gids = n_gids;

  મુક્ત (buffer);
  પાછા ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

સ્થિર bool
is_મૂળ (અક્ષર અચલ *dir)
{
  અક્ષર *resolved = canonicalize_file_name (dir);
  bool is_res_મૂળ = resolved && streq ("/", resolved);
  મુક્ત (resolved);
  પાછા is_res_મૂળ;
}

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), program_name);

      fputs (_("\
Run COMMAND with મૂળ directory set to NEWROOT.\n\
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
         કરો not change working directory to %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, run '\"$SHELL\" -i' (મૂળભૂત: '/bin/sh -i').\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક c;

  /* Input user and groups spec.  */
  અક્ષર *userspec = ખાલી;
  અક્ષર અચલ *username = ખાલી;
  અક્ષર અચલ *groups = ખાલી;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *out_gids = ખાલી;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  જ્યારે ((c = getopt_long (argc, argv, "+", long_opts, ખાલી)) != -1)
    {
      પસંદગી (c)
        {
        સ્થિતિ USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = strlen (userspec);
            જો (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            તોડો;
          }

        સ્થિતિ GROUPS:
          groups = optarg;
          તોડો;

        સ્થિતિ SKIP_CHDIR:
          skip_chdir = true;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_CANCELED);
        }
    }

  જો (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  અક્ષર અચલ *newroot = argv[optind];
  bool is_oldroot = is_મૂળ (newroot);

  જો (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  જો (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      જો (userspec)
        ignore_કિંમત (parse_user_spec (userspec, &uid, &gid, ખાલી, ખાલી));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      જો (uid_set (uid) && (! groups || gid_unset (gid)))
        {
          અચલ સંરચના passwd *pwd;
          જો ((pwd = getpwuid (uid)))
            {
              જો (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      જો (groups && *groups)
        ignore_કિંમત (parse_additional_groups (groups, &out_gids, &n_gids,
                                               false));
#જો HAVE_SETGROUPS
      નહીંતર જો (! groups && gid_set (gid) && username)
        {
          પૂર્ણાંક ngroups = xgetgroups (username, gid, &out_gids);
          જો (0 < ngroups)
            n_gids = ngroups;
        }
#endif
    }

  જો (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  જો (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  જો (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      અક્ષર *shell = getenv ("SHELL");
      જો (shell == ખાલી)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = ખાલી;
    }
  નહીંતર
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  જો (userspec)
    {
      bool warn;
      અક્ષર અચલ *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              ખાલી, ખાલી, &warn);
      જો (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  જો (uid_set (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      અચલ સંરચના passwd *pwd;
      જો ((pwd = getpwuid (uid)))
        {
          જો (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      નહીંતર જો (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = out_gids;
  GETGROUPS_T *in_gids = ખાલી;
  જો (groups && *groups)
    {
      જો (parse_additional_groups (groups, &in_gids, &n_gids, !n_gids) != 0)
        {
          જો (! n_gids)
            પાછા EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      નહીંતર
        gids = in_gids;
    }
#જો HAVE_SETGROUPS
  નહીંતર જો (! groups && gid_set (gid) && username)
    {
      પૂર્ણાંક ngroups = xgetgroups (username, gid, &in_gids);
      જો (ngroups <= 0)
        {
          જો (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      નહીંતર
        {
          n_gids = ngroups;
          gids = in_gids;
        }
    }
#endif

  જો ((uid_set (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  મુક્ત (in_gids);
  મુક્ત (out_gids);

  જો (gid_set (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  જો (uid_set (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  પૂર્ણાંક exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  પાછા exit_status;
}
