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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <selinux/label.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "modechange.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "selinux.h"
#ಅನ್ತರ್ಭಾವ "smack.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mknod"

#define AUTHORS proper_name ("David MacKenzie")

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, ರಿಕ್ತ, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("Usage: %s [OPTION]... NAME TYPE [MAJOR MINOR]\n"),
              program_name);
      fputs (_("\
Create the special file NAME of the given TYPE.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --mode=MODE\n\
         set file permission bits to MODE, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         set the SELinux security context to ಸ್ವಭಾವ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ಯದಿ CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Both MAJOR and MINOR must be specified when TYPE is b, c, or u, and they\n\
must be omitted when TYPE is p.  If MAJOR or MINOR begins with 0x or 0X,\n\
it is interpreted as hexadecimal; otherwise, ಯದಿ it begins with 0, as octal;\n\
otherwise, as decimal.  TYPE may be:\n\
"), stdout);
      fputs (_("\
\n\
  b      create a block (buffered) special file\n\
  c, u   create a character (unbuffered) special file\n\
  p      create a FIFO\n\
"), stdout);
      printf (USAGE_BUILTIN_WARNING, PROGRAM_NAME);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ವರ್ಣ ಧ್ರುವ *specified_mode = ರಿಕ್ತ;
  ವರ್ಣ ಧ್ರುವ *scontext = ರಿಕ್ತ;
  ಸಂರಚನಾ selabel_handle *set_security_context = ರಿಕ್ತ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಙ್ಕ optc;
  ಯಾವತ್ ((optc = getopt_long (argc, argv, "m:Z", longopts, ರಿಕ್ತ)) != -1)
    {
      ವಿಕಲ್ಪ (optc)
        {
        ಪಕ್ಷ 'm':
          specified_mode = optarg;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'Z':
          ಯದಿ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ಅನ್ಯಥಾ ಯದಿ (is_selinux_enabled () > 0)
            {
              ಯದಿ (optarg)
                scontext = optarg;
              ಅನ್ಯಥಾ
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       ರಿಕ್ತ, 0);
                  ಯದಿ (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ಅನ್ಯಥಾ ಯದಿ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ಭಙ್ಗ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  mode_t newmode = MODE_RW_UGO;
  ಯದಿ (specified_mode)
    {
      ಸಂರಚನಾ mode_change *change = mode_compile (specified_mode);
      ಯದಿ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_ಮೌಲ್ಯ = umask (0);
      umask (umask_ಮೌಲ್ಯ);
      newmode = mode_adjust (newmode, false, umask_ಮೌಲ್ಯ, change, ರಿಕ್ತ);
      ಮುಕ್ತ (change);
      ಯದಿ (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  /* If the number of arguments is 0 or 1,
     or (if it's 2 or more and the second one starts with 'p'), then there
     must be exactly two operands.  Otherwise, there must be four.  */
  ಪೂರ್ಣಾಙ್ಕ expected_operands = (argc <= optind
                           || (optind + 1 < argc && argv[optind + 1][0] == 'p')
                           ? 2 : 4);

  ಯದಿ (argc - optind < expected_operands)
    {
      ಯದಿ (argc <= optind)
        error (0, 0, _("missing operand"));
      ಅನ್ಯಥಾ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      ಯದಿ (expected_operands == 4 && argc - optind == 2)
        fprintf (stderr, "%s\n",
                 _("Special files require major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (expected_operands < argc - optind)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[optind + expected_operands]));
      ಯದಿ (expected_operands == 2 && argc - optind == 4)
        fprintf (stderr, "%s\n",
                 _("Fifos do not have major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (scontext)
    {
      ಪೂರ್ಣಾಙ್ಕ ret = 0;
      ಯದಿ (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      ಅನ್ಯಥಾ
        ret = setfscreatecon (scontext);

      ಯದಿ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  /* Only check the first character, to allow mnemonic usage like
     'mknod /dev/rst0 character 18 0'. */

  mode_t ಗಂಟು_type;
  ವಿಕಲ್ಪ (argv[optind + 1][0])
    {
    ಪಕ್ಷ 'b':			/* 'block' or 'buffered' */
#ifndef S_IFBLK
      error (EXIT_FAILURE, 0, _("block special files not supported"));
#ಅನ್ಯಥಾ
      ಗಂಟು_type = S_IFBLK;
#endif
      ಗಚ್ಛ block_or_character;

    ಪಕ್ಷ 'c':			/* 'character' */
    ಪಕ್ಷ 'u':			/* 'unbuffered' */
#ifndef S_IFCHR
      error (EXIT_FAILURE, 0, _("character special files not supported"));
#ಅನ್ಯಥಾ
      ಗಂಟು_type = S_IFCHR;
#endif
      ಗಚ್ಛ block_or_character;

    block_or_character:
      {
        ವರ್ಣ ಧ್ರುವ *s_major = argv[optind + 2];
        uintmax_t i_major;
        ಯದಿ (xstrtoumax (s_major, ರಿಕ್ತ, 0, &i_major, "") != LONGINT_OK
            || i_major != (major_t) i_major)
          error (EXIT_FAILURE, 0,
                 _("invalid major device number %s"), quote (s_major));

        ವರ್ಣ ಧ್ರುವ *s_minor = argv[optind + 3];
        uintmax_t i_minor;
        ಯದಿ (xstrtoumax (s_minor, ರಿಕ್ತ, 0, &i_minor, "") != LONGINT_OK
            || i_minor != (minor_t) i_minor)
          error (EXIT_FAILURE, 0,
                 _("invalid minor device number %s"), quote (s_minor));

        dev_t device = makedev (i_major, i_minor);
#ifdef NODEV
        ಯದಿ (device == NODEV)
          error (EXIT_FAILURE, 0, _("invalid device %s %s"),
                 s_major, s_minor);
#endif

        ಯದಿ (set_security_context)
          defaultcon (set_security_context, argv[optind], ಗಂಟು_type);

        ಯದಿ (mknod (argv[optind], newmode | ಗಂಟು_type, device) != 0)
          error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      }
      ಭಙ್ಗ;

    ಪಕ್ಷ 'p':			/* 'pipe' */
      ಯದಿ (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      ಯದಿ (mkfifo (argv[optind], newmode) != 0)
        error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      error (0, 0, _("invalid device type %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ಯದಿ (specified_mode && lchmod (argv[optind], newmode) != 0)
    error (EXIT_FAILURE, errno, _("cannot set permissions of %s"),
           quoteaf (argv[optind]));

  ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
}
