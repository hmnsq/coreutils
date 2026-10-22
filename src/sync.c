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
#تعریف_کرو پروگرام_NAME "sync"

#تعریف_کرو AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#جے_بنا_تعریف HAVE_SYNCFS
# define HAVE_SYNCFS 0
#جے_انت

تعداد sync_منوال
{
  منوال_FILE,
  منوال_انکڑے,
  منوال_FILE_SYSTEM,
  منوال_SYNC
};

ساكن بنية option ثابت long_options[] =
{
  {"data", no_دلیل, عدم, 'd'},
  {"file-system", no_دلیل, عدم, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [OPTION] [FILE]...\n"), پروگرام_name);
      fputs (_("\
Synchronize cached writes منزل persistent storage\n\
\n\
If one or ہور files are specified, sync only them,\n\
or their containing file systems.\n\
\n\
"), stdout);

      oputs (_("\
  -d, --انکڑے             sync only file انکڑے, no unneeded metadata\n\
"));
      oputs (_("\
  -f, --file-system      sync the file systems that contain the files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

/* Sync the specified FILE, or file systems associated with FILE.
   Return 1 on success.  */

ساكن bool
sync_arg (تعداد sync_منوال منوال_2, حرف ثابت *file)
{
  صحيح open_flags = O_RDONLY | O_NONBLOCK;

#إذا defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  إذا (منوال_2 == منوال_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#جے_انت

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

      بدل (منوال_2)
        {
        حالة منوال_انکڑے:
          sync_حالت = fdatasync (fd);
          اكسر;

        حالة منوال_FILE:
          sync_حالت = fsync (fd);
          اكسر;

        حالة منوال_FILE_SYSTEM:
#إذا HAVE_SYNCFS
          sync_حالت = syncfs (fd);
          اكسر;
#جے_انت
        حالة منوال_SYNC: افتراضي:
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
مکھ (صحيح argc, حرف **argv)
{
  bool arg_انکڑے = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
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
          arg_انکڑے = true;
          اكسر;

        حالة 'f':
          arg_file_system = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  bool دلیلاں_specified = optind < argc;

  إذا (arg_انکڑے && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  إذا (!دلیلاں_specified && arg_انکڑے)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  تعداد sync_منوال منوال_2;
  إذا (! دلیلاں_specified || (arg_file_system && ! HAVE_SYNCFS))
    منوال_2 = منوال_SYNC;
  وإلا إذا (arg_file_system)
    منوال_2 = منوال_FILE_SYSTEM;
  وإلا إذا (! arg_انکڑے)
    منوال_2 = منوال_FILE;
  وإلا
    منوال_2 = منوال_انکڑے;

  إذا (منوال_2 == منوال_SYNC)
    sync ();
  وإلا
    {
      لكل (; optind < argc; optind++)
        ok &= sync_arg (منوال_2, argv[optind]);
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
