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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <selinux/selinux.h>
#అన్తర్భావ <selinux/context.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ "system.h"
#అన్తర్భావ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "runcon"

#నిర్వచించు AUTHORS proper_name ("Russell Coker")

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"role", required_వాదం, ఖాళీ, 'r'},
  {"type", required_వాదం, ఖాళీ, 't'},
  {"user", required_వాదం, ఖాళీ, 'u'},
  {"range", required_వాదం, ఖాళీ, 'l'},
  {"compute", no_వాదం, ఖాళీ, 'c'},
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
      రాయి (_("\
Usage: %s [CONTEXT COMMAND [ARG]...]\n\
  or:  %s [-c] [-u USER] [-r ROLE] [-t TYPE] [-l RANGE] COMMAND [ARG]...\n\
"), కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
నిరంతరం a కార్యక్రమం లోపల a different SELinux security context.\n\
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
  -t, --type=TYPE    type (కొరకు same role as మాతృ)\n\
"));
      oputs (_("\
  -u, --user=USER    user తటస్థం\n\
"));
      oputs (_("\
  -r, --role=ROLE    role\n\
"));
      oputs (_("\
  -l, --range=RANGE  levelrange\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_exec_స్థితి (కార్యక్రమం_NAME);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం *role = ఖాళీ;
  అక్షరం *range = ఖాళీ;
  అక్షరం *user = ఖాళీ;
  అక్షరం *type = ఖాళీ;
  అక్షరం *context = ఖాళీ;
  అక్షరం *cur_context = ఖాళీ;
  అక్షరం *file_context = ఖాళీ;
  అక్షరం *new_context = ఖాళీ;
  bool compute_trans = false;

  context_t con;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  అయితే (true)
    {
      పూర్ణాంకం option_సూచీ = 0;
      పూర్ణాంకం c = getopt_long (argc, argv, "+r:t:u:l:c", long_options,
                           &option_సూచీ);
      ఒకవేళ (c == -1)
        ఆపు;
      ఎంపిక (c)
        {
        స్థితి 'r':
          ఒకవేళ (role)
            error (EXIT_CANCELED, 0, _("multiple roles"));
          role = optarg;
          ఆపు;
        స్థితి 't':
          ఒకవేళ (type)
            error (EXIT_CANCELED, 0, _("multiple types"));
          type = optarg;
          ఆపు;
        స్థితి 'u':
          ఒకవేళ (user)
            error (EXIT_CANCELED, 0, _("multiple users"));
          user = optarg;
          ఆపు;
        స్థితి 'l':
          ఒకవేళ (range)
            error (EXIT_CANCELED, 0, _("multiple levelranges"));
          range = optarg;
          ఆపు;
        స్థితి 'c':
          compute_trans = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_CANCELED);
          ఆపు;
        }
    }

  ఒకవేళ (argc - optind == 0)
    {
      ఒకవేళ (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));
      fputs (cur_context, stdout);
      fputc ('\n', stdout);
      తిరిగి EXIT_SUCCESS;
    }

  ఒకవేళ (!(user || role || type || range || compute_trans))
    {
      ఒకవేళ (optind >= argc)
        {
          error (0, 0, _("you must specify -c, -t, -u, -l, -r, or context"));
          usage (EXIT_CANCELED);
        }
      context = argv[optind++];
    }

  ఒకవేళ (optind >= argc)
    {
      error (0, 0, _("no command specified"));
      usage (EXIT_CANCELED);
    }

  ఒకవేళ (is_selinux_enabled () != 1)
    error (EXIT_CANCELED, 0, _("%s may be used only on a SELinux kernel"),
           కార్యక్రమం_name);

  ఒకవేళ (context)
    {
      con = context_new (context);
      ఒకవేళ (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (context));
    }
  లేకపోతే
    {
      ఒకవేళ (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));

      /* We will generate context based on process transition */
      ఒకవేళ (compute_trans)
        {
          /* Get context of file to be executed */
          ఒకవేళ (getfilecon (argv[optind], &file_context) == -1)
            error (EXIT_CANCELED, errno,
                   _("failed to get security context of %s"),
                   quoteaf (argv[optind]));
          /* compute result of process transition */
          ఒకవేళ (security_compute_create (cur_context, file_context,
                                       string_గమ్యం_security_class ("process"),
                                       &new_context) != 0)
            error (EXIT_CANCELED, errno, _("failed to compute a new context"));
          /* free contexts */
          freecon (file_context);
          freecon (cur_context);

          /* set cur_context equal to new_context */
          cur_context = new_context;
        }

      con = context_new (cur_context);
      ఒకవేళ (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (cur_context));
      ఒకవేళ (user && context_user_సమితి (con, user))
        error (EXIT_CANCELED, errno, _("failed to set new user: %s"),
               quote (user));
      ఒకవేళ (type && context_type_సమితి (con, type))
        error (EXIT_CANCELED, errno, _("failed to set new type: %s"),
               quote (type));
      ఒకవేళ (range && context_range_సమితి (con, range))
        error (EXIT_CANCELED, errno, _("failed to set new range: %s"),
               quote (range));
      ఒకవేళ (role && context_role_సమితి (con, role))
        error (EXIT_CANCELED, errno, _("failed to set new role: %s"),
               quote (role));
    }

  ఒకవేళ (security_check_context (context_str (con)) < 0)
    error (EXIT_CANCELED, errno, _("invalid context: %s"),
           quote (context_str (con)));

  ఒకవేళ (setexeccon (context_str (con)) != 0)
    error (EXIT_CANCELED, errno, _("unable to set security context %s"),
           quote (context_str (con)));
  ఒకవేళ (cur_context != ఖాళీ)
    freecon (cur_context);

  (compute_trans ? execv : execvp) (argv[optind], argv + optind);

  పూర్ణాంకం exit_స్థితి = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[optind]));
  తిరిగి exit_స్థితి;
}
