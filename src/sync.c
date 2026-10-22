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

#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "sync"

#నిర్వచించు AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#నిర్వచితం_కాకపోతే HAVE_SYNCFS
# define HAVE_SYNCFS 0
#అయితే_ముగింపు

గణన sync_బహుళకం
{
  బహుళకం_FILE,
  బహుళకం_దత్తాంశం,
  బహుళకం_FILE_SYSTEM,
  బహుళకం_SYNC
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"data", no_వాదం, ఖాళీ, 'd'},
  {"file-system", no_వాదం, ఖాళీ, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("Usage: %s [OPTION] [FILE]...\n"), కార్యక్రమం_name);
      fputs (_("\
Synchronize cached writes గమ్యం persistent storage\n\
\n\
If one or మరిన్ని files are specified, sync only them,\n\
or their containing file systems.\n\
\n\
"), stdout);

      oputs (_("\
  -d, --దత్తాంశం             sync only file దత్తాంశం, no unneeded metadata\n\
"));
      oputs (_("\
  -f, --file-system      sync the file systems that contain the files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Sync the specified FILE, or file systems associated with FILE.
   Return 1 on success.  */

స్థిర bool
sync_arg (గణన sync_బహుళకం బహుళకం_2, అక్షరం స్థిరరాశి *file)
{
  పూర్ణాంకం open_flags = O_RDONLY | O_NONBLOCK;

#ఒకవేళ defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  ఒకవేళ (బహుళకం_2 == బహుళకం_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#అయితే_ముగింపు

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  పూర్ణాంకం fd = open (file, open_flags);
  ఒకవేళ (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      పూర్ణాంకం rd_errno = errno;
      ఒకవేళ (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      ఒకవేళ (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          తిరిగి false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  పూర్ణాంకం fdflags = fcntl (fd, F_GETFL);
  ఒకవేళ (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  ఒకవేళ (ret)
    {
      పూర్ణాంకం sync_స్థితి = -1;

      ఎంపిక (బహుళకం_2)
        {
        స్థితి బహుళకం_దత్తాంశం:
          sync_స్థితి = fdatasync (fd);
          ఆపు;

        స్థితి బహుళకం_FILE:
          sync_స్థితి = fsync (fd);
          ఆపు;

        స్థితి బహుళకం_FILE_SYSTEM:
#ఒకవేళ HAVE_SYNCFS
          sync_స్థితి = syncfs (fd);
          ఆపు;
#అయితే_ముగింపు
        స్థితి బహుళకం_SYNC: అప్రమేయ:
          affirm (false);
        }

      ఒకవేళ (sync_స్థితి < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  ఒకవేళ (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  తిరిగి ret;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool arg_దత్తాంశం = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  పూర్ణాంకం c;
  అయితే ((c = getopt_long (argc, argv, "df", long_options, ఖాళీ))
         != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'd':
          arg_దత్తాంశం = true;
          ఆపు;

        స్థితి 'f':
          arg_file_system = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  bool వాదనలు_specified = optind < argc;

  ఒకవేళ (arg_దత్తాంశం && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  ఒకవేళ (!వాదనలు_specified && arg_దత్తాంశం)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  గణన sync_బహుళకం బహుళకం_2;
  ఒకవేళ (! వాదనలు_specified || (arg_file_system && ! HAVE_SYNCFS))
    బహుళకం_2 = బహుళకం_SYNC;
  లేకపోతే ఒకవేళ (arg_file_system)
    బహుళకం_2 = బహుళకం_FILE_SYSTEM;
  లేకపోతే ఒకవేళ (! arg_దత్తాంశం)
    బహుళకం_2 = బహుళకం_FILE;
  లేకపోతే
    బహుళకం_2 = బహుళకం_దత్తాంశం;

  ఒకవేళ (బహుళకం_2 == బహుళకం_SYNC)
    sync ();
  లేకపోతే
    {
      కొరకు (; optind < argc; optind++)
        ok &= sync_arg (బహుళకం_2, argv[optind]);
    }

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
