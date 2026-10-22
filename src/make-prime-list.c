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

#شامل <inttypes.h>

#شامل <limits.h>
#شامل <stdint.h>
#شامل <stdio.h>
#شامل <string.h>
#شامل <stdlib.h>
#شامل <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#اگر 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#وگرنه
# define ATTRIBUTE_CONST
#endif
#اگر 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#وگرنه
# define ATTRIBUTE_MALLOC
#endif


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#ifndef wide_uint
# اگر 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
تعریفنوع unsigned __int128 wide_uint;
# وگرنه
تعریفنوع uintmax_t wide_uint;
# endif
#endif

ساختار اول
{
  unsigned p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
ایستا wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  برای (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      اگر (y == x)
        بازگشت x;
      x = y;
    }
}

ایستا تهی
process_اول (ساختار اول *info, unsigned p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

ایستا تهی
print_wide_uint (wide_uint n, صحیح nesting, unsigned wide_uint_bits)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  صحیح hex_digits_per_literal = 7;
  صحیح bits_per_literal = hex_digits_per_literal * 4;

  unsigned remainder = n & ((1 << bits_per_literal) - 1);

  اگر (n != remainder)
    {
      صحیح needs_parentheses = n >> bits_per_literal >> bits_per_literal != 0;
      اگر (needs_parentheses)
        printf ("(");
      print_wide_uint (n >> bits_per_literal, nesting + 1, wide_uint_bits);
      اگر (needs_parentheses)
        printf (")\n%*s", nesting + 3, "");
      printf (" << %d | ", bits_per_literal);
    }
  وگرنه اگر (nesting)
    {
      printf ("(mp_limb_t) ");
      hex_digits_per_literal
        = ((wide_uint_bits - 1) % bits_per_literal) % 4 + 1;
    }

  printf ("0x%0*xU", hex_digits_per_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#اگر 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

ایستا تهی
output_primes (ثابت ساختار اول *primes, unsigned nprimes)
{
  unsigned i;
  unsigned p;
  صحیح is_اول;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  unsigned wide_uint_bits = 0;
  wide_uint mask = -1;
  برای (wide_uint_bits = 0; mask; wide_uint_bits++)
    mask >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  printf ("#define WIDE_UINT_BITS %u\n", wide_uint_bits);

  برای (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      اگر ((1 << (16 - 1)) <= primes[i].p)
        abort ();

      printf ("P (%u,\n   (", primes[i].p);
      print_wide_uint (primes[i].pinv, 0, wide_uint_bits);
      printf ("),\n   (mp_limb_t) -1 / %u)\n", primes[i].p);
    }

  /* Find next prime */
  p = primes[nprimes - 1].p;
  انجامبده
    {
      p += 2;
      برای (i = 0, is_اول = 1; is_اول; i++)
        {
          اگر (primes[i].p * primes[i].p > p)
            بشکن;
          اگر (p * primes[i].pinv <= primes[i].lim)
            {
              is_اول = 0;
              بشکن;
            }
        }
    }
  وقتی (!is_اول);

  printf ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
ایستا تهی *
xalloc (size_t s)
{
  تهی *p = malloc (s);
  اگر (p)
    بازگشت p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  exit (EXIT_FAILURE);
}

صحیح
اصلی (صحیح argc, نویسه **argv)
{
  صحیح limit;

  نویسه *sieve;
  size_t size, i;

  ساختار اول *اول_list;
  unsigned nprimes;

  اگر (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      بازگشت EXIT_FAILURE;
    }
  limit = atoi (argv[1]);
  اگر (limit < 3)
    بازگشت EXIT_SUCCESS;

  /* Make limit odd */
  اگر ( !(limit & 1))
    limit--;

  size = (limit - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (size);
  memset (sieve, 1, size);

  اول_list = xalloc (size * اندازه (*اول_list));
  nprimes = 0;

  برای (i = 0; i < size;)
    {
      unsigned p = 3 + 2 * i;
      unsigned j;

      process_اول (&اول_list[nprimes++], p);

      برای (j = (p * p - 3) / 2; j < size; j += p)
        sieve[j] = 0;

      وقتی (++i < size && sieve[i] == 0)
        ;
    }

  output_primes (اول_list, nprimes);

  آزاد (sieve);
  آزاد (اول_list);

  اگر (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      بازگشت EXIT_FAILURE;
    }

  بازگشت EXIT_SUCCESS;
}
