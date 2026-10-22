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
#تعریف_کرو پروگرام_NAME "mkfifo"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

ساكن بنية option ثابت longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_دلیل, عدم, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [OPTION]... NAME...\n"), پروگرام_name);
      fputs (_("\
Create named pipes (FIFOs) with the given ناں_2.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --منوال_2=منوال\n\
         سیٹ_2 file permission بٹ_2 منزل منوال, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         سیٹ_2 the SELinux security context منزل افتراضي type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or إذا CTX is specified then سیٹ_2 the\n\
         SELinux or SMACK security context منزل CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  حرف ثابت *specified_منوال = عدم;
  حرف ثابت *scontext = عدم;
  بنية selabel_handle *سیٹ_security_context = عدم;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
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
          specified_منوال = optarg;
          اكسر;
        حالة 'Z':
          إذا (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          وإلا إذا (is_selinux_enabled () > 0)
            {
              إذا (optarg)
                scontext = optarg;
              وإلا
                {
                  سیٹ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       عدم, 0);
                  إذا (! سیٹ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          وإلا إذا (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  إذا (scontext)
    {
      صحيح ret = 0;
      إذا (is_smack_enabled ())
        ret = smack_سیٹ_label_for_self (scontext);
      وإلا
        ret = setfscreatecon (scontext);

      إذا (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = منوال_RW_UGO;
  إذا (specified_منوال)
    {
      بنية منوال_change *change = منوال_compile (specified_منوال);
      إذا (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_قدر = umask (0);
      umask (umask_قدر);
      newmode = منوال_adjust (newmode, false, umask_قدر, change, عدم);
      حرر (change);
      إذا (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  صحيح exit_حالت = EXIT_SUCCESS;
  لكل (; optind < argc; ++optind)
    {
      إذا (سیٹ_security_context)
        defaultcon (سیٹ_security_context, argv[optind], S_IFIFO);
      إذا (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_حالت = EXIT_FAILURE;
        }
      وإلا إذا (specified_منوال && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_حالت = EXIT_FAILURE;
        }
    }

  أرجع exit_حالت;
}
