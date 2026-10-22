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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sync"

#define AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ifndef HAVE_SYNCFS
# define HAVE_SYNCFS 0
#endif

ಗಣನಾ sync_mode
{
  MODE_FILE,
  MODE_DATA,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_options[] =
{
  {"data", no_argument, ರಿಕ್ತ, 'd'},
  {"file-system", no_argument, ರಿಕ್ತ, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
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

ಸ್ಥಿರ bool
sync_arg (ಗಣನಾ sync_mode mode, ವರ್ಣ ಧ್ರುವ *file)
{
  ಪೂರ್ಣಾಙ್ಕ open_flags = O_RDONLY | O_NONBLOCK;

#ಯದಿ defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  ಯದಿ (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  ಪೂರ್ಣಾಙ್ಕ fd = open (file, open_flags);
  ಯದಿ (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      ಪೂರ್ಣಾಙ್ಕ rd_errno = errno;
      ಯದಿ (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      ಯದಿ (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          ಪ್ರತ್ಯಾಗಮ false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  ಪೂರ್ಣಾಙ್ಕ fdflags = fcntl (fd, F_GETFL);
  ಯದಿ (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  ಯದಿ (ret)
    {
      ಪೂರ್ಣಾಙ್ಕ sync_status = -1;

      ವಿಕಲ್ಪ (mode)
        {
        ಪಕ್ಷ MODE_DATA:
          sync_status = fdatasync (fd);
          ಭಙ್ಗ;

        ಪಕ್ಷ MODE_FILE:
          sync_status = fsync (fd);
          ಭಙ್ಗ;

        ಪಕ್ಷ MODE_FILE_SYSTEM:
#ಯದಿ HAVE_SYNCFS
          sync_status = syncfs (fd);
          ಭಙ್ಗ;
#endif
        ಪಕ್ಷ MODE_SYNC: ಸ್ವಭಾವ:
          affirm (false);
        }

      ಯದಿ (sync_status < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  ಯದಿ (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  bool arg_data = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಙ್ಕ c;
  ಯಾವತ್ ((c = getopt_long (argc, argv, "df", long_options, ರಿಕ್ತ))
         != -1)
    {
      ವಿಕಲ್ಪ (c)
        {
        ಪಕ್ಷ 'd':
          arg_data = true;
          ಭಙ್ಗ;

        ಪಕ್ಷ 'f':
          arg_file_system = true;
          ಭಙ್ಗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ಸ್ವಭಾವ:
          usage (EXIT_FAILURE);
        }
    }

  bool args_specified = optind < argc;

  ಯದಿ (arg_data && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  ಯದಿ (!args_specified && arg_data)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  ಗಣನಾ sync_mode mode;
  ಯದಿ (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  ಅನ್ಯಥಾ ಯದಿ (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  ಅನ್ಯಥಾ ಯದಿ (! arg_data)
    mode = MODE_FILE;
  ಅನ್ಯಥಾ
    mode = MODE_DATA;

  ಯದಿ (mode == MODE_SYNC)
    sync ();
  ಅನ್ಯಥಾ
    {
      ಪ್ರತಿ (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  ಪ್ರತ್ಯಾಗಮ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
