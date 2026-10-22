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

#அந்தர்பாவ <config.h>

#அந்தர்பாவ <sys/types.h>
#அந்தர்பாவ <getopt.h>
#அந்தர்பாவ "system.h"

#அந்தர்பாவ "fadvise.h"
#அந்தர்பாவ "linebuffer.h"
#அந்தர்பாவ "quote.h"
#அந்தர்பாவ "randint.h"
#அந்தர்பாவ "randperm.h"
#அந்தர்பாவ "read-file.h"
#அந்தர்பாவ "stdio--.h"
#அந்தர்பாவ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "shuf"

#define AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
எண்ணிக்கை { RESERVOIR_MIN_உள்ளீடு = 8192 * 1024 };


வெறுமை
usage (முழு status)
{
  எனில் (status != EXIT_SUCCESS)
    emit_try_help ();
  இல்லையெனில்
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the உள்ளீடு வரிகள் to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an உள்ளீடு வரி\n\
"));
      oputs (_("\
  -i, --உள்ளீடு-range=LO-HI   treat each எண் LO through HI as an உள்ளீடு வரி\n\
"));
      oputs (_("\
  -n, --head-மொத்தம்=மொத்தம்_2    output at most மொத்தம்_2 வரிகள்\n\
"));
      oputs (_("\
  -o, --output=FILE         லிக result to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output வரிகள் can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     வரி delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
எண்ணிக்கை
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

நிலையான கட்டமைப்பு option மாறிலி long_opts[] =
{
  {"echo", no_argument, வெற்று, 'e'},
  {"input-range", required_argument, வெற்று, 'i'},
  {"head-count", required_argument, வெற்று, 'n'},
  {"output", required_argument, வெற்று, 'o'},
  {"random-source", required_argument, வெற்று, RANDOM_SOURCE_OPTION},
  {"repeat", no_argument, வெற்று, 'r'},
  {"zero-terminated", no_argument, வெற்று, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {வெற்று, 0, வெற்று, 0},
};

நிலையான வெறுமை
உள்ளீடு_from_argv (எழுத்து **operand, முழு n_operands, எழுத்து eolbyte)
{
  எழுத்து *p;
  size_t size = n_operands;
  முழு i;

  க்கு (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  க்கு (i = 0; i < n_operands; i++)
    {
      எழுத்து *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

நிலையான எழுத்து *
next_வரி (எழுத்து *வரி, எழுத்து eolbyte)
{
  எழுத்து *p = rawmemchr (வரி, eolbyte);
  திரும்பு p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

நிலையான off_t
உள்ளீடு_size (வெறுமை)
{
  off_t file_size;

  கட்டமைப்பு stat stat_buf;
  எனில் (fstat (STDIN_FILENO, &stat_buf) != 0)
    திரும்பு OFF_T_MAX;
  எனில் (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  இல்லையெனில்
    திரும்பு OFF_T_MAX;

  off_t உள்ளீடு_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  எனில் (உள்ளீடு_offset < 0)
    திரும்பு OFF_T_MAX;

  file_size -= உள்ளீடு_offset;

  திரும்பு file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

நிலையான idx_t
read_உள்ளீடு_reservoir_sampling (FILE *in, எழுத்து eolbyte, idx_t k,
                               கட்டமைப்பு randint_source *s,
                               கட்டமைப்பு linebuffer **out_rsrv)
{
  randint n_வரிகள் = 0;
  idx_t n_alloc_வரிகள் = 0;
  கட்டமைப்பு linebuffer *வரி = வெற்று;
  கட்டமைப்பு linebuffer *rsrv = வெற்று;

  /* Fill the first K lines, directly into the reservoir.  */
  க்கு (n_வரிகள் = 0; n_வரிகள் < k; n_வரிகள்++)
    {
      /* Enlarge reservoir if needed.  */
      எனில் (n_வரிகள் == n_alloc_வரிகள்)
        {
          idx_t old = n_alloc_வரிகள்;
          rsrv = xpalloc (rsrv, &n_alloc_வரிகள், 1, k, அளவு *rsrv);
          memset (&rsrv[n_வரிகள்], 0, (n_alloc_வரிகள் - old) * அளவு *rsrv);
        }

      வரி = readlinebuffer_delim (&rsrv[n_வரிகள்], in, eolbyte);
      எனில் (!வரி)
        நிறுத்து;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  எனில் (வரி != வெற்று)
    {
      கட்டமைப்பு linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      செய்
        {
          randint j = randint_choose (s, n_வரிகள் + 1);  /* 0 .. n_lines.  */
          வரி = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      போது (readlinebuffer_delim (வரி, in, eolbyte) != வெற்று && n_வரிகள்++);

      எனில் (! n_வரிகள்)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  எனில் (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  திரும்பு MIN (k, n_வரிகள்);
}

நிலையான முழு
write_permuted_output_reservoir (size_t n_வரிகள், கட்டமைப்பு linebuffer *வரிகள்,
                                 size_t மாறிலி *permutation)
{
  க்கு (size_t i = 0; i < n_வரிகள்; i++)
    {
      மாறிலி கட்டமைப்பு linebuffer *p = &வரிகள்[permutation[i]];
      எனில் (fwrite (p->buffer, அளவு (எழுத்து), p->length, stdout) != p->length)
        திரும்பு -1;
    }

  திரும்பு 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

நிலையான size_t
read_உள்ளீடு (FILE *in, எழுத்து eolbyte, எழுத்து ***pline)
{
  எழுத்து *p;
  எழுத்து *buf = வெற்று;
  size_t used;
  எழுத்து *lim;
  எழுத்து **வரி;
  size_t n_வரிகள்;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  எனில் (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  எனில் (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_வரிகள் = 0;
  க்கு (p = buf; p < lim; p = next_வரி (p, eolbyte))
    n_வரிகள்++;

  *pline = வரி = xnmalloc (n_வரிகள் + 1, அளவு *வரி);

  வரி[0] = p = buf;
  க்கு (size_t i = 1; i <= n_வரிகள்; i++)
    வரி[i] = p = next_வரி (p, eolbyte);

  திரும்பு n_வரிகள்;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
நிலையான முழு
write_permuted_வரிகள் (size_t n_வரிகள், எழுத்து *மாறிலி *வரி,
                      size_t மாறிலி *permutation)
{
  க்கு (size_t i = 0; i < n_வரிகள்; i++)
    {
      எழுத்து *மாறிலி *p = வரி + permutation[i];
      size_t len = p[1] - p[0];
      எனில் (fwrite (p[0], அளவு *p[0], len, stdout) != len)
        திரும்பு -1;
    }

  திரும்பு 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
நிலையான bool
print_எண் (unsigned long முழு எண், எழுத்து eolbyte)
{
  எழுத்து buf[INT_BUFSIZE_BOUND (unsigned long முழு)];
  எழுத்து *p = buf + INT_STRLEN_BOUND (unsigned long முழு);
  *p = eolbyte;
  செய்
    *--p = '0' + எண் % 10;
  போது ((எண் /= 10) != 0);
  idx_t len = buf + அளவு buf - p;
  திரும்பு fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
நிலையான முழு
write_permuted_numbers (size_t n_வரிகள், size_t lo_உள்ளீடு,
                        size_t மாறிலி *permutation, எழுத்து eolbyte)
{
  க்கு (size_t i = 0; i < n_வரிகள்; i++)
    {
      unsigned long முழு n = lo_உள்ளீடு + permutation[i];
      எனில் (! print_எண் (n, eolbyte))
        திரும்பு -1;
    }

  திரும்பு 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
நிலையான முழு
write_random_numbers (கட்டமைப்பு randint_source *s, size_t மொத்தம்,
                      size_t lo_உள்ளீடு, size_t hi_உள்ளீடு, எழுத்து eolbyte)
{
  மாறிலி randint range = hi_உள்ளீடு - lo_உள்ளீடு + 1;

  க்கு (size_t i = 0; i < மொத்தம்; i++)
    {
      unsigned long முழு j = lo_உள்ளீடு + randint_choose (s, range);
      எனில் (! print_எண் (j, eolbyte))
        திரும்பு -1;
    }

  திரும்பு 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
நிலையான முழு
write_random_வரிகள் (கட்டமைப்பு randint_source *s, size_t மொத்தம்,
                    எழுத்து *மாறிலி *வரிகள், size_t n_வரிகள்)
{
  க்கு (size_t i = 0; i < மொத்தம்; i++)
    {
      மாறிலி randint j = randint_choose (s, n_வரிகள்);
      எழுத்து *மாறிலி *p = வரிகள் + j;
      size_t len = p[1] - p[0];
      எனில் (fwrite (p[0], அளவு *p[0], len, stdout) != len)
        திரும்பு -1;
    }

  திரும்பு 0;
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  bool echo = false;
  bool உள்ளீடு_range = false;
  size_t lo_உள்ளீடு = SIZE_MAX;
  size_t hi_உள்ளீடு = 0;
  idx_t head_வரிகள் = MIN (IDX_MAX, SIZE_MAX);
  எழுத்து மாறிலி *outfile = வெற்று;
  எழுத்து *random_source = வெற்று;
  எழுத்து eolbyte = '\n';
  எழுத்து **உள்ளீடு_வரிகள் = வெற்று;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  முழு optc;
  முழு n_operands;
  எழுத்து **operand;
  size_t n_வரிகள்;
  எழுத்து **வரி = வெற்று;
  கட்டமைப்பு linebuffer *reservoir = வெற்று;
  கட்டமைப்பு randint_source *randint_source;
  size_t *permutation = வெற்று;
  முழு i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  போது ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, வெற்று))
         != -1)
    தேர்வு (optc)
      {
      நிலை 'e':
        echo = true;
        நிறுத்து;

      நிலை 'i':
        {
          எனில் (உள்ளீடு_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          உள்ளீடு_range = true;

          uintmax_t u;
          எழுத்து *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, வெற்று);
          எனில் (err == LONGINT_OK)
            {
              lo_உள்ளீடு = u;
              எனில் (lo_உள்ளீடு != u)
                err = LONGINT_OVERFLOW;
              இல்லையெனில் எனில் (*lo_end != '-')
                err = LONGINT_INVALID;
              இல்லையெனில்
                {
                  err = xstrtoumax (lo_end + 1, வெற்று, 10, &u, "");
                  எனில் (err == LONGINT_OK)
                    {
                      hi_உள்ளீடு = u;
                      எனில் (hi_உள்ளீடு != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_வரிகள் = hi_உள்ளீடு - lo_உள்ளீடு + 1;

          எனில் (err != LONGINT_OK || (lo_உள்ளீடு <= hi_உள்ளீடு) == (n_வரிகள் == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        நிறுத்து;

      நிலை 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, வெற்று, 10, &argval, "");

          எனில் (e == LONGINT_OK)
            head_வரிகள் = MIN (head_வரிகள், argval);
          இல்லையெனில் எனில் (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        நிறுத்து;

      நிலை 'o':
        எனில் (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        நிறுத்து;

      நிலை RANDOM_SOURCE_OPTION:
        எனில் (random_source && !streq (random_source, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_source = optarg;
        நிறுத்து;

      நிலை 'r':
        repeat = true;
        நிறுத்து;

      நிலை 'z':
        eolbyte = '\0';
        நிறுத்து;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      இயல்பு:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  எனில் (echo && உள்ளீடு_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  எனில் (உள்ளீடு_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!உள்ளீடு_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  எனில் (head_வரிகள் == 0)
    {
      n_வரிகள் = 0;
      வரி = வெற்று;
    }
  இல்லையெனில் எனில் (echo)
    {
      உள்ளீடு_from_argv (operand, n_operands, eolbyte);
      n_வரிகள் = n_operands;
      வரி = operand;
    }
  இல்லையெனில் எனில் (உள்ளீடு_range)
    {
      IF_LINT (n_வரிகள் = hi_உள்ளீடு - lo_உள்ளீடு + 1); /* Avoid GCC 10 warning.  */
      வரி = வெற்று;
    }
  இல்லையெனில்
    {
      /* If an input file is specified, re-open it as stdin.  */
      எனில் (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      எனில் (repeat || head_வரிகள் == MIN (IDX_MAX, SIZE_MAX)
          || உள்ளீடு_size () <= RESERVOIR_MIN_உள்ளீடு)
        {
          n_வரிகள் = read_உள்ளீடு (stdin, eolbyte, &உள்ளீடு_வரிகள்);
          வரி = உள்ளீடு_வரிகள்;
        }
      இல்லையெனில்
        {
          use_reservoir_sampling = true;
          n_வரிகள் = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_வரிகள் = repeat || head_வரிகள் < n_வரிகள் ? head_வரிகள் : n_வரிகள்;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_வரிகள், n_வரிகள்)));
  எனில் (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  எனில் (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_வரிகள் = read_உள்ளீடு_reservoir_sampling (stdin, eolbyte, ahead_வரிகள்,
                                               randint_source, &reservoir);
      ahead_வரிகள் = n_வரிகள்;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  எனில் (! (head_வரிகள் == 0 || echo || உள்ளீடு_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  எனில் (!repeat)
    permutation = randperm_new (randint_source, ahead_வரிகள், n_வரிகள்);

  எனில் (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  எனில் (repeat)
    {
      எனில் (head_வரிகள் == 0)
        i = 0;
      இல்லையெனில்
        {
          எனில் (n_வரிகள் == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          எனில் (உள்ளீடு_range)
            i = write_random_numbers (randint_source, ahead_வரிகள்,
                                      lo_உள்ளீடு, hi_உள்ளீடு, eolbyte);
          இல்லையெனில்
            i = write_random_வரிகள் (randint_source, ahead_வரிகள், வரி, n_வரிகள்);
        }
    }
  இல்லையெனில்
    {
      எனில் (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_வரிகள், reservoir, permutation);
      இல்லையெனில் எனில் (உள்ளீடு_range)
        i = write_permuted_numbers (ahead_வரிகள், lo_உள்ளீடு,
                                    permutation, eolbyte);
      இல்லையெனில்
        i = write_permuted_வரிகள் (ahead_வரிகள், வரி, permutation);
    }

  எனில் (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
