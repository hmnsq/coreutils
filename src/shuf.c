/* Shuffle lines of text.

   Copyright (C) 2006-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.

   Written by Paul Eggert.  */

#అన్తర్భావ <config.h>

#అన్తర్భావ <sys/types.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ "system.h"

#అన్తర్భావ "fadvise.h"
#అన్తర్భావ "linebuffer.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "randint.h"
#అన్తర్భావ "randperm.h"
#అన్తర్భావ "read-file.h"
#అన్తర్భావ "stdio--.h"
#అన్తర్భావ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "shuf"

#నిర్వచించు AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
గణన { RESERVOIR_MIN_ఆదానం = 8192 * 1024 };


శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              కార్యక్రమం_name, కార్యక్రమం_name, కార్యక్రమం_name);
      fputs (_("\
Write a random permutation యొక్క the ఆదానం పంక్తులు గమ్యం standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an ఆదానం పంక్తి\n\
"));
      oputs (_("\
  -i, --ఆదానం-range=LO-HI   treat each సంఖ్య LO through HI as an ఆదానం పంక్తి\n\
"));
      oputs (_("\
  -n, --తల-లెక్క=లెక్క_2    output at most లెక్క_2 పంక్తులు\n\
"));
      oputs (_("\
  -o, --output=FILE         లిఖ ఫలితం గమ్యం FILE instead యొక్క standard output\n\
"));
      oputs (_("\
      --random-ఆధారం_2=FILE  get random బైట్లు from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output పంక్తులు can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     పంక్తి delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }

  నిష్క్రమించు (స్థితి_2);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
గణన
{
  RANDOM_ఆధారం_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి long_opts[] =
{
  {"echo", no_వాదం, ఖాళీ, 'e'},
  {"input-range", required_వాదం, ఖాళీ, 'i'},
  {"head-count", required_వాదం, ఖాళీ, 'n'},
  {"output", required_వాదం, ఖాళీ, 'o'},
  {"random-source", required_వాదం, ఖాళీ, RANDOM_ఆధారం_OPTION},
  {"repeat", no_వాదం, ఖాళీ, 'r'},
  {"zero-terminated", no_వాదం, ఖాళీ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0},
};

స్థిర శూన్య
ఆదానం_from_argv (అక్షరం **నిర్వాహ్యం_2, పూర్ణాంకం n_operands, అక్షరం eolbyte)
{
  అక్షరం *p;
  size_t కొలత = n_operands;
  పూర్ణాంకం i;

  కొరకు (i = 0; i < n_operands; i++)
    కొలత += అక్షరశ్రేణి_పొడవు (నిర్వాహ్యం_2[i]);
  p = xmalloc (కొలత);

  కొరకు (i = 0; i < n_operands; i++)
    {
      అక్షరం *p1 = stpcpy (p, నిర్వాహ్యం_2[i]);
      నిర్వాహ్యం_2[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  నిర్వాహ్యం_2[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

స్థిర అక్షరం *
తదుపరి_పంక్తి (అక్షరం *పంక్తి, అక్షరం eolbyte)
{
  అక్షరం *p = rawmemchr (పంక్తి, eolbyte);
  తిరిగి p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

స్థిర off_t
ఆదానం_కొలత (శూన్య)
{
  off_t file_కొలత;

  నిర్మాణం stat stat_buf;
  ఒకవేళ (fstat (STDIN_FILENO, &stat_buf) != 0)
    తిరిగి OFF_T_MAX;
  ఒకవేళ (usable_st_కొలత (&stat_buf))
    file_కొలత = stat_buf.st_కొలత;
  లేకపోతే
    తిరిగి OFF_T_MAX;

  off_t ఆదానం_స్థానభ్రంశం = lseek (STDIN_FILENO, 0, SEEK_CUR);
  ఒకవేళ (ఆదానం_స్థానభ్రంశం < 0)
    తిరిగి OFF_T_MAX;

  file_కొలత -= ఆదానం_స్థానభ్రంశం;

  తిరిగి file_కొలత;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

స్థిర idx_t
read_ఆదానం_reservoir_sampling (FILE *లోపల, అక్షరం eolbyte, idx_t k,
                               నిర్మాణం randint_ఆధారం *s,
                               నిర్మాణం linebuffer **బహిర్గతం_rsrv)
{
  randint n_పంక్తులు = 0;
  idx_t n_alloc_పంక్తులు = 0;
  నిర్మాణం linebuffer *పంక్తి = ఖాళీ;
  నిర్మాణం linebuffer *rsrv = ఖాళీ;

  /* Fill the first K lines, directly into the reservoir.  */
  కొరకు (n_పంక్తులు = 0; n_పంక్తులు < k; n_పంక్తులు++)
    {
      /* Enlarge reservoir if needed.  */
      ఒకవేళ (n_పంక్తులు == n_alloc_పంక్తులు)
        {
          idx_t old = n_alloc_పంక్తులు;
          rsrv = xpalloc (rsrv, &n_alloc_పంక్తులు, 1, k, పరిమాణం *rsrv);
          memset (&rsrv[n_పంక్తులు], 0, (n_alloc_పంక్తులు - old) * పరిమాణం *rsrv);
        }

      పంక్తి = readlinebuffer_delim (&rsrv[n_పంక్తులు], లోపల, eolbyte);
      ఒకవేళ (!పంక్తి)
        ఆపు;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  ఒకవేళ (పంక్తి != ఖాళీ)
    {
      నిర్మాణం linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      చేయి
        {
          randint j = randint_ఎంచుకో (s, n_పంక్తులు + 1);  /* 0 .. n_lines.  */
          పంక్తి = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      అయితే (readlinebuffer_delim (పంక్తి, లోపల, eolbyte) != ఖాళీ && n_పంక్తులు++);

      ఒకవేళ (! n_పంక్తులు)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  ఒకవేళ (ferror (లోపల))
    error (EXIT_FAILURE, errno, _("read error"));

  *బహిర్గతం_rsrv = rsrv;
  తిరిగి MIN (k, n_పంక్తులు);
}

స్థిర పూర్ణాంకం
write_permuted_output_reservoir (size_t n_పంక్తులు, నిర్మాణం linebuffer *పంక్తులు,
                                 size_t స్థిరరాశి *permutation)
{
  కొరకు (size_t i = 0; i < n_పంక్తులు; i++)
    {
      స్థిరరాశి నిర్మాణం linebuffer *p = &పంక్తులు[permutation[i]];
      ఒకవేళ (fwrite (p->బఫరు, పరిమాణం (అక్షరం), p->నిడివి, stdout) != p->నిడివి)
        తిరిగి -1;
    }

  తిరిగి 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

స్థిర size_t
read_ఆదానం (FILE *లోపల, అక్షరం eolbyte, అక్షరం ***pline)
{
  అక్షరం *p;
  అక్షరం *buf = ఖాళీ;
  size_t వాడిన;
  అక్షరం *lim;
  అక్షరం **పంక్తి;
  size_t n_పంక్తులు;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  ఒకవేళ (!(buf = fread_file (లోపల, 0, &వాడిన)))
    error (EXIT_FAILURE, errno, _("read error"));

  ఒకవేళ (వాడిన && buf[వాడిన - 1] != eolbyte)
    buf[వాడిన++] = eolbyte;

  lim = buf + వాడిన;

  n_పంక్తులు = 0;
  కొరకు (p = buf; p < lim; p = తదుపరి_పంక్తి (p, eolbyte))
    n_పంక్తులు++;

  *pline = పంక్తి = xnmalloc (n_పంక్తులు + 1, పరిమాణం *పంక్తి);

  పంక్తి[0] = p = buf;
  కొరకు (size_t i = 1; i <= n_పంక్తులు; i++)
    పంక్తి[i] = p = తదుపరి_పంక్తి (p, eolbyte);

  తిరిగి n_పంక్తులు;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
స్థిర పూర్ణాంకం
write_permuted_పంక్తులు (size_t n_పంక్తులు, అక్షరం *స్థిరరాశి *పంక్తి,
                      size_t స్థిరరాశి *permutation)
{
  కొరకు (size_t i = 0; i < n_పంక్తులు; i++)
    {
      అక్షరం *స్థిరరాశి *p = పంక్తి + permutation[i];
      size_t len = p[1] - p[0];
      ఒకవేళ (fwrite (p[0], పరిమాణం *p[0], len, stdout) != len)
        తిరిగి -1;
    }

  తిరిగి 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
స్థిర bool
print_సంఖ్య (గుర్తులేని దీర్ఘ పూర్ణాంకం సంఖ్య, అక్షరం eolbyte)
{
  అక్షరం buf[INT_BUFSIZE_BOUND (గుర్తులేని దీర్ఘ పూర్ణాంకం)];
  అక్షరం *p = buf + INT_STRLEN_BOUND (గుర్తులేని దీర్ఘ పూర్ణాంకం);
  *p = eolbyte;
  చేయి
    *--p = '0' + సంఖ్య % 10;
  అయితే ((సంఖ్య /= 10) != 0);
  idx_t len = buf + పరిమాణం buf - p;
  తిరిగి fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
స్థిర పూర్ణాంకం
write_permuted_numbers (size_t n_పంక్తులు, size_t lo_ఆదానం,
                        size_t స్థిరరాశి *permutation, అక్షరం eolbyte)
{
  కొరకు (size_t i = 0; i < n_పంక్తులు; i++)
    {
      గుర్తులేని దీర్ఘ పూర్ణాంకం n = lo_ఆదానం + permutation[i];
      ఒకవేళ (! print_సంఖ్య (n, eolbyte))
        తిరిగి -1;
    }

  తిరిగి 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
స్థిర పూర్ణాంకం
write_random_numbers (నిర్మాణం randint_ఆధారం *s, size_t లెక్క,
                      size_t lo_ఆదానం, size_t hi_ఆదానం, అక్షరం eolbyte)
{
  స్థిరరాశి randint range = hi_ఆదానం - lo_ఆదానం + 1;

  కొరకు (size_t i = 0; i < లెక్క; i++)
    {
      గుర్తులేని దీర్ఘ పూర్ణాంకం j = lo_ఆదానం + randint_ఎంచుకో (s, range);
      ఒకవేళ (! print_సంఖ్య (j, eolbyte))
        తిరిగి -1;
    }

  తిరిగి 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
స్థిర పూర్ణాంకం
write_random_పంక్తులు (నిర్మాణం randint_ఆధారం *s, size_t లెక్క,
                    అక్షరం *స్థిరరాశి *పంక్తులు, size_t n_పంక్తులు)
{
  కొరకు (size_t i = 0; i < లెక్క; i++)
    {
      స్థిరరాశి randint j = randint_ఎంచుకో (s, n_పంక్తులు);
      అక్షరం *స్థిరరాశి *p = పంక్తులు + j;
      size_t len = p[1] - p[0];
      ఒకవేళ (fwrite (p[0], పరిమాణం *p[0], len, stdout) != len)
        తిరిగి -1;
    }

  తిరిగి 0;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  bool echo = false;
  bool ఆదానం_range = false;
  size_t lo_ఆదానం = SIZE_MAX;
  size_t hi_ఆదానం = 0;
  idx_t తల_పంక్తులు = MIN (IDX_MAX, SIZE_MAX);
  అక్షరం స్థిరరాశి *outfile = ఖాళీ;
  అక్షరం *random_ఆధారం = ఖాళీ;
  అక్షరం eolbyte = '\n';
  అక్షరం **ఆదానం_పంక్తులు = ఖాళీ;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  పూర్ణాంకం optc;
  పూర్ణాంకం n_operands;
  అక్షరం **నిర్వాహ్యం_2;
  size_t n_పంక్తులు;
  అక్షరం **పంక్తి = ఖాళీ;
  నిర్మాణం linebuffer *reservoir = ఖాళీ;
  నిర్మాణం randint_ఆధారం *randint_ఆధారం;
  size_t *permutation = ఖాళీ;
  పూర్ణాంకం i;

  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  అయితే ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, ఖాళీ))
         != -1)
    ఎంపిక (optc)
      {
      స్థితి 'e':
        echo = true;
        ఆపు;

      స్థితి 'i':
        {
          ఒకవేళ (ఆదానం_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          ఆదానం_range = true;

          uintmax_t u;
          అక్షరం *lo_చివర;
          strtol_error err = xstrtoumax (optarg, &lo_చివర, 10, &u, ఖాళీ);
          ఒకవేళ (err == LONGINT_OK)
            {
              lo_ఆదానం = u;
              ఒకవేళ (lo_ఆదానం != u)
                err = LONGINT_అధికప్రవాహం;
              లేకపోతే ఒకవేళ (*lo_చివర != '-')
                err = LONGINT_INVALID;
              లేకపోతే
                {
                  err = xstrtoumax (lo_చివర + 1, ఖాళీ, 10, &u, "");
                  ఒకవేళ (err == LONGINT_OK)
                    {
                      hi_ఆదానం = u;
                      ఒకవేళ (hi_ఆదానం != u)
                        err = LONGINT_అధికప్రవాహం;
                    }
                }
            }

          n_పంక్తులు = hi_ఆదానం - lo_ఆదానం + 1;

          ఒకవేళ (err != LONGINT_OK || (lo_ఆదానం <= hi_ఆదానం) == (n_పంక్తులు == 0))
            error (EXIT_FAILURE, err == LONGINT_అధికప్రవాహం ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        ఆపు;

      స్థితి 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, ఖాళీ, 10, &argval, "");

          ఒకవేళ (e == LONGINT_OK)
            తల_పంక్తులు = MIN (తల_పంక్తులు, argval);
          లేకపోతే ఒకవేళ (e != LONGINT_అధికప్రవాహం)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        ఆపు;

      స్థితి 'o':
        ఒకవేళ (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        ఆపు;

      స్థితి RANDOM_ఆధారం_OPTION:
        ఒకవేళ (random_ఆధారం && !streq (random_ఆధారం, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_ఆధారం = optarg;
        ఆపు;

      స్థితి 'r':
        repeat = true;
        ఆపు;

      స్థితి 'z':
        eolbyte = '\0';
        ఆపు;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);
      అప్రమేయ:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  నిర్వాహ్యం_2 = argv + optind;

  /* Check invalid usage.  */
  ఒకవేళ (echo && ఆదానం_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  ఒకవేళ (ఆదానం_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (నిర్వాహ్యం_2[!ఆదానం_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  ఒకవేళ (తల_పంక్తులు == 0)
    {
      n_పంక్తులు = 0;
      పంక్తి = ఖాళీ;
    }
  లేకపోతే ఒకవేళ (echo)
    {
      ఆదానం_from_argv (నిర్వాహ్యం_2, n_operands, eolbyte);
      n_పంక్తులు = n_operands;
      పంక్తి = నిర్వాహ్యం_2;
    }
  లేకపోతే ఒకవేళ (ఆదానం_range)
    {
      IF_LINT (n_పంక్తులు = hi_ఆదానం - lo_ఆదానం + 1); /* Avoid GCC 10 warning.  */
      పంక్తి = ఖాళీ;
    }
  లేకపోతే
    {
      /* If an input file is specified, re-open it as stdin.  */
      ఒకవేళ (n_operands == 1
          && ! (streq (నిర్వాహ్యం_2[0], "-")
                || freopen (నిర్వాహ్యం_2[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (నిర్వాహ్యం_2[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      ఒకవేళ (repeat || తల_పంక్తులు == MIN (IDX_MAX, SIZE_MAX)
          || ఆదానం_కొలత () <= RESERVOIR_MIN_ఆదానం)
        {
          n_పంక్తులు = read_ఆదానం (stdin, eolbyte, &ఆదానం_పంక్తులు);
          పంక్తి = ఆదానం_పంక్తులు;
        }
      లేకపోతే
        {
          use_reservoir_sampling = true;
          n_పంక్తులు = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_పంక్తులు = repeat || తల_పంక్తులు < n_పంక్తులు ? తల_పంక్తులు : n_పంక్తులు;

  randint_ఆధారం = randint_all_new (random_ఆధారం,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_పంక్తులు, n_పంక్తులు)));
  ఒకవేళ (! randint_ఆధారం)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_ఆధారం ? random_ఆధారం : "getrandom"));

  ఒకవేళ (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_పంక్తులు = read_ఆదానం_reservoir_sampling (stdin, eolbyte, ahead_పంక్తులు,
                                               randint_ఆధారం, &reservoir);
      ahead_పంక్తులు = n_పంక్తులు;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  ఒకవేళ (! (తల_పంక్తులు == 0 || echo || ఆదానం_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  ఒకవేళ (!repeat)
    permutation = randperm_new (randint_ఆధారం, ahead_పంక్తులు, n_పంక్తులు);

  ఒకవేళ (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  ఒకవేళ (repeat)
    {
      ఒకవేళ (తల_పంక్తులు == 0)
        i = 0;
      లేకపోతే
        {
          ఒకవేళ (n_పంక్తులు == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          ఒకవేళ (ఆదానం_range)
            i = write_random_numbers (randint_ఆధారం, ahead_పంక్తులు,
                                      lo_ఆదానం, hi_ఆదానం, eolbyte);
          లేకపోతే
            i = write_random_పంక్తులు (randint_ఆధారం, ahead_పంక్తులు, పంక్తి, n_పంక్తులు);
        }
    }
  లేకపోతే
    {
      ఒకవేళ (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_పంక్తులు, reservoir, permutation);
      లేకపోతే ఒకవేళ (ఆదానం_range)
        i = write_permuted_numbers (ahead_పంక్తులు, lo_ఆదానం,
                                    permutation, eolbyte);
      లేకపోతే
        i = write_permuted_పంక్తులు (ahead_పంక్తులు, పంక్తి, permutation);
    }

  ఒకవేళ (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_ఆధారం));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
