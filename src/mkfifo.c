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

#شامل <config.h>
#شامل <stdio.h>
#شامل <getopt.h>
#شامل <sys/types.h>
#شامل <selinux/label.h>

#شامل "system.h"
#شامل "modechange.h"
#شامل "quote.h"
#شامل "selinux.h"
#شامل "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkfifo"

#define AUTHORS proper_name ("David MacKenzie")

ایستا ساختار option ثابت longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, پوچ, 'm'},
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
         set the SELinux security context to پیشفرض type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or اگر CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  نویسه ثابت *specified_mode = پوچ;
  نویسه ثابت *scontext = پوچ;
  ساختار selabel_handle *set_security_context = پوچ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحیح optc;
  وقتی ((optc = getopt_long (argc, argv, "m:Z", longopts, پوچ)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'm':
          specified_mode = optarg;
          بشکن;
        حالت 'Z':
          اگر (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          وگرنه اگر (is_selinux_enabled () > 0)
            {
              اگر (optarg)
                scontext = optarg;
              وگرنه
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       پوچ, 0);
                  اگر (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          وگرنه اگر (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          بشکن;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  اگر (scontext)
    {
      صحیح ret = 0;
      اگر (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      وگرنه
        ret = setfscreatecon (scontext);

      اگر (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = MODE_RW_UGO;
  اگر (specified_mode)
    {
      ساختار mode_change *change = mode_compile (specified_mode);
      اگر (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_مقدار = umask (0);
      umask (umask_مقدار);
      newmode = mode_adjust (newmode, false, umask_مقدار, change, پوچ);
      آزاد (change);
      اگر (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  صحیح exit_status = EXIT_SUCCESS;
  برای (; optind < argc; ++optind)
    {
      اگر (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      اگر (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
      وگرنه اگر (specified_mode && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
    }

  بازگشت exit_status;
}
