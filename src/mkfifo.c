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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <getopt.h>
#समावेश <sys/types.h>
#समावेश <selinux/label.h>

#समावेश "system.h"
#समावेश "modechange.h"
#समावेश "quote.h"
#समावेश "selinux.h"
#समावेश "smack.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "mkfifo"

#घोषणा AUTHORS proper_name ("David MacKenzie")

अटल काष्ठा option स्थिर longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, शून्य, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION]... NAME...\n"), program_name);
      ख_माला_दो (_("\
Create named pipes (FIFOs) with the given NAMEs.\n\
"), मानक_निकास);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --mode=MODE\n\
         set file permission bits to MODE, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         set the SELinux security context to शेष type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or अगर CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अक्षर स्थिर *specified_mode = शून्य;
  अक्षर स्थिर *scontext = शून्य;
  काष्ठा selabel_handle *set_security_context = शून्य;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पूर्णांक optc;
  जबतक ((optc = getopt_long (argc, argv, "m:Z", longopts, शून्य)) != -1)
    {
      चयन (optc)
        {
        हाल 'm':
          specified_mode = optarg;
          अवरोध;
        हाल 'Z':
          अगर (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          अन्यथा अगर (is_selinux_enabled () > 0)
            {
              अगर (optarg)
                scontext = optarg;
              अन्यथा
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       शून्य, 0);
                  अगर (! set_security_context)
                    error (0, त्रुटि_सं, _("warning: ignoring --context"));
                }
            }
          अन्यथा अगर (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          अवरोध;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (optind == argc)
    {
      error (0, 0, _("missing operand"));
      usage (निकास_त्रुटि);
    }

  अगर (scontext)
    {
      पूर्णांक ret = 0;
      अगर (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      अन्यथा
        ret = setfscreatecon (scontext);

      अगर (ret < 0)
        error (निकास_त्रुटि, त्रुटि_सं,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  mode_t newmode = MODE_RW_UGO;
  अगर (specified_mode)
    {
      काष्ठा mode_change *change = mode_compile (specified_mode);
      अगर (!change)
        error (निकास_त्रुटि, 0, _("invalid mode"));
      mode_t umask_मूल्य = umask (0);
      umask (umask_मूल्य);
      newmode = mode_adjust (newmode, false, umask_मूल्य, change, शून्य);
      मुक्त (change);
      अगर (newmode & ~S_IRWXUGO)
        error (निकास_त्रुटि, 0,
               _("mode must specify only file permission bits"));
    }

  पूर्णांक exit_status = निकास_सफल;
  क्रम (; optind < argc; ++optind)
    {
      अगर (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      अगर (mkfifo (argv[optind], newmode) != 0)
        {
          error (0, त्रुटि_सं, _("cannot create fifo %s"), quoteaf (argv[optind]));
          exit_status = निकास_त्रुटि;
        }
      अन्यथा अगर (specified_mode && lchmod (argv[optind], newmode) != 0)
        {
          error (0, त्रुटि_सं, _("cannot set permissions of %s"),
                 quoteaf (argv[optind]));
          exit_status = निकास_त्रुटि;
        }
    }

  वापस exit_status;
}
