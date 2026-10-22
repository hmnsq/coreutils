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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <pwd.h>
#ଅନ୍ତର୍ଭାଵ <grp.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <selinux/selinux.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "mgetgroups.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "group-list.h"
#ଅନ୍ତର୍ଭାଵ "smack.h"
#ଅନ୍ତର୍ଭାଵ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "id"

#define AUTHORS \
  proper_name ("Arnold Robbins"), \
  proper_name ("David MacKenzie")

/* If nonzero, output only the SELinux context.  */
ସ୍ଥିର bool just_context = 0;
/* If true, delimit entries with NUL characters, not whitespace */
ସ୍ଥିର bool opt_zero = false;
/* If true, output the list of all group IDs. -G */
ସ୍ଥିର bool just_group_list = false;
/* If true, output only the group ID(s). -g */
ସ୍ଥିର bool just_group = false;
/* If true, output real UID/GID instead of default effective UID/GID. -r */
ସ୍ଥିର bool use_real = false;
/* If true, output only the user ID(s). -u */
ସ୍ଥିର bool just_user = false;
/* True unless errors have been encountered.  */
ସ୍ଥିର bool ok = true;
/* If true, we are using multiple users. Terminate -G with double NUL. */
ସ୍ଥିର bool multiple_users = false;
/* If true, output user/group name instead of ID number. -n */
ସ୍ଥିର bool use_name = false;

/* The real and effective IDs of the user to print. */
ସ୍ଥିର uid_t ruid, euid;
ସ୍ଥିର gid_t rgid, egid;

/* The SELinux context.  Start with a known invalid value so print_full_info
   knows when 'context' has not been set to a meaningful value.  */
ସ୍ଥିର ଅକ୍ଷର *context = ଖାଲି;

ସ୍ଥିର ଶୂନ୍ୟ print_user (uid_t uid);
ସ୍ଥିର ଶୂନ୍ୟ print_full_info (ଅକ୍ଷର ଅଚଳ *username);
ସ୍ଥିର ଶୂନ୍ୟ print_stuff (ଅକ୍ଷର ଅଚଳ *pw_name);

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {"context", no_argument, ଖାଲି, 'Z'},
  {"group", no_argument, ଖାଲି, 'g'},
  {"groups", no_argument, ଖାଲି, 'G'},
  {"name", no_argument, ଖାଲି, 'n'},
  {"real", no_argument, ଖାଲି, 'r'},
  {"user", no_argument, ଖାଲି, 'u'},
  {"zero", no_argument, ଖାଲି, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
Print user and group information ପାଇଁ each specified USER,\n\
or (when USER omitted) ପାଇଁ the current process.\n\
\n"),
             stdout);
      oputs (_("\
  -a\n\
         ignore, ପାଇଁ compatibility with other versions\n\
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
         print a name instead of a ସଂଖ୍ୟା, ପାଇଁ -u, -g, -G\n\
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
         not permitted in ପୂର୍ବନିର୍ଦ୍ଧାରିତ format\n\
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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା selinux_enabled = (is_selinux_enabled () > 0);
  bool smack_enabled = is_smack_enabled ();

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "agnruzGZ", longopts, ଖାଲି)) != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'a':
          /* Ignore -a, for compatibility with SVR4.  */
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'Z':
          /* politely decline if we're not on a SELinux/SMACK-enabled kernel.
             Keep require_selinux_ in tests in sync with these messages.  */
#ifdef HAVE_SMACK
          ଯଦି (!selinux_enabled && !smack_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on "
                     "an SELinux/SMACK-enabled kernel"));
