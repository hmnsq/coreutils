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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>

#அந்தர்பாவ "system.h"
#அந்தர்பாவ "nproc.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nproc"

#define AUTHORS proper_name ("Giuseppe Scrivano")

எண்ணிக்கை
{
  ALL_OPTION = CHAR_MAX + 1,
  IGNORE_OPTION
};

நிலையான கட்டமைப்பு option மாறிலி longopts[] =
{
  {"all", no_argument, வெற்று, ALL_OPTION},
  {"ignore", required_argument, வெற்று, IGNORE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0}
};

வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);
      fputs (_("\
Print the எண் of processing units available to the current process,\n\
which may be less than the எண் of online processors.\n\
If the 'OMP_NUM_THREADS' or 'OMP_THREAD_LIMIT' environment variables are set,\n\
then they will determine the minimum and maximum returned மதிப்பு respectively.\n\
\n\
"), stdout);
      oputs (_("\
      --all\n\
         print the எண் of installed processors,\n\
         disregarding any OpenMP environment variables, or CPU quotas.\n\
"));
      oputs (_("\
      --ignore=N\n\
         எனில் possible, exclude N processing units.\n\
         The result is guaranteed to be at least 1.\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  unsigned long ignore = 0;
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  எண்ணிக்கை nproc_query mode = NPROC_CURRENT_OVERRIDABLE;

  போது (true)
    {
      முழு c = getopt_long (argc, argv, "", longopts, வெற்று);
      எனில் (c == -1)
        நிறுத்து;
      தேர்வு (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        நிலை ALL_OPTION:
          mode = NPROC_ALL;
          நிறுத்து;

        நிலை IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, ULONG_MAX, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          நிறுத்து;

        இயல்பு:
          usage (EXIT_FAILURE);
        }
    }

  எனில் (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  unsigned long nproc = இலக்கம்_processors (mode);

  எனில் (ignore < nproc)
    nproc -= ignore;
  இல்லையெனில்
    nproc = 1;

  printf ("%lu\n", nproc);

  திரும்பு EXIT_SUCCESS;
}
