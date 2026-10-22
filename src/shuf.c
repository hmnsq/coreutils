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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <getopt.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "system.h"

#ਅਨ੍ਤਰ੍ਭਾਵ "fadvise.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "linebuffer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "quote.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "randint.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "randperm.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "read-file.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "stdio--.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਪ੍ਰੋਗਰਾਮ_NAME "shuf"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
ਗਿਣਤੀ { RESERVOIR_MIN_ਨਿਵੇਸ਼ = 8192 * 1024 };


ਖਾਲੀ
usage (ਪੂਰਨ ਸਥਿਤੀ)
{
  ਜੇ (ਸਥਿਤੀ != EXIT_SUCCESS)
    emit_try_help ();
  ਨਹੀਂਤਾਂ
    {
      ਲਿਖੋ (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name, ਪ੍ਰੋਗਰਾਮ_name);
      fputs (_("\
Write a random permutation ਦਾ the ਨਿਵੇਸ਼ ਸਤਰਾਂ ਮੰਜ਼ਿਲ standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an ਨਿਵੇਸ਼ ਸਤਰ\n\
"));
      oputs (_("\
  -i, --ਨਿਵੇਸ਼-range=LO-HI   treat each ਸੰਖਿਆ LO through HI as an ਨਿਵੇਸ਼ ਸਤਰ\n\
"));
      oputs (_("\
  -n, --ਸਿਰ-ਗਿਣਤੀ_2=ਗਿਣਤੀ_3    output at most ਗਿਣਤੀ_3 ਸਤਰਾਂ\n\
"));
      oputs (_("\
  -o, --output=FILE         ਲਿਖ ਨਤੀਜਾ ਮੰਜ਼ਿਲ FILE instead ਦਾ standard output\n\
"));
      oputs (_("\
      --random-ਸਰੋਤ_2=FILE  get random ਬਾਈਟਾਂ from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output ਸਤਰਾਂ can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     ਸਤਰ delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ਸੰਸਕਰਣ_OPTION_DESCRIPTION);
      emit_ancillary_info (ਪ੍ਰੋਗਰਾਮ_NAME);
    }

  ਬਾਹਰ_ਨਿਕਲੋ (ਸਥਿਤੀ);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ਗਿਣਤੀ
{
  RANDOM_ਸਰੋਤ_OPTION = CHAR_MAX + 1
};

ਸਥਿਰ ਬਣਤਰ option ਅਚਲ long_opts[] =
{
  {"echo", no_ਦਲੀਲ, ਨਿਰਾ, 'e'},
  {"input-range", required_ਦਲੀਲ, ਨਿਰਾ, 'i'},
  {"head-count", required_ਦਲੀਲ, ਨਿਰਾ, 'n'},
  {"output", required_ਦਲੀਲ, ਨਿਰਾ, 'o'},
  {"random-source", required_ਦਲੀਲ, ਨਿਰਾ, RANDOM_ਸਰੋਤ_OPTION},
  {"repeat", no_ਦਲੀਲ, ਨਿਰਾ, 'r'},
  {"zero-terminated", no_ਦਲੀਲ, ਨਿਰਾ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ਸੰਸਕਰਣ_OPTION_DECL},
  {ਨਿਰਾ, 0, ਨਿਰਾ, 0},
};

ਸਥਿਰ ਖਾਲੀ
ਨਿਵੇਸ਼_from_argv (ਅੱਖਰ **ਸੰਕਾਰਜ_2, ਪੂਰਨ n_operands, ਅੱਖਰ eolbyte)
{
  ਅੱਖਰ *p;
  size_t ਮਾਪ = n_operands;
  ਪੂਰਨ i;

  ਲਈ (i = 0; i < n_operands; i++)
    ਮਾਪ += ਲੜੀ_ਲੰਬਾਈ (ਸੰਕਾਰਜ_2[i]);
  p = xmalloc (ਮਾਪ);

  ਲਈ (i = 0; i < n_operands; i++)
    {
      ਅੱਖਰ *p1 = stpcpy (p, ਸੰਕਾਰਜ_2[i]);
      ਸੰਕਾਰਜ_2[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  ਸੰਕਾਰਜ_2[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

ਸਥਿਰ ਅੱਖਰ *
ਅਗਲਾ_ਸਤਰ (ਅੱਖਰ *ਸਤਰ, ਅੱਖਰ eolbyte)
{
  ਅੱਖਰ *p = rawmemchr (ਸਤਰ, eolbyte);
  ਵਾਪਸ p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

ਸਥਿਰ off_t
ਨਿਵੇਸ਼_ਮਾਪ (ਖਾਲੀ)
{
  off_t file_ਮਾਪ;

  ਬਣਤਰ stat stat_buf;
  ਜੇ (fstat (STDIN_FILENO, &stat_buf) != 0)
    ਵਾਪਸ OFF_T_MAX;
  ਜੇ (usable_st_ਮਾਪ (&stat_buf))
    file_ਮਾਪ = stat_buf.st_ਮਾਪ;
  ਨਹੀਂਤਾਂ
    ਵਾਪਸ OFF_T_MAX;

  off_t ਨਿਵੇਸ਼_ਵਿੱਥ = lseek (STDIN_FILENO, 0, SEEK_CUR);
  ਜੇ (ਨਿਵੇਸ਼_ਵਿੱਥ < 0)
    ਵਾਪਸ OFF_T_MAX;

  file_ਮਾਪ -= ਨਿਵੇਸ਼_ਵਿੱਥ;

  ਵਾਪਸ file_ਮਾਪ;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

ਸਥਿਰ idx_t
read_ਨਿਵੇਸ਼_reservoir_sampling (FILE *ਵਿੱਚ, ਅੱਖਰ eolbyte, idx_t k,
                               ਬਣਤਰ randint_ਸਰੋਤ *s,
                               ਬਣਤਰ linebuffer **ਬਾਹਰਲਾ_rsrv)
{
  randint n_ਸਤਰਾਂ = 0;
  idx_t n_alloc_ਸਤਰਾਂ = 0;
  ਬਣਤਰ linebuffer *ਸਤਰ = ਨਿਰਾ;
  ਬਣਤਰ linebuffer *rsrv = ਨਿਰਾ;

  /* Fill the first K lines, directly into the reservoir.  */
  ਲਈ (n_ਸਤਰਾਂ = 0; n_ਸਤਰਾਂ < k; n_ਸਤਰਾਂ++)
    {
      /* Enlarge reservoir if needed.  */
      ਜੇ (n_ਸਤਰਾਂ == n_alloc_ਸਤਰਾਂ)
        {
          idx_t old = n_alloc_ਸਤਰਾਂ;
          rsrv = xpalloc (rsrv, &n_alloc_ਸਤਰਾਂ, 1, k, ਆਕਾਰ *rsrv);
          memset (&rsrv[n_ਸਤਰਾਂ], 0, (n_alloc_ਸਤਰਾਂ - old) * ਆਕਾਰ *rsrv);
        }

      ਸਤਰ = readlinebuffer_delim (&rsrv[n_ਸਤਰਾਂ], ਵਿੱਚ, eolbyte);
      ਜੇ (!ਸਤਰ)
        ਰੋਕੋ;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  ਜੇ (ਸਤਰ != ਨਿਰਾ)
    {
      ਬਣਤਰ linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      ਕਰੋ
        {
          randint j = randint_ਚੁਣੋ (s, n_ਸਤਰਾਂ + 1);  /* 0 .. n_lines.  */
          ਸਤਰ = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      ਜਦੋਂ (readlinebuffer_delim (ਸਤਰ, ਵਿੱਚ, eolbyte) != ਨਿਰਾ && n_ਸਤਰਾਂ++);

      ਜੇ (! n_ਸਤਰਾਂ)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  ਜੇ (ferror (ਵਿੱਚ))
    error (EXIT_FAILURE, errno, _("read error"));

  *ਬਾਹਰਲਾ_rsrv = rsrv;
  ਵਾਪਸ MIN (k, n_ਸਤਰਾਂ);
}

ਸਥਿਰ ਪੂਰਨ
write_permuted_output_reservoir (size_t n_ਸਤਰਾਂ, ਬਣਤਰ linebuffer *ਸਤਰਾਂ,
                                 size_t ਅਚਲ *permutation)
{
  ਲਈ (size_t i = 0; i < n_ਸਤਰਾਂ; i++)
    {
      ਅਚਲ ਬਣਤਰ linebuffer *p = &ਸਤਰਾਂ[permutation[i]];
      ਜੇ (fwrite (p->ਬਫਰ, ਆਕਾਰ (ਅੱਖਰ), p->ਲੰਮਾਈ, stdout) != p->ਲੰਮਾਈ)
        ਵਾਪਸ -1;
    }

  ਵਾਪਸ 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

ਸਥਿਰ size_t
read_ਨਿਵੇਸ਼ (FILE *ਵਿੱਚ, ਅੱਖਰ eolbyte, ਅੱਖਰ ***pline)
{
  ਅੱਖਰ *p;
  ਅੱਖਰ *buf = ਨਿਰਾ;
  size_t ਵਰਤਿਆ;
  ਅੱਖਰ *lim;
  ਅੱਖਰ **ਸਤਰ;
  size_t n_ਸਤਰਾਂ;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  ਜੇ (!(buf = fread_file (ਵਿੱਚ, 0, &ਵਰਤਿਆ)))
    error (EXIT_FAILURE, errno, _("read error"));

  ਜੇ (ਵਰਤਿਆ && buf[ਵਰਤਿਆ - 1] != eolbyte)
    buf[ਵਰਤਿਆ++] = eolbyte;

  lim = buf + ਵਰਤਿਆ;

  n_ਸਤਰਾਂ = 0;
  ਲਈ (p = buf; p < lim; p = ਅਗਲਾ_ਸਤਰ (p, eolbyte))
    n_ਸਤਰਾਂ++;

  *pline = ਸਤਰ = xnmalloc (n_ਸਤਰਾਂ + 1, ਆਕਾਰ *ਸਤਰ);

  ਸਤਰ[0] = p = buf;
  ਲਈ (size_t i = 1; i <= n_ਸਤਰਾਂ; i++)
    ਸਤਰ[i] = p = ਅਗਲਾ_ਸਤਰ (p, eolbyte);

  ਵਾਪਸ n_ਸਤਰਾਂ;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
ਸਥਿਰ ਪੂਰਨ
write_permuted_ਸਤਰਾਂ (size_t n_ਸਤਰਾਂ, ਅੱਖਰ *ਅਚਲ *ਸਤਰ,
                      size_t ਅਚਲ *permutation)
{
  ਲਈ (size_t i = 0; i < n_ਸਤਰਾਂ; i++)
    {
      ਅੱਖਰ *ਅਚਲ *p = ਸਤਰ + permutation[i];
      size_t len = p[1] - p[0];
      ਜੇ (fwrite (p[0], ਆਕਾਰ *p[0], len, stdout) != len)
        ਵਾਪਸ -1;
    }

  ਵਾਪਸ 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
ਸਥਿਰ bool
print_ਸੰਖਿਆ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ ਸੰਖਿਆ, ਅੱਖਰ eolbyte)
{
  ਅੱਖਰ buf[INT_BUFSIZE_BOUND (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ)];
  ਅੱਖਰ *p = buf + INT_STRLEN_BOUND (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ);
  *p = eolbyte;
  ਕਰੋ
    *--p = '0' + ਸੰਖਿਆ % 10;
  ਜਦੋਂ ((ਸੰਖਿਆ /= 10) != 0);
  idx_t len = buf + ਆਕਾਰ buf - p;
  ਵਾਪਸ fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
ਸਥਿਰ ਪੂਰਨ
write_permuted_numbers (size_t n_ਸਤਰਾਂ, size_t lo_ਨਿਵੇਸ਼,
                        size_t ਅਚਲ *permutation, ਅੱਖਰ eolbyte)
{
  ਲਈ (size_t i = 0; i < n_ਸਤਰਾਂ; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ n = lo_ਨਿਵੇਸ਼ + permutation[i];
      ਜੇ (! print_ਸੰਖਿਆ (n, eolbyte))
        ਵਾਪਸ -1;
    }

  ਵਾਪਸ 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
ਸਥਿਰ ਪੂਰਨ
write_random_numbers (ਬਣਤਰ randint_ਸਰੋਤ *s, size_t ਗਿਣਤੀ_2,
                      size_t lo_ਨਿਵੇਸ਼, size_t hi_ਨਿਵੇਸ਼, ਅੱਖਰ eolbyte)
{
  ਅਚਲ randint range = hi_ਨਿਵੇਸ਼ - lo_ਨਿਵੇਸ਼ + 1;

  ਲਈ (size_t i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਪੂਰਨ j = lo_ਨਿਵੇਸ਼ + randint_ਚੁਣੋ (s, range);
      ਜੇ (! print_ਸੰਖਿਆ (j, eolbyte))
        ਵਾਪਸ -1;
    }

  ਵਾਪਸ 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
ਸਥਿਰ ਪੂਰਨ
write_random_ਸਤਰਾਂ (ਬਣਤਰ randint_ਸਰੋਤ *s, size_t ਗਿਣਤੀ_2,
                    ਅੱਖਰ *ਅਚਲ *ਸਤਰਾਂ, size_t n_ਸਤਰਾਂ)
{
  ਲਈ (size_t i = 0; i < ਗਿਣਤੀ_2; i++)
    {
      ਅਚਲ randint j = randint_ਚੁਣੋ (s, n_ਸਤਰਾਂ);
      ਅੱਖਰ *ਅਚਲ *p = ਸਤਰਾਂ + j;
      size_t len = p[1] - p[0];
      ਜੇ (fwrite (p[0], ਆਕਾਰ *p[0], len, stdout) != len)
        ਵਾਪਸ -1;
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  bool echo = false;
  bool ਨਿਵੇਸ਼_range = false;
  size_t lo_ਨਿਵੇਸ਼ = SIZE_MAX;
  size_t hi_ਨਿਵੇਸ਼ = 0;
  idx_t ਸਿਰ_ਸਤਰਾਂ = MIN (IDX_MAX, SIZE_MAX);
  ਅੱਖਰ ਅਚਲ *outfile = ਨਿਰਾ;
  ਅੱਖਰ *random_ਸਰੋਤ = ਨਿਰਾ;
  ਅੱਖਰ eolbyte = '\n';
  ਅੱਖਰ **ਨਿਵੇਸ਼_ਸਤਰਾਂ = ਨਿਰਾ;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  ਪੂਰਨ optc;
  ਪੂਰਨ n_operands;
  ਅੱਖਰ **ਸੰਕਾਰਜ_2;
  size_t n_ਸਤਰਾਂ;
  ਅੱਖਰ **ਸਤਰ = ਨਿਰਾ;
  ਬਣਤਰ linebuffer *reservoir = ਨਿਰਾ;
  ਬਣਤਰ randint_ਸਰੋਤ *randint_ਸਰੋਤ;
  size_t *permutation = ਨਿਰਾ;
  ਪੂਰਨ i;

  initialize_main (&argc, &argv);
  ਸੈੱਟ_ਪ੍ਰੋਗਰਾਮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ਜਦੋਂ ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, ਨਿਰਾ))
         != -1)
    ਚੋਣ (optc)
      {
      ਹਾਲਤ 'e':
        echo = true;
        ਰੋਕੋ;

      ਹਾਲਤ 'i':
        {
          ਜੇ (ਨਿਵੇਸ਼_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          ਨਿਵੇਸ਼_range = true;

          uintmax_t u;
          ਅੱਖਰ *lo_ਸਿਰਾ;
          strtol_error err = xstrtoumax (optarg, &lo_ਸਿਰਾ, 10, &u, ਨਿਰਾ);
          ਜੇ (err == LONGINT_OK)
            {
              lo_ਨਿਵੇਸ਼ = u;
              ਜੇ (lo_ਨਿਵੇਸ਼ != u)
                err = LONGINT_ਉਛਾਲ;
              ਨਹੀਂਤਾਂ ਜੇ (*lo_ਸਿਰਾ != '-')
                err = LONGINT_INVALID;
              ਨਹੀਂਤਾਂ
                {
                  err = xstrtoumax (lo_ਸਿਰਾ + 1, ਨਿਰਾ, 10, &u, "");
                  ਜੇ (err == LONGINT_OK)
                    {
                      hi_ਨਿਵੇਸ਼ = u;
                      ਜੇ (hi_ਨਿਵੇਸ਼ != u)
                        err = LONGINT_ਉਛਾਲ;
                    }
                }
            }

          n_ਸਤਰਾਂ = hi_ਨਿਵੇਸ਼ - lo_ਨਿਵੇਸ਼ + 1;

          ਜੇ (err != LONGINT_OK || (lo_ਨਿਵੇਸ਼ <= hi_ਨਿਵੇਸ਼) == (n_ਸਤਰਾਂ == 0))
            error (EXIT_FAILURE, err == LONGINT_ਉਛਾਲ ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        ਰੋਕੋ;

      ਹਾਲਤ 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, ਨਿਰਾ, 10, &argval, "");

          ਜੇ (e == LONGINT_OK)
            ਸਿਰ_ਸਤਰਾਂ = MIN (ਸਿਰ_ਸਤਰਾਂ, argval);
          ਨਹੀਂਤਾਂ ਜੇ (e != LONGINT_ਉਛਾਲ)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        ਰੋਕੋ;

      ਹਾਲਤ 'o':
        ਜੇ (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        ਰੋਕੋ;

      ਹਾਲਤ RANDOM_ਸਰੋਤ_OPTION:
        ਜੇ (random_ਸਰੋਤ && !streq (random_ਸਰੋਤ, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_ਸਰੋਤ = optarg;
        ਰੋਕੋ;

      ਹਾਲਤ 'r':
        repeat = true;
        ਰੋਕੋ;

      ਹਾਲਤ 'z':
        eolbyte = '\0';
        ਰੋਕੋ;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_ਸੰਸਕਰਣ_CHAR (ਪ੍ਰੋਗਰਾਮ_NAME, AUTHORS);
      ਮੂਲ:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  ਸੰਕਾਰਜ_2 = argv + optind;

  /* Check invalid usage.  */
  ਜੇ (echo && ਨਿਵੇਸ਼_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  ਜੇ (ਨਿਵੇਸ਼_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (ਸੰਕਾਰਜ_2[!ਨਿਵੇਸ਼_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  ਜੇ (ਸਿਰ_ਸਤਰਾਂ == 0)
    {
      n_ਸਤਰਾਂ = 0;
      ਸਤਰ = ਨਿਰਾ;
    }
  ਨਹੀਂਤਾਂ ਜੇ (echo)
    {
      ਨਿਵੇਸ਼_from_argv (ਸੰਕਾਰਜ_2, n_operands, eolbyte);
      n_ਸਤਰਾਂ = n_operands;
      ਸਤਰ = ਸੰਕਾਰਜ_2;
    }
  ਨਹੀਂਤਾਂ ਜੇ (ਨਿਵੇਸ਼_range)
    {
      IF_LINT (n_ਸਤਰਾਂ = hi_ਨਿਵੇਸ਼ - lo_ਨਿਵੇਸ਼ + 1); /* Avoid GCC 10 warning.  */
      ਸਤਰ = ਨਿਰਾ;
    }
  ਨਹੀਂਤਾਂ
    {
      /* If an input file is specified, re-open it as stdin.  */
      ਜੇ (n_operands == 1
          && ! (streq (ਸੰਕਾਰਜ_2[0], "-")
                || freopen (ਸੰਕਾਰਜ_2[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (ਸੰਕਾਰਜ_2[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      ਜੇ (repeat || ਸਿਰ_ਸਤਰਾਂ == MIN (IDX_MAX, SIZE_MAX)
          || ਨਿਵੇਸ਼_ਮਾਪ () <= RESERVOIR_MIN_ਨਿਵੇਸ਼)
        {
          n_ਸਤਰਾਂ = read_ਨਿਵੇਸ਼ (stdin, eolbyte, &ਨਿਵੇਸ਼_ਸਤਰਾਂ);
          ਸਤਰ = ਨਿਵੇਸ਼_ਸਤਰਾਂ;
        }
      ਨਹੀਂਤਾਂ
        {
          use_reservoir_sampling = true;
          n_ਸਤਰਾਂ = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_ਸਤਰਾਂ = repeat || ਸਿਰ_ਸਤਰਾਂ < n_ਸਤਰਾਂ ? ਸਿਰ_ਸਤਰਾਂ : n_ਸਤਰਾਂ;

  randint_ਸਰੋਤ = randint_all_new (random_ਸਰੋਤ,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_ਸਤਰਾਂ, n_ਸਤਰਾਂ)));
  ਜੇ (! randint_ਸਰੋਤ)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_ਸਰੋਤ ? random_ਸਰੋਤ : "getrandom"));

  ਜੇ (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_ਸਤਰਾਂ = read_ਨਿਵੇਸ਼_reservoir_sampling (stdin, eolbyte, ahead_ਸਤਰਾਂ,
                                               randint_ਸਰੋਤ, &reservoir);
      ahead_ਸਤਰਾਂ = n_ਸਤਰਾਂ;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  ਜੇ (! (ਸਿਰ_ਸਤਰਾਂ == 0 || echo || ਨਿਵੇਸ਼_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  ਜੇ (!repeat)
    permutation = randperm_new (randint_ਸਰੋਤ, ahead_ਸਤਰਾਂ, n_ਸਤਰਾਂ);

  ਜੇ (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  ਜੇ (repeat)
    {
      ਜੇ (ਸਿਰ_ਸਤਰਾਂ == 0)
        i = 0;
      ਨਹੀਂਤਾਂ
        {
          ਜੇ (n_ਸਤਰਾਂ == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          ਜੇ (ਨਿਵੇਸ਼_range)
            i = write_random_numbers (randint_ਸਰੋਤ, ahead_ਸਤਰਾਂ,
                                      lo_ਨਿਵੇਸ਼, hi_ਨਿਵੇਸ਼, eolbyte);
          ਨਹੀਂਤਾਂ
            i = write_random_ਸਤਰਾਂ (randint_ਸਰੋਤ, ahead_ਸਤਰਾਂ, ਸਤਰ, n_ਸਤਰਾਂ);
        }
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_ਸਤਰਾਂ, reservoir, permutation);
      ਨਹੀਂਤਾਂ ਜੇ (ਨਿਵੇਸ਼_range)
        i = write_permuted_numbers (ahead_ਸਤਰਾਂ, lo_ਨਿਵੇਸ਼,
                                    permutation, eolbyte);
      ਨਹੀਂਤਾਂ
        i = write_permuted_ਸਤਰਾਂ (ahead_ਸਤਰਾਂ, ਸਤਰ, permutation);
    }

  ਜੇ (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_ਸਰੋਤ));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
