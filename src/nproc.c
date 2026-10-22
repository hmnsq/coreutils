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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>

#অন্তর্ভাব "system.h"
#অন্তর্ভাব "nproc.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nproc"

#define AUTHORS proper_name ("Giuseppe Scrivano")

গণনা
{
  ALL_OPTION = CHAR_MAX + 1,
  IGNORE_OPTION
};

স্থির সংরচনা option ধ্রুব longopts[] =
{
  {"all", no_argument, রিক্ত, ALL_OPTION},
  {"ignore", required_argument, রিক্ত, IGNORE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {রিক্ত, 0, রিক্ত, 0}
};

শূন্য
usage (পূর্ণাঙ্ক status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);
      fputs (_("\
Print the সংখ্যা of processing units available to the current process,\n\
which may be less than the সংখ্যা of online processors.\n\
If the 'OMP_NUM_THREADS' or 'OMP_THREAD_LIMIT' environment variables are set,\n\
then they will determine the minimum and maximum returned মান respectively.\n\
\n\
"), stdout);
      oputs (_("\
      --all\n\
         print the সংখ্যা of installed processors,\n\
         disregarding any OpenMP environment variables, or CPU quotas.\n\
"));
      oputs (_("\
      --ignore=N\n\
         যদি possible, exclude N processing units.\n\
         The result is guaranteed to be at least 1.\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

পূর্ণাঙ্ক
main (পূর্ণাঙ্ক argc, বর্ণ **argv)
{
  unsigned long ignore = 0;
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  গণনা nproc_query mode = NPROC_CURRENT_OVERRIDABLE;

  যাবত্ (true)
    {
      পূর্ণাঙ্ক c = getopt_long (argc, argv, "", longopts, রিক্ত);
      যদি (c == -1)
        ভঙ্গ;
      বিকল্প (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        পক্ষ ALL_OPTION:
          mode = NPROC_ALL;
          ভঙ্গ;

        পক্ষ IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, ULONG_MAX, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          ভঙ্গ;

        স্বভাব:
          usage (EXIT_FAILURE);
        }
    }

  যদি (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  unsigned long nproc = অঙ্ক_processors (mode);

  যদি (ignore < nproc)
    nproc -= ignore;
  অন্যথা
    nproc = 1;

  printf ("%lu\n", nproc);

  প্রত্যাগম EXIT_SUCCESS;
}
