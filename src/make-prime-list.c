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
#د_که_پای
#إذا 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#وإلا
# define ATTRIBUTE_MALLOC
#د_که_پای


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#که_نه_تعریف_شوی wide_uint
# إذا 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
تعريفنوع بینښه __int128 wide_uint;
# وإلا
تعريفنوع uintmax_t wide_uint;
# endif
#د_که_پای

بنية اولي
{
  بینښه p;
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
process_اولي (بنية اولي *info, بینښه p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

ساكن فراغ
print_wide_uint (wide_uint n, صحيح nesting, بینښه wide_uint_بټونه)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  صحيح hex_digits_فی_literal = 7;
  صحيح بټونه_فی_literal = hex_digits_فی_literal * 4;

  بینښه remainder = n & ((1 << بټونه_فی_literal) - 1);

  إذا (n != remainder)
    {
      صحيح needs_parentheses = n >> بټونه_فی_literal >> بټونه_فی_literal != 0;
      إذا (needs_parentheses)
        چاپ_کړه ("(");
      print_wide_uint (n >> بټونه_فی_literal, nesting + 1, wide_uint_بټونه);
      إذا (needs_parentheses)
        چاپ_کړه (")\n%*s", nesting + 3, "");
      چاپ_کړه (" << %d | ", بټونه_فی_literal);
    }
  وإلا إذا (nesting)
    {
      چاپ_کړه ("(mp_limb_t) ");
      hex_digits_فی_literal
        = ((wide_uint_بټونه - 1) % بټونه_فی_literal) % 4 + 1;
    }

  چاپ_کړه ("0x%0*xU", hex_digits_فی_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#إذا 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#د_که_پای

ساكن فراغ
output_لومړني_عددونه (ثابت بنية اولي *لومړني_عددونه, بینښه nprimes)
{
  بینښه i;
  بینښه p;
  صحيح is_اولي;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  بینښه wide_uint_بټونه = 0;
  wide_uint ماسک = -1;
  لكل (wide_uint_بټونه = 0; ماسک; wide_uint_بټونه++)
    ماسک >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  چاپ_کړه ("#define WIDE_UINT_BITS %u\n", wide_uint_بټونه);

  لكل (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      إذا ((1 << (16 - 1)) <= لومړني_عددونه[i].p)
        abort ();

      چاپ_کړه ("P (%u,\n   (", لومړني_عددونه[i].p);
      print_wide_uint (لومړني_عددونه[i].pinv, 0, wide_uint_بټونه);
      چاپ_کړه ("),\n   (mp_limb_t) -1 / %u)\n", لومړني_عددونه[i].p);
    }

  /* Find next prime */
  p = لومړني_عددونه[nprimes - 1].p;
  افعل
    {
      p += 2;
      لكل (i = 0, is_اولي = 1; is_اولي; i++)
        {
          إذا (لومړني_عددونه[i].p * لومړني_عددونه[i].p > p)
            اكسر;
          إذا (p * لومړني_عددونه[i].pinv <= لومړني_عددونه[i].lim)
            {
              is_اولي = 0;
              اكسر;
            }
        }
    }
  طالما (!is_اولي);

  چاپ_کړه ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
ساكن فراغ *
xalloc (size_t s)
{
  فراغ *p = حافظه_ورکړه (s);
  إذا (p)
    أرجع p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  وتل (EXIT_FAILURE);
}

صحيح
اصلي (صحيح argc, حرف **argv)
{
  صحيح حد;

  حرف *sieve;
  size_t اندازه, i;

  بنية اولي *اولي_list;
  بینښه nprimes;

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

  اولي_list = xalloc (اندازه * حجم (*اولي_list));
  nprimes = 0;

  لكل (i = 0; i < اندازه;)
    {
      بینښه p = 3 + 2 * i;
      بینښه j;

      process_اولي (&اولي_list[nprimes++], p);

      لكل (j = (p * p - 3) / 2; j < اندازه; j += p)
        sieve[j] = 0;

      طالما (++i < اندازه && sieve[i] == 0)
        ;
    }

  output_لومړني_عددونه (اولي_list, nprimes);

  حرر (sieve);
  حرر (اولي_list);

  إذا (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      أرجع EXIT_FAILURE;
    }

  أرجع EXIT_SUCCESS;
}
