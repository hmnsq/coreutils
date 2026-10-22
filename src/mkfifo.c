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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <selinux/label.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "modechange.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "selinux.h"
#ಅನ್ತರ್ಭಾವ "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkfifo"

#define AUTHORS proper_name ("David MacKenzie")

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, ರಿಕ್ತ, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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
         set the SELinux security context to ಸ್ವಭಾವ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ಯದಿ CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ವರ್ಣ ಧ್ರುವ *specified_mode = ರಿಕ್ತ;
  ವರ್ಣ ಧ್ರುವ *scontext = ರಿಕ್ತ;
  ಸಂರಚನಾ selabel_handle *set_security_context = ರಿಕ್ತ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಙ್ಕ optc;
  ಯಾವತ್ ((optc = getopt_long (argc, argv, "m:Z", longopts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'm':
          specified_mode = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'Z':
          ಯದಿ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ಅನ್ಯಥಾ ಯದಿ (is_selinux_enabled () > 0)
            {
              ಯದಿ (optarg)
                scontext = optarg;
              ಅನ್ಯಥಾ
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       ರಿಕ್ತ, 0);
                  ಯದಿ (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ಅನ್ಯಥಾ ಯದಿ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  ಯದಿ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (scontext)
    {
      ಪೂರ್ಣಾಙ್ಕ ret = 0;
      ಯದಿ (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      ಅನ್ಯಥಾ
        ret = setfscreatecon (scontext);

      ಯದಿ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = MODE_RW_UGO;
  ಯದಿ (specified_mode)
    {
      ಸಂರಚನಾ mode_change *change = mode_compile (specified_mode);
      ಯದಿ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_ಮೌಲ್ಯ = umask (0);
      umask (umask_ಮೌಲ್ಯ);
      newmode = mode_adjust (newmode, false, umask_ಮೌಲ್ಯ, change, ರಿಕ್ತ);
      ಮುಕ್ತ (change);
      ಯದಿ (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  ಪೂರ್ಣಾಙ್ಕ exit_status = EXIT_SUCCESS;
  ಪ್ರತಿ (; optind < argc; ++optind)
    {
      ಯದಿ (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      ಯದಿ (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
      ಅನ್ಯಥಾ ಯದಿ (specified_mode && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
    }

  ಪ್ರತ್ಯಾಗಮ exit_status;
}
