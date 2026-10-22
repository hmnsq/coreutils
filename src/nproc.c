/* nproc - print the number of processors.
   Copyright (C) 2009-2026 Free Software Foundation, Inc.

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

/* Written by Giuseppe Scrivano.  */

#अन्तर्भाव <config.h>
#अन्तर्भाव <getopt.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>

#अन्तर्भाव "system.h"
#अन्तर्भाव "nproc.h"
#अन्तर्भाव "quote.h"
#अन्तर्भाव "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nproc"

#define AUTHORS proper_name ("Giuseppe Scrivano")

गणना
{
  ALL_OPTION = अक्षर_उच्च + 1,
  IGNORE_OPTION
};

स्थिर संरचना option स्थिरांक longopts[] =
{
  {"all", no_argument, रिक्त, ALL_OPTION},
  {"ignore", required_argument, रिक्त, IGNORE_OPTION},
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
      म_लिखो (_("Usage: %s [OPTION]...\n"), program_name);
      ख_माला_दो (_("\
Print the संख्या of processing units available to the current process,\n\
which may be less than the संख्या of online processors.\n\
If the 'OMP_NUM_THREADS' or 'OMP_THREAD_LIMIT' environment variables are set,\n\
then they will determine the minimum and maximum returned मूल्य respectively.\n\
\n\
"), मानक_निकास);
      oputs (_("\
      --all\n\
         print the संख्या of installed processors,\n\
         disregarding any OpenMP environment variables, or CPU quotas.\n\
"));
      oputs (_("\
      --ignore=N\n\
         जर possible, exclude N processing units.\n\
         The result is guaranteed to be at least 1.\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  अचिन्हित दीर्घ ignore = 0;
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  गणना nproc_query mode = NPROC_CURRENT_OVERRIDABLE;

  पर्यंत (true)
    {
      पूर्णांक c = getopt_long (argc, argv, "", longopts, रिक्त);
      जर (c == -1)
        थांबा;
      निवड (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        स्थिती ALL_OPTION:
          mode = NPROC_ALL;
          थांबा;

        स्थिती IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, अच_दीर्घ_उच्च, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          थांबा;

        पूर्वनिर्धारित:
          usage (निकास_त्रुटि);
        }
    }

  जर (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (निकास_त्रुटि);
    }

  अचिन्हित दीर्घ nproc = अंक_processors (mode);

  जर (ignore < nproc)
    nproc -= ignore;
  अन्यथा
    nproc = 1;

  म_लिखो ("%lu\n", nproc);

  परत निकास_सफल;
}
