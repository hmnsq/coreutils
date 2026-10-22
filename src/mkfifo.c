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

#include <config.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <selinux/label.h>

#include "system.h"
#include "modechange.h"
#include "quote.h"
#include "selinux.h"
#include "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkfifo"

#define AUTHORS proper_name ("David MacKenzie")

static sa_nracanaa option const longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, NULL, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (puur_naa_nka status)
{
  ᤀᤡ᤺ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᤀᤡ᤺ᤆᤡ
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
         set the SELinux security context to default type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ᤀᤡ᤺ CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  var_na const *specified_mode = NULL;
  var_na const *scontext = NULL;
  sa_nracanaa selabel_handle *set_security_context = NULL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  puur_naa_nka optc;
  ᤀᤠ᤺ ((optc = getopt_long (argc, argv, "m:Z", longopts, NULL)) != -1)
    {
      switch (optc)
        {
        pak_sa 'm':
          specified_mode = optarg;
          bha_nga;
        pak_sa 'Z':
          ᤀᤡ᤺ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (is_selinux_enabled () > 0)
            {
              ᤀᤡ᤺ (optarg)
                scontext = optarg;
              ᤀᤡ᤺ᤆᤡ
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       NULL, 0);
                  ᤀᤡ᤺ (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          bha_nga;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  ᤀᤡ᤺ (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ᤀᤡ᤺ (scontext)
    {
      puur_naa_nka ret = 0;
      ᤀᤡ᤺ (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      ᤀᤡ᤺ᤆᤡ
        ret = setfscreatecon (scontext);

      ᤀᤡ᤺ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = MODE_RW_UGO;
  ᤀᤡ᤺ (specified_mode)
    {
      sa_nracanaa mode_change *change = mode_compile (specified_mode);
      ᤀᤡ᤺ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_value = umask (0);
      umask (umask_value);
      newmode = mode_adjust (newmode, false, umask_value, change, NULL);
      free (change);
      ᤀᤡ᤺ (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  puur_naa_nka exit_status = EXIT_SUCCESS;
  for (; optind < argc; ++optind)
    {
      ᤀᤡ᤺ (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      ᤀᤡ᤺ (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
      ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (specified_mode && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
    }

  ᤀᤡᤴ exit_status;
}
