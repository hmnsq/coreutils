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
usage (ຈຳນວນເຕັມ status)
{
  ຖ້າ (status != EXIT_SUCCESS)
    emit_try_help ();
  ບໍ່ດັ່ງນັ້ນ
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
  ຈຳນວນເຕັມ open_flags = O_RDONLY | O_NONBLOCK;

#ຖ້າ defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  ຖ້າ (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  ຈຳນວນເຕັມ fd = open (file, open_flags);
  ຖ້າ (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      ຈຳນວນເຕັມ rd_errno = errno;
      ຖ້າ (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      ຖ້າ (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          ສົ່ງຄືນ false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  ຈຳນວນເຕັມ fdflags = fcntl (fd, F_GETFL);
  ຖ້າ (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  ຖ້າ (ret)
    {
      ຈຳນວນເຕັມ sync_status = -1;

      switch (mode)
        {
        pak_sa MODE_DATA:
          sync_status = fdatasync (fd);
          bha_nga;

        pak_sa MODE_FILE:
          sync_status = fsync (fd);
          bha_nga;

        pak_sa MODE_FILE_SYSTEM:
#ຖ້າ HAVE_SYNCFS
          sync_status = syncfs (fd);
          bha_nga;
#endif
        pak_sa MODE_SYNC: default:
          affirm (false);
        }

      ຖ້າ (sync_status < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  ຖ້າ (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ
main (ຈຳນວນເຕັມ argc, var_na **argv)
{
  bool arg_data = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ຈຳນວນເຕັມ c;
  ໃນຂະນະທີ່ ((c = getopt_long (argc, argv, "df", long_options, NULL))
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

  ຖ້າ (arg_data && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  ຖ້າ (!args_specified && arg_data)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  ga_nanaa sync_mode mode;
  ຖ້າ (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  ບໍ່ດັ່ງນັ້ນ ຖ້າ (! arg_data)
    mode = MODE_FILE;
  ບໍ່ດັ່ງນັ້ນ
    mode = MODE_DATA;

  ຖ້າ (mode == MODE_SYNC)
    sync ();
  ບໍ່ດັ່ງນັ້ນ
    {
      ສຳລັບ (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  ສົ່ງຄືນ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
