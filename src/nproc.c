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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <getopt.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>

#അന്തര്ഭാവ "system.h"
#അന്തര്ഭാവ "nproc.h"
#അന്തര്ഭാവ "quote.h"
#അന്തര്ഭാവ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nproc"

#define AUTHORS proper_name ("Giuseppe Scrivano")

എണ്ണൽ
{
  ALL_OPTION = CHAR_MAX + 1,
  IGNORE_OPTION
};

സ്ഥിതം ഘടന option സ്ഥിരം longopts[] =
{
  {"all", no_argument, ശൂന്യ, ALL_OPTION},
  {"ignore", required_argument, ശൂന്യ, IGNORE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ശൂന്യ, 0, ശൂന്യ, 0}
};

ശൂന്യം
usage (പൂർണ്ണസംഖ്യ status)
{
  എങ്കിൽ (status != EXIT_SUCCESS)
    emit_try_help ();
  അല്ലെങ്കിൽ
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);
      fputs (_("\
Print the സംഖ്യ of processing units available to the current process,\n\
which may be less than the സംഖ്യ of online processors.\n\
If the 'OMP_NUM_THREADS' or 'OMP_THREAD_LIMIT' environment variables are set,\n\
then they will determine the minimum and maximum returned മൂല്യം respectively.\n\
\n\
"), stdout);
      oputs (_("\
      --all\n\
         print the സംഖ്യ of installed processors,\n\
         disregarding any OpenMP environment variables, or CPU quotas.\n\
"));
      oputs (_("\
      --ignore=N\n\
         എങ്കിൽ possible, exclude N processing units.\n\
         The result is guaranteed to be at least 1.\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  unsigned long ignore = 0;
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  എണ്ണൽ nproc_query mode = NPROC_CURRENT_OVERRIDABLE;

  ആകുമ്പോൾ (true)
    {
      പൂർണ്ണസംഖ്യ c = getopt_long (argc, argv, "", longopts, ശൂന്യ);
      എങ്കിൽ (c == -1)
        നിർത്തുക;
      തിരഞ്ഞെടുപ്പ് (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        അവസ്ഥ ALL_OPTION:
          mode = NPROC_ALL;
          നിർത്തുക;

        അവസ്ഥ IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, ULONG_MAX, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
          usage (EXIT_FAILURE);
        }
    }

  എങ്കിൽ (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  unsigned long nproc = അക്കം_processors (mode);

  എങ്കിൽ (ignore < nproc)
    nproc -= ignore;
  അല്ലെങ്കിൽ
    nproc = 1;

  printf ("%lu\n", nproc);

  തിരികെ EXIT_SUCCESS;
}
