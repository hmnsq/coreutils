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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <selinux/label.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "modechange.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "selinux.h"
#அந்தர்பாவ "smack.h"
#அந்தர்பாவ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mknod"

#define AUTHORS proper_name ("David MacKenzie")

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, வெற்று, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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
         set the SELinux security context to இயல்பு type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or எனில் CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Both MAJOR and MINOR must be specified when TYPE is b, c, or u, and they\n\
must be omitted when TYPE is p.  If MAJOR or MINOR begins with 0x or 0X,\n\
it is interpreted as hexadecimal; otherwise, எனில் it begins with 0, as octal;\n\
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

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  எழுத்து மாறிலி *specified_mode = வெற்று;
  எழுத்து மாறிலி *scontext = வெற்று;
  கட்டமைப்பு selabel_handle *set_security_context = வெற்று;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  முழு optc;
  போது ((optc = getopt_long (argc, argv, "m:Z", longopts, வெற்று)) != -1)
    {
      தேர்வு (optc)
        {
        நிலை 'm':
          specified_mode = optarg;
          நிறுத்து;
        நிலை 'Z':
          எனில் (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          இல்லையெனில் எனில் (is_selinux_enabled () > 0)
            {
              எனில் (optarg)
                scontext = optarg;
              இல்லையெனில்
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       வெற்று, 0);
                  எனில் (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          இல்லையெனில் எனில் (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          நிறுத்து;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  mode_t newmode = MODE_RW_UGO;
  எனில் (specified_mode)
    {
      கட்டமைப்பு mode_change *change = mode_compile (specified_mode);
      எனில் (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_மதிப்பு = umask (0);
      umask (umask_மதிப்பு);
      newmode = mode_adjust (newmode, false, umask_மதிப்பு, change, வெற்று);
      விடுவி (change);
      எனில் (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  /* If the number of arguments is 0 or 1,
     or (if it's 2 or more and the second one starts with 'p'), then there
     must be exactly two operands.  Otherwise, there must be four.  */
  முழு expected_operands = (argc <= optind
                           || (optind + 1 < argc && argv[optind + 1][0] == 'p')
                           ? 2 : 4);

  எனில் (argc - optind < expected_operands)
    {
      எனில் (argc <= optind)
        error (0, 0, _("missing operand"));
      இல்லையெனில்
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      எனில் (expected_operands == 4 && argc - optind == 2)
        fprintf (stderr, "%s\n",
                 _("Special files require major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  எனில் (expected_operands < argc - optind)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[optind + expected_operands]));
      எனில் (expected_operands == 2 && argc - optind == 4)
        fprintf (stderr, "%s\n",
                 _("Fifos do not have major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  எனில் (scontext)
    {
      முழு ret = 0;
      எனில் (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      இல்லையெனில்
        ret = setfscreatecon (scontext);

      எனில் (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  /* Only check the first character, to allow mnemonic usage like
     'mknod /dev/rst0 character 18 0'. */

  mode_t முனை_type;
  தேர்வு (argv[optind + 1][0])
    {
    நிலை 'b':			/* 'block' or 'buffered' */
#ifndef S_IFBLK
      error (EXIT_FAILURE, 0, _("block special files not supported"));
#இல்லையெனில்
      முனை_type = S_IFBLK;
#endif
      செல் block_or_character;

    நிலை 'c':			/* 'character' */
    நிலை 'u':			/* 'unbuffered' */
#ifndef S_IFCHR
      error (EXIT_FAILURE, 0, _("character special files not supported"));
#இல்லையெனில்
      முனை_type = S_IFCHR;
#endif
      செல் block_or_character;

    block_or_character:
      {
        எழுத்து மாறிலி *s_major = argv[optind + 2];
        uintmax_t i_major;
        எனில் (xstrtoumax (s_major, வெற்று, 0, &i_major, "") != LONGINT_OK
            || i_major != (major_t) i_major)
          error (EXIT_FAILURE, 0,
                 _("invalid major device number %s"), quote (s_major));

        எழுத்து மாறிலி *s_minor = argv[optind + 3];
        uintmax_t i_minor;
        எனில் (xstrtoumax (s_minor, வெற்று, 0, &i_minor, "") != LONGINT_OK
            || i_minor != (minor_t) i_minor)
          error (EXIT_FAILURE, 0,
                 _("invalid minor device number %s"), quote (s_minor));

        dev_t device = makedev (i_major, i_minor);
#ifdef NODEV
        எனில் (device == NODEV)
          error (EXIT_FAILURE, 0, _("invalid device %s %s"),
                 s_major, s_minor);
#endif

        எனில் (set_security_context)
          defaultcon (set_security_context, argv[optind], முனை_type);

        எனில் (mknod (argv[optind], newmode | முனை_type, device) != 0)
          error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      }
      நிறுத்து;

    நிலை 'p':			/* 'pipe' */
      எனில் (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      எனில் (mkfifo (argv[optind], newmode) != 0)
        error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      நிறுத்து;

    இயல்பு:
      error (0, 0, _("invalid device type %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  எனில் (specified_mode && lchmod (argv[optind], newmode) != 0)
    error (EXIT_FAILURE, errno, _("cannot set permissions of %s"),
           quoteaf (argv[optind]));

  திரும்பு EXIT_SUCCESS;
}
