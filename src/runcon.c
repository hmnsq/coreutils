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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <selinux/selinux.h>
#അന്തര്ഭാവ <selinux/context.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "runcon"

#define AUTHORS proper_name ("Russell Coker")

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"role", required_argument, ശൂന്യ, 'r'},
  {"type", required_argument, ശൂന്യ, 't'},
  {"user", required_argument, ശൂന്യ, 'u'},
  {"range", required_argument, ശൂന്യ, 'l'},
  {"compute", no_argument, ശൂന്യ, 'c'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
  -t, --type=TYPE    type (വേണ്ടി same role as parent)\n\
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  അക്ഷരം *role = ശൂന്യ;
  അക്ഷരം *range = ശൂന്യ;
  അക്ഷരം *user = ശൂന്യ;
  അക്ഷരം *type = ശൂന്യ;
  അക്ഷരം *context = ശൂന്യ;
  അക്ഷരം *cur_context = ശൂന്യ;
  അക്ഷരം *file_context = ശൂന്യ;
  അക്ഷരം *new_context = ശൂന്യ;
  bool compute_trans = false;

  context_t con;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ആകുമ്പോൾ (true)
    {
      പൂർണ്ണസംഖ്യ option_index = 0;
      പൂർണ്ണസംഖ്യ c = getopt_long (argc, argv, "+r:t:u:l:c", long_options,
                           &option_index);
      എങ്കിൽ (c == -1)
        നിർത്തുക;
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'r':
          എങ്കിൽ (role)
            error (EXIT_CANCELED, 0, _("multiple roles"));
          role = optarg;
          നിർത്തുക;
        അവസ്ഥ 't':
          എങ്കിൽ (type)
            error (EXIT_CANCELED, 0, _("multiple types"));
          type = optarg;
          നിർത്തുക;
        അവസ്ഥ 'u':
          എങ്കിൽ (user)
            error (EXIT_CANCELED, 0, _("multiple users"));
          user = optarg;
          നിർത്തുക;
        അവസ്ഥ 'l':
          എങ്കിൽ (range)
            error (EXIT_CANCELED, 0, _("multiple levelranges"));
          range = optarg;
          നിർത്തുക;
        അവസ്ഥ 'c':
          compute_trans = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_CANCELED);
          നിർത്തുക;
        }
    }

  എങ്കിൽ (argc - optind == 0)
    {
      എങ്കിൽ (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));
      fputs (cur_context, stdout);
      fputc ('\n', stdout);
      തിരികെ EXIT_SUCCESS;
    }

  എങ്കിൽ (!(user || role || type || range || compute_trans))
    {
      എങ്കിൽ (optind >= argc)
        {
          error (0, 0, _("you must specify -c, -t, -u, -l, -r, or context"));
          usage (EXIT_CANCELED);
        }
      context = argv[optind++];
    }

  എങ്കിൽ (optind >= argc)
    {
      error (0, 0, _("no command specified"));
      usage (EXIT_CANCELED);
    }

  എങ്കിൽ (is_selinux_enabled () != 1)
    error (EXIT_CANCELED, 0, _("%s may be used only on a SELinux kernel"),
           program_name);

  എങ്കിൽ (context)
    {
      con = context_new (context);
      എങ്കിൽ (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (context));
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));

      /* We will generate context based on process transition */
      എങ്കിൽ (compute_trans)
        {
          /* Get context of file to be executed */
          എങ്കിൽ (getfilecon (argv[optind], &file_context) == -1)
            error (EXIT_CANCELED, errno,
                   _("failed to get security context of %s"),
                   quoteaf (argv[optind]));
          /* compute result of process transition */
          എങ്കിൽ (security_compute_create (cur_context, file_context,
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
      എങ്കിൽ (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (cur_context));
      എങ്കിൽ (user && context_user_set (con, user))
        error (EXIT_CANCELED, errno, _("failed to set new user: %s"),
               quote (user));
      എങ്കിൽ (type && context_type_set (con, type))
        error (EXIT_CANCELED, errno, _("failed to set new type: %s"),
               quote (type));
      എങ്കിൽ (range && context_range_set (con, range))
        error (EXIT_CANCELED, errno, _("failed to set new range: %s"),
               quote (range));
      എങ്കിൽ (role && context_role_set (con, role))
        error (EXIT_CANCELED, errno, _("failed to set new role: %s"),
               quote (role));
    }

  എങ്കിൽ (security_check_context (context_str (con)) < 0)
    error (EXIT_CANCELED, errno, _("invalid context: %s"),
           quote (context_str (con)));

  എങ്കിൽ (setexeccon (context_str (con)) != 0)
    error (EXIT_CANCELED, errno, _("unable to set security context %s"),
           quote (context_str (con)));
  എങ്കിൽ (cur_context != ശൂന്യ)
    freecon (cur_context);

  (compute_trans ? execv : execvp) (argv[optind], argv + optind);

  പൂർണ്ണസംഖ്യ exit_status = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[optind]));
  തിരികെ exit_status;
}
