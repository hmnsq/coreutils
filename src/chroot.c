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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <pwd.h>
#ଅନ୍ତର୍ଭାଵ <grp.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "ignore-value.h"
#ଅନ୍ତର୍ଭାଵ "mgetgroups.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "userspec.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "chroot"

#define AUTHORS proper_name ("Roland McGrath")

#ifndef MAXGID
# define MAXGID GID_T_MAX
#endif

ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool uid_unset (uid_t uid) { ଫେରନ୍ତୁ uid == (uid_t) -1; }
ସ୍ଥିର ଧାଡ଼ିଭିତରେ bool gid_unset (gid_t gid) { ଫେରନ୍ତୁ gid == (gid_t) -1; }
#define uid_set(x) (!uid_unset (x))
#define gid_set(x) (!gid_unset (x))

ଗଣନା
{
  GROUPS = UCHAR_MAX + 1,
  USERSPEC,
  SKIP_CHDIR
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_opts[] =
{
  {"groups", required_argument, ଖାଲି, GROUPS},
  {"userspec", required_argument, ଖାଲି, USERSPEC},
  {"skip-chdir", no_argument, ଖାଲି, SKIP_CHDIR},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

#ଯଦି ! HAVE_SETGROUPS
/* At least Interix lacks supplemental group support.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setgroups (size_t size, MAYBE_UNUSED gid_t ଅଚଳ *list)
{
  ଯଦି (size == 0)
    {
      /* Return success when clearing supplemental groups
         as ! HAVE_SETGROUPS should only be the case on
         platforms that don't support supplemental groups.  */
      ଫେରନ୍ତୁ 0;
    }
  ଅନ୍ୟଥା
    {
      errno = ENOTSUP;
      ଫେରନ୍ତୁ -1;
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
parse_additional_groups (ଅକ୍ଷର ଅଚଳ *groups, GETGROUPS_T **pgids,
                         idx_t *pn_gids, bool show_errors)
{
  GETGROUPS_T *gids = ଖାଲି;
  idx_t n_gids_allocated = 0;
  idx_t n_gids = 0;
  ଅକ୍ଷର *buffer = xstrdup (groups);
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

  ପାଇଁ (ଅକ୍ଷର ଅଚଳ *tmp = strtok (buffer, ","); tmp;
       tmp = strtok (ଖାଲି, ","))
    {
      ସଂରଚନା group *g;
      uintmax_t ମୂଲ୍ୟ;

      ଯଦି (xstrtoumax (tmp, ଖାଲି, 10, &ମୂଲ୍ୟ, "") == LONGINT_OK
          && ମୂଲ୍ୟ <= MAXGID)
        {
          ଯେତେବେଳେ (isspace (to_uchar (*tmp)))
            tmp++;
          ଯଦି (*tmp != '+')
            {
              /* Handle the case where the name is numeric.  */
              g = getgrnam (tmp);
              ଯଦି (g != ଖାଲି)
                ମୂଲ୍ୟ = g->gr_gid;
            }
          /* Flag that we've got a group from the number.  */
          g = (ସଂରଚନା group *) (intptr_t) ! ଖାଲି;
        }
      ଅନ୍ୟଥା
        {
          g = getgrnam (tmp);
          ଯଦି (g != ଖାଲି)
            ମୂଲ୍ୟ = g->gr_gid;
        }

      ଯଦି (g == ଖାଲି)
        {
          ret = -1;

          ଯଦି (show_errors)
            {
              error (0, errno, _("invalid group %s"), quote (tmp));
              ଜାରି;
            }

          ଭାଙ୍ଗ;
        }

      ଯଦି (n_gids == n_gids_allocated)
        gids = xpalloc (gids, &n_gids_allocated, 1, -1, ଆକାର *gids);
      gids[n_gids++] = ମୂଲ୍ୟ;
    }

  ଯଦି (ret == 0 && n_gids == 0)
    {
      ଯଦି (show_errors)
        error (0, 0, _("invalid group list %s"), quote (groups));
      ret = -1;
    }

  *pgids = gids;

  ଯଦି (ret == 0)
    *pn_gids = n_gids;

  ମୁକ୍ତ (buffer);
  ଫେରନ୍ତୁ ret;
}

/* Return whether the passed path is equivalent to "/".
   Note we don't compare against get_root_dev_ino() as "/"
   could be bind mounted to a separate location.  */

ସ୍ଥିର bool
is_ମୂଳ (ଅକ୍ଷର ଅଚଳ *dir)
{
  ଅକ୍ଷର *resolved = canonicalize_file_name (dir);
  bool is_res_ମୂଳ = resolved && streq ("/", resolved);
  ମୁକ୍ତ (resolved);
  ଫେରନ୍ତୁ is_res_ମୂଳ;
}

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... NEWROOT [COMMAND [ARG]...]\n"), program_name);

      fputs (_("\
Run COMMAND with ମୂଳ directory set to NEWROOT.\n\
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
         କର not change working directory to %s\n\
"), quoteaf ("/"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
If no command is given, run '\"$SHELL\" -i' (ପୂର୍ବନିର୍ଦ୍ଧାରିତ: '/bin/sh -i').\n\
"), stdout);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;

  /* Input user and groups spec.  */
  ଅକ୍ଷର *userspec = ଖାଲି;
  ଅକ୍ଷର ଅଚଳ *username = ଖାଲି;
  ଅକ୍ଷର ଅଚଳ *groups = ଖାଲି;
  bool skip_chdir = false;

  /* Parsed user and group IDs.  */
  uid_t uid = -1;
  gid_t gid = -1;
  /* Whether GID was inferred from UID outside the chroot,
     in which case a look-up inside the chroot takes precedence.  */
  bool gid_inferred = false;
  GETGROUPS_T *out_gids = ଖାଲି;
  idx_t n_gids = 0;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ଯେତେବେଳେ ((c = getopt_long (argc, argv, "+", long_opts, ଖାଲି)) != -1)
    {
      ବାଛ (c)
        {
        ସ୍ଥିତି USERSPEC:
          {
            userspec = optarg;
            /* Treat 'user:' just like 'user'
               as we lookup the primary group by default
               (and support doing so for UIDs as well as names.  */
            idx_t userlen = strlen (userspec);
            ଯଦି (userlen && userspec[userlen - 1] == ':')
              userspec[userlen - 1] = '\0';
            ଭାଙ୍ଗ;
          }

        ସ୍ଥିତି GROUPS:
          groups = optarg;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି SKIP_CHDIR:
          skip_chdir = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_CANCELED);
        }
    }

  ଯଦି (argc <= optind)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_CANCELED);
    }

  ଅକ୍ଷର ଅଚଳ *newroot = argv[optind];
  bool is_oldroot = is_ମୂଳ (newroot);

  ଯଦି (! is_oldroot && skip_chdir)
    {
      error (0, 0, _("option --skip-chdir only permitted if NEWROOT is old %s"),
             quoteaf ("/"));
      usage (EXIT_CANCELED);
    }

  ଯଦି (! is_oldroot)
    {
      /* We have to look up users and groups twice.
        - First, outside the chroot to load potentially necessary passwd/group
          parsing plugins (e.g. NSS);
        - Second, inside chroot to redo parsing in case IDs are different.
          Within chroot lookup is the main justification for having
          the --user option supported by the chroot command itself.  */
      ଯଦି (userspec)
        ignore_ମୂଲ୍ୟ (parse_user_spec (userspec, &uid, &gid, ଖାଲି, ଖାଲି));

      /* If no gid is supplied or looked up, do so now.
        Also lookup the username for use with getgroups.  */
      ଯଦି (uid_set (uid) && (! groups || gid_unset (gid)))
        {
          ଅଚଳ ସଂରଚନା passwd *pwd;
          ଯଦି ((pwd = getpwuid (uid)))
            {
              ଯଦି (gid_unset (gid))
                {
                  gid = pwd->pw_gid;
                  gid_inferred = true;
                }
              username = pwd->pw_name;
            }
        }

      ଯଦି (groups && *groups)
        ignore_ମୂଲ୍ୟ (parse_additional_groups (groups, &out_gids, &n_gids,
                                               false));
#ଯଦି HAVE_SETGROUPS
      ଅନ୍ୟଥା ଯଦି (! groups && gid_set (gid) && username)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ngroups = xgetgroups (username, gid, &out_gids);
          ଯଦି (0 < ngroups)
            n_gids = ngroups;
        }
#endif
    }

  ଯଦି (chroot (newroot) != 0)
    error (EXIT_CANCELED, errno, _("cannot change root directory to %s"),
           quoteaf (newroot));

  ଯଦି (! skip_chdir && chdir ("/"))
    error (EXIT_CANCELED, errno, _("cannot chdir to root directory"));

  ଯଦି (argc == optind + 1)
    {
      /* No command.  Run an interactive shell.  */
      ଅକ୍ଷର *shell = getenv ("SHELL");
      ଯଦି (shell == ଖାଲି)
        shell = bad_cast ("/bin/sh");
      argv[0] = shell;
      argv[1] = bad_cast ("-i");
      argv[2] = ଖାଲି;
    }
  ଅନ୍ୟଥା
    {
      /* The following arguments give the command.  */
      argv += optind + 1;
    }

  /* Attempt to set all three: supplementary groups, group ID, user ID.
     Diagnose any failures.  If any have failed, exit before execvp.  */
  ଯଦି (userspec)
    {
      bool warn;
      ଅକ୍ଷର ଅଚଳ *err = parse_user_spec_warn (userspec, &uid, &gid,
                                              ଖାଲି, ଖାଲି, &warn);
      ଯଦି (err)
        error (warn ? 0 : EXIT_CANCELED, 0, "%s", (err));
    }

  /* If no gid is supplied or looked up, do so now.
     Also lookup the username for use with getgroups.  */
  ଯଦି (uid_set (uid) && (! groups || gid_unset (gid) || gid_inferred))
    {
      ଅଚଳ ସଂରଚନା passwd *pwd;
      ଯଦି ((pwd = getpwuid (uid)))
        {
          ଯଦି (gid_unset (gid) || gid_inferred)
            gid = pwd->pw_gid;
          username = pwd->pw_name;
        }
      ଅନ୍ୟଥା ଯଦି (gid_unset (gid))
        {
          error (EXIT_CANCELED, errno,
                 _("no group specified for unknown uid: %ju"),
                 (uintmax_t) uid);
        }
    }

  GETGROUPS_T *gids = out_gids;
  GETGROUPS_T *in_gids = ଖାଲି;
  ଯଦି (groups && *groups)
    {
      ଯଦି (parse_additional_groups (groups, &in_gids, &n_gids, !n_gids) != 0)
        {
          ଯଦି (! n_gids)
            ଫେରନ୍ତୁ EXIT_CANCELED;
          /* else look-up outside the chroot worked, then go with those.  */
        }
      ଅନ୍ୟଥା
        gids = in_gids;
    }
