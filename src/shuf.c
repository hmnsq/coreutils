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

#અન્તર્ભાવ <config.h>

#અન્તર્ભાવ <sys/types.h>
#અન્તર્ભાવ <getopt.h>
#અન્તર્ભાવ "system.h"

#અન્તર્ભાવ "fadvise.h"
#અન્તર્ભાવ "linebuffer.h"
#અન્તર્ભાવ "quote.h"
#અન્તર્ભાવ "randint.h"
#અન્તર્ભાવ "randperm.h"
#અન્તર્ભાવ "read-file.h"
#અન્તર્ભાવ "stdio--.h"
#અન્તર્ભાવ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "shuf"

#define AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
ગણના { RESERVOIR_MIN_આગત = 8192 * 1024 };


શૂન્ય
usage (પૂર્ણાંક status)
{
  જો (status != EXIT_SUCCESS)
    emit_try_help ();
  નહીંતર
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the આગત લીટીઓ to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an આગત લીટી\n\
"));
      oputs (_("\
  -i, --આગત-range=LO-HI   treat each સંખ્યા LO through HI as an આગત લીટી\n\
"));
      oputs (_("\
  -n, --head-ગણતરી=ગણતરી_2    output at most ગણતરી_2 લીટીઓ\n\
"));
      oputs (_("\
  -o, --output=FILE         લિખ result to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output લીટીઓ can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     લીટી delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ગણના
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

સ્થિર સંરચના option અચલ long_opts[] =
{
  {"echo", no_argument, ખાલી, 'e'},
  {"input-range", required_argument, ખાલી, 'i'},
  {"head-count", required_argument, ખાલી, 'n'},
  {"output", required_argument, ખાલી, 'o'},
  {"random-source", required_argument, ખાલી, RANDOM_SOURCE_OPTION},
  {"repeat", no_argument, ખાલી, 'r'},
  {"zero-terminated", no_argument, ખાલી, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ખાલી, 0, ખાલી, 0},
};

સ્થિર શૂન્ય
આગત_from_argv (અક્ષર **operand, પૂર્ણાંક n_operands, અક્ષર eolbyte)
{
  અક્ષર *p;
  size_t size = n_operands;
  પૂર્ણાંક i;

  માટે (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  માટે (i = 0; i < n_operands; i++)
    {
      અક્ષર *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

સ્થિર અક્ષર *
next_લીટી (અક્ષર *લીટી, અક્ષર eolbyte)
{
  અક્ષર *p = rawmemchr (લીટી, eolbyte);
  પાછા p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

સ્થિર off_t
આગત_size (શૂન્ય)
{
  off_t file_size;

  સંરચના stat stat_buf;
  જો (fstat (STDIN_FILENO, &stat_buf) != 0)
    પાછા OFF_T_MAX;
  જો (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  નહીંતર
    પાછા OFF_T_MAX;

  off_t આગત_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  જો (આગત_offset < 0)
    પાછા OFF_T_MAX;

  file_size -= આગત_offset;

  પાછા file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

સ્થિર idx_t
read_આગત_reservoir_sampling (FILE *in, અક્ષર eolbyte, idx_t k,
                               સંરચના randint_source *s,
                               સંરચના linebuffer **out_rsrv)
{
  randint n_લીટીઓ = 0;
  idx_t n_alloc_લીટીઓ = 0;
  સંરચના linebuffer *લીટી = ખાલી;
  સંરચના linebuffer *rsrv = ખાલી;

  /* Fill the first K lines, directly into the reservoir.  */
  માટે (n_લીટીઓ = 0; n_લીટીઓ < k; n_લીટીઓ++)
    {
      /* Enlarge reservoir if needed.  */
      જો (n_લીટીઓ == n_alloc_લીટીઓ)
        {
          idx_t old = n_alloc_લીટીઓ;
          rsrv = xpalloc (rsrv, &n_alloc_લીટીઓ, 1, k, કદ *rsrv);
          memset (&rsrv[n_લીટીઓ], 0, (n_alloc_લીટીઓ - old) * કદ *rsrv);
        }

      લીટી = readlinebuffer_delim (&rsrv[n_લીટીઓ], in, eolbyte);
      જો (!લીટી)
        તોડો;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  જો (લીટી != ખાલી)
    {
      સંરચના linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      કરો
        {
          randint j = randint_choose (s, n_લીટીઓ + 1);  /* 0 .. n_lines.  */
          લીટી = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      જ્યારે (readlinebuffer_delim (લીટી, in, eolbyte) != ખાલી && n_લીટીઓ++);

      જો (! n_લીટીઓ)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  જો (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  પાછા MIN (k, n_લીટીઓ);
}

સ્થિર પૂર્ણાંક
write_permuted_output_reservoir (size_t n_લીટીઓ, સંરચના linebuffer *લીટીઓ,
                                 size_t અચલ *permutation)
{
  માટે (size_t i = 0; i < n_લીટીઓ; i++)
    {
      અચલ સંરચના linebuffer *p = &લીટીઓ[permutation[i]];
      જો (fwrite (p->buffer, કદ (અક્ષર), p->length, stdout) != p->length)
        પાછા -1;
    }

  પાછા 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

સ્થિર size_t
read_આગત (FILE *in, અક્ષર eolbyte, અક્ષર ***pline)
{
  અક્ષર *p;
  અક્ષર *buf = ખાલી;
  size_t used;
  અક્ષર *lim;
  અક્ષર **લીટી;
  size_t n_લીટીઓ;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  જો (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  જો (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_લીટીઓ = 0;
  માટે (p = buf; p < lim; p = next_લીટી (p, eolbyte))
    n_લીટીઓ++;

  *pline = લીટી = xnmalloc (n_લીટીઓ + 1, કદ *લીટી);

  લીટી[0] = p = buf;
  માટે (size_t i = 1; i <= n_લીટીઓ; i++)
    લીટી[i] = p = next_લીટી (p, eolbyte);

  પાછા n_લીટીઓ;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
સ્થિર પૂર્ણાંક
write_permuted_લીટીઓ (size_t n_લીટીઓ, અક્ષર *અચલ *લીટી,
                      size_t અચલ *permutation)
{
  માટે (size_t i = 0; i < n_લીટીઓ; i++)
    {
      અક્ષર *અચલ *p = લીટી + permutation[i];
      size_t len = p[1] - p[0];
      જો (fwrite (p[0], કદ *p[0], len, stdout) != len)
        પાછા -1;
    }

  પાછા 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
સ્થિર bool
print_સંખ્યા (unsigned long પૂર્ણાંક સંખ્યા, અક્ષર eolbyte)
{
  અક્ષર buf[INT_BUFSIZE_BOUND (unsigned long પૂર્ણાંક)];
  અક્ષર *p = buf + INT_STRLEN_BOUND (unsigned long પૂર્ણાંક);
  *p = eolbyte;
  કરો
    *--p = '0' + સંખ્યા % 10;
  જ્યારે ((સંખ્યા /= 10) != 0);
  idx_t len = buf + કદ buf - p;
  પાછા fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
સ્થિર પૂર્ણાંક
write_permuted_numbers (size_t n_લીટીઓ, size_t lo_આગત,
                        size_t અચલ *permutation, અક્ષર eolbyte)
{
  માટે (size_t i = 0; i < n_લીટીઓ; i++)
    {
      unsigned long પૂર્ણાંક n = lo_આગત + permutation[i];
      જો (! print_સંખ્યા (n, eolbyte))
        પાછા -1;
    }

  પાછા 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
સ્થિર પૂર્ણાંક
write_random_numbers (સંરચના randint_source *s, size_t ગણતરી,
                      size_t lo_આગત, size_t hi_આગત, અક્ષર eolbyte)
{
  અચલ randint range = hi_આગત - lo_આગત + 1;

  માટે (size_t i = 0; i < ગણતરી; i++)
    {
      unsigned long પૂર્ણાંક j = lo_આગત + randint_choose (s, range);
      જો (! print_સંખ્યા (j, eolbyte))
        પાછા -1;
    }

  પાછા 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
સ્થિર પૂર્ણાંક
write_random_લીટીઓ (સંરચના randint_source *s, size_t ગણતરી,
                    અક્ષર *અચલ *લીટીઓ, size_t n_લીટીઓ)
{
  માટે (size_t i = 0; i < ગણતરી; i++)
    {
      અચલ randint j = randint_choose (s, n_લીટીઓ);
      અક્ષર *અચલ *p = લીટીઓ + j;
      size_t len = p[1] - p[0];
      જો (fwrite (p[0], કદ *p[0], len, stdout) != len)
        પાછા -1;
    }

  પાછા 0;
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  bool echo = false;
  bool આગત_range = false;
  size_t lo_આગત = SIZE_MAX;
  size_t hi_આગત = 0;
  idx_t head_લીટીઓ = MIN (IDX_MAX, SIZE_MAX);
  અક્ષર અચલ *outfile = ખાલી;
  અક્ષર *random_source = ખાલી;
  અક્ષર eolbyte = '\n';
  અક્ષર **આગત_લીટીઓ = ખાલી;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  પૂર્ણાંક optc;
  પૂર્ણાંક n_operands;
  અક્ષર **operand;
  size_t n_લીટીઓ;
  અક્ષર **લીટી = ખાલી;
  સંરચના linebuffer *reservoir = ખાલી;
  સંરચના randint_source *randint_source;
  size_t *permutation = ખાલી;
  પૂર્ણાંક i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  જ્યારે ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, ખાલી))
         != -1)
    પસંદગી (optc)
      {
      સ્થિતિ 'e':
        echo = true;
        તોડો;

      સ્થિતિ 'i':
        {
          જો (આગત_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          આગત_range = true;

          uintmax_t u;
          અક્ષર *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, ખાલી);
          જો (err == LONGINT_OK)
            {
              lo_આગત = u;
              જો (lo_આગત != u)
                err = LONGINT_OVERFLOW;
              નહીંતર જો (*lo_end != '-')
                err = LONGINT_INVALID;
              નહીંતર
                {
                  err = xstrtoumax (lo_end + 1, ખાલી, 10, &u, "");
                  જો (err == LONGINT_OK)
                    {
                      hi_આગત = u;
                      જો (hi_આગત != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_લીટીઓ = hi_આગત - lo_આગત + 1;

          જો (err != LONGINT_OK || (lo_આગત <= hi_આગત) == (n_લીટીઓ == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        તોડો;

      સ્થિતિ 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, ખાલી, 10, &argval, "");

          જો (e == LONGINT_OK)
            head_લીટીઓ = MIN (head_લીટીઓ, argval);
          નહીંતર જો (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        તોડો;

      સ્થિતિ 'o':
        જો (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        તોડો;

      સ્થિતિ RANDOM_SOURCE_OPTION:
        જો (random_source && !streq (random_source, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_source = optarg;
        તોડો;

      સ્થિતિ 'r':
        repeat = true;
        તોડો;

      સ્થિતિ 'z':
        eolbyte = '\0';
        તોડો;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      મૂળભૂત:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  જો (echo && આગત_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  જો (આગત_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!આગત_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  જો (head_લીટીઓ == 0)
    {
      n_લીટીઓ = 0;
      લીટી = ખાલી;
    }
  નહીંતર જો (echo)
    {
      આગત_from_argv (operand, n_operands, eolbyte);
      n_લીટીઓ = n_operands;
      લીટી = operand;
    }
  નહીંતર જો (આગત_range)
    {
      IF_LINT (n_લીટીઓ = hi_આગત - lo_આગત + 1); /* Avoid GCC 10 warning.  */
      લીટી = ખાલી;
    }
  નહીંતર
    {
      /* If an input file is specified, re-open it as stdin.  */
      જો (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      જો (repeat || head_લીટીઓ == MIN (IDX_MAX, SIZE_MAX)
          || આગત_size () <= RESERVOIR_MIN_આગત)
        {
          n_લીટીઓ = read_આગત (stdin, eolbyte, &આગત_લીટીઓ);
          લીટી = આગત_લીટીઓ;
        }
      નહીંતર
        {
          use_reservoir_sampling = true;
          n_લીટીઓ = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_લીટીઓ = repeat || head_લીટીઓ < n_લીટીઓ ? head_લીટીઓ : n_લીટીઓ;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_લીટીઓ, n_લીટીઓ)));
  જો (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  જો (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_લીટીઓ = read_આગત_reservoir_sampling (stdin, eolbyte, ahead_લીટીઓ,
                                               randint_source, &reservoir);
      ahead_લીટીઓ = n_લીટીઓ;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  જો (! (head_લીટીઓ == 0 || echo || આગત_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  જો (!repeat)
    permutation = randperm_new (randint_source, ahead_લીટીઓ, n_લીટીઓ);

  જો (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  જો (repeat)
    {
      જો (head_લીટીઓ == 0)
        i = 0;
      નહીંતર
        {
          જો (n_લીટીઓ == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          જો (આગત_range)
            i = write_random_numbers (randint_source, ahead_લીટીઓ,
                                      lo_આગત, hi_આગત, eolbyte);
          નહીંતર
            i = write_random_લીટીઓ (randint_source, ahead_લીટીઓ, લીટી, n_લીટીઓ);
        }
    }
  નહીંતર
    {
      જો (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_લીટીઓ, reservoir, permutation);
      નહીંતર જો (આગત_range)
        i = write_permuted_numbers (ahead_લીટીઓ, lo_આગત,
                                    permutation, eolbyte);
      નહીંતર
        i = write_permuted_લીટીઓ (ahead_લીટીઓ, લીટી, permutation);
    }

  જો (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
