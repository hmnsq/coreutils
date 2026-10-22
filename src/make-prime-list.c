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
#پایان_اگر
#إذا 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#وإلا
# define ATTRIBUTE_MALLOC
#پایان_اگر


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#اگر_تعریف_نشده wide_uint
# إذا 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
تعريفنوع بیعلامت __int128 wide_uint;
# وإلا
تعريفنوع uintmax_t wide_uint;
# endif
#پایان_اگر

بنية اول
{
  بیعلامت p;
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
process_اول (بنية اول *info, بیعلامت p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

ساكن فراغ
print_wide_uint (wide_uint n, صحيح nesting, بیعلامت wide_uint_بیتها)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  صحيح hex_digits_در_هر_literal = 7;
  صحيح بیتها_در_هر_literal = hex_digits_در_هر_literal * 4;

  بیعلامت remainder = n & ((1 << بیتها_در_هر_literal) - 1);

  إذا (n != remainder)
    {
      صحيح needs_parentheses = n >> بیتها_در_هر_literal >> بیتها_در_هر_literal != 0;
      إذا (needs_parentheses)
        چاپ_کن ("(");
      print_wide_uint (n >> بیتها_در_هر_literal, nesting + 1, wide_uint_بیتها);
      إذا (needs_parentheses)
        چاپ_کن (")\n%*s", nesting + 3, "");
      چاپ_کن (" << %d | ", بیتها_در_هر_literal);
    }
  وإلا إذا (nesting)
    {
      چاپ_کن ("(mp_limb_t) ");
      hex_digits_در_هر_literal
        = ((wide_uint_بیتها - 1) % بیتها_در_هر_literal) % 4 + 1;
    }

  چاپ_کن ("0x%0*xU", hex_digits_در_هر_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#إذا 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#پایان_اگر

ساكن فراغ
output_اعداد_اول (ثابت بنية اول *اعداد_اول, بیعلامت nprimes)
{
  بیعلامت i;
  بیعلامت p;
  صحيح is_اول;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  بیعلامت wide_uint_بیتها = 0;
  wide_uint ماسک = -1;
  لكل (wide_uint_بیتها = 0; ماسک; wide_uint_بیتها++)
    ماسک >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  چاپ_کن ("#define WIDE_UINT_BITS %u\n", wide_uint_بیتها);

  لكل (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      إذا ((1 << (16 - 1)) <= اعداد_اول[i].p)
        abort ();

      چاپ_کن ("P (%u,\n   (", اعداد_اول[i].p);
      print_wide_uint (اعداد_اول[i].pinv, 0, wide_uint_بیتها);
      چاپ_کن ("),\n   (mp_limb_t) -1 / %u)\n", اعداد_اول[i].p);
    }

  /* Find next prime */
  p = اعداد_اول[nprimes - 1].p;
  افعل
    {
      p += 2;
      لكل (i = 0, is_اول = 1; is_اول; i++)
        {
          إذا (اعداد_اول[i].p * اعداد_اول[i].p > p)
            اكسر;
          إذا (p * اعداد_اول[i].pinv <= اعداد_اول[i].lim)
            {
              is_اول = 0;
              اكسر;
            }
        }
    }
  طالما (!is_اول);

  چاپ_کن ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
ساكن فراغ *
xalloc (size_t s)
{
  فراغ *p = حافظه_بده (s);
  إذا (p)
    أرجع p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  خروج (EXIT_FAILURE);
}

صحيح
اصلی (صحيح argc, حرف **argv)
{
  صحيح حد;

  حرف *sieve;
  size_t اندازه, i;

  بنية اول *اول_list;
  بیعلامت nprimes;

  إذا (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      أرجع EXIT_FAILURE;
    }
  حد = atoi (argv[1]);
  إذا (حد < 3)
    أرجع EXIT_SUCCESS;

  /* Make limit odd */
  إذا ( !(حد & 1))
    حد--;

  اندازه = (حد - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (اندازه);
  memset (sieve, 1, اندازه);

  اول_list = xalloc (اندازه * حجم (*اول_list));
  nprimes = 0;

  لكل (i = 0; i < اندازه;)
    {
      بیعلامت p = 3 + 2 * i;
      بیعلامت j;

      process_اول (&اول_list[nprimes++], p);

      لكل (j = (p * p - 3) / 2; j < اندازه; j += p)
        sieve[j] = 0;

      طالما (++i < اندازه && sieve[i] == 0)
        ;
    }

  output_اعداد_اول (اول_list, nprimes);

  حرر (sieve);
  حرر (اول_list);

  إذا (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      أرجع EXIT_FAILURE;
    }

  أرجع EXIT_SUCCESS;
}
