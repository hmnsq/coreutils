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
#تعریف_کرو پروگرام_NAME "mkfifo"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

ساکن ساخت option ثابت longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_دلیل, تہی, 'm'},
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
      لکھو (_("Usage: %s [OPTION]... NAME...\n"), پروگرام_name);
      fputs (_("\
Create named pipes (FIFOs) with the given نام_2.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --منوال_2=منوال\n\
         سیٹ_2 file permission بٹ_2 مقصد منوال, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         سیٹ_2 the SELinux security context مقصد طےشدہ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or اگر CTX is specified then سیٹ_2 the\n\
         SELinux or SMACK security context مقصد CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  حرف ثابت *specified_منوال = تہی;
  حرف ثابت *scontext = تہی;
  ساخت selabel_handle *سیٹ_security_context = تہی;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحیح optc;
  جب ((optc = getopt_long (argc, argv, "m:Z", longopts, تہی)) != -1)
    {
      انتخاب (optc)
        {
        حالت 'm':
          specified_منوال = optarg;
          توڑو;
        حالت 'Z':
          اگر (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ورنہ اگر (is_selinux_enabled () > 0)
            {
              اگر (optarg)
                scontext = optarg;
              ورنہ
                {
                  سیٹ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       تہی, 0);
                  اگر (! سیٹ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ورنہ اگر (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          توڑو;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        طےشدہ:
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
        ret = smack_سیٹ_label_for_self (scontext);
      ورنہ
        ret = setfscreatecon (scontext);

      اگر (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = منوال_RW_UGO;
  اگر (specified_منوال)
    {
      ساخت منوال_change *change = منوال_compile (specified_منوال);
      اگر (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_قدر = umask (0);
      umask (umask_قدر);
      newmode = منوال_adjust (newmode, false, umask_قدر, change, تہی);
      آزاد (change);
      اگر (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  صحیح exit_حالت = EXIT_SUCCESS;
  برائے (; optind < argc; ++optind)
    {
      اگر (سیٹ_security_context)
        defaultcon (سیٹ_security_context, argv[optind], S_IFIFO);
      اگر (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_حالت = EXIT_FAILURE;
        }
      ورنہ اگر (specified_منوال && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_حالت = EXIT_FAILURE;
        }
    }

  واپس exit_حالت;
}
