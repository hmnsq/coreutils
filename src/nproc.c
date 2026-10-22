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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>

#અન્તર્ભાવ "system.h"
#અન્તર્ભાવ "nproc.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nproc"

#define AUTHORS proper_name ("Giuseppe Scrivano")

ગણના
{
  ALL_OPTION = CHAR_MAX + 1,
  IGNORE_OPTION
};

સ્થિર સંરચના option અચલ longopts[] =
{
  {"all", no_argument, ખાલી, ALL_OPTION},
  {"ignore", required_argument, ખાલી, IGNORE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0}
};

શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);
      fputs (_("\
Print the સંખ્યા of processing units available to the current process,\n\
which may be less than the સંખ્યા of online processors.\n\
If the 'OMP_NUM_THREADS' or 'OMP_THREAD_LIMIT' environment variables are set,\n\
then they will determine the minimum and maximum returned કિંમત respectively.\n\
\n\
"), stdout);
      oputs (_("\
      --all\n\
         print the સંખ્યા of installed processors,\n\
         disregarding any OpenMP environment variables, or CPU quotas.\n\
"));
      oputs (_("\
      --ignore=N\n\
         જો possible, exclude N processing units.\n\
         The result is guaranteed to be at least 1.\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  unsigned long ignore = 0;
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ગણના nproc_query mode = NPROC_CURRENT_OVERRIDABLE;

  જ્યારે (true)
    {
      પૂર્ણાંક c = getopt_long (argc, argv, "", longopts, ખાલી);
      જો (c == -1)
        તોડો;
      પસંદગી (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        સ્થિતિ ALL_OPTION:
          mode = NPROC_ALL;
          તોડો;

        સ્થિતિ IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, ULONG_MAX, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          તોડો;

        મૂળભૂત:
          usage (EXIT_FAILURE);
        }
    }

  જો (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  unsigned long nproc = અંક_processors (mode);

  જો (ignore < nproc)
    nproc -= ignore;
  નહીંતર
    nproc = 1;

  printf ("%lu\n", nproc);

  પાછા EXIT_SUCCESS;
}
