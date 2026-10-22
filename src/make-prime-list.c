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

#अन्तर्भाव <inttypes.h>

#अन्तर्भाव <limits.h>
#अन्तर्भाव <stdint.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#जर 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#अन्यथा
# define ATTRIBUTE_CONST
#endif
#जर 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#अन्यथा
# define ATTRIBUTE_MALLOC
#endif


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#ifndef wide_uint
# जर 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && अच_दीर्घ_उच्च >> 31 >> 31 >> 1 != 0
प्रकारव्याख्या अचिन्हित __int128 wide_uint;
# अन्यथा
प्रकारव्याख्या uintmax_t wide_uint;
# endif
#endif

संरचना मूळसंख्या
{
  अचिन्हित p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
स्थिर wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  साठी (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      जर (y == x)
        परत x;
      x = y;
    }
}

स्थिर शून्य
process_मूळसंख्या (संरचना मूळसंख्या *info, अचिन्हित p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

स्थिर शून्य
print_wide_uint (wide_uint n, पूर्णांक nesting, अचिन्हित wide_uint_bits)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  पूर्णांक hex_digits_per_literal = 7;
  पूर्णांक bits_per_literal = hex_digits_per_literal * 4;

  अचिन्हित remainder = n & ((1 << bits_per_literal) - 1);

  जर (n != remainder)
    {
      पूर्णांक needs_parentheses = n >> bits_per_literal >> bits_per_literal != 0;
      जर (needs_parentheses)
        म_लिखो ("(");
      print_wide_uint (n >> bits_per_literal, nesting + 1, wide_uint_bits);
      जर (needs_parentheses)
        म_लिखो (")\n%*s", nesting + 3, "");
      म_लिखो (" << %d | ", bits_per_literal);
    }
  अन्यथा जर (nesting)
    {
      म_लिखो ("(mp_limb_t) ");
      hex_digits_per_literal
        = ((wide_uint_bits - 1) % bits_per_literal) % 4 + 1;
    }

  म_लिखो ("0x%0*xU", hex_digits_per_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#जर 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

स्थिर शून्य
output_primes (स्थिरांक संरचना मूळसंख्या *primes, अचिन्हित nprimes)
{
  अचिन्हित i;
  अचिन्हित p;
  पूर्णांक is_मूळसंख्या;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  अचिन्हित wide_uint_bits = 0;
  wide_uint mask = -1;
  साठी (wide_uint_bits = 0; mask; wide_uint_bits++)
    mask >>= 1;

  माला_दो ("/* Generated file -- DO NOT EDIT */\n");
  म_लिखो ("#define WIDE_UINT_BITS %u\n", wide_uint_bits);

  साठी (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      जर ((1 << (16 - 1)) <= primes[i].p)
        पात ();

      म_लिखो ("P (%u,\n   (", primes[i].p);
      print_wide_uint (primes[i].pinv, 0, wide_uint_bits);
      म_लिखो ("),\n   (mp_limb_t) -1 / %u)\n", primes[i].p);
    }

  /* Find next prime */
  p = primes[nprimes - 1].p;
  करा
    {
      p += 2;
      साठी (i = 0, is_मूळसंख्या = 1; is_मूळसंख्या; i++)
        {
          जर (primes[i].p * primes[i].p > p)
            थांबा;
          जर (p * primes[i].pinv <= primes[i].lim)
            {
              is_मूळसंख्या = 0;
              थांबा;
            }
        }
    }
  पर्यंत (!is_मूळसंख्या);

  म_लिखो ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
स्थिर शून्य *
xalloc (माप_प्रकार s)
{
  शून्य *p = दो_स्मृति (s);
  जर (p)
    परत p;

  ख_लिखो (मानक_त्रुटि, "Virtual memory exhausted.\n");
  निकास (निकास_त्रुटि);
}

पूर्णांक
main (पूर्णांक argc, अक्षर **argv)
{
  पूर्णांक limit;

  अक्षर *sieve;
  माप_प्रकार size, i;

  संरचना मूळसंख्या *मूळसंख्या_list;
  अचिन्हित nprimes;

  जर (argc != 2)
    {
      ख_लिखो (मानक_त्रुटि, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      परत निकास_त्रुटि;
    }
  limit = म_से_प (argv[1]);
  जर (limit < 3)
    परत निकास_सफल;

  /* Make limit odd */
  जर ( !(limit & 1))
    limit--;

  size = (limit - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (size);
  स_रखो (sieve, 1, size);

  मूळसंख्या_list = xalloc (size * आकार (*मूळसंख्या_list));
  nprimes = 0;

  साठी (i = 0; i < size;)
    {
      अचिन्हित p = 3 + 2 * i;
      अचिन्हित j;

      process_मूळसंख्या (&मूळसंख्या_list[nprimes++], p);

      साठी (j = (p * p - 3) / 2; j < size; j += p)
        sieve[j] = 0;

      पर्यंत (++i < size && sieve[i] == 0)
        ;
    }

  output_primes (मूळसंख्या_list, nprimes);

  मुक्त (sieve);
  मुक्त (मूळसंख्या_list);

  जर (ख_त्रुटि (मानक_निकास) || ख_बंद (मानक_निकास))
    {
      ख_लिखो (मानक_त्रुटि, "write error: %s\n", म_त्रुटि (त्रुटि_सं));
      परत निकास_त्रुटि;
    }

  परत निकास_सफल;
}
