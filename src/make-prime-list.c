/* Factoring of uintmax_t numbers. Generation of needed tables.

   Contributed to the GNU project by Torbjörn Granlund and Niels Möller
   Contains code from GNU MP.

Copyright 2012-2026 Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see https://www.gnu.org/licenses/.  */

#ಅನ್ತರ್ಭಾವ <inttypes.h>

#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <stdint.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#ಯದಿ 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#ಅನ್ಯಥಾ
# define ATTRIBUTE_CONST
#endif
#ಯದಿ 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#ಅನ್ಯಥಾ
# define ATTRIBUTE_MALLOC
#endif


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#ifndef wide_uint
# ಯದಿ 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
ಪ್ರಕಾರನಿರ್ದೇಶ unsigned __int128 wide_uint;
# ಅನ್ಯಥಾ
ಪ್ರಕಾರನಿರ್ದೇಶ uintmax_t wide_uint;
# endif
#endif

ಸಂರಚನಾ ಅವಿಭಾಜ್ಯ
{
  unsigned p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
ಸ್ಥಿರ wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  ಪ್ರತಿ (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      ಯದಿ (y == x)
        ಪ್ರತ್ಯಾಗಮ x;
      x = y;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
process_ಅವಿಭಾಜ್ಯ (ಸಂರಚನಾ ಅವಿಭಾಜ್ಯ *info, unsigned p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_wide_uint (wide_uint n, ಪೂರ್ಣಾಙ್ಕ nesting, unsigned wide_uint_bits)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  ಪೂರ್ಣಾಙ್ಕ hex_digits_per_literal = 7;
  ಪೂರ್ಣಾಙ್ಕ bits_per_literal = hex_digits_per_literal * 4;

  unsigned remainder = n & ((1 << bits_per_literal) - 1);

  ಯದಿ (n != remainder)
    {
      ಪೂರ್ಣಾಙ್ಕ needs_parentheses = n >> bits_per_literal >> bits_per_literal != 0;
      ಯದಿ (needs_parentheses)
        printf ("(");
      print_wide_uint (n >> bits_per_literal, nesting + 1, wide_uint_bits);
      ಯದಿ (needs_parentheses)
        printf (")\n%*s", nesting + 3, "");
      printf (" << %d | ", bits_per_literal);
    }
  ಅನ್ಯಥಾ ಯದಿ (nesting)
    {
      printf ("(mp_limb_t) ");
      hex_digits_per_literal
        = ((wide_uint_bits - 1) % bits_per_literal) % 4 + 1;
    }

  printf ("0x%0*xU", hex_digits_per_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#ಯದಿ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

ಸ್ಥಿರ ಶೂನ್ಯ
output_primes (ಧ್ರುವ ಸಂರಚನಾ ಅವಿಭಾಜ್ಯ *primes, unsigned nprimes)
{
  unsigned i;
  unsigned p;
  ಪೂರ್ಣಾಙ್ಕ is_ಅವಿಭಾಜ್ಯ;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  unsigned wide_uint_bits = 0;
  wide_uint mask = -1;
  ಪ್ರತಿ (wide_uint_bits = 0; mask; wide_uint_bits++)
    mask >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  printf ("#define WIDE_UINT_BITS %u\n", wide_uint_bits);

  ಪ್ರತಿ (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      ಯದಿ ((1 << (16 - 1)) <= primes[i].p)
        abort ();

      printf ("P (%u,\n   (", primes[i].p);
      print_wide_uint (primes[i].pinv, 0, wide_uint_bits);
      printf ("),\n   (mp_limb_t) -1 / %u)\n", primes[i].p);
    }

  /* Find next prime */
  p = primes[nprimes - 1].p;
  ಕುರು
    {
      p += 2;
      ಪ್ರತಿ (i = 0, is_ಅವಿಭಾಜ್ಯ = 1; is_ಅವಿಭಾಜ್ಯ; i++)
        {
          ಯದಿ (primes[i].p * primes[i].p > p)
            ಭಙ್ಗ;
          ಯದಿ (p * primes[i].pinv <= primes[i].lim)
            {
              is_ಅವಿಭಾಜ್ಯ = 0;
              ಭಙ್ಗ;
            }
        }
    }
  ಯಾವತ್ (!is_ಅವಿಭಾಜ್ಯ);

  printf ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
ಸ್ಥಿರ ಶೂನ್ಯ *
xalloc (size_t s)
{
  ಶೂನ್ಯ *p = malloc (s);
  ಯದಿ (p)
    ಪ್ರತ್ಯಾಗಮ p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  exit (EXIT_FAILURE);
}

ಪೂರ್ಣಾಙ್ಕ
main (ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv)
{
  ಪೂರ್ಣಾಙ್ಕ limit;

  ವರ್ಣ *sieve;
  size_t size, i;

  ಸಂರಚನಾ ಅವಿಭಾಜ್ಯ *ಅವಿಭಾಜ್ಯ_list;
  unsigned nprimes;

  ಯದಿ (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      ಪ್ರತ್ಯಾಗಮ EXIT_FAILURE;
    }
  limit = atoi (argv[1]);
  ಯದಿ (limit < 3)
    ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;

  /* Make limit odd */
  ಯದಿ ( !(limit & 1))
    limit--;

  size = (limit - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (size);
  memset (sieve, 1, size);

  ಅವಿಭಾಜ್ಯ_list = xalloc (size * ಪರಿಮಾಣ (*ಅವಿಭಾಜ್ಯ_list));
  nprimes = 0;

  ಪ್ರತಿ (i = 0; i < size;)
    {
      unsigned p = 3 + 2 * i;
      unsigned j;

      process_ಅವಿಭಾಜ್ಯ (&ಅವಿಭಾಜ್ಯ_list[nprimes++], p);

      ಪ್ರತಿ (j = (p * p - 3) / 2; j < size; j += p)
        sieve[j] = 0;

      ಯಾವತ್ (++i < size && sieve[i] == 0)
        ;
    }

  output_primes (ಅವಿಭಾಜ್ಯ_list, nprimes);

  ಮುಕ್ತ (sieve);
  ಮುಕ್ತ (ಅವಿಭಾಜ್ಯ_list);

  ಯದಿ (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      ಪ್ರತ್ಯಾಗಮ EXIT_FAILURE;
    }

  ಪ್ರತ್ಯಾಗಮ EXIT_SUCCESS;
}
