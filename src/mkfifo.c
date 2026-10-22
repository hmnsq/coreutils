/* mkfifo -- make fifo's (named pipes)
   Copyright (C) 1990-2026 Free Software Foundation, Inc.

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

/* David MacKenzie <djm@ai.mit.edu>  */

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <selinux/label.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "modechange.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "selinux.h"
#অন্তর্ভাৱ "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkfifo"

#define AUTHORS proper_name ("David MacKenzie")

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, রিক্ত, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]... NAME...\n"), program_name);
      fputs (_("\
Create named pipes (FIFOs) with the given NAMEs.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --mode=MODE\n\
         set file permission bits to MODE, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         set the SELinux security context to স্ৱভাৱ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or যদি CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  ৱর্ণ ধ্রুৱ *specified_mode = রিক্ত;
  ৱর্ণ ধ্রুৱ *scontext = রিক্ত;
  সংরচনা selabel_handle *set_security_context = রিক্ত;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  পূর্ণাঙ্ক optc;
  যেতিয়ালৈ ((optc = getopt_long (argc, argv, "m:Z", longopts, রিক্ত)) != -1)
    {
      ৱিকল্প (optc)
        {
        পক্ষ 'm':
          specified_mode = optarg;
          ভঙ্গ;
        পক্ষ 'Z':
          যদি (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          অন্যথা যদি (is_selinux_enabled () > 0)
            {
              যদি (optarg)
                scontext = optarg;
              অন্যথা
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       রিক্ত, 0);
                  যদি (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          অন্যথা যদি (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ভঙ্গ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  যদি (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  যদি (scontext)
    {
      পূর্ণাঙ্ক ret = 0;
      যদি (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      অন্যথা
        ret = setfscreatecon (scontext);

      যদি (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = MODE_RW_UGO;
  যদি (specified_mode)
    {
      সংরচনা mode_change *change = mode_compile (specified_mode);
      যদি (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_মান = umask (0);
      umask (umask_মান);
      newmode = mode_adjust (newmode, false, umask_মান, change, রিক্ত);
      মুক্ত (change);
      যদি (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  পূর্ণাঙ্ক exit_status = EXIT_SUCCESS;
  বাবে (; optind < argc; ++optind)
    {
      যদি (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      যদি (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
      অন্যথা যদি (specified_mode && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
    }

  ঘূৰাই exit_status;
}
