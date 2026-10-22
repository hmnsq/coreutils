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

#ଅନ୍ତର୍ଭାଵ <inttypes.h>

#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <stdint.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#ଯଦି 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#ଅନ୍ୟଥା
# define ATTRIBUTE_CONST
#endif
#ଯଦି 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#ଅନ୍ୟଥା
# define ATTRIBUTE_MALLOC
#endif


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#ifndef wide_uint
# ଯଦି 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned __int128 wide_uint;
# ଅନ୍ୟଥା
ପ୍ରକାରବ୍ୟାଖ୍ୟା uintmax_t wide_uint;
# endif
#endif

ସଂରଚନା ମୌଳିକ
{
  unsigned p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
ସ୍ଥିର wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  ପାଇଁ (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      ଯଦି (y == x)
        ଫେରନ୍ତୁ x;
      x = y;
    }
}

ସ୍ଥିର ଶୂନ୍ୟ
process_ମୌଳିକ (ସଂରଚନା ମୌଳିକ *info, unsigned p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

ସ୍ଥିର ଶୂନ୍ୟ
print_wide_uint (wide_uint n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା nesting, unsigned wide_uint_bits)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା hex_digits_per_literal = 7;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା bits_per_literal = hex_digits_per_literal * 4;

  unsigned remainder = n & ((1 << bits_per_literal) - 1);

  ଯଦି (n != remainder)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା needs_parentheses = n >> bits_per_literal >> bits_per_literal != 0;
      ଯଦି (needs_parentheses)
        printf ("(");
      print_wide_uint (n >> bits_per_literal, nesting + 1, wide_uint_bits);
      ଯଦି (needs_parentheses)
        printf (")\n%*s", nesting + 3, "");
      printf (" << %d | ", bits_per_literal);
    }
  ଅନ୍ୟଥା ଯଦି (nesting)
    {
      printf ("(mp_limb_t) ");
      hex_digits_per_literal
        = ((wide_uint_bits - 1) % bits_per_literal) % 4 + 1;
    }

  printf ("0x%0*xU", hex_digits_per_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#ଯଦି 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

ସ୍ଥିର ଶୂନ୍ୟ
output_primes (ଅଚଳ ସଂରଚନା ମୌଳିକ *primes, unsigned nprimes)
{
  unsigned i;
  unsigned p;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା is_ମୌଳିକ;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  unsigned wide_uint_bits = 0;
  wide_uint mask = -1;
  ପାଇଁ (wide_uint_bits = 0; mask; wide_uint_bits++)
    mask >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  printf ("#define WIDE_UINT_BITS %u\n", wide_uint_bits);

  ପାଇଁ (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      ଯଦି ((1 << (16 - 1)) <= primes[i].p)
        abort ();

      printf ("P (%u,\n   (", primes[i].p);
      print_wide_uint (primes[i].pinv, 0, wide_uint_bits);
      printf ("),\n   (mp_limb_t) -1 / %u)\n", primes[i].p);
    }

  /* Find next prime */
  p = primes[nprimes - 1].p;
  କର
    {
      p += 2;
      ପାଇଁ (i = 0, is_ମୌଳିକ = 1; is_ମୌଳିକ; i++)
        {
          ଯଦି (primes[i].p * primes[i].p > p)
            ଭାଙ୍ଗ;
          ଯଦି (p * primes[i].pinv <= primes[i].lim)
            {
              is_ମୌଳିକ = 0;
              ଭାଙ୍ଗ;
            }
        }
    }
  ଯେତେବେଳେ (!is_ମୌଳିକ);

  printf ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
ସ୍ଥିର ଶୂନ୍ୟ *
xalloc (size_t s)
{
  ଶୂନ୍ୟ *p = malloc (s);
  ଯଦି (p)
    ଫେରନ୍ତୁ p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  exit (EXIT_FAILURE);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ମୁଖ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା limit;

  ଅକ୍ଷର *sieve;
  size_t size, i;

  ସଂରଚନା ମୌଳିକ *ମୌଳିକ_list;
  unsigned nprimes;

  ଯଦି (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      ଫେରନ୍ତୁ EXIT_FAILURE;
    }
  limit = atoi (argv[1]);
  ଯଦି (limit < 3)
    ଫେରନ୍ତୁ EXIT_SUCCESS;

  /* Make limit odd */
  ଯଦି ( !(limit & 1))
    limit--;

  size = (limit - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (size);
  memset (sieve, 1, size);

  ମୌଳିକ_list = xalloc (size * ଆକାର (*ମୌଳିକ_list));
  nprimes = 0;

  ପାଇଁ (i = 0; i < size;)
    {
      unsigned p = 3 + 2 * i;
      unsigned j;

      process_ମୌଳିକ (&ମୌଳିକ_list[nprimes++], p);

      ପାଇଁ (j = (p * p - 3) / 2; j < size; j += p)
        sieve[j] = 0;

      ଯେତେବେଳେ (++i < size && sieve[i] == 0)
        ;
    }

  output_primes (ମୌଳିକ_list, nprimes);

  ମୁକ୍ତ (sieve);
  ମୁକ୍ତ (ମୌଳିକ_list);

  ଯଦି (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      ଫେରନ୍ତୁ EXIT_FAILURE;
    }

  ଫେରନ୍ତୁ EXIT_SUCCESS;
}
