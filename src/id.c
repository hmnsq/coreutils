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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <pwd.h>
#अन्तर्भाव <grp.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <selinux/selinux.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "mgetgroups.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "group-list.h"
#अन्तर्भाव "smack.h"
#अन्तर्भाव "userspec.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "id"

#define AUTHORS \
  proper_name ("Arnold Robbins"), \
  proper_name ("David MacKenzie")

/* If nonzero, output only the SELinux context.  */
स्थिर bool just_context = 0;
/* If true, delimit entries with NUL characters, not whitespace */
स्थिर bool opt_zero = false;
/* If true, output the list of all group IDs. -G */
स्थिर bool just_group_list = false;
/* If true, output only the group ID(s). -g */
स्थिर bool just_group = false;
/* If true, output real UID/GID instead of default effective UID/GID. -r */
स्थिर bool use_real = false;
/* If true, output only the user ID(s). -u */
स्थिर bool just_user = false;
/* True unless errors have been encountered.  */
स्थिर bool ok = true;
/* If true, we are using multiple users. Terminate -G with double NUL. */
स्थिर bool multiple_users = false;
/* If true, output user/group name instead of ID number. -n */
स्थिर bool use_name = false;

/* The real and effective IDs of the user to print. */
स्थिर uid_t ruid, euid;
स्थिर gid_t rgid, egid;

/* The SELinux context.  Start with a known invalid value so print_full_info
   knows when 'context' has not been set to a meaningful value.  */
स्थिर वर्ण *context = रिक्त;

स्थिर शून्य print_user (uid_t uid);
स्थिर शून्य print_full_info (वर्ण ध्रुव *username);
स्थिर शून्य print_stuff (वर्ण ध्रुव *pw_name);

