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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <pwd.h>
#അന്തര്ഭാവ <grp.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <selinux/selinux.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "mgetgroups.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "group-list.h"
#അന്തര്ഭാവ "smack.h"
#അന്തര്ഭാവ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "id"

#define AUTHORS \
  proper_name ("Arnold Robbins"), \
  proper_name ("David MacKenzie")

/* If nonzero, output only the SELinux context.  */
സ്ഥിതം bool just_context = 0;
/* If true, delimit entries with NUL characters, not whitespace */
സ്ഥിതം bool opt_zero = false;
/* If true, output the list of all group IDs. -G */
സ്ഥിതം bool just_group_list = false;
/* If true, output only the group ID(s). -g */
സ്ഥിതം bool just_group = false;
/* If true, output real UID/GID instead of default effective UID/GID. -r */
സ്ഥിതം bool use_real = false;
/* If true, output only the user ID(s). -u */
സ്ഥിതം bool just_user = false;
/* True unless errors have been encountered.  */
സ്ഥിതം bool ok = true;
/* If true, we are using multiple users. Terminate -G with double NUL. */
സ്ഥിതം bool multiple_users = false;
/* If true, output user/group name instead of ID number. -n */
സ്ഥിതം bool use_name = false;

/* The real and effective IDs of the user to print. */
സ്ഥിതം uid_t ruid, euid;
സ്ഥിതം gid_t rgid, egid;

/* The SELinux context.  Start with a known invalid value so print_full_info
   knows when 'context' has not been set to a meaningful value.  */
സ്ഥിതം അക്ഷരം *context = ശൂന്യ;

