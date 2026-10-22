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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <getopt.h>
#অন্তর্ভাৱ <sys/types.h>
#অন্তর্ভাৱ <selinux/label.h>

#অন্তর্ভাৱ "system.h"
#অন্তর্ভাৱ "modechange.h"
#অন্তর্ভাৱ "quote.h"
#অন্তর্ভাৱ "selinux.h"
#অন্তর্ভাৱ "smack.h"
#অন্তর্ভাৱ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mknod"

#define AUTHORS proper_name ("David MacKenzie")

স্থির সংরচনা option ধ্রুৱ longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, রিক্ত, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
         set the SELinux security context to স্ৱভাৱ type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or যদি CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Both MAJOR and MINOR must be specified when TYPE is b, c, or u, and they\n\
must be omitted when TYPE is p.  If MAJOR or MINOR begins with 0x or 0X,\n\
it is interpreted as hexadecimal; otherwise, যদি it begins with 0, as octal;\n\
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

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, ৱর্ণ **argv)
{
  ৱর্ণ ধ্রুৱ *specified_mode = রিক্ত;
  ৱর্ণ ধ্রুৱ *scontext = রিক্ত;
  সংরচনা selabel_handle *set_security_context = রিক্ত;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  পূর্ণাঙ্ক optc;
  যেতিয়ালৈ ((optc = getopt_long (argc, argv, "m:Z", longopts, রিক্ত)) != -1)
    {
      ৱিকল্প (optc)
        {
        পক্ষ 'm':
          specified_mode = optarg;
          ভঙ্গ;
        পক্ষ 'Z':
          যদি (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          অন্যথা যদি (is_selinux_enabled () > 0)
            {
              যদি (optarg)
                scontext = optarg;
              অন্যথা
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       রিক্ত, 0);
                  যদি (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          অন্যথা যদি (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          ভঙ্গ;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        স্ৱভাৱ:
          usage (EXIT_FAILURE);
        }
    }

  mode_t newmode = MODE_RW_UGO;
  যদি (specified_mode)
    {
      সংরচনা mode_change *change = mode_compile (specified_mode);
      যদি (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_মান = umask (0);
      umask (umask_মান);
      newmode = mode_adjust (newmode, false, umask_মান, change, রিক্ত);
      মুক্ত (change);
      যদি (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  /* If the number of arguments is 0 or 1,
     or (if it's 2 or more and the second one starts with 'p'), then there
     must be exactly two operands.  Otherwise, there must be four.  */
  পূর্ণাঙ্ক expected_operands = (argc <= optind
                           || (optind + 1 < argc && argv[optind + 1][0] == 'p')
                           ? 2 : 4);

  যদি (argc - optind < expected_operands)
    {
      যদি (argc <= optind)
        error (0, 0, _("missing operand"));
      অন্যথা
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      যদি (expected_operands == 4 && argc - optind == 2)
        fprintf (stderr, "%s\n",
                 _("Special files require major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  যদি (expected_operands < argc - optind)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[optind + expected_operands]));
      যদি (expected_operands == 2 && argc - optind == 4)
        fprintf (stderr, "%s\n",
                 _("Fifos do not have major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  যদি (scontext)
    {
      পূর্ণাঙ্ক ret = 0;
      যদি (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      অন্যথা
        ret = setfscreatecon (scontext);

      যদি (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  /* Only check the first character, to allow mnemonic usage like
     'mknod /dev/rst0 character 18 0'. */

  mode_t গ্ৰন্থি_type;
  ৱিকল্প (argv[optind + 1][0])
    {
    পক্ষ 'b':			/* 'block' or 'buffered' */
#ifndef S_IFBLK
      error (EXIT_FAILURE, 0, _("block special files not supported"));
#অন্যথা
      গ্ৰন্থি_type = S_IFBLK;
#endif
      গচ্ছ block_or_character;

    পক্ষ 'c':			/* 'character' */
    পক্ষ 'u':			/* 'unbuffered' */
#ifndef S_IFCHR
      error (EXIT_FAILURE, 0, _("character special files not supported"));
#অন্যথা
      গ্ৰন্থি_type = S_IFCHR;
#endif
      গচ্ছ block_or_character;

    block_or_character:
      {
        ৱর্ণ ধ্রুৱ *s_major = argv[optind + 2];
        uintmax_t i_major;
        যদি (xstrtoumax (s_major, রিক্ত, 0, &i_major, "") != LONGINT_OK
            || i_major != (major_t) i_major)
          error (EXIT_FAILURE, 0,
                 _("invalid major device number %s"), quote (s_major));

        ৱর্ণ ধ্রুৱ *s_minor = argv[optind + 3];
        uintmax_t i_minor;
        যদি (xstrtoumax (s_minor, রিক্ত, 0, &i_minor, "") != LONGINT_OK
            || i_minor != (minor_t) i_minor)
          error (EXIT_FAILURE, 0,
                 _("invalid minor device number %s"), quote (s_minor));

        dev_t device = makedev (i_major, i_minor);
#ifdef NODEV
        যদি (device == NODEV)
          error (EXIT_FAILURE, 0, _("invalid device %s %s"),
                 s_major, s_minor);
#endif

        যদি (set_security_context)
          defaultcon (set_security_context, argv[optind], গ্ৰন্থি_type);

        যদি (mknod (argv[optind], newmode | গ্ৰন্থি_type, device) != 0)
          error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      }
      ভঙ্গ;

    পক্ষ 'p':			/* 'pipe' */
      যদি (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      যদি (mkfifo (argv[optind], newmode) != 0)
        error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      ভঙ্গ;

    স্ৱভাৱ:
      error (0, 0, _("invalid device type %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  যদি (specified_mode && lchmod (argv[optind], newmode) != 0)
    error (EXIT_FAILURE, errno, _("cannot set permissions of %s"),
           quoteaf (argv[optind]));

  ঘূৰাই EXIT_SUCCESS;
}
