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
#تعریف_کن برنامه_NAME "sync"

#تعریف_کن AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#اگر_تعریف_نشده HAVE_SYNCFS
# define HAVE_SYNCFS 0
#پایان_اگر

تعداد sync_نما
{
  نما_FILE,
  نما_داده,
  نما_FILE_SYSTEM,
  نما_SYNC
};

ساكن بنية option ثابت long_options[] =
{
  {"data", no_آرگومان, عدم, 'd'},
  {"file-system", no_آرگومان, عدم, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخه_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح وضعیت)
{
  إذا (وضعیت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      چاپ_کن (_("Usage: %s [OPTION] [FILE]...\n"), برنامه_name);
      fputs (_("\
Synchronize cached writes مقصد persistent storage\n\
\n\
If one or بیشتر files are specified, sync only them,\n\
or their containing file systems.\n\
\n\
"), stdout);

      oputs (_("\
  -d, --داده             sync only file داده, no unneeded metadata\n\
"));
      oputs (_("\
  -f, --file-system      sync the file systems that contain the files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخه_OPTION_DESCRIPTION);
      emit_ancillary_info (برنامه_NAME);
    }
  خروج (وضعیت);
}

/* Sync the specified FILE, or file systems associated with FILE.
   Return 1 on success.  */

ساكن bool
sync_arg (تعداد sync_نما نما_2, حرف ثابت *file)
{
  صحيح open_flags = O_RDONLY | O_NONBLOCK;

#إذا defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  إذا (نما_2 == نما_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#پایان_اگر

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
      صحيح sync_وضعیت = -1;

      بدل (نما_2)
        {
        حالة نما_داده:
          sync_وضعیت = fdatasync (fd);
          اكسر;

        حالة نما_FILE:
          sync_وضعیت = fsync (fd);
          اكسر;

        حالة نما_FILE_SYSTEM:
#إذا HAVE_SYNCFS
          sync_وضعیت = syncfs (fd);
          اكسر;
#پایان_اگر
        حالة نما_SYNC: افتراضي:
          affirm (false);
        }

      إذا (sync_وضعیت < 0)
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
اصلی (صحيح argc, حرف **argv)
{
  bool arg_داده = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  مجموعه_برنامه_name (argv[0]);
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
          arg_داده = true;
          اكسر;

        حالة 'f':
          arg_file_system = true;
          اكسر;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخه_CHAR (برنامه_NAME, AUTHORS);

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  bool آرگومانها_specified = optind < argc;

  إذا (arg_داده && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  إذا (!آرگومانها_specified && arg_داده)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  تعداد sync_نما نما_2;
  إذا (! آرگومانها_specified || (arg_file_system && ! HAVE_SYNCFS))
    نما_2 = نما_SYNC;
  وإلا إذا (arg_file_system)
    نما_2 = نما_FILE_SYSTEM;
  وإلا إذا (! arg_داده)
    نما_2 = نما_FILE;
  وإلا
    نما_2 = نما_داده;

  إذا (نما_2 == نما_SYNC)
    sync ();
  وإلا
    {
      لكل (; optind < argc; optind++)
        ok &= sync_arg (نما_2, argv[optind]);
    }

  أرجع ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
