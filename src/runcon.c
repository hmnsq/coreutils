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
#تعریف_کرو پروگرام_NAME "runcon"

#تعریف_کرو AUTHORS proper_name ("Russell Coker")

ساکن ساخت option ثابت long_options[] =
{
  {"role", required_دلیل, تہی, 'r'},
  {"type", required_دلیل, تہی, 't'},
  {"user", required_دلیل, تہی, 'u'},
  {"range", required_دلیل, تہی, 'l'},
  {"compute", no_دلیل, تہی, 'c'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [CONTEXT COMMAND [ARG]...]\n\
  or:  %s [-c] [-u USER] [-r ROLE] [-t TYPE] [-l RANGE] COMMAND [ARG]...\n\
"), پروگرام_name, پروگرام_name);
      fputs (_("\
تسلسل a پروگرام اندر a different SELinux security context.\n\
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
  -t, --type=TYPE    type (برائے same role as والد)\n\
"));
      oputs (_("\
  -u, --user=USER    user شناختی_عنصر\n\
"));
      oputs (_("\
  -r, --role=ROLE    role\n\
"));
      oputs (_("\
  -l, --range=RANGE  levelrange\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_exec_حالت (پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  حرف *role = تہی;
  حرف *range = تہی;
  حرف *user = تہی;
  حرف *type = تہی;
  حرف *context = تہی;
  حرف *cur_context = تہی;
  حرف *file_context = تہی;
  حرف *new_context = تہی;
  bool compute_trans = false;

  context_t con;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  initialize_exit_failure (EXIT_CANCELED);
  atexit (close_stdout);

  جب (true)
    {
      صحیح option_اشاریہ = 0;
      صحیح c = getopt_long (argc, argv, "+r:t:u:l:c", long_options,
                           &option_اشاریہ);
      اگر (c == -1)
        توڑو;
      انتخاب (c)
        {
        حالت 'r':
          اگر (role)
            error (EXIT_CANCELED, 0, _("multiple roles"));
          role = optarg;
          توڑو;
        حالت 't':
          اگر (type)
            error (EXIT_CANCELED, 0, _("multiple types"));
          type = optarg;
          توڑو;
        حالت 'u':
          اگر (user)
            error (EXIT_CANCELED, 0, _("multiple users"));
          user = optarg;
          توڑو;
        حالت 'l':
          اگر (range)
            error (EXIT_CANCELED, 0, _("multiple levelranges"));
          range = optarg;
          توڑو;
        حالت 'c':
          compute_trans = true;
          توڑو;

        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
          usage (EXIT_CANCELED);
          توڑو;
        }
    }

  اگر (argc - optind == 0)
    {
      اگر (getcon (&cur_context) < 0)
        error (EXIT_CANCELED, errno, _("failed to get current context"));
      fputs (cur_context, stdout);
      fputc ('\n', stdout);
      واپس EXIT_SUCCESS;
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
           پروگرام_name);

  اگر (context)
    {
      con = context_new (context);
      اگر (!con)
        error (EXIT_CANCELED, errno, _("failed to create security context: %s"),
               quote (context));
    }
  ورنہ
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
                                       string_مقصد_security_class ("process"),
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
      اگر (user && context_user_سیٹ (con, user))
        error (EXIT_CANCELED, errno, _("failed to set new user: %s"),
               quote (user));
      اگر (type && context_type_سیٹ (con, type))
        error (EXIT_CANCELED, errno, _("failed to set new type: %s"),
               quote (type));
      اگر (range && context_range_سیٹ (con, range))
        error (EXIT_CANCELED, errno, _("failed to set new range: %s"),
               quote (range));
      اگر (role && context_role_سیٹ (con, role))
        error (EXIT_CANCELED, errno, _("failed to set new role: %s"),
               quote (role));
    }

  اگر (security_check_context (context_str (con)) < 0)
    error (EXIT_CANCELED, errno, _("invalid context: %s"),
           quote (context_str (con)));

  اگر (setexeccon (context_str (con)) != 0)
    error (EXIT_CANCELED, errno, _("unable to set security context %s"),
           quote (context_str (con)));
  اگر (cur_context != تہی)
    freecon (cur_context);

  (compute_trans ? execv : execvp) (argv[optind], argv + optind);

  صحیح exit_حالت = errno == ENOENT ? EXIT_ENOENT : EXIT_CANNOT_INVOKE;
  error (0, errno, "%s", quote (argv[optind]));
  واپس exit_حالت;
}
