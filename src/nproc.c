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
#परिभाषय क्रमादेशः_NAME "nproc"

#परिभाषय AUTHORS proper_name ("Giuseppe Scrivano")

गणना
{
  ALL_OPTION = CHAR_MAX + 1,
  IGNORE_OPTION
};

स्थिर संरचना option ध्रुव longopts[] =
{
  {"all", no_तर्कः, रिक्त, ALL_OPTION},
  {"ignore", required_तर्कः, रिक्त, IGNORE_OPTION},
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
      प्रारूप_लिख (_("Usage: %s [OPTION]...\n"), क्रमादेशः_name);
      fputs (_("\
Print the सङ्ख्या सम्बन्धि processing units उपलब्धम् गन्तव्यम् the current process,\n\
which may be less than the सङ्ख्या सम्बन्धि online processors.\n\
If the 'OMP_NUM_THREADS' or 'OMP_THREAD_LIMIT' environment variables are समुच्चयः_2,\n\
then they will determine the minimum and अधिकतमम् returned मानम् respectively.\n\
\n\
"), stdout);
      oputs (_("\
      --all\n\
         print the सङ्ख्या सम्बन्धि installed processors,\n\
         disregarding any OpenMP environment variables, or CPU quotas.\n\
"));
      oputs (_("\
      --ignore=N\n\
         यदि possible, exclude N processing units.\n\
         The परिणाम is guaranteed गन्तव्यम् be at न्यूनतमम् 1.\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (संस्करणम्_OPTION_DESCRIPTION);
      emit_ancillary_info (क्रमादेशः_NAME);
    }
  निर्गच्छ (स्थितिः);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  अचिह्नित दीर्घ ignore = 0;
  initialize_main (&argc, &argv);
  समुच्चयः_क्रमादेशः_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  गणना nproc_query बहुलकम्_2 = NPROC_CURRENT_OVERRIDABLE;

  यावत् (true)
    {
      पूर्णाङ्क c = getopt_long (argc, argv, "", longopts, रिक्त);
      यदि (c == -1)
        भङ्ग;
      विकल्प (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_संस्करणम्_CHAR (क्रमादेशः_NAME, AUTHORS);

        पक्ष ALL_OPTION:
          बहुलकम्_2 = NPROC_ALL;
          भङ्ग;

        पक्ष IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, ULONG_MAX, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          भङ्ग;

        स्वभाव:
          usage (EXIT_FAILURE);
        }
    }

  यदि (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  अचिह्नित दीर्घ nproc = अङ्कः_processors (बहुलकम्_2);

  यदि (ignore < nproc)
    nproc -= ignore;
  अन्यथा
    nproc = 1;

  प्रारूप_लिख ("%lu\n", nproc);

  प्रत्यागम EXIT_SUCCESS;
}
