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
#परिभाषय क्रमादेशः_NAME "sync"

#परिभाषय AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#यदि_अपरिभाषितम् HAVE_SYNCFS
# define HAVE_SYNCFS 0
#यदि_अन्तः

गणना sync_बहुलकम्
{
  बहुलकम्_FILE,
  बहुलकम्_दत्त,
  बहुलकम्_FILE_SYSTEM,
  बहुलकम्_SYNC
};

स्थिर संरचना option ध्रुव long_options[] =
{
  {"data", no_तर्कः, रिक्त, 'd'},
  {"file-system", no_तर्कः, रिक्त, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_संस्करणम्_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णाङ्क स्थितिः)
{
  यदि (स्थितिः != EXIT_SUCCESS)
    emit_try_help ();
  अन्यथा
    {
      प्रारूप_लिख (_("Usage: %s [OPTION] [FILE]...\n"), क्रमादेशः_name);
      fputs (_("\
Synchronize cached writes गन्तव्यम् persistent storage\n\
\n\
If one or अधिकम् files are specified, sync only them,\n\
or their containing file systems.\n\
\n\
"), stdout);

      oputs (_("\
  -d, --दत्त             sync only file दत्त, no unneeded metadata\n\
"));
      oputs (_("\
  -f, --file-system      sync the file systems that contain the files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

/* Sync the specified FILE, or file systems associated with FILE.
   Return 1 on success.  */

स्थिर bool
sync_arg (गणना sync_बहुलकम् बहुलकम्_2, वर्ण ध्रुव *file)
{
  पूर्णाङ्क open_flags = O_RDONLY | O_NONBLOCK;

#यदि defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  यदि (बहुलकम्_2 == बहुलकम्_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#यदि_अन्तः

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  पूर्णाङ्क fd = उद्घाटय (file, open_flags);
  यदि (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      पूर्णाङ्क rd_errno = errno;
      यदि (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = उद्घाटय (file, O_WRONLY | O_NONBLOCK);
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
      पूर्णाङ्क sync_स्थितिः = -1;

      विकल्प (बहुलकम्_2)
        {
        पक्ष बहुलकम्_दत्त:
          sync_स्थितिः = fdatasync (fd);
          भङ्ग;

        पक्ष बहुलकम्_FILE:
          sync_स्थितिः = fsync (fd);
          भङ्ग;

        पक्ष बहुलकम्_FILE_SYSTEM:
#यदि HAVE_SYNCFS
          sync_स्थितिः = syncfs (fd);
          भङ्ग;
#यदि_अन्तः
        पक्ष बहुलकम्_SYNC: स्वभाव:
          affirm (false);
        }

      यदि (sync_स्थितिः < 0)
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
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  bool arg_दत्त = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
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
          arg_दत्त = true;
          भङ्ग;

        पक्ष 'f':
          arg_file_system = true;
          भङ्ग;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  bool तर्काः_specified = optind < argc;

  यदि (arg_दत्त && arg_file_system)
    error (EXIT_FAILURE, 0,
           _("cannot specify both --data and --file-system"));

  यदि (!तर्काः_specified && arg_दत्त)
    error (EXIT_FAILURE, 0, _("--data needs at least one argument"));

  गणना sync_बहुलकम् बहुलकम्_2;
  यदि (! तर्काः_specified || (arg_file_system && ! HAVE_SYNCFS))
    बहुलकम्_2 = बहुलकम्_SYNC;
  अन्यथा यदि (arg_file_system)
    बहुलकम्_2 = बहुलकम्_FILE_SYSTEM;
  अन्यथा यदि (! arg_दत्त)
    बहुलकम्_2 = बहुलकम्_FILE;
  अन्यथा
    बहुलकम्_2 = बहुलकम्_दत्त;

  यदि (बहुलकम्_2 == बहुलकम्_SYNC)
    sync ();
  अन्यथा
    {
      प्रति (; optind < argc; optind++)
        ok &= sync_arg (बहुलकम्_2, argv[optind]);
    }

  प्रत्यागम ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
