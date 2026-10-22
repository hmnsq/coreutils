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
#अन्तर्भाव "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mknod"

#define AUTHORS proper_name ("David MacKenzie")

स्थिर संरचना option अचर longopts[] =
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
  यदि (status != निकास_सफल)
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
         set the SELinux security context to स्वभाव type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or यदि CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      ख_माला_दो (_("\
\n\
Both MAJOR and MINOR must be specified when TYPE is b, c, or u, and they\n\
must be omitted when TYPE is p.  If MAJOR or MINOR begins with 0x or 0X,\n\
it is interpreted as hexadecimal; otherwise, यदि it begins with 0, as octal;\n\
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

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  वर्ण अचर *specified_mode = रिक्त;
  वर्ण अचर *scontext = रिक्त;
  संरचना selabel_handle *set_security_context = रिक्त;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पूर्णाङ्क optc;
  जबसम्म ((optc = getopt_long (argc, argv, "m:Z", longopts, रिक्त)) != -1)
    {
      विकल्प (optc)
        {
        पक्ष 'm':
          specified_mode = optarg;
          रोक;
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
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       रिक्त, 0);
                  यदि (! set_security_context)
                    error (0, त्रुटि_सं, _("warning: ignoring --context"));
                }
            }
          अन्यथा यदि (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          रोक;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        स्वभाव:
          usage (निकास_त्रुटि);
        }
    }

  mode_t newmode = MODE_RW_UGO;
  यदि (specified_mode)
    {
      संरचना mode_change *change = mode_compile (specified_mode);
      यदि (!change)
        error (निकास_त्रुटि, 0, _("invalid mode"));
      mode_t umask_मान = umask (0);
      umask (umask_मान);
      newmode = mode_adjust (newmode, false, umask_मान, change, रिक्त);
      मुक्त (change);
      यदि (newmode & ~S_IRWXUGO)
        error (निकास_त्रुटि, 0,
               _("mode must specify only file permission bits"));
    }

  /* If the number of arguments is 0 or 1,
     or (if it's 2 or more and the second one starts with 'p'), then there
     must be exactly two operands.  Otherwise, there must be four.  */
  पूर्णाङ्क expected_operands = (argc <= optind
                           || (optind + 1 < argc && argv[optind + 1][0] == 'p')
                           ? 2 : 4);

  यदि (argc - optind < expected_operands)
    {
      यदि (argc <= optind)
        error (0, 0, _("missing operand"));
      अन्यथा
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      यदि (expected_operands == 4 && argc - optind == 2)
        ख_लिखो (मानक_त्रुटि, "%s\n",
                 _("Special files require major and minor device numbers."));
      usage (निकास_त्रुटि);
    }

  यदि (expected_operands < argc - optind)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[optind + expected_operands]));
      यदि (expected_operands == 2 && argc - optind == 4)
        ख_लिखो (मानक_त्रुटि, "%s\n",
                 _("Fifos do not have major and minor device numbers."));
      usage (निकास_त्रुटि);
    }

  यदि (scontext)
    {
      पूर्णाङ्क ret = 0;
      यदि (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      अन्यथा
        ret = setfscreatecon (scontext);

      यदि (ret < 0)
        error (निकास_त्रुटि, त्रुटि_सं,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  /* Only check the first character, to allow mnemonic usage like
     'mknod /dev/rst0 character 18 0'. */

  mode_t गाँठो_type;
  विकल्प (argv[optind + 1][0])
    {
    पक्ष 'b':			/* 'block' or 'buffered' */
#ifndef S_IFBLK
      error (निकास_त्रुटि, 0, _("block special files not supported"));
#अन्यथा
      गाँठो_type = S_IFBLK;
#endif
      गच्छ block_or_character;

    पक्ष 'c':			/* 'character' */
    पक्ष 'u':			/* 'unbuffered' */
#ifndef S_IFCHR
      error (निकास_त्रुटि, 0, _("character special files not supported"));
#अन्यथा
      गाँठो_type = S_IFCHR;
#endif
      गच्छ block_or_character;

    block_or_character:
      {
        वर्ण अचर *s_major = argv[optind + 2];
        uintmax_t i_major;
        यदि (xstrtoumax (s_major, रिक्त, 0, &i_major, "") != LONGINT_OK
            || i_major != (major_t) i_major)
          error (निकास_त्रुटि, 0,
                 _("invalid major device number %s"), quote (s_major));

        वर्ण अचर *s_minor = argv[optind + 3];
        uintmax_t i_minor;
        यदि (xstrtoumax (s_minor, रिक्त, 0, &i_minor, "") != LONGINT_OK
            || i_minor != (minor_t) i_minor)
          error (निकास_त्रुटि, 0,
                 _("invalid minor device number %s"), quote (s_minor));

        dev_t device = makedev (i_major, i_minor);
#ifdef NODEV
        यदि (device == NODEV)
          error (निकास_त्रुटि, 0, _("invalid device %s %s"),
                 s_major, s_minor);
#endif

        यदि (set_security_context)
          defaultcon (set_security_context, argv[optind], गाँठो_type);

        यदि (mknod (argv[optind], newmode | गाँठो_type, device) != 0)
          error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (argv[optind]));
      }
      रोक;

    पक्ष 'p':			/* 'pipe' */
      यदि (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      यदि (mkfifo (argv[optind], newmode) != 0)
        error (निकास_त्रुटि, त्रुटि_सं, "%s", quotef (argv[optind]));
      रोक;

    स्वभाव:
      error (0, 0, _("invalid device type %s"), quote (argv[optind + 1]));
      usage (निकास_त्रुटि);
    }

  यदि (specified_mode && lchmod (argv[optind], newmode) != 0)
    error (निकास_त्रुटि, त्रुटि_सं, _("cannot set permissions of %s"),
           quoteaf (argv[optind]));

  फर्काऊ निकास_सफल;
}
