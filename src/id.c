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

#شامل <config.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#شامل <pwd.h>
#شامل <grp.h>
#شامل <getopt.h>
#شامل <selinux/selinux.h>

#شامل "system.h"
#شامل "mgetgroups.h"
#شامل "quote.h"
#شامل "group-list.h"
#شامل "smack.h"
#شامل "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "id"

#تعریف_کرو AUTHORS \
  proper_name ("Arnold Robbins"), \
  proper_name ("David MacKenzie")

/* If nonzero, output only the SELinux context.  */
ساکن bool just_context = 0;
/* If true, delimit entries with NUL characters, not whitespace */
ساکن bool opt_zero = false;
/* If true, output the list of all group IDs. -G */
ساکن bool just_گروہ_list = false;
/* If true, output only the group ID(s). -g */
ساکن bool just_گروہ = false;
/* If true, output real UID/GID instead of default effective UID/GID. -r */
ساکن bool use_real = false;
/* If true, output only the user ID(s). -u */
ساکن bool just_user = false;
/* True unless errors have been encountered.  */
ساکن bool ok = true;
/* If true, we are using multiple users. Terminate -G with double NUL. */
ساکن bool مضرب_users = false;
/* If true, output user/group name instead of ID number. -n */
ساکن bool use_name = false;

/* The real and effective IDs of the user to print. */
ساکن uid_t ruid, euid;
ساکن gid_t rgid, egid;

/* The SELinux context.  Start with a known invalid value so print_full_info
   knows when 'context' has not been set to a meaningful value.  */
ساکن حرف *context = تہی;

ساکن خالی print_user (uid_t uid);
ساکن خالی print_بھرا_info (حرف ثابت *username);
ساکن خالی print_stuff (حرف ثابت *pw_name);

