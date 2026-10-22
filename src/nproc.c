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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "nproc.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "nproc"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Giuseppe Scrivano")

ಎಣಿಕೆ
{
  ALL_OPTION = CHAR_MAX + 1,
  IGNORE_OPTION
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ longopts[] =
{
  {"all", no_ವಾದ, ಖಾಲಿ, ALL_OPTION},
  {"ignore", required_ವಾದ, ಖಾಲಿ, IGNORE_OPTION},
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
      ಬರೆ (_("Usage: %s [OPTION]...\n"), ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Print the ಸಂಖ್ಯೆ ದ processing units ಲಭ್ಯ ಗಮ್ಯ the current process,\n\
which may be less than the ಸಂಖ್ಯೆ ದ online processors.\n\
If the 'OMP_NUM_THREADS' or 'OMP_THREAD_LIMIT' environment variables are ಗಣ_2,\n\
then they will determine the minimum and ಗರಿಷ್ಠ returned ಮೌಲ್ಯ respectively.\n\
\n\
"), stdout);
      oputs (_("\
      --all\n\
         print the ಸಂಖ್ಯೆ ದ installed processors,\n\
         disregarding any OpenMP environment variables, or CPU quotas.\n\
"));
      oputs (_("\
      --ignore=N\n\
         ಆದರೆ possible, exclude N processing units.\n\
         The ಫಲಿತಾಂಶ is guaranteed ಗಮ್ಯ be at ಕನಿಷ್ಠ 1.\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ignore = 0;
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಎಣಿಕೆ nproc_query ಬಹುಲಕ_2 = NPROC_CURRENT_OVERRIDABLE;

  ಆಗ (true)
    {
      ಪೂರ್ಣಾಂಕ c = getopt_long (argc, argv, "", longopts, ಖಾಲಿ);
      ಆದರೆ (c == -1)
        ನಿಲ್ಲಿಸು;
      ಆಯ್ಕೆ (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಸ್ಥಿತಿ ALL_OPTION:
          ಬಹುಲಕ_2 = NPROC_ALL;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, ULONG_MAX, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          ನಿಲ್ಲಿಸು;

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  ಆದರೆ (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ nproc = ಅಂಕೆ_processors (ಬಹುಲಕ_2);

  ಆದರೆ (ignore < nproc)
    nproc -= ignore;
  ಇಲ್ಲದಿದ್ದರೆ
    nproc = 1;

  ಬರೆ ("%lu\n", nproc);

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
