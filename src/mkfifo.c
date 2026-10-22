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
#परिभाषय क्रमादेशः_NAME "mkfifo"

#परिभाषय AUTHORS proper_name ("David MacKenzie")

स्थिर संरचना option ध्रुव longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_तर्कः, रिक्त, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION]... NAME...\n"), क्रमादेशः_name);
      fputs (_("\
Create named pipes (FIFOs) with the given नामानि_2.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --बहुलकम्_2=बहुलकम्\n\
         समुच्चयः_2 file permission द्वयङ्काः_2 गन्तव्यम् बहुलकम्, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         समुच्चयः_2 the SELinux security context गन्तव्यम् स्वभाव type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or यदि CTX is specified then समुच्चयः_2 the\n\
         SELinux or SMACK security context गन्तव्यम् CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण ध्रुव *specified_बहुलकम् = रिक्त;
  वर्ण ध्रुव *scontext = रिक्त;
  संरचना selabel_handle *समुच्चयः_security_context = रिक्त;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  पूर्णाङ्क optc;
  यावत् ((optc = getopt_long (argc, argv, "m:Z", longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'm':
          specified_बहुलकम् = optarg;
          भङ्ग;
        पक्ष 'Z':
          यदि (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          अन्यथा यदि (is_selinux_enabled () > 0)
            {
              यदि (optarg)
                scontext = optarg;
              अन्यथा
                {
                  समुच्चयः_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       रिक्त, 0);
                  यदि (! समुच्चयः_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          अन्यथा यदि (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          भङ्ग;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);
        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (EXIT_FAILURE);
    }

  यदि (scontext)
    {
      पूर्णाङ्क ret = 0;
      यदि (is_smack_enabled ())
        ret = smack_समुच्चयः_label_for_self (scontext);
      अन्यथा
        ret = setfscreatecon (scontext);

      यदि (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = बहुलकम्_RW_UGO;
  यदि (specified_बहुलकम्)
    {
      संरचना बहुलकम्_change *change = बहुलकम्_compile (specified_बहुलकम्);
      यदि (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_मानम् = umask (0);
      umask (umask_मानम्);
      newmode = बहुलकम्_adjust (newmode, false, umask_मानम्, change, रिक्त);
      मुक्त (change);
      यदि (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  पूर्णाङ्क exit_स्थितिः = EXIT_SUCCESS;
  प्रति (; optind < argc; ++optind)
    {
      यदि (समुच्चयः_security_context)
        defaultcon (समुच्चयः_security_context, argv[optind], S_IFIFO);
      यदि (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_स्थितिः = EXIT_FAILURE;
        }
      अन्यथा यदि (specified_बहुलकम् && lchmod (argv[optind], newmode) != 0)
        {
          error (0, errno, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_स्थितिः = EXIT_FAILURE;
        }
    }

  प्रत्यागम exit_स्थितिः;
}
