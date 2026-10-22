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

#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdint.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#ਜੇ 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#ਨਹੀਂਤਾਂ
# define ATTRIBUTE_CONST
#ਜੇ_ਅੰਤ
#ਜੇ 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#ਨਹੀਂਤਾਂ
# define ATTRIBUTE_MALLOC
#ਜੇ_ਅੰਤ


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ wide_uint
# ਜੇ 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ __int128 wide_uint;
# ਨਹੀਂਤਾਂ
ਕਿਸਮਪਰਿਭਾਸ਼ਾ uintmax_t wide_uint;
# endif
#ਜੇ_ਅੰਤ

ਬਣਤਰ ਅਭਾਜ
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
ਸਥਿਰ wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  ਲਈ (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      ਜੇ (y == x)
        ਵਾਪਸ x;
      x = y;
    }
}

ਸਥਿਰ ਖਾਲੀ
process_ਅਭਾਜ (ਬਣਤਰ ਅਭਾਜ *info, ਬਿਨਾਨਿਸ਼ਾਨੀ p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

ਸਥਿਰ ਖਾਲੀ
print_wide_uint (wide_uint n, ਪੂਰਨ nesting, ਬਿਨਾਨਿਸ਼ਾਨੀ wide_uint_ਬਿੱਟ)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  ਪੂਰਨ hex_digits_ਪ੍ਰਤੀ_literal = 7;
  ਪੂਰਨ ਬਿੱਟ_ਪ੍ਰਤੀ_literal = hex_digits_ਪ੍ਰਤੀ_literal * 4;

  ਬਿਨਾਨਿਸ਼ਾਨੀ remainder = n & ((1 << ਬਿੱਟ_ਪ੍ਰਤੀ_literal) - 1);

  ਜੇ (n != remainder)
    {
      ਪੂਰਨ needs_parentheses = n >> ਬਿੱਟ_ਪ੍ਰਤੀ_literal >> ਬਿੱਟ_ਪ੍ਰਤੀ_literal != 0;
      ਜੇ (needs_parentheses)
        ਲਿਖੋ ("(");
      print_wide_uint (n >> ਬਿੱਟ_ਪ੍ਰਤੀ_literal, nesting + 1, wide_uint_ਬਿੱਟ);
      ਜੇ (needs_parentheses)
        ਲਿਖੋ (")\n%*s", nesting + 3, "");
      ਲਿਖੋ (" << %d | ", ਬਿੱਟ_ਪ੍ਰਤੀ_literal);
    }
  ਨਹੀਂਤਾਂ ਜੇ (nesting)
    {
      ਲਿਖੋ ("(mp_limb_t) ");
      hex_digits_ਪ੍ਰਤੀ_literal
        = ((wide_uint_ਬਿੱਟ - 1) % ਬਿੱਟ_ਪ੍ਰਤੀ_literal) % 4 + 1;
    }

  ਲਿਖੋ ("0x%0*xU", hex_digits_ਪ੍ਰਤੀ_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#ਜੇ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ
output_ਅਭਾਜ_ਸੰਖਿਆਵਾਂ (ਅਚਲ ਬਣਤਰ ਅਭਾਜ *ਅਭਾਜ_ਸੰਖਿਆਵਾਂ, ਬਿਨਾਨਿਸ਼ਾਨੀ nprimes)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ i;
  ਬਿਨਾਨਿਸ਼ਾਨੀ p;
  ਪੂਰਨ is_ਅਭਾਜ;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  ਬਿਨਾਨਿਸ਼ਾਨੀ wide_uint_ਬਿੱਟ = 0;
  wide_uint ਨਕਾਬ = -1;
  ਲਈ (wide_uint_ਬਿੱਟ = 0; ਨਕਾਬ; wide_uint_ਬਿੱਟ++)
    ਨਕਾਬ >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  ਲਿਖੋ ("#define WIDE_UINT_BITS %u\n", wide_uint_ਬਿੱਟ);

  ਲਈ (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      ਜੇ ((1 << (16 - 1)) <= ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[i].p)
        abort ();

      ਲਿਖੋ ("P (%u,\n   (", ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[i].p);
      print_wide_uint (ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[i].pinv, 0, wide_uint_ਬਿੱਟ);
      ਲਿਖੋ ("),\n   (mp_limb_t) -1 / %u)\n", ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[i].p);
    }

  /* Find next prime */
  p = ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[nprimes - 1].p;
  ਕਰੋ
    {
      p += 2;
      ਲਈ (i = 0, is_ਅਭਾਜ = 1; is_ਅਭਾਜ; i++)
        {
          ਜੇ (ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[i].p * ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[i].p > p)
            ਰੋਕੋ;
          ਜੇ (p * ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[i].pinv <= ਅਭਾਜ_ਸੰਖਿਆਵਾਂ[i].lim)
            {
              is_ਅਭਾਜ = 0;
              ਰੋਕੋ;
            }
        }
    }
  ਜਦੋਂ (!is_ਅਭਾਜ);

  ਲਿਖੋ ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
ਸਥਿਰ ਖਾਲੀ *
xalloc (size_t s)
{
  ਖਾਲੀ *p = ਸਮ੍ਰਿਤੀ_ਦਿਓ (s);
  ਜੇ (p)
    ਵਾਪਸ p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  ਬਾਹਰ_ਨਿਕਲੋ (EXIT_FAILURE);
}

ਪੂਰਨ
ਮੁੱਖ (ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
  ਪੂਰਨ ਹੱਦਬੰਦੀ;

  ਅੱਖਰ *sieve;
  size_t ਮਾਪ, i;

  ਬਣਤਰ ਅਭਾਜ *ਅਭਾਜ_list;
  ਬਿਨਾਨਿਸ਼ਾਨੀ nprimes;

  ਜੇ (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      ਵਾਪਸ EXIT_FAILURE;
    }
  ਹੱਦਬੰਦੀ = atoi (argv[1]);
  ਜੇ (ਹੱਦਬੰਦੀ < 3)
    ਵਾਪਸ EXIT_SUCCESS;

  /* Make limit odd */
  ਜੇ ( !(ਹੱਦਬੰਦੀ & 1))
    ਹੱਦਬੰਦੀ--;

  ਮਾਪ = (ਹੱਦਬੰਦੀ - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (ਮਾਪ);
  memset (sieve, 1, ਮਾਪ);

  ਅਭਾਜ_list = xalloc (ਮਾਪ * ਆਕਾਰ (*ਅਭਾਜ_list));
  nprimes = 0;

  ਲਈ (i = 0; i < ਮਾਪ;)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ p = 3 + 2 * i;
      ਬਿਨਾਨਿਸ਼ਾਨੀ j;

      process_ਅਭਾਜ (&ਅਭਾਜ_list[nprimes++], p);

      ਲਈ (j = (p * p - 3) / 2; j < ਮਾਪ; j += p)
        sieve[j] = 0;

      ਜਦੋਂ (++i < ਮਾਪ && sieve[i] == 0)
        ;
    }

  output_ਅਭਾਜ_ਸੰਖਿਆਵਾਂ (ਅਭਾਜ_list, nprimes);

  ਮੁਕਤ (sieve);
  ਮੁਕਤ (ਅਭਾਜ_list);

  ਜੇ (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      ਵਾਪਸ EXIT_FAILURE;
    }

  ਵਾਪਸ EXIT_SUCCESS;
}
