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

#شامل <inttypes.h>

#شامل <limits.h>
#شامل <stdint.h>
#شامل <stdio.h>
#شامل <string.h>
#شامل <stdlib.h>
#شامل <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#اگر 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#ورنہ
# define ATTRIBUTE_CONST
#اگر_ختم
#اگر 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#ورنہ
# define ATTRIBUTE_MALLOC
#اگر_ختم


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#اگر_غیر_تعریف_شدہ wide_uint
# اگر 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
قسمتعریف غیرعلامتی __int128 wide_uint;
# ورنہ
قسمتعریف uintmax_t wide_uint;
# endif
#اگر_ختم

ساخت مفرد
{
  غیرعلامتی p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
ساکن wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  برائے (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      اگر (y == x)
        واپس x;
      x = y;
    }
}

ساکن خالی
process_مفرد (ساخت مفرد *info, غیرعلامتی p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

ساکن خالی
print_wide_uint (wide_uint n, صحیح nesting, غیرعلامتی wide_uint_بٹ)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  صحیح hex_digits_فی_literal = 7;
  صحیح بٹ_فی_literal = hex_digits_فی_literal * 4;

  غیرعلامتی remainder = n & ((1 << بٹ_فی_literal) - 1);

  اگر (n != remainder)
    {
      صحیح needs_parentheses = n >> بٹ_فی_literal >> بٹ_فی_literal != 0;
      اگر (needs_parentheses)
        لکھو ("(");
      print_wide_uint (n >> بٹ_فی_literal, nesting + 1, wide_uint_بٹ);
      اگر (needs_parentheses)
        لکھو (")\n%*s", nesting + 3, "");
      لکھو (" << %d | ", بٹ_فی_literal);
    }
  ورنہ اگر (nesting)
    {
      لکھو ("(mp_limb_t) ");
      hex_digits_فی_literal
        = ((wide_uint_بٹ - 1) % بٹ_فی_literal) % 4 + 1;
    }

  لکھو ("0x%0*xU", hex_digits_فی_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#اگر 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#اگر_ختم

ساکن خالی
output_مفرد_اعداد (ثابت ساخت مفرد *مفرد_اعداد, غیرعلامتی nprimes)
{
  غیرعلامتی i;
  غیرعلامتی p;
  صحیح is_مفرد;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  غیرعلامتی wide_uint_بٹ = 0;
  wide_uint نقاب = -1;
  برائے (wide_uint_بٹ = 0; نقاب; wide_uint_بٹ++)
    نقاب >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  لکھو ("#define WIDE_UINT_BITS %u\n", wide_uint_بٹ);

  برائے (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      اگر ((1 << (16 - 1)) <= مفرد_اعداد[i].p)
        abort ();

      لکھو ("P (%u,\n   (", مفرد_اعداد[i].p);
      print_wide_uint (مفرد_اعداد[i].pinv, 0, wide_uint_بٹ);
      لکھو ("),\n   (mp_limb_t) -1 / %u)\n", مفرد_اعداد[i].p);
    }

  /* Find next prime */
  p = مفرد_اعداد[nprimes - 1].p;
  کرو
    {
      p += 2;
      برائے (i = 0, is_مفرد = 1; is_مفرد; i++)
        {
          اگر (مفرد_اعداد[i].p * مفرد_اعداد[i].p > p)
            توڑو;
          اگر (p * مفرد_اعداد[i].pinv <= مفرد_اعداد[i].lim)
            {
              is_مفرد = 0;
              توڑو;
            }
        }
    }
  جب (!is_مفرد);

  لکھو ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
ساکن خالی *
xalloc (size_t s)
{
  خالی *p = حافظہ_دو (s);
  اگر (p)
    واپس p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  باہر_نکلو (EXIT_FAILURE);
}

صحیح
مین (صحیح argc, حرف **argv)
{
  صحیح حد_بندی;

  حرف *sieve;
  size_t سائز, i;

  ساخت مفرد *مفرد_list;
  غیرعلامتی nprimes;

  اگر (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      واپس EXIT_FAILURE;
    }
  حد_بندی = atoi (argv[1]);
  اگر (حد_بندی < 3)
    واپس EXIT_SUCCESS;

  /* Make limit odd */
  اگر ( !(حد_بندی & 1))
    حد_بندی--;

  سائز = (حد_بندی - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (سائز);
  memset (sieve, 1, سائز);

  مفرد_list = xalloc (سائز * حجم (*مفرد_list));
  nprimes = 0;

  برائے (i = 0; i < سائز;)
    {
      غیرعلامتی p = 3 + 2 * i;
      غیرعلامتی j;

      process_مفرد (&مفرد_list[nprimes++], p);

      برائے (j = (p * p - 3) / 2; j < سائز; j += p)
        sieve[j] = 0;

      جب (++i < سائز && sieve[i] == 0)
        ;
    }

  output_مفرد_اعداد (مفرد_list, nprimes);

  آزاد (sieve);
  آزاد (مفرد_list);

  اگر (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      واپس EXIT_FAILURE;
    }

  واپس EXIT_SUCCESS;
}
