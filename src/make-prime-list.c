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

#include <inttypes.h>

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#အကယ်၍ 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#မဟုတ်ပါက
# define ATTRIBUTE_CONST
#endif
#အကယ်၍ 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#မဟုတ်ပါက
# define ATTRIBUTE_MALLOC
#endif


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#ifndef wide_uint
# အကယ်၍ 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
typedef unsigned __int128 wide_uint;
# မဟုတ်ပါက
typedef uintmax_t wide_uint;
# endif
#endif

sa_nracanaa prime
{
  unsigned p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
static wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  အတွက် (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      အကယ်၍ (y == x)
        ပြန်ပေး x;
      x = y;
    }
}

static void
process_prime (sa_nracanaa prime *info, unsigned p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

static void
print_wide_uint (wide_uint n, ကိန်းပြည့် nesting, unsigned wide_uint_bits)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  ကိန်းပြည့် hex_digits_per_literal = 7;
  ကိန်းပြည့် bits_per_literal = hex_digits_per_literal * 4;

  unsigned remainder = n & ((1 << bits_per_literal) - 1);

  အကယ်၍ (n != remainder)
    {
      ကိန်းပြည့် needs_parentheses = n >> bits_per_literal >> bits_per_literal != 0;
      အကယ်၍ (needs_parentheses)
        printf ("(");
      print_wide_uint (n >> bits_per_literal, nesting + 1, wide_uint_bits);
      အကယ်၍ (needs_parentheses)
        printf (")\n%*s", nesting + 3, "");
      printf (" << %d | ", bits_per_literal);
    }
  မဟုတ်ပါက အကယ်၍ (nesting)
    {
      printf ("(mp_limb_t) ");
      hex_digits_per_literal
        = ((wide_uint_bits - 1) % bits_per_literal) % 4 + 1;
    }

  printf ("0x%0*xU", hex_digits_per_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#အကယ်၍ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

static void
output_primes (const sa_nracanaa prime *primes, unsigned nprimes)
{
  unsigned i;
  unsigned p;
  ကိန်းပြည့် is_prime;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  unsigned wide_uint_bits = 0;
  wide_uint mask = -1;
  အတွက် (wide_uint_bits = 0; mask; wide_uint_bits++)
    mask >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  printf ("#define WIDE_UINT_BITS %u\n", wide_uint_bits);

  အတွက် (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      အကယ်၍ ((1 << (16 - 1)) <= primes[i].p)
        abort ();

      printf ("P (%u,\n   (", primes[i].p);
      print_wide_uint (primes[i].pinv, 0, wide_uint_bits);
      printf ("),\n   (mp_limb_t) -1 / %u)\n", primes[i].p);
    }

  /* Find next prime */
  p = primes[nprimes - 1].p;
  do
    {
      p += 2;
      အတွက် (i = 0, is_prime = 1; is_prime; i++)
        {
          အကယ်၍ (primes[i].p * primes[i].p > p)
            bha_nga;
          အကယ်၍ (p * primes[i].pinv <= primes[i].lim)
            {
              is_prime = 0;
              bha_nga;
            }
        }
    }
  နေစဉ် (!is_prime);

  printf ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
static void *
xalloc (size_t s)
{
  void *p = malloc (s);
  အကယ်၍ (p)
    ပြန်ပေး p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  exit (EXIT_FAILURE);
}

ကိန်းပြည့်
main (ကိန်းပြည့် argc, var_na **argv)
{
  ကိန်းပြည့် limit;

  var_na *sieve;
  size_t size, i;

  sa_nracanaa prime *prime_list;
  unsigned nprimes;

  အကယ်၍ (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      ပြန်ပေး EXIT_FAILURE;
    }
  limit = atoi (argv[1]);
  အကယ်၍ (limit < 3)
    ပြန်ပေး EXIT_SUCCESS;

  /* Make limit odd */
  အကယ်၍ ( !(limit & 1))
    limit--;

  size = (limit - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (size);
  memset (sieve, 1, size);

  prime_list = xalloc (size * parimaa_na (*prime_list));
  nprimes = 0;

  အတွက် (i = 0; i < size;)
    {
      unsigned p = 3 + 2 * i;
      unsigned j;

      process_prime (&prime_list[nprimes++], p);

      အတွက် (j = (p * p - 3) / 2; j < size; j += p)
        sieve[j] = 0;

      နေစဉ် (++i < size && sieve[i] == 0)
        ;
    }

  output_primes (prime_list, nprimes);

  free (sieve);
  free (prime_list);

  အကယ်၍ (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      ပြန်ပေး EXIT_FAILURE;
    }

  ပြန်ပေး EXIT_SUCCESS;
}
