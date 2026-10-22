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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <pwd.h>
#అన్తర్భావ <grp.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <selinux/selinux.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "mgetgroups.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "group-list.h"
#అన్తర్భావ "smack.h"
#అన్తర్భావ "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "id"

#నిర్వచించు AUTHORS \
  proper_name ("Arnold Robbins"), \
  proper_name ("David MacKenzie")

/* If nonzero, output only the SELinux context.  */
స్థిర bool just_context = 0;
/* If true, delimit entries with NUL characters, not whitespace */
స్థిర bool opt_zero = false;
/* If true, output the list of all group IDs. -G */
స్థిర bool just_సమూహం_list = false;
/* If true, output only the group ID(s). -g */
స్థిర bool just_సమూహం = false;
/* If true, output real UID/GID instead of default effective UID/GID. -r */
స్థిర bool use_real = false;
/* If true, output only the user ID(s). -u */
స్థిర bool just_user = false;
/* True unless errors have been encountered.  */
స్థిర bool ok = true;
/* If true, we are using multiple users. Terminate -G with double NUL. */
స్థిర bool గుణిజం_users = false;
/* If true, output user/group name instead of ID number. -n */
స్థిర bool use_name = false;

/* The real and effective IDs of the user to print. */
స్థిర uid_t ruid, euid;
స్థిర gid_t rgid, egid;

/* The SELinux context.  Start with a known invalid value so print_full_info
   knows when 'context' has not been set to a meaningful value.  */
స్థిర అక్షరం *context = ఖాళీ;

స్థిర శూన్య print_user (uid_t uid);
స్థిర శూన్య print_నిండిన_info (అక్షరం స్థిరరాశి *username);
స్థిర శూన్య print_stuff (అక్షరం స్థిరరాశి *pw_name);

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {"context", no_వాదం, ఖాళీ, 'Z'},
  {"group", no_వాదం, ఖాళీ, 'g'},
  {"groups", no_వాదం, ఖాళీ, 'G'},
  {"name", no_వాదం, ఖాళీ, 'n'},
  {"real", no_వాదం, ఖాళీ, 'r'},
  {"user", no_వాదం, ఖాళీ, 'u'},
  {"zero", no_వాదం, ఖాళీ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... [USER]...\n"), కార్యక్రమం_name);
      fputs (_("\
Print user and సమూహం information కొరకు each specified USER,\n\
or (when USER omitted) కొరకు the current process.\n\
\n"),
             stdout);
      oputs (_("\
  -a\n\
         ignore, కొరకు compatibility with other versions\n\
"));
      oputs (_("\
  -Z, --context\n\
         print only the security context యొక్క the process\n\
"));
      oputs (_("\
  -g, --సమూహం\n\
         print only the effective సమూహం ID\n\
"));
      oputs (_("\
  -G, --groups\n\
         print all సమూహం IDs\n\
"));
      oputs (_("\
  -n, --name\n\
         print a name instead యొక్క a సంఖ్య, కొరకు -u, -g, -G\n\
"));
      oputs (_("\
  -r, --real\n\
         print the real ID instead యొక్క the effective ID, with -u, -g, -G\n\
"));
      oputs (_("\
  -u, --user\n\
         print only the effective user ID\n\
"));
      oputs (_("\
  -z, --zero\n\
         delimit entries with NUL characters, not whitespace;\n\
         not permitted లోపల అప్రమేయ format\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Without any OPTION, print some useful సమితి_2 యొక్క identified information.\n\
"), stdout);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం optc;
  పూర్ణాంకం selinux_enabled = (is_selinux_enabled () > 0);
  bool smack_enabled = is_smack_enabled ();

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, "agnruzGZ", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'a':
          /* Ignore -a, for compatibility with SVR4.  */
          ఆపు;

        స్థితి 'Z':
          /* politely decline if we're not on a SELinux/SMACK-enabled kernel.
             Keep require_selinux_ in tests in sync with these messages.  */
#నిర్వచితమైతే HAVE_SMACK
          ఒకవేళ (!selinux_enabled && !smack_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on "
                     "an SELinux/SMACK-enabled kernel"));
