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

#અન્તર્ભાવ <inttypes.h>

#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <stdint.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <errno.h>

/* This program is compiled in a separate directory to avoid linking to Gnulib
   which may be cross-compiled.  Therefore, we also do not have config.h and
   attribute.h.  Just define what we need.  */
#જો 2 < __GNUC__ + (95 <= __GNUC_MINOR__)
# define ATTRIBUTE_CONST __attribute__ ((__const__))
#નહીંતર
# define ATTRIBUTE_CONST
#endif
#જો 3 < __GNUC__
# define ATTRIBUTE_MALLOC __attribute__ ((__malloc__))
#નહીંતર
# define ATTRIBUTE_MALLOC
#endif


/* An unsigned type that is no narrower than 32 bits and no narrower
   than unsigned int.  It's best to make it as wide as possible.
   For GCC 4.6 and later, use a heuristic to guess whether unsigned
   __int128 works on your platform.  If this heuristic does not work
   for you, please report a bug; in the meantime compile with, e.g.,
   -Dwide_uint='unsigned __int128' to override the heuristic.  */
#ifndef wide_uint
# જો 4 < __GNUC__ + (6 <= __GNUC_MINOR__) && ULONG_MAX >> 31 >> 31 >> 1 != 0
પ્રકારવ્યાખ્યા unsigned __int128 wide_uint;
# નહીંતર
પ્રકારવ્યાખ્યા uintmax_t wide_uint;
# endif
#endif

સંરચના અવિભાજ્ય
{
  unsigned p;
  wide_uint pinv; /* Inverse mod b = 2^{bitsize of wide_uint} */
  wide_uint lim; /* floor ((wide_uint) -1 / p) */
};

ATTRIBUTE_CONST
સ્થિર wide_uint
binvert (wide_uint a)
{
  wide_uint x = 0xf5397db1 >> (4 * ((a / 2) & 0x7));
  માટે (;;)
    {
      wide_uint y = 2 * x - x * x * a;
      જો (y == x)
        પાછા x;
      x = y;
    }
}

સ્થિર શૂન્ય
process_અવિભાજ્ય (સંરચના અવિભાજ્ય *info, unsigned p)
{
  wide_uint max = -1;
  info->p = p;
  info->pinv = binvert (p);
  info->lim = max / p;
}

સ્થિર શૂન્ય
print_wide_uint (wide_uint n, પૂર્ણાંક nesting, unsigned wide_uint_bits)
{
  /* Number of bits per integer literal.  8 is too many, because
     uintmax_t is 32 bits on some machines so we cannot shift by 32 bits.
     So use 7.  */
  પૂર્ણાંક hex_digits_per_literal = 7;
  પૂર્ણાંક bits_per_literal = hex_digits_per_literal * 4;

  unsigned remainder = n & ((1 << bits_per_literal) - 1);

  જો (n != remainder)
    {
      પૂર્ણાંક needs_parentheses = n >> bits_per_literal >> bits_per_literal != 0;
      જો (needs_parentheses)
        printf ("(");
      print_wide_uint (n >> bits_per_literal, nesting + 1, wide_uint_bits);
      જો (needs_parentheses)
        printf (")\n%*s", nesting + 3, "");
      printf (" << %d | ", bits_per_literal);
    }
  નહીંતર જો (nesting)
    {
      printf ("(mp_limb_t) ");
      hex_digits_per_literal
        = ((wide_uint_bits - 1) % bits_per_literal) % 4 + 1;
    }

  printf ("0x%0*xU", hex_digits_per_literal, remainder);
}

/* Work around <https://gcc.gnu.org/PR109635>.  */
#જો 13 <= __GNUC__
# pragma GCC diagnostic ignored "-Wanalyzer-use-of-uninitialized-value"
#endif

સ્થિર શૂન્ય
output_primes (અચલ સંરચના અવિભાજ્ય *primes, unsigned nprimes)
{
  unsigned i;
  unsigned p;
  પૂર્ણાંક is_અવિભાજ્ય;

  /* Compute wide_uint_bits by repeated shifting, rather than by
     multiplying sizeof by CHAR_BIT, as this works even if the
     wide_uint representation has holes.  */
  unsigned wide_uint_bits = 0;
  wide_uint mask = -1;
  માટે (wide_uint_bits = 0; mask; wide_uint_bits++)
    mask >>= 1;

  puts ("/* Generated file -- DO NOT EDIT */\n");
  printf ("#define WIDE_UINT_BITS %u\n", wide_uint_bits);

  માટે (i = 0; i < nprimes; i++)
    {
      /* Check that primes[i].p fits in int_least16_t on all platforms,
         and that its square fits in int_least32_t on all platforms,
         as factor.c relies on this.  */
      જો ((1 << (16 - 1)) <= primes[i].p)
        abort ();

      printf ("P (%u,\n   (", primes[i].p);
      print_wide_uint (primes[i].pinv, 0, wide_uint_bits);
      printf ("),\n   (mp_limb_t) -1 / %u)\n", primes[i].p);
    }

  /* Find next prime */
  p = primes[nprimes - 1].p;
  કરો
    {
      p += 2;
      માટે (i = 0, is_અવિભાજ્ય = 1; is_અવિભાજ્ય; i++)
        {
          જો (primes[i].p * primes[i].p > p)
            તોડો;
          જો (p * primes[i].pinv <= primes[i].lim)
            {
              is_અવિભાજ્ય = 0;
              તોડો;
            }
        }
    }
  જ્યારે (!is_અવિભાજ્ય);

  printf ("#define SQUARE_OF_FIRST_OMITTED_PRIME %u\n", p * p);
}

ATTRIBUTE_MALLOC
સ્થિર શૂન્ય *
xalloc (size_t s)
{
  શૂન્ય *p = malloc (s);
  જો (p)
    પાછા p;

  fprintf (stderr, "Virtual memory exhausted.\n");
  exit (EXIT_FAILURE);
}

પૂર્ણાંક
મુખ્ય (પૂર્ણાંક argc, અક્ષર **argv)
{
  પૂર્ણાંક limit;

  અક્ષર *sieve;
  size_t size, i;

  સંરચના અવિભાજ્ય *અવિભાજ્ય_list;
  unsigned nprimes;

  જો (argc != 2)
    {
      fprintf (stderr, "Usage: %s LIMIT\n"
               "Produces a list of odd primes <= LIMIT\n", argv[0]);
      પાછા EXIT_FAILURE;
    }
  limit = atoi (argv[1]);
  જો (limit < 3)
    પાછા EXIT_SUCCESS;

  /* Make limit odd */
  જો ( !(limit & 1))
    limit--;

  size = (limit - 1) / 2;
  /* sieve[i] represents 3+2*i */
  sieve = xalloc (size);
  memset (sieve, 1, size);

  અવિભાજ્ય_list = xalloc (size * કદ (*અવિભાજ્ય_list));
  nprimes = 0;

  માટે (i = 0; i < size;)
    {
      unsigned p = 3 + 2 * i;
      unsigned j;

      process_અવિભાજ્ય (&અવિભાજ્ય_list[nprimes++], p);

      માટે (j = (p * p - 3) / 2; j < size; j += p)
        sieve[j] = 0;

      જ્યારે (++i < size && sieve[i] == 0)
        ;
    }

  output_primes (અવિભાજ્ય_list, nprimes);

  મુક્ત (sieve);
  મુક્ત (અવિભાજ્ય_list);

  જો (ferror (stdout) || fclose (stdout))
    {
      fprintf (stderr, "write error: %s\n", strerror (errno));
      પાછા EXIT_FAILURE;
    }

  પાછા EXIT_SUCCESS;
}
