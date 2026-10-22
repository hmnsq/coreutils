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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <selinux/selinux.h>
#अन्तर्भाव <selinux/context.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव "system.h"
#अन्तर्भाव "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "runcon"

#define AUTHORS proper_name ("Russell Coker")

स्थिर संरचना option अचर long_options[] =
{
  {"role", required_argument, रिक्त, 'r'},
  {"type", required_argument, रिक्त, 't'},
  {"user", required_argument, रिक्त, 'u'},
  {"range", required_argument, रिक्त, 'l'},
  {"compute", no_argument, रिक्त, 'c'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != निकास_सफल)
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
  -t, --type=TYPE    type (लागि same role as parent)\n\
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

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण *role = रिक्त;
  वर्ण *range = रिक्त;
  वर्ण *user = रिक्त;
  वर्ण *type = रिक्त;
  वर्ण *context = रिक्त;
  वर्ण *cur_context = रिक्त;
  वर्ण *file_context = रिक्त;
  वर्ण *new_context = रिक्त;
  bool compute_trans = false;

  context_t con;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  निकास_पर (close_stdout);

  जबसम्म (true)
    {
      पूर्णाङ्क option_index = 0;
      पूर्णाङ्क c = getopt_long (argc, argv, "+r:t:u:l:c", long_options,
                           &option_index);
      यदि (c == -1)
        रोक;
      विकल्प (c)
        {
        पक्ष 'r':
          यदि (role)
            error (EXIT_CANCELED, 0, _("multiple roles"));
          role = optarg;
          रोक;
        पक्ष 't':
          यदि (type)
            error (EXIT_CANCELED, 0, _("multiple types"));
          type = optarg;
          रोक;
        पक्ष 'u':
          यदि (user)
            error (EXIT_CANCELED, 0, _("multiple users"));
          user = optarg;
          रोक;
        पक्ष 'l':
          यदि (range)
            error (EXIT_CANCELED, 0, _("multiple levelranges"));
          range = optarg;
          रोक;
        पक्ष 'c':
          compute_trans = true;
          रोक;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_CANCELED);
          रोक;
        }
    }

  यदि (argc - optind == 0)
    {
      यदि (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to get current context"));
      ख_माला_दो (cur_context, मानक_निकास);
      ख_अक्षर_दो ('\n', मानक_निकास);
      फर्काऊ निकास_सफल;
    }

  यदि (!(user || role || type || range || compute_trans))
    {
      यदि (optind >= argc)
        {
          error (0, 0, _("you must specify -c, -t, -u, -l, -r, or context"));
          usage (EXIT_CANCELED);
        }
      context = argv[optind++];
    }

  यदि (optind >= argc)
    {
      error (0, 0, _("no command specified"));
      usage (EXIT_CANCELED);
    }

  यदि (is_selinux_enabled () != 1)
    error (EXIT_CANCELED, 0, _("%s may be used only on a SELinux kernel"),
           program_name);

  यदि (context)
    {
      con = context_new (context);
      यदि (!con)
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to create security context: %s"),
               quote (context));
    }
  अन्यथा
    {
      यदि (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to get current context"));

      /* We will generate context based on process transition */
      यदि (compute_trans)
        {
          /* Get context of file to be executed */
          यदि (getfilecon (argv[optind], &file_context) == -1)
            error (EXIT_CANCELED, त्रुटि_सं,
                   _("failed to get security context of %s"),
                   quoteaf (argv[optind]));
          /* compute result of process transition */
          यदि (security_compute_create (cur_context, file_context,
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
      यदि (!con)
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to create security context: %s"),
               quote (cur_context));
      यदि (user && context_user_set (con, user))
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to set new user: %s"),
               quote (user));
      यदि (type && context_type_set (con, type))
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to set new type: %s"),
               quote (type));
      यदि (range && context_range_set (con, range))
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to set new range: %s"),
               quote (range));
      यदि (role && context_role_set (con, role))
        error (EXIT_CANCELED, त्रुटि_सं, _("failed to set new role: %s"),
               quote (role));
    }

  यदि (security_check_context (context_str (con)) < 0)
    error (EXIT_CANCELED, त्रुटि_सं, _("invalid context: %s"),
           quote (context_str (con)));

  यदि (setexeccon (context_str (con)) != 0)
    error (EXIT_CANCELED, त्रुटि_सं, _("unable to set security context %s"),
           quote (context_str (con)));
  यदि (cur_context != रिक्त)
    freecon (cur_context);

  (compute_trans ? execv : execvp) (argv[optind], argv + optind);

  पूर्णाङ्क exit_status = त्रुटि_सं == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, त्रुटि_सं, "%s", quote (argv[optind]));
  फर्काऊ exit_status;
}
