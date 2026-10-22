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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <selinux/selinux.h>
#شامل <selinux/context.h>
#شامل <sys/types.h>
#شامل "system.h"
#شامل "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "runcon"

#define AUTHORS proper_name ("Russell Coker")

ایستا ساختار option ثابت long_options[] =
{
  {"role", required_argument, پوچ, 'r'},
  {"type", required_argument, پوچ, 't'},
  {"user", required_argument, پوچ, 'u'},
  {"range", required_argument, پوچ, 'l'},
  {"compute", no_argument, پوچ, 'c'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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
  -t, --type=TYPE    type (برای same role as parent)\n\
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

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  نویسه *role = پوچ;
  نویسه *range = پوچ;
  نویسه *user = پوچ;
  نویسه *type = پوچ;
  نویسه *context = پوچ;
  نویسه *cur_context = پوچ;
  نویسه *file_context = پوچ;
  نویسه *new_context = پوچ;
  bool compute_trans = false;

  context_t con;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  وقتی (true)
    {
      صحیح option_index = 0;
      صحیح c = getopt_long (argc, argv, "+r:t:u:l:c", long_options,
                           &option_index);
      اگر (c == -1)
        بشکن;
      انتخاب (c)
        {
        حالت 'r':
          اگر (role)
            error (EXIT_CANCELED, 0, _("multiple roles"));
          role = optarg;
          بشکن;
        حالت 't':
          اگر (type)
            error (EXIT_CANCELED, 0, _("multiple types"));
          type = optarg;
          بشکن;
        حالت 'u':
          اگر (user)
            error (EXIT_CANCELED, 0, _("multiple users"));
          user = optarg;
          بشکن;
        حالت 'l':
          اگر (range)
            error (EXIT_CANCELED, 0, _("multiple levelranges"));
          range = optarg;
          بشکن;
        حالت 'c':
          compute_trans = true;
          بشکن;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (EXIT_CANCELED);
          بشکن;
        }
    }

  اگر (argc - optind == 0)
    {
      اگر (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));
      fputs (cur_context, stdout);
      fputc ('\n', stdout);
      بازگشت EXIT_SUCCESS;
    }

  اگر (!(user || role || type || range || compute_trans))
    {
      اگر (optind >= argc)
        {
          error (0, 0, _("you must specify -c, -t, -u, -l, -r, or context"));
          usage (EXIT_CANCELED);
        }
      context = argv[optind++];
    }

  اگر (optind >= argc)
    {
      error (0, 0, _("no command specified"));
      usage (EXIT_CANCELED);
    }

  اگر (is_selinux_enabled () != 1)
    error (EXIT_CANCELED, 0, _("%s may be used only on a SELinux kernel"),
           program_name);

  اگر (context)
    {
      con = context_new (context);
      اگر (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (context));
    }
  وگرنه
    {
      اگر (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));

      /* We will generate context based on process transition */
      اگر (compute_trans)
        {
          /* Get context of file to be executed */
          اگر (getfilecon (argv[optind], &file_context) == -1)
            error (EXIT_CANCELED, errno,
                   _("failed to get security context of %s"),
                   quoteaf (argv[optind]));
          /* compute result of process transition */
          اگر (security_compute_create (cur_context, file_context,
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
      اگر (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (cur_context));
      اگر (user && context_user_set (con, user))
        error (EXIT_CANCELED, errno, _("failed to set new user: %s"),
               quote (user));
      اگر (type && context_type_set (con, type))
        error (EXIT_CANCELED, errno, _("failed to set new type: %s"),
               quote (type));
      اگر (range && context_range_set (con, range))
        error (EXIT_CANCELED, errno, _("failed to set new range: %s"),
               quote (range));
      اگر (role && context_role_set (con, role))
        error (EXIT_CANCELED, errno, _("failed to set new role: %s"),
               quote (role));
    }

  اگر (security_check_context (context_str (con)) < 0)
    error (EXIT_CANCELED, errno, _("invalid context: %s"),
           quote (context_str (con)));

  اگر (setexeccon (context_str (con)) != 0)
    error (EXIT_CANCELED, errno, _("unable to set security context %s"),
           quote (context_str (con)));
  اگر (cur_context != پوچ)
    freecon (cur_context);

  (compute_trans ? execv : execvp) (argv[optind], argv + optind);

  صحیح exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[optind]));
  بازگشت exit_status;
}
