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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <pwd.h>
#ಅನ್ತರ್ಭಾವ <grp.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <selinux/selinux.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "mgetgroups.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "group-list.h"
#ಅನ್ತರ್ಭಾವ "smack.h"
#ಅನ್ತರ್ಭಾವ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "id"

#define AUTHORS \
  proper_name ("Arnold Robbins"), \
  proper_name ("David MacKenzie")

/* If nonzero, output only the SELinux context.  */
ಸ್ಥಿರ bool just_context = 0;
/* If true, delimit entries with NUL characters, not whitespace */
ಸ್ಥಿರ bool opt_zero = false;
/* If true, output the list of all group IDs. -G */
ಸ್ಥಿರ bool just_group_list = false;
/* If true, output only the group ID(s). -g */
ಸ್ಥಿರ bool just_group = false;
/* If true, output real UID/GID instead of default effective UID/GID. -r */
ಸ್ಥಿರ bool use_real = false;
/* If true, output only the user ID(s). -u */
ಸ್ಥಿರ bool just_user = false;
/* True unless errors have been encountered.  */
ಸ್ಥಿರ bool ok = true;
/* If true, we are using multiple users. Terminate -G with double NUL. */
ಸ್ಥಿರ bool multiple_users = false;
/* If true, output user/group name instead of ID number. -n */
ಸ್ಥಿರ bool use_name = false;

/* The real and effective IDs of the user to print. */
ಸ್ಥಿರ uid_t ruid, euid;
ಸ್ಥಿರ gid_t rgid, egid;

/* The SELinux context.  Start with a known invalid value so print_full_info
   knows when 'context' has not been set to a meaningful value.  */
ಸ್ಥಿರ ವರ್ಣ *context = ರಿಕ್ತ;

