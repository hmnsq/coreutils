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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <pwd.h>
#અન્તર્ભાવ <grp.h>
#અન્તર્ભાવ <getopt.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "group-list.h"
#અન્તર્ભાવ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "groups"

#define AUTHORS \
  proper_name ("David MacKenzie"), \
  proper_name ("James Youngman")


સ્થિર સંરચના option અચલ longopts[] =
{
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]... [USERNAME]...\n"), program_name);
      fputs (_("\
Print group memberships માટે each USERNAME or, જો no USERNAME is specified, માટે\
\n\
the current process (which may differ જો the groups database has changed).\n"),
             stdout);
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  /* Processing the arguments this way makes groups.c behave differently to
     groups.sh if one of the arguments is "--".  */
  પૂર્ણાંક optc;
  જ્યારે ((optc = getopt_long (argc, argv, "", longopts, ખાલી)) != -1)
    {
      પસંદગી (optc)
        {
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  bool ok = true;
  જો (optind == argc)
    {
      /* No arguments.  Divulge the details of the current process. */
      uid_t NO_UID = -1;
      gid_t NO_GID = -1;

      errno = 0;
      uid_t ruid = getuid ();
      જો (ruid == NO_UID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real UID"));

      errno = 0;
      gid_t egid = getegid ();
      જો (egid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get effective GID"));

      errno = 0;
      gid_t rgid = getgid ();
      જો (rgid == NO_GID && errno)
        error (EXIT_FAILURE, errno, _("cannot get real GID"));

      જો (!print_group_list (ખાલી, ruid, rgid, egid, true, ' '))
        ok = false;
      putchar ('\n');
    }
  નહીંતર
    {
      /* At least one argument.  Divulge the details of the specified users.  */
      માટે ( ; optind < argc; optind++)
        {
          સંરચના passwd *pwd = getpwnam (argv[optind]);
          જો (pwd == ખાલી)
            {
              error (0, 0, _("%s: no such user"), quote (argv[optind]));
              ok = false;
              ચાલુ;
            }
          uid_t ruid = pwd->pw_uid;
          gid_t rgid = pwd->pw_gid;
          gid_t egid = rgid;

          printf ("%s : ", argv[optind]);
          જો (!print_group_list (argv[optind], ruid, rgid, egid, true, ' '))
            ok = false;
          putchar ('\n');

          જો (ferror (stdout))
            write_error ();
        }
    }

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
