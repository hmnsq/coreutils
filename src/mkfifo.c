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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "mkfifo"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_ವಾದ, ಖಾಲಿ, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... NAME...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Create named pipes (FIFOs) with the given ಹೆಸರುಗಳು_2.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --ಬಹುಲಕ_2=ಬಹುಲಕ\n\
         ಗಣ_2 file permission ಬಿಟ್ಟುಗಳು_2 ಗಮ್ಯ ಬಹುಲಕ, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         ಗಣ_2 the SELinux security context ಗಮ್ಯ ಪೂರ್ವನಿಯೋಜಿತ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ಆದರೆ CTX is specified then ಗಣ_2 the\n\
         SELinux or SMACK security context ಗಮ್ಯ CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *specified_ಬಹುಲಕ = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *scontext = ಖಾಲಿ;
  ರಚನೆ selabel_handle *ಗಣ_security_context = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಂಕ optc;
  ಆಗ ((optc = getopt_long (argc, argv, "m:Z", longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'm':
          specified_ಬಹುಲಕ = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'Z':
          ಆದರೆ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_selinux_enabled () > 0)
            {
              ಆದರೆ (optarg)
                scontext = optarg;
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಗಣ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       ಖಾಲಿ, 0);
                  ಆದರೆ (! ಗಣ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (scontext)
    {
      ಪೂರ್ಣಾಂಕ ret = 0;
      ಆದರೆ (is_smack_enabled ())
        ret = smack_ಗಣ_label_for_self (scontext);
      ಇಲ್ಲದಿದ್ದರೆ
        ret = setfscreatecon (scontext);

      ಆದರೆ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = ಬಹುಲಕ_RW_UGO;
  ಆದರೆ (specified_ಬಹುಲಕ)
    {
      ರಚನೆ ಬಹುಲಕ_change *change = ಬಹುಲಕ_compile (specified_ಬಹುಲಕ);
      ಆದರೆ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_ಮೌಲ್ಯ = umask (0);
      umask (umask_ಮೌಲ್ಯ);
      newmode = ಬಹುಲಕ_adjust (newmode, false, umask_ಮೌಲ್ಯ, change, ಖಾಲಿ);
      ಬಿಡುಗಡೆ (change);
      ಆದರೆ (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  ಪೂರ್ಣಾಂಕ exit_ಸ್ಥಿತಿ = EXIT_SUCCESS;
  ಗಾಗಿ (; optind < argc; ++optind)
    {
      ಆದರೆ (ಗಣ_security_context)
        defaultcon (ಗಣ_security_context, argv[optind], S_IFIFO);
      ಆದರೆ (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
        }
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (specified_ಬಹುಲಕ && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_ಸ್ಥಿತಿ = EXIT_FAILURE;
        }
    }

  ಹಿಂತಿರುಗಿ exit_ಸ್ಥಿತಿ;
}