#లేకపోతే
          ఒకవేళ (!selinux_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on an SELinux-enabled kernel"));
#అయితే_ముగింపు
          just_context = true;
          ఆపు;

        స్థితి 'g':
          just_సమూహం = true;
          ఆపు;
        స్థితి 'n':
          use_name = true;
          ఆపు;
        స్థితి 'r':
          use_real = true;
          ఆపు;
        స్థితి 'u':
          just_user = true;
          ఆపు;
        స్థితి 'z':
          opt_zero = true;
          ఆపు;
        స్థితి 'G':
          just_సమూహం_list = true;
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  size_t n_ids = argc - optind;

  ఒకవేళ (n_ids && just_context)
    error (EXIT_FAILURE, 0,
           _("cannot print security context when user specified"));

  ఒకవేళ (just_user + just_సమూహం + just_సమూహం_list + just_context > 1)
    error (EXIT_FAILURE, 0, _("cannot print \"only\" of more than one choice"));

  bool default_format = ! (just_user
                           || just_సమూహం
                           || just_సమూహం_list
                           || just_context);

  ఒకవేళ (default_format && (use_real || use_name))
    error (EXIT_FAILURE, 0,
           _("printing only names or real IDs requires -u, -g, or -G"));

  ఒకవేళ (default_format && opt_zero)
    error (EXIT_FAILURE, 0,
           _("option --zero not permitted in default format"));

  /* If we are on a SELinux/SMACK-enabled kernel, no user is specified, and
     either --context is specified or none of (-u,-g,-G) is specified,
     and we're not in POSIXLY_CORRECT mode, get our context.  Otherwise,
     leave the context variable alone - it has been initialized to an
     invalid value that will be not displayed in print_full_info().  */
  ఒకవేళ (n_ids == 0
      && (just_context
          || (default_format && ! getenv ("POSIXLY_CORRECT"))))
    {
      /* Report failure only if --context (-Z) was explicitly requested.  */
      ఒకవేళ ((selinux_enabled && getcon (&context) && just_context)
          || (smack_enabled
              && smack_new_label_from_self (&context) < 0
              && just_context))
        error (EXIT_FAILURE, 0, _("can't get process context"));
    }

  ఒకవేళ (n_ids >= 1)
    {
      గుణిజం_users = n_ids > 1 ? true : false;
      /* Changing the value of n_ids to the last index in the array where we
         have the last possible user id. This helps us because we don't have
         to declare a different variable to keep a track of where the
         last username lies in argv[].  */
      n_ids += optind;
      /* For each username/userid to get its pw_name field */
      కొరకు (; optind < n_ids; optind++)
        {
          అక్షరం *pw_name = ఖాళీ;
          నిర్మాణం passwd *pwd = ఖాళీ;
          అక్షరం స్థిరరాశి *spec = argv[optind];
          /* Disallow an empty spec here as parse_user_spec() doesn't
             give an error for that as it seems it's a valid way to
             specify a noop or "reset special bits" depending on the system.  */
          ఒకవేళ (*spec)
            {
              ఒకవేళ (! parse_user_spec (spec, &euid, ఖాళీ, &pw_name, ఖాళీ))
                pwd = pw_name ? getpwnam (pw_name) : getpwuid (euid);
            }
          ఒకవేళ (pwd == ఖాళీ)
            {
              error (0, errno, _("%s: no such user"), quote (spec));
              ok &= false;
            }
          లేకపోతే
            {
              ఒకవేళ (!pw_name)
                pw_name = xstrdup (pwd->pw_name);
              ruid = euid = pwd->pw_uid;
              rgid = egid = pwd->pw_gid;
              print_stuff (pw_name);
              ఒకవేళ (ferror (stdout))
                write_error ();
            }
          విడుదల (pw_name);
        }
    }
  లేకపోతే
    {
      /* POSIX says identification functions (getuid, getgid, and
         others) cannot fail, but they can fail under GNU/Hurd and a
         few other systems.  Test for failure by checking errno.  */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      ఒకవేళ (just_user ? !use_real
          : !just_సమూహం && !just_సమూహం_list && !just_context)
        {
          errno = 0;
          euid = geteuid ();
          ఒకవేళ (euid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective UID"));
        }

      ఒకవేళ (just_user ? use_real
          : !just_సమూహం && (just_సమూహం_list || !just_context))
        {
          errno = 0;
          ruid = getuid ();
          ఒకవేళ (ruid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real UID"));
        }

      ఒకవేళ (!just_user && (just_సమూహం || just_సమూహం_list || !just_context))
        {
          errno = 0;
          egid = getegid ();
          ఒకవేళ (egid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective GID"));

          errno = 0;
          rgid = getgid ();
          ఒకవేళ (rgid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real GID"));
        }
        print_stuff (ఖాళీ);
    }

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Print the name or value of user ID UID. */

స్థిర శూన్య
print_user (uid_t uid)
{
  నిర్మాణం passwd *pwd = ఖాళీ;

  ఒకవేళ (use_name)
    {
      pwd = getpwuid (uid);
      ఒకవేళ (pwd == ఖాళీ)
        {
          error (0, 0, _("cannot find name for user ID %ju"), (uintmax_t) uid);
          ok &= false;
        }
    }

  ఒకవేళ (pwd)
    రాయి ("%s", pwd->pw_name);
  లేకపోతే
    రాయి ("%ju", (uintmax_t) uid);
}

/* Print all of the info about the user's user and group IDs. */

స్థిర శూన్య
print_నిండిన_info (అక్షరం స్థిరరాశి *username)
{
  నిర్మాణం passwd *pwd;
  నిర్మాణం సమూహం *grp;

  రాయి (_("uid=%ju"), (uintmax_t) ruid);
  pwd = getpwuid (ruid);
  ఒకవేళ (pwd)
    రాయి ("(%s)", pwd->pw_name);

  రాయి (_(" gid=%ju"), (uintmax_t) rgid);
  grp = getgrgid (rgid);
  ఒకవేళ (grp)
    రాయి ("(%s)", grp->gr_name);

  ఒకవేళ (euid != ruid)
    {
      రాయి (_(" euid=%ju"), (uintmax_t) euid);
      pwd = getpwuid (euid);
      ఒకవేళ (pwd)
        రాయి ("(%s)", pwd->pw_name);
    }

  ఒకవేళ (egid != rgid)
    {
      రాయి (_(" egid=%ju"), (uintmax_t) egid);
      grp = getgrgid (egid);
      ఒకవేళ (grp)
        రాయి ("(%s)", grp->gr_name);
    }

  {
    gid_t *groups;

    gid_t primary_సమూహం;
    ఒకవేళ (username)
      primary_సమూహం = pwd ? pwd->pw_gid : -1;
    లేకపోతే
      primary_సమూహం = egid;

    పూర్ణాంకం n_groups = xgetgroups (username, primary_సమూహం, &groups);
    ఒకవేళ (n_groups < 0)
      {
        ఒకవేళ (username)
          error (0, errno, _("failed to get groups for user %s"),
                 quote (username));
        లేకపోతే
          error (0, errno, _("failed to get groups for the current process"));
        ok &= false;
        తిరిగి;
      }

    ఒకవేళ (n_groups > 0)
      fputs (_(" groups="), stdout);
    కొరకు (పూర్ణాంకం i = 0; i < n_groups; i++)
      {
        ఒకవేళ (i > 0)
          అక్షరం_రాయి (',');
        రాయి ("%ju", (uintmax_t) groups[i]);
        grp = getgrgid (groups[i]);
        ఒకవేళ (grp)
          రాయి ("(%s)", grp->gr_name);
      }
    విడుదల (groups);
  }

  /* POSIX mandates the precise output format, and that it not include
     any context=... part, so skip that if POSIXLY_CORRECT is set.  */
  ఒకవేళ (context)
    రాయి (_(" context=%s"), context);
}

/* Print information about the user based on the arguments passed. */

స్థిర శూన్య
print_stuff (అక్షరం స్థిరరాశి *pw_name)
{
  ఒకవేళ (just_user)
      print_user (use_real ? ruid : euid);

  /* print_group and print_group_list return true on successful
     execution but false if something goes wrong. We then AND this value with
     the current value of 'ok' because we want to know if one of the previous
     users faced a problem in these functions. This value of 'ok' is later used
     to understand what status program should exit with. */
  లేకపోతే ఒకవేళ (just_సమూహం)
    ok &= print_సమూహం (use_real ? rgid : egid, use_name);
  లేకపోతే ఒకవేళ (just_సమూహం_list)
    ok &= print_సమూహం_list (pw_name, ruid, rgid, egid,
                            use_name, opt_zero ? '\0' : ' ');
  లేకపోతే ఒకవేళ (just_context)
    fputs (context, stdout);
  లేకపోతే
    print_నిండిన_info (pw_name);

  /* When printing records for more than 1 user, at the end of groups
     of each user terminate the record with two consequent NUL characters
     to make parsing and distinguishing between two records possible. */
  ఒకవేళ (opt_zero && just_సమూహం_list && గుణిజం_users)
    {
      అక్షరం_రాయి ('\0');
      అక్షరం_రాయి ('\0');
    }
  లేకపోతే
    {
      అక్షరం_రాయి (opt_zero ? '\0' : '\n');
    }
}
