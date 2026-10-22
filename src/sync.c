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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sync"

#define AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ifndef HAVE_SYNCFS
# define HAVE_SYNCFS 0
#endif

எண்ணிக்கை sync_mode
{
  MODE_FILE,
  MODE_DATA,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

நிலையான கட்டமைப்பு option மாறிலி long_options[] =
{
  {"data", no_argument, வெற்று, 'd'},
  {"file-system", no_argument, வெற்று, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
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

நிலையான bool
sync_arg (எண்ணிக்கை sync_mode mode, எழுத்து மாறிலி *file)
{
  முழு open_flags = O_RDONLY | O_NONBLOCK;

#எனில் defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  எனில் (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  முழு fd = open (file, open_flags);
  எனில் (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      முழு rd_errno = errno;
      எனில் (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      எனில் (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          திரும்பு false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  முழு fdflags = fcntl (fd, F_GETFL);
  எனில் (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  எனில் (ret)
    {
      முழு sync_status = -1;

      தேர்வு (mode)
        {
        நிலை MODE_DATA:
          sync_status = fdatasync (fd);
          நிறுத்து;

        நிலை MODE_FILE:
          sync_status = fsync (fd);
          நிறுத்து;

        நிலை MODE_FILE_SYSTEM:
#எனில் HAVE_SYNCFS
          sync_status = syncfs (fd);
          நிறுத்து;
#endif
        நிலை MODE_SYNC: இயல்பு:
          affirm (false);
        }

      எனில் (sync_status < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  எனில் (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  திரும்பு ret;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool arg_data = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  முழு c;
  போது ((c = getopt_long (argc, argv, "df", long_options, வெற்று))
         != -1)
    {
      தேர்வு (c)
        {
        நிலை 'd':
          arg_data = true;
          நிறுத்து;

        நிலை 'f':
          arg_file_system = true;
          நிறுத்து;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  bool args_specified = optind < argc;

  எனில் (arg_data && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  எனில் (!args_specified && arg_data)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  எண்ணிக்கை sync_mode mode;
  எனில் (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  இல்லையெனில் எனில் (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  இல்லையெனில் எனில் (! arg_data)
    mode = MODE_FILE;
  இல்லையெனில்
    mode = MODE_DATA;

  எனில் (mode == MODE_SYNC)
    sync ();
  இல்லையெனில்
    {
      க்கு (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  திரும்பு ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
