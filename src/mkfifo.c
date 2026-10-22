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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن <selinux/label.h>

#ضمن "system.h"
#ضمن "modechange.h"
#ضمن "quote.h"
#ضمن "selinux.h"
#ضمن "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkfifo"

#define AUTHORS proper_name ("David MacKenzie")

ساكن بنية option ثابت longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, عدم, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  𐡄𐡍 (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
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
         set the SELinux security context to افتراضي type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or 𐡄𐡍 CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحيح
main (صحيح argc, حرف **argv)
{
  حرف ثابت *specified_mode = عدم;
  حرف ثابت *scontext = عدم;
  بنية selabel_handle *set_security_context = عدم;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحيح optc;
  طالما ((optc = getopt_long (argc, argv, "m:Z", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'm':
          specified_mode = optarg;
          اكسر;
        حالة 'Z':
          𐡄𐡍 (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          وإلا 𐡄𐡍 (is_selinux_enabled () > 0)
            {
              𐡄𐡍 (optarg)
                scontext = optarg;
              وإلا
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       عدم, 0);
                  𐡄𐡍 (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          وإلا 𐡄𐡍 (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  𐡄𐡍 (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  𐡄𐡍 (scontext)
    {
      صحيح ret = 0;
      𐡄𐡍 (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      وإلا
        ret = setfscreatecon (scontext);

      𐡄𐡍 (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = MODE_RW_UGO;
  𐡄𐡍 (specified_mode)
    {
      بنية mode_change *change = mode_compile (specified_mode);
      𐡄𐡍 (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_value = umask (0);
      umask (umask_value);
      newmode = mode_adjust (newmode, false, umask_value, change, عدم);
      حرر (change);
      𐡄𐡍 (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  صحيح exit_status = EXIT_SUCCESS;
  لكل (; optind < argc; ++optind)
    {
      𐡄𐡍 (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      𐡄𐡍 (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
      وإلا 𐡄𐡍 (specified_mode && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
    }

  أرجع exit_status;
}
