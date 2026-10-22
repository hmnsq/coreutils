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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <pwd.h>
#অন্তর্ভাৱ <grp.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <selinux/selinux.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "mgetgroups.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "group-list.h"
#অন্তর্ভাৱ "smack.h"
#অন্তর্ভাৱ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "id"

#define AUTHORS \
  proper_name ("Arnold Robbins"), \
  proper_name ("David MacKenzie")

/* If nonzero, output only the SELinux context.  */
স্থির bool just_context = 0;
/* If true, delimit entries with NUL characters, not whitespace */
স্থির bool opt_zero = false;
/* If true, output the list of all group IDs. -G */
স্থির bool just_group_list = false;
/* If true, output only the group ID(s). -g */
স্থির bool just_group = false;
/* If true, output real UID/GID instead of default effective UID/GID. -r */
স্থির bool use_real = false;
/* If true, output only the user ID(s). -u */
স্থির bool just_user = false;
/* True unless errors have been encountered.  */
স্থির bool ok = true;
/* If true, we are using multiple users. Terminate -G with double NUL. */
স্থির bool multiple_users = false;
/* If true, output user/group name instead of ID number. -n */
স্থির bool use_name = false;

/* The real and effective IDs of the user to print. */
স্থির uid_t ruid, euid;
স্থির gid_t rgid, egid;

/* The SELinux context.  Start with a known invalid value so print_full_info
   knows when 'context' has not been set to a meaningful value.  */
স্থির ৱর্ণ *context = রিক্ত;