स्थिर संरचना option ध्रुव longopts[] =
{
  {"context", no_argument, रिक्त, 'Z'},
  {"group", no_argument, रिक्त, 'g'},
  {"groups", no_argument, रिक्त, 'G'},
  {"name", no_argument, रिक्त, 'n'},
  {"real", no_argument, रिक्त, 'r'},
  {"user", no_argument, रिक्त, 'u'},
  {"zero", no_argument, रिक्त, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      printf (_("Usage: %s [OPTION]... [USER]...\n"), program_name);
      fputs (_("\
Print user and group information प्रति each specified USER,\n\
or (when USER omitted) प्रति the current process.\n\
\n"),
             stdout);
      oputs (_("\
  -a\n\
         ignore, प्रति compatibility with other versions\n\
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
         print a name instead of a संख्या, प्रति -u, -g, -G\n\
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
         not permitted in स्वभाव format\n\
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

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क optc;
  पूर्णाङ्क selinux_enabled = (is_selinux_enabled () > 0);
  bool smack_enabled = is_smack_enabled ();

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  यावत् ((optc = getopt_long (argc, argv, "agnruzGZ", longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'a':
          /* Ignore -a, for compatibility with SVR4.  */
          भङ्ग;

        पक्ष 'Z':
          /* politely decline if we're not on a SELinux/SMACK-enabled kernel.
             Keep require_selinux_ in tests in sync with these messages.  */
#ifdef HAVE_SMACK
          यदि (!selinux_enabled && !smack_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on "
                     "an SELinux/SMACK-enabled kernel"));
#अन्यथा
          यदि (!selinux_enabled)
            error (EXIT_FAILURE, 0,
                   _("--context (-Z) works only on an SELinux-enabled kernel"));
#endif
          just_context = true;
          भङ्ग;

        पक्ष 'g':
          just_group = true;
          भङ्ग;
        पक्ष 'n':
          use_name = true;
          भङ्ग;
        पक्ष 'r':
          use_real = true;
          भङ्ग;
        पक्ष 'u':
          just_user = true;
          भङ्ग;
        पक्ष 'z':
          opt_zero = true;
          भङ्ग;
        पक्ष 'G':
          just_group_list = true;
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  size_t n_ids = argc - optind;

  यदि (n_ids && just_context)
    error (EXIT_FAILURE, 0,
           _("cannot print security context when user specified"));

  यदि (just_user + just_group + just_group_list + just_context > 1)
    error (EXIT_FAILURE, 0, _("cannot print \"only\" of more than one choice"));

  bool default_format = ! (just_user
                           || just_group
                           || just_group_list
                           || just_context);

  यदि (default_format && (use_real || use_name))
    error (EXIT_FAILURE, 0,
           _("printing only names or real IDs requires -u, -g, or -G"));

  यदि (default_format && opt_zero)
    error (EXIT_FAILURE, 0,
           _("option --zero not permitted in default format"));

  /* If we are on a SELinux/SMACK-enabled kernel, no user is specified, and
     either --context is specified or none of (-u,-g,-G) is specified,
     and we're not in POSIXLY_CORRECT mode, get our context.  Otherwise,
     leave the context variable alone - it has been initialized to an
     invalid value that will be not displayed in print_full_info().  */
  यदि (n_ids == 0
      && (just_context
          || (default_format && ! getenv ("POSIXLY_CORRECT"))))
    {
      /* Report failure only if --context (-Z) was explicitly requested.  */
      यदि ((selinux_enabled && getcon (&context) && just_context)
          || (smack_enabled
              && smack_new_label_from_self (&context) < 0
              && just_context))
        error (EXIT_FAILURE, 0, _("can't get process context"));
    }

  यदि (n_ids >= 1)
    {
      multiple_users = n_ids > 1 ? true : false;
      /* Changing the value of n_ids to the last index in the array where we
         have the last possible user id. This helps us because we don't have
         to declare a different variable to keep a track of where the
         last username lies in argv[].  */
      n_ids += optind;
      /* For each username/userid to get its pw_name field */
      प्रति (; optind < n_ids; optind++)
        {
          वर्ण *pw_name = रिक्त;
          संरचना passwd *pwd = रिक्त;
          वर्ण ध्रुव *spec = argv[optind];
          /* Disallow an empty spec here as parse_user_spec() doesn't
             give an error for that as it seems it's a valid way to
             specify a noop or "reset special bits" depending on the system.  */
          यदि (*spec)
            {
              यदि (! parse_user_spec (spec, &euid, रिक्त, &pw_name, रिक्त))
                pwd = pw_name ? getpwnam (pw_name) : getpwuid (euid);
            }
          यदि (pwd == रिक्त)
            {
              error (0, errno, _("%s: no such user"), quote (spec));
              ok &= false;
            }
          अन्यथा
            {
              यदि (!pw_name)
                pw_name = xstrdup (pwd->pw_name);
              ruid = euid = pwd->pw_uid;
              rgid = egid = pwd->pw_gid;
              print_stuff (pw_name);
              यदि (ferror (stdout))
                write_error ();
            }
          मुक्त (pw_name);
        }
    }
  अन्यथा
    {
      /* POSIX says identification functions (getuid, getgid, and
         others) cannot fail, but they can fail under GNU/Hurd and a
         few other systems.  Test for failure by checking errno.  */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      यदि (just_user ? !use_real
          : !just_group && !just_group_list && !just_context)
        {
          errno = 0;
          euid = geteuid ();
          यदि (euid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective UID"));
        }

      यदि (just_user ? use_real
          : !just_group && (just_group_list || !just_context))
        {
          errno = 0;
          ruid = getuid ();
          यदि (ruid == NO_UID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real UID"));
        }

      यदि (!just_user && (just_group || just_group_list || !just_context))
        {
          errno = 0;
          egid = getegid ();
          यदि (egid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get effective GID"));

          errno = 0;
          rgid = getgid ();
          यदि (rgid == NO_GID && errno)
            error (EXIT_FAILURE, errno, _("cannot get real GID"));
        }
        print_stuff (रिक्त);
    }

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}

/* Print the name or value of user ID UID. */

स्थिर शून्य
print_user (uid_t uid)
{
  संरचना passwd *pwd = रिक्त;

  यदि (use_name)
    {
      pwd = getpwuid (uid);
      यदि (pwd == रिक्त)
        {
          error (0, 0, _("cannot find name for user ID %ju"), (uintmax_t) uid);
          ok &= false;
        }
    }

  यदि (pwd)
    printf ("%s", pwd->pw_name);
  अन्यथा
    printf ("%ju", (uintmax_t) uid);
}

/* Print all of the info about the user's user and group IDs. */

स्थिर शून्य
print_full_info (वर्ण ध्रुव *username)
{
  संरचना passwd *pwd;
  संरचना group *grp;

  printf (_("uid=%ju"), (uintmax_t) ruid);
  pwd = getpwuid (ruid);
  यदि (pwd)
    printf ("(%s)", pwd->pw_name);

  printf (_(" gid=%ju"), (uintmax_t) rgid);
  grp = getgrgid (rgid);
  यदि (grp)
    printf ("(%s)", grp->gr_name);

  यदि (euid != ruid)
    {
      printf (_(" euid=%ju"), (uintmax_t) euid);
      pwd = getpwuid (euid);
      यदि (pwd)
        printf ("(%s)", pwd->pw_name);
    }

  यदि (egid != rgid)
    {
      printf (_(" egid=%ju"), (uintmax_t) egid);
      grp = getgrgid (egid);
      यदि (grp)
        printf ("(%s)", grp->gr_name);
    }

  {
    gid_t *groups;

    gid_t primary_group;
    यदि (username)
      primary_group = pwd ? pwd->pw_gid : -1;
    अन्यथा
      primary_group = egid;

    पूर्णाङ्क n_groups = xgetgroups (username, primary_group, &groups);
    यदि (n_groups < 0)
      {
        यदि (username)
          error (0, errno, _("failed to get groups for user %s"),
                 quote (username));
        अन्यथा
          error (0, errno, _("failed to get groups for the current process"));
        ok &= false;
        प्रत्यागम;
      }

    यदि (n_groups > 0)
      fputs (_(" groups="), stdout);
    प्रति (पूर्णाङ्क i = 0; i < n_groups; i++)
      {
        यदि (i > 0)
          putchar (',');
        printf ("%ju", (uintmax_t) groups[i]);
        grp = getgrgid (groups[i]);
        यदि (grp)
          printf ("(%s)", grp->gr_name);
      }
    मुक्त (groups);
  }

  /* POSIX mandates the precise output format, and that it not include
     any context=... part, so skip that if POSIXLY_CORRECT is set.  */
  यदि (context)
    printf (_(" context=%s"), context);
}

/* Print information about the user based on the arguments passed. */

स्थिर शून्य
print_stuff (वर्ण ध्रुव *pw_name)
{
  यदि (just_user)
      print_user (use_real ? ruid : euid);

  /* print_group and print_group_list return true on successful
     execution but false if something goes wrong. We then AND this value with
     the current value of 'ok' because we want to know if one of the previous
     users faced a problem in these functions. This value of 'ok' is later used
     to understand what status program should exit with. */
  अन्यथा यदि (just_group)
    ok &= print_group (use_real ? rgid : egid, use_name);
  अन्यथा यदि (just_group_list)
    ok &= print_group_list (pw_name, ruid, rgid, egid,
                            use_name, opt_zero ? '\0' : ' ');
  अन्यथा यदि (just_context)
    fputs (context, stdout);
  अन्यथा
    print_full_info (pw_name);

  /* When printing records for more than 1 user, at the end of groups
     of each user terminate the record with two consequent NUL characters
     to make parsing and distinguishing between two records possible. */
  यदि (opt_zero && just_group_list && multiple_users)
    {
      putchar ('\0');
      putchar ('\0');
    }
  अन्यथा
    {
      putchar (opt_zero ? '\0' : '\n');
    }
}
