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
#جيڪڏهن_پورو
#إذا 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#وإلا
# define ATTRIBUTE_MALLOC
#جيڪڏهن_پورو


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#جيڪڏهن_وصف_نه_ٿيل wide_uint
# إذا 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
تعريفنوع بنانشان __int128 wide_uint;
# وإلا
تعريفنوع uintmax_t wide_uint;
# endif
#جيڪڏهن_پورو

بنية مفرد
{
  بنانشان p;
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
process_مفرد (بنية مفرد *info, بنانشان p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

ساكن فراغ
print_wide_uint (wide_uint n, صحيح nesting, بنانشان wide_uint_بٽ)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  صحيح hex_digits_في_literal = 7;
  صحيح بٽ_في_literal = hex_digits_في_literal * 4;

  بنانشان remainder = n & ((1 << بٽ_في_literal) - 1);

  إذا (n != remainder)
    {
      صحيح needs_parentheses = n >> بٽ_في_literal >> بٽ_في_literal != 0;
      إذا (needs_parentheses)
        لکو ("(");
      print_wide_uint (n >> بٽ_في_literal, nesting + 1, wide_uint_بٽ);
      إذا (needs_parentheses)
        لکو (")\n%*s", nesting + 3, "");
      لکو (" << %d | ", بٽ_في_literal);
    }
  وإلا إذا (nesting)
    {
      لکو ("(mp_limb_t) ");
      hex_digits_في_literal
        = ((wide_uint_بٽ - 1) % بٽ_في_literal) % 4 + 1;
    }

  لکو ("0x%0*xU", hex_digits_في_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#إذا 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#جيڪڏهن_پورو

ساكن فراغ
output_مفرد_انگ (ثابت بنية مفرد *مفرد_انگ, بنانشان nprimes)
{
  بنانشان i;
  بنانشان p;
  صحيح is_مفرد;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  بنانشان wide_uint_بٽ = 0;
  wide_uint نقاب = -1;
  لكل (wide_uint_بٽ = 0; نقاب; wide_uint_بٽ++)
    نقاب >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  لکو ("#define WIDE_UINT_BITS %u\n", wide_uint_بٽ);

  لكل (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      إذا ((1 << (16 - 1)) <= مفرد_انگ[i].p)
        abort ();

      لکو ("P (%u,\n   (", مفرد_انگ[i].p);
      print_wide_uint (مفرد_انگ[i].pinv, 0, wide_uint_بٽ);
      لکو ("),\n   (mp_limb_t) -1 / %u)\n", مفرد_انگ[i].p);
    }

  /* Find next prime */
  p = مفرد_انگ[nprimes - 1].p;
  افعل
    {
      p += 2;
      لكل (i = 0, is_مفرد = 1; is_مفرد; i++)
        {
          إذا (مفرد_انگ[i].p * مفرد_انگ[i].p > p)
            اكسر;
          إذا (p * مفرد_انگ[i].pinv <= مفرد_انگ[i].lim)
            {
              is_مفرد = 0;
              اكسر;
            }
        }
    }
  طالما (!is_مفرد);

  لکو ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
ساكن فراغ *
xalloc (size_t s)
{
  فراغ *p = يادگيري_ڏيو (s);
  إذا (p)
    أرجع p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  ٻاهر_نڪرو (EXIT_FAILURE);
}

صحيح
مکيه (صحيح argc, حرف **argv)
{
  صحيح حد_بندي;

  حرف *sieve;
  size_t سائيز, i;

  بنية مفرد *مفرد_list;
  بنانشان nprimes;

  إذا (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      أرجع EXIT_FAILURE;
    }
  حد_بندي = atoi (argv[1]);
  إذا (حد_بندي < 3)
    أرجع EXIT_SUCCESS;

  /* Make limit odd */
  إذا ( !(حد_بندي & 1))
    حد_بندي--;

  سائيز = (حد_بندي - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (سائيز);
  memset (sieve, 1, سائيز);

  مفرد_list = xalloc (سائيز * حجم (*مفرد_list));
  nprimes = 0;

  لكل (i = 0; i < سائيز;)
    {
      بنانشان p = 3 + 2 * i;
      بنانشان j;

      process_مفرد (&مفرد_list[nprimes++], p);

      لكل (j = (p * p - 3) / 2; j < سائيز; j += p)
        sieve[j] = 0;

      طالما (++i < سائيز && sieve[i] == 0)
        ;
    }

  output_مفرد_انگ (مفرد_list, nprimes);

  حرر (sieve);
  حرر (مفرد_list);

  إذا (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      أرجع EXIT_FAILURE;
    }

  أرجع EXIT_SUCCESS;
}
