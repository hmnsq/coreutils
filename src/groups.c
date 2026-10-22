/* groups -- print the groups a user is in
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

/* Written by James Youngman based on id.c and groups.sh,
   which were written by Arnold Robbins and David MacKenzie. */

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <pwd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <grp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "group-list.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "groups"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("James Youngman")


ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
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
      ਲਿਖੋ (_("Usage: %s [OPTION]... [USERNAME]...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Print ਸਮੂਹ memberships ਲਈ each USERNAME or, ਜੇ no USERNAME is specified, ਲਈ\
\n\
the current process (which may ਫਰਕ ਜੇ the groups database has ਬਦਲਿਆ).\n"),
             stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Processing the arguments this way makes groups.c behave differently to
     groups.sh if one of the arguments is "--".  */
  ਪੂਰਨ optc;
  ਜਦੋਂ ((optc = getopt_long (argc, argv, "", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  bool ok = true;
  ਜੇ (optind == argc)
    {
      /* No arguments.  Divulge the details of the current process. */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      errno = 0;
      uid_t ruid = getuid ();
      ਜੇ (ruid == NO_UID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real UID"));

      errno = 0;
      gid_t egid = getegid ();
      ਜੇ (egid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get effective GID"));

      errno = 0;
      gid_t rgid = getgid ();
      ਜੇ (rgid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real GID"));

      ਜੇ (!print_ਸਮੂਹ_list (ਨਿਰਾ, ruid, rgid, egid, true, ' '))
        ok = false;
      ਅੱਖਰ_ਲਿਖੋ ('\n');
    }
  ਨਹੀਂਤਾਂ
    {
      /* At least one argument.  Divulge the details of the specified users.  */
      ਲਈ ( ; optind < argc; optind++)
        {
          ਬਣਤਰ passwd *pwd = getpwnam (argv[optind]);
          ਜੇ (pwd == ਨਿਰਾ)
            {
              error (0, 0, _("%s: no such user"), quote (argv[optind]));
              ok = false;
              ਜਾਰੀ;
            }
          uid_t ruid = pwd->pw_uid;
          gid_t rgid = pwd->pw_gid;
          gid_t egid = rgid;

          ਲਿਖੋ ("%s : ", argv[optind]);
          ਜੇ (!print_ਸਮੂਹ_list (argv[optind], ruid, rgid, egid, true, ' '))
            ok = false;
          ਅੱਖਰ_ਲਿਖੋ ('\n');

          ਜੇ (ferror (stdout))
            write_error ();
        }
    }

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
