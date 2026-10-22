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
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "shuf"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS proper_name ("Paul Eggert")

/* reservoir-sampling introduces CPU overhead for small inputs.
   So only enable it for inputs >= this limit.
   This limit was determined using these commands:
     $ for p in $(seq 7); do src/seq $((10**$p)) > 10p$p.in; done
     $ for p in $(seq 7); do time shuf-nores -n10 10p$p.in >/dev/null; done
     $ for p in $(seq 7); do time shuf -n10 10p$p.in >/dev/null; done  .*/
ಎಣಿಕೆ { RESERVOIR_MIN_ಒಳಹರಿವು = 8192 * 1024 };


ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION]... [FILE]\n\
  or:  %s -e [OPTION]... [ARG]...\n\
  or:  %s -i LO-HI [OPTION]...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name, ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Write a random permutation ದ the ಒಳಹರಿವು ಸಾಲುಗಳು ಗಮ್ಯ standard output.\n\
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
  -n, --ತಲೆ-ಎಣಿಕೆ_2=ಎಣಿಕೆ_3    output at most ಎಣಿಕೆ_3 ಸಾಲುಗಳು\n\
"));
      oputs (_("\
  -o, --output=FILE         ಲಿಖ ಫಲಿತಾಂಶ ಗಮ್ಯ FILE instead ದ standard output\n\
"));
      oputs (_("\
      --random-ಮೂಲ_2=FILE  get random ಬೈಟುಗಳು from FILE\n\
"));
      oputs (_("\
  -r, --repeat              output ಸಾಲುಗಳು can be repeated\n\
"));
      oputs (_("\
  -z, --zero-terminated     ಸಾಲು delimiter is NUL, not newline\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }

  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* For long options that have no equivalent short option, use a
   non-character as a pseudo short option, starting with CHAR_MAX + 1.  */
ಎಣಿಕೆ
{
  RANDOM_ಮೂಲ_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_opts[] =
{
  {"echo", no_ವಾದ, ಖಾಲಿ, 'e'},
  {"input-range", required_ವಾದ, ಖಾಲಿ, 'i'},
  {"head-count", required_ವಾದ, ಖಾಲಿ, 'n'},
  {"output", required_ವಾದ, ಖಾಲಿ, 'o'},
  {"random-source", required_ವಾದ, ಖಾಲಿ, RANDOM_ಮೂಲ_OPTION},
  {"repeat", no_ವಾದ, ಖಾಲಿ, 'r'},
  {"zero-terminated", no_ವಾದ, ಖಾಲಿ, 'z'},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0},
};

ಸ್ಥಿರ ಶೂನ್ಯ
ಒಳಹರಿವು_from_argv (ಅಕ್ಷರ **ಕಾರ್ಯಾಂಶ_2, ಪೂರ್ಣಾಂಕ n_operands, ಅಕ್ಷರ eolbyte)
{
  ಅಕ್ಷರ *p;
  size_t ಗಾತ್ರ_2 = n_operands;
  ಪೂರ್ಣಾಂಕ i;

  ಗಾಗಿ (i = 0; i < n_operands; i++)
    ಗಾತ್ರ_2 += ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (ಕಾರ್ಯಾಂಶ_2[i]);
  p = xmalloc (ಗಾತ್ರ_2);

  ಗಾಗಿ (i = 0; i < n_operands; i++)
    {
      ಅಕ್ಷರ *p1 = stpcpy (p, ಕಾರ್ಯಾಂಶ_2[i]);
      ಕಾರ್ಯಾಂಶ_2[i] = p;
      p = p1;
      *p++ = eolbyte;
    }

  ಕಾರ್ಯಾಂಶ_2[n_operands] = p;
}

/* Return the start of the next line after LINE, which is guaranteed
   to end in EOLBYTE.  */

ಸ್ಥಿರ ಅಕ್ಷರ *
ಮುಂದಿನ_ಸಾಲು (ಅಕ್ಷರ *ಸಾಲು, ಅಕ್ಷರ eolbyte)
{
  ಅಕ್ಷರ *p = rawmemchr (ಸಾಲು, eolbyte);
  ಹಿಂತಿರುಗಿ p + 1;
}

/* Return the size of the input if possible or OFF_T_MAX if not.  */

ಸ್ಥಿರ off_t
ಒಳಹರಿವು_ಗಾತ್ರ (ಶೂನ್ಯ)
{
  off_t file_ಗಾತ್ರ;

  ರಚನೆ stat stat_buf;
  ಆದರೆ (fstat (STDIN_FILENO, &stat_buf) != 0)
    ಹಿಂತಿರುಗಿ OFF_T_MAX;
  ಆದರೆ (usable_st_ಗಾತ್ರ (&stat_buf))
    file_ಗಾತ್ರ = stat_buf.st_ಗಾತ್ರ;
  ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ OFF_T_MAX;

  off_t ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ = lseek (STDIN_FILENO, 0, SEEK_CUR);
  ಆದರೆ (ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ < 0)
    ಹಿಂತಿರುಗಿ OFF_T_MAX;

  file_ಗಾತ್ರ -= ಒಳಹರಿವು_ಸ್ಥಾನಾಂತರ;

  ಹಿಂತಿರುಗಿ file_ಗಾತ್ರ;
}

/* Read all lines and store up to K permuted lines in *OUT_RSRV.
   Return the number of lines read, up to a maximum of K.  */

ಸ್ಥಿರ idx_t
read_ಒಳಹರಿವು_reservoir_sampling (FILE *ಒಳ, ಅಕ್ಷರ eolbyte, idx_t k,
                               ರಚನೆ randint_ಮೂಲ *s,
                               ರಚನೆ linebuffer **ಹೊರಹೋಗು_rsrv)
{
  randint n_ಸಾಲುಗಳು = 0;
  idx_t n_alloc_ಸಾಲುಗಳು = 0;
  ರಚನೆ linebuffer *ಸಾಲು = ಖಾಲಿ;
  ರಚನೆ linebuffer *rsrv = ಖಾಲಿ;

  /* Fill the first K lines, directly into the reservoir.  */
  ಗಾಗಿ (n_ಸಾಲುಗಳು = 0; n_ಸಾಲುಗಳು < k; n_ಸಾಲುಗಳು++)
    {
      /* Enlarge reservoir if needed.  */
      ಆದರೆ (n_ಸಾಲುಗಳು == n_alloc_ಸಾಲುಗಳು)
        {
          idx_t old = n_alloc_ಸಾಲುಗಳು;
          rsrv = xpalloc (rsrv, &n_alloc_ಸಾಲುಗಳು, 1, k, ಗಾತ್ರ *rsrv);
          memset (&rsrv[n_ಸಾಲುಗಳು], 0, (n_alloc_ಸಾಲುಗಳು - old) * ಗಾತ್ರ *rsrv);
        }

      ಸಾಲು = readlinebuffer_delim (&rsrv[n_ಸಾಲುಗಳು], ಒಳ, eolbyte);
      ಆದರೆ (!ಸಾಲು)
        ನಿಲ್ಲಿಸು;
    }

  /* last line wasn't null - so there may be more lines to read.  */
  ಆದರೆ (ಸಾಲು != ಖಾಲಿ)
    {
      ರಚನೆ linebuffer dummy;
      initbuffer (&dummy);  /* space for lines not put in reservoir.  */

      /* Choose the fate of the next line, with decreasing probability (as
         n_lines increases in size).

         If the line will be used, store it directly in the reservoir.
         Otherwise, store it in dummy space.

         With 'struct linebuffer', storing into existing buffer will reduce
         re-allocations (will only re-allocate if the new line is longer than
         the currently allocated space).  */
      ಮಾಡು
        {
          randint j = randint_ಆರಿಸು (s, n_ಸಾಲುಗಳು + 1);  /* 0 .. n_lines.  */
          ಸಾಲು = (j < k) ? (&rsrv[j]) : (&dummy);
        }
      ಆಗ (readlinebuffer_delim (ಸಾಲು, ಒಳ, eolbyte) != ಖಾಲಿ && n_ಸಾಲುಗಳು++);

      ಆದರೆ (! n_ಸಾಲುಗಳು)
        error (EXIT_FAILURE, EOVERFLOW, _("too many input lines"));

      freebuffer (&dummy);
    }

  /* no more input lines, or an input error.  */
  ಆದರೆ (ferror (ಒಳ))
    error (EXIT_FAILURE, errno, _("read error"));

  *ಹೊರಹೋಗು_rsrv = rsrv;
  ಹಿಂತಿರುಗಿ MIN (k, n_ಸಾಲುಗಳು);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_permuted_output_reservoir (size_t n_ಸಾಲುಗಳು, ರಚನೆ linebuffer *ಸಾಲುಗಳು,
                                 size_t ಸ್ಥಿರಾಂಕ *permutation)
{
  ಗಾಗಿ (size_t i = 0; i < n_ಸಾಲುಗಳು; i++)
    {
      ಸ್ಥಿರಾಂಕ ರಚನೆ linebuffer *p = &ಸಾಲುಗಳು[permutation[i]];
      ಆದರೆ (fwrite (p->ಬಫರ್, ಗಾತ್ರ (ಅಕ್ಷರ), p->ಉದ್ದಳತೆ, stdout) != p->ಉದ್ದಳತೆ)
        ಹಿಂತಿರುಗಿ -1;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Read data from file IN.  Input lines are delimited by EOLBYTE;
   silently append a trailing EOLBYTE if the file ends in some other
   byte.  Store a pointer to the resulting array of lines into *PLINE.
   Return the number of lines read.  Report an error and exit on
   failure.  */

ಸ್ಥಿರ size_t
read_ಒಳಹರಿವು (FILE *ಒಳ, ಅಕ್ಷರ eolbyte, ಅಕ್ಷರ ***pline)
{
  ಅಕ್ಷರ *p;
  ಅಕ್ಷರ *buf = ಖಾಲಿ;
  size_t ಬಳಸಿದ;
  ಅಕ್ಷರ *lim;
  ಅಕ್ಷರ **ಸಾಲು;
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
  ಆದರೆ (!(buf = fread_file (ಒಳ, 0, &ಬಳಸಿದ)))
    error (EXIT_FAILURE, errno, _("read error"));

  ಆದರೆ (ಬಳಸಿದ && buf[ಬಳಸಿದ - 1] != eolbyte)
    buf[ಬಳಸಿದ++] = eolbyte;

  lim = buf + ಬಳಸಿದ;

  n_ಸಾಲುಗಳು = 0;
  ಗಾಗಿ (p = buf; p < lim; p = ಮುಂದಿನ_ಸಾಲು (p, eolbyte))
    n_ಸಾಲುಗಳು++;

  *pline = ಸಾಲು = xnmalloc (n_ಸಾಲುಗಳು + 1, ಗಾತ್ರ *ಸಾಲು);

  ಸಾಲು[0] = p = buf;
  ಗಾಗಿ (size_t i = 1; i <= n_ಸಾಲುಗಳು; i++)
    ಸಾಲು[i] = p = ಮುಂದಿನ_ಸಾಲು (p, eolbyte);

  ಹಿಂತಿರುಗಿ n_ಸಾಲುಗಳು;
}

/* Output N_LINES lines to stdout from LINE array,
   chosen by the indices in PERMUTATION.
   PERMUTATION and LINE must have at least N_LINES elements.
   Strings in LINE must include the line-terminator character.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_permuted_ಸಾಲುಗಳು (size_t n_ಸಾಲುಗಳು, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *ಸಾಲು,
                      size_t ಸ್ಥಿರಾಂಕ *permutation)
{
  ಗಾಗಿ (size_t i = 0; i < n_ಸಾಲುಗಳು; i++)
    {
      ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *p = ಸಾಲು + permutation[i];
      size_t len = p[1] - p[0];
      ಆದರೆ (fwrite (p[0], ಗಾತ್ರ *p[0], len, stdout) != len)
        ಹಿಂತಿರುಗಿ -1;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Print NUMBER followed by EOLBYTE to standard output.
   Return false on failure, true on success.  */
ಸ್ಥಿರ bool
print_ಸಂಖ್ಯೆ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ ಸಂಖ್ಯೆ, ಅಕ್ಷರ eolbyte)
{
  ಅಕ್ಷರ buf[INT_BUFSIZE_BOUND (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ)];
  ಅಕ್ಷರ *p = buf + INT_STRLEN_BOUND (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ);
  *p = eolbyte;
  ಮಾಡು
    *--p = '0' + ಸಂಖ್ಯೆ % 10;
  ಆಗ ((ಸಂಖ್ಯೆ /= 10) != 0);
  idx_t len = buf + ಗಾತ್ರ buf - p;
  ಹಿಂತಿರುಗಿ fwrite (p, 1, len, stdout) == len;
}

/* Output N_LINES of numbers to stdout, from PERMUTATION array.
   PERMUTATION must have at least N_LINES elements.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_permuted_numbers (size_t n_ಸಾಲುಗಳು, size_t lo_ಒಳಹರಿವು,
                        size_t ಸ್ಥಿರಾಂಕ *permutation, ಅಕ್ಷರ eolbyte)
{
  ಗಾಗಿ (size_t i = 0; i < n_ಸಾಲುಗಳು; i++)
    {
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ n = lo_ಒಳಹರಿವು + permutation[i];
      ಆದರೆ (! print_ಸಂಖ್ಯೆ (n, eolbyte))
        ಹಿಂತಿರುಗಿ -1;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Output COUNT numbers to stdout, chosen randomly from range
   LO_INPUT through HI_INPUT.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_random_numbers (ರಚನೆ randint_ಮೂಲ *s, size_t ಎಣಿಕೆ_2,
                      size_t lo_ಒಳಹರಿವು, size_t hi_ಒಳಹರಿವು, ಅಕ್ಷರ eolbyte)
{
  ಸ್ಥಿರಾಂಕ randint range = hi_ಒಳಹರಿವು - lo_ಒಳಹರಿವು + 1;

  ಗಾಗಿ (size_t i = 0; i < ಎಣಿಕೆ_2; i++)
    {
      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ j = lo_ಒಳಹರಿವು + randint_ಆರಿಸು (s, range);
      ಆದರೆ (! print_ಸಂಖ್ಯೆ (j, eolbyte))
        ಹಿಂತಿರುಗಿ -1;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* Output COUNT lines to stdout from LINES array.
   LINES must have at least N_LINES elements in it.
   Strings in LINES_ must include the line-terminator character.  */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_random_ಸಾಲುಗಳು (ರಚನೆ randint_ಮೂಲ *s, size_t ಎಣಿಕೆ_2,
                    ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *ಸಾಲುಗಳು, size_t n_ಸಾಲುಗಳು)
{
  ಗಾಗಿ (size_t i = 0; i < ಎಣಿಕೆ_2; i++)
    {
      ಸ್ಥಿರಾಂಕ randint j = randint_ಆರಿಸು (s, n_ಸಾಲುಗಳು);
      ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *p = ಸಾಲುಗಳು + j;
      size_t len = p[1] - p[0];
      ಆದರೆ (fwrite (p[0], ಗಾತ್ರ *p[0], len, stdout) != len)
        ಹಿಂತಿರುಗಿ -1;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  bool echo = false;
  bool ಒಳಹರಿವು_range = false;
  size_t lo_ಒಳಹರಿವು = SIZE_MAX;
  size_t hi_ಒಳಹರಿವು = 0;
  idx_t ತಲೆ_ಸಾಲುಗಳು = MIN (IDX_MAX, SIZE_MAX);
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *outfile = ಖಾಲಿ;
  ಅಕ್ಷರ *random_ಮೂಲ = ಖಾಲಿ;
  ಅಕ್ಷರ eolbyte = '\n';
  ಅಕ್ಷರ **ಒಳಹರಿವು_ಸಾಲುಗಳು = ಖಾಲಿ;
  bool use_reservoir_sampling = false;
  bool repeat = false;

  ಪೂರ್ಣಾಂಕ optc;
  ಪೂರ್ಣಾಂಕ n_operands;
  ಅಕ್ಷರ **ಕಾರ್ಯಾಂಶ_2;
  size_t n_ಸಾಲುಗಳು;
  ಅಕ್ಷರ **ಸಾಲು = ಖಾಲಿ;
  ರಚನೆ linebuffer *reservoir = ಖಾಲಿ;
  ರಚನೆ randint_ಮೂಲ *randint_ಮೂಲ;
  size_t *permutation = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ i;

  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಆಗ ((optc = getopt_long (argc, argv, "ei:n:o:rz", long_opts, ಖಾಲಿ))
         != -1)
    ಆಯ್ಕೆ (optc)
      {
      ಸ್ಥಿತಿ 'e':
        echo = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'i':
        {
          ಆದರೆ (ಒಳಹರಿವು_range)
            error (EXIT_FAILURE, 0, _("multiple -i options specified"));
          ಒಳಹರಿವು_range = true;

          uintmax_t u;
          ಅಕ್ಷರ *lo_ಕೊನೆ;
          strtol_error err = xstrtoumax (optarg, &lo_ಕೊನೆ, 10, &u, ಖಾಲಿ);
          ಆದರೆ (err == LONGINT_OK)
            {
              lo_ಒಳಹರಿವು = u;
              ಆದರೆ (lo_ಒಳಹರಿವು != u)
                err = LONGINT_ಅತಿಪ್ರವಾಹ;
              ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (*lo_ಕೊನೆ != '-')
                err = LONGINT_INVALID;
              ಇಲ್ಲದಿದ್ದರೆ
                {
                  err = xstrtoumax (lo_ಕೊನೆ + 1, ಖಾಲಿ, 10, &u, "");
                  ಆದರೆ (err == LONGINT_OK)
                    {
                      hi_ಒಳಹರಿವು = u;
                      ಆದರೆ (hi_ಒಳಹರಿವು != u)
                        err = LONGINT_ಅತಿಪ್ರವಾಹ;
                    }
                }
            }

          n_ಸಾಲುಗಳು = hi_ಒಳಹರಿವು - lo_ಒಳಹರಿವು + 1;

          ಆದರೆ (err != LONGINT_OK || (lo_ಒಳಹರಿವು <= hi_ಒಳಹರಿವು) == (n_ಸಾಲುಗಳು == 0))
            error (EXIT_FAILURE, err == LONGINT_ಅತಿಪ್ರವಾಹ ? EOVERFLOW : 0,
                   "%s: %s", _("invalid input range"), quote (optarg));
        }
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'n':
        {
          uintmax_t argval;
          strtol_error e = xstrtoumax (optarg, ಖಾಲಿ, 10, &argval, "");

          ಆದರೆ (e == LONGINT_OK)
            ತಲೆ_ಸಾಲುಗಳು = MIN (ತಲೆ_ಸಾಲುಗಳು, argval);
          ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (e != LONGINT_ಅತಿಪ್ರವಾಹ)
            error (EXIT_FAILURE, 0, _("invalid line count: %s"),
                   quote (optarg));
        }
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'o':
        ಆದರೆ (outfile && !streq (outfile, optarg))
          error (EXIT_FAILURE, 0, _("multiple output files specified"));
        outfile = optarg;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ RANDOM_ಮೂಲ_OPTION:
        ಆದರೆ (random_ಮೂಲ && !streq (random_ಮೂಲ, optarg))
          error (EXIT_FAILURE, 0, _("multiple random sources specified"));
        random_ಮೂಲ = optarg;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'r':
        repeat = true;
        ನಿಲ್ಲಿಸು;

      ಸ್ಥಿತಿ 'z':
        eolbyte = '\0';
        ನಿಲ್ಲಿಸು;

      case_GETOPT_HELP_CHAR;
      case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);
      ಪೂರ್ವನಿಯೋಜಿತ:
        usage (EXIT_FAILURE);
      }

  n_operands = argc - optind;
  ಕಾರ್ಯಾಂಶ_2 = argv + optind;

  /* Check invalid usage.  */
  ಆದರೆ (echo && ಒಳಹರಿವು_range)
    {
      error (0, 0, _("cannot combine -e and -i options"));
      usage (EXIT_FAILURE);
    }
  ಆದರೆ (ಒಳಹರಿವು_range ? 0 < n_operands : !echo && 1 < n_operands)
    {
      error (0, 0, _("extra operand %s"), quote (ಕಾರ್ಯಾಂಶ_2[!ಒಳಹರಿವು_range]));
      usage (EXIT_FAILURE);
    }

  /* Prepare input.  */
  ಆದರೆ (ತಲೆ_ಸಾಲುಗಳು == 0)
    {
      n_ಸಾಲುಗಳು = 0;
      ಸಾಲು = ಖಾಲಿ;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (echo)
    {
      ಒಳಹರಿವು_from_argv (ಕಾರ್ಯಾಂಶ_2, n_operands, eolbyte);
      n_ಸಾಲುಗಳು = n_operands;
      ಸಾಲು = ಕಾರ್ಯಾಂಶ_2;
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಳಹರಿವು_range)
    {
      IF_LINT (n_ಸಾಲುಗಳು = hi_ಒಳಹರಿವು - lo_ಒಳಹರಿವು + 1); /* Avoid GCC 10 warning.  */
      ಸಾಲು = ಖಾಲಿ;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      /* If an input file is specified, re-open it as stdin.  */
      ಆದರೆ (n_operands == 1
          && ! (streq (ಕಾರ್ಯಾಂಶ_2[0], "-")
                || freopen (ಕಾರ್ಯಾಂಶ_2[0], "r", stdin)))
        error (EXIT_FAILURE, errno, "%s", quotef (ಕಾರ್ಯಾಂಶ_2[0]));

      fadvise (stdin, FADVISE_SEQUENTIAL);

      ಆದರೆ (repeat || ತಲೆ_ಸಾಲುಗಳು == MIN (IDX_MAX, SIZE_MAX)
          || ಒಳಹರಿವು_ಗಾತ್ರ () <= RESERVOIR_MIN_ಒಳಹರಿವು)
        {
          n_ಸಾಲುಗಳು = read_ಒಳಹರಿವು (stdin, eolbyte, &ಒಳಹರಿವು_ಸಾಲುಗಳು);
          ಸಾಲು = ಒಳಹರಿವು_ಸಾಲುಗಳು;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          use_reservoir_sampling = true;
          n_ಸಾಲುಗಳು = SIZE_MAX;   /* unknown number of input lines, for now.  */
        }
    }

  /* The adjusted head line count; can be less than HEAD_LINES if the
     input is small and if not repeating.  */
  idx_t ahead_ಸಾಲುಗಳು = repeat || ತಲೆ_ಸಾಲುಗಳು < n_ಸಾಲುಗಳು ? ತಲೆ_ಸಾಲುಗಳು : n_ಸಾಲುಗಳು;

  randint_ಮೂಲ = randint_all_new (random_ಮೂಲ,
                                    (use_reservoir_sampling || repeat
                                     ? SIZE_MAX
                                     : randperm_bound (ahead_ಸಾಲುಗಳು, n_ಸಾಲುಗಳು)));
  ಆದರೆ (! randint_ಮೂಲ)
    error (EXIT_FAILURE, errno, "%s",
           quotef (random_ಮೂಲ ? random_ಮೂಲ : "getrandom"));

  ಆದರೆ (use_reservoir_sampling)
    {
      /* Instead of reading the entire file into 'line',
         use reservoir-sampling to store just AHEAD_LINES random lines.  */
      n_ಸಾಲುಗಳು = read_ಒಳಹರಿವು_reservoir_sampling (stdin, eolbyte, ahead_ಸಾಲುಗಳು,
                                               randint_ಮೂಲ, &reservoir);
      ahead_ಸಾಲುಗಳು = n_ಸಾಲುಗಳು;
    }

  /* Close stdin now, rather than earlier, so that randint_all_new
     doesn't have to worry about opening something other than
     stdin.  */
  ಆದರೆ (! (ತಲೆ_ಸಾಲುಗಳು == 0 || echo || ಒಳಹರಿವು_range || fclose (stdin) == 0))
    error (EXIT_FAILURE, errno, _("read error"));

  ಆದರೆ (!repeat)
    permutation = randperm_new (randint_ಮೂಲ, ahead_ಸಾಲುಗಳು, n_ಸಾಲುಗಳು);

  ಆದರೆ (outfile && ! freopen (outfile, "w", stdout))
    error (EXIT_FAILURE, errno, "%s", quotef (outfile));

  /* Generate output according to requested method */
  ಆದರೆ (repeat)
    {
      ಆದರೆ (ತಲೆ_ಸಾಲುಗಳು == 0)
        i = 0;
      ಇಲ್ಲದಿದ್ದರೆ
        {
          ಆದರೆ (n_ಸಾಲುಗಳು == 0)
            error (EXIT_FAILURE, 0, _("no lines to repeat"));
          ಆದರೆ (ಒಳಹರಿವು_range)
            i = write_random_numbers (randint_ಮೂಲ, ahead_ಸಾಲುಗಳು,
                                      lo_ಒಳಹರಿವು, hi_ಒಳಹರಿವು, eolbyte);
          ಇಲ್ಲದಿದ್ದರೆ
            i = write_random_ಸಾಲುಗಳು (randint_ಮೂಲ, ahead_ಸಾಲುಗಳು, ಸಾಲು, n_ಸಾಲುಗಳು);
        }
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (use_reservoir_sampling)
        i = write_permuted_output_reservoir (n_ಸಾಲುಗಳು, reservoir, permutation);
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಒಳಹರಿವು_range)
        i = write_permuted_numbers (ahead_ಸಾಲುಗಳು, lo_ಒಳಹರಿವು,
                                    permutation, eolbyte);
      ಇಲ್ಲದಿದ್ದರೆ
        i = write_permuted_ಸಾಲುಗಳು (ahead_ಸಾಲುಗಳು, ಸಾಲು, permutation);
    }

  ಆದರೆ (i != 0)
    write_error ();

  IF_LINT (randint_all_free (randint_ಮೂಲ));  /* For older valgrind.  */

  main_exit (EXIT_SUCCESS);
}
