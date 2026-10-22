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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pwd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <grp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/selinux.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "mgetgroups.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "group-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "smack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "id"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("Arnold Robbins"), \
  proper_name ("David MacKenzie")

/* If nonzero, output only the SELinux context.  */
ਸਥਿਰ bool just_context = 0;
/* If true, delimit entries with NUL characters, not whitespace */
ਸਥਿਰ bool opt_zero = false;
/* If true, output the list of all group IDs. -G */
ਸਥਿਰ bool just_ਸਮੂਹ_list = false;
/* If true, output only the group ID(s). -g */
ਸਥਿਰ bool just_ਸਮੂਹ = false;
/* If true, output real UID/GID instead of default effective UID/GID. -r */
ਸਥਿਰ bool use_real = false;
/* If true, output only the user ID(s). -u */
ਸਥਿਰ bool just_user = false;
/* True unless errors have been encountered.  */
ਸਥਿਰ bool ok = true;
/* If true, we are using multiple users. Terminate -G with double NUL. */
ਸਥਿਰ bool ਗੁਣਜ_users = false;
/* If true, output user/group name instead of ID number. -n */
ਸਥਿਰ bool use_name = false;

/* The real and effective IDs of the user to print. */
ਸਥਿਰ uid_t ruid, euid;
ਸਥਿਰ gid_t rgid, egid;

/* The SELinux context.  Start with a known invalid value so print_full_info
   knows when 'context' has not been set to a meaningful value.  */
ਸਥਿਰ ਅੱਖਰ *context = ਨਿਰਾ;

ਸਥਿਰ ਖਾਲੀ print_user (uid_t uid);
ਸਥਿਰ ਖਾਲੀ print_ਭਰਿਆ_info (ਅੱਖਰ ਅਚਲ *username);
ਸਥਿਰ ਖਾਲੀ print_stuff (ਅੱਖਰ ਅਚਲ *pw_name);

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"context", no_ਦਲੀਲ, ਨਿਰਾ, 'Z'},
  {"group", no_ਦਲੀਲ, ਨਿਰਾ, 'g'},
  {"groups", no_ਦਲੀਲ, ਨਿਰਾ, 'G'},
  {"name", no_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"real", no_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"user", no_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"zero", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... [USER]...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Print user and ਸਮੂਹ information ਲਈ each specified USER,\n\
or (when USER omitted) ਲਈ the current process.\n\
\n"),
             stdout);
      oputs (_("\
  -a\n\
         ignore, ਲਈ compatibility with other versions\n\
"));
      oputs (_("\
  -Z, --context\n\
         print only the security context ਦਾ the process\n\
"));
      oputs (_("\
  -g, --ਸਮੂਹ\n\
         print only the effective ਸਮੂਹ ID\n\
"));
      oputs (_("\
  -G, --groups\n\
         print all ਸਮੂਹ IDs\n\
"));
      oputs (_("\
  -n, --name\n\
         print a name instead ਦਾ a ਸੰਖਿਆ, ਲਈ -u, -g, -G\n\
"));
      oputs (_("\
  -r, --real\n\
         print the real ID instead ਦਾ the effective ID, with -u, -g, -G\n\
"));
      oputs (_("\
  -u, --user\n\
         print only the effective user ID\n\
"));
      oputs (_("\
  -z, --zero\n\
         delimit entries with NUL characters, not whitespace;\n\
         not permitted ਵਿੱਚ ਮੂਲ format\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Without any OPTION, print some useful ਸੈੱਟ_2 ਦਾ identified information.\n\
"), stdout);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ optc;
  ਪੂਰਨ selinux_enabled = (is_selinux_enabled () > 0);
  bool smack_enabled = is_smack_enabled ();

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "agnruzGZ", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'a':
          /* Ignore -a, for compatibility with SVR4.  */
          ਰੋਕੋ;

        ਹਾਲਤ 'Z':
          /* politely decline if we're not on a SELinux/SMACK-enabled kernel.
             Keep require_selinux_ in tests in sync with these messages.  */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SMACK
          ਜੇ (!selinux_enabled && !smack_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on "
                     "an SELinux/SMACK-enabled kernel"));