স্থির শূন্য print_user (uid_t uid);
স্থির শূন্য print_full_info (ৱর্ণ ধ্রুৱ *username);
স্থির শূন্য print_stuff (ৱর্ণ ধ্রুৱ *pw_name);

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {"context", no_argument, রিক্ত, 'Z'},
  {"group", no_argument, রিক্ত, 'g'},
  {"groups", no_argument, রিক্ত, 'G'},
  {"name", no_argument, রিক্ত, 'n'},
  {"real", no_argument, রিক্ত, 'r'},
  {"user", no_argument, রিক্ত, 'u'},
  {"zero", no_argument, রিক্ত, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
Print user and group information বাবে each specified USER,\n\
or (when USER omitted) বাবে the current process.\n\
\n"),
             stdout);
      oputs (_("\
  -a\n\
         ignore, বাবে compatibility with other versions\n\
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
         print a name instead of a সংখ্যা, বাবে -u, -g, -G\n\
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
         not permitted in স্ৱভাৱ format\n\
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

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  পূর্ণাঙ্ক optc;
  পূর্ণাঙ্ক selinux_enabled = (is_selinux_enabled () > 0);
  bool smack_enabled = is_smack_enabled ();

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যেতিয়ালৈ ((optc = getopt_long (argc, argv, "agnruzGZ", longopts, রিক্ত)) != -1)
    {
      ৱিকল্প (optc)
        {
        পক্ষ 'a':
          /* Ignore -a, for compatibility with SVR4.  */
          ভঙ্গ;

        পক্ষ 'Z':
          /* politely decline if we're not on a SELinux/SMACK-enabled kernel.
             Keep require_selinux_ in tests in sync with these messages.  */
#ifdef HAVE_SMACK
          যদি (!selinux_enabled && !smack_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on "
                     "an SELinux/SMACK-enabled kernel"));
#অন্যথা
          যদি (!selinux_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on an SELinux-enabled kernel"));
#endif
          just_context = true;
          ভঙ্গ;

        পক্ষ 'g':
          just_group = true;
          ভঙ্গ;
        পক্ষ 'n':
          use_name = true;
          ভঙ্গ;
        পক্ষ 'r':
          use_real = true;
          ভঙ্গ;
        পক্ষ 'u':
          just_user = true;
          ভঙ্গ;
        পক্ষ 'z':
          opt_zero = true;
          ভঙ্গ;
        পক্ষ 'G':
          just_group_list = true;
          ভঙ্গ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  size_t n_ids = argc - optind;

  যদি (n_ids && just_context)
    error (EXIT_FAILURE, 0,
           _("cannot print security context when user specified"));

  যদি (just_user + just_group + just_group_list + just_context > 1)
    error (EXIT_FAILURE, 0, _("cannot print \"only\" of more than one choice"));

  bool default_format = ! (just_user
                           || just_group
                           || just_group_list
                           || just_context);

  যদি (default_format && (use_real || use_name))
    error (EXIT_FAILURE, 0,
           _("printing only names or real IDs requires -u, -g, or -G"));

  যদি (default_format && opt_zero)
    error (EXIT_FAILURE, 0,
           _("option --zero not permitted in default format"));

  /* If we are on a SELinux/SMACK-enabled kernel, no user is specified, and
     either --context is specified or none of (-u,-g,-G) is specified,
     and we're not in POSIXLY_CORRECT mode, get our context.  Otherwise,
     leave the context variable alone - it has been initialized to an
     invalid value that will be not displayed in print_full_info().  */
  যদি (n_ids == 0
      && (just_context
          || (default_format && ! getenv ("POSIXLY_CORRECT"))))
    {
      /* Report failure only if --context (-Z) was explicitly requested.  */
      যদি ((selinux_enabled && getcon (&context) && just_context)
          || (smack_enabled
              && smack_new_label_from_self (&context) < 0
              && just_context))
        error (EXIT_FAILURE, 0, _("can't get process context"));
    }

  যদি (n_ids >= 1)
    {
      multiple_users = n_ids > 1 ? true : false;
      /* Changing the value of n_ids to the last index in the array where we
         have the last possible user id. This helps us because we don't have
         to declare a different variable to keep a track of where the
         last username lies in argv[].  */
      n_ids += optind;
      /* For each username/userid to get its pw_name field */
      বাবে (; optind < n_ids; optind++)
        {
          ৱর্ণ *pw_name = রিক্ত;
          সংরচনা passwd *pwd = রিক্ত;
          ৱর্ণ ধ্রুৱ *spec = argv[optind];
          /* Disallow an empty spec here as parse_user_spec() doesn't
             give an error for that as it seems it's a valid way to
             specify a noop or "reset special bits" depending on the system.  */
          যদি (*spec)
            {
              যদি (! parse_user_spec (spec, &euid, রিক্ত, &pw_name, রিক্ত))
                pwd = pw_name ? getpwnam (pw_name) : getpwuid (euid);
            }
          যদি (pwd == রিক্ত)
            {
              error (0, errno, _("%s: no such user"), quote (spec));
              ok &= false;
            }
          অন্যথা
            {
              যদি (!pw_name)
                pw_name = xstrdup (pwd->pw_name);
              ruid = euid = pwd->pw_uid;
              rgid = egid = pwd->pw_gid;
              print_stuff (pw_name);
              যদি (ferror (stdout))
                write_error ();
            }
          মুক্ত (pw_name);
        }
    }
  অন্যথা
    {
      /* POSIX says identification functions (getuid, getgid, and
         others) cannot fail, but they can fail under GNU/Hurd and a
         few other systems.  Test for failure by checking errno.  */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      যদি (just_user ? !use_real
          : !just_group && !just_group_list && !just_context)
        {
          errno = 0;
          euid = geteuid ();
          যদি (euid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective UID"));
        }

      যদি (just_user ? use_real
          : !just_group && (just_group_list || !just_context))
        {
          errno = 0;
          ruid = getuid ();
          যদি (ruid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real UID"));
        }

      যদি (!just_user && (just_group || just_group_list || !just_context))
        {
          errno = 0;
          egid = getegid ();
          যদি (egid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective GID"));

          errno = 0;
          rgid = getgid ();
          যদি (rgid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real GID"));
        }
        print_stuff (রিক্ত);
    }

  ঘূৰাই ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Print the name or value of user ID UID. */

স্থির শূন্য
print_user (uid_t uid)
{
  সংরচনা passwd *pwd = রিক্ত;

  যদি (use_name)
    {
      pwd = getpwuid (uid);
      যদি (pwd == রিক্ত)
        {
          error (0, 0, _("cannot find name for user ID %ju"), (uintmax_t) uid);
          ok &= false;
        }
    }

  যদি (pwd)
    printf ("%s", pwd->pw_name);
  অন্যথা
    printf ("%ju", (uintmax_t) uid);
}

/* Print all of the info about the user's user and group IDs. */

স্থির শূন্য
print_full_info (ৱর্ণ ধ্রুৱ *username)
{
  সংরচনা passwd *pwd;
  সংরচনা group *grp;

  printf (_("uid=%ju"), (uintmax_t) ruid);
  pwd = getpwuid (ruid);
  যদি (pwd)
    printf ("(%s)", pwd->pw_name);

  printf (_(" gid=%ju"), (uintmax_t) rgid);
  grp = getgrgid (rgid);
  যদি (grp)
    printf ("(%s)", grp->gr_name);

  যদি (euid != ruid)
    {
      printf (_(" euid=%ju"), (uintmax_t) euid);
      pwd = getpwuid (euid);
      যদি (pwd)
        printf ("(%s)", pwd->pw_name);
    }

  যদি (egid != rgid)
    {
      printf (_(" egid=%ju"), (uintmax_t) egid);
      grp = getgrgid (egid);
      যদি (grp)
        printf ("(%s)", grp->gr_name);
    }

  {
    gid_t *groups;

    gid_t primary_group;
    যদি (username)
      primary_group = pwd ? pwd->pw_gid : -1;
    অন্যথা
      primary_group = egid;

    পূর্ণাঙ্ক n_groups = xgetgroups (username, primary_group, &groups);
    যদি (n_groups < 0)
      {
        যদি (username)
          error (0, errno, _("failed to get groups for user %s"),
                 quote (username));
        অন্যথা
          error (0, errno, _("failed to get groups for the current process"));
        ok &= false;
        ঘূৰাই;
      }

    যদি (n_groups > 0)
      fputs (_(" groups="), stdout);
    বাবে (পূর্ণাঙ্ক i = 0; i < n_groups; i++)
      {
        যদি (i > 0)
          putchar (',');
        printf ("%ju", (uintmax_t) groups[i]);
        grp = getgrgid (groups[i]);
        যদি (grp)
          printf ("(%s)", grp->gr_name);
      }
    মুক্ত (groups);
  }

  /* POSIX mandates the precise output format, and that it not include
     any context=... part, so skip that if POSIXLY_CORRECT is set.  */
  যদি (context)
    printf (_(" context=%s"), context);
}

/* Print information about the user based on the arguments passed. */

স্থির শূন্য
print_stuff (ৱর্ণ ধ্রুৱ *pw_name)
{
  যদি (just_user)
      print_user (use_real ? ruid : euid);

  /* print_group and print_group_list return true on successful
     execution but false if something goes wrong. We then AND this value with
     the current value of 'ok' because we want to know if one of the previous
     users faced a problem in these functions. This value of 'ok' is later used
     to understand what status program should exit with. */
  অন্যথা যদি (just_group)
    ok &= print_group (use_real ? rgid : egid, use_name);
  অন্যথা যদি (just_group_list)
    ok &= print_group_list (pw_name, ruid, rgid, egid,
                            use_name, opt_zero ? '\0' : ' ');
  অন্যথা যদি (just_context)
    fputs (context, stdout);
  অন্যথা
    print_full_info (pw_name);

  /* When printing records for more than 1 user, at the end of groups
     of each user terminate the record with two consequent NUL characters
     to make parsing and distinguishing between two records possible. */
  যদি (opt_zero && just_group_list && multiple_users)
    {
      putchar ('\0');
      putchar ('\0');
    }
  অন্যথা
    {
      putchar (opt_zero ? '\0' : '\n');
    }
}
