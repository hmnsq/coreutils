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
#ಆದರೆ 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#ಇಲ್ಲದಿದ್ದರೆ
# define ATTRIBUTE_CONST
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#ಇಲ್ಲದಿದ್ದರೆ
# define ATTRIBUTE_MALLOC
#ಆದರೆ_ಅಂತ್ಯ


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ wide_uint
# ಆದರೆ 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ __int128 wide_uint;
# ಇಲ್ಲದಿದ್ದರೆ
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uintmax_t wide_uint;
# endif
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ ಅವಿಭಾಜ್ಯ
{
  ಚಿಹ್ನೆರಹಿತ p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
ಸ್ಥಿರ wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  ಗಾಗಿ (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      ಆದರೆ (y == x)
        ಹಿಂತಿರುಗಿ x;
      x = y;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
process_ಅವಿಭಾಜ್ಯ (ರಚನೆ ಅವಿಭಾಜ್ಯ *info, ಚಿಹ್ನೆರಹಿತ p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

ಸ್ಥಿರ ಶೂನ್ಯ
print_wide_uint (wide_uint n, ಪೂರ್ಣಾಂಕ nesting, ಚಿಹ್ನೆರಹಿತ wide_uint_ಬಿಟ್ಟುಗಳು)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  ಪೂರ್ಣಾಂಕ hex_digits_ಪ್ರತಿ_literal = 7;
  ಪೂರ್ಣಾಂಕ ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_literal = hex_digits_ಪ್ರತಿ_literal * 4;

  ಚಿಹ್ನೆರಹಿತ remainder = n & ((1 << ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_literal) - 1);

  ಆದರೆ (n != remainder)
    {
      ಪೂರ್ಣಾಂಕ needs_parentheses = n >> ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_literal >> ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_literal != 0;
      ಆದರೆ (needs_parentheses)
        ಬರೆ ("(");
      print_wide_uint (n >> ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_literal, nesting + 1, wide_uint_ಬಿಟ್ಟುಗಳು);
      ಆದರೆ (needs_parentheses)
        ಬರೆ (")\n%*s", nesting + 3, "");
      ಬರೆ (" << %d | ", ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_literal);
    }
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (nesting)
    {
      ಬರೆ ("(mp_limb_t) ");
      hex_digits_ಪ್ರತಿ_literal
        = ((wide_uint_ಬಿಟ್ಟುಗಳು - 1) % ಬಿಟ್ಟುಗಳು_ಪ್ರತಿ_literal) % 4 + 1;
    }

  ಬರೆ ("0x%0*xU", hex_digits_ಪ್ರತಿ_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#ಆದರೆ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಶೂನ್ಯ
output_ಅವಿಭಾಜ್ಯಗಳು (ಸ್ಥಿರಾಂಕ ರಚನೆ ಅವಿಭಾಜ್ಯ *ಅವಿಭಾಜ್ಯಗಳು, ಚಿಹ್ನೆರಹಿತ nprimes)
{
  ಚಿಹ್ನೆರಹಿತ i;
  ಚಿಹ್ನೆರಹಿತ p;
  ಪೂರ್ಣಾಂಕ is_ಅವಿಭಾಜ್ಯ;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  ಚಿಹ್ನೆರಹಿತ wide_uint_ಬಿಟ್ಟುಗಳು = 0;
  wide_uint ಮುಖವಾಡ = -1;
  ಗಾಗಿ (wide_uint_ಬಿಟ್ಟುಗಳು = 0; ಮುಖವಾಡ; wide_uint_ಬಿಟ್ಟುಗಳು++)
    ಮುಖವಾಡ >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  ಬರೆ ("#define WIDE_UINT_BITS %u\n", wide_uint_ಬಿಟ್ಟುಗಳು);

  ಗಾಗಿ (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      ಆದರೆ ((1 << (16 - 1)) <= ಅವಿಭಾಜ್ಯಗಳು[i].p)
        abort ();

      ಬರೆ ("P (%u,\n   (", ಅವಿಭಾಜ್ಯಗಳು[i].p);
      print_wide_uint (ಅವಿಭಾಜ್ಯಗಳು[i].pinv, 0, wide_uint_ಬಿಟ್ಟುಗಳು);
      ಬರೆ ("),\n   (mp_limb_t) -1 / %u)\n", ಅವಿಭಾಜ್ಯಗಳು[i].p);
    }

  /* Find next prime */
  p = ಅವಿಭಾಜ್ಯಗಳು[nprimes - 1].p;
  ಮಾಡು
    {
      p += 2;
      ಗಾಗಿ (i = 0, is_ಅವಿಭಾಜ್ಯ = 1; is_ಅವಿಭಾಜ್ಯ; i++)
        {
          ಆದರೆ (ಅವಿಭಾಜ್ಯಗಳು[i].p * ಅವಿಭಾಜ್ಯಗಳು[i].p > p)
            ನಿಲ್ಲಿಸು;
          ಆದರೆ (p * ಅವಿಭಾಜ್ಯಗಳು[i].pinv <= ಅವಿಭಾಜ್ಯಗಳು[i].lim)
            {
              is_ಅವಿಭಾಜ್ಯ = 0;
              ನಿಲ್ಲಿಸು;
            }
        }
    }
  ಆಗ (!is_ಅವಿಭಾಜ್ಯ);

  ಬರೆ ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
ಸ್ಥಿರ ಶೂನ್ಯ *
xalloc (size_t s)
{
  ಶೂನ್ಯ *p = ಸ್ಮರಣೆ_ಕೊಡು (s);
  ಆದರೆ (p)
    ಹಿಂತಿರುಗಿ p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  ನಿರ್ಗಮಿಸು (EXIT_FAILURE);
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  ಪೂರ್ಣಾಂಕ ಎಲ್ಲೆ;

  ಅಕ್ಷರ *sieve;
  size_t ಗಾತ್ರ_2, i;

  ರಚನೆ ಅವಿಭಾಜ್ಯ *ಅವಿಭಾಜ್ಯ_list;
  ಚಿಹ್ನೆರಹಿತ nprimes;

  ಆದರೆ (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      ಹಿಂತಿರುಗಿ EXIT_FAILURE;
    }
  ಎಲ್ಲೆ = atoi (argv[1]);
  ಆದರೆ (ಎಲ್ಲೆ < 3)
    ಹಿಂತಿರುಗಿ EXIT_SUCCESS;

  /* Make limit odd */
  ಆದರೆ ( !(ಎಲ್ಲೆ & 1))
    ಎಲ್ಲೆ--;

  ಗಾತ್ರ_2 = (ಎಲ್ಲೆ - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (ಗಾತ್ರ_2);
  memset (sieve, 1, ಗಾತ್ರ_2);

  ಅವಿಭಾಜ್ಯ_list = xalloc (ಗಾತ್ರ_2 * ಗಾತ್ರ (*ಅವಿಭಾಜ್ಯ_list));
  nprimes = 0;

  ಗಾಗಿ (i = 0; i < ಗಾತ್ರ_2;)
    {
      ಚಿಹ್ನೆರಹಿತ p = 3 + 2 * i;
      ಚಿಹ್ನೆರಹಿತ j;

      process_ಅವಿಭಾಜ್ಯ (&ಅವಿಭಾಜ್ಯ_list[nprimes++], p);

      ಗಾಗಿ (j = (p * p - 3) / 2; j < ಗಾತ್ರ_2; j += p)
        sieve[j] = 0;

      ಆಗ (++i < ಗಾತ್ರ_2 && sieve[i] == 0)
        ;
    }

  output_ಅವಿಭಾಜ್ಯಗಳು (ಅವಿಭಾಜ್ಯ_list, nprimes);

  ಬಿಡುಗಡೆ (sieve);
  ಬಿಡುಗಡೆ (ಅವಿಭಾಜ್ಯ_list);

  ಆದರೆ (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      ಹಿಂತಿರುಗಿ EXIT_FAILURE;
    }

  ಹಿಂತಿರುಗಿ EXIT_SUCCESS;
}
