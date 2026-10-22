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

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sync"

#define AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ifndef HAVE_SYNCFS
# define HAVE_SYNCFS 0
#endif

गणना sync_mode
{
  MODE_FILE,
  MODE_DATA,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"data", no_argument, रिक्त, 'd'},
  {"file-system", no_argument, रिक्त, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क status)
{
  यदि (status != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
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

स्थिर bool
sync_arg (गणना sync_mode mode, वर्ण ध्रुव *file)
{
  पूर्णाङ्क open_flags = O_RDONLY | O_NONBLOCK;

#यदि defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  यदि (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  पूर्णाङ्क fd = open (file, open_flags);
  यदि (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      पूर्णाङ्क rd_errno = errno;
      यदि (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      यदि (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          प्रत्यागम false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  पूर्णाङ्क fdflags = fcntl (fd, F_GETFL);
  यदि (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  यदि (ret)
    {
      पूर्णाङ्क sync_status = -1;

      विकल्प (mode)
        {
        पक्ष MODE_DATA:
          sync_status = fdatasync (fd);
          भङ्ग;

        पक्ष MODE_FILE:
          sync_status = fsync (fd);
          भङ्ग;

        पक्ष MODE_FILE_SYSTEM:
#यदि HAVE_SYNCFS
          sync_status = syncfs (fd);
          भङ्ग;
#endif
        पक्ष MODE_SYNC: स्वभाव:
          affirm (false);
        }

      यदि (sync_status < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  यदि (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  प्रत्यागम ret;
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  bool arg_data = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  पूर्णाङ्क c;
  यावत् ((c = getopt_long (argc, argv, "df", long_options, रिक्त))
         != -1)
    {
      विकल्प (c)
        {
        पक्ष 'd':
          arg_data = true;
          भङ्ग;

        पक्ष 'f':
          arg_file_system = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  bool args_specified = optind < argc;

  यदि (arg_data && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  यदि (!args_specified && arg_data)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  गणना sync_mode mode;
  यदि (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  अन्यथा यदि (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  अन्यथा यदि (! arg_data)
    mode = MODE_FILE;
  अन्यथा
    mode = MODE_DATA;

  यदि (mode == MODE_SYNC)
    sync ();
  अन्यथा
    {
      प्रति (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
