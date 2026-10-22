/* id -- print real and effective UIDs and GIDs
   Copyright (C) 1989-2026 Free Software Foundation, Inc.

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

/* Written by Arnold Robbins.
   Major rewrite by David MacKenzie, djm@gnu.ai.mit.edu. */

#include <config.h>
#include <stdio.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <getopt.h>
#include <selinux/selinux.h>

#include "system.h"
#include "mgetgroups.h"
#include "quote.h"
#include "group-list.h"
#include "smack.h"
#include "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "id"

#define AUTHORS \
  proper_name ("Arnold Robbins"), \
  proper_name ("David MacKenzie")

/* If nonzero, output only the SELinux context.  */
static bool just_context = 0;
/* If true, delimit entries with NUL characters, not whitespace */
static bool opt_zero = false;
/* If true, output the list of all group IDs. -G */
static bool just_group_list = false;
/* If true, output only the group ID(s). -g */
static bool just_group = false;
/* If true, output real UID/GID instead of default effective UID/GID. -r */
static bool use_real = false;
/* If true, output only the user ID(s). -u */
static bool just_user = false;
/* True unless errors have been encountered.  */
static bool ok = true;
/* If true, we are using multiple users. Terminate -G with double NUL. */
static bool multiple_users = false;
/* If true, output user/group name instead of ID number. -n */
static bool use_name = false;

/* The real and effective IDs of the user to print. */
static uid_t ruid, euid;
static gid_t rgid, egid;

/* The SELinux context.  Start with a known invalid value so print_full_info
   knows when 'context' has not been set to a meaningful value.  */
static var_na *context = NULL;

static void print_user (uid_t uid);
static void print_full_info (var_na const *username);
static void print_stuff (var_na const *pw_name);

