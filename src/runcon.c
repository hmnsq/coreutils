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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/selinux.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/context.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "runcon"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Russell Coker")

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"role", required_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"type", required_ਦਲੀਲ, ਨਿਰਾ, 't'},
  {"user", required_ਦਲੀਲ, ਨਿਰਾ, 'u'},
  {"range", required_ਦਲੀਲ, ਨਿਰਾ, 'l'},
  {"compute", no_ਦਲੀਲ, ਨਿਰਾ, 'c'},
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
      ਲਿਖੋ (_("\
Usage: %s [CONTEXT COMMAND [ARG]...]\n\
  or:  %s [-c] [-u USER] [-r ROLE] [-t TYPE] [-l RANGE] COMMAND [ARG]...\n\
"), ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
ਲੜੀਵਾਰ a ਪ੍ਰੋਗਰਾਮ ਵਿੱਚ a different SELinux security context.\n\
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
  -t, --type=TYPE    type (ਲਈ same role as ਮਾਪਾ)\n\
"));
      oputs (_("\
  -u, --user=USER    user ਪਛਾਣ_ਤੱਤ\n\
"));
      oputs (_("\
  -r, --role=ROLE    role\n\
"));
      oputs (_("\
  -l, --range=RANGE  levelrange\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_exec_ਸਥਿਤੀ (ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ *role = ਨਿਰਾ;
  ਅੱਖਰ *range = ਨਿਰਾ;
  ਅੱਖਰ *user = ਨਿਰਾ;
  ਅੱਖਰ *type = ਨਿਰਾ;
  ਅੱਖਰ *context = ਨਿਰਾ;
  ਅੱਖਰ *cur_context = ਨਿਰਾ;
  ਅੱਖਰ *file_context = ਨਿਰਾ;
  ਅੱਖਰ *new_context = ਨਿਰਾ;
  bool compute_trans = false;

  context_t con;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  ਜਦੋਂ (true)
    {
      ਪੂਰਨ option_ਸੂਚਕਾਂਕ = 0;
      ਪੂਰਨ c = getopt_long (argc, argv, "+r:t:u:l:c", long_options,
                           &option_ਸੂਚਕਾਂਕ);
      ਜੇ (c == -1)
        ਰੋਕੋ;
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'r':
          ਜੇ (role)
            error (EXIT_CANCELED, 0, _("multiple roles"));
          role = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 't':
          ਜੇ (type)
            error (EXIT_CANCELED, 0, _("multiple types"));
          type = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'u':
          ਜੇ (user)
            error (EXIT_CANCELED, 0, _("multiple users"));
          user = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'l':
          ਜੇ (range)
            error (EXIT_CANCELED, 0, _("multiple levelranges"));
          range = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'c':
          compute_trans = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_CANCELED);
          ਰੋਕੋ;
        }
    }

  ਜੇ (argc - optind == 0)
    {
      ਜੇ (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));
      fputs (cur_context, stdout);
      fputc ('\n', stdout);
      ਵਾਪਸ EXIT_SUCCESS;
    }

  ਜੇ (!(user || role || type || range || compute_trans))
    {
      ਜੇ (optind >= argc)
        {
          error (0, 0, _("you must specify -c, -t, -u, -l, -r, or context"));
          usage (EXIT_CANCELED);
        }
      context = argv[optind++];
    }

  ਜੇ (optind >= argc)
    {
      error (0, 0, _("no command specified"));
      usage (EXIT_CANCELED);
    }

  ਜੇ (is_selinux_enabled () != 1)
    error (EXIT_CANCELED, 0, _("%s may be used only on a SELinux kernel"),
           ਪ੍ਰੋਗਰਾਮ_name);

  ਜੇ (context)
    {
      con = context_new (context);
      ਜੇ (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (context));
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));

      /* We will generate context based on process transition */
      ਜੇ (compute_trans)
        {
          /* Get context of file to be executed */
          ਜੇ (getfilecon (argv[optind], &file_context) == -1)
            error (EXIT_CANCELED, errno,
                   _("failed to get security context of %s"),
                   quoteaf (argv[optind]));
          /* compute result of process transition */
          ਜੇ (security_compute_create (cur_context, file_context,
                                       string_ਮੰਜ਼ਿਲ_security_class ("process"),
                                       &new_context) != 0)
            error (EXIT_CANCELED, errno, _("failed to compute a new context"));
          /* free contexts */
          freecon (file_context);
          freecon (cur_context);

          /* set cur_context equal to new_context */
          cur_context = new_context;
        }

      con = context_new (cur_context);
      ਜੇ (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (cur_context));
      ਜੇ (user && context_user_ਸੈੱਟ (con, user))
        error (EXIT_CANCELED, errno, _("failed to set new user: %s"),
               quote (user));
      ਜੇ (type && context_type_ਸੈੱਟ (con, type))
        error (EXIT_CANCELED, errno, _("failed to set new type: %s"),
               quote (type));
      ਜੇ (range && context_range_ਸੈੱਟ (con, range))
        error (EXIT_CANCELED, errno, _("failed to set new range: %s"),
               quote (range));
      ਜੇ (role && context_role_ਸੈੱਟ (con, role))
        error (EXIT_CANCELED, errno, _("failed to set new role: %s"),
               quote (role));
    }

  ਜੇ (security_check_context (context_str (con)) < 0)
    error (EXIT_CANCELED, errno, _("invalid context: %s"),
           quote (context_str (con)));

  ਜੇ (setexeccon (context_str (con)) != 0)
    error (EXIT_CANCELED, errno, _("unable to set security context %s"),
           quote (context_str (con)));
  ਜੇ (cur_context != ਨਿਰਾ)
    freecon (cur_context);

  (compute_trans ? execv : execvp) (argv[optind], argv + optind);

  ਪੂਰਨ exit_ਸਥਿਤੀ = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[optind]));
  ਵਾਪਸ exit_ਸਥਿਤੀ;
}
