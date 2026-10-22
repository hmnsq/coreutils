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
#تعریف_کرو پروگرام_NAME "nproc"

#تعریف_کرو AUTHORS proper_name ("Giuseppe Scrivano")

شمار
{
  ALL_OPTION = CHAR_MAX + 1,
  IGNORE_OPTION
};

ساکن ساخت option ثابت longopts[] =
{
  {"all", no_دلیل, تہی, ALL_OPTION},
  {"ignore", required_دلیل, تہی, IGNORE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("Usage: %s [OPTION]...\n"), پروگرام_name);
      fputs (_("\
Print the نمبر کا processing units دستیاب مقصد the current process,\n\
which may be less than the نمبر کا online processors.\n\
If the 'OMP_NUM_THREADS' or 'OMP_THREAD_LIMIT' environment variables are سیٹ_2,\n\
then they will determine the minimum and زیادہ_سے_زیادہ returned قدر respectively.\n\
\n\
"), stdout);
      oputs (_("\
      --all\n\
         print the نمبر کا installed processors,\n\
         disregarding any OpenMP environment variables, or CPU quotas.\n\
"));
      oputs (_("\
      --ignore=N\n\
         اگر possible, exclude N processing units.\n\
         The نتیجہ is guaranteed مقصد be at کم_سے_کم 1.\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  غیرعلامتی طویل ignore = 0;
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  شمار nproc_query منوال_2 = NPROC_CURRENT_OVERRIDABLE;

  جب (true)
    {
      صحیح c = getopt_long (argc, argv, "", longopts, تہی);
      اگر (c == -1)
        توڑو;
      انتخاب (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        حالت ALL_OPTION:
          منوال_2 = NPROC_ALL;
          توڑو;

        حالت IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, ULONG_MAX, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          توڑو;

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  اگر (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  غیرعلامتی طویل nproc = عدد_processors (منوال_2);

  اگر (ignore < nproc)
    nproc -= ignore;
  ورنہ
    nproc = 1;

  لکھو ("%lu\n", nproc);

  واپس EXIT_SUCCESS;
}
