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

#అన్తర్భావ <inttypes.h>

#అన్తర్భావ <limits.h>
#అన్తర్భావ <stdint.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#ఒకవేళ 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#లేకపోతే
# define ATTRIBUTE_CONST
#అయితే_ముగింపు
#ఒకవేళ 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#లేకపోతే
# define ATTRIBUTE_MALLOC
#అయితే_ముగింపు


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#నిర్వచితం_కాకపోతే wide_uint
# ఒకవేళ 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
రకనిర్వచనం గుర్తులేని __int128 wide_uint;
# లేకపోతే
రకనిర్వచనం uintmax_t wide_uint;
# endif
#అయితే_ముగింపు

నిర్మాణం ప్రధానసంఖ్య
{
  గుర్తులేని p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
స్థిర wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  కొరకు (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      ఒకవేళ (y == x)
        తిరిగి x;
      x = y;
    }
}

స్థిర శూన్య
process_ప్రధానసంఖ్య (నిర్మాణం ప్రధానసంఖ్య *info, గుర్తులేని p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

స్థిర శూన్య
print_wide_uint (wide_uint n, పూర్ణాంకం nesting, గుర్తులేని wide_uint_బిట్లు)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  పూర్ణాంకం hex_digits_చొప్పున_literal = 7;
  పూర్ణాంకం బిట్లు_చొప్పున_literal = hex_digits_చొప్పున_literal * 4;

  గుర్తులేని remainder = n & ((1 << బిట్లు_చొప్పున_literal) - 1);

  ఒకవేళ (n != remainder)
    {
      పూర్ణాంకం needs_parentheses = n >> బిట్లు_చొప్పున_literal >> బిట్లు_చొప్పున_literal != 0;
      ఒకవేళ (needs_parentheses)
        రాయి ("(");
      print_wide_uint (n >> బిట్లు_చొప్పున_literal, nesting + 1, wide_uint_బిట్లు);
      ఒకవేళ (needs_parentheses)
        రాయి (")\n%*s", nesting + 3, "");
      రాయి (" << %d | ", బిట్లు_చొప్పున_literal);
    }
  లేకపోతే ఒకవేళ (nesting)
    {
      రాయి ("(mp_limb_t) ");
      hex_digits_చొప్పున_literal
        = ((wide_uint_బిట్లు - 1) % బిట్లు_చొప్పున_literal) % 4 + 1;
    }

  రాయి ("0x%0*xU", hex_digits_చొప్పున_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#ఒకవేళ 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#అయితే_ముగింపు

స్థిర శూన్య
output_ప్రధానసంఖ్యలు (స్థిరరాశి నిర్మాణం ప్రధానసంఖ్య *ప్రధానసంఖ్యలు, గుర్తులేని nprimes)
{
  గుర్తులేని i;
  గుర్తులేని p;
  పూర్ణాంకం is_ప్రధానసంఖ్య;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  గుర్తులేని wide_uint_బిట్లు = 0;
  wide_uint ముసుగు = -1;
  కొరకు (wide_uint_బిట్లు = 0; ముసుగు; wide_uint_బిట్లు++)
    ముసుగు >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  రాయి ("#define WIDE_UINT_BITS %u\n", wide_uint_బిట్లు);

  కొరకు (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      ఒకవేళ ((1 << (16 - 1)) <= ప్రధానసంఖ్యలు[i].p)
        abort ();

      రాయి ("P (%u,\n   (", ప్రధానసంఖ్యలు[i].p);
      print_wide_uint (ప్రధానసంఖ్యలు[i].pinv, 0, wide_uint_బిట్లు);
      రాయి ("),\n   (mp_limb_t) -1 / %u)\n", ప్రధానసంఖ్యలు[i].p);
    }

  /* Find next prime */
  p = ప్రధానసంఖ్యలు[nprimes - 1].p;
  చేయి
    {
      p += 2;
      కొరకు (i = 0, is_ప్రధానసంఖ్య = 1; is_ప్రధానసంఖ్య; i++)
        {
          ఒకవేళ (ప్రధానసంఖ్యలు[i].p * ప్రధానసంఖ్యలు[i].p > p)
            ఆపు;
          ఒకవేళ (p * ప్రధానసంఖ్యలు[i].pinv <= ప్రధానసంఖ్యలు[i].lim)
            {
              is_ప్రధానసంఖ్య = 0;
              ఆపు;
            }
        }
    }
  అయితే (!is_ప్రధానసంఖ్య);

  రాయి ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
స్థిర శూన్య *
xalloc (size_t s)
{
  శూన్య *p = జ్ఞాపకం_ఇవ్వు (s);
  ఒకవేళ (p)
    తిరిగి p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  నిష్క్రమించు (EXIT_FAILURE);
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  పూర్ణాంకం హద్దు;

  అక్షరం *sieve;
  size_t కొలత, i;

  నిర్మాణం ప్రధానసంఖ్య *ప్రధానసంఖ్య_list;
  గుర్తులేని nprimes;

  ఒకవేళ (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      తిరిగి EXIT_FAILURE;
    }
  హద్దు = atoi (argv[1]);
  ఒకవేళ (హద్దు < 3)
    తిరిగి EXIT_SUCCESS;

  /* Make limit odd */
  ఒకవేళ ( !(హద్దు & 1))
    హద్దు--;

  కొలత = (హద్దు - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (కొలత);
  memset (sieve, 1, కొలత);

  ప్రధానసంఖ్య_list = xalloc (కొలత * పరిమాణం (*ప్రధానసంఖ్య_list));
  nprimes = 0;

  కొరకు (i = 0; i < కొలత;)
    {
      గుర్తులేని p = 3 + 2 * i;
      గుర్తులేని j;

      process_ప్రధానసంఖ్య (&ప్రధానసంఖ్య_list[nprimes++], p);

      కొరకు (j = (p * p - 3) / 2; j < కొలత; j += p)
        sieve[j] = 0;

      అయితే (++i < కొలత && sieve[i] == 0)
        ;
    }

  output_ప్రధానసంఖ్యలు (ప్రధానసంఖ్య_list, nprimes);

  విడుదల (sieve);
  విడుదల (ప్రధానసంఖ్య_list);

  ఒకవేళ (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      తిరిగి EXIT_FAILURE;
    }

  తిరిగి EXIT_SUCCESS;
}
