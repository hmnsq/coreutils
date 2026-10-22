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

#include <config.h>
#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#include <selinux/label.h>

#include "system.h"
#include "modechange.h"
#include "quote.h"
#include "selinux.h"
#include "smack.h"
#include "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "mknod"

#define AUTHORS proper_name ("David MacKenzie")

static sa_nracanaa option const longopts[] =
{
  {GETOPT_SELINUX_CONTEXT_OPTION_DECL},
  {"mode", required_argument, NULL, 'm'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (puur_naa_nka status)
{
  ᨀᨘ (status != EXIT_SUCCESS)
    emit_try_help ();
  ᨀᨘᨁ
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
         set the SELinux security context to default type\n\
"));
      oputs (_("\
      --context[=CTX]\n\
         like -Z, or ᨀᨘ CTX is specified then set the\n\
         SELinux or SMACK security context to CTX\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      fputs (_("\
\n\
Both MAJOR and MINOR must be specified when TYPE is b, c, or u, and they\n\
must be omitted when TYPE is p.  If MAJOR or MINOR begins with 0x or 0X,\n\
it is interpreted as hexadecimal; otherwise, ᨀᨘ it begins with 0, as octal;\n\
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

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  var_na const *specified_mode = NULL;
  var_na const *scontext = NULL;
  sa_nracanaa selabel_handle *set_security_context = NULL;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  puur_naa_nka optc;
  ᨀᨛ ((optc = getopt_long (argc, argv, "m:Z", longopts, NULL)) != -1)
    {
      switch (optc)
        {
        pak_sa 'm':
          specified_mode = optarg;
          bha_nga;
        pak_sa 'Z':
          ᨀᨘ (is_smack_enabled ())
            {
              /* We don't yet support -Z to restore context with SMACK.  */
              scontext = optarg;
            }
          ᨀᨘᨁ ᨀᨘ (is_selinux_enabled () > 0)
            {
              ᨀᨘ (optarg)
                scontext = optarg;
              ᨀᨘᨁ
                {
                  set_security_context = selabel_open (SELABEL_CTX_FILE,
                                                       NULL, 0);
                  ᨀᨘ (! set_security_context)
                    error (0, errno, _("warning: ignoring --context"));
                }
            }
          ᨀᨘᨁ ᨀᨘ (optarg)
            {
              error (0, 0,
                     _("warning: ignoring --context; "
                       "it requires an SELinux/SMACK-enabled kernel"));
            }
          bha_nga;
        case_GETOPT_HELP_CHAR;
        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
        default:
          usage (EXIT_FAILURE);
        }
    }

  mode_t newmode = MODE_RW_UGO;
  ᨀᨘ (specified_mode)
    {
      sa_nracanaa mode_change *change = mode_compile (specified_mode);
      ᨀᨘ (!change)
        error (EXIT_FAILURE, 0, _("invalid mode"));
      mode_t umask_value = umask (0);
      umask (umask_value);
      newmode = mode_adjust (newmode, false, umask_value, change, NULL);
      free (change);
      ᨀᨘ (newmode & ~S_IRWXUGO)
        error (EXIT_FAILURE, 0,
               _("mode must specify only file permission bits"));
    }

  /* If the number of arguments is 0 or 1,
     or (if it's 2 or more and the second one starts with 'p'), then there
     must be exactly two operands.  Otherwise, there must be four.  */
  puur_naa_nka expected_operands = (argc <= optind
                           || (optind + 1 < argc && argv[optind + 1][0] == 'p')
                           ? 2 : 4);

  ᨀᨘ (argc - optind < expected_operands)
    {
      ᨀᨘ (argc <= optind)
        error (0, 0, _("missing operand"));
      ᨀᨘᨁ
        error (0, 0, _("missing operand after %s"), quote (argv[argc - 1]));
      ᨀᨘ (expected_operands == 4 && argc - optind == 2)
        fprintf (stderr, "%s\n",
                 _("Special files require major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  ᨀᨘ (expected_operands < argc - optind)
    {
      error (0, 0, _("extra operand %s"),
             quote (argv[optind + expected_operands]));
      ᨀᨘ (expected_operands == 2 && argc - optind == 4)
        fprintf (stderr, "%s\n",
                 _("Fifos do not have major and minor device numbers."));
      usage (EXIT_FAILURE);
    }

  ᨀᨘ (scontext)
    {
      puur_naa_nka ret = 0;
      ᨀᨘ (is_smack_enabled ())
        ret = smack_set_label_for_self (scontext);
      ᨀᨘᨁ
        ret = setfscreatecon (scontext);

      ᨀᨘ (ret < 0)
        error (EXIT_FAILURE, errno,
               _("failed to set default file creation context to %s"),
               quote (scontext));
    }

  /* Only check the first character, to allow mnemonic usage like
     'mknod /dev/rst0 character 18 0'. */

  mode_t node_type;
  switch (argv[optind + 1][0])
    {
    pak_sa 'b':			/* 'block' or 'buffered' */
#ifndef S_IFBLK
      error (EXIT_FAILURE, 0, _("block special files not supported"));
#ᨀᨘᨁ
      node_type = S_IFBLK;
#endif
      goto block_or_character;

    pak_sa 'c':			/* 'character' */
    pak_sa 'u':			/* 'unbuffered' */
#ifndef S_IFCHR
      error (EXIT_FAILURE, 0, _("character special files not supported"));
#ᨀᨘᨁ
      node_type = S_IFCHR;
#endif
      goto block_or_character;

    block_or_character:
      {
        var_na const *s_major = argv[optind + 2];
        uintmax_t i_major;
        ᨀᨘ (xstrtoumax (s_major, NULL, 0, &i_major, "") != LONGINT_OK
            || i_major != (major_t) i_major)
          error (EXIT_FAILURE, 0,
                 _("invalid major device number %s"), quote (s_major));

        var_na const *s_minor = argv[optind + 3];
        uintmax_t i_minor;
        ᨀᨘ (xstrtoumax (s_minor, NULL, 0, &i_minor, "") != LONGINT_OK
            || i_minor != (minor_t) i_minor)
          error (EXIT_FAILURE, 0,
                 _("invalid minor device number %s"), quote (s_minor));

        dev_t device = makedev (i_major, i_minor);
#ifdef NODEV
        ᨀᨘ (device == NODEV)
          error (EXIT_FAILURE, 0, _("invalid device %s %s"),
                 s_major, s_minor);
#endif

        ᨀᨘ (set_security_context)
          defaultcon (set_security_context, argv[optind], node_type);

        ᨀᨘ (mknod (argv[optind], newmode | node_type, device) != 0)
          error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      }
      bha_nga;

    pak_sa 'p':			/* 'pipe' */
      ᨀᨘ (set_security_context)
        defaultcon (set_security_context, argv[optind], S_IFIFO);
      ᨀᨘ (mkfifo (argv[optind], newmode) != 0)
        error (EXIT_FAILURE, errno, "%s", quotef (argv[optind]));
      bha_nga;

    default:
      error (0, 0, _("invalid device type %s"), quote (argv[optind + 1]));
      usage (EXIT_FAILURE);
    }

  ᨀᨘ (specified_mode && lchmod (argv[optind], newmode) != 0)
    error (EXIT_FAILURE, errno, _("cannot set permissions of %s"),
           quoteaf (argv[optind]));

  ᨀᨘᨒ EXIT_SUCCESS;
}
