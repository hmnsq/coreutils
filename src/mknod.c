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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <getopt.h>
#ضمن <sys/types.h>
#ضمن <selinux/label.h>

#ضمن "system.h"
#ضمن "modechange.h"
#ضمن "quote.h"
#ضمن "selinux.h"
#ضمن "smack.h"
#ضمن "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "mknod"

#تعریف_کرو AUTHORS proper_name ("David MacKenzie")

ساكن بنية option ثابت longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_دلیل, عدم, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [OPTION]... NAME TYPE [MAJOR MINOR]\n"),
              پروگرام_name);
      fputs (_("\
Create the special file NAME دا the given TYPE.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --منوال_2=منوال\n\
         سیٹ_2 file permission بٹ_2 منزل منوال, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         سیٹ_2 the SELinux security context منزل افتراضي type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or إذا CTX is specified then سیٹ_2 the\n\
         SELinux or SMACK security context منزل CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Both MAJOR and MINOR must be specified when TYPE is b, c, or u, and they\n\
must be omitted when TYPE is p.  If MAJOR or MINOR begins with 0x or 0X,\n\
it is interpreted as hexadecimal; otherwise, إذا it begins with 0, as octal;\n\
otherwise, as decimal.  TYPE may be:\n\
"), stdout);
      fputs (_("\
\n\
  b      create a بلاک (buffered) special file\n\
  c, u   create a character (unbuffered) special file\n\
  p      create a FIFO\n\
"), stdout);
      لکھو (USAGE_BUILTIN_WARNING, پروگرام_NAME);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  حرف ثابت *specified_منوال = عدم;
  حرف ثابت *scontext = عدم;
  بنية selabel_handle *سیٹ_security_context = عدم;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحيح optc;
  طالما ((optc = getopt_long (argc, argv, "m:Z", longopts, عدم)) != -1)
    {
      بدل (optc)
        {
        حالة 'm':
          specified_منوال = optarg;
          اكسر;
        حالة 'Z':
          إذا (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          وإلا إذا (is_selinux_enabled () > 0)
            {
              إذا (optarg)
                scontext = optarg;
              وإلا
                {
                  سیٹ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       عدم, 0);
                  إذا (! سیٹ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          وإلا إذا (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          اكسر;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);
        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  mode_t newmode = منوال_RW_UGO;
  إذا (specified_منوال)
    {
      بنية منوال_change *change = منوال_compile (specified_منوال);
      إذا (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_قدر = umask (0);
      umask (umask_قدر);
      newmode = منوال_adjust (newmode, false, umask_قدر, change, عدم);
      حرر (change);
      إذا (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  /* If the number of arguments is 0 or 1,
     or (if it's 2 or more and the second one starts with 'p'), then there
     must be exactly two operands.  Otherwise, there must be four.  */
  صحيح expected_operands = (argc <= optind
                           || (optind + 1 < argc && argv[optind + 1][0] == 'p')
                           ? 2 : 4);

  إذا (argc - optind < expected_operands)
    {
      إذا (argc <= optind)
        error (0, 0, _("missing operand"));
      وإلا
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      إذا (expected_operands == 4 && argc - optind == 2)
        fprintf (stderr, "%s\n",
                 _("Special files require major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  إذا (expected_operands < argc - optind)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[optind + expected_operands]));
      إذا (expected_operands == 2 && argc - optind == 4)
        fprintf (stderr, "%s\n",
                 _("Fifos do not have major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  إذا (scontext)
    {
      صحيح ret = 0;
      إذا (is_smack_enabled ())
        ret = smack_سیٹ_label_for_self (scontext);
      وإلا
        ret = setfscreatecon (scontext);

      إذا (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  /* Only check the first character, to allow mnemonic usage like
     'mknod /dev/rst0 character 18 0'. */

  mode_t گرہ_type;
  بدل (argv[optind + 1][0])
    {
    حالة 'b':			/* 'block' or 'buffered' */
#جے_بنا_تعریف S_IFBLK
      error (EXIT_FAILURE, 0, _("block special files not supported"));
#وإلا
      گرہ_type = S_IFBLK;
#جے_انت
      اذهب بلاک_or_character;

    حالة 'c':			/* 'character' */
    حالة 'u':			/* 'unbuffered' */
#جے_بنا_تعریف S_IFCHR
      error (EXIT_FAILURE, 0, _("character special files not supported"));
#وإلا
      گرہ_type = S_IFCHR;
#جے_انت
      اذهب بلاک_or_character;

    بلاک_or_character:
      {
        حرف ثابت *s_major = argv[optind + 2];
        uintmax_t i_major;
        إذا (xstrtoumax (s_major, عدم, 0, &i_major, "") != LONGINT_OK
            || i_major != (major_t) i_major)
          error (EXIT_FAILURE, 0,
                 _("invalid major device number %s"), quote (s_major));

        حرف ثابت *s_minor = argv[optind + 3];
        uintmax_t i_minor;
        إذا (xstrtoumax (s_minor, عدم, 0, &i_minor, "") != LONGINT_OK
            || i_minor != (minor_t) i_minor)
          error (EXIT_FAILURE, 0,
                 _("invalid minor device number %s"), quote (s_minor));

        dev_t device = makedev (i_major, i_minor);
#جے_تعریف_شدہ NODEV
        إذا (device == NODEV)
          error (EXIT_FAILURE, 0, _("invalid device %s %s"),
                 s_major, s_minor);
#جے_انت

        إذا (سیٹ_security_context)
          defaultcon (سیٹ_security_context, argv[optind], گرہ_type);

        إذا (mknod (argv[optind], newmode | گرہ_type, device) != 0)
          error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      }
      اكسر;

    حالة 'p':			/* 'pipe' */
      إذا (سیٹ_security_context)
        defaultcon (سیٹ_security_context, argv[optind], S_IFIFO);
      إذا (mkfifo (argv[optind], newmode) != 0)
        error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      اكسر;

    افتراضي:
      error (0, 0, _("invalid device type %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  إذا (specified_منوال && lchmod (argv[optind], newmode) != 0)
    error (EXIT_FAILURE, errno, _("cannot set permissions of %s"),
           quoteaf (argv[optind]));

  أرجع EXIT_SUCCESS;
}
