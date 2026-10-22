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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "id"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Arnold Robbins"), \
  proper_name ("David MacKenzie")

/* If nonzero, output only the SELinux context.  */
ಸ್ಥಿರ bool just_context = 0;
/* If true, delimit entries with NUL characters, not whitespace */
ಸ್ಥಿರ bool opt_zero = false;
/* If true, output the list of all group IDs. -G */
ಸ್ಥಿರ bool just_ಗುಂಪು_list = false;
/* If true, output only the group ID(s). -g */
ಸ್ಥಿರ bool just_ಗುಂಪು = false;
/* If true, output real UID/GID instead of default effective UID/GID. -r */
ಸ್ಥಿರ bool use_real = false;
/* If true, output only the user ID(s). -u */
ಸ್ಥಿರ bool just_user = false;
/* True unless errors have been encountered.  */
ಸ್ಥಿರ bool ok = true;
/* If true, we are using multiple users. Terminate -G with double NUL. */
ಸ್ಥಿರ bool ಅಪವರ್ತ್ಯ_users = false;
/* If true, output user/group name instead of ID number. -n */
ಸ್ಥಿರ bool use_name = false;

/* The real and effective IDs of the user to print. */
ಸ್ಥಿರ uid_t ruid, euid;
ಸ್ಥಿರ gid_t rgid, egid;

/* The SELinux context.  Start with a known invalid value so print_full_info
   knows when 'context' has not been set to a meaningful value.  */
ಸ್ಥಿರ ಅಕ್ಷರ *context = ಖಾಲಿ;

