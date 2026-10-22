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

#شامل <config.h>
#شامل <getopt.h>
#شامل <stdio.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sync"

#define AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ifndef HAVE_SYNCFS
# define HAVE_SYNCFS 0
#endif

شمارش sync_mode
{
  MODE_FILE,
  MODE_DATA,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

ایستا ساختار option ثابت long_options[] =
{
  {"data", no_argument, پوچ, 'd'},
  {"file-system", no_argument, پوچ, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
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

ایستا bool
sync_arg (شمارش sync_mode mode, نویسه ثابت *file)
{
  صحیح open_flags = O_RDONLY | O_NONBLOCK;

#اگر defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  اگر (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  صحیح fd = open (file, open_flags);
  اگر (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      صحیح rd_errno = errno;
      اگر (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      اگر (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          بازگشت false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  صحیح fdflags = fcntl (fd, F_GETFL);
  اگر (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  اگر (ret)
    {
      صحیح sync_status = -1;

      انتخاب (mode)
        {
        حالت MODE_DATA:
          sync_status = fdatasync (fd);
          بشکن;

        حالت MODE_FILE:
          sync_status = fsync (fd);
          بشکن;

        حالت MODE_FILE_SYSTEM:
#اگر HAVE_SYNCFS
          sync_status = syncfs (fd);
          بشکن;
#endif
        حالت MODE_SYNC: پیشفرض:
          affirm (false);
        }

      اگر (sync_status < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  اگر (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  بازگشت ret;
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  bool arg_data = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحیح c;
  وقتی ((c = getopt_long (argc, argv, "df", long_options, پوچ))
         != -1)
    {
      انتخاب (c)
        {
        حالت 'd':
          arg_data = true;
          بشکن;

        حالت 'f':
          arg_file_system = true;
          بشکن;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  bool args_specified = optind < argc;

  اگر (arg_data && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  اگر (!args_specified && arg_data)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  شمارش sync_mode mode;
  اگر (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  وگرنه اگر (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  وگرنه اگر (! arg_data)
    mode = MODE_FILE;
  وگرنه
    mode = MODE_DATA;

  اگر (mode == MODE_SYNC)
    sync ();
  وگرنه
    {
      برای (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  بازگشت ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
