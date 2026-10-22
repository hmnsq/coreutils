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

#ضمن <config.h>
#ضمن <getopt.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>

#ضمن "system.h"
#ضمن "nproc.h"
#ضمن "quote.h"
#ضمن "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "nproc"

#تعریف_کرو AUTHORS proper_name ("Giuseppe Scrivano")

تعداد
{
  ALL_OPTION = CHAR_MAX + 1,
  IGNORE_OPTION
};

ساكن بنية option ثابت longopts[] =
{
  {"all", no_دلیل, عدم, ALL_OPTION},
  {"ignore", required_دلیل, عدم, IGNORE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {عدم, 0, عدم, 0}
};

فراغ
usage (صحيح حالت)
{
  إذا (حالت != EXIT_SUCCESS)
    emit_try_help ();
  وإلا
    {
      لکھو (_("Usage: %s [OPTION]...\n"), پروگرام_name);
      fputs (_("\
Print the نمبر دا processing units دستیاب منزل the current process,\n\
which may be less than the نمبر دا online processors.\n\
If the 'OMP_NUM_THREADS' or 'OMP_THREAD_LIMIT' environment variables are سیٹ_2,\n\
then they will determine the minimum and ودھ_توں_ودھ returned قدر respectively.\n\
\n\
"), stdout);
      oputs (_("\
      --all\n\
         print the نمبر دا installed processors,\n\
         disregarding any OpenMP environment variables, or CPU quotas.\n\
"));
      oputs (_("\
      --ignore=N\n\
         إذا possible, exclude N processing units.\n\
         The نتیجہ is guaranteed منزل be at گھٹ_توں_گھٹ 1.\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت);
}

صحيح
مکھ (صحيح argc, حرف **argv)
{
  بنانشانی لما ignore = 0;
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  تعداد nproc_query منوال_2 = NPROC_CURRENT_OVERRIDABLE;

  طالما (true)
    {
      صحيح c = getopt_long (argc, argv, "", longopts, عدم);
      إذا (c == -1)
        اكسر;
      بدل (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        حالة ALL_OPTION:
          منوال_2 = NPROC_ALL;
          اكسر;

        حالة IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, ULONG_MAX, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          اكسر;

        افتراضي:
          usage (EXIT_FAILURE);
        }
    }

  إذا (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  بنانشانی لما nproc = عدد_processors (منوال_2);

  إذا (ignore < nproc)
    nproc -= ignore;
  وإلا
    nproc = 1;

  لکھو ("%lu\n", nproc);

  أرجع EXIT_SUCCESS;
}