#ଅନ୍ୟଥା
          ଯଦି (!selinux_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on an SELinux-enabled kernel"));
#endif
          just_context = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'g':
          just_group = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'n':
          use_name = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'r':
          use_real = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'u':
          just_user = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'z':
          opt_zero = true;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'G':
          just_group_list = true;
          ଭାଙ୍ଗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  size_t n_ids = argc - optind;

  ଯଦି (n_ids && just_context)
    error (EXIT_FAILURE, 0,
           _("cannot print security context when user specified"));

  ଯଦି (just_user + just_group + just_group_list + just_context > 1)
    error (EXIT_FAILURE, 0, _("cannot print \"only\" of more than one choice"));

  bool default_format = ! (just_user
                           || just_group
                           || just_group_list
                           || just_context);

  ଯଦି (default_format && (use_real || use_name))
    error (EXIT_FAILURE, 0,
           _("printing only names or real IDs requires -u, -g, or -G"));

  ଯଦି (default_format && opt_zero)
    error (EXIT_FAILURE, 0,
           _("option --zero not permitted in default format"));

  /* If we are on a SELinux/SMACK-enabled kernel, no user is specified, and
     either --context is specified or none of (-u,-g,-G) is specified,
     and we're not in POSIXLY_CORRECT mode, get our context.  Otherwise,
     leave the context variable alone - it has been initialized to an
     invalid value that will be not displayed in print_full_info().  */
  ଯଦି (n_ids == 0
      && (just_context
          || (default_format && ! getenv ("POSIXLY_CORRECT"))))
    {
      /* Report failure only if --context (-Z) was explicitly requested.  */
      ଯଦି ((selinux_enabled && getcon (&context) && just_context)
          || (smack_enabled
              && smack_new_label_from_self (&context) < 0
              && just_context))
        error (EXIT_FAILURE, 0, _("can't get process context"));
    }

  ଯଦି (n_ids >= 1)
    {
      multiple_users = n_ids > 1 ? true : false;
      /* Changing the value of n_ids to the last index in the array where we
         have the last possible user id. This helps us because we don't have
         to declare a different variable to keep a track of where the
         last username lies in argv[].  */
      n_ids += optind;
      /* For each username/userid to get its pw_name field */
      ପାଇଁ (; optind < n_ids; optind++)
        {
          ଅକ୍ଷର *pw_name = ଖାଲି;
          ସଂରଚନା passwd *pwd = ଖାଲି;
          ଅକ୍ଷର ଅଚଳ *spec = argv[optind];
          /* Disallow an empty spec here as parse_user_spec() doesn't
             give an error for that as it seems it's a valid way to
             specify a noop or "reset special bits" depending on the system.  */
          ଯଦି (*spec)
            {
              ଯଦି (! parse_user_spec (spec, &euid, ଖାଲି, &pw_name, ଖାଲି))
                pwd = pw_name ? getpwnam (pw_name) : getpwuid (euid);
            }
          ଯଦି (pwd == ଖାଲି)
            {
              error (0, errno, _("%s: no such user"), quote (spec));
              ok &= false;
            }
          ଅନ୍ୟଥା
            {
              ଯଦି (!pw_name)
                pw_name = xstrdup (pwd->pw_name);
              ruid = euid = pwd->pw_uid;
              rgid = egid = pwd->pw_gid;
              print_stuff (pw_name);
              ଯଦି (ferror (stdout))
                write_error ();
            }
          ମୁକ୍ତ (pw_name);
        }
    }
  ଅନ୍ୟଥା
    {
      /* POSIX says identification functions (getuid, getgid, and
         others) cannot fail, but they can fail under GNU/Hurd and a
         few other systems.  Test for failure by checking errno.  */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      ଯଦି (just_user ? !use_real
          : !just_group && !just_group_list && !just_context)
        {
          errno = 0;
          euid = geteuid ();
          ଯଦି (euid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective UID"));
        }

      ଯଦି (just_user ? use_real
          : !just_group && (just_group_list || !just_context))
        {
          errno = 0;
          ruid = getuid ();
          ଯଦି (ruid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real UID"));
        }

      ଯଦି (!just_user && (just_group || just_group_list || !just_context))
        {
          errno = 0;
          egid = getegid ();
          ଯଦି (egid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective GID"));

          errno = 0;
          rgid = getgid ();
          ଯଦି (rgid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real GID"));
        }
        print_stuff (ଖାଲି);
    }

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Print the name or value of user ID UID. */

ସ୍ଥିର ଶୂନ୍ୟ
print_user (uid_t uid)
{
  ସଂରଚନା passwd *pwd = ଖାଲି;

  ଯଦି (use_name)
    {
      pwd = getpwuid (uid);
      ଯଦି (pwd == ଖାଲି)
        {
          error (0, 0, _("cannot find name for user ID %ju"), (uintmax_t) uid);
          ok &= false;
        }
    }

  ଯଦି (pwd)
    printf ("%s", pwd->pw_name);
  ଅନ୍ୟଥା
    printf ("%ju", (uintmax_t) uid);
}

/* Print all of the info about the user's user and group IDs. */

ସ୍ଥିର ଶୂନ୍ୟ
print_full_info (ଅକ୍ଷର ଅଚଳ *username)
{
  ସଂରଚନା passwd *pwd;
  ସଂରଚନା group *grp;

  printf (_("uid=%ju"), (uintmax_t) ruid);
  pwd = getpwuid (ruid);
  ଯଦି (pwd)
    printf ("(%s)", pwd->pw_name);

  printf (_(" gid=%ju"), (uintmax_t) rgid);
  grp = getgrgid (rgid);
  ଯଦି (grp)
    printf ("(%s)", grp->gr_name);

  ଯଦି (euid != ruid)
    {
      printf (_(" euid=%ju"), (uintmax_t) euid);
      pwd = getpwuid (euid);
      ଯଦି (pwd)
        printf ("(%s)", pwd->pw_name);
    }

  ଯଦି (egid != rgid)
    {
      printf (_(" egid=%ju"), (uintmax_t) egid);
      grp = getgrgid (egid);
      ଯଦି (grp)
        printf ("(%s)", grp->gr_name);
    }

  {
    gid_t *groups;

    gid_t primary_group;
    ଯଦି (username)
      primary_group = pwd ? pwd->pw_gid : -1;
    ଅନ୍ୟଥା
      primary_group = egid;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_groups = xgetgroups (username, primary_group, &groups);
    ଯଦି (n_groups < 0)
      {
        ଯଦି (username)
          error (0, errno, _("failed to get groups for user %s"),
                 quote (username));
        ଅନ୍ୟଥା
          error (0, errno, _("failed to get groups for the current process"));
        ok &= false;
        ଫେରନ୍ତୁ;
      }

    ଯଦି (n_groups > 0)
      fputs (_(" groups="), stdout);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n_groups; i++)
      {
        ଯଦି (i > 0)
          putchar (',');
        printf ("%ju", (uintmax_t) groups[i]);
        grp = getgrgid (groups[i]);
        ଯଦି (grp)
          printf ("(%s)", grp->gr_name);
      }
    ମୁକ୍ତ (groups);
  }

  /* POSIX mandates the precise output format, and that it not include
     any context=... part, so skip that if POSIXLY_CORRECT is set.  */
  ଯଦି (context)
    printf (_(" context=%s"), context);
}

/* Print information about the user based on the arguments passed. */

ସ୍ଥିର ଶୂନ୍ୟ
print_stuff (ଅକ୍ଷର ଅଚଳ *pw_name)
{
  ଯଦି (just_user)
      print_user (use_real ? ruid : euid);

  /* print_group and print_group_list return true on successful
     execution but false if something goes wrong. We then AND this value with
     the current value of 'ok' because we want to know if one of the previous
     users faced a problem in these functions. This value of 'ok' is later used
     to understand what status program should exit with. */
  ଅନ୍ୟଥା ଯଦି (just_group)
    ok &= print_group (use_real ? rgid : egid, use_name);
  ଅନ୍ୟଥା ଯଦି (just_group_list)
    ok &= print_group_list (pw_name, ruid, rgid, egid,
                            use_name, opt_zero ? '\0' : ' ');
  ଅନ୍ୟଥା ଯଦି (just_context)
    fputs (context, stdout);
  ଅନ୍ୟଥା
    print_full_info (pw_name);

  /* When printing records for more than 1 user, at the end of groups
     of each user terminate the record with two consequent NUL characters
     to make parsing and distinguishing between two records possible. */
  ଯଦି (opt_zero && just_group_list && multiple_users)
    {
      putchar ('\0');
      putchar ('\0');
    }
  ଅନ୍ୟଥା
    {
      putchar (opt_zero ? '\0' : '\n');
    }
}
