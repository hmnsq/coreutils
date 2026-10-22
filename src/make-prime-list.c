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
#यदि_अन्तः
#यदि 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#अन्यथा
# define ATTRIBUTE_MALLOC
#यदि_अन्तः


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#यदि_अपरिभाषितम् wide_uint
# यदि 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
प्रकारनिर्देश अचिह्नित __int128 wide_uint;
# अन्यथा
प्रकारनिर्देश uintmax_t wide_uint;
# endif
#यदि_अन्तः

संरचना अविभाज्यः
{
  अचिह्नित p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
स्थिर wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  प्रति (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      यदि (y == x)
        प्रत्यागम x;
      x = y;
    }
}

स्थिर शून्य
process_अविभाज्यः (संरचना अविभाज्यः *info, अचिह्नित p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

स्थिर शून्य
print_wide_uint (wide_uint n, पूर्णाङ्क nesting, अचिह्नित wide_uint_द्वयङ्काः)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  पूर्णाङ्क hex_digits_प्रत्येकम्_literal = 7;
  पूर्णाङ्क द्वयङ्काः_प्रत्येकम्_literal = hex_digits_प्रत्येकम्_literal * 4;

  अचिह्नित remainder = n & ((1 << द्वयङ्काः_प्रत्येकम्_literal) - 1);

  यदि (n != remainder)
    {
      पूर्णाङ्क needs_parentheses = n >> द्वयङ्काः_प्रत्येकम्_literal >> द्वयङ्काः_प्रत्येकम्_literal != 0;
      यदि (needs_parentheses)
        प्रारूप_लिख ("(");
      print_wide_uint (n >> द्वयङ्काः_प्रत्येकम्_literal, nesting + 1, wide_uint_द्वयङ्काः);
      यदि (needs_parentheses)
        प्रारूप_लिख (")\n%*s", nesting + 3, "");
      प्रारूप_लिख (" << %d | ", द्वयङ्काः_प्रत्येकम्_literal);
    }
  अन्यथा यदि (nesting)
    {
      प्रारूप_लिख ("(mp_limb_t) ");
      hex_digits_प्रत्येकम्_literal
        = ((wide_uint_द्वयङ्काः - 1) % द्वयङ्काः_प्रत्येकम्_literal) % 4 + 1;
    }

  प्रारूप_लिख ("0x%0*xU", hex_digits_प्रत्येकम्_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#यदि 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#यदि_अन्तः

स्थिर शून्य
output_अभाज्याः (ध्रुव संरचना अविभाज्यः *अभाज्याः, अचिह्नित nprimes)
{
  अचिह्नित i;
  अचिह्नित p;
  पूर्णाङ्क is_अविभाज्यः;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  अचिह्नित wide_uint_द्वयङ्काः = 0;
  wide_uint आवरणम् = -1;
  प्रति (wide_uint_द्वयङ्काः = 0; आवरणम्; wide_uint_द्वयङ्काः++)
    आवरणम् >>= 1;

  पङ्क्तिं_लिख ("/* Generated file -- DO NOT EDIT */\n");
  प्रारूप_लिख ("#define WIDE_UINT_BITS %u\n", wide_uint_द्वयङ्काः);

  प्रति (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      यदि ((1 << (16 - 1)) <= अभाज्याः[i].p)
        abort ();

      प्रारूप_लिख ("P (%u,\n   (", अभाज्याः[i].p);
      print_wide_uint (अभाज्याः[i].pinv, 0, wide_uint_द्वयङ्काः);
      प्रारूप_लिख ("),\n   (mp_limb_t) -1 / %u)\n", अभाज्याः[i].p);
    }

  /* Find next prime */
  p = अभाज्याः[nprimes - 1].p;
  कुरु
    {
      p += 2;
      प्रति (i = 0, is_अविभाज्यः = 1; is_अविभाज्यः; i++)
        {
          यदि (अभाज्याः[i].p * अभाज्याः[i].p > p)
            भङ्ग;
          यदि (p * अभाज्याः[i].pinv <= अभाज्याः[i].lim)
            {
              is_अविभाज्यः = 0;
              भङ्ग;
            }
        }
    }
  यावत् (!is_अविभाज्यः);

  प्रारूप_लिख ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
स्थिर शून्य *
xalloc (size_t s)
{
  शून्य *p = स्मृतिं_देहि (s);
  यदि (p)
    प्रत्यागम p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  निर्गच्छ (EXIT_FAILURE);
}

पूर्णाङ्क
मुख्यम् (पूर्णाङ्क argc, वर्ण **argv)
{
  पूर्णाङ्क सीमा;

  वर्ण *sieve;
  size_t आकार, i;

  संरचना अविभाज्यः *अविभाज्यः_list;
  अचिह्नित nprimes;

  यदि (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      प्रत्यागम EXIT_FAILURE;
    }
  सीमा = atoi (argv[1]);
  यदि (सीमा < 3)
    प्रत्यागम EXIT_SUCCESS;

  /* Make limit odd */
  यदि ( !(सीमा & 1))
    सीमा--;

  आकार = (सीमा - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (आकार);
  स्मृतिं_पूरय (sieve, 1, आकार);

  अविभाज्यः_list = xalloc (आकार * परिमाण (*अविभाज्यः_list));
  nprimes = 0;

  प्रति (i = 0; i < आकार;)
    {
      अचिह्नित p = 3 + 2 * i;
      अचिह्नित j;

      process_अविभाज्यः (&अविभाज्यः_list[nprimes++], p);

      प्रति (j = (p * p - 3) / 2; j < आकार; j += p)
        sieve[j] = 0;

      यावत् (++i < आकार && sieve[i] == 0)
        ;
    }

  output_अभाज्याः (अविभाज्यः_list, nprimes);

  मुक्त (sieve);
  मुक्त (अविभाज्यः_list);

  यदि (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      प्रत्यागम EXIT_FAILURE;
    }

  प्रत्यागम EXIT_SUCCESS;
}
