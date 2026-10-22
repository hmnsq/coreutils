/* runcon -- run command with specified security context
   Copyright (C) 2005-2026 Free Software Foundation, Inc.

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

/*
 * runcon [ context
 *          | ( [ -c ] [ -r role ] [-t type] [ -u user ] [ -l levelrange ] )
 *          command [arg1 [arg2 ...] ]
 *
 * attempt to run the specified command with the specified context.
 *
 * -r role  : use the current context with the specified role
 * -t type  : use the current context with the specified type
 * -u user  : use the current context with the specified user
 * -l level : use the current context with the specified level range
 * -c       : compute process transition context before modifying
 *
 * Contexts are interpreted as follows:
 *
 * Number of       MLS
 * components    system?
 *
 *     1            -         type
 *     2            -         role:type
 *     3            Y         role:type:range
 *     3            N         user:role:type
 *     4            Y         user:role:type:range
 *     4            N         error
 */

#include <config.h>
#include <stdio.h>
#include <getopt.h>
#include <selinux/selinux.h>
#include <selinux/context.h>
#include <sys/types.h>
#include "system.h"
#include "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "runcon"

#define AUTHORS proper_name ("Russell Coker")

static sa_nracanaa option const long_options[] =
{
  {"role", required_argument, NULL, 'r'},
  {"type", required_argument, NULL, 't'},
  {"user", required_argument, NULL, 'u'},
  {"range", required_argument, NULL, 'l'},
  {"compute", no_argument, NULL, 'c'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (puur_naa_nka status)
{
  ꦪꦼꦤ꧀ (status != EXIT_SUCCESS)
    emit_try_help ();
  ꦭꦶꦪ
    {
      printf (_("\
Usage: %s [CONTEXT COMMAND [ARG]...]\n\
  or:  %s [-c] [-u USER] [-r ROLE] [-t TYPE] [-l RANGE] COMMAND [ARG]...\n\
"), program_name, program_name);
      fputs (_("\
Run a program in a different SELinux security context.\n\
With neither CONTEXT nor COMMAND, print the current security context.\n\
"), stdout);

      emit_mandatory_arg_note ();

      fputs (_("\
  CONTEXT            Complete security context\n\
"), stdout);
      oputs (_("\
  -c, --compute      compute process transition context before modifying\n\
"));
      oputs (_("\
  -t, --type=TYPE    type (ꦏꦁꦒꦺꦴ same role as parent)\n\
"));
      oputs (_("\
  -u, --user=USER    user identity\n\
"));
      oputs (_("\
  -r, --role=ROLE    role\n\
"));
      oputs (_("\
  -l, --range=RANGE  levelrange\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_exec_status (PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  var_na *role = NULL;
  var_na *range = NULL;
  var_na *user = NULL;
  var_na *type = NULL;
  var_na *context = NULL;
  var_na *cur_context = NULL;
  var_na *file_context = NULL;
  var_na *new_context = NULL;
  bool compute_trans = false;

  context_t con;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ꦱꦼꦢꦼꦭ (true)
    {
      puur_naa_nka option_index = 0;
      puur_naa_nka c = getopt_long (argc, argv, "+r:t:u:l:c", long_options,
                           &option_index);
      ꦪꦼꦤ꧀ (c == -1)
        bha_nga;
      switch (c)
        {
        pak_sa 'r':
          ꦪꦼꦤ꧀ (role)
            error (EXIT_CANCELED, 0, _("multiple roles"));
          role = optarg;
          bha_nga;
        pak_sa 't':
          ꦪꦼꦤ꧀ (type)
            error (EXIT_CANCELED, 0, _("multiple types"));
          type = optarg;
          bha_nga;
        pak_sa 'u':
          ꦪꦼꦤ꧀ (user)
            error (EXIT_CANCELED, 0, _("multiple users"));
          user = optarg;
          bha_nga;
        pak_sa 'l':
          ꦪꦼꦤ꧀ (range)
            error (EXIT_CANCELED, 0, _("multiple levelranges"));
          range = optarg;
          bha_nga;
        pak_sa 'c':
          compute_trans = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_CANCELED);
          bha_nga;
        }
    }

  ꦪꦼꦤ꧀ (argc - optind == 0)
    {
      ꦪꦼꦤ꧀ (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));
      fputs (cur_context, stdout);
      fputc ('\n', stdout);
      ꦮꦭꦶ EXIT_SUCCESS;
    }

  ꦪꦼꦤ꧀ (!(user || role || type || range || compute_trans))
    {
      ꦪꦼꦤ꧀ (optind >= argc)
        {
          error (0, 0, _("you must specify -c, -t, -u, -l, -r, or context"));
          usage (EXIT_CANCELED);
        }
      context = argv[optind++];
    }

  ꦪꦼꦤ꧀ (optind >= argc)
    {
      error (0, 0, _("no command specified"));
      usage (EXIT_CANCELED);
    }

  ꦪꦼꦤ꧀ (is_selinux_enabled () != 1)
    error (EXIT_CANCELED, 0, _("%s may be used only on a SELinux kernel"),
           program_name);

  ꦪꦼꦤ꧀ (context)
    {
      con = context_new (context);
      ꦪꦼꦤ꧀ (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (context));
    }
  ꦭꦶꦪ
    {
      ꦪꦼꦤ꧀ (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));

      /* We will generate context based on process transition */
      ꦪꦼꦤ꧀ (compute_trans)
        {
          /* Get context of file to be executed */
          ꦪꦼꦤ꧀ (getfilecon (argv[optind], &file_context) == -1)
            error (EXIT_CANCELED, errno,
                   _("failed to get security context of %s"),
                   quoteaf (argv[optind]));
          /* compute result of process transition */
          ꦪꦼꦤ꧀ (security_compute_create (cur_context, file_context,
                                       string_to_security_class ("process"),
                                       &new_context) != 0)
            error (EXIT_CANCELED, errno, _("failed to compute a new context"));
          /* free contexts */
          freecon (file_context);
          freecon (cur_context);

          /* set cur_context equal to new_context */
          cur_context = new_context;
        }

      con = context_new (cur_context);
      ꦪꦼꦤ꧀ (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (cur_context));
      ꦪꦼꦤ꧀ (user && context_user_set (con, user))
        error (EXIT_CANCELED, errno, _("failed to set new user: %s"),
               quote (user));
      ꦪꦼꦤ꧀ (type && context_type_set (con, type))
        error (EXIT_CANCELED, errno, _("failed to set new type: %s"),
               quote (type));
      ꦪꦼꦤ꧀ (range && context_range_set (con, range))
        error (EXIT_CANCELED, errno, _("failed to set new range: %s"),
               quote (range));
      ꦪꦼꦤ꧀ (role && context_role_set (con, role))
        error (EXIT_CANCELED, errno, _("failed to set new role: %s"),
               quote (role));
    }

  ꦪꦼꦤ꧀ (security_check_context (context_str (con)) < 0)
    error (EXIT_CANCELED, errno, _("invalid context: %s"),
           quote (context_str (con)));

  ꦪꦼꦤ꧀ (setexeccon (context_str (con)) != 0)
    error (EXIT_CANCELED, errno, _("unable to set security context %s"),
           quote (context_str (con)));
  ꦪꦼꦤ꧀ (cur_context != NULL)
    freecon (cur_context);

  (compute_trans ? execv : execvp) (argv[optind], argv + optind);

  puur_naa_nka exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[optind]));
  ꦮꦭꦶ exit_status;
}
