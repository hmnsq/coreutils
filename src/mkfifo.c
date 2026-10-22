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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <selinux/label.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "modechange.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "selinux.h"
#అన్తర్భావ "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "mkfifo"

#నిర్వచించు AUTHORS proper_name ("David MacKenzie")

స్థిర నిర్మాణం option స్థిరరాశి longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_వాదం, ఖాళీ, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION]... NAME...\n"), కార్యక్రమం_name);
      fputs (_("\
Create named pipes (FIFOs) with the given పేర్లు_2.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --బహుళకం_2=బహుళకం\n\
         సమితి_2 file permission బిట్లు_2 గమ్యం బహుళకం, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         సమితి_2 the SELinux security context గమ్యం అప్రమేయ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ఒకవేళ CTX is specified then సమితి_2 the\n\
         SELinux or SMACK security context గమ్యం CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  అక్షరం స్థిరరాశి *specified_బహుళకం = ఖాళీ;
  అక్షరం స్థిరరాశి *scontext = ఖాళీ;
  నిర్మాణం selabel_handle *సమితి_security_context = ఖాళీ;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  పూర్ణాంకం optc;
  అయితే ((optc = getopt_long (argc, argv, "m:Z", longopts, ఖాళీ)) != -1)
    {
      ఎంపిక (optc)
        {
        స్థితి 'm':
          specified_బహుళకం = optarg;
          ఆపు;
        స్థితి 'Z':
          ఒకవేళ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          లేకపోతే ఒకవేళ (is_selinux_enabled () > 0)
            {
              ఒకవేళ (optarg)
                scontext = optarg;
              లేకపోతే
                {
                  సమితి_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       ఖాళీ, 0);
                  ఒకవేళ (! సమితి_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          లేకపోతే ఒకవేళ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ఆపు;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  ఒకవేళ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ఒకవేళ (scontext)
    {
      పూర్ణాంకం ret = 0;
      ఒకవేళ (is_smack_enabled ())
        ret = smack_సమితి_label_for_self (scontext);
      లేకపోతే
        ret = setfscreatecon (scontext);

      ఒకవేళ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = బహుళకం_RW_UGO;
  ఒకవేళ (specified_బహుళకం)
    {
      నిర్మాణం బహుళకం_change *change = బహుళకం_compile (specified_బహుళకం);
      ఒకవేళ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_విలువ = umask (0);
      umask (umask_విలువ);
      newmode = బహుళకం_adjust (newmode, false, umask_విలువ, change, ఖాళీ);
      విడుదల (change);
      ఒకవేళ (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  పూర్ణాంకం exit_స్థితి = EXIT_SUCCESS;
  కొరకు (; optind < argc; ++optind)
    {
      ఒకవేళ (సమితి_security_context)
        defaultcon (సమితి_security_context, argv[optind], S_IFIFO);
      ఒకవేళ (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_స్థితి = EXIT_FAILURE;
        }
      లేకపోతే ఒకవేళ (specified_బహుళకం && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_స్థితి = EXIT_FAILURE;
        }
    }

  తిరిగి exit_స్థితి;
}
