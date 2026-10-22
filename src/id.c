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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#ضمن <pwd.h>
#ضمن <grp.h>
#ضمن <getopt.h>
#ضمن <selinux/selinux.h>

#ضمن "system.h"
#ضمن "mgetgroups.h"
#ضمن "quote.h"
#ضمن "group-list.h"
#ضمن "smack.h"
#ضمن "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "id"

#define AUTHORS \
  proper_name ("Arnold Robbins"), \
  proper_name ("David MacKenzie")

/* If nonzero, output only the SELinux context.  */
ساكن bool just_context = 0;
/* If true, delimit entries with NUL characters, not whitespace */
ساكن bool opt_zero = false;
/* If true, output the list of all group IDs. -G */
ساكن bool just_group_list = false;
/* If true, output only the group ID(s). -g */
ساكن bool just_group = false;
/* If true, output real UID/GID instead of default effective UID/GID. -r */
ساكن bool use_real = false;
/* If true, output only the user ID(s). -u */
ساكن bool just_user = false;
/* True unless errors have been encountered.  */
ساكن bool ok = true;
/* If true, we are using multiple users. Terminate -G with double NUL. */
ساكن bool multiple_users = false;
/* If true, output user/group name instead of ID number. -n */
ساكن bool use_name = false;

/* The real and effective IDs of the user to print. */
ساكن uid_t ruid, euid;
ساكن gid_t rgid, egid;

/* The SELinux context.  Start with a known invalid value so print_full_info
   knows when 'context' has not been set to a meaningful value.  */
ساكن حرف *context = عدم;

ساكن فراغ بغ_user (uid_t uid);
ساكن فراغ بغ_full_info (حرف ثابت *username);
ساكن فراغ بغ_stuff (حرف ثابت *pw_name);

