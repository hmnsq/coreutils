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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "sync"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_SYNCFS
# define HAVE_SYNCFS 0
#ਜੇ_ਅੰਤ

ਗਿਣਤੀ sync_ਬਹੁਲਕ
{
  ਬਹੁਲਕ_FILE,
  ਬਹੁਲਕ_ਅੰਕੜੇ,
  ਬਹੁਲਕ_FILE_SYSTEM,
  ਬਹੁਲਕ_SYNC
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_options[] =
{
  {"data", no_ਦਲੀਲ, ਨਿਰਾ, 'd'},
  {"file-system", no_ਦਲੀਲ, ਨਿਰਾ, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION] [FILE]...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Synchronize cached writes ਮੰਜ਼ਿਲ persistent storage\n\
\n\
If one or ਹੋਰ files are specified, sync only them,\n\
or their containing file systems.\n\
\n\
"), stdout);

      oputs (_("\
  -d, --ਅੰਕੜੇ             sync only file ਅੰਕੜੇ, no unneeded metadata\n\
"));
      oputs (_("\
  -f, --file-system      sync the file systems that contain the files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* Sync the specified FILE, or file systems associated with FILE.
   Return 1 on success.  */

ਸਥਿਰ bool
sync_arg (ਗਿਣਤੀ sync_ਬਹੁਲਕ ਬਹੁਲਕ_2, ਅੱਖਰ ਅਚਲ *file)
{
  ਪੂਰਨ open_flags = O_RDONLY | O_NONBLOCK;

#ਜੇ defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  ਜੇ (ਬਹੁਲਕ_2 == ਬਹੁਲਕ_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#ਜੇ_ਅੰਤ

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  ਪੂਰਨ fd = open (file, open_flags);
  ਜੇ (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      ਪੂਰਨ rd_errno = errno;
      ਜੇ (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      ਜੇ (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          ਵਾਪਸ false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  ਪੂਰਨ fdflags = fcntl (fd, F_GETFL);
  ਜੇ (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  ਜੇ (ret)
    {
      ਪੂਰਨ sync_ਸਥਿਤੀ = -1;

      ਚੋਣ (ਬਹੁਲਕ_2)
        {
        ਹਾਲਤ ਬਹੁਲਕ_ਅੰਕੜੇ:
          sync_ਸਥਿਤੀ = fdatasync (fd);
          ਰੋਕੋ;

        ਹਾਲਤ ਬਹੁਲਕ_FILE:
          sync_ਸਥਿਤੀ = fsync (fd);
          ਰੋਕੋ;

        ਹਾਲਤ ਬਹੁਲਕ_FILE_SYSTEM:
#ਜੇ HAVE_SYNCFS
          sync_ਸਥਿਤੀ = syncfs (fd);
          ਰੋਕੋ;
#ਜੇ_ਅੰਤ
        ਹਾਲਤ ਬਹੁਲਕ_SYNC: ਮੂਲ:
          affirm (false);
        }

      ਜੇ (sync_ਸਥਿਤੀ < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  ਜੇ (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  ਵਾਪਸ ret;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool arg_ਅੰਕੜੇ = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਪੂਰਨ c;
  ਜਦੋਂ ((c = getopt_long (argc, argv, "df", long_options, ਨਿਰਾ))
         != -1)
    {
      ਚੋਣ (c)
        {
        ਹਾਲਤ 'd':
          arg_ਅੰਕੜੇ = true;
          ਰੋਕੋ;

        ਹਾਲਤ 'f':
          arg_file_system = true;
          ਰੋਕੋ;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  bool ਦਲੀਲਾਂ_specified = optind < argc;

  ਜੇ (arg_ਅੰਕੜੇ && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  ਜੇ (!ਦਲੀਲਾਂ_specified && arg_ਅੰਕੜੇ)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  ਗਿਣਤੀ sync_ਬਹੁਲਕ ਬਹੁਲਕ_2;
  ਜੇ (! ਦਲੀਲਾਂ_specified || (arg_file_system && ! HAVE_SYNCFS))
    ਬਹੁਲਕ_2 = ਬਹੁਲਕ_SYNC;
  ਨਹੀਂਤਾਂ ਜੇ (arg_file_system)
    ਬਹੁਲਕ_2 = ਬਹੁਲਕ_FILE_SYSTEM;
  ਨਹੀਂਤਾਂ ਜੇ (! arg_ਅੰਕੜੇ)
    ਬਹੁਲਕ_2 = ਬਹੁਲਕ_FILE;
  ਨਹੀਂਤਾਂ
    ਬਹੁਲਕ_2 = ਬਹੁਲਕ_ਅੰਕੜੇ;

  ਜੇ (ਬਹੁਲਕ_2 == ਬਹੁਲਕ_SYNC)
    sync ();
  ਨਹੀਂਤਾਂ
    {
      ਲਈ (; optind < argc; optind++)
        ok &= sync_arg (ਬਹੁਲਕ_2, argv[optind]);
    }

  ਵਾਪਸ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
