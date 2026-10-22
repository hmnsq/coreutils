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

#ಅನ್ತರ್ಭಾವ <config.h>

#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ "system.h"

#ಅನ್ತರ್ಭಾವ "fadvise.h"
#ಅನ್ತರ್ಭಾವ "linebuffer.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "randint.h"
#ಅನ್ತರ್ಭಾವ "randperm.h"
#ಅನ್ತರ್ಭಾವ "read-file.h"
#ಅನ್ತರ್ಭಾವ "stdio--.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#define PROGRAM_NAME "shuf"

#define AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
ಗಣನಾ { RESERVOIR_MIN_ಒಳಹರಿವು = 8192 * 1024 };


ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಙ್ಕ status)
{
  ಯದಿ (status != EXIT_SUCCESS)
    emit_try_help ();
  ಅನ್ಯಥಾ
    {
      printf (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              program_name, program_name, program_name);
      fputs (_("\
Write a random permutation of the ಒಳಹರಿವು ಸಾಲುಗಳು to standard output.\n\
"), stdout);

      emit_stdin_note ();
      emit_mandatory_arg_note ();

      oputs (_("\
  -e, --echo                treat each ARG as an ಒಳಹರಿವು ಸಾಲು\n\
"));
      oputs (_("\
  -i, --ಒಳಹರಿವು-range=LO-HI   treat each ಸಂಖ್ಯೆ LO through HI as an ಒಳಹರಿವು ಸಾಲು\n\
"));
      oputs (_("\
  -n, --head-ಎಣಿಕೆ=ಎಣಿಕೆ_2    output at most ಎಣಿಕೆ_2 ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -o, --output=FILE         ಲಿಖ result to FILE instead of standard output\n\
"));
      oputs (_("\
      --random-source=FILE  get random bytes from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output ಸಾಲುಗಳು can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (VERSION_OPTION_DESCRIPTION);
      emit_ancillary_info (PROGRAM_NAME);
    }

  exit (status);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಗಣನಾ
{
  RANDOM_SOURCE_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ಸಂರಚನಾ option ಧ್ರುವ long_opts[] =
{
  {"echo", no_argument, ರಿಕ್ತ, 'e'},
  {"input-range", required_argument, ರಿಕ್ತ, 'i'},
  {"head-count", required_argument, ರಿಕ್ತ, 'n'},
  {"output", required_argument, ರಿಕ್ತ, 'o'},
  {"random-source", required_argument, ರಿಕ್ತ, RANDOM_SOURCE_OPTION},
  {"repeat", no_argument, ರಿಕ್ತ, 'r'},
  {"zero-terminated", no_argument, ರಿಕ್ತ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_VERSION_OPTION_DECL},
  {ರಿಕ್ತ, 0, ರಿಕ್ತ, 0},
};

ಸ್ಥಿರ ಶೂನ್ಯ
ಒಳಹರಿವು_from_argv (ವರ್ಣ **operand, ಪೂರ್ಣಾಙ್ಕ n_operands, ವರ್ಣ eolbyte)
{
  ವರ್ಣ *p;
  size_t size = n_operands;
  ಪೂರ್ಣಾಙ್ಕ i;

  ಪ್ರತಿ (i = 0; i < n_operands; i++)
    size += strlen (operand[i]);
  p = xmalloc (size);

  ಪ್ರತಿ (i = 0; i < n_operands; i++)
    {
      ವರ್ಣ *p1 = stpcpy (p, operand[i]);
      operand[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  operand[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

ಸ್ಥಿರ ವರ್ಣ *
next_ಸಾಲು (ವರ್ಣ *ಸಾಲು, ವರ್ಣ eolbyte)
{
  ವರ್ಣ *p = rawmemchr (ಸಾಲು, eolbyte);
  ಪ್ರತ್ಯಾಗಮ p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

ಸ್ಥಿರ off_t
ಒಳಹರಿವು_size (ಶೂನ್ಯ)
{
  off_t file_size;

  ಸಂರಚನಾ stat stat_buf;
  ಯದಿ (fstat (STDIN_FILENO, &stat_buf) != 0)
    ಪ್ರತ್ಯಾಗಮ OFF_T_MAX;
  ಯದಿ (usable_st_size (&stat_buf))
    file_size = stat_buf.st_size;
  ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ OFF_T_MAX;

  off_t ಒಳಹರಿವು_offset = lseek (STDIN_FILENO, 0, SEEK_CUR);
  ಯದಿ (ಒಳಹರಿವು_offset < 0)
    ಪ್ರತ್ಯಾಗಮ OFF_T_MAX;

  file_size -= ಒಳಹರಿವು_offset;

  ಪ್ರತ್ಯಾಗಮ file_size;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

ಸ್ಥಿರ idx_t
read_ಒಳಹರಿವು_reservoir_sampling (FILE *in, ವರ್ಣ eolbyte, idx_t k,
                               ಸಂರಚನಾ randint_source *s,
                               ಸಂರಚನಾ linebuffer **out_rsrv)
{
  randint n_ಸಾಲುಗಳು = 0;
  idx_t n_alloc_ಸಾಲುಗಳು = 0;
  ಸಂರಚನಾ linebuffer *ಸಾಲು = ರಿಕ್ತ;
  ಸಂರಚನಾ linebuffer *rsrv = ರಿಕ್ತ;

  /* Fill the first K lines, directly into the reservoir.  */
  ಪ್ರತಿ (n_ಸಾಲುಗಳು = 0; n_ಸಾಲುಗಳು < k; n_ಸಾಲುಗಳು++)
    {
      /* Enlarge reservoir if needed.  */
      ಯದಿ (n_ಸಾಲುಗಳು == n_alloc_ಸಾಲುಗಳು)
        {
          idx_t old = n_alloc_ಸಾಲುಗಳು;
          rsrv = xpalloc (rsrv, &n_alloc_ಸಾಲುಗಳು, 1, k, ಪರಿಮಾಣ *rsrv);
          memset (&rsrv[n_ಸಾಲುಗಳು], 0, (n_alloc_ಸಾಲುಗಳು - old) * ಪರಿಮಾಣ *rsrv);
        }

      ಸಾಲು = readlinebuffer_delim (&rsrv[n_ಸಾಲುಗಳು], in, eolbyte);
      ಯದಿ (!ಸಾಲು)
        ಭಙ್ಗ;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  ಯದಿ (ಸಾಲು != ರಿಕ್ತ)
    {
      ಸಂರಚನಾ linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      ಕುರು
        {
          randint j = randint_choose (s, n_ಸಾಲುಗಳು + 1);  /* 0 .. n_lines.  */
          ಸಾಲು = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      ಯಾವತ್ (readlinebuffer_delim (ಸಾಲು, in, eolbyte) != ರಿಕ್ತ && n_ಸಾಲುಗಳು++);

      ಯದಿ (! n_ಸಾಲುಗಳು)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  ಯದಿ (ferror (in))
    error (EXIT_FAILURE, errno, _("read error"));

  *out_rsrv = rsrv;
  ಪ್ರತ್ಯಾಗಮ MIN (k, n_ಸಾಲುಗಳು);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_permuted_output_reservoir (size_t n_ಸಾಲುಗಳು, ಸಂರಚನಾ linebuffer *ಸಾಲುಗಳು,
                                 size_t ಧ್ರುವ *permutation)
{
  ಪ್ರತಿ (size_t i = 0; i < n_ಸಾಲುಗಳು; i++)
    {
      ಧ್ರುವ ಸಂರಚನಾ linebuffer *p = &ಸಾಲುಗಳು[permutation[i]];
      ಯದಿ (fwrite (p->buffer, ಪರಿಮಾಣ (ವರ್ಣ), p->length, stdout) != p->length)
        ಪ್ರತ್ಯಾಗಮ -1;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

ಸ್ಥಿರ size_t
read_ಒಳಹರಿವು (FILE *in, ವರ್ಣ eolbyte, ವರ್ಣ ***pline)
{
  ವರ್ಣ *p;
  ವರ್ಣ *buf = ರಿಕ್ತ;
  size_t used;
  ವರ್ಣ *lim;
  ವರ್ಣ **ಸಾಲು;
  size_t n_ಸಾಲುಗಳು;

  /* TODO: We should limit the amount of data read here,
     to less than RESERVOIR_MIN_INPUT.  I.e., adjust fread_file() to support
     taking a byte limit.  We'd then need to ensure we handle a line spanning
     this boundary.  With that in place we could set use_reservoir_sampling
     when used==RESERVOIR_MIN_INPUT, and have read_input_reservoir_sampling()
     call a wrapper function to populate a linebuffer from the internal pline
     or if none left, stdin.  Doing that would give better performance by
     avoiding the reservoir CPU overhead when reading < RESERVOIR_MIN_INPUT
     from a pipe, and allow us to dispense with the input_size() function.  */
  ಯದಿ (!(buf = fread_file (in, 0, &used)))
    error (EXIT_FAILURE, errno, _("read error"));

  ಯದಿ (used && buf[used - 1] != eolbyte)
    buf[used++] = eolbyte;

  lim = buf + used;

  n_ಸಾಲುಗಳು = 0;
  ಪ್ರತಿ (p = buf; p < lim; p = next_ಸಾಲು (p, eolbyte))
    n_ಸಾಲುಗಳು++;

  *pline = ಸಾಲು = xnmalloc (n_ಸಾಲುಗಳು + 1, ಪರಿಮಾಣ *ಸಾಲು);

  ಸಾಲು[0] = p = buf;
  ಪ್ರತಿ (size_t i = 1; i <= n_ಸಾಲುಗಳು; i++)
    ಸಾಲು[i] = p = next_ಸಾಲು (p, eolbyte);

  ಪ್ರತ್ಯಾಗಮ n_ಸಾಲುಗಳು;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_permuted_ಸಾಲುಗಳು (size_t n_ಸಾಲುಗಳು, ವರ್ಣ *ಧ್ರುವ *ಸಾಲು,
                      size_t ಧ್ರುವ *permutation)
{
  ಪ್ರತಿ (size_t i = 0; i < n_ಸಾಲುಗಳು; i++)
    {
      ವರ್ಣ *ಧ್ರುವ *p = ಸಾಲು + permutation[i];
      size_t len = p[1] - p[0];
      ಯದಿ (fwrite (p[0], ಪರಿಮಾಣ *p[0], len, stdout) != len)
        ಪ್ರತ್ಯಾಗಮ -1;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
ಸ್ಥಿರ bool
print_ಸಂಖ್ಯೆ (unsigned long ಪೂರ್ಣಾಙ್ಕ ಸಂಖ್ಯೆ, ವರ್ಣ eolbyte)
{
  ವರ್ಣ buf[INT_BUFSIZE_BOUND (unsigned long ಪೂರ್ಣಾಙ್ಕ)];
  ವರ್ಣ *p = buf + INT_STRLEN_BOUND (unsigned long ಪೂರ್ಣಾಙ್ಕ);
  *p = eolbyte;
  ಕುರು
    *--p = '0' + ಸಂಖ್ಯೆ % 10;
  ಯಾವತ್ ((ಸಂಖ್ಯೆ /= 10) != 0);
  idx_t len = buf + ಪರಿಮಾಣ buf - p;
  ಪ್ರತ್ಯಾಗಮ fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_permuted_numbers (size_t n_ಸಾಲುಗಳು, size_t lo_ಒಳಹರಿವು,
                        size_t ಧ್ರುವ *permutation, ವರ್ಣ eolbyte)
{
  ಪ್ರತಿ (size_t i = 0; i < n_ಸಾಲುಗಳು; i++)
    {
      unsigned long ಪೂರ್ಣಾಙ್ಕ n = lo_ಒಳಹರಿವು + permutation[i];
      ಯದಿ (! print_ಸಂಖ್ಯೆ (n, eolbyte))
        ಪ್ರತ್ಯಾಗಮ -1;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_random_numbers (ಸಂರಚನಾ randint_source *s, size_t ಎಣಿಕೆ,
                      size_t lo_ಒಳಹರಿವು, size_t hi_ಒಳಹರಿವು, ವರ್ಣ eolbyte)
{
  ಧ್ರುವ randint range = hi_ಒಳಹರಿವು - lo_ಒಳಹರಿವು + 1;

  ಪ್ರತಿ (size_t i = 0; i < ಎಣಿಕೆ; i++)
    {
      unsigned long ಪೂರ್ಣಾಙ್ಕ j = lo_ಒಳಹರಿವು + randint_choose (s, range);
      ಯದಿ (! print_ಸಂಖ್ಯೆ (j, eolbyte))
        ಪ್ರತ್ಯಾಗಮ -1;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_random_ಸಾಲುಗಳು (ಸಂರಚನಾ randint_source *s, size_t ಎಣಿಕೆ,
                    ವರ್ಣ *ಧ್ರುವ *ಸಾಲುಗಳು, size_t n_ಸಾಲುಗಳು)
{
  ಪ್ರತಿ (size_t i = 0; i < ಎಣಿಕೆ; i++)
    {
      ಧ್ರುವ randint j = randint_choose (s, n_ಸಾಲುಗಳು);
      ವರ್ಣ *ಧ್ರುವ *p = ಸಾಲುಗಳು + j;
      size_t len = p[1] - p[0];
      ಯದಿ (fwrite (p[0], ಪರಿಮಾಣ *p[0], len, stdout) != len)
        ಪ್ರತ್ಯಾಗಮ -1;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  bool echo = false;
  bool ಒಳಹರಿವು_range = false;
  size_t lo_ಒಳಹರಿವು = SIZE_MAX;
  size_t hi_ಒಳಹರಿವು = 0;
  idx_t head_ಸಾಲುಗಳು = MIN (IDX_MAX, SIZE_MAX);
  ವರ್ಣ ಧ್ರುವ *outfile = ರಿಕ್ತ;
  ವರ್ಣ *random_source = ರಿಕ್ತ;
  ವರ್ಣ eolbyte = '\n';
  ವರ್ಣ **ಒಳಹರಿವು_ಸಾಲುಗಳು = ರಿಕ್ತ;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  ಪೂರ್ಣಾಙ್ಕ optc;
  ಪೂರ್ಣಾಙ್ಕ n_operands;
  ವರ್ಣ **operand;
  size_t n_ಸಾಲುಗಳು;
  ವರ್ಣ **ಸಾಲು = ರಿಕ್ತ;
  ಸಂರಚನಾ linebuffer *reservoir = ರಿಕ್ತ;
  ಸಂರಚನಾ randint_source *randint_source;
  size_t *permutation = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ i;

  initialize_main (&argc, &argv);
  set_program_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಯಾವತ್ ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, ರಿಕ್ತ))
         != -1)
    ವಿಕಲ್ಪ (optc)
      {
      ಪಕ್ಷ 'e':
        echo = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'i':
        {
          ಯದಿ (ಒಳಹರಿವು_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          ಒಳಹರಿವು_range = true;

          uintmax_t u;
          ವರ್ಣ *lo_end;
          strtol_error err = xstrtoumax (optarg, &lo_end, 10, &u, ರಿಕ್ತ);
          ಯದಿ (err == LONGINT_OK)
            {
              lo_ಒಳಹರಿವು = u;
              ಯದಿ (lo_ಒಳಹರಿವು != u)
                err = LONGINT_OVERFLOW;
              ಅನ್ಯಥಾ ಯದಿ (*lo_end != '-')
                err = LONGINT_INVALID;
              ಅನ್ಯಥಾ
                {
                  err = xstrtoumax (lo_end + 1, ರಿಕ್ತ, 10, &u, "");
                  ಯದಿ (err == LONGINT_OK)
                    {
                      hi_ಒಳಹರಿವು = u;
                      ಯದಿ (hi_ಒಳಹರಿವು != u)
                        err = LONGINT_OVERFLOW;
                    }
                }
            }

          n_ಸಾಲುಗಳು = hi_ಒಳಹರಿವು - lo_ಒಳಹರಿವು + 1;

          ಯದಿ (err != LONGINT_OK || (lo_ಒಳಹರಿವು <= hi_ಒಳಹರಿವು) == (n_ಸಾಲುಗಳು == 0))
            error (EXIT_FAILURE, err == LONGINT_OVERFLOW ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        ಭಙ್ಗ;

      ಪಕ್ಷ 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, ರಿಕ್ತ, 10, &argval, "");

          ಯದಿ (e == LONGINT_OK)
            head_ಸಾಲುಗಳು = MIN (head_ಸಾಲುಗಳು, argval);
          ಅನ್ಯಥಾ ಯದಿ (e != LONGINT_OVERFLOW)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        ಭಙ್ಗ;

      ಪಕ್ಷ 'o':
        ಯದಿ (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        ಭಙ್ಗ;

      ಪಕ್ಷ RANDOM_SOURCE_OPTION:
        ಯದಿ (random_source && !streq (random_source, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_source = optarg;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'r':
        repeat = true;
        ಭಙ್ಗ;

      ಪಕ್ಷ 'z':
        eolbyte = '\0';
        ಭಙ್ಗ;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_VERSION_CHAR (PROGRAM_NAME, AUTHORS);
      ಸ್ವಭಾವ:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  operand = argv + optind;

  /* Check invalid usage.  */
  ಯದಿ (echo && ಒಳಹರಿವು_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  ಯದಿ (ಒಳಹರಿವು_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (operand[!ಒಳಹರಿವು_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  ಯದಿ (head_ಸಾಲುಗಳು == 0)
    {
      n_ಸಾಲುಗಳು = 0;
      ಸಾಲು = ರಿಕ್ತ;
    }
  ಅನ್ಯಥಾ ಯದಿ (echo)
    {
      ಒಳಹರಿವು_from_argv (operand, n_operands, eolbyte);
      n_ಸಾಲುಗಳು = n_operands;
      ಸಾಲು = operand;
    }
  ಅನ್ಯಥಾ ಯದಿ (ಒಳಹರಿವು_range)
    {
      IF_LINT (n_ಸಾಲುಗಳು = hi_ಒಳಹರಿವು - lo_ಒಳಹರಿವು + 1); /* Avoid GCC 10 warning.  */
      ಸಾಲು = ರಿಕ್ತ;
    }
  ಅನ್ಯಥಾ
    {
      /* If an input file is specified, re-open it as stdin.  */
      ಯದಿ (n_operands == 1
          && ! (streq (operand[0], "-")
                || freopen (operand[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (operand[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      ಯದಿ (repeat || head_ಸಾಲುಗಳು == MIN (IDX_MAX, SIZE_MAX)
          || ಒಳಹರಿವು_size () <= RESERVOIR_MIN_ಒಳಹರಿವು)
        {
          n_ಸಾಲುಗಳು = read_ಒಳಹರಿವು (stdin, eolbyte, &ಒಳಹರಿವು_ಸಾಲುಗಳು);
          ಸಾಲು = ಒಳಹರಿವು_ಸಾಲುಗಳು;
        }
      ಅನ್ಯಥಾ
        {
          use_reservoir_sampling = true;
          n_ಸಾಲುಗಳು = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_ಸಾಲುಗಳು = repeat || head_ಸಾಲುಗಳು < n_ಸಾಲುಗಳು ? head_ಸಾಲುಗಳು : n_ಸಾಲುಗಳು;

  randint_source = randint_all_new (random_source,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_ಸಾಲುಗಳು, n_ಸಾಲುಗಳು)));
  ಯದಿ (! randint_source)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_source ? random_source : "getrandom"));

  ಯದಿ (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_ಸಾಲುಗಳು = read_ಒಳಹರಿವು_reservoir_sampling (stdin, eolbyte, ahead_ಸಾಲುಗಳು,
                                               randint_source, &reservoir);
      ahead_ಸಾಲುಗಳು = n_ಸಾಲುಗಳು;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  ಯದಿ (! (head_ಸಾಲುಗಳು == 0 || echo || ಒಳಹರಿವು_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  ಯದಿ (!repeat)
    permutation = randperm_new (randint_source, ahead_ಸಾಲುಗಳು, n_ಸಾಲುಗಳು);

  ಯದಿ (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  ಯದಿ (repeat)
    {
      ಯದಿ (head_ಸಾಲುಗಳು == 0)
        i = 0;
      ಅನ್ಯಥಾ
        {
          ಯದಿ (n_ಸಾಲುಗಳು == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          ಯದಿ (ಒಳಹರಿವು_range)
            i = write_random_numbers (randint_source, ahead_ಸಾಲುಗಳು,
                                      lo_ಒಳಹರಿವು, hi_ಒಳಹರಿವು, eolbyte);
          ಅನ್ಯಥಾ
            i = write_random_ಸಾಲುಗಳು (randint_source, ahead_ಸಾಲುಗಳು, ಸಾಲು, n_ಸಾಲುಗಳು);
        }
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_ಸಾಲುಗಳು, reservoir, permutation);
      ಅನ್ಯಥಾ ಯದಿ (ಒಳಹರಿವು_range)
        i = write_permuted_numbers (ahead_ಸಾಲುಗಳು, lo_ಒಳಹರಿವು,
                                    permutation, eolbyte);
      ಅನ್ಯಥಾ
        i = write_permuted_ಸಾಲುಗಳು (ahead_ಸಾಲುಗಳು, ಸಾಲು, permutation);
    }

  ಯದಿ (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_source));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