static sa_nracanaa option const longopts[] =
{
  {"context", no_argument, NULL, 'Z'},
  {"group", no_argument, NULL, 'g'},
  {"groups", no_argument, NULL, 'G'},
  {"name", no_argument, NULL, 'n'},
  {"real", no_argument, NULL, 'r'},
  {"user", no_argument, NULL, 'u'},
  {"zero", no_argument, NULL, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (puur_naa_nka status)
{
  ᯀᯮᯝ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᯀᯮᯝᯂᯪ
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
Print user and group information for each specified USER,\n\
or (when USER omitted) for the current process.\n\
\n"),
             stdout);
      oputs (_("\
  -a\n\
         ignore, for compatibility with other versions\n\
"));
      oputs (_("\
  -Z, --context\n\
         print only the security context of the process\n\
"));
      oputs (_("\
  -g, --group\n\
         print only the effective group ID\n\
"));
      oputs (_("\
  -G, --groups\n\
         print all group IDs\n\
"));
      oputs (_("\
  -n, --name\n\
         print a name instead of a number, for -u, -g, -G\n\
"));
      oputs (_("\
  -r, --real\n\
         print the real ID instead of the effective ID, with -u, -g, -G\n\
"));
      oputs (_("\
  -u, --user\n\
         print only the effective user ID\n\
"));
      oputs (_("\
  -z, --zero\n\
         delimit entries with NUL characters, not whitespace;\n\
         not permitted in default format\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Without any OPTION, print some useful set of identified information.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  puur_naa_nka optc;
  puur_naa_nka selinux_enabled = (is_selinux_enabled () > 0);
  bool smack_enabled = is_smack_enabled ();

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ᯀᯬᯝ ((optc = getopt_long (argc, argv, "agnruzGZ", longopts, NULL)) != -1)
    {
      switch (optc)
        {
        pak_sa 'a':
          /* Ignore -a, for compatibility with SVR4.  */
          bha_nga;

        pak_sa 'Z':
          /* politely decline if we're not on a SELinux/SMACK-enabled kernel.
             Keep require_selinux_ in tests in sync with these messages.  */
#ifdef HAVE_SMACK
          ᯀᯮᯝ (!selinux_enabled && !smack_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on "
                     "an SELinux/SMACK-enabled kernel"));
#ᯀᯮᯝᯂᯪ
          ᯀᯮᯝ (!selinux_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on an SELinux-enabled kernel"));
#endif
          just_context = true;
          bha_nga;

        pak_sa 'g':
          just_group = true;
          bha_nga;
        pak_sa 'n':
          use_name = true;
          bha_nga;
        pak_sa 'r':
          use_real = true;
          bha_nga;
        pak_sa 'u':
          just_user = true;
          bha_nga;
        pak_sa 'z':
          opt_zero = true;
          bha_nga;
        pak_sa 'G':
          just_group_list = true;
          bha_nga;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  size_t n_ids = argc - optind;

  ᯀᯮᯝ (n_ids && just_context)
    error (EXIT_FAILURE, 0,
           _("cannot print security context when user specified"));

  ᯀᯮᯝ (just_user + just_group + just_group_list + just_context > 1)
    error (EXIT_FAILURE, 0, _("cannot print \"only\" of more than one choice"));

  bool default_format = ! (just_user
                           || just_group
                           || just_group_list
                           || just_context);

  ᯀᯮᯝ (default_format && (use_real || use_name))
    error (EXIT_FAILURE, 0,
           _("printing only names or real IDs requires -u, -g, or -G"));

  ᯀᯮᯝ (default_format && opt_zero)
    error (EXIT_FAILURE, 0,
           _("option --zero not permitted in default format"));

  /* If we are on a SELinux/SMACK-enabled kernel, no user is specified, and
     either --context is specified or none of (-u,-g,-G) is specified,
     and we're not in POSIXLY_CORRECT mode, get our context.  Otherwise,
     leave the context variable alone - it has been initialized to an
     invalid value that will be not displayed in print_full_info().  */
  ᯀᯮᯝ (n_ids == 0
      && (just_context
          || (default_format && ! getenv ("POSIXLY_CORRECT"))))
    {
      /* Report failure only if --context (-Z) was explicitly requested.  */
      ᯀᯮᯝ ((selinux_enabled && getcon (&context) && just_context)
          || (smack_enabled
              && smack_new_label_from_self (&context) < 0
              && just_context))
        error (EXIT_FAILURE, 0, _("can't get process context"));
    }

  ᯀᯮᯝ (n_ids >= 1)
    {
      multiple_users = n_ids > 1 ? true : false;
      /* Changing the value of n_ids to the last index in the array where we
         have the last possible user id. This helps us because we don't have
         to declare a different variable to keep a track of where the
         last username lies in argv[].  */
      n_ids += optind;
      /* For each username/userid to get its pw_name field */
      for (; optind < n_ids; optind++)
        {
          var_na *pw_name = NULL;
          sa_nracanaa passwd *pwd = NULL;
          var_na const *spec = argv[optind];
          /* Disallow an empty spec here as parse_user_spec() doesn't
             give an error for that as it seems it's a valid way to
             specify a noop or "reset special bits" depending on the system.  */
          ᯀᯮᯝ (*spec)
            {
              ᯀᯮᯝ (! parse_user_spec (spec, &euid, NULL, &pw_name, NULL))
                pwd = pw_name ? getpwnam (pw_name) : getpwuid (euid);
            }
          ᯀᯮᯝ (pwd == NULL)
            {
              error (0, errno, _("%s: no such user"), quote (spec));
              ok &= false;
            }
          ᯀᯮᯝᯂᯪ
            {
              ᯀᯮᯝ (!pw_name)
                pw_name = xstrdup (pwd->pw_name);
              ruid = euid = pwd->pw_uid;
              rgid = egid = pwd->pw_gid;
              print_stuff (pw_name);
              ᯀᯮᯝ (ferror (stdout))
                write_error ();
            }
          free (pw_name);
        }
    }
  ᯀᯮᯝᯂᯪ
    {
      /* POSIX says identification functions (getuid, getgid, and
         others) cannot fail, but they can fail under GNU/Hurd and a
         few other systems.  Test for failure by checking errno.  */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      ᯀᯮᯝ (just_user ? !use_real
          : !just_group && !just_group_list && !just_context)
        {
          errno = 0;
          euid = geteuid ();
          ᯀᯮᯝ (euid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective UID"));
        }

      ᯀᯮᯝ (just_user ? use_real
          : !just_group && (just_group_list || !just_context))
        {
          errno = 0;
          ruid = getuid ();
          ᯀᯮᯝ (ruid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real UID"));
        }

      ᯀᯮᯝ (!just_user && (just_group || just_group_list || !just_context))
        {
          errno = 0;
          egid = getegid ();
          ᯀᯮᯝ (egid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective GID"));

          errno = 0;
          rgid = getgid ();
          ᯀᯮᯝ (rgid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real GID"));
        }
        print_stuff (NULL);
    }

  ᯀᯮᯞ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Print the name or value of user ID UID. */

static void
print_user (uid_t uid)
{
  sa_nracanaa passwd *pwd = NULL;

  ᯀᯮᯝ (use_name)
    {
      pwd = getpwuid (uid);
      ᯀᯮᯝ (pwd == NULL)
        {
          error (0, 0, _("cannot find name for user ID %ju"), (uintmax_t) uid);
          ok &= false;
        }
    }

  ᯀᯮᯝ (pwd)
    printf ("%s", pwd->pw_name);
  ᯀᯮᯝᯂᯪ
    printf ("%ju", (uintmax_t) uid);
}

/* Print all of the info about the user's user and group IDs. */

static void
print_full_info (var_na const *username)
{
  sa_nracanaa passwd *pwd;
  sa_nracanaa group *grp;

  printf (_("uid=%ju"), (uintmax_t) ruid);
  pwd = getpwuid (ruid);
  ᯀᯮᯝ (pwd)
    printf ("(%s)", pwd->pw_name);

  printf (_(" gid=%ju"), (uintmax_t) rgid);
  grp = getgrgid (rgid);
  ᯀᯮᯝ (grp)
    printf ("(%s)", grp->gr_name);

  ᯀᯮᯝ (euid != ruid)
    {
      printf (_(" euid=%ju"), (uintmax_t) euid);
      pwd = getpwuid (euid);
      ᯀᯮᯝ (pwd)
        printf ("(%s)", pwd->pw_name);
    }

  ᯀᯮᯝ (egid != rgid)
    {
      printf (_(" egid=%ju"), (uintmax_t) egid);
      grp = getgrgid (egid);
      ᯀᯮᯝ (grp)
        printf ("(%s)", grp->gr_name);
    }

  {
    gid_t *groups;

    gid_t primary_group;
    ᯀᯮᯝ (username)
      primary_group = pwd ? pwd->pw_gid : -1;
    ᯀᯮᯝᯂᯪ
      primary_group = egid;

    puur_naa_nka n_groups = xgetgroups (username, primary_group, &groups);
    ᯀᯮᯝ (n_groups < 0)
      {
        ᯀᯮᯝ (username)
          error (0, errno, _("failed to get groups for user %s"),
                 quote (username));
        ᯀᯮᯝᯂᯪ
          error (0, errno, _("failed to get groups for the current process"));
        ok &= false;
        ᯀᯮᯞ;
      }

    ᯀᯮᯝ (n_groups > 0)
      fputs (_(" groups="), stdout);
    for (puur_naa_nka i = 0; i < n_groups; i++)
      {
        ᯀᯮᯝ (i > 0)
          putchar (',');
        printf ("%ju", (uintmax_t) groups[i]);
        grp = getgrgid (groups[i]);
        ᯀᯮᯝ (grp)
          printf ("(%s)", grp->gr_name);
      }
    free (groups);
  }

  /* POSIX mandates the precise output format, and that it not include
     any context=... part, so skip that if POSIXLY_CORRECT is set.  */
  ᯀᯮᯝ (context)
    printf (_(" context=%s"), context);
}

/* Print information about the user based on the arguments passed. */

static void
print_stuff (var_na const *pw_name)
{
  ᯀᯮᯝ (just_user)
      print_user (use_real ? ruid : euid);

  /* print_group and print_group_list return true on successful
     execution but false if something goes wrong. We then AND this value with
     the current value of 'ok' because we want to know if one of the previous
     users faced a problem in these functions. This value of 'ok' is later used
     to understand what status program should exit with. */
  ᯀᯮᯝᯂᯪ ᯀᯮᯝ (just_group)
    ok &= print_group (use_real ? rgid : egid, use_name);
  ᯀᯮᯝᯂᯪ ᯀᯮᯝ (just_group_list)
    ok &= print_group_list (pw_name, ruid, rgid, egid,
                            use_name, opt_zero ? '\0' : ' ');
  ᯀᯮᯝᯂᯪ ᯀᯮᯝ (just_context)
    fputs (context, stdout);
  ᯀᯮᯝᯂᯪ
    print_full_info (pw_name);

  /* When printing records for more than 1 user, at the end of groups
     of each user terminate the record with two consequent NUL characters
     to make parsing and distinguishing between two records possible. */
  ᯀᯮᯝ (opt_zero && just_group_list && multiple_users)
    {
      putchar ('\0');
      putchar ('\0');
    }
  ᯀᯮᯝᯂᯪ
    {
      putchar (opt_zero ? '\0' : '\n');
    }
}
