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
#وصف_ڪريو پروگرام_NAME "sync"

#وصف_ڪريو AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#جيڪڏهن_وصف_نه_ٿيل HAVE_SYNCFS
# define HAVE_SYNCFS 0
#جيڪڏهن_پورو

تعداد sync_گهڻائي
{
  گهڻائي_FILE,
  گهڻائي_معلومات,
  گهڻائي_FILE_SYSTEM,
  گهڻائي_SYNC
};

ساكن بنية option ثابت long_options[] =
{
  {"data", no_دليل, عدم, 'd'},
  {"file-system", no_دليل, عدم, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخو_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکو (_("Usage: %s [OPTION] [FILE]...\n"), پروگرام_name);
      fputs (_("\
Synchronize cached writes منزل persistent storage\n\
\n\
If one or وڌيڪ files are specified, sync only them,\n\
or their containing file systems.\n\
\n\
"), stdout);

      oputs (_("\
  -d, --معلومات             sync only file معلومات, no unneeded metadata\n\
"));
      oputs (_("\
  -f, --file-system      sync the file systems that contain the files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخو_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  ٻاهر_نڪرو (حالت);
}

/* Sync the specified FILE, or file systems associated with FILE.
   Return 1 on success.  */

ساكن bool
sync_arg (تعداد sync_گهڻائي گهڻائي_2, حرف ثابت *file)
{
  صحيح open_flags = O_RDONLY | O_NONBLOCK;

#إذا defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  إذا (گهڻائي_2 == گهڻائي_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#جيڪڏهن_پورو

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
          أرجع false;
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
      صحيح sync_حالت = -1;

      بدل (گهڻائي_2)
        {
        حالة گهڻائي_معلومات:
          sync_حالت = fdatasync (fd);
          اكسر;

        حالة گهڻائي_FILE:
          sync_حالت = fsync (fd);
          اكسر;

        حالة گهڻائي_FILE_SYSTEM:
#إذا HAVE_SYNCFS
          sync_حالت = syncfs (fd);
          اكسر;
#جيڪڏهن_پورو
        حالة گهڻائي_SYNC: افتراضي:
          affirm (false);
        }

      إذا (sync_حالت < 0)
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

  أرجع ret;
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  bool arg_معلومات = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  سيٽ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحيح c;
  طالما ((c = getopt_long (argc, argv, "df", long_options, عدم))
         != -1)
    {
      بدل (c)
        {
        حالة 'd':
          arg_معلومات = true;
          اكسر;

        حالة 'f':
          arg_file_system = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخو_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  bool دليلون_specified = optind < argc;

  إذا (arg_معلومات && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  إذا (!دليلون_specified && arg_معلومات)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  تعداد sync_گهڻائي گهڻائي_2;
  إذا (! دليلون_specified || (arg_file_system && ! HAVE_SYNCFS))
    گهڻائي_2 = گهڻائي_SYNC;
  وإلا إذا (arg_file_system)
    گهڻائي_2 = گهڻائي_FILE_SYSTEM;
  وإلا إذا (! arg_معلومات)
    گهڻائي_2 = گهڻائي_FILE;
  وإلا
    گهڻائي_2 = گهڻائي_معلومات;

  إذا (گهڻائي_2 == گهڻائي_SYNC)
    sync ();
  وإلا
    {
      لكل (; optind < argc; optind++)
        ok &= sync_arg (گهڻائي_2, argv[optind]);
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
