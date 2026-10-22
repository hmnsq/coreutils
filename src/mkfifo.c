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
#تعریف_کړه پروګرام_NAME "mkfifo"

#تعریف_کړه AUTHORS proper_name ("David MacKenzie")

ساكن بنية option ثابت longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_دلیل, عدم, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کړه (_("Usage: %s [OPTION]... NAME...\n"), پروګرام_name);
      fputs (_("\
Create named pipes (FIFOs) with the given نومونه_2.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --ډېرپېښ_2=ډېرپېښ\n\
         سېټ_2 file permission بټونه_2 منزل ډېرپېښ, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         سېټ_2 the SELinux security context منزل افتراضي type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or إذا CTX is specified then سېټ_2 the\n\
         SELinux or SMACK security context منزل CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (پروګرام_NAME);
    }
  وتل (حالت);
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  حرف ثابت *specified_ډېرپېښ = عدم;
  حرف ثابت *scontext = عدم;
  بنية selabel_handle *سېټ_security_context = عدم;

  initialize_main (&argc, &argv);
  سېټ_پروګرام_name (argv[0]);
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
          specified_ډېرپېښ = optarg;
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
                  سېټ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       عدم, 0);
                  إذا (! سېټ_security_context)
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
        case_GETOPT_نسخه_CHAR (پروګرام_NAME, AUTHORS);
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
        ret = smack_سېټ_label_for_self (scontext);
      وإلا
        ret = setfscreatecon (scontext);

      إذا (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = ډېرپېښ_RW_UGO;
  إذا (specified_ډېرپېښ)
    {
      بنية ډېرپېښ_change *change = ډېرپېښ_compile (specified_ډېرپېښ);
      إذا (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_ارزښت = umask (0);
      umask (umask_ارزښت);
      newmode = ډېرپېښ_adjust (newmode, false, umask_ارزښت, change, عدم);
      حرر (change);
      إذا (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  صحيح exit_حالت = EXIT_SUCCESS;
  لكل (; optind < argc; ++optind)
    {
      إذا (سېټ_security_context)
        defaultcon (سېټ_security_context, argv[optind], S_IFIFO);
      إذا (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_حالت = EXIT_FAILURE;
        }
      وإلا إذا (specified_ډېرپېښ && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_حالت = EXIT_FAILURE;
        }
    }

  أرجع exit_حالت;
}
