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

#ضمن <config.h>
#ضمن <getopt.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sync"

#define AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ifndef HAVE_SYNCFS
# define HAVE_SYNCFS 0
#endif

تعداد sync_mode
{
  MODE_FILE,
  MODE_DATA,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

ساكن بنية option ثابت long_options[] =
{
  {"data", no_argument, عدم, 'd'},
  {"file-system", no_argument, عدم, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح status)
{
  إذا (status != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
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

ساكن bool
sync_arg (تعداد sync_mode mode, حرف ثابت *file)
{
  صحيح open_flags = O_RDONLY | O_NONBLOCK;

#إذا defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  إذا (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  صحيح fd = open (file, open_flags);
  إذا (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      صحيح rd_errno = errno;
      إذا (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      إذا (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          إرجاع false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  صحيح fdflags = fcntl (fd, F_GETFL);
  إذا (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  إذا (ret)
    {
      صحيح sync_status = -1;

      بدل (mode)
        {
        حالة MODE_DATA:
          sync_status = fdatasync (fd);
          اكسر;

        حالة MODE_FILE:
          sync_status = fsync (fd);
          اكسر;

        حالة MODE_FILE_SYSTEM:
#إذا HAVE_SYNCFS
          sync_status = syncfs (fd);
          اكسر;
#endif
        حالة MODE_SYNC: افتراضي:
          affirm (false);
        }

      إذا (sync_status < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  إذا (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  إرجاع ret;
}

صحيح
الرئيسية (صحيح argc, حرف **argv)
{
  bool arg_data = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحيح c;
  بينما ((c = getopt_long (argc, argv, "df", long_options, عدم))
         != -1)
    {
      بدل (c)
        {
        حالة 'd':
          arg_data = true;
          اكسر;

        حالة 'f':
          arg_file_system = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  bool args_specified = optind < argc;

  إذا (arg_data && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  إذا (!args_specified && arg_data)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  تعداد sync_mode mode;
  إذا (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  وإلا إذا (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  وإلا إذا (! arg_data)
    mode = MODE_FILE;
  وإلا
    mode = MODE_DATA;

  إذا (mode == MODE_SYNC)
    sync ();
  وإلا
    {
      لكل (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  إرجاع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
