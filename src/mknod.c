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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "mknod"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("David MacKenzie")

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_ವಾದ, ಖಾಲಿ, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION]... NAME TYPE [MAJOR MINOR]\n"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Create the special file NAME ದ the given TYPE.\n\
"), stdout);

      emit_mandatory_arg_note ();

      oputs (_("\
  -m, --ಬಹುಲಕ_2=ಬಹುಲಕ\n\
         ಗಣ_2 file permission ಬಿಟ್ಟುಗಳು_2 ಗಮ್ಯ ಬಹುಲಕ, not a=rw - umask\n\
"));
      oputs (_("\
  -Z\n\
         ಗಣ_2 the SELinux security context ಗಮ್ಯ ಪೂರ್ವನಿಯೋಜಿತ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ಆದರೆ CTX is specified then ಗಣ_2 the\n\
         SELinux or SMACK security context ಗಮ್ಯ CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Both MAJOR and MINOR must be specified when TYPE is b, c, or u, and they\n\
must be omitted when TYPE is p.  If MAJOR or MINOR begins with 0x or 0X,\n\
it is interpreted as hexadecimal; otherwise, ಆದರೆ it begins with 0, as octal;\n\
otherwise, as decimal.  TYPE may be:\n\
"), stdout);
      fputs (_("\
\n\
  b      create a ಖಂಡ (buffered) special file\n\
  c, u   create a character (unbuffered) special file\n\
  p      create a FIFO\n\
"), stdout);
      ಬರೆ (USAGE_BUILTIN_WARNING, ಕಾರ್ಯಕ್ರಮ_NAME);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *specified_ಬಹುಲಕ = ಖಾಲಿ;
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *scontext = ಖಾಲಿ;
  ರಚನೆ selabel_handle *ಗಣ_security_context = ಖಾಲಿ;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಂಕ optc;
  ಆಗ ((optc = getopt_long (argc, argv, "m:Z", longopts, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (optc)
        {
        ಸ್ಥಿತಿ 'm':
          specified_ಬಹುಲಕ = optarg;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'Z':
          ಆದರೆ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (is_selinux_enabled () > 0)
            {
              ಆದರೆ (optarg)
                scontext = optarg;
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  ಗಣ_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       ಖಾಲಿ, 0);
                  ಆದರೆ (! ಗಣ_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ನಿಲ್ಲಿಸು;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  mode_t newmode = ಬಹುಲಕ_RW_UGO;
  ಆದರೆ (specified_ಬಹುಲಕ)
    {
      ರಚನೆ ಬಹುಲಕ_change *change = ಬಹುಲಕ_compile (specified_ಬಹುಲಕ);
      ಆದರೆ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_ಮೌಲ್ಯ = umask (0);
      umask (umask_ಮೌಲ್ಯ);
      newmode = ಬಹುಲಕ_adjust (newmode, false, umask_ಮೌಲ್ಯ, change, ಖಾಲಿ);
      ಬಿಡುಗಡೆ (change);
      ಆದರೆ (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  /* If the number of arguments is 0 or 1,
     or (if it's 2 or more and the second one starts with 'p'), then there
     must be exactly two operands.  Otherwise, there must be four.  */
  ಪೂರ್ಣಾಂಕ expected_operands = (argc <= optind
                           || (optind + 1 < argc && argv[optind + 1][0] == 'p')
                           ? 2 : 4);

  ಆದರೆ (argc - optind < expected_operands)
    {
      ಆದರೆ (argc <= optind)
        error (0, 0, _("missing operand"));
      ಇಲ್ಲದಿದ್ದರೆ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      ಆದರೆ (expected_operands == 4 && argc - optind == 2)
        fprintf (stderr, "%s\n",
                 _("Special files require major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (expected_operands < argc - optind)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[optind + expected_operands]));
      ಆದರೆ (expected_operands == 2 && argc - optind == 4)
        fprintf (stderr, "%s\n",
                 _("Fifos do not have major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (scontext)
    {
      ಪೂರ್ಣಾಂಕ ret = 0;
      ಆದರೆ (is_smack_enabled ())
        ret = smack_ಗಣ_label_for_self (scontext);
      ಇಲ್ಲದಿದ್ದರೆ
        ret = setfscreatecon (scontext);

      ಆದರೆ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  /* Only check the first character, to allow mnemonic usage like
     'mknod /dev/rst0 character 18 0'. */

  mode_t ಗಂಟು_type;
  ಆಯ್ಕೆ (argv[optind + 1][0])
    {
    ಸ್ಥಿತಿ 'b':			/* 'block' or 'buffered' */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ S_IFBLK
      error (EXIT_FAILURE, 0, _("block special files not supported"));
#ಇಲ್ಲದಿದ್ದರೆ
      ಗಂಟು_type = S_IFBLK;
#ಆದರೆ_ಅಂತ್ಯ
      ಹೋಗು ಖಂಡ_or_character;

    ಸ್ಥಿತಿ 'c':			/* 'character' */
    ಸ್ಥಿತಿ 'u':			/* 'unbuffered' */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ S_IFCHR
      error (EXIT_FAILURE, 0, _("character special files not supported"));
#ಇಲ್ಲದಿದ್ದರೆ
      ಗಂಟು_type = S_IFCHR;
#ಆದರೆ_ಅಂತ್ಯ
      ಹೋಗು ಖಂಡ_or_character;

    ಖಂಡ_or_character:
      {
        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s_major = argv[optind + 2];
        uintmax_t i_major;
        ಆದರೆ (xstrtoumax (s_major, ಖಾಲಿ, 0, &i_major, "") != LONGINT_OK
            || i_major != (major_t) i_major)
          error (EXIT_FAILURE, 0,
                 _("invalid major device number %s"), quote (s_major));

        ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s_minor = argv[optind + 3];
        uintmax_t i_minor;
        ಆದರೆ (xstrtoumax (s_minor, ಖಾಲಿ, 0, &i_minor, "") != LONGINT_OK
            || i_minor != (minor_t) i_minor)
          error (EXIT_FAILURE, 0,
                 _("invalid minor device number %s"), quote (s_minor));

        dev_t device = makedev (i_major, i_minor);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NODEV
        ಆದರೆ (device == NODEV)
          error (EXIT_FAILURE, 0, _("invalid device %s %s"),
                 s_major, s_minor);
#ಆದರೆ_ಅಂತ್ಯ

        ಆದರೆ (ಗಣ_security_context)
          defaultcon (ಗಣ_security_context, argv[optind], ಗಂಟು_type);

        ಆದರೆ (mknod (argv[optind], newmode | ಗಂಟು_type, device) != 0)
          error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      }
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ 'p':			/* 'pipe' */
      ಆದರೆ (ಗಣ_security_context)
        defaultcon (ಗಣ_security_context, argv[optind], S_IFIFO);
      ಆದರೆ (mkfifo (argv[optind], newmode) != 0)
        error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      error (0, 0, _("invalid device type %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ಆದರೆ (specified_ಬಹುಲಕ && lchmod (argv[optind], newmode) != 0)
    error (EXIT_FAILURE, errno, _("cannot set permissions of %s"),
           quoteaf (argv[optind]));

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