ساکن ساخت option ثابت longopts[] =
{
  {"context", no_دلیل, تہی, 'Z'},
  {"group", no_دلیل, تہی, 'g'},
  {"groups", no_دلیل, تہی, 'G'},
  {"name", no_دلیل, تہی, 'n'},
  {"real", no_دلیل, تہی, 'r'},
  {"user", no_دلیل, تہی, 'u'},
  {"zero", no_دلیل, تہی, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]... [USER]...\n"), پروگرام_name);
      fputs (_("\
Print user and گروہ information برائے each specified USER,\n\
or (when USER omitted) برائے the current process.\n\
\n"),
             stdout);
      oputs (_("\
  -a\n\
         ignore, برائے compatibility with other versions\n\
"));
      oputs (_("\
  -Z, --context\n\
         print only the security context کا the process\n\
"));
      oputs (_("\
  -g, --گروہ\n\
         print only the effective گروہ ID\n\
"));
      oputs (_("\
  -G, --groups\n\
         print all گروہ IDs\n\
"));
      oputs (_("\
  -n, --name\n\
         print a name instead کا a نمبر, برائے -u, -g, -G\n\
"));
      oputs (_("\
  -r, --real\n\
         print the real ID instead کا the effective ID, with -u, -g, -G\n\
"));
      oputs (_("\
  -u, --user\n\
         print only the effective user ID\n\
"));
      oputs (_("\
  -z, --zero\n\
         delimit entries with NUL characters, not whitespace;\n\
         not permitted اندر طےشدہ format\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Without any OPTION, print some useful سیٹ_2 کا identified information.\n\
"), stdout);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح optc;
  صحیح selinux_enabled = (is_selinux_enabled () > 0);
  bool smack_enabled = is_smack_enabled ();

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  جب ((optc = getopt_long (argc, argv, "agnruzGZ", longopts, تہی)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'a':
          /* Ignore -a, for compatibility with SVR4.  */
          توڑو;

        حالت 'Z':
          /* politely decline if we're not on a SELinux/SMACK-enabled kernel.
             Keep require_selinux_ in tests in sync with these messages.  */
#اگر_تعریف_شدہ HAVE_SMACK
          اگر (!selinux_enabled && !smack_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on "
                     "an SELinux/SMACK-enabled kernel"));
#ورنہ
          اگر (!selinux_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on an SELinux-enabled kernel"));
#اگر_ختم
          just_context = true;
          توڑو;

        حالت 'g':
          just_گروہ = true;
          توڑو;
        حالت 'n':
          use_name = true;
          توڑو;
        حالت 'r':
          use_real = true;
          توڑو;
        حالت 'u':
          just_user = true;
          توڑو;
        حالت 'z':
          opt_zero = true;
          توڑو;
        حالت 'G':
          just_گروہ_list = true;
          توڑو;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  size_t n_ids = argc - optind;

  اگر (n_ids && just_context)
    error (EXIT_FAILURE, 0,
           _("cannot print security context when user specified"));

  اگر (just_user + just_گروہ + just_گروہ_list + just_context > 1)
    error (EXIT_FAILURE, 0, _("cannot print \"only\" of more than one choice"));

  bool default_format = ! (just_user
                           || just_گروہ
                           || just_گروہ_list
                           || just_context);

  اگر (default_format && (use_real || use_name))
    error (EXIT_FAILURE, 0,
           _("printing only names or real IDs requires -u, -g, or -G"));

  اگر (default_format && opt_zero)
    error (EXIT_FAILURE, 0,
           _("option --zero not permitted in default format"));

  /* If we are on a SELinux/SMACK-enabled kernel, no user is specified, and
     either --context is specified or none of (-u,-g,-G) is specified,
     and we're not in POSIXLY_CORRECT mode, get our context.  Otherwise,
     leave the context variable alone - it has been initialized to an
     invalid value that will be not displayed in print_full_info().  */
  اگر (n_ids == 0
      && (just_context
          || (default_format && ! getenv ("POSIXLY_CORRECT"))))
    {
      /* Report failure only if --context (-Z) was explicitly requested.  */
      اگر ((selinux_enabled && getcon (&context) && just_context)
          || (smack_enabled
              && smack_new_label_from_self (&context) < 0
              && just_context))
        error (EXIT_FAILURE, 0, _("can't get process context"));
    }

  اگر (n_ids >= 1)
    {
      مضرب_users = n_ids > 1 ? true : false;
      /* Changing the value of n_ids to the last index in the array where we
         have the last possible user id. This helps us because we don't have
         to declare a different variable to keep a track of where the
         last username lies in argv[].  */
      n_ids += optind;
      /* For each username/userid to get its pw_name field */
      برائے (; optind < n_ids; optind++)
        {
          حرف *pw_name = تہی;
          ساخت passwd *pwd = تہی;
          حرف ثابت *spec = argv[optind];
          /* Disallow an empty spec here as parse_user_spec() doesn't
             give an error for that as it seems it's a valid way to
             specify a noop or "reset special bits" depending on the system.  */
          اگر (*spec)
            {
              اگر (! parse_user_spec (spec, &euid, تہی, &pw_name, تہی))
                pwd = pw_name ? getpwnam (pw_name) : getpwuid (euid);
            }
          اگر (pwd == تہی)
            {
              error (0, errno, _("%s: no such user"), quote (spec));
              ok &= false;
            }
          ورنہ
            {
              اگر (!pw_name)
                pw_name = xstrdup (pwd->pw_name);
              ruid = euid = pwd->pw_uid;
              rgid = egid = pwd->pw_gid;
              print_stuff (pw_name);
              اگر (ferror (stdout))
                write_error ();
            }
          آزاد (pw_name);
        }
    }
  ورنہ
    {
      /* POSIX says identification functions (getuid, getgid, and
         others) cannot fail, but they can fail under GNU/Hurd and a
         few other systems.  Test for failure by checking errno.  */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      اگر (just_user ? !use_real
          : !just_گروہ && !just_گروہ_list && !just_context)
        {
          errno = 0;
          euid = geteuid ();
          اگر (euid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective UID"));
        }

      اگر (just_user ? use_real
          : !just_گروہ && (just_گروہ_list || !just_context))
        {
          errno = 0;
          ruid = getuid ();
          اگر (ruid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real UID"));
        }

      اگر (!just_user && (just_گروہ || just_گروہ_list || !just_context))
        {
          errno = 0;
          egid = getegid ();
          اگر (egid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective GID"));

          errno = 0;
          rgid = getgid ();
          اگر (rgid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real GID"));
        }
        print_stuff (تہی);
    }

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Print the name or value of user ID UID. */

ساکن خالی
print_user (uid_t uid)
{
  ساخت passwd *pwd = تہی;

  اگر (use_name)
    {
      pwd = getpwuid (uid);
      اگر (pwd == تہی)
        {
          error (0, 0, _("cannot find name for user ID %ju"), (uintmax_t) uid);
          ok &= false;
        }
    }

  اگر (pwd)
    لکھو ("%s", pwd->pw_name);
  ورنہ
    لکھو ("%ju", (uintmax_t) uid);
}

/* Print all of the info about the user's user and group IDs. */

ساکن خالی
print_بھرا_info (حرف ثابت *username)
{
  ساخت passwd *pwd;
  ساخت گروہ *grp;

  لکھو (_("uid=%ju"), (uintmax_t) ruid);
  pwd = getpwuid (ruid);
  اگر (pwd)
    لکھو ("(%s)", pwd->pw_name);

  لکھو (_(" gid=%ju"), (uintmax_t) rgid);
  grp = getgrgid (rgid);
  اگر (grp)
    لکھو ("(%s)", grp->gr_name);

  اگر (euid != ruid)
    {
      لکھو (_(" euid=%ju"), (uintmax_t) euid);
      pwd = getpwuid (euid);
      اگر (pwd)
        لکھو ("(%s)", pwd->pw_name);
    }

  اگر (egid != rgid)
    {
      لکھو (_(" egid=%ju"), (uintmax_t) egid);
      grp = getgrgid (egid);
      اگر (grp)
        لکھو ("(%s)", grp->gr_name);
    }

  {
    gid_t *groups;

    gid_t primary_گروہ;
    اگر (username)
      primary_گروہ = pwd ? pwd->pw_gid : -1;
    ورنہ
      primary_گروہ = egid;

    صحیح n_groups = xgetgroups (username, primary_گروہ, &groups);
    اگر (n_groups < 0)
      {
        اگر (username)
          error (0, errno, _("failed to get groups for user %s"),
                 quote (username));
        ورنہ
          error (0, errno, _("failed to get groups for the current process"));
        ok &= false;
        واپس;
      }

    اگر (n_groups > 0)
      fputs (_(" groups="), stdout);
    برائے (صحیح i = 0; i < n_groups; i++)
      {
        اگر (i > 0)
          حرف_لکھو (',');
        لکھو ("%ju", (uintmax_t) groups[i]);
        grp = getgrgid (groups[i]);
        اگر (grp)
          لکھو ("(%s)", grp->gr_name);
      }
    آزاد (groups);
  }

  /* POSIX mandates the precise output format, and that it not include
     any context=... part, so skip that if POSIXLY_CORRECT is set.  */
  اگر (context)
    لکھو (_(" context=%s"), context);
}

/* Print information about the user based on the arguments passed. */

ساکن خالی
print_stuff (حرف ثابت *pw_name)
{
  اگر (just_user)
      print_user (use_real ? ruid : euid);

  /* print_group and print_group_list return true on successful
     execution but false if something goes wrong. We then AND this value with
     the current value of 'ok' because we want to know if one of the previous
     users faced a problem in these functions. This value of 'ok' is later used
     to understand what status program should exit with. */
  ورنہ اگر (just_گروہ)
    ok &= print_گروہ (use_real ? rgid : egid, use_name);
  ورنہ اگر (just_گروہ_list)
    ok &= print_گروہ_list (pw_name, ruid, rgid, egid,
                            use_name, opt_zero ? '\0' : ' ');
  ورنہ اگر (just_context)
    fputs (context, stdout);
  ورنہ
    print_بھرا_info (pw_name);

  /* When printing records for more than 1 user, at the end of groups
     of each user terminate the record with two consequent NUL characters
     to make parsing and distinguishing between two records possible. */
  اگر (opt_zero && just_گروہ_list && مضرب_users)
    {
      حرف_لکھو ('\0');
      حرف_لکھو ('\0');
    }
  ورنہ
    {
      حرف_لکھو (opt_zero ? '\0' : '\n');
    }
}
