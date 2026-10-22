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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <getopt.h>
#ଅନ୍ତର୍ଭାଵ "system.h"

#ଅନ୍ତର୍ଭାଵ "fadvise.h"
#ଅନ୍ତର୍ଭାଵ "linebuffer.h"
#ଅନ୍ତର୍ଭାଵ "quote.h"
#ଅନ୍ତର୍ଭାଵ "randint.h"
#ଅନ୍ତର୍ଭାଵ "randperm.h"
#ଅନ୍ତର୍ଭାଵ "read-file.h"
#ଅନ୍ତର୍ଭାଵ "stdio--.h"
#ଅନ୍ତର୍ଭାଵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "shuf"

#define AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
ଗଣନା { RESERVOIR_MIN_ନିବେଶ = 8192 * 1024 };


ଶୂନ୍ୟ
usage (ପୂର୍ଣ୍ଣସଂଖ୍ୟା status)
{
  ଯଦି (status != EXIT_SUCCESS)
    emit_try_help ();
  ଅନ୍ୟଥା
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the ନିବେଶ ଧାଡ଼ିଗୁଡ଼ିକ to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an ନିବେଶ ଧାଡ଼ି\n\
"));
      oputs (_("\
  -i, --ନିବେଶ-range=LO-HI   treat each ସଂଖ୍ୟା LO through HI as an ନିବେଶ ଧାଡ଼ି\n\
"));
      oputs (_("\
  -n, --head-ଗଣନା_2=ଗଣନା_3    output at most ଗଣନା_3 ଧାଡ଼ିଗୁଡ଼ିକ\n\
"));
      oputs (_("\
  -o, --output=FILE         ଲିଖ result to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output ଧାଡ଼ିଗୁଡ଼ିକ can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     ଧାଡ଼ି delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ଗଣନା
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

ସ୍ଥିର ସଂରଚନା option ଅଚଳ long_opts[] =
{
  {"echo", no_argument, ଖାଲି, 'e'},
  {"input-range", required_argument, ଖାଲି, 'i'},
  {"head-count", required_argument, ଖାଲି, 'n'},
  {"output", required_argument, ଖାଲି, 'o'},
  {"random-source", required_argument, ଖାଲି, RANDOM_SOURCE_OPTION},
  {"repeat", no_argument, ଖାଲି, 'r'},
  {"zero-terminated", no_argument, ଖାଲି, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ଖାଲି, 0, ଖାଲି, 0},
};

ସ୍ଥିର ଶୂନ୍ୟ
ନିବେଶ_from_argv (ଅକ୍ଷର **operand, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_operands, ଅକ୍ଷର eolbyte)
{
  ଅକ୍ଷର *p;
  size_t size = n_operands;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  ପାଇଁ (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  ପାଇଁ (i = 0; i < n_operands; i++)
    {
      ଅକ୍ଷର *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

ସ୍ଥିର ଅକ୍ଷର *
next_ଧାଡ଼ି (ଅକ୍ଷର *ଧାଡ଼ି, ଅକ୍ଷର eolbyte)
{
  ଅକ୍ଷର *p = rawmemchr (ଧାଡ଼ି, eolbyte);
  ଫେରନ୍ତୁ p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

ସ୍ଥିର off_t
ନିବେଶ_size (ଶୂନ୍ୟ)
{
  off_t file_size;

  ସଂରଚନା stat stat_buf;
  ଯଦି (fstat (STDIN_FILENO, &stat_buf) != 0)
    ଫେରନ୍ତୁ OFF_T_MAX;
  ଯଦି (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  ଅନ୍ୟଥା
    ଫେରନ୍ତୁ OFF_T_MAX;

  off_t ନିବେଶ_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  ଯଦି (ନିବେଶ_offset < 0)
    ଫେରନ୍ତୁ OFF_T_MAX;

  file_size -= ନିବେଶ_offset;

  ଫେରନ୍ତୁ file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

ସ୍ଥିର idx_t
read_ନିବେଶ_reservoir_sampling (FILE *in, ଅକ୍ଷର eolbyte, idx_t k,
                               ସଂରଚନା randint_source *s,
                               ସଂରଚନା linebuffer **out_rsrv)
{
  randint n_ଧାଡ଼ିଗୁଡ଼ିକ = 0;
  idx_t n_alloc_ଧାଡ଼ିଗୁଡ଼ିକ = 0;
  ସଂରଚନା linebuffer *ଧାଡ଼ି = ଖାଲି;
  ସଂରଚନା linebuffer *rsrv = ଖାଲି;

  /* Fill the first K lines, directly into the reservoir.  */
  ପାଇଁ (n_ଧାଡ଼ିଗୁଡ଼ିକ = 0; n_ଧାଡ଼ିଗୁଡ଼ିକ < k; n_ଧାଡ଼ିଗୁଡ଼ିକ++)
    {
      /* Enlarge reservoir if needed.  */
      ଯଦି (n_ଧାଡ଼ିଗୁଡ଼ିକ == n_alloc_ଧାଡ଼ିଗୁଡ଼ିକ)
        {
          idx_t old = n_alloc_ଧାଡ଼ିଗୁଡ଼ିକ;
          rsrv = xpalloc (rsrv, &n_alloc_ଧାଡ଼ିଗୁଡ଼ିକ, 1, k, ଆକାର *rsrv);
          memset (&rsrv[n_ଧାଡ଼ିଗୁଡ଼ିକ], 0, (n_alloc_ଧାଡ଼ିଗୁଡ଼ିକ - old) * ଆକାର *rsrv);
        }

      ଧାଡ଼ି = readlinebuffer_delim (&rsrv[n_ଧାଡ଼ିଗୁଡ଼ିକ], in, eolbyte);
      ଯଦି (!ଧାଡ଼ି)
        ଭାଙ୍ଗ;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  ଯଦି (ଧାଡ଼ି != ଖାଲି)
    {
      ସଂରଚନା linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      କର
        {
          randint j = randint_choose (s, n_ଧାଡ଼ିଗୁଡ଼ିକ + 1);  /* 0 .. n_lines.  */
          ଧାଡ଼ି = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      ଯେତେବେଳେ (readlinebuffer_delim (ଧାଡ଼ି, in, eolbyte) != ଖାଲି && n_ଧାଡ଼ିଗୁଡ଼ିକ++);

      ଯଦି (! n_ଧାଡ଼ିଗୁଡ଼ିକ)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  ଯଦି (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  ଫେରନ୍ତୁ MIN (k, n_ଧାଡ଼ିଗୁଡ଼ିକ);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_permuted_output_reservoir (size_t n_ଧାଡ଼ିଗୁଡ଼ିକ, ସଂରଚନା linebuffer *ଧାଡ଼ିଗୁଡ଼ିକ,
                                 size_t ଅଚଳ *permutation)
{
  ପାଇଁ (size_t i = 0; i < n_ଧାଡ଼ିଗୁଡ଼ିକ; i++)
    {
      ଅଚଳ ସଂରଚନା linebuffer *p = &ଧାଡ଼ିଗୁଡ଼ିକ[permutation[i]];
      ଯଦି (fwrite (p->buffer, ଆକାର (ଅକ୍ଷର), p->length, stdout) != p->length)
        ଫେରନ୍ତୁ -1;
    }

  ଫେରନ୍ତୁ 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

ସ୍ଥିର size_t
read_ନିବେଶ (FILE *in, ଅକ୍ଷର eolbyte, ଅକ୍ଷର ***pline)
{
  ଅକ୍ଷର *p;
  ଅକ୍ଷର *buf = ଖାଲି;
  size_t used;
  ଅକ୍ଷର *lim;
  ଅକ୍ଷର **ଧାଡ଼ି;
  size_t n_ଧାଡ଼ିଗୁଡ଼ିକ;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  ଯଦି (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  ଯଦି (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_ଧାଡ଼ିଗୁଡ଼ିକ = 0;
  ପାଇଁ (p = buf; p < lim; p = next_ଧାଡ଼ି (p, eolbyte))
    n_ଧାଡ଼ିଗୁଡ଼ିକ++;

  *pline = ଧାଡ଼ି = xnmalloc (n_ଧାଡ଼ିଗୁଡ଼ିକ + 1, ଆକାର *ଧାଡ଼ି);

  ଧାଡ଼ି[0] = p = buf;
  ପାଇଁ (size_t i = 1; i <= n_ଧାଡ଼ିଗୁଡ଼ିକ; i++)
    ଧାଡ଼ି[i] = p = next_ଧାଡ଼ି (p, eolbyte);

  ଫେରନ୍ତୁ n_ଧାଡ଼ିଗୁଡ଼ିକ;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_permuted_ଧାଡ଼ିଗୁଡ଼ିକ (size_t n_ଧାଡ଼ିଗୁଡ଼ିକ, ଅକ୍ଷର *ଅଚଳ *ଧାଡ଼ି,
                      size_t ଅଚଳ *permutation)
{
  ପାଇଁ (size_t i = 0; i < n_ଧାଡ଼ିଗୁଡ଼ିକ; i++)
    {
      ଅକ୍ଷର *ଅଚଳ *p = ଧାଡ଼ି + permutation[i];
      size_t len = p[1] - p[0];
      ଯଦି (fwrite (p[0], ଆକାର *p[0], len, stdout) != len)
        ଫେରନ୍ତୁ -1;
    }

  ଫେରନ୍ତୁ 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
ସ୍ଥିର bool
print_ସଂଖ୍ୟା (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଂଖ୍ୟା, ଅକ୍ଷର eolbyte)
{
  ଅକ୍ଷର buf[INT_BUFSIZE_BOUND (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)];
  ଅକ୍ଷର *p = buf + INT_STRLEN_BOUND (unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା);
  *p = eolbyte;
  କର
    *--p = '0' + ସଂଖ୍ୟା % 10;
  ଯେତେବେଳେ ((ସଂଖ୍ୟା /= 10) != 0);
  idx_t len = buf + ଆକାର buf - p;
  ଫେରନ୍ତୁ fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_permuted_numbers (size_t n_ଧାଡ଼ିଗୁଡ଼ିକ, size_t lo_ନିବେଶ,
                        size_t ଅଚଳ *permutation, ଅକ୍ଷର eolbyte)
{
  ପାଇଁ (size_t i = 0; i < n_ଧାଡ଼ିଗୁଡ଼ିକ; i++)
    {
      unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = lo_ନିବେଶ + permutation[i];
      ଯଦି (! print_ସଂଖ୍ୟା (n, eolbyte))
        ଫେରନ୍ତୁ -1;
    }

  ଫେରନ୍ତୁ 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_random_numbers (ସଂରଚନା randint_source *s, size_t ଗଣନା_2,
                      size_t lo_ନିବେଶ, size_t hi_ନିବେଶ, ଅକ୍ଷର eolbyte)
{
  ଅଚଳ randint range = hi_ନିବେଶ - lo_ନିବେଶ + 1;

  ପାଇଁ (size_t i = 0; i < ଗଣନା_2; i++)
    {
      unsigned long ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = lo_ନିବେଶ + randint_choose (s, range);
      ଯଦି (! print_ସଂଖ୍ୟା (j, eolbyte))
        ଫେରନ୍ତୁ -1;
    }

  ଫେରନ୍ତୁ 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_random_ଧାଡ଼ିଗୁଡ଼ିକ (ସଂରଚନା randint_source *s, size_t ଗଣନା_2,
                    ଅକ୍ଷର *ଅଚଳ *ଧାଡ଼ିଗୁଡ଼ିକ, size_t n_ଧାଡ଼ିଗୁଡ଼ିକ)
{
  ପାଇଁ (size_t i = 0; i < ଗଣନା_2; i++)
    {
      ଅଚଳ randint j = randint_choose (s, n_ଧାଡ଼ିଗୁଡ଼ିକ);
      ଅକ୍ଷର *ଅଚଳ *p = ଧାଡ଼ିଗୁଡ଼ିକ + j;
      size_t len = p[1] - p[0];
      ଯଦି (fwrite (p[0], ଆକାର *p[0], len, stdout) != len)
        ଫେରନ୍ତୁ -1;
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  bool echo = false;
  bool ନିବେଶ_range = false;
  size_t lo_ନିବେଶ = SIZE_MAX;
  size_t hi_ନିବେଶ = 0;
  idx_t head_ଧାଡ଼ିଗୁଡ଼ିକ = MIN (IDX_MAX, SIZE_MAX);
  ଅକ୍ଷର ଅଚଳ *outfile = ଖାଲି;
  ଅକ୍ଷର *random_source = ଖାଲି;
  ଅକ୍ଷର eolbyte = '\n';
  ଅକ୍ଷର **ନିବେଶ_ଧାଡ଼ିଗୁଡ଼ିକ = ଖାଲି;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  ପୂର୍ଣ୍ଣସଂଖ୍ୟା optc;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା n_operands;
  ଅକ୍ଷର **operand;
  size_t n_ଧାଡ଼ିଗୁଡ଼ିକ;
  ଅକ୍ଷର **ଧାଡ଼ି = ଖାଲି;
  ସଂରଚନା linebuffer *reservoir = ଖାଲି;
  ସଂରଚନା randint_source *randint_source;
  size_t *permutation = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ଯେତେବେଳେ ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, ଖାଲି))
         != -1)
    ବାଛ (optc)
      {
      ସ୍ଥିତି 'e':
        echo = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'i':
        {
          ଯଦି (ନିବେଶ_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          ନିବେଶ_range = true;

          uintmax_t u;
          ଅକ୍ଷର *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, ଖାଲି);
          ଯଦି (err == LONGINT_OK)
            {
              lo_ନିବେଶ = u;
              ଯଦି (lo_ନିବେଶ != u)
                err = LONGINT_OVERFLOW;
              ଅନ୍ୟଥା ଯଦି (*lo_end != '-')
                err = LONGINT_INVALID;
              ଅନ୍ୟଥା
                {
                  err = xstrtoumax (lo_end + 1, ଖାଲି, 10, &u, "");
                  ଯଦି (err == LONGINT_OK)
                    {
                      hi_ନିବେଶ = u;
                      ଯଦି (hi_ନିବେଶ != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_ଧାଡ଼ିଗୁଡ଼ିକ = hi_ନିବେଶ - lo_ନିବେଶ + 1;

          ଯଦି (err != LONGINT_OK || (lo_ନିବେଶ <= hi_ନିବେଶ) == (n_ଧାଡ଼ିଗୁଡ଼ିକ == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, ଖାଲି, 10, &argval, "");

          ଯଦି (e == LONGINT_OK)
            head_ଧାଡ଼ିଗୁଡ଼ିକ = MIN (head_ଧାଡ଼ିଗୁଡ଼ିକ, argval);
          ଅନ୍ୟଥା ଯଦି (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'o':
        ଯଦି (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି RANDOM_SOURCE_OPTION:
        ଯଦି (random_source && !streq (random_source, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_source = optarg;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'r':
        repeat = true;
        ଭାଙ୍ଗ;

      ସ୍ଥିତି 'z':
        eolbyte = '\0';
        ଭାଙ୍ଗ;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  ଯଦି (echo && ନିବେଶ_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  ଯଦି (ନିବେଶ_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!ନିବେଶ_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  ଯଦି (head_ଧାଡ଼ିଗୁଡ଼ିକ == 0)
    {
      n_ଧାଡ଼ିଗୁଡ଼ିକ = 0;
      ଧାଡ଼ି = ଖାଲି;
    }
  ଅନ୍ୟଥା ଯଦି (echo)
    {
      ନିବେଶ_from_argv (operand, n_operands, eolbyte);
      n_ଧାଡ଼ିଗୁଡ଼ିକ = n_operands;
      ଧାଡ଼ି = operand;
    }
  ଅନ୍ୟଥା ଯଦି (ନିବେଶ_range)
    {
      IF_LINT (n_ଧାଡ଼ିଗୁଡ଼ିକ = hi_ନିବେଶ - lo_ନିବେଶ + 1); /* Avoid GCC 10 warning.  */
      ଧାଡ଼ି = ଖାଲି;
    }
  ଅନ୍ୟଥା
    {
      /* If an input file is specified, re-open it as stdin.  */
      ଯଦି (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      ଯଦି (repeat || head_ଧାଡ଼ିଗୁଡ଼ିକ == MIN (IDX_MAX, SIZE_MAX)
          || ନିବେଶ_size () <= RESERVOIR_MIN_ନିବେଶ)
        {
          n_ଧାଡ଼ିଗୁଡ଼ିକ = read_ନିବେଶ (stdin, eolbyte, &ନିବେଶ_ଧାଡ଼ିଗୁଡ଼ିକ);
          ଧାଡ଼ି = ନିବେଶ_ଧାଡ଼ିଗୁଡ଼ିକ;
        }
      ଅନ୍ୟଥା
        {
          use_reservoir_sampling = true;
          n_ଧାଡ଼ିଗୁଡ଼ିକ = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_ଧାଡ଼ିଗୁଡ଼ିକ = repeat || head_ଧାଡ଼ିଗୁଡ଼ିକ < n_ଧାଡ଼ିଗୁଡ଼ିକ ? head_ଧାଡ଼ିଗୁଡ଼ିକ : n_ଧାଡ଼ିଗୁଡ଼ିକ;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_ଧାଡ଼ିଗୁଡ଼ିକ, n_ଧାଡ଼ିଗୁଡ଼ିକ)));
  ଯଦି (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  ଯଦି (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_ଧାଡ଼ିଗୁଡ଼ିକ = read_ନିବେଶ_reservoir_sampling (stdin, eolbyte, ahead_ଧାଡ଼ିଗୁଡ଼ିକ,
                                               randint_source, &reservoir);
      ahead_ଧାଡ଼ିଗୁଡ଼ିକ = n_ଧାଡ଼ିଗୁଡ଼ିକ;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  ଯଦି (! (head_ଧାଡ଼ିଗୁଡ଼ିକ == 0 || echo || ନିବେଶ_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  ଯଦି (!repeat)
    permutation = randperm_new (randint_source, ahead_ଧାଡ଼ିଗୁଡ଼ିକ, n_ଧାଡ଼ିଗୁଡ଼ିକ);

  ଯଦି (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  ଯଦି (repeat)
    {
      ଯଦି (head_ଧାଡ଼ିଗୁଡ଼ିକ == 0)
        i = 0;
      ଅନ୍ୟଥା
        {
          ଯଦି (n_ଧାଡ଼ିଗୁଡ଼ିକ == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          ଯଦି (ନିବେଶ_range)
            i = write_random_numbers (randint_source, ahead_ଧାଡ଼ିଗୁଡ଼ିକ,
                                      lo_ନିବେଶ, hi_ନିବେଶ, eolbyte);
          ଅନ୍ୟଥା
            i = write_random_ଧାଡ଼ିଗୁଡ଼ିକ (randint_source, ahead_ଧାଡ଼ିଗୁଡ଼ିକ, ଧାଡ଼ି, n_ଧାଡ଼ିଗୁଡ଼ିକ);
        }
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_ଧାଡ଼ିଗୁଡ଼ିକ, reservoir, permutation);
      ଅନ୍ୟଥା ଯଦି (ନିବେଶ_range)
        i = write_permuted_numbers (ahead_ଧାଡ଼ିଗୁଡ଼ିକ, lo_ନିବେଶ,
                                    permutation, eolbyte);
      ଅନ୍ୟଥା
        i = write_permuted_ଧାଡ଼ିଗୁଡ଼ିକ (ahead_ଧାଡ଼ିଗୁଡ଼ିକ, ଧାଡ଼ି, permutation);
    }

  ଯଦି (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
