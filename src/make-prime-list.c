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
#यदि 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#अन्यथा
# define ATTRIBUTE_CONST
#endif
#यदि 3 < __GNUC__
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
# यदि 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && अच_दीर्घ_उच्च >> 31 >> 31 >> 1 != 0
प्रकारनिर्देश अचिन्हित __int128 wide_uint;
# अन्यथा
प्रकारनिर्देश uintmax_t wide_uint;
# endif
#endif

संरचना अविभाज्य
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
  लागि (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      यदि (y == x)
        फर्काऊ x;
      x = y;
    }
}

स्थिर शून्य
process_अविभाज्य (संरचना अविभाज्य *info, अचिन्हित p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

स्थिर शून्य
print_wide_uint (wide_uint n, पूर्णाङ्क nesting, अचिन्हित wide_uint_bits)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  पूर्णाङ्क hex_digits_per_literal = 7;
  पूर्णाङ्क bits_per_literal = hex_digits_per_literal * 4;

  अचिन्हित remainder = n & ((1 << bits_per_literal) - 1);

  यदि (n != remainder)
    {
      पूर्णाङ्क needs_parentheses = n >> bits_per_literal >> bits_per_literal != 0;
      यदि (needs_parentheses)
        म_लिखो ("(");
      print_wide_uint (n >> bits_per_literal, nesting + 1, wide_uint_bits);
      यदि (needs_parentheses)
        म_लिखो (")\n%*s", nesting + 3, "");
      म_लिखो (" << %d | ", bits_per_literal);
    }
  अन्यथा यदि (nesting)
    {
      म_लिखो ("(mp_limb_t) ");
      hex_digits_per_literal
        = ((wide_uint_bits - 1) % bits_per_literal) % 4 + 1;
    }

  म_लिखो ("0x%0*xU", hex_digits_per_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#यदि 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

स्थिर शून्य
output_primes (अचर संरचना अविभाज्य *primes, अचिन्हित nprimes)
{
  अचिन्हित i;
  अचिन्हित p;
  पूर्णाङ्क is_अविभाज्य;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  अचिन्हित wide_uint_bits = 0;
  wide_uint mask = -1;
  लागि (wide_uint_bits = 0; mask; wide_uint_bits++)
    mask >>= 1;

  माला_दो ("/* Generated file -- DO NOT EDIT */\n");
  म_लिखो ("#define WIDE_UINT_BITS %u\n", wide_uint_bits);

  लागि (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      यदि ((1 << (16 - 1)) <= primes[i].p)
        पात ();

      म_लिखो ("P (%u,\n   (", primes[i].p);
      print_wide_uint (primes[i].pinv, 0, wide_uint_bits);
      म_लिखो ("),\n   (mp_limb_t) -1 / %u)\n", primes[i].p);
    }

  /* Find next prime */
  p = primes[nprimes - 1].p;
  गर
    {
      p += 2;
      लागि (i = 0, is_अविभाज्य = 1; is_अविभाज्य; i++)
        {
          यदि (primes[i].p * primes[i].p > p)
            रोक;
          यदि (p * primes[i].pinv <= primes[i].lim)
            {
              is_अविभाज्य = 0;
              रोक;
            }
        }
    }
  जबसम्म (!is_अविभाज्य);

  म_लिखो ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
स्थिर शून्य *
xalloc (माप_प्रकार s)
{
  शून्य *p = दो_स्मृति (s);
  यदि (p)
    फर्काऊ p;

  ख_लिखो (मानक_त्रुटि, "Virtual memory exhausted.\n");
  निकास (निकास_त्रुटि);
}

पूर्णाङ्क
main (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क limit;

  वर्ण *sieve;
  माप_प्रकार size, i;

  संरचना अविभाज्य *अविभाज्य_list;
  अचिन्हित nprimes;

  यदि (argc != 2)
    {
      ख_लिखो (मानक_त्रुटि, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      फर्काऊ निकास_त्रुटि;
    }
  limit = म_से_प (argv[1]);
  यदि (limit < 3)
    फर्काऊ निकास_सफल;

  /* Make limit odd */
  यदि ( !(limit & 1))
    limit--;

  size = (limit - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (size);
  स_रखो (sieve, 1, size);

  अविभाज्य_list = xalloc (size * परिमाण (*अविभाज्य_list));
  nprimes = 0;

  लागि (i = 0; i < size;)
    {
      अचिन्हित p = 3 + 2 * i;
      अचिन्हित j;

      process_अविभाज्य (&अविभाज्य_list[nprimes++], p);

      लागि (j = (p * p - 3) / 2; j < size; j += p)
        sieve[j] = 0;

      जबसम्म (++i < size && sieve[i] == 0)
        ;
    }

  output_primes (अविभाज्य_list, nprimes);

  मुक्त (sieve);
  मुक्त (अविभाज्य_list);

  यदि (ख_त्रुटि (मानक_निकास) || ख_बंद (मानक_निकास))
    {
      ख_लिखो (मानक_त्रुटि, "write error: %s\n", म_त्रुटि (त्रुटि_सं));
      फर्काऊ निकास_त्रुटि;
    }

  फर्काऊ निकास_सफल;
}
