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

#অন্তর্ভাব <inttypes.h>

#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <stdint.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#যদি 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#অন্যথা
# define ATTRIBUTE_CONST
#endif
#যদি 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#অন্যথা
# define ATTRIBUTE_MALLOC
#endif


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#ifndef wide_uint
# যদি 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
প্রকারলেখ unsigned __int128 wide_uint;
# অন্যথা
প্রকারলেখ uintmax_t wide_uint;
# endif
#endif

কাঠামো মৌলিক
{
  unsigned p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
স্থির wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  জন্য (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      যদি (y == x)
        ফেরত x;
      x = y;
    }
}

স্থির শূন্য
process_মৌলিক (কাঠামো মৌলিক *info, unsigned p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

স্থির শূন্য
print_wide_uint (wide_uint n, পূর্ণসংখ্যা nesting, unsigned wide_uint_bits)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  পূর্ণসংখ্যা hex_digits_per_literal = 7;
  পূর্ণসংখ্যা bits_per_literal = hex_digits_per_literal * 4;

  unsigned remainder = n & ((1 << bits_per_literal) - 1);

  যদি (n != remainder)
    {
      পূর্ণসংখ্যা needs_parentheses = n >> bits_per_literal >> bits_per_literal != 0;
      যদি (needs_parentheses)
        printf ("(");
      print_wide_uint (n >> bits_per_literal, nesting + 1, wide_uint_bits);
      যদি (needs_parentheses)
        printf (")\n%*s", nesting + 3, "");
      printf (" << %d | ", bits_per_literal);
    }
  অন্যথা যদি (nesting)
    {
      printf ("(mp_limb_t) ");
      hex_digits_per_literal
        = ((wide_uint_bits - 1) % bits_per_literal) % 4 + 1;
    }

  printf ("0x%0*xU", hex_digits_per_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#যদি 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

স্থির শূন্য
output_primes (ধ্রুবক কাঠামো মৌলিক *primes, unsigned nprimes)
{
  unsigned i;
  unsigned p;
  পূর্ণসংখ্যা is_মৌলিক;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  unsigned wide_uint_bits = 0;
  wide_uint mask = -1;
  জন্য (wide_uint_bits = 0; mask; wide_uint_bits++)
    mask >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  printf ("#define WIDE_UINT_BITS %u\n", wide_uint_bits);

  জন্য (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      যদি ((1 << (16 - 1)) <= primes[i].p)
        abort ();

      printf ("P (%u,\n   (", primes[i].p);
      print_wide_uint (primes[i].pinv, 0, wide_uint_bits);
      printf ("),\n   (mp_limb_t) -1 / %u)\n", primes[i].p);
    }

  /* Find next prime */
  p = primes[nprimes - 1].p;
  করো
    {
      p += 2;
      জন্য (i = 0, is_মৌলিক = 1; is_মৌলিক; i++)
        {
          যদি (primes[i].p * primes[i].p > p)
            বিরতি;
          যদি (p * primes[i].pinv <= primes[i].lim)
            {
              is_মৌলিক = 0;
              বিরতি;
            }
        }
    }
  যখন (!is_মৌলিক);

  printf ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
স্থির শূন্য *
xalloc (size_t s)
{
  শূন্য *p = malloc (s);
  যদি (p)
    ফেরত p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  exit (EXIT_FAILURE);
}

পূর্ণসংখ্যা
প্রধান (পূর্ণসংখ্যা argc, অক্ষর **argv)
{
  পূর্ণসংখ্যা limit;

  অক্ষর *sieve;
  size_t আকার_2, i;

  কাঠামো মৌলিক *মৌলিক_list;
  unsigned nprimes;

  যদি (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      ফেরত EXIT_FAILURE;
    }
  limit = atoi (argv[1]);
  যদি (limit < 3)
    ফেরত EXIT_SUCCESS;

  /* Make limit odd */
  যদি ( !(limit & 1))
    limit--;

  আকার_2 = (limit - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (আকার_2);
  memset (sieve, 1, আকার_2);

  মৌলিক_list = xalloc (আকার_2 * আকার (*মৌলিক_list));
  nprimes = 0;

  জন্য (i = 0; i < আকার_2;)
    {
      unsigned p = 3 + 2 * i;
      unsigned j;

      process_মৌলিক (&মৌলিক_list[nprimes++], p);

      জন্য (j = (p * p - 3) / 2; j < আকার_2; j += p)
        sieve[j] = 0;

      যখন (++i < আকার_2 && sieve[i] == 0)
        ;
    }

  output_primes (মৌলিক_list, nprimes);

  মুক্ত (sieve);
  মুক্ত (মৌলিক_list);

  যদি (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      ফেরত EXIT_FAILURE;
    }

  ফেরত EXIT_SUCCESS;
}