ಸ್ಥಿರ ಶೂನ್ಯ print_user (uid_t uid);
ಸ್ಥಿರ ಶೂನ್ಯ print_ತುಂಬಿದ_info (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *username);
ಸ್ಥಿರ ಶೂನ್ಯ print_stuff (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *pw_name);

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"context", no_ವಾದ, ಖಾಲಿ, 'Z'},
  {"group", no_ವಾದ, ಖಾಲಿ, 'g'},
  {"groups", no_ವಾದ, ಖಾಲಿ, 'G'},
  {"name", no_ವಾದ, ಖಾಲಿ, 'n'},
  {"real", no_ವಾದ, ಖಾಲಿ, 'r'},
  {"user", no_ವಾದ, ಖಾಲಿ, 'u'},
  {"zero", no_ವಾದ, ಖಾಲಿ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... [USER]...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Print user and ಗುಂಪು information ಗಾಗಿ each specified USER,\n\
or (when USER omitted) ಗಾಗಿ the current process.\n\
\n"),
             stdout);
      oputs (_("\
  -a\n\
         ignore, ಗಾಗಿ compatibility with other versions\n\
"));
      oputs (_("\
  -Z, --context\n\
         print only the security context ದ the process\n\
"));
      oputs (_("\
  -g, --ಗುಂಪು\n\
         print only the effective ಗುಂಪು ID\n\
"));
      oputs (_("\
  -G, --groups\n\
         print all ಗುಂಪು IDs\n\
"));
      oputs (_("\
  -n, --name\n\
         print a name instead ದ a ಸಂಖ್ಯೆ, ಗಾಗಿ -u, -g, -G\n\
"));
      oputs (_("\
  -r, --real\n\
         print the real ID instead ದ the effective ID, with -u, -g, -G\n\
"));
      oputs (_("\
  -u, --user\n\
         print only the effective user ID\n\
"));
      oputs (_("\
  -z, --zero\n\
         delimit entries with NUL characters, not whitespace;\n\
         not permitted ಒಳ ಪೂರ್ವನಿಯೋಜಿತ format\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Without any OPTION, print some useful ಗಣ_2 ದ identified information.\n\
"), stdout);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ optc;
  ಪೂರ್ಣಾಂಕ selinux_enabled = (is_selinux_enabled () > 0);
  bool smack_enabled = is_smack_enabled ();

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((optc = getopt_long (argc, argv, "agnruzGZ", longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'a':
          /* Ignore -a, for compatibility with SVR4.  */
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'Z':
          /* politely decline if we're not on a SELinux/SMACK-enabled kernel.
             Keep require_selinux_ in tests in sync with these messages.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SMACK
          ಆದರೆ (!selinux_enabled && !smack_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on "
                     "an SELinux/SMACK-enabled kernel"));
#ಇಲ್ಲದಿದ್ದರೆ
          ಆದರೆ (!selinux_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on an SELinux-enabled kernel"));
#ಆದರೆ_ಅಂತ್ಯ
          just_context = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'g':
          just_ಗುಂಪು = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'n':
          use_name = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'r':
          use_real = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'u':
          just_user = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'z':
          opt_zero = true;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'G':
          just_ಗುಂಪು_list = true;
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  size_t n_ids = argc - optind;

  ಆದರೆ (n_ids && just_context)
    error (EXIT_FAILURE, 0,
           _("cannot print security context when user specified"));

  ಆದರೆ (just_user + just_ಗುಂಪು + just_ಗುಂಪು_list + just_context > 1)
    error (EXIT_FAILURE, 0, _("cannot print \"only\" of more than one choice"));

  bool default_format = ! (just_user
                           || just_ಗುಂಪು
                           || just_ಗುಂಪು_list
                           || just_context);

  ಆದರೆ (default_format && (use_real || use_name))
    error (EXIT_FAILURE, 0,
           _("printing only names or real IDs requires -u, -g, or -G"));

  ಆದರೆ (default_format && opt_zero)
    error (EXIT_FAILURE, 0,
           _("option --zero not permitted in default format"));

  /* If we are on a SELinux/SMACK-enabled kernel, no user is specified, and
     either --context is specified or none of (-u,-g,-G) is specified,
     and we're not in POSIXLY_CORRECT mode, get our context.  Otherwise,
     leave the context variable alone - it has been initialized to an
     invalid value that will be not displayed in print_full_info().  */
  ಆದರೆ (n_ids == 0
      && (just_context
          || (default_format && ! getenv ("POSIXLY_CORRECT"))))
    {
      /* Report failure only if --context (-Z) was explicitly requested.  */
      ಆದರೆ ((selinux_enabled && getcon (&context) && just_context)
          || (smack_enabled
              && smack_new_label_from_self (&context) < 0
              && just_context))
        error (EXIT_FAILURE, 0, _("can't get process context"));
    }

  ಆದರೆ (n_ids >= 1)
    {
      ಅಪವರ್ತ್ಯ_users = n_ids > 1 ? true : false;
      /* Changing the value of n_ids to the last index in the array where we
         have the last possible user id. This helps us because we don't have
         to declare a different variable to keep a track of where the
         last username lies in argv[].  */
      n_ids += optind;
      /* For each username/userid to get its pw_name field */
      ಗಾಗಿ (; optind < n_ids; optind++)
        {
          ಅಕ್ಷರ *pw_name = ಖಾಲಿ;
          ರಚನೆ passwd *pwd = ಖಾಲಿ;
          ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *spec = argv[optind];
          /* Disallow an empty spec here as parse_user_spec() doesn't
             give an error for that as it seems it's a valid way to
             specify a noop or "reset special bits" depending on the system.  */
          ಆದರೆ (*spec)
            {
              ಆದರೆ (! parse_user_spec (spec, &euid, ಖಾಲಿ, &pw_name, ಖಾಲಿ))
                pwd = pw_name ? getpwnam (pw_name) : getpwuid (euid);
            }
          ಆದರೆ (pwd == ಖಾಲಿ)
            {
              error (0, errno, _("%s: no such user"), quote (spec));
              ok &= false;
            }
          ಇಲ್ಲದಿದ್ದರೆ
            {
              ಆದರೆ (!pw_name)
                pw_name = xstrdup (pwd->pw_name);
              ruid = euid = pwd->pw_uid;
              rgid = egid = pwd->pw_gid;
              print_stuff (pw_name);
              ಆದರೆ (ferror (stdout))
                write_error ();
            }
          ಬಿಡುಗಡೆ (pw_name);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* POSIX says identification functions (getuid, getgid, and
         others) cannot fail, but they can fail under GNU/Hurd and a
         few other systems.  Test for failure by checking errno.  */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      ಆದರೆ (just_user ? !use_real
          : !just_ಗುಂಪು && !just_ಗುಂಪು_list && !just_context)
        {
          errno = 0;
          euid = geteuid ();
          ಆದರೆ (euid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective UID"));
        }

      ಆದರೆ (just_user ? use_real
          : !just_ಗುಂಪು && (just_ಗುಂಪು_list || !just_context))
        {
          errno = 0;
          ruid = getuid ();
          ಆದರೆ (ruid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real UID"));
        }

      ಆದರೆ (!just_user && (just_ಗುಂಪು || just_ಗುಂಪು_list || !just_context))
        {
          errno = 0;
          egid = getegid ();
          ಆದರೆ (egid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective GID"));

          errno = 0;
          rgid = getgid ();
          ಆದರೆ (rgid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real GID"));
        }
        print_stuff (ಖಾಲಿ);
    }

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Print the name or value of user ID UID. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_user (uid_t uid)
{
  ರಚನೆ passwd *pwd = ಖಾಲಿ;

  ಆದರೆ (use_name)
    {
      pwd = getpwuid (uid);
      ಆದರೆ (pwd == ಖಾಲಿ)
        {
          error (0, 0, _("cannot find name for user ID %ju"), (uintmax_t) uid);
          ok &= false;
        }
    }

  ಆದರೆ (pwd)
    ಬರೆ ("%s", pwd->pw_name);
  ಇಲ್ಲದಿದ್ದರೆ
    ಬರೆ ("%ju", (uintmax_t) uid);
}

/* Print all of the info about the user's user and group IDs. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_ತುಂಬಿದ_info (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *username)
{
  ರಚನೆ passwd *pwd;
  ರಚನೆ ಗುಂಪು *grp;

  ಬರೆ (_("uid=%ju"), (uintmax_t) ruid);
  pwd = getpwuid (ruid);
  ಆದರೆ (pwd)
    ಬರೆ ("(%s)", pwd->pw_name);

  ಬರೆ (_(" gid=%ju"), (uintmax_t) rgid);
  grp = getgrgid (rgid);
  ಆದರೆ (grp)
    ಬರೆ ("(%s)", grp->gr_name);

  ಆದರೆ (euid != ruid)
    {
      ಬರೆ (_(" euid=%ju"), (uintmax_t) euid);
      pwd = getpwuid (euid);
      ಆದರೆ (pwd)
        ಬರೆ ("(%s)", pwd->pw_name);
    }

  ಆದರೆ (egid != rgid)
    {
      ಬರೆ (_(" egid=%ju"), (uintmax_t) egid);
      grp = getgrgid (egid);
      ಆದರೆ (grp)
        ಬರೆ ("(%s)", grp->gr_name);
    }

  {
    gid_t *groups;

    gid_t primary_ಗುಂಪು;
    ಆದರೆ (username)
      primary_ಗುಂಪು = pwd ? pwd->pw_gid : -1;
    ಇಲ್ಲದಿದ್ದರೆ
      primary_ಗುಂಪು = egid;

    ಪೂರ್ಣಾಂಕ n_groups = xgetgroups (username, primary_ಗುಂಪು, &groups);
    ಆದರೆ (n_groups < 0)
      {
        ಆದರೆ (username)
          error (0, errno, _("failed to get groups for user %s"),
                 quote (username));
        ಇಲ್ಲದಿದ್ದರೆ
          error (0, errno, _("failed to get groups for the current process"));
        ok &= false;
        ಹಿಂತಿರುಗಿ;
      }

    ಆದರೆ (n_groups > 0)
      fputs (_(" groups="), stdout);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n_groups; i++)
      {
        ಆದರೆ (i > 0)
          ಅಕ್ಷರ_ಬರೆ (',');
        ಬರೆ ("%ju", (uintmax_t) groups[i]);
        grp = getgrgid (groups[i]);
        ಆದರೆ (grp)
          ಬರೆ ("(%s)", grp->gr_name);
      }
    ಬಿಡುಗಡೆ (groups);
  }

  /* POSIX mandates the precise output format, and that it not include
     any context=... part, so skip that if POSIXLY_CORRECT is set.  */
  ಆದರೆ (context)
    ಬರೆ (_(" context=%s"), context);
}

/* Print information about the user based on the arguments passed. */

ಸ್ಥಿರ ಶೂನ್ಯ
print_stuff (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *pw_name)
{
  ಆದರೆ (just_user)
      print_user (use_real ? ruid : euid);

  /* print_group and print_group_list return true on successful
     execution but false if something goes wrong. We then AND this value with
     the current value of 'ok' because we want to know if one of the previous
     users faced a problem in these functions. This value of 'ok' is later used
     to understand what status program should exit with. */
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (just_ಗುಂಪು)
    ok &= print_ಗುಂಪು (use_real ? rgid : egid, use_name);
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (just_ಗುಂಪು_list)
    ok &= print_ಗುಂಪು_list (pw_name, ruid, rgid, egid,
                            use_name, opt_zero ? '\0' : ' ');
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (just_context)
    fputs (context, stdout);
  ಇಲ್ಲದಿದ್ದರೆ
    print_ತುಂಬಿದ_info (pw_name);

  /* When printing records for more than 1 user, at the end of groups
     of each user terminate the record with two consequent NUL characters
     to make parsing and distinguishing between two records possible. */
  ಆದರೆ (opt_zero && just_ಗುಂಪು_list && ಅಪವರ್ತ್ಯ_users)
    {
      ಅಕ್ಷರ_ಬರೆ ('\0');
      ಅಕ್ಷರ_ಬರೆ ('\0');
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಅಕ್ಷರ_ಬರೆ (opt_zero ? '\0' : '\n');
    }
}
