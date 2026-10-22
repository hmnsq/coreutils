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

#समावेश <config.h>
#समावेश <getopt.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>

#समावेश "system.h"
#समावेश "nproc.h"
#समावेश "quote.h"
#समावेश "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#घोषणा PROGRAM_NAME "nproc"

#घोषणा AUTHORS proper_name ("Giuseppe Scrivano")

क्रमागत
{
  ALL_OPTION = अक्षर_उच्च + 1,
  IGNORE_OPTION
};

अटल काष्ठा option स्थिर longopts[] =
{
  {"all", no_argument, शून्य, ALL_OPTION},
  {"ignore", required_argument, शून्य, IGNORE_OPTION},
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
         अगर possible, exclude N processing units.\n\
         The परिणाम is guaranteed to be at least 1.\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  निकास (status);
}

पूर्णांक
मुख्य (पूर्णांक argc, अक्षर **argv)
{
  अचिन्हित दीर्घ ignore = 0;
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  रखो_क्षेत्र (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  निकास_पर (close_stdout);

  क्रमागत nproc_query mode = NPROC_CURRENT_OVERRIDABLE;

  जबतक (true)
    {
      पूर्णांक c = getopt_long (argc, argv, "", longopts, शून्य);
      अगर (c == -1)
        अवरोध;
      चयन (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        हाल ALL_OPTION:
          mode = NPROC_ALL;
          अवरोध;

        हाल IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, अच_दीर्घ_उच्च, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          अवरोध;

        शेष:
          usage (निकास_त्रुटि);
        }
    }

  अगर (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (निकास_त्रुटि);
    }

  अचिन्हित दीर्घ nproc = अंक_processors (mode);

  अगर (ignore < nproc)
    nproc -= ignore;
  अन्यथा
    nproc = 1;

  म_लिखो ("%lu\n", nproc);

  वापस निकास_सफल;
}
