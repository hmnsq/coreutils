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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <selinux/label.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "modechange.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "selinux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "smack.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "mknod"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("David MacKenzie")

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_ਦਲੀਲ, ਨਿਰਾ, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]... NAME TYPE [MAJOR MINOR]\n"),
              ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Create the special file NAME ਦਾ the given TYPE.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --ਬਹੁਲਕ_2=ਬਹੁਲਕ\n\
         ਸੈੱਟ_2 file permission ਬਿੱਟ_2 ਮੰਜ਼ਿਲ ਬਹੁਲਕ, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         ਸੈੱਟ_2 the SELinux security context ਮੰਜ਼ਿਲ ਮੂਲ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ਜੇ CTX is specified then ਸੈੱਟ_2 the\n\
         SELinux or SMACK security context ਮੰਜ਼ਿਲ CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Both MAJOR and MINOR must be specified when TYPE is b, c, or u, and they\n\
must be omitted when TYPE is p.  If MAJOR or MINOR begins with 0x or 0X,\n\
it is interpreted as hexadecimal; otherwise, ਜੇ it begins with 0, as octal;\n\
otherwise, as decimal.  TYPE may be:\n\
"), stdout);
      fputs (_("\
\n\
  b      create a ਖੰਡ (buffered) special file\n\
  c, u   create a character (unbuffered) special file\n\
  p      create a FIFO\n\
"), stdout);
      ਲਿਖੋ (USAGE_BUILTIN_WARNING, ਪ੍ਰੋਗਰਾਮ_NAME);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਅੱਖਰ ਅਚਲ *specified_ਬਹੁਲਕ = ਨਿਰਾ;
  ਅੱਖਰ ਅਚਲ *scontext = ਨਿਰਾ;
  ਬਣਤਰ selabel_handle *ਸੈੱਟ_security_context = ਨਿਰਾ;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਪੂਰਨ optc;
  ਜਦੋਂ ((optc = getopt_long (argc, argv, "m:Z", longopts, ਨਿਰਾ)) != -1)
    {
      ਚੋਣ (optc)
        {
        ਹਾਲਤ 'm':
          specified_ਬਹੁਲਕ = optarg;
          ਰੋਕੋ;
        ਹਾਲਤ 'Z':
          ਜੇ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ਨਹੀਂਤਾਂ ਜੇ (is_selinux_enabled () > 0)
            {
              ਜੇ (optarg)
                scontext = optarg;
              ਨਹੀਂਤਾਂ
                {
                  ਸੈੱਟ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       ਨਿਰਾ, 0);
                  ਜੇ (! ਸੈੱਟ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ਨਹੀਂਤਾਂ ਜੇ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ਰੋਕੋ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  mode_t newmode = ਬਹੁਲਕ_RW_UGO;
  ਜੇ (specified_ਬਹੁਲਕ)
    {
      ਬਣਤਰ ਬਹੁਲਕ_change *change = ਬਹੁਲਕ_compile (specified_ਬਹੁਲਕ);
      ਜੇ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_ਮੁੱਲ = umask (0);
      umask (umask_ਮੁੱਲ);
      newmode = ਬਹੁਲਕ_adjust (newmode, false, umask_ਮੁੱਲ, change, ਨਿਰਾ);
      ਮੁਕਤ (change);
      ਜੇ (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  /* If the number of arguments is 0 or 1,
     or (if it's 2 or more and the second one starts with 'p'), then there
     must be exactly two operands.  Otherwise, there must be four.  */
  ਪੂਰਨ expected_operands = (argc <= optind
                           || (optind + 1 < argc && argv[optind + 1][0] == 'p')
                           ? 2 : 4);

  ਜੇ (argc - optind < expected_operands)
    {
      ਜੇ (argc <= optind)
        error (0, 0, _("missing operand"));
      ਨਹੀਂਤਾਂ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      ਜੇ (expected_operands == 4 && argc - optind == 2)
        fprintf (stderr, "%s\n",
                 _("Special files require major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  ਜੇ (expected_operands < argc - optind)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[optind + expected_operands]));
      ਜੇ (expected_operands == 2 && argc - optind == 4)
        fprintf (stderr, "%s\n",
                 _("Fifos do not have major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  ਜੇ (scontext)
    {
      ਪੂਰਨ ret = 0;
      ਜੇ (is_smack_enabled ())
        ret = smack_ਸੈੱਟ_label_for_self (scontext);
      ਨਹੀਂਤਾਂ
        ret = setfscreatecon (scontext);

      ਜੇ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  /* Only check the first character, to allow mnemonic usage like
     'mknod /dev/rst0 character 18 0'. */

  mode_t ਗੰਢ_type;
  ਚੋਣ (argv[optind + 1][0])
    {
    ਹਾਲਤ 'b':			/* 'block' or 'buffered' */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ S_IFBLK
      error (EXIT_FAILURE, 0, _("block special files not supported"));
#ਨਹੀਂਤਾਂ
      ਗੰਢ_type = S_IFBLK;
#ਜੇ_ਅੰਤ
      ਜਾਓ ਖੰਡ_or_character;

    ਹਾਲਤ 'c':			/* 'character' */
    ਹਾਲਤ 'u':			/* 'unbuffered' */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ S_IFCHR
      error (EXIT_FAILURE, 0, _("character special files not supported"));
#ਨਹੀਂਤਾਂ
      ਗੰਢ_type = S_IFCHR;
#ਜੇ_ਅੰਤ
      ਜਾਓ ਖੰਡ_or_character;

    ਖੰਡ_or_character:
      {
        ਅੱਖਰ ਅਚਲ *s_major = argv[optind + 2];
        uintmax_t i_major;
        ਜੇ (xstrtoumax (s_major, ਨਿਰਾ, 0, &i_major, "") != LONGINT_OK
            || i_major != (major_t) i_major)
          error (EXIT_FAILURE, 0,
                 _("invalid major device number %s"), quote (s_major));

        ਅੱਖਰ ਅਚਲ *s_minor = argv[optind + 3];
        uintmax_t i_minor;
        ਜੇ (xstrtoumax (s_minor, ਨਿਰਾ, 0, &i_minor, "") != LONGINT_OK
            || i_minor != (minor_t) i_minor)
          error (EXIT_FAILURE, 0,
                 _("invalid minor device number %s"), quote (s_minor));

        dev_t device = makedev (i_major, i_minor);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NODEV
        ਜੇ (device == NODEV)
          error (EXIT_FAILURE, 0, _("invalid device %s %s"),
                 s_major, s_minor);
#ਜੇ_ਅੰਤ

        ਜੇ (ਸੈੱਟ_security_context)
          defaultcon (ਸੈੱਟ_security_context, argv[optind], ਗੰਢ_type);

        ਜੇ (mknod (argv[optind], newmode | ਗੰਢ_type, device) != 0)
          error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      }
      ਰੋਕੋ;

    ਹਾਲਤ 'p':			/* 'pipe' */
      ਜੇ (ਸੈੱਟ_security_context)
        defaultcon (ਸੈੱਟ_security_context, argv[optind], S_IFIFO);
      ਜੇ (mkfifo (argv[optind], newmode) != 0)
        error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      ਰੋਕੋ;

    ਮੂਲ:
      error (0, 0, _("invalid device type %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ਜੇ (specified_ਬਹੁਲਕ && lchmod (argv[optind], newmode) != 0)
    error (EXIT_FAILURE, errno, _("cannot set permissions of %s"),
           quoteaf (argv[optind]));

  ਵਾਪਸ EXIT_SUCCESS;
}
