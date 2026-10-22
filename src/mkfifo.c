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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <selinux/label.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "modechange.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "selinux.h"
#ଅନ୍ତର୍ଭାଵ "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkfifo"

#define AUTHORS proper_name ("David MacKenzie")

ସ୍ଥିର ସଂରଚନା option ଅଚଳ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, ଖାଲି, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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
         set the SELinux security context to ପୂର୍ବନିର୍ଦ୍ଧାରିତ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ଯଦି CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ଅକ୍ଷର ଅଚଳ *specified_mode = ଖାଲି;
  ଅକ୍ଷର ଅଚଳ *scontext = ଖାଲି;
  ସଂରଚନା selabel_handle *set_security_context = ଖାଲି;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "m:Z", longopts, ଖାଲି)) != -1)
    {
      ବାଛ (optc)
        {
        ସ୍ଥିତି 'm':
          specified_mode = optarg;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'Z':
          ଯଦି (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ଅନ୍ୟଥା ଯଦି (is_selinux_enabled () > 0)
            {
              ଯଦି (optarg)
                scontext = optarg;
              ଅନ୍ୟଥା
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       ଖାଲି, 0);
                  ଯଦି (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ଅନ୍ୟଥା ଯଦି (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ଭାଙ୍ଗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  ଯଦି (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  ଯଦି (scontext)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
      ଯଦି (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      ଅନ୍ୟଥା
        ret = setfscreatecon (scontext);

      ଯଦି (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = MODE_RW_UGO;
  ଯଦି (specified_mode)
    {
      ସଂରଚନା mode_change *change = mode_compile (specified_mode);
      ଯଦି (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_ମୂଲ୍ୟ = umask (0);
      umask (umask_ମୂଲ୍ୟ);
      newmode = mode_adjust (newmode, false, umask_ମୂଲ୍ୟ, change, ଖାଲି);
      ମୁକ୍ତ (change);
      ଯଦି (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exit_status = EXIT_SUCCESS;
  ପାଇଁ (; optind < argc; ++optind)
    {
      ଯଦି (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      ଯଦି (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
      ଅନ୍ୟଥା ଯଦି (specified_mode && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
    }

  ଫେରନ୍ତୁ exit_status;
}