#ਨਹੀਂਤਾਂ
          ਜੇ (!selinux_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on an SELinux-enabled kernel"));
#ਜੇ_ਅੰਤ
          just_context = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'g':
          just_ਸਮੂਹ = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'n':
          use_name = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'r':
          use_real = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'u':
          just_user = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'z':
          opt_zero = true;
          ਰੋਕੋ;
        ਹਾਲਤ 'G':
          just_ਸਮੂਹ_list = true;
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  size_t n_ids = argc - optind;

  ਜੇ (n_ids && just_context)
    error (EXIT_FAILURE, 0,
           _("cannot print security context when user specified"));

  ਜੇ (just_user + just_ਸਮੂਹ + just_ਸਮੂਹ_list + just_context > 1)
    error (EXIT_FAILURE, 0, _("cannot print \"only\" of more than one choice"));

  bool default_format = ! (just_user
                           || just_ਸਮੂਹ
                           || just_ਸਮੂਹ_list
                           || just_context);

  ਜੇ (default_format && (use_real || use_name))
    error (EXIT_FAILURE, 0,
           _("printing only names or real IDs requires -u, -g, or -G"));

  ਜੇ (default_format && opt_zero)
    error (EXIT_FAILURE, 0,
           _("option --zero not permitted in default format"));

  /* If we are on a SELinux/SMACK-enabled kernel, no user is specified, and
     either --context is specified or none of (-u,-g,-G) is specified,
     and we're not in POSIXLY_CORRECT mode, get our context.  Otherwise,
     leave the context variable alone - it has been initialized to an
     invalid value that will be not displayed in print_full_info().  */
  ਜੇ (n_ids == 0
      && (just_context
          || (default_format && ! getenv ("POSIXLY_CORRECT"))))
    {
      /* Report failure only if --context (-Z) was explicitly requested.  */
      ਜੇ ((selinux_enabled && getcon (&context) && just_context)
          || (smack_enabled
              && smack_new_label_from_self (&context) < 0
              && just_context))
        error (EXIT_FAILURE, 0, _("can't get process context"));
    }

  ਜੇ (n_ids >= 1)
    {
      ਗੁਣਜ_users = n_ids > 1 ? true : false;
      /* Changing the value of n_ids to the last index in the array where we
         have the last possible user id. This helps us because we don't have
         to declare a different variable to keep a track of where the
         last username lies in argv[].  */
      n_ids += optind;
      /* For each username/userid to get its pw_name field */
      ਲਈ (; optind < n_ids; optind++)
        {
          ਅੱਖਰ *pw_name = ਨਿਰਾ;
          ਬਣਤਰ passwd *pwd = ਨਿਰਾ;
          ਅੱਖਰ ਅਚਲ *spec = argv[optind];
          /* Disallow an empty spec here as parse_user_spec() doesn't
             give an error for that as it seems it's a valid way to
             specify a noop or "reset special bits" depending on the system.  */
          ਜੇ (*spec)
            {
              ਜੇ (! parse_user_spec (spec, &euid, ਨਿਰਾ, &pw_name, ਨਿਰਾ))
                pwd = pw_name ? getpwnam (pw_name) : getpwuid (euid);
            }
          ਜੇ (pwd == ਨਿਰਾ)
            {
              error (0, errno, _("%s: no such user"), quote (spec));
              ok &= false;
            }
          ਨਹੀਂਤਾਂ
            {
              ਜੇ (!pw_name)
                pw_name = xstrdup (pwd->pw_name);
              ruid = euid = pwd->pw_uid;
              rgid = egid = pwd->pw_gid;
              print_stuff (pw_name);
              ਜੇ (ferror (stdout))
                write_error ();
            }
          ਮੁਕਤ (pw_name);
        }
    }
  ਨਹੀਂਤਾਂ
    {
      /* POSIX says identification functions (getuid, getgid, and
         others) cannot fail, but they can fail under GNU/Hurd and a
         few other systems.  Test for failure by checking errno.  */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      ਜੇ (just_user ? !use_real
          : !just_ਸਮੂਹ && !just_ਸਮੂਹ_list && !just_context)
        {
          errno = 0;
          euid = geteuid ();
          ਜੇ (euid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective UID"));
        }

      ਜੇ (just_user ? use_real
          : !just_ਸਮੂਹ && (just_ਸਮੂਹ_list || !just_context))
        {
          errno = 0;
          ruid = getuid ();
          ਜੇ (ruid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real UID"));
        }

      ਜੇ (!just_user && (just_ਸਮੂਹ || just_ਸਮੂਹ_list || !just_context))
        {
          errno = 0;
          egid = getegid ();
          ਜੇ (egid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective GID"));

          errno = 0;
          rgid = getgid ();
          ਜੇ (rgid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real GID"));
        }
        print_stuff (ਨਿਰਾ);
    }

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Print the name or value of user ID UID. */

ਸਥਿਰ ਖਾਲੀ
print_user (uid_t uid)
{
  ਬਣਤਰ passwd *pwd = ਨਿਰਾ;

  ਜੇ (use_name)
    {
      pwd = getpwuid (uid);
      ਜੇ (pwd == ਨਿਰਾ)
        {
          error (0, 0, _("cannot find name for user ID %ju"), (uintmax_t) uid);
          ok &= false;
        }
    }

  ਜੇ (pwd)
    ਲਿਖੋ ("%s", pwd->pw_name);
  ਨਹੀਂਤਾਂ
    ਲਿਖੋ ("%ju", (uintmax_t) uid);
}

/* Print all of the info about the user's user and group IDs. */

ਸਥਿਰ ਖਾਲੀ
print_ਭਰਿਆ_info (ਅੱਖਰ ਅਚਲ *username)
{
  ਬਣਤਰ passwd *pwd;
  ਬਣਤਰ ਸਮੂਹ *grp;

  ਲਿਖੋ (_("uid=%ju"), (uintmax_t) ruid);
  pwd = getpwuid (ruid);
  ਜੇ (pwd)
    ਲਿਖੋ ("(%s)", pwd->pw_name);

  ਲਿਖੋ (_(" gid=%ju"), (uintmax_t) rgid);
  grp = getgrgid (rgid);
  ਜੇ (grp)
    ਲਿਖੋ ("(%s)", grp->gr_name);

  ਜੇ (euid != ruid)
    {
      ਲਿਖੋ (_(" euid=%ju"), (uintmax_t) euid);
      pwd = getpwuid (euid);
      ਜੇ (pwd)
        ਲਿਖੋ ("(%s)", pwd->pw_name);
    }

  ਜੇ (egid != rgid)
    {
      ਲਿਖੋ (_(" egid=%ju"), (uintmax_t) egid);
      grp = getgrgid (egid);
      ਜੇ (grp)
        ਲਿਖੋ ("(%s)", grp->gr_name);
    }

  {
    gid_t *groups;

    gid_t primary_ਸਮੂਹ;
    ਜੇ (username)
      primary_ਸਮੂਹ = pwd ? pwd->pw_gid : -1;
    ਨਹੀਂਤਾਂ
      primary_ਸਮੂਹ = egid;

    ਪੂਰਨ n_groups = xgetgroups (username, primary_ਸਮੂਹ, &groups);
    ਜੇ (n_groups < 0)
      {
        ਜੇ (username)
          error (0, errno, _("failed to get groups for user %s"),
                 quote (username));
        ਨਹੀਂਤਾਂ
          error (0, errno, _("failed to get groups for the current process"));
        ok &= false;
        ਵਾਪਸ;
      }

    ਜੇ (n_groups > 0)
      fputs (_(" groups="), stdout);
    ਲਈ (ਪੂਰਨ i = 0; i < n_groups; i++)
      {
        ਜੇ (i > 0)
          ਅੱਖਰ_ਲਿਖੋ (',');
        ਲਿਖੋ ("%ju", (uintmax_t) groups[i]);
        grp = getgrgid (groups[i]);
        ਜੇ (grp)
          ਲਿਖੋ ("(%s)", grp->gr_name);
      }
    ਮੁਕਤ (groups);
  }

  /* POSIX mandates the precise output format, and that it not include
     any context=... part, so skip that if POSIXLY_CORRECT is set.  */
  ਜੇ (context)
    ਲਿਖੋ (_(" context=%s"), context);
}

/* Print information about the user based on the arguments passed. */

ਸਥਿਰ ਖਾਲੀ
print_stuff (ਅੱਖਰ ਅਚਲ *pw_name)
{
  ਜੇ (just_user)
      print_user (use_real ? ruid : euid);

  /* print_group and print_group_list return true on successful
     execution but false if something goes wrong. We then AND this value with
     the current value of 'ok' because we want to know if one of the previous
     users faced a problem in these functions. This value of 'ok' is later used
     to understand what status program should exit with. */
  ਨਹੀਂਤਾਂ ਜੇ (just_ਸਮੂਹ)
    ok &= print_ਸਮੂਹ (use_real ? rgid : egid, use_name);
  ਨਹੀਂਤਾਂ ਜੇ (just_ਸਮੂਹ_list)
    ok &= print_ਸਮੂਹ_list (pw_name, ruid, rgid, egid,
                            use_name, opt_zero ? '\0' : ' ');
  ਨਹੀਂਤਾਂ ਜੇ (just_context)
    fputs (context, stdout);
  ਨਹੀਂਤਾਂ
    print_ਭਰਿਆ_info (pw_name);

  /* When printing records for more than 1 user, at the end of groups
     of each user terminate the record with two consequent NUL characters
     to make parsing and distinguishing between two records possible. */
  ਜੇ (opt_zero && just_ਸਮੂਹ_list && ਗੁਣਜ_users)
    {
      ਅੱਖਰ_ਲਿਖੋ ('\0');
      ਅੱਖਰ_ਲਿਖੋ ('\0');
    }
  ਨਹੀਂਤਾਂ
    {
      ਅੱਖਰ_ਲਿਖੋ (opt_zero ? '\0' : '\n');
    }
}
