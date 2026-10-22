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

#அந்தர்பாவ <inttypes.h>

#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <stdint.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#எனில் 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#இல்லையெனில்
# define ATTRIBUTE_CONST
#endif
#எனில் 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#இல்லையெனில்
# define ATTRIBUTE_MALLOC
#endif


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#ifndef wide_uint
# எனில் 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
வகைவரை unsigned __int128 wide_uint;
# இல்லையெனில்
வகைவரை uintmax_t wide_uint;
# endif
#endif

கட்டமைப்பு பகாஎண்
{
  unsigned p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
நிலையான wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  க்கு (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      எனில் (y == x)
        திரும்பு x;
      x = y;
    }
}

நிலையான வெறுமை
process_பகாஎண் (கட்டமைப்பு பகாஎண் *info, unsigned p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

நிலையான வெறுமை
print_wide_uint (wide_uint n, முழு nesting, unsigned wide_uint_bits)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  முழு hex_digits_per_literal = 7;
  முழு bits_per_literal = hex_digits_per_literal * 4;

  unsigned remainder = n & ((1 << bits_per_literal) - 1);

  எனில் (n != remainder)
    {
      முழு needs_parentheses = n >> bits_per_literal >> bits_per_literal != 0;
      எனில் (needs_parentheses)
        printf ("(");
      print_wide_uint (n >> bits_per_literal, nesting + 1, wide_uint_bits);
      எனில் (needs_parentheses)
        printf (")\n%*s", nesting + 3, "");
      printf (" << %d | ", bits_per_literal);
    }
  இல்லையெனில் எனில் (nesting)
    {
      printf ("(mp_limb_t) ");
      hex_digits_per_literal
        = ((wide_uint_bits - 1) % bits_per_literal) % 4 + 1;
    }

  printf ("0x%0*xU", hex_digits_per_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#எனில் 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

நிலையான வெறுமை
output_primes (மாறிலி கட்டமைப்பு பகாஎண் *primes, unsigned nprimes)
{
  unsigned i;
  unsigned p;
  முழு is_பகாஎண்;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  unsigned wide_uint_bits = 0;
  wide_uint mask = -1;
  க்கு (wide_uint_bits = 0; mask; wide_uint_bits++)
    mask >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  printf ("#define WIDE_UINT_BITS %u\n", wide_uint_bits);

  க்கு (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      எனில் ((1 << (16 - 1)) <= primes[i].p)
        abort ();

      printf ("P (%u,\n   (", primes[i].p);
      print_wide_uint (primes[i].pinv, 0, wide_uint_bits);
      printf ("),\n   (mp_limb_t) -1 / %u)\n", primes[i].p);
    }

  /* Find next prime */
  p = primes[nprimes - 1].p;
  செய்
    {
      p += 2;
      க்கு (i = 0, is_பகாஎண் = 1; is_பகாஎண்; i++)
        {
          எனில் (primes[i].p * primes[i].p > p)
            நிறுத்து;
          எனில் (p * primes[i].pinv <= primes[i].lim)
            {
              is_பகாஎண் = 0;
              நிறுத்து;
            }
        }
    }
  போது (!is_பகாஎண்);

  printf ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
நிலையான வெறுமை *
xalloc (size_t s)
{
  வெறுமை *p = malloc (s);
  எனில் (p)
    திரும்பு p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  exit (EXIT_FAILURE);
}

முழு
முதன்மை (முழு argc, எழுத்து **argv)
{
  முழு limit;

  எழுத்து *sieve;
  size_t size, i;

  கட்டமைப்பு பகாஎண் *பகாஎண்_list;
  unsigned nprimes;

  எனில் (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      திரும்பு EXIT_FAILURE;
    }
  limit = atoi (argv[1]);
  எனில் (limit < 3)
    திரும்பு EXIT_SUCCESS;

  /* Make limit odd */
  எனில் ( !(limit & 1))
    limit--;

  size = (limit - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (size);
  memset (sieve, 1, size);

  பகாஎண்_list = xalloc (size * அளவு (*பகாஎண்_list));
  nprimes = 0;

  க்கு (i = 0; i < size;)
    {
      unsigned p = 3 + 2 * i;
      unsigned j;

      process_பகாஎண் (&பகாஎண்_list[nprimes++], p);

      க்கு (j = (p * p - 3) / 2; j < size; j += p)
        sieve[j] = 0;

      போது (++i < size && sieve[i] == 0)
        ;
    }

  output_primes (பகாஎண்_list, nprimes);

  விடுவி (sieve);
  விடுவி (பகாஎண்_list);

  எனில் (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      திரும்பு EXIT_FAILURE;
    }

  திரும்பு EXIT_SUCCESS;
}
