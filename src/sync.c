/* sync - update the super block
   Copyright (C) 1994-2026 Free Software Foundation, Inc.

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

/* Written by Jim Meyering */

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sync"

#define AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ifndef HAVE_SYNCFS
# define HAVE_SYNCFS 0
#endif

എണ്ണൽ sync_mode
{
  MODE_FILE,
  MODE_DATA,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

സ്ഥിതം ഘടന option സ്ഥിരം long_options[] =
{
  {"data", no_argument, ശൂന്യ, 'd'},
  {"file-system", no_argument, ശൂന്യ, 'f'},
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
      printf (_("Usage: %s [OPTION] [FILE]...\n"), program_name);
      fputs (_("\
Synchronize cached writes to persistent storage\n\
\n\
If one or more files are specified, sync only them,\n\
or their containing file systems.\n\
\n\
"), stdout);

      oputs (_("\
  -d, --data             sync only file data, no unneeded metadata\n\
"));
      oputs (_("\
  -f, --file-system      sync the file systems that contain the files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

/* Sync the specified FILE, or file systems associated with FILE.
   Return 1 on success.  */

സ്ഥിതം bool
sync_arg (എണ്ണൽ sync_mode mode, അക്ഷരം സ്ഥിരം *file)
{
  പൂർണ്ണസംഖ്യ open_flags = O_RDONLY | O_NONBLOCK;

#എങ്കിൽ defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  എങ്കിൽ (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  പൂർണ്ണസംഖ്യ fd = open (file, open_flags);
  എങ്കിൽ (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      പൂർണ്ണസംഖ്യ rd_errno = errno;
      എങ്കിൽ (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      എങ്കിൽ (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          തിരികെ false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  പൂർണ്ണസംഖ്യ fdflags = fcntl (fd, F_GETFL);
  എങ്കിൽ (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  എങ്കിൽ (ret)
    {
      പൂർണ്ണസംഖ്യ sync_status = -1;

      തിരഞ്ഞെടുപ്പ് (mode)
        {
        അവസ്ഥ MODE_DATA:
          sync_status = fdatasync (fd);
          നിർത്തുക;

        അവസ്ഥ MODE_FILE:
          sync_status = fsync (fd);
          നിർത്തുക;

        അവസ്ഥ MODE_FILE_SYSTEM:
#എങ്കിൽ HAVE_SYNCFS
          sync_status = syncfs (fd);
          നിർത്തുക;
#endif
        അവസ്ഥ MODE_SYNC: സ്ഥിരസ്ഥിതി:
          affirm (false);
        }

      എങ്കിൽ (sync_status < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  എങ്കിൽ (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  bool arg_data = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  പൂർണ്ണസംഖ്യ c;
  ആകുമ്പോൾ ((c = getopt_long (argc, argv, "df", long_options, ശൂന്യ))
         != -1)
    {
      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'd':
          arg_data = true;
          നിർത്തുക;

        അവസ്ഥ 'f':
          arg_file_system = true;
          നിർത്തുക;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  bool args_specified = optind < argc;

  എങ്കിൽ (arg_data && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  എങ്കിൽ (!args_specified && arg_data)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  എണ്ണൽ sync_mode mode;
  എങ്കിൽ (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  അല്ലെങ്കിൽ എങ്കിൽ (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  അല്ലെങ്കിൽ എങ്കിൽ (! arg_data)
    mode = MODE_FILE;
  അല്ലെങ്കിൽ
    mode = MODE_DATA;

  എങ്കിൽ (mode == MODE_SYNC)
    sync ();
  അല്ലെങ്കിൽ
    {
      വേണ്ടി (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  തിരികെ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
