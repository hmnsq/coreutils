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

#অন্তর্ভাব <config.h>

#অন্তর্ভাব <sys/types.h>
#অন্তর্ভাব <getopt.h>
#অন্তর্ভাব "system.h"

#অন্তর্ভাব "fadvise.h"
#অন্তর্ভাব "linebuffer.h"
#অন্তর্ভাব "quote.h"
#অন্তর্ভাব "randint.h"
#অন্তর্ভাব "randperm.h"
#অন্তর্ভাব "read-file.h"
#অন্তর্ভাব "stdio--.h"
#অন্তর্ভাব "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "shuf"

#define AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
গণনা { RESERVOIR_MIN_প্রবেশ = 8192 * 1024 };


শূন্য
usage (পূর্ণসংখ্যা status)
{
  যদি (status != EXIT_SUCCESS)
    emit_try_help ();
  অন্যথা
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the প্রবেশ সারিগুলি to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an প্রবেশ সারি\n\
"));
      oputs (_("\
  -i, --প্রবেশ-range=LO-HI   treat each সংখ্যা LO through HI as an প্রবেশ সারি\n\
"));
      oputs (_("\
  -n, --শীর্ষ-গুনতি=গুনতি_2    output at most গুনতি_2 সারিগুলি\n\
"));
      oputs (_("\
  -o, --output=FILE         লিখ ফলাফল to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output সারিগুলি can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     সারি delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
গণনা
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

স্থির কাঠামো option ধ্রুবক long_opts[] =
{
  {"echo", no_argument, ফাঁকা, 'e'},
  {"input-range", required_argument, ফাঁকা, 'i'},
  {"head-count", required_argument, ফাঁকা, 'n'},
  {"output", required_argument, ফাঁকা, 'o'},
  {"random-source", required_argument, ফাঁকা, RANDOM_SOURCE_OPTION},
  {"repeat", no_argument, ফাঁকা, 'r'},
  {"zero-terminated", no_argument, ফাঁকা, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ফাঁকা, 0, ফাঁকা, 0},
};

স্থির শূন্য
প্রবেশ_from_argv (অক্ষর **operand, পূর্ণসংখ্যা n_operands, অক্ষর eolbyte)
{
  অক্ষর *p;
  size_t আকার_2 = n_operands;
  পূর্ণসংখ্যা i;

  জন্য (i = 0; i < n_operands; i++)
    আকার_2 += strlen (operand[i]);
  p = xmalloc (আকার_2);

  জন্য (i = 0; i < n_operands; i++)
    {
      অক্ষর *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

স্থির অক্ষর *
পরবর্তী_সারি (অক্ষর *সারি, অক্ষর eolbyte)
{
  অক্ষর *p = rawmemchr (সারি, eolbyte);
  ফেরত p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

স্থির off_t
প্রবেশ_আকার (শূন্য)
{
  off_t file_আকার;

  কাঠামো stat stat_buf;
  যদি (fstat (STDIN_FILENO, &stat_buf) != 0)
    ফেরত OFF_T_MAX;
  যদি (usable_st_আকার (&stat_buf))
    file_আকার = stat_buf.st_আকার;
  অন্যথা
    ফেরত OFF_T_MAX;

  off_t প্রবেশ_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  যদি (প্রবেশ_offset < 0)
    ফেরত OFF_T_MAX;

  file_আকার -= প্রবেশ_offset;

  ফেরত file_আকার;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

স্থির idx_t
read_প্রবেশ_reservoir_sampling (FILE *in, অক্ষর eolbyte, idx_t k,
                               কাঠামো randint_source *s,
                               কাঠামো linebuffer **out_rsrv)
{
  randint n_সারিগুলি = 0;
  idx_t n_alloc_সারিগুলি = 0;
  কাঠামো linebuffer *সারি = ফাঁকা;
  কাঠামো linebuffer *rsrv = ফাঁকা;

  /* Fill the first K lines, directly into the reservoir.  */
  জন্য (n_সারিগুলি = 0; n_সারিগুলি < k; n_সারিগুলি++)
    {
      /* Enlarge reservoir if needed.  */
      যদি (n_সারিগুলি == n_alloc_সারিগুলি)
        {
          idx_t old = n_alloc_সারিগুলি;
          rsrv = xpalloc (rsrv, &n_alloc_সারিগুলি, 1, k, আকার *rsrv);
          memset (&rsrv[n_সারিগুলি], 0, (n_alloc_সারিগুলি - old) * আকার *rsrv);
        }

      সারি = readlinebuffer_delim (&rsrv[n_সারিগুলি], in, eolbyte);
      যদি (!সারি)
        বিরতি;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  যদি (সারি != ফাঁকা)
    {
      কাঠামো linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      করো
        {
          randint j = randint_choose (s, n_সারিগুলি + 1);  /* 0 .. n_lines.  */
          সারি = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      যখন (readlinebuffer_delim (সারি, in, eolbyte) != ফাঁকা && n_সারিগুলি++);

      যদি (! n_সারিগুলি)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  যদি (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  ফেরত MIN (k, n_সারিগুলি);
}

স্থির পূর্ণসংখ্যা
write_permuted_output_reservoir (size_t n_সারিগুলি, কাঠামো linebuffer *সারিগুলি,
                                 size_t ধ্রুবক *permutation)
{
  জন্য (size_t i = 0; i < n_সারিগুলি; i++)
    {
      ধ্রুবক কাঠামো linebuffer *p = &সারিগুলি[permutation[i]];
      যদি (fwrite (p->বাফার, আকার (অক্ষর), p->দৈর্ঘ্য, stdout) != p->দৈর্ঘ্য)
        ফেরত -1;
    }

  ফেরত 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

স্থির size_t
read_প্রবেশ (FILE *in, অক্ষর eolbyte, অক্ষর ***pline)
{
  অক্ষর *p;
  অক্ষর *buf = ফাঁকা;
  size_t used;
  অক্ষর *lim;
  অক্ষর **সারি;
  size_t n_সারিগুলি;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  যদি (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  যদি (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_সারিগুলি = 0;
  জন্য (p = buf; p < lim; p = পরবর্তী_সারি (p, eolbyte))
    n_সারিগুলি++;

  *pline = সারি = xnmalloc (n_সারিগুলি + 1, আকার *সারি);

  সারি[0] = p = buf;
  জন্য (size_t i = 1; i <= n_সারিগুলি; i++)
    সারি[i] = p = পরবর্তী_সারি (p, eolbyte);

  ফেরত n_সারিগুলি;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
স্থির পূর্ণসংখ্যা
write_permuted_সারিগুলি (size_t n_সারিগুলি, অক্ষর *ধ্রুবক *সারি,
                      size_t ধ্রুবক *permutation)
{
  জন্য (size_t i = 0; i < n_সারিগুলি; i++)
    {
      অক্ষর *ধ্রুবক *p = সারি + permutation[i];
      size_t len = p[1] - p[0];
      যদি (fwrite (p[0], আকার *p[0], len, stdout) != len)
        ফেরত -1;
    }

  ফেরত 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
স্থির bool
print_সংখ্যা (unsigned long পূর্ণসংখ্যা সংখ্যা, অক্ষর eolbyte)
{
  অক্ষর buf[INT_BUFSIZE_BOUND (unsigned long পূর্ণসংখ্যা)];
  অক্ষর *p = buf + INT_STRLEN_BOUND (unsigned long পূর্ণসংখ্যা);
  *p = eolbyte;
  করো
    *--p = '0' + সংখ্যা % 10;
  যখন ((সংখ্যা /= 10) != 0);
  idx_t len = buf + আকার buf - p;
  ফেরত fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
স্থির পূর্ণসংখ্যা
write_permuted_numbers (size_t n_সারিগুলি, size_t lo_প্রবেশ,
                        size_t ধ্রুবক *permutation, অক্ষর eolbyte)
{
  জন্য (size_t i = 0; i < n_সারিগুলি; i++)
    {
      unsigned long পূর্ণসংখ্যা n = lo_প্রবেশ + permutation[i];
      যদি (! print_সংখ্যা (n, eolbyte))
        ফেরত -1;
    }

  ফেরত 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
স্থির পূর্ণসংখ্যা
write_random_numbers (কাঠামো randint_source *s, size_t গুনতি,
                      size_t lo_প্রবেশ, size_t hi_প্রবেশ, অক্ষর eolbyte)
{
  ধ্রুবক randint range = hi_প্রবেশ - lo_প্রবেশ + 1;

  জন্য (size_t i = 0; i < গুনতি; i++)
    {
      unsigned long পূর্ণসংখ্যা j = lo_প্রবেশ + randint_choose (s, range);
      যদি (! print_সংখ্যা (j, eolbyte))
        ফেরত -1;
    }

  ফেরত 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
স্থির পূর্ণসংখ্যা
write_random_সারিগুলি (কাঠামো randint_source *s, size_t গুনতি,
                    অক্ষর *ধ্রুবক *সারিগুলি, size_t n_সারিগুলি)
{
  জন্য (size_t i = 0; i < গুনতি; i++)
    {
      ধ্রুবক randint j = randint_choose (s, n_সারিগুলি);
      অক্ষর *ধ্রুবক *p = সারিগুলি + j;
      size_t len = p[1] - p[0];
      যদি (fwrite (p[0], আকার *p[0], len, stdout) != len)
        ফেরত -1;
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  bool echo = false;
  bool প্রবেশ_range = false;
  size_t lo_প্রবেশ = SIZE_MAX;
  size_t hi_প্রবেশ = 0;
  idx_t শীর্ষ_সারিগুলি = MIN (IDX_MAX, SIZE_MAX);
  অক্ষর ধ্রুবক *outfile = ফাঁকা;
  অক্ষর *random_source = ফাঁকা;
  অক্ষর eolbyte = '\n';
  অক্ষর **প্রবেশ_সারিগুলি = ফাঁকা;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  পূর্ণসংখ্যা optc;
  পূর্ণসংখ্যা n_operands;
  অক্ষর **operand;
  size_t n_সারিগুলি;
  অক্ষর **সারি = ফাঁকা;
  কাঠামো linebuffer *reservoir = ফাঁকা;
  কাঠামো randint_source *randint_source;
  size_t *permutation = ফাঁকা;
  পূর্ণসংখ্যা i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  যখন ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, ফাঁকা))
         != -1)
    নির্বাচন (optc)
      {
      ক্ষেত্র 'e':
        echo = true;
        বিরতি;

      ক্ষেত্র 'i':
        {
          যদি (প্রবেশ_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          প্রবেশ_range = true;

          uintmax_t u;
          অক্ষর *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, ফাঁকা);
          যদি (err == LONGINT_OK)
            {
              lo_প্রবেশ = u;
              যদি (lo_প্রবেশ != u)
                err = LONGINT_OVERFLOW;
              অন্যথা যদি (*lo_end != '-')
                err = LONGINT_INVALID;
              অন্যথা
                {
                  err = xstrtoumax (lo_end + 1, ফাঁকা, 10, &u, "");
                  যদি (err == LONGINT_OK)
                    {
                      hi_প্রবেশ = u;
                      যদি (hi_প্রবেশ != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_সারিগুলি = hi_প্রবেশ - lo_প্রবেশ + 1;

          যদি (err != LONGINT_OK || (lo_প্রবেশ <= hi_প্রবেশ) == (n_সারিগুলি == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        বিরতি;

      ক্ষেত্র 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, ফাঁকা, 10, &argval, "");

          যদি (e == LONGINT_OK)
            শীর্ষ_সারিগুলি = MIN (শীর্ষ_সারিগুলি, argval);
          অন্যথা যদি (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        বিরতি;

      ক্ষেত্র 'o':
        যদি (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        বিরতি;

      ক্ষেত্র RANDOM_SOURCE_OPTION:
        যদি (random_source && !streq (random_source, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_source = optarg;
        বিরতি;

      ক্ষেত্র 'r':
        repeat = true;
        বিরতি;

      ক্ষেত্র 'z':
        eolbyte = '\0';
        বিরতি;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      পূর্বনির্ধারিত:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  যদি (echo && প্রবেশ_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  যদি (প্রবেশ_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!প্রবেশ_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  যদি (শীর্ষ_সারিগুলি == 0)
    {
      n_সারিগুলি = 0;
      সারি = ফাঁকা;
    }
  অন্যথা যদি (echo)
    {
      প্রবেশ_from_argv (operand, n_operands, eolbyte);
      n_সারিগুলি = n_operands;
      সারি = operand;
    }
  অন্যথা যদি (প্রবেশ_range)
    {
      IF_LINT (n_সারিগুলি = hi_প্রবেশ - lo_প্রবেশ + 1); /* Avoid GCC 10 warning.  */
      সারি = ফাঁকা;
    }
  অন্যথা
    {
      /* If an input file is specified, re-open it as stdin.  */
      যদি (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      যদি (repeat || শীর্ষ_সারিগুলি == MIN (IDX_MAX, SIZE_MAX)
          || প্রবেশ_আকার () <= RESERVOIR_MIN_প্রবেশ)
        {
          n_সারিগুলি = read_প্রবেশ (stdin, eolbyte, &প্রবেশ_সারিগুলি);
          সারি = প্রবেশ_সারিগুলি;
        }
      অন্যথা
        {
          use_reservoir_sampling = true;
          n_সারিগুলি = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_সারিগুলি = repeat || শীর্ষ_সারিগুলি < n_সারিগুলি ? শীর্ষ_সারিগুলি : n_সারিগুলি;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_সারিগুলি, n_সারিগুলি)));
  যদি (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  যদি (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_সারিগুলি = read_প্রবেশ_reservoir_sampling (stdin, eolbyte, ahead_সারিগুলি,
                                               randint_source, &reservoir);
      ahead_সারিগুলি = n_সারিগুলি;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  যদি (! (শীর্ষ_সারিগুলি == 0 || echo || প্রবেশ_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  যদি (!repeat)
    permutation = randperm_new (randint_source, ahead_সারিগুলি, n_সারিগুলি);

  যদি (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  যদি (repeat)
    {
      যদি (শীর্ষ_সারিগুলি == 0)
        i = 0;
      অন্যথা
        {
          যদি (n_সারিগুলি == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          যদি (প্রবেশ_range)
            i = write_random_numbers (randint_source, ahead_সারিগুলি,
                                      lo_প্রবেশ, hi_প্রবেশ, eolbyte);
          অন্যথা
            i = write_random_সারিগুলি (randint_source, ahead_সারিগুলি, সারি, n_সারিগুলি);
        }
    }
  অন্যথা
    {
      যদি (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_সারিগুলি, reservoir, permutation);
      অন্যথা যদি (প্রবেশ_range)
        i = write_permuted_numbers (ahead_সারিগুলি, lo_প্রবেশ,
                                    permutation, eolbyte);
      অন্যথা
        i = write_permuted_সারিগুলি (ahead_সারিগুলি, সারি, permutation);
    }

  যদি (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
