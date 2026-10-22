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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <selinux/selinux.h>
#समावेश <selinux/context.h>
#समावेश <sys/types.h>
#समावेश "system.h"
#समावेश "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "runcon"

#घोषणा AUTHORS proper_name ("Russell Coker")

अटल काष्ठा option स्थिर long_options[] =
{
  {"role", required_argument, शून्य, 'r'},
  {"type", required_argument, शून्य, 't'},
  {"user", required_argument, शून्य, 'u'},
  {"range", required_argument, शून्य, 'l'},
  {"compute", no_argument, शून्य, 'c'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("\
Usage: %s [CONTEXT COMMAND [ARG]...]\n\
  or:  %s [-c] [-u USER] [-r ROLE] [-t TYPE] [-l RANGE] COMMAND [ARG]...\n\
"), program_name, program_name);
      ख_माला_दो (_("\
Run a program in a different SELinux security context.\n\
With neither CONTEXT nor COMMAND, print the current security context.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      ख_माला_दो (_("\
  CONTEXT            Complete security context\n\
"), मानक_निकास);
      oputs (_("\
  -c, --compute      compute process transition context before modifying\n\
"));
      oputs (_("\
  -t, --type=TYPE    type (क्रम same role as parent)\n\
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
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अक्षर *role = शून्य;
  अक्षर *range = शून्य;
  अक्षर *user = शून्य;
  अक्षर *type = शून्य;
  अक्षर *context = शून्य;
  अक्षर *cur_context = शून्य;
  अक्षर *file_context = शून्य;
  अक्षर *new_context = शून्य;
  bool compute_trans = false;

  context_t con;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  निकास_पर (close_stdout);

  जबतक (true)
    {
      पूर्णांक option_सूचकांक = 0;
      पूर्णांक c = getopt_long (argc, argv, "+r:t:u:l:c", long_options,
                           &option_सूचकांक);
      अगर (c == -1)
        अवरोध;
      चयन (c)
        {
        हाल 'r':
          अगर (role)
            error (EXIT_CANCELED, 0, _("multiple roles"));
          role = optarg;
          अवरोध;
        हाल 't':
          अगर (type)
            error (EXIT_CANCELED, 0, _("multiple types"));
          type = optarg;
          अवरोध;
        हाल 'u':
          अगर (user)
            error (EXIT_CANCELED, 0, _("multiple users"));
          user = optarg;
          अवरोध;
        हाल 'l':
          अगर (range)
            error (EXIT_CANCELED, 0, _("multiple levelranges"));
          range = optarg;
          अवरोध;
        हाल 'c':
          compute_trans = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (EXIT_CANCELED);
          अवरोध;
        }
    }

  अगर (argc - optind == 0)
    {
      अगर (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to get current context"));
      ख_माला_दो (cur_context, मानक_निकास);
      ख_अक्षर_दो ('\n', मानक_निकास);
      वापस निकास_सफल;
    }

  अगर (!(user || role || type || range || compute_trans))
    {
      अगर (optind >= argc)
        {
          error (0, 0, _("you must specify -c, -t, -u, -l, -r, or context"));
          usage (EXIT_CANCELED);
        }
      context = argv[optind++];
    }

  अगर (optind >= argc)
    {
      error (0, 0, _("no command specified"));
      usage (EXIT_CANCELED);
    }

  अगर (is_selinux_enabled () != 1)
    error (EXIT_CANCELED, 0, _("%s may be used only on a SELinux kernel"),
           program_name);

  अगर (context)
    {
      con = context_new (context);
      अगर (!con)
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to create security context: %s"),
               quote (context));
    }
  अन्यथा
    {
      अगर (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to get current context"));

      /* We will generate context based on process transition */
      अगर (compute_trans)
        {
          /* Get context of file to be executed */
          अगर (getfilecon (argv[optind], &file_context) == -1)
            error (EXIT_CANCELED, त्रुटि_सं,
                   _("failed to get security context of %s"),
                   quoteaf (argv[optind]));
          /* compute result of process transition */
          अगर (security_compute_create (cur_context, file_context,
                                       string_to_security_class ("process"),
                                       &new_context) != 0)
            error (EXIT_CANCELED, त्रुटि_सं, _("failed to compute a new context"));
          /* free contexts */
          freecon (file_context);
          freecon (cur_context);

          /* set cur_context equal to new_context */
          cur_context = new_context;
        }

      con = context_new (cur_context);
      अगर (!con)
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to create security context: %s"),
               quote (cur_context));
      अगर (user && context_user_set (con, user))
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to set new user: %s"),
               quote (user));
      अगर (type && context_type_set (con, type))
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to set new type: %s"),
               quote (type));
      अगर (range && context_range_set (con, range))
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to set new range: %s"),
               quote (range));
      अगर (role && context_role_set (con, role))
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to set new role: %s"),
               quote (role));
    }

  अगर (security_check_context (context_str (con)) < 0)
    error (EXIT_CANCELED, त्रुटि_सं, _("invalid context: %s"),
           quote (context_str (con)));

  अगर (setexeccon (context_str (con)) != 0)
    error (EXIT_CANCELED, त्रुटि_सं, _("unable to set security context %s"),
           quote (context_str (con)));
  अगर (cur_context != शून्य)
    freecon (cur_context);

  (compute_trans ? execv : execvp) (argv[optind], argv + optind);

  पूर्णांक exit_status = त्रुटि_सं == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, त्रुटि_सं, "%s", quote (argv[optind]));
  वापस exit_status;
}
