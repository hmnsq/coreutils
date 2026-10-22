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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <sys/types.h>
#अन्तर्भाव <selinux/label.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "modechange.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "selinux.h"
#अन्तर्भाव "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mkfifo"

#define AUTHORS proper_name ("David MacKenzie")

स्थिर संरचना option ध्रुव longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, रिक्त, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  सचे (status != EXIT_SUCCESS)
    emit_try_help ();
  अञ्ञथा
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
         set the SELinux security context to स्वभाव type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or सचे CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण ध्रुव *specified_mode = रिक्त;
  वर्ण ध्रुव *scontext = रिक्त;
  संरचना selabel_handle *set_security_context = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  पूर्णाङ्क optc;
  याव ((optc = getopt_long (argc, argv, "m:Z", longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'm':
          specified_mode = optarg;
          भङ्ग;
        पक्ष 'Z':
          सचे (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          अञ्ञथा सचे (is_selinux_enabled () > 0)
            {
              सचे (optarg)
                scontext = optarg;
              अञ्ञथा
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       रिक्त, 0);
                  सचे (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          अञ्ञथा सचे (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  सचे (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  सचे (scontext)
    {
      पूर्णाङ्क ret = 0;
      सचे (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      अञ्ञथा
        ret = setfscreatecon (scontext);

      सचे (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = MODE_RW_UGO;
  सचे (specified_mode)
    {
      संरचना mode_change *change = mode_compile (specified_mode);
      सचे (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_मानम् = umask (0);
      umask (umask_मानम्);
      newmode = mode_adjust (newmode, false, umask_मानम्, change, रिक्त);
      मुक्त (change);
      सचे (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  पूर्णाङ्क exit_status = EXIT_SUCCESS;
  अत्थाय (; optind < argc; ++optind)
    {
      सचे (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      सचे (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
      अञ्ञथा सचे (specified_mode && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_status = EXIT_FAILURE;
        }
    }

  पटिनिवत्त exit_status;
}
