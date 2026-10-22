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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "sync"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_SYNCFS
# define HAVE_SYNCFS 0
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ sync_ಬಹುಲಕ
{
  ಬಹುಲಕ_FILE,
  ಬಹುಲಕ_ದತ್ತಾಂಶ,
  ಬಹುಲಕ_FILE_SYSTEM,
  ಬಹುಲಕ_SYNC
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"data", no_ವಾದ, ಖಾಲಿ, 'd'},
  {"file-system", no_ವಾದ, ಖಾಲಿ, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("Usage: %s [OPTION] [FILE]...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Synchronize cached writes ಗಮ್ಯ persistent storage\n\
\n\
If one or ಇನ್ನಷ್ಟು files are specified, sync only them,\n\
or their containing file systems.\n\
\n\
"), stdout);

      oputs (_("\
  -d, --ದತ್ತಾಂಶ             sync only file ದತ್ತಾಂಶ, no unneeded metadata\n\
"));
      oputs (_("\
  -f, --file-system      sync the file systems that contain the files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Sync the specified FILE, or file systems associated with FILE.
   Return 1 on success.  */

ಸ್ಥಿರ bool
sync_arg (ಎಣಿಕೆ sync_ಬಹುಲಕ ಬಹುಲಕ_2, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file)
{
  ಪೂರ್ಣಾಂಕ open_flags = O_RDONLY | O_NONBLOCK;

#ಆದರೆ defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  ಆದರೆ (ಬಹುಲಕ_2 == ಬಹುಲಕ_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#ಆದರೆ_ಅಂತ್ಯ

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  ಪೂರ್ಣಾಂಕ fd = open (file, open_flags);
  ಆದರೆ (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      ಪೂರ್ಣಾಂಕ rd_errno = errno;
      ಆದರೆ (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = open (file, O_WRONLY | O_NONBLOCK);
      ಆದರೆ (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          ಹಿಂತಿರುಗಿ false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  ಪೂರ್ಣಾಂಕ fdflags = fcntl (fd, F_GETFL);
  ಆದರೆ (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, errno, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  ಆದರೆ (ret)
    {
      ಪೂರ್ಣಾಂಕ sync_ಸ್ಥಿತಿ = -1;

      ಆಯ್ಕೆ (ಬಹುಲಕ_2)
        {
        ಸ್ಥಿತಿ ಬಹುಲಕ_ದತ್ತಾಂಶ:
          sync_ಸ್ಥಿತಿ = fdatasync (fd);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಬಹುಲಕ_FILE:
          sync_ಸ್ಥಿತಿ = fsync (fd);
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ ಬಹುಲಕ_FILE_SYSTEM:
#ಆದರೆ HAVE_SYNCFS
          sync_ಸ್ಥಿತಿ = syncfs (fd);
          ನಿಲ್ಲಿಸು;
#ಆದರೆ_ಅಂತ್ಯ
        ಸ್ಥಿತಿ ಬಹುಲಕ_SYNC: ಪೂರ್ವನಿಯೋಜಿತ:
          affirm (false);
        }

      ಆದರೆ (sync_ಸ್ಥಿತಿ < 0)
        {
          error (0, errno, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  ಆದರೆ (close (fd) < 0)
    {
      error (0, errno, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool arg_ದತ್ತಾಂಶ = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಂಕ c;
  ಆಗ ((c = getopt_long (argc, argv, "df", long_options, ಖಾಲಿ))
         != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'd':
          arg_ದತ್ತಾಂಶ = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ 'f':
          arg_file_system = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  bool ವಾದಗಳು_specified = optind < argc;

  ಆದರೆ (arg_ದತ್ತಾಂಶ && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  ಆದರೆ (!ವಾದಗಳು_specified && arg_ದತ್ತಾಂಶ)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  ಎಣಿಕೆ sync_ಬಹುಲಕ ಬಹುಲಕ_2;
  ಆದರೆ (! ವಾದಗಳು_specified || (arg_file_system && ! HAVE_SYNCFS))
    ಬಹುಲಕ_2 = ಬಹುಲಕ_SYNC;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (arg_file_system)
    ಬಹುಲಕ_2 = ಬಹುಲಕ_FILE_SYSTEM;
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (! arg_ದತ್ತಾಂಶ)
    ಬಹುಲಕ_2 = ಬಹುಲಕ_FILE;
  ಇಲ್ಲದಿದ್ದರೆ
    ಬಹುಲಕ_2 = ಬಹುಲಕ_ದತ್ತಾಂಶ;

  ಆದರೆ (ಬಹುಲಕ_2 == ಬಹುಲಕ_SYNC)
    sync ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಗಾಗಿ (; optind < argc; optind++)
        ok &= sync_arg (ಬಹುಲಕ_2, argv[optind]);
    }

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