ساكن بنية option ثابت longopts[] =
{
  {"context", no_argument, عدم, 'Z'},
  {"group", no_argument, عدم, 'g'},
  {"groups", no_argument, عدم, 'G'},
  {"name", no_argument, عدم, 'n'},
  {"real", no_argument, عدم, 'r'},
  {"user", no_argument, عدم, 'u'},
  {"zero", no_argument, عدم, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
بغ user and group information لكل each specified USER,\n\
or (when USER omitted) لكل the current process.\n\
\n"),
             stdout);
      oputs (_("\
  -a\n\
         ignore, لكل compatibility with other versions\n\
"));
      oputs (_("\
  -Z, --context\n\
         بغ_2 only the security context of the process\n\
"));
      oputs (_("\
  -g, --group\n\
         بغ_2 only the effective group ID\n\
"));
      oputs (_("\
  -G, --groups\n\
         بغ_2 all group IDs\n\
"));
      oputs (_("\
  -n, --name\n\
         بغ_2 a name instead of a لمب_2, لكل -u, -g, -G\n\
"));
      oputs (_("\
  -r, --real\n\
         بغ_2 the real ID instead of the effective ID, with -u, -g, -G\n\
"));
      oputs (_("\
  -u, --user\n\
         بغ_2 only the effective user ID\n\
"));
      oputs (_("\
  -z, --zero\n\
         delimit entries with NUL characters, not whitespace;\n\
         not permitted in افتراضي format\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Without any OPTION, بغ_2 some useful set of identified information.\n\
"), stdout);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحيح
main (صحيح argc, حرف **argv)
{
  صحيح optc;
  صحيح selinux_enabled = (is_selinux_enabled () > 0);
  bool smack_enabled = is_smack_enabled ();

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  طالما ((optc = getopt_long (argc, argv, "agnruzGZ", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'a':
          /* Ignore -a, for compatibility with SVR4.  */
          اكسر;

        حالة 'Z':
          /* politely decline if we're not on a SELinux/SMACK-enabled kernel.
             Keep require_selinux_ in tests in sync with these messages.  */
#ifdef HAVE_SMACK
          إذا (!selinux_enabled && !smack_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on "
                     "an SELinux/SMACK-enabled kernel"));
#وإلا
          إذا (!selinux_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on an SELinux-enabled kernel"));
#endif
          just_context = true;
          اكسر;

        حالة 'g':
          just_group = true;
          اكسر;
        حالة 'n':
          use_name = true;
          اكسر;
        حالة 'r':
          use_real = true;
          اكسر;
        حالة 'u':
          just_user = true;
          اكسر;
        حالة 'z':
          opt_zero = true;
          اكسر;
        حالة 'G':
          just_group_list = true;
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  size_t n_ids = argc - optind;

  إذا (n_ids && just_context)
    error (EXIT_FAILURE, 0,
           _("cannot print security context when user specified"));

  إذا (just_user + just_group + just_group_list + just_context > 1)
    error (EXIT_FAILURE, 0, _("cannot print \"only\" of more than one choice"));

  bool default_format = ! (just_user
                           || just_group
                           || just_group_list
                           || just_context);

  إذا (default_format && (use_real || use_name))
    error (EXIT_FAILURE, 0,
           _("printing only names or real IDs requires -u, -g, or -G"));

  إذا (default_format && opt_zero)
    error (EXIT_FAILURE, 0,
           _("option --zero not permitted in default format"));

  /* If we are on a SELinux/SMACK-enabled kernel, no user is specified, and
     either --context is specified or none of (-u,-g,-G) is specified,
     and we're not in POSIXLY_CORRECT mode, get our context.  Otherwise,
     leave the context variable alone - it has been initialized to an
     invalid value that will be not displayed in print_full_info().  */
  إذا (n_ids == 0
      && (just_context
          || (default_format && ! getenv ("POSIXLY_CORRECT"))))
    {
      /* Report failure only if --context (-Z) was explicitly requested.  */
      إذا ((selinux_enabled && getcon (&context) && just_context)
          || (smack_enabled
              && smack_new_label_from_self (&context) < 0
              && just_context))
        error (EXIT_FAILURE, 0, _("can't get process context"));
    }

  إذا (n_ids >= 1)
    {
      multiple_users = n_ids > 1 ? true : false;
      /* Changing the value of n_ids to the last index in the array where we
         have the last possible user id. This helps us because we don't have
         to declare a different variable to keep a track of where the
         last username lies in argv[].  */
      n_ids += optind;
      /* For each username/userid to get its pw_name field */
      لكل (; optind < n_ids; optind++)
        {
          حرف *pw_name = عدم;
          بنية passwd *pwd = عدم;
          حرف ثابت *spec = argv[optind];
          /* Disallow an empty spec here as parse_user_spec() doesn't
             give an error for that as it seems it's a valid way to
             specify a noop or "reset special bits" depending on the system.  */
          إذا (*spec)
            {
              إذا (! parse_user_spec (spec, &euid, عدم, &pw_name, عدم))
                pwd = pw_name ? getpwnam (pw_name) : getpwuid (euid);
            }
          إذا (pwd == عدم)
            {
              error (0, errno, _("%s: no such user"), quote (spec));
              ok &= false;
            }
          وإلا
            {
              إذا (!pw_name)
                pw_name = xstrdup (pwd->pw_name);
              ruid = euid = pwd->pw_uid;
              rgid = egid = pwd->pw_gid;
              بغ_stuff (pw_name);
              إذا (ferror (stdout))
                write_error ();
            }
          حرر (pw_name);
        }
    }
  وإلا
    {
      /* POSIX says identification functions (getuid, getgid, and
         others) cannot fail, but they can fail under GNU/Hurd and a
         few other systems.  Test for failure by checking errno.  */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      إذا (just_user ? !use_real
          : !just_group && !just_group_list && !just_context)
        {
          errno = 0;
          euid = geteuid ();
          إذا (euid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective UID"));
        }

      إذا (just_user ? use_real
          : !just_group && (just_group_list || !just_context))
        {
          errno = 0;
          ruid = getuid ();
          إذا (ruid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real UID"));
        }

      إذا (!just_user && (just_group || just_group_list || !just_context))
        {
          errno = 0;
          egid = getegid ();
          إذا (egid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective GID"));

          errno = 0;
          rgid = getgid ();
          إذا (rgid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real GID"));
        }
        بغ_stuff (عدم);
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Print the name or value of user ID UID. */

ساكن فراغ
بغ_user (uid_t uid)
{
  بنية passwd *pwd = عدم;

  إذا (use_name)
    {
      pwd = getpwuid (uid);
      إذا (pwd == عدم)
        {
          error (0, 0, _("cannot find name for user ID %ju"), (uintmax_t) uid);
          ok &= false;
        }
    }

  إذا (pwd)
    printf ("%s", pwd->pw_name);
  وإلا
    printf ("%ju", (uintmax_t) uid);
}

/* Print all of the info about the user's user and group IDs. */

ساكن فراغ
بغ_full_info (حرف ثابت *username)
{
  بنية passwd *pwd;
  بنية group *grp;

  printf (_("uid=%ju"), (uintmax_t) ruid);
  pwd = getpwuid (ruid);
  إذا (pwd)
    printf ("(%s)", pwd->pw_name);

  printf (_(" gid=%ju"), (uintmax_t) rgid);
  grp = getgrgid (rgid);
  إذا (grp)
    printf ("(%s)", grp->gr_name);

  إذا (euid != ruid)
    {
      printf (_(" euid=%ju"), (uintmax_t) euid);
      pwd = getpwuid (euid);
      إذا (pwd)
        printf ("(%s)", pwd->pw_name);
    }

  إذا (egid != rgid)
    {
      printf (_(" egid=%ju"), (uintmax_t) egid);
      grp = getgrgid (egid);
      إذا (grp)
        printf ("(%s)", grp->gr_name);
    }

  {
    gid_t *groups;

    gid_t primary_group;
    إذا (username)
      primary_group = pwd ? pwd->pw_gid : -1;
    وإلا
      primary_group = egid;

    صحيح n_groups = xgetgroups (username, primary_group, &groups);
    إذا (n_groups < 0)
      {
        إذا (username)
          error (0, errno, _("failed to get groups for user %s"),
                 quote (username));
        وإلا
          error (0, errno, _("failed to get groups for the current process"));
        ok &= false;
        أرجع;
      }

    إذا (n_groups > 0)
      fputs (_(" groups="), stdout);
    لكل (صحيح i = 0; i < n_groups; i++)
      {
        إذا (i > 0)
          putchar (',');
        printf ("%ju", (uintmax_t) groups[i]);
        grp = getgrgid (groups[i]);
        إذا (grp)
          printf ("(%s)", grp->gr_name);
      }
    حرر (groups);
  }

  /* POSIX mandates the precise output format, and that it not include
     any context=... part, so skip that if POSIXLY_CORRECT is set.  */
  إذا (context)
    printf (_(" context=%s"), context);
}

/* Print information about the user based on the arguments passed. */

ساكن فراغ
بغ_stuff (حرف ثابت *pw_name)
{
  إذا (just_user)
      بغ_user (use_real ? ruid : euid);

  /* print_group and print_group_list return true on successful
     execution but false if something goes wrong. We then AND this value with
     the current value of 'ok' because we want to know if one of the previous
     users faced a problem in these functions. This value of 'ok' is later used
     to understand what status program should exit with. */
  وإلا إذا (just_group)
    ok &= بغ_group (use_real ? rgid : egid, use_name);
  وإلا إذا (just_group_list)
    ok &= بغ_group_list (pw_name, ruid, rgid, egid,
                            use_name, opt_zero ? '\0' : ' ');
  وإلا إذا (just_context)
    fputs (context, stdout);
  وإلا
    بغ_full_info (pw_name);

  /* When printing records for more than 1 user, at the end of groups
     of each user terminate the record with two consequent NUL characters
     to make parsing and distinguishing between two records possible. */
  إذا (opt_zero && just_group_list && multiple_users)
    {
      putchar ('\0');
      putchar ('\0');
    }
  وإلا
    {
      putchar (opt_zero ? '\0' : '\n');
    }
}
