/* mknod -- make special files
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

/* Written by David MacKenzie <djm@ai.mit.edu>  */

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
#समावेश "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "mknod"

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
      म_लिखो (_("Usage: %s [OPTION]... NAME TYPE [MAJOR MINOR]\n"),
              program_name);
      ख_माला_दो (_("\
Create the special file NAME of the given TYPE.\n\
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
      ख_माला_दो (_("\
\n\
Both MAJOR and MINOR must be specified when TYPE is b, c, or u, and they\n\
must be omitted when TYPE is p.  If MAJOR or MINOR begins with 0x or 0X,\n\
it is interpreted as hexadecimal; otherwise, अगर it begins with 0, as octal;\n\
otherwise, as decimal.  TYPE may be:\n\
"), मानक_निकास);
      ख_माला_दो (_("\
\n\
  b      create a block (buffered) special file\n\
  c, u   create a character (unbuffered) special file\n\
  p      create a FIFO\n\
"), मानक_निकास);
      म_लिखो (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
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

  /* If the number of arguments is 0 or 1,
     or (if it's 2 or more and the second one starts with 'p'), then there
     must be exactly two operands.  Otherwise, there must be four.  */
  पूर्णांक expected_operands = (argc <= optind
                           || (optind + 1 < argc && argv[optind + 1][0] == 'p')
                           ? 2 : 4);

  अगर (argc - optind < expected_operands)
    {
      अगर (argc <= optind)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      अगर (expected_operands == 4 && argc - optind == 2)
        ख_लिखो (मानक_त्रुटि, "%s\n",
                 _("Special files require major and minor device numbers."));
      usage (निकास_त्रुटि);
    }

  अगर (expected_operands < argc - optind)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[optind + expected_operands]));
      अगर (expected_operands == 2 && argc - optind == 4)
        ख_लिखो (मानक_त्रुटि, "%s\n",
                 _("Fifos do not have major and minor device numbers."));
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

  /* Only check the first character, to allow mnemonic usage like
     'mknod /dev/rst0 character 18 0'. */

  mode_t गाँठ_type;
  चयन (argv[optind + 1][0])
    {
    हाल 'b':			/* 'block' or 'buffered' */
#अगर_अघोषित S_IFBLK
      error (निकास_त्रुटि, 0, _("block special files not supported"));
#अन्यथा
      गाँठ_type = S_IFBLK;
#पूर्ण_अगर
      जाओ block_or_character;

    हाल 'c':			/* 'character' */
    हाल 'u':			/* 'unbuffered' */
#अगर_अघोषित S_IFCHR
      error (निकास_त्रुटि, 0, _("character special files not supported"));
#अन्यथा
      गाँठ_type = S_IFCHR;
#पूर्ण_अगर
      जाओ block_or_character;

    block_or_character:
      {
        अक्षर स्थिर *s_major = argv[optind + 2];
        uintmax_t i_major;
        अगर (xstrtoumax (s_major, शून्य, 0, &i_major, "") != LONGINT_OK
            || i_major != (major_t) i_major)
          error (निकास_त्रुटि, 0,
                 _("invalid major device number %s"), quote (s_major));

        अक्षर स्थिर *s_minor = argv[optind + 3];
        uintmax_t i_minor;
        अगर (xstrtoumax (s_minor, शून्य, 0, &i_minor, "") != LONGINT_OK
            || i_minor != (minor_t) i_minor)
          error (निकास_त्रुटि, 0,
                 _("invalid minor device number %s"), quote (s_minor));

        dev_t device = makedev (i_major, i_minor);
#अगर_घोषित NODEV
        अगर (device == NODEV)
          error (निकास_त्रुटि, 0, _("invalid device %s %s"),
                 s_major, s_minor);
#पूर्ण_अगर

        अगर (set_security_context)
          defaultcon (set_security_context, argv[optind], गाँठ_type);

        अगर (mknod (argv[optind], newmode | गाँठ_type, device) != 0)
          error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (argv[optind]));
      }
      अवरोध;

    हाल 'p':			/* 'pipe' */
      अगर (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      अगर (mkfifo (argv[optind], newmode) != 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (argv[optind]));
      अवरोध;

    शेष:
      error (0, 0, _("invalid device type %s"), quote (argv[optind + 1]));
      usage (निकास_त्रुटि);
    }

  अगर (specified_mode && lchmod (argv[optind], newmode) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot set permissions of %s"),
           quoteaf (argv[optind]));

  वापस निकास_सफल;
}
