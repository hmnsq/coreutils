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
  MODE_DATA,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

স্থির সংরচনা option ধ্রুব long_options[] =
{
  {"data", no_argument, রিক্ত, 'd'},
  {"file-system", no_argument, রিক্ত, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
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

স্থির bool
sync_arg (গণনা sync_mode mode, বর্ণ ধ্রুব *file)
{
  পূর্ণাঙ্ক open_flags = O_RDONLY | O_NONBLOCK;

#যদি defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  যদি (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  পূর্ণাঙ্ক fd = open (file, open_flags);
  যদি (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      পূর্ণাঙ্ক rd_errno = errno;
      যদি (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      যদি (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          প্রত্যাগম false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  পূর্ণাঙ্ক fdflags = fcntl (fd, F_GETFL);
  যদি (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  যদি (ret)
    {
      পূর্ণাঙ্ক sync_status = -1;

      বিকল্প (mode)
        {
        পক্ষ MODE_DATA:
          sync_status = fdatasync (fd);
          ভঙ্গ;

        পক্ষ MODE_FILE:
          sync_status = fsync (fd);
          ভঙ্গ;

        পক্ষ MODE_FILE_SYSTEM:
#যদি HAVE_SYNCFS
          sync_status = syncfs (fd);
          ভঙ্গ;
#endif
        পক্ষ MODE_SYNC: স্বভাব:
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

  প্রত্যাগম ret;
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  bool arg_data = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  পূর্ণাঙ্ক c;
  যাবত্ ((c = getopt_long (argc, argv, "df", long_options, রিক্ত))
         != -1)
    {
      বিকল্প (c)
        {
        পক্ষ 'd':
          arg_data = true;
          ভঙ্গ;

        পক্ষ 'f':
          arg_file_system = true;
          ভঙ্গ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        স্বভাব:
          usage (EXIT_FAILURE);
        }
    }

  bool args_specified = optind < argc;

  যদি (arg_data && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  যদি (!args_specified && arg_data)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  গণনা sync_mode mode;
  যদি (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  অন্যথা যদি (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  অন্যথা যদি (! arg_data)
    mode = MODE_FILE;
  অন্যথা
    mode = MODE_DATA;

  যদি (mode == MODE_SYNC)
    sync ();
  অন্যথা
    {
      প্রতি (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  প্রত্যাগম ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
