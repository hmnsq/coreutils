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
#परिभाषय क्रमादेशः_NAME "mknod"

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
      प्रारूप_लिख (_("Usage: %s [OPTION]... NAME TYPE [MAJOR MINOR]\n"),
              क्रमादेशः_name);
      fputs (_("\
Create the special file NAME सम्बन्धि the given TYPE.\n\
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
      fputs (_("\
\n\
Both MAJOR and MINOR must be specified when TYPE is b, c, or u, and they\n\
must be omitted when TYPE is p.  If MAJOR or MINOR begins with 0x or 0X,\n\
it is interpreted as hexadecimal; otherwise, यदि it begins with 0, as octal;\n\
otherwise, as decimal.  TYPE may be:\n\
"), stdout);
      fputs (_("\
\n\
  b      create a खण्डम् (buffered) special file\n\
  c, u   create a character (unbuffered) special file\n\
  p      create a FIFO\n\
"), stdout);
      प्रारूप_लिख (USAGE_BUILTIN_WARNING, क्रमादेशः_NAME);
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
        fprintf (stderr, "%s\n",
                 _("Special files require major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  यदि (expected_operands < argc - optind)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[optind + expected_operands]));
      यदि (expected_operands == 2 && argc - optind == 4)
        fprintf (stderr, "%s\n",
                 _("Fifos do not have major and minor device numbers."));
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

  /* Only check the first character, to allow mnemonic usage like
     'mknod /dev/rst0 character 18 0'. */

  mode_t ग्रन्थिः_type;
  विकल्प (argv[optind + 1][0])
    {
    पक्ष 'b':			/* 'block' or 'buffered' */
#यदि_अपरिभाषितम् S_IFBLK
      error (EXIT_FAILURE, 0, _("block special files not supported"));
#अन्यथा
      ग्रन्थिः_type = S_IFBLK;
#यदि_अन्तः
      गच्छ खण्डम्_or_character;

    पक्ष 'c':			/* 'character' */
    पक्ष 'u':			/* 'unbuffered' */
#यदि_अपरिभाषितम् S_IFCHR
      error (EXIT_FAILURE, 0, _("character special files not supported"));
#अन्यथा
      ग्रन्थिः_type = S_IFCHR;
#यदि_अन्तः
      गच्छ खण्डम्_or_character;

    खण्डम्_or_character:
      {
        वर्ण ध्रुव *s_major = argv[optind + 2];
        uintmax_t i_major;
        यदि (xstrtoumax (s_major, रिक्त, 0, &i_major, "") != LONGINT_OK
            || i_major != (major_t) i_major)
          error (EXIT_FAILURE, 0,
                 _("invalid major device number %s"), quote (s_major));

        वर्ण ध्रुव *s_minor = argv[optind + 3];
        uintmax_t i_minor;
        यदि (xstrtoumax (s_minor, रिक्त, 0, &i_minor, "") != LONGINT_OK
            || i_minor != (minor_t) i_minor)
          error (EXIT_FAILURE, 0,
                 _("invalid minor device number %s"), quote (s_minor));

        dev_t device = makedev (i_major, i_minor);
#यदि_परिभाषितम् NODEV
        यदि (device == NODEV)
          error (EXIT_FAILURE, 0, _("invalid device %s %s"),
                 s_major, s_minor);
#यदि_अन्तः

        यदि (समुच्चयः_security_context)
          defaultcon (समुच्चयः_security_context, argv[optind], ग्रन्थिः_type);

        यदि (mknod (argv[optind], newmode | ग्रन्थिः_type, device) != 0)
          error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      }
      भङ्ग;

    पक्ष 'p':			/* 'pipe' */
      यदि (समुच्चयः_security_context)
        defaultcon (समुच्चयः_security_context, argv[optind], S_IFIFO);
      यदि (mkfifo (argv[optind], newmode) != 0)
        error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      भङ्ग;

    स्वभाव:
      error (0, 0, _("invalid device type %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  यदि (specified_बहुलकम् && lchmod (argv[optind], newmode) != 0)
    error (EXIT_FAILURE, errno, _("cannot set permissions of %s"),
           quoteaf (argv[optind]));

  प्रत्यागम EXIT_SUCCESS;
}