ಸ್ಥಿರ ಶೂನ್ಯ print_user (uid_t uid);
ಸ್ಥಿರ ಶೂನ್ಯ print_full_info (ವರ್ಣ ಧ್ರುವ *username);
ಸ್ಥಿರ ಶೂನ್ಯ print_stuff (ವರ್ಣ ಧ್ರುವ *pw_name);

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {"context", no_argument, ರಿಕ್ತ, 'Z'},
  {"group", no_argument, ರಿಕ್ತ, 'g'},
  {"groups", no_argument, ರಿಕ್ತ, 'G'},
  {"name", no_argument, ರಿಕ್ತ, 'n'},
  {"real", no_argument, ರಿಕ್ತ, 'r'},
  {"user", no_argument, ರಿಕ್ತ, 'u'},
  {"zero", no_argument, ರಿಕ್ತ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
Print user and group information ಪ್ರತಿ each specified USER,\n\
or (when USER omitted) ಪ್ರತಿ the current process.\n\
\n"),
             stdout);
      oputs (_("\
  -a\n\
         ignore, ಪ್ರತಿ compatibility with other versions\n\
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
         print a name instead of a ಸಂಖ್ಯೆ, ಪ್ರತಿ -u, -g, -G\n\
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
         not permitted in ಸ್ವಭಾವ format\n\
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

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ optc;
  ಪೂರ್ಣಾಙ್ಕ selinux_enabled = (is_selinux_enabled () > 0);
  bool smack_enabled = is_smack_enabled ();

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, "agnruzGZ", longopts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'a':
          /* Ignore -a, for compatibility with SVR4.  */
          ಭಙ್ಗ;

        ಪಕ್ಷ 'Z':
          /* politely decline if we're not on a SELinux/SMACK-enabled kernel.
             Keep require_selinux_ in tests in sync with these messages.  */
#ifdef HAVE_SMACK
          ಯದಿ (!selinux_enabled && !smack_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on "
                     "an SELinux/SMACK-enabled kernel"));
#ಅನ್ಯಥಾ
          ಯದಿ (!selinux_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on an SELinux-enabled kernel"));
#endif
          just_context = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'g':
          just_group = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'n':
          use_name = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'r':
          use_real = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'u':
          just_user = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'z':
          opt_zero = true;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'G':
          just_group_list = true;
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  size_t n_ids = argc - optind;

  ಯದಿ (n_ids && just_context)
    error (EXIT_FAILURE, 0,
           _("cannot print security context when user specified"));

  ಯದಿ (just_user + just_group + just_group_list + just_context > 1)
    error (EXIT_FAILURE, 0, _("cannot print \"only\" of more than one choice"));

  bool default_format = ! (just_user
                           || just_group
                           || just_group_list
                           || just_context);

  ಯದಿ (default_format && (use_real || use_name))
    error (EXIT_FAILURE, 0,
           _("printing only names or real IDs requires -u, -g, or -G"));

  ಯದಿ (default_format && opt_zero)
    error (EXIT_FAILURE, 0,
           _("option --zero not permitted in default format"));

  /* If we are on a SELinux/SMACK-enabled kernel, no user is specified, and
     either --context is specified or none of (-u,-g,-G) is specified,
     and we're not in POSIXLY_CORRECT mode, get our context.  Otherwise,
     leave the context variable alone - it has been initialized to an
     invalid value that will be not displayed in print_full_info().  */
  ಯದಿ (n_ids == 0
      && (just_context
          || (default_format && ! getenv ("POSIXLY_CORRECT"))))
    {
      /* Report failure only if --context (-Z) was explicitly requested.  */
      ಯದಿ ((selinux_enabled && getcon (&context) && just_context)
          || (smack_enabled
              && smack_new_label_from_self (&context) < 0
              && just_context))
        error (EXIT_FAILURE, 0, _("can't get process context"));
    }

  ಯದಿ (n_ids >= 1)
    {
      multiple_users = n_ids > 1 ? true : false;
      /* Changing the value of n_ids to the last index in the array where we
         have the last possible user id. This helps us because we don't have
         to declare a different variable to keep a track of where the
         last username lies in argv[].  */
      n_ids += optind;
      /* For each username/userid to get its pw_name field */
      ಪ್ರತಿ (; optind < n_ids; optind++)
        {
          ವರ್ಣ *pw_name = ರಿಕ್ತ;
          ಸಂರಚನಾ passwd *pwd = ರಿಕ್ತ;
          ವರ್ಣ ಧ್ರುವ *spec = argv[optind];
          /* Disallow an empty spec here as parse_user_spec() doesn't
             give an error for that as it seems it's a valid way to
             specify a noop or "reset special bits" depending on the system.  */
          ಯದಿ (*spec)
            {
              ಯದಿ (! parse_user_spec (spec, &euid, ರಿಕ್ತ, &pw_name, ರಿಕ್ತ))
                pwd = pw_name ? getpwnam (pw_name) : getpwuid (euid);
            }
          ಯದಿ (pwd == ರಿಕ್ತ)
            {
              error (0, errno, _("%s: no such user"), quote (spec));
              ok &= false;
            }
          ಅನ್ಯಥಾ
            {
              ಯದಿ (!pw_name)
                pw_name = xstrdup (pwd->pw_name);
              ruid = euid = pwd->pw_uid;
              rgid = egid = pwd->pw_gid;
              print_stuff (pw_name);
              ಯದಿ (ferror (stdout))
                write_error ();
            }
          ಮುಕ್ತ (pw_name);
        }
    }
  ಅನ್ಯಥಾ
    {
      /* POSIX says identification functions (getuid, getgid, and
         others) cannot fail, but they can fail under GNU/Hurd and a
         few other systems.  Test for failure by checking errno.  */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      ಯದಿ (just_user ? !use_real
          : !just_group && !just_group_list && !just_context)
        {
          errno = 0;
          euid = geteuid ();
          ಯದಿ (euid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective UID"));
        }

      ಯದಿ (just_user ? use_real
          : !just_group && (just_group_list || !just_context))
        {
          errno = 0;
          ruid = getuid ();
          ಯದಿ (ruid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real UID"));
        }

      ಯದಿ (!just_user && (just_group || just_group_list || !just_context))
        {
          errno = 0;
          egid = getegid ();
          ಯದಿ (egid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective GID"));

          errno = 0;
          rgid = getgid ();
          ಯದಿ (rgid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real GID"));
        }
        print_stuff (ರಿಕ್ತ);
    }

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Print the name or value of user ID UID. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_user (uid_t uid)
{
  ಸಂರಚನಾ passwd *pwd = ರಿಕ್ತ;

  ಯದಿ (use_name)
    {
      pwd = getpwuid (uid);
      ಯದಿ (pwd == ರಿಕ್ತ)
        {
          error (0, 0, _("cannot find name for user ID %ju"), (uintmax_t) uid);
          ok &= false;
        }
    }

  ಯದಿ (pwd)
    printf ("%s", pwd->pw_name);
  ಅನ್ಯಥಾ
    printf ("%ju", (uintmax_t) uid);
}

/* Print all of the info about the user's user and group IDs. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_full_info (ವರ್ಣ ಧ್ರುವ *username)
{
  ಸಂರಚನಾ passwd *pwd;
  ಸಂರಚನಾ group *grp;

  printf (_("uid=%ju"), (uintmax_t) ruid);
  pwd = getpwuid (ruid);
  ಯದಿ (pwd)
    printf ("(%s)", pwd->pw_name);

  printf (_(" gid=%ju"), (uintmax_t) rgid);
  grp = getgrgid (rgid);
  ಯದಿ (grp)
    printf ("(%s)", grp->gr_name);

  ಯದಿ (euid != ruid)
    {
      printf (_(" euid=%ju"), (uintmax_t) euid);
      pwd = getpwuid (euid);
      ಯದಿ (pwd)
        printf ("(%s)", pwd->pw_name);
    }

  ಯದಿ (egid != rgid)
    {
      printf (_(" egid=%ju"), (uintmax_t) egid);
      grp = getgrgid (egid);
      ಯದಿ (grp)
        printf ("(%s)", grp->gr_name);
    }

  {
    gid_t *groups;

    gid_t primary_group;
    ಯದಿ (username)
      primary_group = pwd ? pwd->pw_gid : -1;
    ಅನ್ಯಥಾ
      primary_group = egid;

    ಪೂರ್ಣಾಙ್ಕ n_groups = xgetgroups (username, primary_group, &groups);
    ಯದಿ (n_groups < 0)
      {
        ಯದಿ (username)
          error (0, errno, _("failed to get groups for user %s"),
                 quote (username));
        ಅನ್ಯಥಾ
          error (0, errno, _("failed to get groups for the current process"));
        ok &= false;
        ಪ್ರತ್ಯಾಗಮ;
      }

    ಯದಿ (n_groups > 0)
      fputs (_(" groups="), stdout);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n_groups; i++)
      {
        ಯದಿ (i > 0)
          putchar (',');
        printf ("%ju", (uintmax_t) groups[i]);
        grp = getgrgid (groups[i]);
        ಯದಿ (grp)
          printf ("(%s)", grp->gr_name);
      }
    ಮುಕ್ತ (groups);
  }

  /* POSIX mandates the precise output format, and that it not include
     any context=... part, so skip that if POSIXLY_CORRECT is set.  */
  ಯದಿ (context)
    printf (_(" context=%s"), context);
}

/* Print information about the user based on the arguments passed. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_stuff (ವರ್ಣ ಧ್ರುವ *pw_name)
{
  ಯದಿ (just_user)
      print_user (use_real ? ruid : euid);

  /* print_group and print_group_list return true on successful
     execution but false if something goes wrong. We then AND this value with
     the current value of 'ok' because we want to know if one of the previous
     users faced a problem in these functions. This value of 'ok' is later used
     to understand what status program should exit with. */
  ಅನ್ಯಥಾ ಯದಿ (just_group)
    ok &= print_group (use_real ? rgid : egid, use_name);
  ಅನ್ಯಥಾ ಯದಿ (just_group_list)
    ok &= print_group_list (pw_name, ruid, rgid, egid,
                            use_name, opt_zero ? '\0' : ' ');
  ಅನ್ಯಥಾ ಯದಿ (just_context)
    fputs (context, stdout);
  ಅನ್ಯಥಾ
    print_full_info (pw_name);

  /* When printing records for more than 1 user, at the end of groups
     of each user terminate the record with two consequent NUL characters
     to make parsing and distinguishing between two records possible. */
  ಯದಿ (opt_zero && just_group_list && multiple_users)
    {
      putchar ('\0');
      putchar ('\0');
    }
  ಅನ್ಯಥಾ
    {
      putchar (opt_zero ? '\0' : '\n');
    }
}
