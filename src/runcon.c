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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <selinux/selinux.h>
#ಅನ್ತರ್ಭಾವ <selinux/context.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "runcon"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Russell Coker")

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"role", required_ವಾದ, ಖಾಲಿ, 'r'},
  {"type", required_ವಾದ, ಖಾಲಿ, 't'},
  {"user", required_ವಾದ, ಖಾಲಿ, 'u'},
  {"range", required_ವಾದ, ಖಾಲಿ, 'l'},
  {"compute", no_ವಾದ, ಖಾಲಿ, 'c'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [CONTEXT COMMAND [ARG]...]\n\
  or:  %s [-c] [-u USER] [-r ROLE] [-t TYPE] [-l RANGE] COMMAND [ARG]...\n\
"), ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
ಸತತ a ಕಾರ್ಯಕ್ರಮ ಒಳ a different SELinux security context.\n\
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
  -t, --type=TYPE    type (ಗಾಗಿ same role as ಪೋಷಕ)\n\
"));
      oputs (_("\
  -u, --user=USER    user ತಟಸ್ಥ\n\
"));
      oputs (_("\
  -r, --role=ROLE    role\n\
"));
      oputs (_("\
  -l, --range=RANGE  levelrange\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_exec_ಸ್ಥಿತಿ (ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ *role = ಖಾಲಿ;
  ಅಕ್ಷರ *range = ಖಾಲಿ;
  ಅಕ್ಷರ *user = ಖಾಲಿ;
  ಅಕ್ಷರ *type = ಖಾಲಿ;
  ಅಕ್ಷರ *context = ಖಾಲಿ;
  ಅಕ್ಷರ *cur_context = ಖಾಲಿ;
  ಅಕ್ಷರ *file_context = ಖಾಲಿ;
  ಅಕ್ಷರ *new_context = ಖಾಲಿ;
  bool compute_trans = false;

  context_t con;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ option_ಸೂಚ್ಯಂಕ = 0;
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv, "+r:t:u:l:c", long_options,
                           &option_ಸೂಚ್ಯಂಕ);
      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'r':
          ಆದರೆ (role)
            error (EXIT_CANCELED, 0, _("multiple roles"));
          role = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 't':
          ಆದರೆ (type)
            error (EXIT_CANCELED, 0, _("multiple types"));
          type = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'u':
          ಆದರೆ (user)
            error (EXIT_CANCELED, 0, _("multiple users"));
          user = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'l':
          ಆದರೆ (range)
            error (EXIT_CANCELED, 0, _("multiple levelranges"));
          range = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'c':
          compute_trans = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_CANCELED);
          ನಿಲ್ಲಿಸು;
        }
    }

  ಆದರೆ (argc - optind == 0)
    {
      ಆದರೆ (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));
      fputs (cur_context, stdout);
      fputc ('\n', stdout);
      ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
    }

  ಆದರೆ (!(user || role || type || range || compute_trans))
    {
      ಆದರೆ (optind >= argc)
        {
          error (0, 0, _("you must specify -c, -t, -u, -l, -r, or context"));
          usage (EXIT_CANCELED);
        }
      context = argv[optind++];
    }

  ಆದರೆ (optind >= argc)
    {
      error (0, 0, _("no command specified"));
      usage (EXIT_CANCELED);
    }

  ಆದರೆ (is_selinux_enabled () != 1)
    error (EXIT_CANCELED, 0, _("%s may be used only on a SELinux kernel"),
           ಕಾರ್ಯಕ್ರಮ_name);

  ಆದರೆ (context)
    {
      con = context_new (context);
      ಆದರೆ (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (context));
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));

      /* We will generate context based on process transition */
      ಆದರೆ (compute_trans)
        {
          /* Get context of file to be executed */
          ಆದರೆ (getfilecon (argv[optind], &file_context) == -1)
            error (EXIT_CANCELED, errno,
                   _("failed to get security context of %s"),
                   quoteaf (argv[optind]));
          /* compute result of process transition */
          ಆದರೆ (security_compute_create (cur_context, file_context,
                                       string_ಗಮ್ಯ_security_class ("process"),
                                       &new_context) != 0)
            error (EXIT_CANCELED, errno, _("failed to compute a new context"));
          /* free contexts */
          freecon (file_context);
          freecon (cur_context);

          /* set cur_context equal to new_context */
          cur_context = new_context;
        }

      con = context_new (cur_context);
      ಆದರೆ (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (cur_context));
      ಆದರೆ (user && context_user_ಗಣ (con, user))
        error (EXIT_CANCELED, errno, _("failed to set new user: %s"),
               quote (user));
      ಆದರೆ (type && context_type_ಗಣ (con, type))
        error (EXIT_CANCELED, errno, _("failed to set new type: %s"),
               quote (type));
      ಆದರೆ (range && context_range_ಗಣ (con, range))
        error (EXIT_CANCELED, errno, _("failed to set new range: %s"),
               quote (range));
      ಆದರೆ (role && context_role_ಗಣ (con, role))
        error (EXIT_CANCELED, errno, _("failed to set new role: %s"),
               quote (role));
    }

  ಆದರೆ (security_check_context (context_str (con)) < 0)
    error (EXIT_CANCELED, errno, _("invalid context: %s"),
           quote (context_str (con)));

  ಆದರೆ (setexeccon (context_str (con)) != 0)
    error (EXIT_CANCELED, errno, _("unable to set security context %s"),
           quote (context_str (con)));
  ಆದರೆ (cur_context != ಖಾಲಿ)
    freecon (cur_context);

  (compute_trans ? execv : execvp) (argv[optind], argv + optind);

  ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[optind]));
  ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}
