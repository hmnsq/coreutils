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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sync"

#define AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ifndef HAVE_SYNCFS
# define HAVE_SYNCFS 0
#endif

গণনা sync_mode
{
  MODE_FILE,
  MODE_তথ্য,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

স্থির কাঠামো option ধ্রুবক long_options[] =
{
  {"data", no_argument, ফাঁকা, 'd'},
  {"file-system", no_argument, ফাঁকা, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0}
};

শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
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
  -d, --তথ্য             sync only file তথ্য, no unneeded metadata\n\
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

স্থির bool
sync_arg (গণনা sync_mode mode, অক্ষর ধ্রুবক *file)
{
  পূর্ণসংখ্যা open_flags = O_RDONLY | O_NONBLOCK;

#যদি defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  যদি (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  পূর্ণসংখ্যা fd = open (file, open_flags);
  যদি (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      পূর্ণসংখ্যা rd_errno = errno;
      যদি (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      যদি (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          ফেরত false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  পূর্ণসংখ্যা fdflags = fcntl (fd, F_GETFL);
  যদি (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  যদি (ret)
    {
      পূর্ণসংখ্যা sync_status = -1;

      নির্বাচন (mode)
        {
        ক্ষেত্র MODE_তথ্য:
          sync_status = fdatasync (fd);
          বিরতি;

        ক্ষেত্র MODE_FILE:
          sync_status = fsync (fd);
          বিরতি;

        ক্ষেত্র MODE_FILE_SYSTEM:
#যদি HAVE_SYNCFS
          sync_status = syncfs (fd);
          বিরতি;
#endif
        ক্ষেত্র MODE_SYNC: পূর্বনির্ধারিত:
          affirm (false);
        }

      যদি (sync_status < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  যদি (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  ফেরত ret;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool arg_তথ্য = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  পূর্ণসংখ্যা c;
  যখন ((c = getopt_long (argc, argv, "df", long_options, ফাঁকা))
         != -1)
    {
      নির্বাচন (c)
        {
        ক্ষেত্র 'd':
          arg_তথ্য = true;
          বিরতি;

        ক্ষেত্র 'f':
          arg_file_system = true;
          বিরতি;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পূর্বনির্ধারিত:
          usage (EXIT_FAILURE);
        }
    }

  bool args_specified = optind < argc;

  যদি (arg_তথ্য && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  যদি (!args_specified && arg_তথ্য)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  গণনা sync_mode mode;
  যদি (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  অন্যথা যদি (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  অন্যথা যদি (! arg_তথ্য)
    mode = MODE_FILE;
  অন্যথা
    mode = MODE_তথ্য;

  যদি (mode == MODE_SYNC)
    sync ();
  অন্যথা
    {
      জন্য (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  ফেরত ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
