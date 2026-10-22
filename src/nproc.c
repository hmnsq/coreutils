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

#شامل <config.h>
#شامل <getopt.h>
#شامل <stdio.h>
#شامل <sys/types.h>

#شامل "system.h"
#شامل "nproc.h"
#شامل "quote.h"
#شامل "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "nproc"

#define AUTHORS proper_name ("Giuseppe Scrivano")

شمارش
{
  ALL_OPTION = CHAR_MAX + 1,
  IGNORE_OPTION
};

ایستا ساختار option ثابت longopts[] =
{
  {"all", no_argument, پوچ, ALL_OPTION},
  {"ignore", required_argument, پوچ, IGNORE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {پوچ, 0, پوچ, 0}
};

تهی
usage (صحیح status)
{
  اگر (status != EXIT_SUCCESS)
    emit_try_help ();
  وگرنه
    {
      printf (_("Usage: %s [OPTION]...\n"), program_name);
      fputs (_("\
Print the عدد of processing units available to the current process,\n\
which may be less than the عدد of online processors.\n\
If the 'OMP_NUM_THREADS' or 'OMP_THREAD_LIMIT' environment variables are set,\n\
then they will determine the minimum and maximum returned مقدار respectively.\n\
\n\
"), stdout);
      oputs (_("\
      --all\n\
         print the عدد of installed processors,\n\
         disregarding any OpenMP environment variables, or CPU quotas.\n\
"));
      oputs (_("\
      --ignore=N\n\
         اگر possible, exclude N processing units.\n\
         The result is guaranteed to be at least 1.\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }
  exit (status);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  unsigned long ignore = 0;
  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  شمارش nproc_query mode = NPROC_CURRENT_OVERRIDABLE;

  وقتی (true)
    {
      صحیح c = getopt_long (argc, argv, "", longopts, پوچ);
      اگر (c == -1)
        بشکن;
      انتخاب (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);

        حالت ALL_OPTION:
          mode = NPROC_ALL;
          بشکن;

        حالت IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, ULONG_MAX, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          بشکن;

        پیشفرض:
          usage (EXIT_FAILURE);
        }
    }

  اگر (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  unsigned long nproc = رقم_processors (mode);

  اگر (ignore < nproc)
    nproc -= ignore;
  وگرنه
    nproc = 1;

  printf ("%lu\n", nproc);

  بازگشت EXIT_SUCCESS;
}
