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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "nproc.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xdectoint.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "nproc"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Giuseppe Scrivano")

ਗਿਣਤੀ
{
  ALL_OPTION = CHAR_MAX + 1,
  IGNORE_OPTION
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ longopts[] =
{
  {"all", no_ਦਲੀਲ, ਨਿਰਾ, ALL_OPTION},
  {"ignore", required_ਦਲੀਲ, ਨਿਰਾ, IGNORE_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0}
};

ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("Usage: %s [OPTION]...\n"), ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Print the ਸੰਖਿਆ ਦਾ processing units ਉਪਲਬਧ ਮੰਜ਼ਿਲ the current process,\n\
which may be less than the ਸੰਖਿਆ ਦਾ online processors.\n\
If the 'OMP_NUM_THREADS' or 'OMP_THREAD_LIMIT' environment variables are ਸੈੱਟ_2,\n\
then they will determine the minimum and ਵੱਧ_ਤੋਂ_ਵੱਧ returned ਮੁੱਲ respectively.\n\
\n\
"), stdout);
      oputs (_("\
      --all\n\
         print the ਸੰਖਿਆ ਦਾ installed processors,\n\
         disregarding any OpenMP environment variables, or CPU quotas.\n\
"));
      oputs (_("\
      --ignore=N\n\
         ਜੇ possible, exclude N processing units.\n\
         The ਨਤੀਜਾ is guaranteed ਮੰਜ਼ਿਲ be at ਘੱਟ_ਤੋਂ_ਘੱਟ 1.\n\
"));

      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }
  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ignore = 0;
  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਗਿਣਤੀ nproc_query ਬਹੁਲਕ_2 = NPROC_CURRENT_OVERRIDABLE;

  ਜਦੋਂ (true)
    {
      ਪੂਰਨ c = getopt_long (argc, argv, "", longopts, ਨਿਰਾ);
      ਜੇ (c == -1)
        ਰੋਕੋ;
      ਚੋਣ (c)
        {
        case_GETOPT_HELP_CHAR;

        case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);

        ਹਾਲਤ ALL_OPTION:
          ਬਹੁਲਕ_2 = NPROC_ALL;
          ਰੋਕੋ;

        ਹਾਲਤ IGNORE_OPTION:
          ignore = xnumtoumax (optarg, 10, 0, ULONG_MAX, "",
                               _("invalid number"), 0, XTOINT_MAX_QUIET);
          ਰੋਕੋ;

        ਮੂਲ:
          usage (EXIT_FAILURE);
        }
    }

  ਜੇ (argc != optind)
    {
      error (0, 0, _("extra operand %s"), quote (argv[optind]));
      usage (EXIT_FAILURE);
    }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ nproc = ਅੰਕ_processors (ਬਹੁਲਕ_2);

  ਜੇ (ignore < nproc)
    nproc -= ignore;
  ਨਹੀਂਤਾਂ
    nproc = 1;

  ਲਿਖੋ ("%lu\n", nproc);

  ਵਾਪਸ EXIT_SUCCESS;
}