#ଯଦି HAVE_SETGROUPS
  ଅନ୍ୟଥା ଯଦି (! groups && gid_set (gid) && username)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ngroups = xgetgroups (username, gid, &in_gids);
      ଯଦି (ngroups <= 0)
        {
          ଯଦି (! n_gids)
            error (EXIT_CANCELED, errno,
                   _("failed to get supplemental groups"));
          /* else look-up outside the chroot worked, then go with those.  */
        }
      ଅନ୍ୟଥା
        {
          n_gids = ngroups;
          gids = in_gids;
        }
    }
#endif

  ଯଦି ((uid_set (uid) || groups) && setgroups (n_gids, gids) != 0)
    error (EXIT_CANCELED, errno, _("failed to set supplemental groups"));

  ମୁକ୍ତ (in_gids);
  ମୁକ୍ତ (out_gids);

  ଯଦି (gid_set (gid) && setgid (gid))
    error (EXIT_CANCELED, errno, _("failed to set group-ID"));

  ଯଦି (uid_set (uid) && setuid (uid))
    error (EXIT_CANCELED, errno, _("failed to set user-ID"));

  /* Execute the given command.  */
  execvp (argv[0], argv);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, _("failed to run command %s"), quote (argv[0]));
  ଫେରନ୍ତୁ exit_status;
}
