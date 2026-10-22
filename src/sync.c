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
#تعریف_کرو پروگرام_NAME "sync"

#تعریف_کرو AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#اگر_غیر_تعریف_شدہ HAVE_SYNCFS
# define HAVE_SYNCFS 0
#اگر_ختم

شمار sync_منوال
{
  منوال_FILE,
  منوال_ڈیٹا,
  منوال_FILE_SYSTEM,
  منوال_SYNC
};

ساکن ساخت option ثابت long_options[] =
{
  {"data", no_دلیل, تہی, 'd'},
  {"file-system", no_دلیل, تہی, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION] [FILE]...\n"), پروگرام_name);
      fputs (_("\
Synchronize cached writes مقصد persistent storage\n\
\n\
If one or مزید files are specified, sync only them,\n\
or their containing file systems.\n\
\n\
"), stdout);

      oputs (_("\
  -d, --ڈیٹا             sync only file ڈیٹا, no unneeded metadata\n\
"));
      oputs (_("\
  -f, --file-system      sync the file systems that contain the files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Sync the specified FILE, or file systems associated with FILE.
   Return 1 on success.  */

ساکن bool
sync_arg (شمار sync_منوال منوال_2, حرف ثابت *file)
{
  صحیح open_flags = O_RDONLY | O_NONBLOCK;

#اگر defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  اگر (منوال_2 == منوال_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#اگر_ختم

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
          واپس false;
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
      صحیح sync_حالت = -1;

      انتخاب (منوال_2)
        {
        حالت منوال_ڈیٹا:
          sync_حالت = fdatasync (fd);
          توڑو;

        حالت منوال_FILE:
          sync_حالت = fsync (fd);
          توڑو;

        حالت منوال_FILE_SYSTEM:
#اگر HAVE_SYNCFS
          sync_حالت = syncfs (fd);
          توڑو;
#اگر_ختم
        حالت منوال_SYNC: طےشدہ:
          affirm (false);
        }

      اگر (sync_حالت < 0)
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

  واپس ret;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  bool arg_ڈیٹا = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحیح c;
  جب ((c = getopt_long (argc, argv, "df", long_options, تہی))
         != -1)
    {
      انتخاب (c)
        {
        حالت 'd':
          arg_ڈیٹا = true;
          توڑو;

        حالت 'f':
          arg_file_system = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  bool دلائل_specified = optind < argc;

  اگر (arg_ڈیٹا && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  اگر (!دلائل_specified && arg_ڈیٹا)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  شمار sync_منوال منوال_2;
  اگر (! دلائل_specified || (arg_file_system && ! HAVE_SYNCFS))
    منوال_2 = منوال_SYNC;
  ورنہ اگر (arg_file_system)
    منوال_2 = منوال_FILE_SYSTEM;
  ورنہ اگر (! arg_ڈیٹا)
    منوال_2 = منوال_FILE;
  ورنہ
    منوال_2 = منوال_ڈیٹا;

  اگر (منوال_2 == منوال_SYNC)
    sync ();
  ورنہ
    {
      برائے (; optind < argc; optind++)
        ok &= sync_arg (منوال_2, argv[optind]);
    }

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