സ്ഥിതം ശൂന്യം print_user (uid_t uid);
സ്ഥിതം ശൂന്യം print_full_info (അക്ഷരം സ്ഥിരം *username);
സ്ഥിതം ശൂന്യം print_stuff (അക്ഷരം സ്ഥിരം *pw_name);

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"context", no_argument, ശൂന്യ, 'Z'},
  {"group", no_argument, ശൂന്യ, 'g'},
  {"groups", no_argument, ശൂന്യ, 'G'},
  {"name", no_argument, ശൂന്യ, 'n'},
  {"real", no_argument, ശൂന്യ, 'r'},
  {"user", no_argument, ശൂന്യ, 'u'},
  {"zero", no_argument, ശൂന്യ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
Print user and group information വേണ്ടി each specified USER,\n\
or (when USER omitted) വേണ്ടി the current process.\n\
\n"),
             stdout);
      oputs (_("\
  -a\n\
         ignore, വേണ്ടി compatibility with other versions\n\
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
         print a name instead of a സംഖ്യ, വേണ്ടി -u, -g, -G\n\
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
         not permitted in സ്ഥിരസ്ഥിതി format\n\
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ optc;
  പൂർണ്ണസംഖ്യ selinux_enabled = (is_selinux_enabled () > 0);
  bool smack_enabled = is_smack_enabled ();

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "agnruzGZ", longopts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'a':
          /* Ignore -a, for compatibility with SVR4.  */
          നിർത്തുക;

        അവസ്ഥ 'Z':
          /* politely decline if we're not on a SELinux/SMACK-enabled kernel.
             Keep require_selinux_ in tests in sync with these messages.  */
#ifdef HAVE_SMACK
          എങ്കിൽ (!selinux_enabled && !smack_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on "
                     "an SELinux/SMACK-enabled kernel"));
#അല്ലെങ്കിൽ
          എങ്കിൽ (!selinux_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on an SELinux-enabled kernel"));
#endif
          just_context = true;
          നിർത്തുക;

        അവസ്ഥ 'g':
          just_group = true;
          നിർത്തുക;
        അവസ്ഥ 'n':
          use_name = true;
          നിർത്തുക;
        അവസ്ഥ 'r':
          use_real = true;
          നിർത്തുക;
        അവസ്ഥ 'u':
          just_user = true;
          നിർത്തുക;
        അവസ്ഥ 'z':
          opt_zero = true;
          നിർത്തുക;
        അവസ്ഥ 'G':
          just_group_list = true;
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  size_t n_ids = argc - optind;

  എങ്കിൽ (n_ids && just_context)
    error (EXIT_FAILURE, 0,
           _("cannot print security context when user specified"));

  എങ്കിൽ (just_user + just_group + just_group_list + just_context > 1)
    error (EXIT_FAILURE, 0, _("cannot print \"only\" of more than one choice"));

  bool default_format = ! (just_user
                           || just_group
                           || just_group_list
                           || just_context);

  എങ്കിൽ (default_format && (use_real || use_name))
    error (EXIT_FAILURE, 0,
           _("printing only names or real IDs requires -u, -g, or -G"));

  എങ്കിൽ (default_format && opt_zero)
    error (EXIT_FAILURE, 0,
           _("option --zero not permitted in default format"));

  /* If we are on a SELinux/SMACK-enabled kernel, no user is specified, and
     either --context is specified or none of (-u,-g,-G) is specified,
     and we're not in POSIXLY_CORRECT mode, get our context.  Otherwise,
     leave the context variable alone - it has been initialized to an
     invalid value that will be not displayed in print_full_info().  */
  എങ്കിൽ (n_ids == 0
      && (just_context
          || (default_format && ! getenv ("POSIXLY_CORRECT"))))
    {
      /* Report failure only if --context (-Z) was explicitly requested.  */
      എങ്കിൽ ((selinux_enabled && getcon (&context) && just_context)
          || (smack_enabled
              && smack_new_label_from_self (&context) < 0
              && just_context))
        error (EXIT_FAILURE, 0, _("can't get process context"));
    }

  എങ്കിൽ (n_ids >= 1)
    {
      multiple_users = n_ids > 1 ? true : false;
      /* Changing the value of n_ids to the last index in the array where we
         have the last possible user id. This helps us because we don't have
         to declare a different variable to keep a track of where the
         last username lies in argv[].  */
      n_ids += optind;
      /* For each username/userid to get its pw_name field */
      വേണ്ടി (; optind < n_ids; optind++)
        {
          അക്ഷരം *pw_name = ശൂന്യ;
          ഘടന passwd *pwd = ശൂന്യ;
          അക്ഷരം സ്ഥിരം *spec = argv[optind];
          /* Disallow an empty spec here as parse_user_spec() doesn't
             give an error for that as it seems it's a valid way to
             specify a noop or "reset special bits" depending on the system.  */
          എങ്കിൽ (*spec)
            {
              എങ്കിൽ (! parse_user_spec (spec, &euid, ശൂന്യ, &pw_name, ശൂന്യ))
                pwd = pw_name ? getpwnam (pw_name) : getpwuid (euid);
            }
          എങ്കിൽ (pwd == ശൂന്യ)
            {
              error (0, errno, _("%s: no such user"), quote (spec));
              ok &= false;
            }
          അല്ലെങ്കിൽ
            {
              എങ്കിൽ (!pw_name)
                pw_name = xstrdup (pwd->pw_name);
              ruid = euid = pwd->pw_uid;
              rgid = egid = pwd->pw_gid;
              print_stuff (pw_name);
              എങ്കിൽ (ferror (stdout))
                write_error ();
            }
          സ്വതന്ത്രം (pw_name);
        }
    }
  അല്ലെങ്കിൽ
    {
      /* POSIX says identification functions (getuid, getgid, and
         others) cannot fail, but they can fail under GNU/Hurd and a
         few other systems.  Test for failure by checking errno.  */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      എങ്കിൽ (just_user ? !use_real
          : !just_group && !just_group_list && !just_context)
        {
          errno = 0;
          euid = geteuid ();
          എങ്കിൽ (euid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective UID"));
        }

      എങ്കിൽ (just_user ? use_real
          : !just_group && (just_group_list || !just_context))
        {
          errno = 0;
          ruid = getuid ();
          എങ്കിൽ (ruid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real UID"));
        }

      എങ്കിൽ (!just_user && (just_group || just_group_list || !just_context))
        {
          errno = 0;
          egid = getegid ();
          എങ്കിൽ (egid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective GID"));

          errno = 0;
          rgid = getgid ();
          എങ്കിൽ (rgid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real GID"));
        }
        print_stuff (ശൂന്യ);
    }

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Print the name or value of user ID UID. */

സ്ഥിതം ശൂന്യം
print_user (uid_t uid)
{
  ഘടന passwd *pwd = ശൂന്യ;

  എങ്കിൽ (use_name)
    {
      pwd = getpwuid (uid);
      എങ്കിൽ (pwd == ശൂന്യ)
        {
          error (0, 0, _("cannot find name for user ID %ju"), (uintmax_t) uid);
          ok &= false;
        }
    }

  എങ്കിൽ (pwd)
    printf ("%s", pwd->pw_name);
  അല്ലെങ്കിൽ
    printf ("%ju", (uintmax_t) uid);
}

/* Print all of the info about the user's user and group IDs. */

സ്ഥിതം ശൂന്യം
print_full_info (അക്ഷരം സ്ഥിരം *username)
{
  ഘടന passwd *pwd;
  ഘടന group *grp;

  printf (_("uid=%ju"), (uintmax_t) ruid);
  pwd = getpwuid (ruid);
  എങ്കിൽ (pwd)
    printf ("(%s)", pwd->pw_name);

  printf (_(" gid=%ju"), (uintmax_t) rgid);
  grp = getgrgid (rgid);
  എങ്കിൽ (grp)
    printf ("(%s)", grp->gr_name);

  എങ്കിൽ (euid != ruid)
    {
      printf (_(" euid=%ju"), (uintmax_t) euid);
      pwd = getpwuid (euid);
      എങ്കിൽ (pwd)
        printf ("(%s)", pwd->pw_name);
    }

  എങ്കിൽ (egid != rgid)
    {
      printf (_(" egid=%ju"), (uintmax_t) egid);
      grp = getgrgid (egid);
      എങ്കിൽ (grp)
        printf ("(%s)", grp->gr_name);
    }

  {
    gid_t *groups;

    gid_t primary_group;
    എങ്കിൽ (username)
      primary_group = pwd ? pwd->pw_gid : -1;
    അല്ലെങ്കിൽ
      primary_group = egid;

    പൂർണ്ണസംഖ്യ n_groups = xgetgroups (username, primary_group, &groups);
    എങ്കിൽ (n_groups < 0)
      {
        എങ്കിൽ (username)
          error (0, errno, _("failed to get groups for user %s"),
                 quote (username));
        അല്ലെങ്കിൽ
          error (0, errno, _("failed to get groups for the current process"));
        ok &= false;
        തിരികെ;
      }

    എങ്കിൽ (n_groups > 0)
      fputs (_(" groups="), stdout);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n_groups; i++)
      {
        എങ്കിൽ (i > 0)
          putchar (',');
        printf ("%ju", (uintmax_t) groups[i]);
        grp = getgrgid (groups[i]);
        എങ്കിൽ (grp)
          printf ("(%s)", grp->gr_name);
      }
    സ്വതന്ത്രം (groups);
  }

  /* POSIX mandates the precise output format, and that it not include
     any context=... part, so skip that if POSIXLY_CORRECT is set.  */
  എങ്കിൽ (context)
    printf (_(" context=%s"), context);
}

/* Print information about the user based on the arguments passed. */

സ്ഥിതം ശൂന്യം
print_stuff (അക്ഷരം സ്ഥിരം *pw_name)
{
  എങ്കിൽ (just_user)
      print_user (use_real ? ruid : euid);

  /* print_group and print_group_list return true on successful
     execution but false if something goes wrong. We then AND this value with
     the current value of 'ok' because we want to know if one of the previous
     users faced a problem in these functions. This value of 'ok' is later used
     to understand what status program should exit with. */
  അല്ലെങ്കിൽ എങ്കിൽ (just_group)
    ok &= print_group (use_real ? rgid : egid, use_name);
  അല്ലെങ്കിൽ എങ്കിൽ (just_group_list)
    ok &= print_group_list (pw_name, ruid, rgid, egid,
                            use_name, opt_zero ? '\0' : ' ');
  അല്ലെങ്കിൽ എങ്കിൽ (just_context)
    fputs (context, stdout);
  അല്ലെങ്കിൽ
    print_full_info (pw_name);

  /* When printing records for more than 1 user, at the end of groups
     of each user terminate the record with two consequent NUL characters
     to make parsing and distinguishing between two records possible. */
  എങ്കിൽ (opt_zero && just_group_list && multiple_users)
    {
      putchar ('\0');
      putchar ('\0');
    }
  അല്ലെങ്കിൽ
    {
      putchar (opt_zero ? '\0' : '\n');
    }
}
