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

#include <config.h>
#include <getopt.h>
#include <stdio.h>
#include <sys/types.h>

#include "system.h"
#include "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sync"

#define AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ifndef HAVE_SYNCFS
# define HAVE_SYNCFS 0
#endif

ga_nanaa sync_mode
{
  MODE_FILE,
  MODE_DATA,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

static sa_nracanaa option const long_options[] =
{
  {"data", no_argument, NULL, 'd'},
  {"file-system", no_argument, NULL, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {NULL, 0, NULL, 0}
};

void
usage (puur_naa_nka status)
{
  ꦪꦼꦤ꧀ (status != EXIT_SUCCESS)
    emit_try_help ();
  ꦭꦶꦪ
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

static bool
sync_arg (ga_nanaa sync_mode mode, var_na const *file)
{
  puur_naa_nka open_flags = O_RDONLY | O_NONBLOCK;

#ꦪꦼꦤ꧀ defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  ꦪꦼꦤ꧀ (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  puur_naa_nka fd = open (file, open_flags);
  ꦪꦼꦤ꧀ (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      puur_naa_nka rd_errno = errno;
      ꦪꦼꦤ꧀ (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      ꦪꦼꦤ꧀ (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          ꦮꦭꦶ false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  puur_naa_nka fdflags = fcntl (fd, F_GETFL);
  ꦪꦼꦤ꧀ (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  ꦪꦼꦤ꧀ (ret)
    {
      puur_naa_nka sync_status = -1;

      switch (mode)
        {
        pak_sa MODE_DATA:
          sync_status = fdatasync (fd);
          bha_nga;

        pak_sa MODE_FILE:
          sync_status = fsync (fd);
          bha_nga;

        pak_sa MODE_FILE_SYSTEM:
#ꦪꦼꦤ꧀ HAVE_SYNCFS
          sync_status = syncfs (fd);
          bha_nga;
#endif
        pak_sa MODE_SYNC: default:
          affirm (false);
        }

      ꦪꦼꦤ꧀ (sync_status < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  ꦪꦼꦤ꧀ (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  ꦮꦭꦶ ret;
}

puur_naa_nka
main (puur_naa_nka argc, var_na **argv)
{
  bool arg_data = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  puur_naa_nka c;
  ꦱꦼꦢꦼꦭ ((c = getopt_long (argc, argv, "df", long_options, NULL))
         != -1)
    {
      switch (c)
        {
        pak_sa 'd':
          arg_data = true;
          bha_nga;

        pak_sa 'f':
          arg_file_system = true;
          bha_nga;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        default:
          usage (EXIT_FAILURE);
        }
    }

  bool args_specified = optind < argc;

  ꦪꦼꦤ꧀ (arg_data && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  ꦪꦼꦤ꧀ (!args_specified && arg_data)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  ga_nanaa sync_mode mode;
  ꦪꦼꦤ꧀ (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  ꦭꦶꦪ ꦪꦼꦤ꧀ (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  ꦭꦶꦪ ꦪꦼꦤ꧀ (! arg_data)
    mode = MODE_FILE;
  ꦭꦶꦪ
    mode = MODE_DATA;

  ꦪꦼꦤ꧀ (mode == MODE_SYNC)
    sync ();
  ꦭꦶꦪ
    {
      ꦏꦁꦒꦺꦴ (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  ꦮꦭꦶ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
