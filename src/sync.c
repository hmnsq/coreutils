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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sync"

#define AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ifndef HAVE_SYNCFS
# define HAVE_SYNCFS 0
#endif

ગણના sync_mode
{
  MODE_FILE,
  MODE_DATA,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

સ્થિર સંરચના option અચલ long_options[] =
{
  {"data", no_argument, ખાલી, 'd'},
  {"file-system", no_argument, ખાલી, 'f'},
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

સ્થિર bool
sync_arg (ગણના sync_mode mode, અક્ષર અચલ *file)
{
  પૂર્ણાંક open_flags = O_RDONLY | O_NONBLOCK;

#જો defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  જો (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  પૂર્ણાંક fd = open (file, open_flags);
  જો (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      પૂર્ણાંક rd_errno = errno;
      જો (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      જો (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          પાછા false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  પૂર્ણાંક fdflags = fcntl (fd, F_GETFL);
  જો (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  જો (ret)
    {
      પૂર્ણાંક sync_status = -1;

      પસંદગી (mode)
        {
        સ્થિતિ MODE_DATA:
          sync_status = fdatasync (fd);
          તોડો;

        સ્થિતિ MODE_FILE:
          sync_status = fsync (fd);
          તોડો;

        સ્થિતિ MODE_FILE_SYSTEM:
#જો HAVE_SYNCFS
          sync_status = syncfs (fd);
          તોડો;
#endif
        સ્થિતિ MODE_SYNC: મૂળભૂત:
          affirm (false);
        }

      જો (sync_status < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  જો (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  પાછા ret;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  bool arg_data = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  પૂર્ણાંક c;
  જ્યારે ((c = getopt_long (argc, argv, "df", long_options, ખાલી))
         != -1)
    {
      પસંદગી (c)
        {
        સ્થિતિ 'd':
          arg_data = true;
          તોડો;

        સ્થિતિ 'f':
          arg_file_system = true;
          તોડો;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  bool args_specified = optind < argc;

  જો (arg_data && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  જો (!args_specified && arg_data)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  ગણના sync_mode mode;
  જો (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  નહીંતર જો (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  નહીંતર જો (! arg_data)
    mode = MODE_FILE;
  નહીંતર
    mode = MODE_DATA;

  જો (mode == MODE_SYNC)
    sync ();
  નહીંતર
    {
      માટે (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  પાછા ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
