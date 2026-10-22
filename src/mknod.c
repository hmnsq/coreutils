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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <selinux/label.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "modechange.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "selinux.h"
#അന്തര്ഭാവ "smack.h"
#അന്തര്ഭാവ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mknod"

#define AUTHORS proper_name ("David MacKenzie")

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, ശൂന്യ, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
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
         set the SELinux security context to സ്ഥിരസ്ഥിതി type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or എങ്കിൽ CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Both MAJOR and MINOR must be specified when TYPE is b, c, or u, and they\n\
must be omitted when TYPE is p.  If MAJOR or MINOR begins with 0x or 0X,\n\
it is interpreted as hexadecimal; otherwise, എങ്കിൽ it begins with 0, as octal;\n\
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

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  അക്ഷരം സ്ഥിരം *specified_mode = ശൂന്യ;
  അക്ഷരം സ്ഥിരം *scontext = ശൂന്യ;
  ഘടന selabel_handle *set_security_context = ശൂന്യ;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  പൂർണ്ണസംഖ്യ optc;
  ആകുമ്പോൾ ((optc = getopt_long (argc, argv, "m:Z", longopts, ശൂന്യ)) != -1)
    {
      തിരഞ്ഞെടുപ്പ് (optc)
        {
        അവസ്ഥ 'm':
          specified_mode = optarg;
          നിർത്തുക;
        അവസ്ഥ 'Z':
          എങ്കിൽ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          അല്ലെങ്കിൽ എങ്കിൽ (is_selinux_enabled () > 0)
            {
              എങ്കിൽ (optarg)
                scontext = optarg;
              അല്ലെങ്കിൽ
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       ശൂന്യ, 0);
                  എങ്കിൽ (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          അല്ലെങ്കിൽ എങ്കിൽ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          നിർത്തുക;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  mode_t newmode = MODE_RW_UGO;
  എങ്കിൽ (specified_mode)
    {
      ഘടന mode_change *change = mode_compile (specified_mode);
      എങ്കിൽ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_മൂല്യം = umask (0);
      umask (umask_മൂല്യം);
      newmode = mode_adjust (newmode, false, umask_മൂല്യം, change, ശൂന്യ);
      സ്വതന്ത്രം (change);
      എങ്കിൽ (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  /* If the number of arguments is 0 or 1,
     or (if it's 2 or more and the second one starts with 'p'), then there
     must be exactly two operands.  Otherwise, there must be four.  */
  പൂർണ്ണസംഖ്യ expected_operands = (argc <= optind
                           || (optind + 1 < argc && argv[optind + 1][0] == 'p')
                           ? 2 : 4);

  എങ്കിൽ (argc - optind < expected_operands)
    {
      എങ്കിൽ (argc <= optind)
        error (0, 0, _("missing operand"));
      അല്ലെങ്കിൽ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      എങ്കിൽ (expected_operands == 4 && argc - optind == 2)
        fprintf (stderr, "%s\n",
                 _("Special files require major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (expected_operands < argc - optind)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[optind + expected_operands]));
      എങ്കിൽ (expected_operands == 2 && argc - optind == 4)
        fprintf (stderr, "%s\n",
                 _("Fifos do not have major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (scontext)
    {
      പൂർണ്ണസംഖ്യ ret = 0;
      എങ്കിൽ (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      അല്ലെങ്കിൽ
        ret = setfscreatecon (scontext);

      എങ്കിൽ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  /* Only check the first character, to allow mnemonic usage like
     'mknod /dev/rst0 character 18 0'. */

  mode_t കണ്ണി_type;
  തിരഞ്ഞെടുപ്പ് (argv[optind + 1][0])
    {
    അവസ്ഥ 'b':			/* 'block' or 'buffered' */
#ifndef S_IFBLK
      error (EXIT_FAILURE, 0, _("block special files not supported"));
#അല്ലെങ്കിൽ
      കണ്ണി_type = S_IFBLK;
#endif
      പോകുക block_or_character;

    അവസ്ഥ 'c':			/* 'character' */
    അവസ്ഥ 'u':			/* 'unbuffered' */
#ifndef S_IFCHR
      error (EXIT_FAILURE, 0, _("character special files not supported"));
#അല്ലെങ്കിൽ
      കണ്ണി_type = S_IFCHR;
#endif
      പോകുക block_or_character;

    block_or_character:
      {
        അക്ഷരം സ്ഥിരം *s_major = argv[optind + 2];
        uintmax_t i_major;
        എങ്കിൽ (xstrtoumax (s_major, ശൂന്യ, 0, &i_major, "") != LONGINT_OK
            || i_major != (major_t) i_major)
          error (EXIT_FAILURE, 0,
                 _("invalid major device number %s"), quote (s_major));

        അക്ഷരം സ്ഥിരം *s_minor = argv[optind + 3];
        uintmax_t i_minor;
        എങ്കിൽ (xstrtoumax (s_minor, ശൂന്യ, 0, &i_minor, "") != LONGINT_OK
            || i_minor != (minor_t) i_minor)
          error (EXIT_FAILURE, 0,
                 _("invalid minor device number %s"), quote (s_minor));

        dev_t device = makedev (i_major, i_minor);
#ifdef NODEV
        എങ്കിൽ (device == NODEV)
          error (EXIT_FAILURE, 0, _("invalid device %s %s"),
                 s_major, s_minor);
#endif

        എങ്കിൽ (set_security_context)
          defaultcon (set_security_context, argv[optind], കണ്ണി_type);

        എങ്കിൽ (mknod (argv[optind], newmode | കണ്ണി_type, device) != 0)
          error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      }
      നിർത്തുക;

    അവസ്ഥ 'p':			/* 'pipe' */
      എങ്കിൽ (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      എങ്കിൽ (mkfifo (argv[optind], newmode) != 0)
        error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      error (0, 0, _("invalid device type %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  എങ്കിൽ (specified_mode && lchmod (argv[optind], newmode) != 0)
    error (EXIT_FAILURE, errno, _("cannot set permissions of %s"),
           quoteaf (argv[optind]));

  തിരികെ EXIT_SUCCESS;
}
