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

#समावेश <config.h>
#समावेश <getopt.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "assure.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "sync"

#घोषणा AUTHORS                                 \
  proper_name ("Jim Meyering"),                 \
  proper_name ("Giuseppe Scrivano")

#अगर_अघोषित HAVE_SYNCFS
# define HAVE_SYNCFS 0
#पूर्ण_अगर

क्रमागत sync_mode
{
  MODE_FILE,
  MODE_तथ्य,
  MODE_FILE_SYSTEM,
  MODE_SYNC
};

अटल काष्ठा option स्थिर long_options[] =
{
  {"data", no_argument, शून्य, 'd'},
  {"file-system", no_argument, शून्य, 'f'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {शून्य, 0, शून्य, 0}
};

व्योम
usage (पूर्णांक status)
{
  अगर (status != निकास_सफल)
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
  -d, --तथ्य             sync only file तथ्य, no unneeded metadata\n\
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

अटल bool
sync_arg (क्रमागत sync_mode mode, अक्षर स्थिर *file)
{
  पूर्णांक open_flags = O_RDONLY | O_NONBLOCK;

#अगर defined _AIX || defined __CYGWIN__
  /* AIX 7.1, CYGWIN 2.9.0, fsync requires write access to file.  */
  अगर (mode == MODE_FILE)
    open_flags = O_WRONLY | O_NONBLOCK;
#पूर्ण_अगर

  /* Note O_PATH might be supported with syncfs(),
     though as of Linux 3.18 is not.  */
  पूर्णांक fd = खोलो (file, open_flags);
  अगर (fd < 0)
    {
      /* Use the O_RDONLY errno, which is significant
         with directories for example.  */
      पूर्णांक rd_errno = त्रुटि_सं;
      अगर (open_flags != (O_WRONLY | O_NONBLOCK))
        fd = खोलो (file, O_WRONLY | O_NONBLOCK);
      अगर (fd < 0)
        {
          error (0, rd_errno, _("error opening %s"), quoteaf (file));
          वापस false;
        }
    }

  bool ret = true;
  /* We used O_NONBLOCK above to not hang with fifos,
     so reset that here.  */
  पूर्णांक fdflags = fcntl (fd, F_GETFL);
  अगर (fdflags == -1
      || fcntl (fd, F_SETFL, fdflags & ~O_NONBLOCK) < 0)
    {
      error (0, त्रुटि_सं, _("couldn't reset non-blocking mode %s"),
             quoteaf (file));
      ret = false;
    }

  अगर (ret)
    {
      पूर्णांक sync_status = -1;

      चयन (mode)
        {
        हाल MODE_तथ्य:
          sync_status = fdatasync (fd);
          अवरोध;

        हाल MODE_FILE:
          sync_status = fsync (fd);
          अवरोध;

        हाल MODE_FILE_SYSTEM:
#अगर HAVE_SYNCFS
          sync_status = syncfs (fd);
          अवरोध;
#पूर्ण_अगर
        हाल MODE_SYNC: शेष:
          affirm (false);
        }

      अगर (sync_status < 0)
        {
          error (0, त्रुटि_सं, _("error syncing %s"), quoteaf (file));
          ret = false;
        }
    }

  अगर (बंद (fd) < 0)
    {
      error (0, त्रुटि_सं, _("failed to close %s"), quoteaf (file));
      ret = false;
    }

  वापस ret;
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  bool arg_तथ्य = false, arg_file_system = false;
  bool ok = true;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  पूर्णांक c;
  जबतक ((c = getopt_long (argc, argv, "df", long_options, शून्य))
         != -1)
    {
      चयन (c)
        {
        हाल 'd':
          arg_तथ्य = true;
          अवरोध;

        हाल 'f':
          arg_file_system = true;
          अवरोध;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  bool args_specified = optind < argc;

  अगर (arg_तथ्य && arg_file_system)
    error (निकास_त्रुटि, 0,
           _("cannot specify both --data and --file-system"));

  अगर (!args_specified && arg_तथ्य)
    error (निकास_त्रुटि, 0, _("--data needs at least one argument"));

  क्रमागत sync_mode mode;
  अगर (! args_specified || (arg_file_system && ! HAVE_SYNCFS))
    mode = MODE_SYNC;
  अन्यथा अगर (arg_file_system)
    mode = MODE_FILE_SYSTEM;
  अन्यथा अगर (! arg_तथ्य)
    mode = MODE_FILE;
  अन्यथा
    mode = MODE_तथ्य;

  अगर (mode == MODE_SYNC)
    sync ();
  अन्यथा
    {
      क्रम (; optind < argc; optind++)
        ok &= sync_arg (mode, argv[optind]);
    }

  वापस ok ? निकास_सफल : निकास_त्रुटि;
}
