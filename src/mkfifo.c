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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/label.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "modechange.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "selinux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "mkfifo"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_ਦਲੀਲ, ਨਿਰਾ, 'm'},
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
      ਲਿਖੋ (_("Usage: %s [OPTION]... NAME...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Create named pipes (FIFOs) with the given ਨਾਂ_2.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --ਬਹੁਲਕ_2=ਬਹੁਲਕ\n\
         ਸੈੱਟ_2 file permission ਬਿੱਟ_2 ਮੰਜ਼ਿਲ ਬਹੁਲਕ, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         ਸੈੱਟ_2 the SELinux security context ਮੰਜ਼ਿਲ ਮੂਲ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ਜੇ CTX is specified then ਸੈੱਟ_2 the\n\
         SELinux or SMACK security context ਮੰਜ਼ਿਲ CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ ਅਚਲ *specified_ਬਹੁਲਕ = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *scontext = ਨਿਰਾ;
  ਬਣਤਰ selabel_handle *ਸੈੱਟ_security_context = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਪੂਰਨ optc;
  ਜਦੋਂ ((optc = getopt_long (argc, argv, "m:Z", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'm':
          specified_ਬਹੁਲਕ = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'Z':
          ਜੇ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ਨਹੀਂਤਾਂ ਜੇ (is_selinux_enabled () > 0)
            {
              ਜੇ (optarg)
                scontext = optarg;
              ਨਹੀਂਤਾਂ
                {
                  ਸੈੱਟ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       ਨਿਰਾ, 0);
                  ਜੇ (! ਸੈੱਟ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ਨਹੀਂਤਾਂ ਜੇ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ਜੇ (scontext)
    {
      ਪੂਰਨ ret = 0;
      ਜੇ (is_smack_enabled ())
        ret = smack_ਸੈੱਟ_label_for_self (scontext);
      ਨਹੀਂਤਾਂ
        ret = setfscreatecon (scontext);

      ਜੇ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = ਬਹੁਲਕ_RW_UGO;
  ਜੇ (specified_ਬਹੁਲਕ)
    {
      ਬਣਤਰ ਬਹੁਲਕ_change *change = ਬਹੁਲਕ_compile (specified_ਬਹੁਲਕ);
      ਜੇ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_ਮੁੱਲ = umask (0);
      umask (umask_ਮੁੱਲ);
      newmode = ਬਹੁਲਕ_adjust (newmode, false, umask_ਮੁੱਲ, change, ਨਿਰਾ);
      ਮੁਕਤ (change);
      ਜੇ (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  ਪੂਰਨ exit_ਸਥਿਤੀ = EXIT_SUCCESS;
  ਲਈ (; optind < argc; ++optind)
    {
      ਜੇ (ਸੈੱਟ_security_context)
        defaultcon (ਸੈੱਟ_security_context, argv[optind], S_IFIFO);
      ਜੇ (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_ਸਥਿਤੀ = EXIT_FAILURE;
        }
      ਨਹੀਂਤਾਂ ਜੇ (specified_ਬਹੁਲਕ && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_ਸਥਿਤੀ = EXIT_FAILURE;
        }
    }

  ਵਾਪਸ exit_ਸਥਿਤੀ;
}
