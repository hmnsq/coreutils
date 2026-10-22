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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <selinux/label.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "modechange.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "selinux.h"
#અન્તર્ભાવ "smack.h"
#અન્તર્ભાવ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mknod"

#define AUTHORS proper_name ("David MacKenzie")

સ્થિર સંરચના option અચલ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, ખાલી, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
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
         set the SELinux security context to મૂળભૂત type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or જો CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Both MAJOR and MINOR must be specified when TYPE is b, c, or u, and they\n\
must be omitted when TYPE is p.  If MAJOR or MINOR begins with 0x or 0X,\n\
it is interpreted as hexadecimal; otherwise, જો it begins with 0, as octal;\n\
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

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  અક્ષર અચલ *specified_mode = ખાલી;
  અક્ષર અચલ *scontext = ખાલી;
  સંરચના selabel_handle *set_security_context = ખાલી;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  પૂર્ણાંક optc;
  જ્યારે ((optc = getopt_long (argc, argv, "m:Z", longopts, ખાલી)) != -1)
    {
      પસંદગી (optc)
        {
        સ્થિતિ 'm':
          specified_mode = optarg;
          તોડો;
        સ્થિતિ 'Z':
          જો (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          નહીંતર જો (is_selinux_enabled () > 0)
            {
              જો (optarg)
                scontext = optarg;
              નહીંતર
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       ખાલી, 0);
                  જો (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          નહીંતર જો (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          તોડો;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  mode_t newmode = MODE_RW_UGO;
  જો (specified_mode)
    {
      સંરચના mode_change *change = mode_compile (specified_mode);
      જો (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_કિંમત = umask (0);
      umask (umask_કિંમત);
      newmode = mode_adjust (newmode, false, umask_કિંમત, change, ખાલી);
      મુક્ત (change);
      જો (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  /* If the number of arguments is 0 or 1,
     or (if it's 2 or more and the second one starts with 'p'), then there
     must be exactly two operands.  Otherwise, there must be four.  */
  પૂર્ણાંક expected_operands = (argc <= optind
                           || (optind + 1 < argc && argv[optind + 1][0] == 'p')
                           ? 2 : 4);

  જો (argc - optind < expected_operands)
    {
      જો (argc <= optind)
        error (0, 0, _("missing operand"));
      નહીંતર
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      જો (expected_operands == 4 && argc - optind == 2)
        fprintf (stderr, "%s\n",
                 _("Special files require major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  જો (expected_operands < argc - optind)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[optind + expected_operands]));
      જો (expected_operands == 2 && argc - optind == 4)
        fprintf (stderr, "%s\n",
                 _("Fifos do not have major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  જો (scontext)
    {
      પૂર્ણાંક ret = 0;
      જો (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      નહીંતર
        ret = setfscreatecon (scontext);

      જો (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  /* Only check the first character, to allow mnemonic usage like
     'mknod /dev/rst0 character 18 0'. */

  mode_t ગાંઠ_type;
  પસંદગી (argv[optind + 1][0])
    {
    સ્થિતિ 'b':			/* 'block' or 'buffered' */
#ifndef S_IFBLK
      error (EXIT_FAILURE, 0, _("block special files not supported"));
#નહીંતર
      ગાંઠ_type = S_IFBLK;
#endif
      જાઓ block_or_character;

    સ્થિતિ 'c':			/* 'character' */
    સ્થિતિ 'u':			/* 'unbuffered' */
#ifndef S_IFCHR
      error (EXIT_FAILURE, 0, _("character special files not supported"));
#નહીંતર
      ગાંઠ_type = S_IFCHR;
#endif
      જાઓ block_or_character;

    block_or_character:
      {
        અક્ષર અચલ *s_major = argv[optind + 2];
        uintmax_t i_major;
        જો (xstrtoumax (s_major, ખાલી, 0, &i_major, "") != LONGINT_OK
            || i_major != (major_t) i_major)
          error (EXIT_FAILURE, 0,
                 _("invalid major device number %s"), quote (s_major));

        અક્ષર અચલ *s_minor = argv[optind + 3];
        uintmax_t i_minor;
        જો (xstrtoumax (s_minor, ખાલી, 0, &i_minor, "") != LONGINT_OK
            || i_minor != (minor_t) i_minor)
          error (EXIT_FAILURE, 0,
                 _("invalid minor device number %s"), quote (s_minor));

        dev_t device = makedev (i_major, i_minor);
#ifdef NODEV
        જો (device == NODEV)
          error (EXIT_FAILURE, 0, _("invalid device %s %s"),
                 s_major, s_minor);
#endif

        જો (set_security_context)
          defaultcon (set_security_context, argv[optind], ગાંઠ_type);

        જો (mknod (argv[optind], newmode | ગાંઠ_type, device) != 0)
          error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      }
      તોડો;

    સ્થિતિ 'p':			/* 'pipe' */
      જો (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      જો (mkfifo (argv[optind], newmode) != 0)
        error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      તોડો;

    મૂળભૂત:
      error (0, 0, _("invalid device type %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  જો (specified_mode && lchmod (argv[optind], newmode) != 0)
    error (EXIT_FAILURE, errno, _("cannot set permissions of %s"),
           quoteaf (argv[optind]));

  પાછા EXIT_SUCCESS;
}
