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
#define PROGRAM_NAME "sync"

#define AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#ifndef HAVE_SYNCFS
# define HAVE_SYNCFS 0
#endif

गणना sync_mode
{
  MODE_FILE,
  MODE_DATA,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

स्थिर संरचना option स्थिरांक long_options[] =
{
  {"data", no_argument, रिक्त, 'd'},
  {"file-system", no_argument, रिक्त, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {रिक्त, 0, रिक्त, 0}
};

शून्य
usage (पूर्णांक status)
{
  जर (status != निकास_सफल)
    emit_try_help ();
  अन्यथा
    {
      म_लिखो (_("Usage: %s [OPTION] [FILE]...\n"), program_name);
      ख_माला_दो (_("\
Synchronize cached writes to persistent storage\n\
\n\
If one or more files are specified, sync only them,\n\
or their containing file systems.\n\
\n\
"), मानक_निकास);

      oputs (_("\
  -d, --data             sync only file data, no unneeded metadata\n\
"));
      oputs (_("\
  -f, --file-प्रणाली      sync the file systems that contain the files\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

/* Sync the specified FILE, or file systems associated with FILE.
   Return 1 on success.  */

स्थिर bool
sync_arg (गणना sync_mode mode, अक्षर स्थिरांक *file)
{
  पूर्णांक open_flags = O_RDONLY | O_NONBLOCK;

#जर defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  जर (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#endif

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  पूर्णांक fd = खोलो (file, open_flags);
  जर (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      पूर्णांक rd_errno = त्रुटि_सं;
      जर (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = खोलो (file, O_WRONLY | O_NONBLOCK);
      जर (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          परत false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  पूर्णांक fdflags = fcntl (fd, F_GETFL);
  जर (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, त्रुटि_सं, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  जर (ret)
    {
      पूर्णांक sync_status = -1;

      निवड (mode)
        {
        स्थिती MODE_DATA:
          sync_status = fdatasync (fd);
          थांबा;

        स्थिती MODE_FILE:
          sync_status = fsync (fd);
          थांबा;

        स्थिती MODE_FILE_SYSTEM:
#जर HAVE_SYNCFS
          sync_status = syncfs (fd);
          थांबा;
#endif
        स्थिती MODE_SYNC: पूर्वनिर्धारित:
          affirm (false);
        }

      जर (sync_status < 0)
        {
          error (0, त्रुटि_सं, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  जर (बंद (fd) < 0)
    {
      error (0, त्रुटि_सं, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  परत ret;
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  bool arg_data = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पूर्णांक c;
  पर्यंत ((c = getopt_long (argc, argv, "df", long_options, रिक्त))
         != -1)
    {
      निवड (c)
        {
        स्थिती 'd':
          arg_data = true;
          थांबा;

        स्थिती 'f':
          arg_file_system = true;
          थांबा;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  bool args_specified = optind < argc;

  जर (arg_data && arg_file_system)
    error (निकास_त्रुटि, 0,
           _("cannot specify both --data and --file-system"));

  जर (!args_specified && arg_data)
    error (निकास_त्रुटि, 0, _("--data needs at least one argument"));

  गणना sync_mode mode;
  जर (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  अन्यथा जर (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  अन्यथा जर (! arg_data)
    mode = MODE_FILE;
  अन्यथा
    mode = MODE_DATA;

  जर (mode == MODE_SYNC)
    sync ();
  अन्यथा
    {
      साठी (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  परत ok ? निकास_सफल : निकास_त्रुटि;
}
