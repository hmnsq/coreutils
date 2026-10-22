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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>

#ଅନ୍ତର୍ଭାଵ "system.h"
#ଅନ୍ତର୍ଭାଵ "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "sync"

#define AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ifndef HAVE_SYNCFS
# define HAVE_SYNCFS 0
#endif

ଗଣନା sync_mode
{
  MODE_FILE,
  MODE_DATA,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_options[] =
{
  {"data", no_argument, ଖାଲି, 'd'},
  {"file-system", no_argument, ଖାଲି, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0}
};

ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
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

ସ୍ଥିର bool
sync_arg (ଗଣନା sync_mode mode, ଅକ୍ଷର ଅଚଳ *file)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା open_flags = O_RDONLY | O_NONBLOCK;

#ଯଦି defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  ଯଦି (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = open (file, open_flags);
  ଯଦି (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା rd_errno = errno;
      ଯଦି (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      ଯଦି (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          ଫେରନ୍ତୁ false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fdflags = fcntl (fd, F_GETFL);
  ଯଦି (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  ଯଦି (ret)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା sync_status = -1;

      ବାଛ (mode)
        {
        ସ୍ଥିତି MODE_DATA:
          sync_status = fdatasync (fd);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି MODE_FILE:
          sync_status = fsync (fd);
          ଭାଙ୍ଗ;

        ସ୍ଥିତି MODE_FILE_SYSTEM:
#ଯଦି HAVE_SYNCFS
          sync_status = syncfs (fd);
          ଭାଙ୍ଗ;
#endif
        ସ୍ଥିତି MODE_SYNC: ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          affirm (false);
        }

      ଯଦି (sync_status < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  ଯଦି (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  bool arg_data = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
  ଯେତେବେଳେ ((c = getopt_long (argc, argv, "df", long_options, ଖାଲି))
         != -1)
    {
      ବାଛ (c)
        {
        ସ୍ଥିତି 'd':
          arg_data = true;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'f':
          arg_file_system = true;
          ଭାଙ୍ଗ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          usage (EXIT_FAILURE);
        }
    }

  bool args_specified = optind < argc;

  ଯଦି (arg_data && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  ଯଦି (!args_specified && arg_data)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  ଗଣନା sync_mode mode;
  ଯଦି (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  ଅନ୍ୟଥା ଯଦି (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  ଅନ୍ୟଥା ଯଦି (! arg_data)
    mode = MODE_FILE;
  ଅନ୍ୟଥା
    mode = MODE_DATA;

  ଯଦି (mode == MODE_SYNC)
    sync ();
  ଅନ୍ୟଥା
    {
      ପାଇଁ (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  ଫେରନ୍ତୁ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
