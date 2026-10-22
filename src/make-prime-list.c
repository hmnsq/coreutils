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

#ضمن <inttypes.h>

#ضمن <limits.h>
#ضمن <stdint.h>
#ضمن <stdio.h>
#ضمن <string.h>
#ضمن <stdlib.h>
#ضمن <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#إذا 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#وإلا
# define ATTRIBUTE_CONST
#endif
#إذا 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#وإلا
# define ATTRIBUTE_MALLOC
#endif


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#ifndef wide_uint
# إذا 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
تعريفنوع unsigned __int128 wide_uint;
# وإلا
تعريفنوع uintmax_t wide_uint;
# endif
#endif

بنية سەرەتایی
{
  unsigned p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
ساكن wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  لكل (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      إذا (y == x)
        أرجع x;
      x = y;
    }
}

ساكن فراغ
process_سەرەتایی (بنية سەرەتایی *info, unsigned p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

ساكن فراغ
print_wide_uint (wide_uint n, صحيح nesting, unsigned wide_uint_bits)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  صحيح hex_digits_per_literal = 7;
  صحيح bits_per_literal = hex_digits_per_literal * 4;

  unsigned remainder = n & ((1 << bits_per_literal) - 1);

  إذا (n != remainder)
    {
      صحيح needs_parentheses = n >> bits_per_literal >> bits_per_literal != 0;
      إذا (needs_parentheses)
        printf ("(");
      print_wide_uint (n >> bits_per_literal, nesting + 1, wide_uint_bits);
      إذا (needs_parentheses)
        printf (")\n%*s", nesting + 3, "");
      printf (" << %d | ", bits_per_literal);
    }
  وإلا إذا (nesting)
    {
      printf ("(mp_limb_t) ");
      hex_digits_per_literal
        = ((wide_uint_bits - 1) % bits_per_literal) % 4 + 1;
    }

  printf ("0x%0*xU", hex_digits_per_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#إذا 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

ساكن فراغ
output_primes (ثابت بنية سەرەتایی *primes, unsigned nprimes)
{
  unsigned i;
  unsigned p;
  صحيح is_سەرەتایی;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  unsigned wide_uint_bits = 0;
  wide_uint mask = -1;
  لكل (wide_uint_bits = 0; mask; wide_uint_bits++)
    mask >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  printf ("#define WIDE_UINT_BITS %u\n", wide_uint_bits);

  لكل (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      إذا ((1 << (16 - 1)) <= primes[i].p)
        abort ();

      printf ("P (%u,\n   (", primes[i].p);
      print_wide_uint (primes[i].pinv, 0, wide_uint_bits);
      printf ("),\n   (mp_limb_t) -1 / %u)\n", primes[i].p);
    }

  /* Find next prime */
  p = primes[nprimes - 1].p;
  افعل
    {
      p += 2;
      لكل (i = 0, is_سەرەتایی = 1; is_سەرەتایی; i++)
        {
          إذا (primes[i].p * primes[i].p > p)
            اكسر;
          إذا (p * primes[i].pinv <= primes[i].lim)
            {
              is_سەرەتایی = 0;
              اكسر;
            }
        }
    }
  طالما (!is_سەرەتایی);

  printf ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
ساكن فراغ *
xalloc (size_t s)
{
  فراغ *p = malloc (s);
  إذا (p)
    أرجع p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  exit (EXIT_FAILURE);
}

صحيح
main (صحيح argc, حرف **argv)
{
  صحيح limit;

  حرف *sieve;
  size_t size, i;

  بنية سەرەتایی *سەرەتایی_list;
  unsigned nprimes;

  إذا (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      أرجع EXIT_FAILURE;
    }
  limit = atoi (argv[1]);
  إذا (limit < 3)
    أرجع EXIT_SUCCESS;

  /* Make limit odd */
  إذا ( !(limit & 1))
    limit--;

  size = (limit - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (size);
  memset (sieve, 1, size);

  سەرەتایی_list = xalloc (size * حجم (*سەرەتایی_list));
  nprimes = 0;

  لكل (i = 0; i < size;)
    {
      unsigned p = 3 + 2 * i;
      unsigned j;

      process_سەرەتایی (&سەرەتایی_list[nprimes++], p);

      لكل (j = (p * p - 3) / 2; j < size; j += p)
        sieve[j] = 0;

      طالما (++i < size && sieve[i] == 0)
        ;
    }

  output_primes (سەرەتایی_list, nprimes);

  حرر (sieve);
  حرر (سەرەتایی_list);

  إذا (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      أرجع EXIT_FAILURE;
    }

  أرجع EXIT_SUCCESS;
}
