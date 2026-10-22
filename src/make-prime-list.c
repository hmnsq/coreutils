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

#അന്തര്ഭാവ <inttypes.h>

#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <stdint.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#എങ്കിൽ 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#അല്ലെങ്കിൽ
# define ATTRIBUTE_CONST
#endif
#എങ്കിൽ 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#അല്ലെങ്കിൽ
# define ATTRIBUTE_MALLOC
#endif


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#ifndef wide_uint
# എങ്കിൽ 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
തരനിർവചനം unsigned __int128 wide_uint;
# അല്ലെങ്കിൽ
തരനിർവചനം uintmax_t wide_uint;
# endif
#endif

ഘടന അഭാജ്യം
{
  unsigned p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
സ്ഥിതം wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  വേണ്ടി (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      എങ്കിൽ (y == x)
        തിരികെ x;
      x = y;
    }
}

സ്ഥിതം ശൂന്യം
process_അഭാജ്യം (ഘടന അഭാജ്യം *info, unsigned p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

സ്ഥിതം ശൂന്യം
print_wide_uint (wide_uint n, പൂർണ്ണസംഖ്യ nesting, unsigned wide_uint_bits)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  പൂർണ്ണസംഖ്യ hex_digits_per_literal = 7;
  പൂർണ്ണസംഖ്യ bits_per_literal = hex_digits_per_literal * 4;

  unsigned remainder = n & ((1 << bits_per_literal) - 1);

  എങ്കിൽ (n != remainder)
    {
      പൂർണ്ണസംഖ്യ needs_parentheses = n >> bits_per_literal >> bits_per_literal != 0;
      എങ്കിൽ (needs_parentheses)
        printf ("(");
      print_wide_uint (n >> bits_per_literal, nesting + 1, wide_uint_bits);
      എങ്കിൽ (needs_parentheses)
        printf (")\n%*s", nesting + 3, "");
      printf (" << %d | ", bits_per_literal);
    }
  അല്ലെങ്കിൽ എങ്കിൽ (nesting)
    {
      printf ("(mp_limb_t) ");
      hex_digits_per_literal
        = ((wide_uint_bits - 1) % bits_per_literal) % 4 + 1;
    }

  printf ("0x%0*xU", hex_digits_per_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#എങ്കിൽ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

സ്ഥിതം ശൂന്യം
output_primes (സ്ഥിരം ഘടന അഭാജ്യം *primes, unsigned nprimes)
{
  unsigned i;
  unsigned p;
  പൂർണ്ണസംഖ്യ is_അഭാജ്യം;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  unsigned wide_uint_bits = 0;
  wide_uint mask = -1;
  വേണ്ടി (wide_uint_bits = 0; mask; wide_uint_bits++)
    mask >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  printf ("#define WIDE_UINT_BITS %u\n", wide_uint_bits);

  വേണ്ടി (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      എങ്കിൽ ((1 << (16 - 1)) <= primes[i].p)
        abort ();

      printf ("P (%u,\n   (", primes[i].p);
      print_wide_uint (primes[i].pinv, 0, wide_uint_bits);
      printf ("),\n   (mp_limb_t) -1 / %u)\n", primes[i].p);
    }

  /* Find next prime */
  p = primes[nprimes - 1].p;
  ചെയ്യുക
    {
      p += 2;
      വേണ്ടി (i = 0, is_അഭാജ്യം = 1; is_അഭാജ്യം; i++)
        {
          എങ്കിൽ (primes[i].p * primes[i].p > p)
            നിർത്തുക;
          എങ്കിൽ (p * primes[i].pinv <= primes[i].lim)
            {
              is_അഭാജ്യം = 0;
              നിർത്തുക;
            }
        }
    }
  ആകുമ്പോൾ (!is_അഭാജ്യം);

  printf ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
സ്ഥിതം ശൂന്യം *
xalloc (size_t s)
{
  ശൂന്യം *p = malloc (s);
  എങ്കിൽ (p)
    തിരികെ p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  exit (EXIT_FAILURE);
}

പൂർണ്ണസംഖ്യ
പ്രധാന (പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv)
{
  പൂർണ്ണസംഖ്യ limit;

  അക്ഷരം *sieve;
  size_t size, i;

  ഘടന അഭാജ്യം *അഭാജ്യം_list;
  unsigned nprimes;

  എങ്കിൽ (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      തിരികെ EXIT_FAILURE;
    }
  limit = atoi (argv[1]);
  എങ്കിൽ (limit < 3)
    തിരികെ EXIT_SUCCESS;

  /* Make limit odd */
  എങ്കിൽ ( !(limit & 1))
    limit--;

  size = (limit - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (size);
  memset (sieve, 1, size);

  അഭാജ്യം_list = xalloc (size * വലുപ്പം (*അഭാജ്യം_list));
  nprimes = 0;

  വേണ്ടി (i = 0; i < size;)
    {
      unsigned p = 3 + 2 * i;
      unsigned j;

      process_അഭാജ്യം (&അഭാജ്യം_list[nprimes++], p);

      വേണ്ടി (j = (p * p - 3) / 2; j < size; j += p)
        sieve[j] = 0;

      ആകുമ്പോൾ (++i < size && sieve[i] == 0)
        ;
    }

  output_primes (അഭാജ്യം_list, nprimes);

  സ്വതന്ത്രം (sieve);
  സ്വതന്ത്രം (അഭാജ്യം_list);

  എങ്കിൽ (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      തിരികെ EXIT_FAILURE;
    }

  തിരികെ EXIT_SUCCESS;
}
