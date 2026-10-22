/* factor -- print prime factors of n.
   Copyright (C) 1986-2026 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* Originally written by Paul Rubin <phr@ocf.berkeley.edu>.
   Adapted for GNU, fixed to factor UINT_MAX by Jim Meyering.
   Arbitrary-precision code adapted by James Youngman from Torbjörn
   Granlund's factorize.c, from GNU MP version 4.2.2.
   In 2012, the core was rewritten by Torbjörn Granlund and Niels Möller.
   In 2025, Torbjörn Granlund and Paul Eggert sped up primality checking,
   and improved performance on composite numbers greater than 2^128.
   Contains code from GNU MP.  */

/* Efficiently factor numbers that fit in one or two words (word = mp_limb_t),
   or, with GMP, numbers of any size.

  Code organization:

    There are several variants of many functions, for handling one word, two
    words, and GMP's mpz_t type.  If the one-word variant is called foo, the
    two-word variant will be foo2, and the one for mpz_t will be mp_foo.  In
    some cases, the plain function variants will handle both one-word and
    two-word numbers, evidenced by function arguments.

    The factoring code for two words will fall into the code for one word when
    progress allows that.

  Algorithm:

    (1) Perform trial division using a small primes table, but without hardware
        division since the primes table store inverses modulo the word base.
        (The GMP variant of this code doesn't make use of the precomputed
        inverses, but instead relies on GMP for fast divisibility testing.)
    (2) Check the nature of any non-factored part using Baillie-PSW.
    (3) Factor any remaining composite part using Pollard-Brent rho.
        Status of found factors are checked again using Baillie-PSW.

    Prefer using Hensel norm in the divisions, not the more familiar
    Euclidean norm, since the former leads to much faster code.  In the
    Pollard-Brent rho code, use Montgomery's trick of multiplying
    all n-residues by the word base, allowing cheap Hensel reductions mod n.

    The GMP code uses an algorithm that can be considerably slower.

  Improvements:

    * Use modular inverses also for exact division.  A problem is to
      locate the inverses not from an index, but from a prime.
      We might instead compute the inverse on-the-fly.

    * Tune trial division table size (not forgetting that this is a standalone
      program where the table will be read from secondary storage for
      each invocation).

    * Try to speed trial division code for single word numbers, i.e., the
      code using divblock.  It currently runs at 2 cycles per prime (Intel SBR,
      IBR), 3 cycles per prime (AMD Stars) and 5 cycles per prime (AMD BD) when
      using gcc 4.6 and 4.7.  Some software pipelining should help; 1, 2, and 4
      respectively cycles ought to be possible.

    * The redcify function could be vastly improved by using (plain Euclidean)
      pre-inversion (such as GMP's invert_limb) and udiv_qrnnd_preinv (from
      GMP's gmp-impl.h).  The redcify2 function could be vastly improved using
      similar methods.
*/


#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <getopt.h>
#ಅನ್ತರ್ಭಾವ <stdbit.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <gmp.h>

#ಅನ್ತರ್ಭಾವ "system.h"
#ಅನ್ತರ್ಭಾವ "assure.h"
#ಅನ್ತರ್ಭಾವ "full-write.h"
#ಅನ್ತರ್ಭಾವ "quote.h"
#ಅನ್ತರ್ಭಾವ "readtokens.h"
#ಅನ್ತರ್ಭಾವ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#ವ್ಯಾಖ್ಯಾನಿಸು ಕಾರ್ಯಕ್ರಮ_NAME "factor"

#ವ್ಯಾಖ್ಯಾನಿಸು AUTHORS \
  proper_name ("Paul Rubin"),                                           \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"),   \
  proper_name_lite ("Niels Moller", "Niels M\303\266ller")

/* Token delimiters when reading from a file.  */
ಸ್ಥಿರ ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ DELIM[] = "\n\t ";

/* GMP uses the unsigned integer type mp_limb_t as its word in
   multiprecision arithmetic.  This code uses the same word for single
   and double precision integer arithmetic.  Although previous
   versions of this code used uintmax_t for single and double
   precision, that introduced opportunities for bugs and was not worth
   the hassle, as mp_limb_t and uintmax_t are invariably the same on
   64-bit platforms, and 32-bit platforms are less important now.

   Although GMP can be built with GMP_NUMB_BITS < GMP_LIMB_BITS,
   so that some high-order bits of a word are not used, do not
   do this in single and double precision integer arithmetic.
   Instead, always use the full word.  */

/* The word size in bits.
   In the rest of this file's comments, B = 2^W_TYPE_SIZE is the base,
   and the notation (a1,a0) stands for B*a1 + a0.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GMP_LIMB_ಬಿಟ್ಟುಗಳು
# define W_TYPE_ಗಾತ್ರ GMP_LIMB_ಬಿಟ್ಟುಗಳು
#ಇಲ್ಲದಿದ್ದರೆ
/* An older GMP, or mini-gmp; guess the usual value.  */
# define W_TYPE_ಗಾತ್ರ ULONG_ಅಗಲ
#ಆದರೆ_ಅಂತ್ಯ

/* The maximum value of a word.  */
#ವ್ಯಾಖ್ಯಾನಿಸು MP_LIMB_MAX ((mp_limb_t) -1)

/* Check W_TYPE_SIZE's value, as it might be a guess.  */
static_assert (MP_LIMB_MAX >> (W_TYPE_ಗಾತ್ರ - 1) == 1);

/* Check that the builder didn't specify something perverse like
   "-DMINI_GMP_LIMB_TYPE=short -DW_TYPE_SIZE=USHRT_WIDTH".
   This could result in undefined behavior due to signed integer
   overflow if a word promotes to int.  */
static_assert (ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ < MP_LIMB_MAX);

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ USE_LONGLONG_H
# ಆದರೆ (defined INT32_MAX && defined UINT32_MAX \
      && defined INT64_MAX && defined UINT64_MAX)
#  define USE_LONGLONG_H true
# ಇಲ್ಲದಿದ್ದರೆ
#  define USE_LONGLONG_H false
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ USE_LONGLONG_H

/* Make definitions for longlong.h to make it do what it can do for us */

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ mp_limb_t UWtype;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ UHWtype;
# undef UDWtype
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ int32_t SItype;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint32_t USItype;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ int64_t DItype;
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ uint64_t UDItype;
# define LONGLONG_STANDALONE     /* Don't require GMP's longlong.h mdep files */

/* FIXME make longlong.h really standalone, so that ASSERT, __GMP_DECLSPEC
   and __GMP_GNUC_PREREQ need not be defined here.  */
# define ASSERT(x)
# define __GMP_DECLSPEC
# ifndef __GMP_GNUC_PREREQ
#  ಆದರೆ defined __GNUC__ && defined __GNUC_MINOR__
#   define __GMP_GNUC_PREREQ(a, b) ((a) < __GNUC__ + ((b) <= __GNUC_MINOR__))
#  ಇಲ್ಲದಿದ್ದರೆ
#   define __GMP_GNUC_PREREQ(a, b) false
#  endif
# endif

/* longlong.h uses these macros only in certain system compiler combinations.
   Ensure usage to pacify -Wunused-macros.  */
# ಆದರೆ defined ASSERT || defined __GMP_DECLSPEC || defined __GMP_GNUC_PREREQ
# endif

/* longlong.h uses casts even when useless.  */
# ಆದರೆ 14 <= __GNUC__
#  pragma GCC diagnostic ignored "-Wuseless-cast"
# endif

# ಆದರೆ _ARCH_PPC
#  define HAVE_HOST_CPU_FAMILY_powerpc 1
# endif
# include "longlong.h"
#ಆದರೆ_ಅಂತ್ಯ

/* 2*3*5*7*11...*101 fits in 128 bits, and has 26 prime factors.
   This code can be extended in the future as needed; show as an example
   2*3*5*7*11...*193 which fits in 257 bits, and has 44 prime factors.  */
#ಆದರೆ 2 * W_TYPE_ಗಾತ್ರ <= 128
ಎಣಿಕೆ { MAX_NFACTS = 26 };
#ಅಥವಾಆದರೆ 2 * W_TYPE_ಗಾತ್ರ <= 257
ಎಣಿಕೆ { MAX_NFACTS = 44 };
#ಇಲ್ಲದಿದ್ದರೆ
# error "configuration has a wide word; please update MAX_NFACTS definition"
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1
};

ಸ್ಥಿರ ರಚನೆ option ಸ್ಥಿರಾಂಕ long_options[] =
{
  {"exponents", no_ವಾದ, ಖಾಲಿ, 'h'},
  {"-debug", no_ವಾದ, ಖಾಲಿ, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_ಆವೃತ್ತಿ_OPTION_DECL},
  {ಖಾಲಿ, 0, ಖಾಲಿ, 0}
};

/* If true, use p^e output format.  */
ಸ್ಥಿರ bool print_exponents;

/* This represents an unsigned integer twice as wide as a word.  */
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ { mp_limb_t uu[2]; } uuint;

/* Accessors and constructors for the type.  Programs should not
   access the type's internals directly, in case some future version
   replaces the type with unsigned __int256 or whatever.  */
ಸ್ಥಿರ mp_limb_t lo (uuint u) { ಹಿಂತಿರುಗಿ u.uu[0]; }
ಸ್ಥಿರ mp_limb_t hi (uuint u) { ಹಿಂತಿರುಗಿ u.uu[1]; }
ಸ್ಥಿರ ಶೂನ್ಯ hiset (uuint *u, mp_limb_t hi) { u->uu[1] = hi; }
ಸ್ಥಿರ bool hi_is_ಗಣ (uuint ಸ್ಥಿರಾಂಕ *pu) { ಹಿಂತಿರುಗಿ pu->uu[1] != 0; }
ಸ್ಥಿರ ಶೂನ್ಯ
uuset (mp_limb_t *phi, mp_limb_t *plo, uuint uu)
{
  *phi = hi (uu);
  *plo = lo (uu);
}
ಸ್ಥಿರ uuint
make_uuint (mp_limb_t hi, mp_limb_t lo)
{
  ಹಿಂತಿರುಗಿ (uuint) {{lo, hi}};
}

/* BIG_POWER_OF_10 is a positive power of 10 that fits in a word.
   The larger it is, the more efficient the code will likely be.
   LOG_BIG_POWER_OF_10 = log (BIG_POWER_OF_10).  */
#ಆದರೆ W_TYPE_ಗಾತ್ರ < 4
# error "Configuration error; platform word is impossibly narrow"
#ಅಥವಾಆದರೆ W_TYPE_ಗಾತ್ರ < 30
/* An unusually narrow word.  */
ಸ್ಥಿರ mp_limb_t ಸ್ಥಿರಾಂಕ BIG_POWER_ದ_10 = 10;
ಎಣಿಕೆ { LOG_BIG_POWER_ದ_10 = 1 };
#ಅಥವಾಆದರೆ W_TYPE_ಗಾತ್ರ < 64
/* Almost surely a 32-bit word.  */
ಸ್ಥಿರ mp_limb_t ಸ್ಥಿರಾಂಕ BIG_POWER_ದ_10 = 1000000000;
ಎಣಿಕೆ { LOG_BIG_POWER_ದ_10 = 9 };
#ಅಥವಾಆದರೆ W_TYPE_ಗಾತ್ರ < 128
/* Almost surely a 64-bit word.  */
ಸ್ಥಿರ mp_limb_t ಸ್ಥಿರಾಂಕ BIG_POWER_ದ_10 = 10000000000000000000llu;
ಎಣಿಕೆ { LOG_BIG_POWER_ದ_10 = 19 };
#ಇಲ್ಲದಿದ್ದರೆ
/* An unusually wide word.  */
ಸ್ಥಿರ mp_limb_t ಸ್ಥಿರಾಂಕ BIG_POWER_ದ_10 =
  (mp_limb_t) 10000000000000000000llu * 10000000000000000000llu;
ಎಣಿಕೆ { LOG_BIG_POWER_ದ_10 = 38 };
#ಆದರೆ_ಅಂತ್ಯ

/* Check that struct factors can use unsigned char to record a uuint's
   prime factor's multiplicity, which is at most 2 * W_TYPE_SIZE - 1.  */
static_assert (2 * W_TYPE_ಗಾತ್ರ - 1 <= UCHAR_MAX);

/* Likewise for recording the number of prime factors.  */
static_assert (MAX_NFACTS <= UCHAR_MAX);

/* Prime factors of a uuint.  At most one is a uuint.  */
ರಚನೆ factors
{
  /* If PLARGE.uu[1] is nonzero, PLARGE is a double-word prime factor
     with multiplicity 1; otherwise, PLARGE is not a factor and
     PLARGE.uu[0] might not be initialized.  */
  uuint plarge;

  /* Distinct single-word prime factors, their multiplicities,
     and their number.  */
  mp_limb_t     p[MAX_NFACTS];
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ e[MAX_NFACTS];
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ nfactors;
};

/* An mpz's prime factor and multiplicity.  */
ರಚನೆ mp_ಗುಣಕ
{
  mpz_t p;
  mp_bitcnt_t e;
};

/* Prime factors of an mpz_t.  */
ರಚನೆ mp_factors
{
  idx_t nfactors;
  idx_t nalloc;

  /* A vector of distinct prime factors, a count of the factors,
     and the number of allocated slots in the vector.  */
  ರಚನೆ mp_ಗುಣಕ *f COUNTED_BY (nfactors);
};

ಸ್ಥಿರ ಶೂನ್ಯ ಗುಣಕ (ರಚನೆ factors *, mp_limb_t, mp_limb_t);
ಸ್ಥಿರ ಶೂನ್ಯ ಗುಣಕ_ಮೇಲೆ (ರಚನೆ factors *, mp_limb_t, mp_limb_t, idx_t);

/* Set (w1,w0) = u * v.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ umul_ppmm
/* Speed things up if there is an unsigned type uuroom_t that is wide
   enough to hold two words.  */
# ಆದರೆ W_TYPE_ಗಾತ್ರ <= UINTMAX_ಅಗಲ / 2
#  define uuroom_t uintmax_t
# elif W_TYPE_ಗಾತ್ರ <= 64 && defined __SIZEOF_INT128__
#  define uuroom_t ಚಿಹ್ನೆರಹಿತ __int128
# endif
# ifdef uuroom_t
#  define umul_ppmm(w1, w0, u, v)		\
    ಮಾಡು {					\
      uuroom_t _u = u, _w = _u * (v);		\
      (w1) = _w >> W_TYPE_ಗಾತ್ರ;			\
      (w0) = _w;				\
    } ಆಗ (false)
# endif
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ umul_ppmm
ಸ್ಥಿರ mp_limb_t ಸ್ಥಿರಾಂಕ __ll_B = (mp_limb_t) 1 << (W_TYPE_ಗಾತ್ರ / 2);
ಸ್ಥಿರ mp_limb_t __ll_lowpart (mp_limb_t t) { ಹಿಂತಿರುಗಿ t & (__ll_B - 1); }
ಸ್ಥಿರ mp_limb_t __ll_highpart (mp_limb_t t) { ಹಿಂತಿರುಗಿ t >> (W_TYPE_ಗಾತ್ರ / 2); }
# define umul_ppmm(w1, w0, u, v)                                        \
  ಮಾಡು {                                                                  \
    mp_limb_t __u = u, __v = v,						\
                                                                        \
      __ul = __ll_lowpart (__u),					\
      __uh = __ll_highpart (__u),					\
      __vl = __ll_lowpart (__v),					\
      __vh = __ll_highpart (__v),					\
                                                                        \
      __x0 = __ul * __vl,						\
      __x1 = __ul * __vh,						\
      __x2 = __uh * __vl,						\
      __x3 = __uh * __vh;						\
                                                                        \
    __x1 += __ll_highpart (__x0);/* This can't give carry.  */		\
    ಆದರೆ (ckd_ಕೂಡಿಸು (&__x1, __x1, __x2))	/* Did this give a carry?  */	\
      __x3 += __ll_B;		/* Yes, add it in the proper pos.  */	\
                                                                        \
    (w1) = __x3 + __ll_highpart (__x1);                                 \
    (w0) = (__x1 << W_TYPE_ಗಾತ್ರ / 2) + __ll_lowpart (__x0);             \
  } ಆಗ (false)
#ಆದರೆ_ಅಂತ್ಯ

/* Set (q,r) to the quotient and remainder of dividing (n1,n0) by d.  */
#ಆದರೆ !defined udiv_qrnnd || defined UDIV_NEEDS_NORMALIZATION
/* Define our own, not needing normalization.  This function is
   currently not performance critical, so keep it simple.  Similar to
   the mod macro below.  */
# undef udiv_qrnnd
# define udiv_qrnnd(q, r, n1, n0, d)                                    \
  ಮಾಡು {                                                                  \
    mp_limb_t __d1 = d, __d0 = 0, __q = 0, __r1 = n1, __r0 = n0;	\
                                                                        \
    affirm (__r1 < __d1);                                               \
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ __i = W_TYPE_ಗಾತ್ರ; __i > 0; __i--)                         \
      {                                                                 \
        rsh2 (__d1, __d0, __d1, __d0, 1);                               \
        __q <<= 1;                                                      \
        ಆದರೆ (ge2 (__r1, __r0, __d1, __d0))                               \
          {                                                             \
            __q++;                                                      \
            sub_ddmmss (__r1, __r0, __r1, __r0, __d1, __d0);            \
          }                                                             \
      }                                                                 \
    (r) = __r0;                                                         \
    (q) = __q;                                                          \
  } ಆಗ (false)
#ಆದರೆ_ಅಂತ್ಯ

/* Set (sh,sl) = (ah,al) + (bh,bl).  Overflow wraps around.  */
#ಆದರೆ !defined ಕೂಡಿಸು_ssaaaa
# define ಕೂಡಿಸು_ssaaaa(sh, sl, ah, al, bh, bl)                             \
   ((sh) = (ah) + (bh) + ckd_ಕೂಡಿಸು (&(sl), al, bl))
#ಆದರೆ_ಅಂತ್ಯ

/* Set (rh,rl) = (ah,al) >> cnt, where 0 < cnt < W_TYPE_SIZE.  */
#ವ್ಯಾಖ್ಯಾನಿಸು rsh2(rh, rl, ah, al, cnt)                                       \
  ಮಾಡು {                                                                  \
    (rl) = ((ah) << (W_TYPE_ಗಾತ್ರ - (cnt))) | ((al) >> (cnt));           \
    (rh) = (ah) >> (cnt);                                               \
  } ಆಗ (false)

/* Set (rh,rl) = (ah,al) << cnt, where 0 < cnt < W_TYPE_SIZE.
   Overflow wraps around.  */
#ವ್ಯಾಖ್ಯಾನಿಸು lsh2(rh, rl, ah, al, cnt)                                       \
  ಮಾಡು {                                                                  \
    (rh) = ((ah) << cnt) | ((al) >> (W_TYPE_ಗಾತ್ರ - (cnt)));             \
    (rl) = (al) << (cnt);                                               \
  } ಆಗ (false)

/* (ah,hl) < (bh,bl)?  */
ಸ್ಥಿರ bool
lt2 (mp_limb_t ah, mp_limb_t al, mp_limb_t bh, mp_limb_t bl)
{
  mp_limb_t dh, dl;
  bool vh = ckd_sub (&dh, ah, bh);
  mp_limb_t vl = ckd_sub (&dl, al, bl);
  ಹಿಂತಿರುಗಿ vh | ckd_sub (&dh, dh, vl);
}

/* (ah,hl) >= (bh,bl)?  */
ಸ್ಥಿರ bool
ge2 (mp_limb_t ah, mp_limb_t al, mp_limb_t bh, mp_limb_t bl)
{
  ಹಿಂತಿರುಗಿ !lt2 (ah, al, bh, bl);
}

/* Set (rh,rl) = (ah,al) - (bh,bl).  Overflow wraps around.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ sub_ddmmss
# define sub_ddmmss(rh, rl, ah, al, bh, bl)                             \
   ((rh) = (ah) - (bh) - ckd_sub (&(rl), al, bl))
#ಆದರೆ_ಅಂತ್ಯ

/* Set r = (a - b) mod n, where a < n & b <= n.  */
#ವ್ಯಾಖ್ಯಾನಿಸು submod(r,a,b,n)                                                 \
  ಮಾಡು {                                                                  \
    mp_limb_t _s, _t = -ckd_sub (&_s, a, b);				\
    (r) = ((n) & _t) + _s;						\
  } ಆಗ (false)

/* Set r = (a + b) mod n, where a < n & b <= n.  */
#ವ್ಯಾಖ್ಯಾನಿಸು addmod(r,a,b,n)                                                 \
  submod (r, a, (n) - (b), n)

/* Modular two-word addition and subtraction.  For performance reasons, the
   most significant bit of n1 must be clear.  The destination variables must be
   distinct from the mod operand.  */
#ವ್ಯಾಖ್ಯಾನಿಸು addmod2(r1, r0, a1, a0, b1, b0, n1, n0)                         \
  ಮಾಡು {                                                                  \
    ಕೂಡಿಸು_ssaaaa (r1, r0, a1, a0, b1, b0);				\
    ಆದರೆ (ge2 (r1, r0, n1, n0))						\
      sub_ddmmss (r1, r0, r1, r0, n1, n0);				\
  } ಆಗ (false)
#ವ್ಯಾಖ್ಯಾನಿಸು submod2(r1, r0, a1, a0, b1, b0, n1, n0)                         \
  ಮಾಡು {                                                                  \
    bool _v1 = ckd_sub (&(r1), a1, b1);					\
    mp_limb_t _v0 = ckd_sub (&(r0), a0, b0);				\
    ಆದರೆ (_v1 | ckd_sub (&(r1), r1, _v0))					\
      ಕೂಡಿಸು_ssaaaa (r1, r0, r1, r0, n1, n0);				\
  } ಆಗ (false)

/* Return 0 if x < B/2, MP_LIMB_MAX otherwise.  */
ಸ್ಥಿರ mp_limb_t
highbit_ಗಮ್ಯ_ಮುಖವಾಡ (mp_limb_t x)
{
  ಹಿಂತಿರುಗಿ - (x >> (W_TYPE_ಗಾತ್ರ - 1));
}

/* Return (a1,a0) mod (d1,d0), where d1 != 0.  */
ATTRIBUTE_ಶುದ್ಧ ಸ್ಥಿರ uuint
mod2 (mp_limb_t a1, mp_limb_t a0, mp_limb_t d1, mp_limb_t d0)
{
  affirm (d1 != 0);

  ಆದರೆ (a1)
    {
      ಪೂರ್ಣಾಂಕ cntd = stdc_leading_zeros (d1);
      ಪೂರ್ಣಾಂಕ cnta = stdc_leading_zeros (a1);
      ಪೂರ್ಣಾಂಕ cnt = cntd - cnta;
      ಆದರೆ (0 < cnt)
        {
          lsh2 (d1, d0, d1, d0, cnt);
          ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < cnt; i++)
            {
              ಆದರೆ (ge2 (a1, a0, d1, d0))
                sub_ddmmss (a1, a0, a1, a0, d1, d0);
              rsh2 (d1, d0, d1, d0, 1);
            }
        }
    }

  ಹಿಂತಿರುಗಿ make_uuint (a1, a0);
}

/* Return the greatest common divisor of a and b,
   where b is odd.  */
ATTRIBUTE_CONST
ಸ್ಥಿರ mp_limb_t
ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ_odd (mp_limb_t a, mp_limb_t b)
{
  ಆದರೆ (a == 0)
    ಹಿಂತಿರುಗಿ b;

  /* Take out least significant one bit, to make room for sign */
  b >>= 1;

  ಗಾಗಿ (;;)
    {
      mp_limb_t t;
      mp_limb_t bgta;

      assume (a);
      mp_limb_t ao = a >> stdc_trailing_zeros (a);

      t = (ao >> 1) - b;
      ಆದರೆ (t == 0)
        ಹಿಂತಿರುಗಿ ao;

      bgta = highbit_ಗಮ್ಯ_ಮುಖವಾಡ (t);

      /* b <-- min (a, b) */
      b += (bgta & t);

      /* a <-- |a - b| */
      a = (t ^ bgta) - bgta;
    }
}

/* Return the greatest common divisor of (a1,a0) and (b1,b0),
   where (b1,b0) is odd.  */
ATTRIBUTE_ಶುದ್ಧ ಸ್ಥಿರ uuint
gcd2_odd (mp_limb_t a1, mp_limb_t a0, mp_limb_t b1, mp_limb_t b0)
{
  affirm (b0 & 1);

  ಆದರೆ (!a0)
    {
      a0 = a1, a1 = 0;
      ಆದರೆ (!a0)
        ಹಿಂತಿರುಗಿ make_uuint (b1, b0);
    }
  ಹೋಗು make_A_odd;

  ಗಾಗಿ (;;)
    {
      ಆದರೆ ((b1 | a1) == 0)
        ಹಿಂತಿರುಗಿ make_uuint (0, ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ_odd (b0, a0));

      ಆದರೆ (lt2 (b1, b0, a1, a0))
        {
          sub_ddmmss (a1, a0, a1, a0, b1, b0);
          ಆದರೆ (!a0)
            a0 = a1, a1 = 0;
        make_A_odd:
          assume (a0);
          ಪೂರ್ಣಾಂಕ ctz = stdc_trailing_zeros (a0);
          ಆದರೆ (ctz)
            rsh2 (a1, a0, a1, a0, ctz);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          sub_ddmmss (b1, b0, b1, b0, a1, a0);
          ಆದರೆ (!b0)
            {
              b0 = b1, b1 = 0;
              ಆದರೆ (!b0)
                ನಿಲ್ಲಿಸು;
            }
          ಪೂರ್ಣಾಂಕ ctz = stdc_trailing_zeros (b0);
          ಆದರೆ (ctz)
            rsh2 (b1, b0, b1, b0, ctz);
        }
    }

  ಹಿಂತಿರುಗಿ make_uuint (a1, a0);
}

/* Store into FACTORS a prime factor PRIME with multiplicity M.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಗುಣಕ_ಸೇರಿಸು_multiplicity (ರಚನೆ factors *factors,
                            mp_limb_t ಅವಿಭಾಜ್ಯ, ಪೂರ್ಣಾಂಕ m)
{
  ಪೂರ್ಣಾಂಕ nfactors = factors->nfactors;
  mp_limb_t *p = factors->p;
  ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *e = factors->e;

  /* Locate position for insert new or increment e.  */
  ಪೂರ್ಣಾಂಕ i;
  ಗಾಗಿ (i = nfactors; 0 < i; i--)
    {
      ಆದರೆ (p[i - 1] < ಅವಿಭಾಜ್ಯ)
        ನಿಲ್ಲಿಸು;
      ಆದರೆ (p[i - 1] == ಅವಿಭಾಜ್ಯ)
        {
          e[i - 1] += m;
          ಹಿಂತಿರುಗಿ;
        }
    }

  factors->nfactors = nfactors + 1;
  memmove (&p[i + 1], &p[i], (nfactors - i) * ಗಾತ್ರ *p);
  memmove (&e[i + 1], &e[i], (nfactors - i) * ಗಾತ್ರ *e);
  e[i] = m;
  p[i] = ಅವಿಭಾಜ್ಯ;
}

/* Store into FACTORS a prime factor PRIME.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಗುಣಕ_ಸೇರಿಸು (ರಚನೆ factors *factors, mp_limb_t ಅವಿಭಾಜ್ಯ)
{
  ಗುಣಕ_ಸೇರಿಸು_multiplicity (factors, ಅವಿಭಾಜ್ಯ, 1);
}

/* Store into FACTORS a prime factor (P1,P0).  If P1 != 0,
   FACTORS must not already contain a large prime factor.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಗುಣಕ_ಸೇರಿಸು_large (ರಚನೆ factors *factors,
                     mp_limb_t p1, mp_limb_t p0)
{
  ಆದರೆ (p1 > 0)
    {
      affirm (!hi_is_ಗಣ (&factors->plarge));
      factors->plarge = make_uuint (p1, p0);
    }
  ಇಲ್ಲದಿದ್ದರೆ
    ಗುಣಕ_ಸೇರಿಸು (factors, p0);
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ mpz_inits

# include <stdarg.h>

# define mpz_inits(...) mpz_va_init (mpz_init, __VA_ARGS__)
# define mpz_clears(...) mpz_va_init (mpz_clear, __VA_ARGS__)

ಸ್ಥಿರ ಶೂನ್ಯ
mpz_va_init (ಶೂನ್ಯ (*mpz_single_init)(mpz_t), ...)
{
  va_list ap;

  va_ಆರಂಭ (ap, mpz_single_init);

  mpz_t *mpz;
  ಆಗ ((mpz = va_arg (ap, mpz_t *)))
    mpz_single_init (*mpz);

  va_ಕೊನೆ (ap);
}
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ mpn_tdiv_qr

ಸ್ಥಿರ ಶೂನ್ಯ
copy_mpn_from_mpz (mp_limb_t *p, mp_ಗಾತ್ರ_t n, mpz_t z)
{
  mp_ಗಾತ್ರ_t zsize = mpz_ಗಾತ್ರ (z);
  mpn_copyi (p, mpz_limbs_read (z), zsize);
  mpn_zero (p + zsize, n - zsize);
}

ಸ್ಥಿರ ಶೂನ್ಯ
mpn_tdiv_qr (mp_limb_t *qp, mp_limb_t *rp, MAYBE_ಬಳಸದ mp_ಗಾತ್ರ_t qxn,
             mp_limb_t ಸ್ಥಿರಾಂಕ *np, mp_ಗಾತ್ರ_t nn,
             mp_limb_t ಸ್ಥಿರಾಂಕ *dp, mp_ಗಾತ್ರ_t dn)
{
  mpz_t q, r, n, d;
  mpz_inits (q, r, ಖಾಲಿ);
  mpz_tdiv_qr (q, r, mpz_roinit_n (n, np, nn), mpz_roinit_n (d, dp, dn));
  copy_mpn_from_mpz (qp, nn - dn + 1, q);
  copy_mpn_from_mpz (rp, dn, r);
  mpz_clears (q, r, ಖಾಲಿ);
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ರಚನೆ mp_factors mp_ಗುಣಕ (mpz_t);

/* Return an empty set of factors.  */
ಸ್ಥಿರ ರಚನೆ mp_factors
mp_no_factors (ಶೂನ್ಯ)
{
  ಹಿಂತಿರುಗಿ (ರಚನೆ mp_factors) {0};
}

/* Free storage allocated for FACTORS, making it uninitialized.  */
ಸ್ಥಿರ ಶೂನ್ಯ
mp_ಗುಣಕ_clear (ರಚನೆ mp_factors *factors)
{
  ರಚನೆ mp_ಗುಣಕ *f = factors->f;
  ಗಾಗಿ (idx_t i = 0; i < factors->nfactors; i++)
    mpz_clear (f[i].p);
  ಬಿಡುಗಡೆ (f);
}

/* Store into FACTORS a prime factor PRIME with multiplicity M.  */
ಸ್ಥಿರ ಶೂನ್ಯ
mp_ಗುಣಕ_ಸೇರಿಸು (ರಚನೆ mp_factors *factors, mpz_t ಅವಿಭಾಜ್ಯ, mp_bitcnt_t m)
{
  idx_t nfactors = factors->nfactors;
  ರಚನೆ mp_ಗುಣಕ *f = factors->f;
  idx_t i;

  /* Locate position for insert new or increment e.  */
  ಗಾಗಿ (i = nfactors; 0 < i; i--)
    {
      ಪೂರ್ಣಾಂಕ sgn = mpz_cmp (f[i - 1].p, ಅವಿಭಾಜ್ಯ);
      ಆದರೆ (sgn < 0)
        ನಿಲ್ಲಿಸು;
      ಆದರೆ (sgn == 0)
        {
          f[i - 1].e += m;
          ಹಿಂತಿರುಗಿ;
        }
    }

  ಆದರೆ (nfactors == factors->nalloc)
    factors->f = f = xpalloc (f, &factors->nalloc, 1, -1, ಗಾತ್ರ *f);

  factors->nfactors = nfactors + 1;
  memmove (&f[i + 1], &f[i], (nfactors - i) * ಗಾತ್ರ *f);
  f[i].e = m;
  mpz_init_ಗಣ (f[i].p, ಅವಿಭಾಜ್ಯ);
}

/* Store into FACTORS a prime factor PRIME with multiplicity M.  */
ಸ್ಥಿರ ಶೂನ್ಯ
mp_ಗುಣಕ_insert1 (ರಚನೆ mp_factors *factors, mp_limb_t ಅವಿಭಾಜ್ಯ, mp_bitcnt_t m)
{
  mpz_t pz = MPZ_ROINIT_N (&ಅವಿಭಾಜ್ಯ, 1);
  mp_ಗುಣಕ_ಸೇರಿಸು (factors, pz, m);
}


/* primes_ptab[i] is prime i, where the zeroth prime is 3.
   However, the last 7 entries are sentinels.  */
#ವ್ಯಾಖ್ಯಾನಿಸು P(a,b,c) a,
ಸ್ಥಿರ int_least16_t ಸ್ಥಿರಾಂಕ ಅವಿಭಾಜ್ಯಗಳು_ptab[] = {
#ಅನ್ತರ್ಭಾವ "primes.h"
0,0,0,0,0,0,0                           /* 7 sentinels for 8-way loop */
};
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು P

ಎಣಿಕೆ { ಅವಿಭಾಜ್ಯಗಳು_PTAB_ENTRIES = countof (ಅವಿಭಾಜ್ಯಗಳು_ptab) - 8 + 1 };

ರಚನೆ ಅವಿಭಾಜ್ಯಗಳು_dtab
{
  mp_limb_t binv, lim;
};

/* primes_dtab[i].binv is the multiplicative inverse of prime i
   modulo B, where the zeroth prime is 3.  That is,
   ((primes_dtab[i].binv) * (prime i)) mod B = 1.
   primes_dtab[i].lim is the maximum value that won't overflow in
   mp_limb_t arithmetic when multiplied by the ith prime.
   However, the last 7 entries of primes_dtab are sentinels.  */
#ವ್ಯಾಖ್ಯಾನಿಸು P(a,b,c) {b,c},
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ರಚನೆ ಅವಿಭಾಜ್ಯಗಳು_dtab ಅವಿಭಾಜ್ಯಗಳು_dtab[] = {
#ಅನ್ತರ್ಭಾವ "primes.h"
{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0} /* 7 sentinels for 8-way loop */
};
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು P

/* Verify that a word is not wider than
   the integers used to generate primes.h.  */
static_assert (W_TYPE_ಗಾತ್ರ <= WIDE_UINT_ಬಿಟ್ಟುಗಳು);

/* debugging for developers.  Enables devmsg().
   This flag is used only in the GMP code.  */
ಸ್ಥಿರ bool dev_debug = false;

/* Number of Miller-Rabin tests to run.  For more, see:

     Ishmukhametov ST, Mubarakov BG, Rubtsova RG.
     On the Number of Witnesses in the Miller-Rabin Primality Test.
     Symmetry. 2020;12(6):890. https://doi.org/10.3390/sym12060890

   Its Corollary 1 suggests that the probability of error on random inputs
   is less than 16^-MR_REPS, an improvement on the 4^-MR_REPS commonly cited.
   If MR_REPS is 24 this means the probability of error is less than 1.26e-29,
   which is much less than the likelihood of hardware error and so can
   be treated as essentially zero.

   For adversarial inputs, no known false positives exist for Baillie-PSW,
   which mpz_probab_prime_p always uses.  So default MR_REPS to 24,
   the maximum value for which mpz_probab_prime_p does not do extra
   Miller-Rabin tests.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MR_REPS
# define MR_REPS 24
#ಆದರೆ_ಅಂತ್ಯ

/* Trial division with odd primes uses the following trick.

   Let p be an odd prime.  For simplicity, consider the case t < B;
   this is the second loop below.

   From our tables we get

     binv = p^{-1} (mod B)
     lim = floor ((B-1) / p).

   First assume that t is a multiple of p, t = q * p.  Then 0 <= q <= lim
   (and all quotients in this range occur for some t).

   Then t = q * p is true also (mod B), and p is invertible we get

     q = t * binv (mod B).

   Next, assume that t is *not* divisible by p.  Since multiplication
   by binv (mod B) is a one-to-one mapping,

     t * binv (mod B) > lim,

   because all the smaller values are already taken.

   This can be summed up by saying that the function

     q(t) = binv * t (mod B)

   is a permutation of the range 0 <= t < B, with the curious property
   that it maps the multiples of p onto the range 0 <= q <= lim, in
   order, and the non-multiples of p onto the range lim < q < B.
 */

/* The kernel of factor_using_division.  This function is called in a
   loop unrolled by hand.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ mp_limb_t
divblock (ರಚನೆ factors *factors, mp_limb_t t0, ರಚನೆ ಅವಿಭಾಜ್ಯಗಳು_dtab ಸ್ಥಿರಾಂಕ *pd,
          idx_t i, ಪೂರ್ಣಾಂಕ ioff)
{
  ಗಾಗಿ (;;)
    {
      mp_limb_t q = t0 * pd[ioff].binv;
      ಆದರೆ (LIKELY (pd[ioff].lim < q))
        ಹಿಂತಿರುಗಿ t0;
      t0 = q;
      ಗುಣಕ_ಸೇರಿಸು (factors, ಅವಿಭಾಜ್ಯಗಳು_ptab[i + ioff]);
    }
}

/* Insert into FACTORS the factors of (T1,T0) found via trial division.
   The candidate factors are 2 and the primes in the primes table.
   However, primes less than prime I have
   already been considered, and need not be looked at again.
   Return (T1,T0) divided by the factors found.  */
ಸ್ಥಿರ uuint
ಗುಣಕ_using_division (ರಚನೆ factors *factors, mp_limb_t t1, mp_limb_t t0,
                       idx_t i)
{
  ಆದರೆ (t0 % 2 == 0)
    {
      ಪೂರ್ಣಾಂಕ cnt;

      ಆದರೆ (t0 == 0)
        {
          assume (t1);
          cnt = stdc_trailing_zeros (t1);
          t0 = t1 >> cnt;
          t1 = 0;
          cnt += W_TYPE_ಗಾತ್ರ;
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          cnt = stdc_trailing_zeros (t0);
          rsh2 (t1, t0, t1, t0, cnt);
        }

      ಗುಣಕ_ಸೇರಿಸು_multiplicity (factors, 2, cnt);
    }

  ಗಾಗಿ (; t1 > 0 && i < ಅವಿಭಾಜ್ಯಗಳು_PTAB_ENTRIES; i++)
    {
      mp_limb_t p = ಅವಿಭಾಜ್ಯಗಳು_ptab[i];
      ಗಾಗಿ (;;)
        {
          mp_limb_t q1, q0, hi;
          MAYBE_ಬಳಸದ mp_limb_t lo;

          q0 = t0 * ಅವಿಭಾಜ್ಯಗಳು_dtab[i].binv;
          umul_ppmm (hi, lo, q0, p);
          ಆದರೆ (hi > t1)
            ನಿಲ್ಲಿಸು;
          hi = t1 - hi;
          q1 = hi * ಅವಿಭಾಜ್ಯಗಳು_dtab[i].binv;
          ಆದರೆ (LIKELY (q1 > ಅವಿಭಾಜ್ಯಗಳು_dtab[i].lim))
            ನಿಲ್ಲಿಸು;
          t1 = q1; t0 = q0;
          ಗುಣಕ_ಸೇರಿಸು (factors, p);
        }
    }

  ಗಾಗಿ (; i < ಅವಿಭಾಜ್ಯಗಳು_PTAB_ENTRIES; i += 8)
    {
      ಸ್ಥಿರಾಂಕ ರಚನೆ ಅವಿಭಾಜ್ಯಗಳು_dtab *pd = &ಅವಿಭಾಜ್ಯಗಳು_dtab[i];
      t0 = divblock (factors, t0, pd, i, 0);
      t0 = divblock (factors, t0, pd, i, 1);
      t0 = divblock (factors, t0, pd, i, 2);
      t0 = divblock (factors, t0, pd, i, 3);
      t0 = divblock (factors, t0, pd, i, 4);
      t0 = divblock (factors, t0, pd, i, 5);
      t0 = divblock (factors, t0, pd, i, 6);
      t0 = divblock (factors, t0, pd, i, 7);

      ಆದರೆ (ಅವಿಭಾಜ್ಯಗಳು_PTAB_ENTRIES <= i + 8)
        ನಿಲ್ಲಿಸು;
      int_least32_t p = ಅವಿಭಾಜ್ಯಗಳು_ptab[i + 8];
      ಆದರೆ (p * p > t0)
        ನಿಲ್ಲಿಸು;
    }

  ಹಿಂತಿರುಗಿ make_uuint (t1, t0);
}

/* Return the number of limbs in positive N.  */
ಸ್ಥಿರ mp_ಗಾತ್ರ_t
mp_ಗಾತ್ರ (mpz_t n)
{
  /* Tell the compiler that N is positive; this can speed up access to N.  */
  assume (0 < mpz_sgn (n));

  ಹಿಂತಿರುಗಿ mpz_ಗಾತ್ರ (n);
}

/* Insert into MP_FACTORS the factors of N if N < B^2 / 2,
   and return true.  Otherwise, return false.
   Primes less than prime PRIME_IDX have
   already been considered, and need not be looked at again.  */
ಸ್ಥಿರ bool
mp_ಮುಕ್ತಾಯ_ಮೇಲೆ_ಒಳ_single (ರಚನೆ mp_factors *mp_factors, mpz_t n,
                        idx_t ಅವಿಭಾಜ್ಯ_idx)
{
  ಆದರೆ (2 < mp_ಗಾತ್ರ (n))
    ಹಿಂತಿರುಗಿ false;
  mp_limb_t n1 = mpz_getlimbn (n, 1);
  ಆದರೆ (n1 >> (W_TYPE_ಗಾತ್ರ - 1))
    ಹಿಂತಿರುಗಿ false;
  mp_limb_t n0 = mpz_getlimbn (n, 0);
  mpz_ಗಣ_ui (n, 1);

  ರಚನೆ factors factors;
  ಗುಣಕ_ಮೇಲೆ (&factors, n1, n0, ಅವಿಭಾಜ್ಯ_idx);

  ಆದರೆ (hi_is_ಗಣ (&factors.plarge))
    {
      mpz_t p = MPZ_ROINIT_N (factors.plarge.uu, 2);
      mp_ಗುಣಕ_ಸೇರಿಸು (mp_factors, p, 1);
    }

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = factors.nfactors; 0 < i; i--)
    {
      mpz_t p = MPZ_ROINIT_N (&factors.p[i - 1], 1);
      mp_ಗುಣಕ_ಸೇರಿಸು (mp_factors, p, factors.e[i - 1]);
    }

  ಹಿಂತಿರುಗಿ true;
}

/* Insert into MP_FACTORS the factors of N if N < B^2 / 2, and
   return true.  Otherwise, return false.  N must be odd.  */
ಸ್ಥಿರ bool
mp_ಮುಕ್ತಾಯ_ಒಳ_single (ರಚನೆ mp_factors *mp_factors, mpz_t n)
{
  ಹಿಂತಿರುಗಿ mp_ಮುಕ್ತಾಯ_ಮೇಲೆ_ಒಳ_single (mp_factors, n, 0);
}

/* Return some or all factors of T.
   Divide T by the factors found.  */
ಸ್ಥಿರ ರಚನೆ mp_factors
mp_ಗುಣಕ_using_division (mpz_t t)
{
  devmsg ("[trial division] ");
  ರಚನೆ mp_factors factors = mp_no_factors ();

  mp_bitcnt_t m = mpz_scan1 (t, 0);
  ಆದರೆ (m)
    {
      mpz_fdiv_q_2exp (t, t, m);
      mp_ಗುಣಕ_insert1 (&factors, 2, m);
      ಆದರೆ (mp_ಮುಕ್ತಾಯ_ಒಳ_single (&factors, t))
        ಹಿಂತಿರುಗಿ factors;
    }

  ಗಾಗಿ (idx_t i = 0; i < ಅವಿಭಾಜ್ಯಗಳು_PTAB_ENTRIES; i++)
    {
      mp_limb_t d = ಅವಿಭಾಜ್ಯಗಳು_ptab[i];
      ಗಾಗಿ (m = 0; mpz_divisible_ui_p (t, d); m++)
        {
          mpz_divexact_ui (t, t, d);
          ಆದರೆ (mp_ಮುಕ್ತಾಯ_ಮೇಲೆ_ಒಳ_single (&factors, t, i))
            {
              mp_ಗುಣಕ_insert1 (&factors, d, m + 1);
              ಹಿಂತಿರುಗಿ factors;
            }
        }
      ಆದರೆ (m)
        mp_ಗುಣಕ_insert1 (&factors, d, m);
      static_assert (ವರ್ಗಫಲ_ದ_ಮೊದಲ_OMITTED_ಅವಿಭಾಜ್ಯ
                     <= MIN (MP_LIMB_MAX, ULONG_MAX));
      ಆದರೆ (mpz_cmp_ui (t, d * d) < 0)
        ನಿಲ್ಲಿಸು;
    }

  ಹಿಂತಿರುಗಿ factors;
}

/* Entry i contains (2i+1)^(-1) mod 2^8.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ binvert_ಕೋಷ್ಟಕ[128] =
{
  0x01, 0xAB, 0xCD, 0xB7, 0x39, 0xA3, 0xC5, 0xEF,
  0xF1, 0x1B, 0x3D, 0xA7, 0x29, 0x13, 0x35, 0xDF,
  0xE1, 0x8B, 0xAD, 0x97, 0x19, 0x83, 0xA5, 0xCF,
  0xD1, 0xFB, 0x1D, 0x87, 0x09, 0xF3, 0x15, 0xBF,
  0xC1, 0x6B, 0x8D, 0x77, 0xF9, 0x63, 0x85, 0xAF,
  0xB1, 0xDB, 0xFD, 0x67, 0xE9, 0xD3, 0xF5, 0x9F,
  0xA1, 0x4B, 0x6D, 0x57, 0xD9, 0x43, 0x65, 0x8F,
  0x91, 0xBB, 0xDD, 0x47, 0xC9, 0xB3, 0xD5, 0x7F,
  0x81, 0x2B, 0x4D, 0x37, 0xB9, 0x23, 0x45, 0x6F,
  0x71, 0x9B, 0xBD, 0x27, 0xA9, 0x93, 0xB5, 0x5F,
  0x61, 0x0B, 0x2D, 0x17, 0x99, 0x03, 0x25, 0x4F,
  0x51, 0x7B, 0x9D, 0x07, 0x89, 0x73, 0x95, 0x3F,
  0x41, 0xEB, 0x0D, 0xF7, 0x79, 0xE3, 0x05, 0x2F,
  0x31, 0x5B, 0x7D, 0xE7, 0x69, 0x53, 0x75, 0x1F,
  0x21, 0xCB, 0xED, 0xD7, 0x59, 0xC3, 0xE5, 0x0F,
  0x11, 0x3B, 0x5D, 0xC7, 0x49, 0x33, 0x55, 0xFF
};

/* Compute n^(-1) mod B.  n must be odd.  */
ಸ್ಥಿರ mp_limb_t
binv_limb (mp_limb_t n)
{
  mp_limb_t inv = binvert_ಕೋಷ್ಟಕ[(n / 2) & 0x7F];
  ಆದರೆ ( 8 < W_TYPE_ಗಾತ್ರ) inv = 2 * inv - inv * inv * n;
  ಆದರೆ (16 < W_TYPE_ಗಾತ್ರ) inv = 2 * inv - inv * inv * n;
  ಆದರೆ (32 < W_TYPE_ಗಾತ್ರ) inv = 2 * inv - inv * inv * n;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ invbits = 64; invbits < W_TYPE_ಗಾತ್ರ; invbits *= 2)
    inv = 2 * inv - inv * inv * n;
  ಹಿಂತಿರುಗಿ inv;
}

/* q = u / d, assuming d|u.  */
#ವ್ಯಾಖ್ಯಾನಿಸು divexact_21(q1, q0, u1, u0, d)                                  \
  ಮಾಡು {                                                                  \
    mp_limb_t _di, _q0;							\
    _di = binv_limb (d);						\
    _q0 = (u0) * _di;                                                   \
    ಆದರೆ ((u1) >= (d))                                                    \
      {                                                                 \
        mp_limb_t _p1;							\
        MAYBE_ಬಳಸದ mp_limb_t _p0;					\
        umul_ppmm (_p1, _p0, _q0, d);                                   \
        (q1) = ((u1) - _p1) * _di;                                      \
        (q0) = _q0;                                                     \
      }                                                                 \
    ಇಲ್ಲದಿದ್ದರೆ                                                                \
      {                                                                 \
        (q0) = _q0;                                                     \
        (q1) = 0;                                                       \
      }                                                                 \
  } ಆಗ (false)

/* x B (mod n).  */
#ವ್ಯಾಖ್ಯಾನಿಸು redcify(r_prim, r, n)                                           \
  ಮಾಡು {                                                                  \
    MAYBE_ಬಳಸದ mp_limb_t _redcify_q;					\
    udiv_qrnnd (_redcify_q, r_prim, r, 0, n);                           \
  } ಆಗ (false)

/* x B^2 (mod n).  Requires x > 0, n1 < B/2.  */
#ವ್ಯಾಖ್ಯಾನಿಸು redcify2(r1, r0, x, n1, n0)                                     \
  ಮಾಡು {                                                                  \
    mp_limb_t _r1, _r0, _i;						\
    ಆದರೆ ((x) < (n1))                                                     \
      {                                                                 \
        _r1 = (x); _r0 = 0;                                             \
        _i = W_TYPE_ಗಾತ್ರ;                                               \
      }                                                                 \
    ಇಲ್ಲದಿದ್ದರೆ                                                                \
      {                                                                 \
        _r1 = 0; _r0 = (x);                                             \
        _i = 2 * W_TYPE_ಗಾತ್ರ;                                           \
      }                                                                 \
    ಆಗ (_i-- > 0)                                                    \
      {                                                                 \
        lsh2 (_r1, _r0, _r1, _r0, 1);                                   \
        ಆದರೆ (ge2 (_r1, _r0, n1, n0))					\
          sub_ddmmss (_r1, _r0, _r1, _r0, n1, n0);			\
      }                                                                 \
    (r1) = _r1;                                                         \
    (r0) = _r0;                                                         \
  } ಆಗ (false)

/* Modular two-word multiplication, r = a * b mod m, with mi = m^(-1) mod B.
   Both a and b must be in redc form, the result will be in redc form too.  */
ಸ್ಥಿರ ಸಾಲೊಳಗೆ mp_limb_t
mulredc (mp_limb_t a, mp_limb_t b, mp_limb_t m, mp_limb_t mi)
{
  mp_limb_t rh, rl, q, th, xh;
  MAYBE_ಬಳಸದ mp_limb_t tl;

  umul_ppmm (rh, rl, a, b);
  q = rl * mi;
  umul_ppmm (th, tl, q, m);
  ಆದರೆ (ckd_sub (&xh, rh, th))
    xh += m;

  ಹಿಂತಿರುಗಿ xh;
}

/* Modular two-word multiplication, r = a * b mod m, with mi = m^(-1) mod B.
   Both a and b must be in redc form, the result will be in redc form too.
   For performance reasons, the most significant bit of m must be clear.  */
ಸ್ಥಿರ mp_limb_t
mulredc2 (mp_limb_t *r1p,
          mp_limb_t a1, mp_limb_t a0, mp_limb_t b1, mp_limb_t b0,
          mp_limb_t m1, mp_limb_t m0, mp_limb_t mi)
{
  mp_limb_t r1, r0, q, p1, t1, t0, s1, s0;
  MAYBE_ಬಳಸದ mp_limb_t p0;
  mi = -mi;
  affirm ((m1 >> (W_TYPE_ಗಾತ್ರ - 1)) == 0);

  /* First compute a0 * (b1,b0) B^{-1}
        +-----+
        |a0 b0|
     +--+--+--+
     |a0 b1|
     +--+--+--+
        |q0 m0|
     +--+--+--+
     |q0 m1|
    -+--+--+--+
     |r1|r0| 0|
     +--+--+--+
  */
  umul_ppmm (t1, t0, a0, b0);
  umul_ppmm (r1, r0, a0, b1);
  q = mi * t0;
  umul_ppmm (p1, p0, q, m0);
  umul_ppmm (s1, s0, q, m1);
  r0 += (t0 != 0); /*ವಾಹಕ*/
  ಕೂಡಿಸು_ssaaaa (r1, r0, r1, r0, 0, p1);
  ಕೂಡಿಸು_ssaaaa (r1, r0, r1, r0, 0, t1);
  ಕೂಡಿಸು_ssaaaa (r1, r0, r1, r0, s1, s0);

  /* Next, (a1 * (b1,b0) + (r1,r0) B^{-1}
        +-----+
        |a1 b0|
        +--+--+
        |r1|r0|
     +--+--+--+
     |a1 b1|
     +--+--+--+
        |q1 m0|
     +--+--+--+
     |q1 m1|
    -+--+--+--+
     |r1|r0| 0|
     +--+--+--+
  */
  umul_ppmm (t1, t0, a1, b0);
  umul_ppmm (s1, s0, a1, b1);
  ಕೂಡಿಸು_ssaaaa (t1, t0, t1, t0, 0, r0);
  q = mi * t0;
  ಕೂಡಿಸು_ssaaaa (r1, r0, s1, s0, 0, r1);
  umul_ppmm (p1, p0, q, m0);
  umul_ppmm (s1, s0, q, m1);
  r0 += (t0 != 0); /*ವಾಹಕ*/
  ಕೂಡಿಸು_ssaaaa (r1, r0, r1, r0, 0, p1);
  ಕೂಡಿಸು_ssaaaa (r1, r0, r1, r0, 0, t1);
  ಕೂಡಿಸು_ssaaaa (r1, r0, r1, r0, s1, s0);

  ಆದರೆ (ge2 (r1, r0, m1, m0))
    sub_ddmmss (r1, r0, r1, r0, m1, m0);

  *r1p = r1;
  ಹಿಂತಿರುಗಿ r0;
}

ಸ್ಥಿರ bool mp_ಅವಿಭಾಜ್ಯ_p (mpz_t);

/* Is N prime?  N cannot be even or be a composite number less than
   SQUARE_OF_FIRST_OMITTED_PRIME.  */
ಸ್ಥಿರ bool
ಅವಿಭಾಜ್ಯ_p (mp_limb_t n)
{
  ಆದರೆ (n <= 1)
    ಹಿಂತಿರುಗಿ false;

  /* We have already cast out small primes.  */
  ಆದರೆ (n < ವರ್ಗಫಲ_ದ_ಮೊದಲ_OMITTED_ಅವಿಭಾಜ್ಯ)
    ಹಿಂತಿರುಗಿ true;

  mpz_t mn = MPZ_ROINIT_N (&n, 1);
  ಹಿಂತಿರುಗಿ mp_ಅವಿಭಾಜ್ಯ_p (mn);
}

/* Is (n1,n0) prime?  (n1,n0) cannot be even or be a composite number
   less than SQUARE_OF_FIRST_OMITTED_PRIME.  */
ಸ್ಥಿರ bool
prime2_p (mp_limb_t n1, mp_limb_t n0)
{
  ಆದರೆ (n1 == 0)
    ಹಿಂತಿರುಗಿ ಅವಿಭಾಜ್ಯ_p (n0);

  uuint n = make_uuint (n1, n0);
  mpz_t mn = MPZ_ROINIT_N (n.uu, 2);
  ಹಿಂತಿರುಗಿ mp_ಅವಿಭಾಜ್ಯ_p (mn);
}

/* Is N prime?  N cannot be even or be a composite number less than
   SQUARE_OF_FIRST_OMITTED_PRIME.  */
ಸ್ಥಿರ bool
mp_ಅವಿಭಾಜ್ಯ_p (mpz_t n)
{
  ಆದರೆ (mpz_cmp_ui (n, 1) <= 0)
    ಹಿಂತಿರುಗಿ false;

  /* We have already cast out small primes.  */
  ಆದರೆ (mpz_cmp_ui (n, ವರ್ಗಫಲ_ದ_ಮೊದಲ_OMITTED_ಅವಿಭಾಜ್ಯ) < 0)
    ಹಿಂತಿರುಗಿ true;

  ಹಿಂತಿರುಗಿ !!mpz_probab_ಅವಿಭಾಜ್ಯ_p (n, MR_REPS);
}

/* Insert into FACTORS the result of factoring N,
   using Pollard-rho with starting value A.  N must be odd.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಗುಣಕ_using_pollard_rho (ರಚನೆ factors *factors, mp_limb_t n, mp_limb_t a)
{
  mp_limb_t x, z, y, P, t, ni, g;

  int_fast64_t k = 1;
  int_fast64_t l = 1;

  redcify (P, 1, n);
  addmod (x, P, P, n);          /* i.e., redcify(2) */
  y = z = x;

  ಆಗ (n != 1)
    {
      affirm (a < n);

      ni = binv_limb (n);	/* FIXME: when could we use old 'ni' value?  */

      ಗಾಗಿ (;;)
        {
          ಮಾಡು
            {
              x = mulredc (x, x, n, ni);
              addmod (x, x, a, n);

              submod (t, z, x, n);
              P = mulredc (P, t, n, ni);

              ಆದರೆ ((k & 31) == 1)
                {
                  ಆದರೆ (ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ_odd (P, n) != 1)
                    ಹೋಗು ಗುಣಕ_ಸಿಕ್ಕಿತು;
                  y = x;
                }
            }
          ಆಗ (--k != 0);

          z = x;
          k = l;
          l = 2 * l;
          ಗಾಗಿ (int_fast64_t i = 0; i < k; i++)
            {
              x = mulredc (x, x, n, ni);
              addmod (x, x, a, n);
            }
          y = x;
        }

    ಗುಣಕ_ಸಿಕ್ಕಿತು:
      ಮಾಡು
        {
          y = mulredc (y, y, n, ni);
          addmod (y, y, a, n);

          submod (t, z, y, n);
          g = ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ_odd (t, n);
        }
      ಆಗ (g == 1);

      ಆದರೆ (n == g)
        {
          /* Found n itself as factor.  Restart with different params.  */
          ಗುಣಕ_using_pollard_rho (factors, n, a + 1);
          ಹಿಂತಿರುಗಿ;
        }

      n = n / g;

      ಆದರೆ (!ಅವಿಭಾಜ್ಯ_p (g))
        ಗುಣಕ_using_pollard_rho (factors, g, a + 1);
      ಇಲ್ಲದಿದ್ದರೆ
        ಗುಣಕ_ಸೇರಿಸು (factors, g);

      ಆದರೆ (ಅವಿಭಾಜ್ಯ_p (n))
        {
          ಗುಣಕ_ಸೇರಿಸು (factors, n);
          ನಿಲ್ಲಿಸು;
        }

      x = x % n;
      z = z % n;
      y = y % n;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ
ಗುಣಕ_using_pollard_rho2 (ರಚನೆ factors *factors,
                           mp_limb_t n1, mp_limb_t n0, mp_limb_t a)
{
  mp_limb_t x1, x0, z1, z0, y1, y0, P1, P0, t1, t0, g1, g0, r1m;

  int_fast64_t k = 1;
  int_fast64_t l = 1;

  redcify2 (P1, P0, 1, n1, n0);
  addmod2 (x1, x0, P1, P0, P1, P0, n1, n0); /* i.e., redcify(2) */
  y1 = z1 = x1;
  y0 = z0 = x0;

  ಆಗ (n1 != 0 || n0 != 1)
    {
      mp_limb_t ni = binv_limb (n0);

      ಗಾಗಿ (;;)
        {
          ಮಾಡು
            {
              x0 = mulredc2 (&r1m, x1, x0, x1, x0, n1, n0, ni);
              x1 = r1m;
              addmod2 (x1, x0, x1, x0, 0, a, n1, n0);

              submod2 (t1, t0, z1, z0, x1, x0, n1, n0);
              P0 = mulredc2 (&r1m, P1, P0, t1, t0, n1, n0, ni);
              P1 = r1m;

              ಆದರೆ ((k & 31) == 1)
                {
                  uuset (&g1, &g0, gcd2_odd (P1, P0, n1, n0));
                  ಆದರೆ (g1 != 0 || g0 != 1)
                    ಹೋಗು ಗುಣಕ_ಸಿಕ್ಕಿತು;
                  y1 = x1; y0 = x0;
                }
            }
          ಆಗ (--k != 0);

          z1 = x1; z0 = x0;
          k = l;
          l = 2 * l;
          ಗಾಗಿ (int_fast64_t i = 0; i < k; i++)
            {
              x0 = mulredc2 (&r1m, x1, x0, x1, x0, n1, n0, ni);
              x1 = r1m;
              addmod2 (x1, x0, x1, x0, 0, a, n1, n0);
            }
          y1 = x1; y0 = x0;
        }

    ಗುಣಕ_ಸಿಕ್ಕಿತು:
      ಮಾಡು
        {
          y0 = mulredc2 (&r1m, y1, y0, y1, y0, n1, n0, ni);
          y1 = r1m;
          addmod2 (y1, y0, y1, y0, 0, a, n1, n0);

          submod2 (t1, t0, z1, z0, y1, y0, n1, n0);
          uuset (&g1, &g0, gcd2_odd (t1, t0, n1, n0));
        }
      ಆಗ (g1 == 0 && g0 == 1);

      ಆದರೆ (g1 == 0)
        {
          /* The found factor is one word, and > 1.  */
          divexact_21 (n1, n0, n1, n0, g0);     /* n = n / g */

          ಆದರೆ (!ಅವಿಭಾಜ್ಯ_p (g0))
            ಗುಣಕ_using_pollard_rho (factors, g0, a + 1);
          ಇಲ್ಲದಿದ್ದರೆ
            ಗುಣಕ_ಸೇರಿಸು (factors, g0);
        }
      ಇಲ್ಲದಿದ್ದರೆ
        {
          /* The found factor is two words.  This is highly unlikely, thus hard
             to trigger.  Please be careful before you change this code!  */

          ಆದರೆ (n1 == g1 && n0 == g0)
            {
              /* Found n itself as factor.  Restart with different params.  */
              ಗುಣಕ_using_pollard_rho2 (factors, n1, n0, a + 1);
              ಹಿಂತಿರುಗಿ;
            }

          /* Compute n = n / g.  Since the result will fit one word,
             we can compute the quotient modulo B, ignoring the high
             divisor word.  */
          n0 = binv_limb (g0) * n0;
          n1 = 0;

          ಆದರೆ (!prime2_p (g1, g0))
            ಗುಣಕ_using_pollard_rho2 (factors, g1, g0, a + 1);
          ಇಲ್ಲದಿದ್ದರೆ
            ಗುಣಕ_ಸೇರಿಸು_large (factors, g1, g0);
        }

      ಆದರೆ (n1 == 0)
        {
          ಆದರೆ (ಅವಿಭಾಜ್ಯ_p (n0))
            {
              ಗುಣಕ_ಸೇರಿಸು (factors, n0);
              ನಿಲ್ಲಿಸು;
            }

          ಗುಣಕ_using_pollard_rho (factors, n0, a);
          ಹಿಂತಿರುಗಿ;
        }

      ಆದರೆ (prime2_p (n1, n0))
        {
          ಗುಣಕ_ಸೇರಿಸು_large (factors, n1, n0);
          ನಿಲ್ಲಿಸು;
        }

      uuset (&x1, &x0, mod2 (x1, x0, n1, n0));
      uuset (&z1, &z0, mod2 (z1, z0, n1, n0));
      uuset (&y1, &y0, mod2 (y1, y0, n1, n0));
    }
}

/* Set RP = (AP + BP) mod MP.  All values are nonnegative and take up
   N>0 words, and AP and BP are both less than MP.  */
ಸ್ಥಿರ ಶೂನ್ಯ
mp_modadd (mp_limb_t *rp, mp_limb_t ಸ್ಥಿರಾಂಕ *ap, mp_limb_t ಸ್ಥಿರಾಂಕ *bp,
           mp_limb_t ಸ್ಥಿರಾಂಕ *mp, mp_ಗಾತ್ರ_t n)
{
  mp_limb_t cy = mpn_ಕೂಡಿಸು_n (rp, ap, bp, n);
  ಆದರೆ (cy || mpn_cmp (rp, mp, n) >= 0)
    mpn_sub_n (rp, rp, mp, n);
}

/* Set RP = (AP - BP) mod MP.  All values are nonnegative and take up
   N>0 words, and AP and BP are both less than MP.  */
ಸ್ಥಿರ ಶೂನ್ಯ
mp_modsub (mp_limb_t *rp, mp_limb_t ಸ್ಥಿರಾಂಕ *ap, mp_limb_t ಸ್ಥಿರಾಂಕ *bp,
           mp_limb_t ಸ್ಥಿರಾಂಕ *mp, mp_ಗಾತ್ರ_t n)
{
  mp_limb_t cy = mpn_sub_n (rp, ap, bp, n);
  ಆದರೆ (cy)
    mpn_ಕೂಡಿಸು_n (rp, rp, mp, n);
}

/* Set RP = (AP - B0) mod MP.  All values are nonnegative, AP and MP
   both take up N>0 words, and AP < MP.  */
ಸ್ಥಿರ ಶೂನ್ಯ
mp_modadd_1 (mp_limb_t *rp, mp_limb_t ಸ್ಥಿರಾಂಕ *ap, mp_limb_t b0,
             mp_limb_t ಸ್ಥಿರಾಂಕ *mp, mp_ಗಾತ್ರ_t n)
{
  mp_limb_t cy = mpn_ಕೂಡಿಸು_1 (rp, ap, n, b0);
  ಆದರೆ (cy || mpn_cmp (rp, mp, n) >= 0)
    mpn_sub_n (rp, rp, mp, n);
}

ಸ್ಥಿರ ಶೂನ್ಯ
mp_mulredc (mp_limb_t *rp, mp_limb_t ಸ್ಥಿರಾಂಕ *ap, mp_limb_t ಸ್ಥಿರಾಂಕ *bp,
            mp_limb_t ಸ್ಥಿರಾಂಕ *mp, mp_ಗಾತ್ರ_t n, mp_limb_t m0inv, mp_limb_t *tp)
{
  tp[n] = mpn_mul_1 (tp, ap, n, bp[0]);
  tp[0] = mpn_addmul_1 (tp, mp, n, tp[0] * m0inv);

  ಗಾಗಿ (mp_ಗಾತ್ರ_t i = 1; i < n; i++)
    {
      tp[n + i] = mpn_addmul_1 (tp + i, ap, n, bp[i]);
      tp[i] = mpn_addmul_1 (tp + i, mp, n, tp[i] * m0inv);
    }
  mp_modadd (rp, tp, tp + n, mp, n);
}

/* Maximum value for N in mp_factor_using_pollard_rho,
   to avoid integer overflow when it calls xinmalloc.  */
#ವ್ಯಾಖ್ಯಾನಿಸು MP_ಗುಣಕ_USING_POLLARD_RHO_N_MAX \
  ((MIN (IDX_MAX, TYPE_ಗರಿಷ್ಠ (mp_ಗಾತ್ರ_t)) - 3) / 10)

/* Insert into FACTORS the result of factoring MP, of size N,
   using Pollard-rho with starting value A.  MP must be odd. */
ಸ್ಥಿರ ಶೂನ್ಯ
mp_ಗುಣಕ_using_pollard_rho (ರಚನೆ mp_factors *factors,
                             mp_limb_t ಸ್ಥಿರಾಂಕ *mp, mp_ಗಾತ್ರ_t n,
                             mp_limb_t a)
{
  devmsg ("[pollard-rho (%lu)] ", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಪೂರ್ಣಾಂಕ) a);

  static_assert (10 * MP_ಗುಣಕ_USING_POLLARD_RHO_N_MAX + 3
                 <= MIN (IDX_MAX, TYPE_ಗರಿಷ್ಠ (mp_ಗಾತ್ರ_t)));
  mp_limb_t *scratch = xinmalloc (10 * n + 3, ಗಾತ್ರ *scratch);
  mp_limb_t *qp = scratch + 2 * n + 1, *pp = qp + n + 2,
    *xp = pp + n, *yp = xp + n, *zp = yp + n,
    *tp = zp + n, *sp = tp + n, *gp = sp + n;
  mp_ಗಾತ್ರ_t gn;

  mpn_zero (scratch, n);
  scratch[n] = 1;
  mpn_tdiv_qr (qp, pp, 0, scratch, n + 1, mp, n);

  mp_modadd (xp, pp, pp, mp, n);
  mpn_copyi (yp, xp, n);
  mpn_copyi (zp, xp, n);

  mp_limb_t m0inv = binv_limb (-mp[0]);

  ಗಾಗಿ (int_fast64_t k = 1; ; k *= 2)
    {
      ಗಾಗಿ (int_fast64_t i = k; 0 < i; i--)
        {
          mp_mulredc (tp, xp, xp, mp, n, m0inv, scratch);
          mp_modadd_1 (xp, tp, a, mp, n);

          mp_modsub (tp, zp, xp, mp, n);
          mp_mulredc (pp, pp, tp, mp, n, m0inv, scratch);

          ಆದರೆ (i % 128 == 1)
            {
              ಆದರೆ (mpn_zero_p (pp, n))
                {
                  mp_ಗುಣಕ_using_pollard_rho (factors, mp, n, a + 1);
                  ಹೋಗು ಮುಕ್ತಾಯ;
                }
              mpn_copyi (tp, pp, n);
              mpn_copyi (sp, mp, n);
              gn = mpn_ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ (gp, tp, n, sp, n);
              ಆದರೆ (gn != 1 || gp[0] != 1)
                ಹೋಗು ಗುಣಕ_ಸಿಕ್ಕಿತು;
              mpn_copyi (yp, xp, n);
            }
        }

      mpn_copyi (zp, xp, n);
      ಗಾಗಿ (int_fast64_t i = 2 * k; 0 < i; i--)
        {
          mp_mulredc (tp, xp, xp, mp, n, m0inv, scratch);
          mp_modadd_1 (xp, tp, a, mp, n);
        }
      mpn_copyi (yp, xp, n);
    }

 ಗುಣಕ_ಸಿಕ್ಕಿತು:
  ಮಾಡು
    {
      mp_mulredc (tp, yp, yp, mp, n, m0inv, scratch);
      mp_modadd_1 (yp, tp, a, mp, n);
      mp_modsub (tp, zp, yp, mp, n);
      mpn_copyi (sp, mp, n);
      gn = mpn_ಮಹತ್ತಮ_ಸಾಮಾನ್ಯ_ಅಪವರ್ತನ (gp, tp, n, sp, n);
    }
  ಆಗ (gn == 1 && gp[0] == 1);

  mpz_t g = MPZ_ROINIT_N (gp, gn);
  mpz_t m = MPZ_ROINIT_N ((mp_limb_t *) mp, n), q;
  mpz_init (q);
  mpz_divexact (q, m, g);

  ಆದರೆ (!mp_ಮುಕ್ತಾಯ_ಒಳ_single (factors, g))
    {
      ಆದರೆ (mp_ಅವಿಭಾಜ್ಯ_p (g))
        mp_ಗುಣಕ_ಸೇರಿಸು (factors, g, 1);
      ಇಲ್ಲದಿದ್ದರೆ
        mp_ಗುಣಕ_using_pollard_rho (factors, gp, gn, a + 1);
    }

  ಆದರೆ (!mp_ಮುಕ್ತಾಯ_ಒಳ_single (factors, q))
    {
      ಆದರೆ (mp_ಅವಿಭಾಜ್ಯ_p (q))
        mp_ಗುಣಕ_ಸೇರಿಸು (factors, q, 1);
      ಇಲ್ಲದಿದ್ದರೆ
        {
          devmsg ("[composite factor--restarting pollard-rho] ");
          mp_ಗುಣಕ_using_pollard_rho (factors, mpz_limbs_read (q),
                                       mp_ಗಾತ್ರ (q), a + 1);
        }
    }

  mpz_clear (q);

 ಮುಕ್ತಾಯ:
  ಬಿಡುಗಡೆ (scratch);
}

/* Insert into FACTORS the prime factors of the two-word number (T1,T0).
   Primes less than the prime with index PRIME_IDX
   have already been considered, and need not be looked at again.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಗುಣಕ_ಮೇಲೆ (ರಚನೆ factors *factors, mp_limb_t t1, mp_limb_t t0,
           idx_t ಅವಿಭಾಜ್ಯ_idx)
{
  factors->nfactors = 0;
  hiset (&factors->plarge, 0);

  ಆದರೆ (t1 == 0 && t0 < 2)
    ಹಿಂತಿರುಗಿ;

  uuset (&t1, &t0, ಗುಣಕ_using_division (factors, t1, t0, ಅವಿಭಾಜ್ಯ_idx));

  ಆದರೆ (t1 == 0 && t0 < 2)
    ಹಿಂತಿರುಗಿ;

  ಆದರೆ (prime2_p (t1, t0))
    ಗುಣಕ_ಸೇರಿಸು_large (factors, t1, t0);
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (t1 == 0)
        ಗುಣಕ_using_pollard_rho (factors, t0, 1);
      ಇಲ್ಲದಿದ್ದರೆ
        ಗುಣಕ_using_pollard_rho2 (factors, t1, t0, 1);
    }
}

/* Compute the prime factors of the two-word number (T1,T0),
   and put the results in FACTORS.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಗುಣಕ (ರಚನೆ factors *factors, mp_limb_t t1, mp_limb_t t0)
{
  ಗುಣಕ_ಮೇಲೆ (factors, t1, t0, 0);
}

/* Return the prime factors of T.  */
ಸ್ಥಿರ ರಚನೆ mp_factors
mp_ಗುಣಕ (mpz_t t)
{
  ರಚನೆ mp_factors factors = mp_no_factors ();

  ಆದರೆ (mpz_sgn (t) != 0)
    {
      factors = mp_ಗುಣಕ_using_division (t);

      ಆದರೆ (mpz_cmp_ui (t, 1) != 0)
        {
          devmsg ("[is number prime?] ");
          ಆದರೆ (mp_ಅವಿಭಾಜ್ಯ_p (t))
            mp_ಗುಣಕ_ಸೇರಿಸು (&factors, t, 1);
          ಇಲ್ಲದಿದ್ದರೆ
            mp_ಗುಣಕ_using_pollard_rho (&factors, mpz_limbs_read (t),
                                         mp_ಗಾತ್ರ (t), 1);
        }
    }

  ಹಿಂತಿರುಗಿ factors;
}

/* Convert to *U the value represented by S, and return LONGINT_OK.
   Return LONGINT_OVERFLOW if S is composed of all digits but is too
   large to fit in *U, otherwise LONGINT_INVALID if S is not a valid
   positive integer.  */
ಸ್ಥಿರ strtol_error
strtouuint (uuint *u, ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *s)
{
  mp_limb_t hi = 0, lo = *s++ - '0';

  ಆದರೆ (UNLIKELY (9 < lo))
    ಹಿಂತಿರುಗಿ LONGINT_INVALID;

  ಗಾಗಿ (; LIKELY (0 <= *s - '0' && *s - '0' <= 9); s++)
    {
      ಆದರೆ (UNLIKELY (ckd_mul (&hi, hi, 10)))
        ಹೋಗು ಅತಿಪ್ರವಾಹ;

      ಪೂರ್ಣಾಂಕ lo_ವಾಹಕ = (lo >> (W_TYPE_ಗಾತ್ರ - 3)) + (lo >> (W_TYPE_ಗಾತ್ರ - 1));
      lo_ವಾಹಕ += 10 * lo < 2 * lo;

      lo = 10 * lo;
      lo_ವಾಹಕ += ckd_ಕೂಡಿಸು (&lo, lo, *s - '0');
      ಆದರೆ (UNLIKELY (ckd_ಕೂಡಿಸು (&hi, hi, lo_ವಾಹಕ)))
        ಹೋಗು ಅತಿಪ್ರವಾಹ;
    }

  ಆದರೆ (UNLIKELY (*s))
    ಹಿಂತಿರುಗಿ LONGINT_INVALID;

  *u = make_uuint (hi, lo);
  ಹಿಂತಿರುಗಿ LONGINT_OK;

 ಅತಿಪ್ರವಾಹ:
  /* Since s[0] caused the overflow, we know it is a valid integer.  */
  s++;
  ಹಿಂತಿರುಗಿ s[strspn (s, "0123456789")] ? LONGINT_INVALID : LONGINT_ಅತಿಪ್ರವಾಹ;
}

/* FACTOR_PIPE_BUF is chosen to give good performance,
   and also is the max guaranteed size that
   consumers can read atomically through pipes.
   Also it's big enough to cater for max line length
   even with 128 bit word.  */
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _POSIX_PIPE_BUF
# define _POSIX_PIPE_BUF 512
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ PIPE_BUF
ಎಣಿಕೆ { ಗುಣಕ_PIPE_BUF = PIPE_BUF };
#ಇಲ್ಲದಿದ್ದರೆ
ಎಣಿಕೆ { ಗುಣಕ_PIPE_BUF = _POSIX_PIPE_BUF };
#ಆದರೆ_ಅಂತ್ಯ

/* Structure and routines for buffering and outputting full lines, to
   support parallel operation efficiently.

   The buffer is twice FACTOR_PIPE_BUF so that its second half can
   hold the remainder of data that is somewhat too large.  Also, the
   very end of the second half is used to hold temporary data when
   stringifying integers, which is most conveniently done
   right-to-left.

   Although the buffer's second half doesn't need to be quite so large
   - its necessary size is bounded above by roughly the maximum output
   line for a uuint plus the string length of a uuint - it'd be a bit
   of a pain to figure out exactly how small it can be without causing
   trouble.  */
ಸ್ಥಿರ ಅಕ್ಷರ lbuf_buf[2 * ಗುಣಕ_PIPE_BUF];
ಸ್ಥಿರ idx_t lbuffered;

/* Write complete LBUF to standard output.  */
ಸ್ಥಿರ ಶೂನ್ಯ
lbuf_flush (ಶೂನ್ಯ)
{
  idx_t ಗಾತ್ರ_2 = lbuffered;

  /* Update lbuffered now, to avoid infinite recursion on write error.  */
  lbuffered = 0;

  ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, lbuf_buf, ಗಾತ್ರ_2) != ಗಾತ್ರ_2)
    write_error ();
}

/* Write LBUF to standard output.
   LBUF should contain at least FACTOR_PIPE_BUF bytes.
   If possible, write a prefix of LBUF that is newline terminated
   and contains <= FACTOR_PIPE_BUF bytes, so consumers can read atomically.
   But if the first FACTOR_PIPE_BUF bytes contain no newlines,
   give up on atomicity and just write the first FACTOR_PIPE_BUF bytes.  */
ಸ್ಥಿರ ಶೂನ್ಯ
lbuf_half_flush (ಶೂನ್ಯ)
{
  ಅಕ್ಷರ *nl = memrchr (lbuf_buf, '\n', ಗುಣಕ_PIPE_BUF);
  ಅಕ್ಷರ *suffix = nl ? nl + 1 : lbuf_buf + ಗುಣಕ_PIPE_BUF;
  idx_t ಪೂರ್ವಪ್ರತ್ಯಯ_ಗಾತ್ರ = suffix - lbuf_buf;
  idx_t suffix_ಗಾತ್ರ = lbuffered - ಪೂರ್ವಪ್ರತ್ಯಯ_ಗಾತ್ರ;
  lbuffered = ಪೂರ್ವಪ್ರತ್ಯಯ_ಗಾತ್ರ;
  lbuf_flush ();
  lbuffered = suffix_ಗಾತ್ರ;
  memmove (lbuf_buf, suffix, suffix_ಗಾತ್ರ);
}

/* Add a character C to lbuf_buf.  */
ಸ್ಥಿರ ಶೂನ್ಯ
lbuf_putc (ಅಕ್ಷರ c)
{
  lbuf_buf[lbuffered++] = c;
}

/* Add a newline to lbuf_buf.  Then, if enough bytes are already
   buffered, write the buffer atomically to standard output.  */
ಸ್ಥಿರ ಶೂನ್ಯ
lbuf_putnl (ಶೂನ್ಯ)
{
  lbuf_putc ('\n');

  /* Provide immediate output for interactive use.  */
  ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ಸಾಲು_buffered = -1;
  ಆದರೆ (ಸಾಲು_buffered < 0)
    ಸಾಲು_buffered = isatty (STDOUT_FILENO);

  ಆದರೆ (ಸಾಲು_buffered)
    lbuf_flush ();
  ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಗುಣಕ_PIPE_BUF <= lbuffered)
    lbuf_half_flush ();
}

/* Append the string representation of I to lbuf_buf, followed by
   everything from BUFEND to lbuf_buf's end.  Use the area just before
   BUFEND temporarily.  */
ಸ್ಥಿರ ಶೂನ್ಯ
lbuf_putint_append (mp_limb_t i, ಅಕ್ಷರ *bufend)
{
  ಅಕ್ಷರ *istr = bufend;
  ಮಾಡು
    {
      *--istr = '0' + i % 10;
      i /= 10;
    }
  ಆಗ (i);

  ಅಕ್ಷರ *p = lbuf_buf + lbuffered;
  ಮಾಡು
    *p++ = *istr++;
  ಆಗ (istr < lbuf_buf + ಗಾತ್ರ lbuf_buf);

  lbuffered = p - lbuf_buf;
}

/* Append the string representation of I to lbuf_buf.  */
ಸ್ಥಿರ ಶೂನ್ಯ
lbuf_putint (mp_limb_t i)
{
  lbuf_putint_append (i, lbuf_buf + ಗಾತ್ರ lbuf_buf);
}
ಸ್ಥಿರ ಶೂನ್ಯ
lbuf_putbitcnt (mp_bitcnt_t i)
{
  ಅಕ್ಷರ *bufend = lbuf_buf + ಗಾತ್ರ lbuf_buf;
  ಗಾಗಿ (; MP_LIMB_MAX < i; i /= 10)
    *--bufend = '0' + i % 10;
  lbuf_putint_append (i, bufend);
}

/* Append the string representation of T to lbuf_buf.  */
ಸ್ಥಿರ ಶೂನ್ಯ
print_uuint (uuint t)
{
  mp_limb_t t1 = hi (t), t0 = lo (t);
  ಅಕ್ಷರ *bufend = lbuf_buf + ಗಾತ್ರ lbuf_buf;

  ಆಗ (t1)
    {
      mp_limb_t r = t1 % BIG_POWER_ದ_10;
      t1 /= BIG_POWER_ದ_10;
      udiv_qrnnd (t0, r, r, t0, BIG_POWER_ದ_10);
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < LOG_BIG_POWER_ದ_10; i++)
        {
          *--bufend = '0' + r % 10;
          r /= 10;
        }
    }

  lbuf_putint_append (t0, bufend);
}

/* Buffer the mpz I to lbuf_buf, possibly writing if it is long.  */
ಸ್ಥಿರ ಶೂನ್ಯ
lbuf_putmpz (mpz_t ಸ್ಥಿರಾಂಕ i)
{
  idx_t sizeinbase = mpz_sizeinbase (i, 10);
  ಅಕ್ಷರ *lbuf_bufend = lbuf_buf + ಗಾತ್ರ lbuf_buf;
  ಅಕ್ಷರ *p = lbuf_buf + lbuffered;
  ಆದರೆ (sizeinbase < lbuf_bufend - p)
    {
      mpz_get_str (p, 10, i);
      p += sizeinbase;
      lbuffered = p - !p[-1] - lbuf_buf;
      ಆಗ (ಗುಣಕ_PIPE_BUF <= lbuffered)
        lbuf_half_flush ();
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      lbuf_flush ();
      ಅಕ್ಷರ *istr = ximalloc (sizeinbase + 1);
      mpz_get_str (istr, 10, i);
      idx_t istrlen = sizeinbase - !istr[sizeinbase - 1];
      ಆದರೆ (ತುಂಬಿದ_write (STDOUT_FILENO, istr, istrlen) != istrlen)
        write_error ();
      ಬಿಡುಗಡೆ (istr);
    }
}

/* Emit the factors of T, which is less than B^2 / 2.  */
ಸ್ಥಿರ ಶೂನ್ಯ
print_factors_single (uuint t)
{
  ರಚನೆ factors factors;

  print_uuint (t);
  lbuf_putc (':');

  ಗುಣಕ (&factors, hi (t), lo (t));

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ j = 0; j < factors.nfactors; j++)
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < factors.e[j]; k++)
      {
        lbuf_putc (' ');
        print_uuint (make_uuint (0, factors.p[j]));
        ಆದರೆ (print_exponents && factors.e[j] > 1)
          {
            lbuf_putc ('^');
            lbuf_putint (factors.e[j]);
            ನಿಲ್ಲಿಸು;
          }
      }

  ಆದರೆ (hi_is_ಗಣ (&factors.plarge))
    {
      lbuf_putc (' ');
      print_uuint (factors.plarge);
    }

  lbuf_putnl ();
}

/* Emit the factors of the indicated number.  If we have the option of using
   either algorithm, we select on the basis of the length of the number.
   For longer numbers, we prefer the MP algorithm even if the native algorithm
   has enough digits, because the algorithm is better.  The turnover point
   depends on the value.  */
ಸ್ಥಿರ bool
print_factors (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *ಒಳಹರಿವು)
{
  /* Skip initial spaces and '+'.  */
  ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *str = ಒಳಹರಿವು;
  ಆಗ (*str == ' ')
    str++;
  str += *str == '+';

  /* Try converting the number to one or two words.  If it fails, use GMP or
     print an error message.  The 2nd condition checks that the most
     significant bit of the two-word number is clear, in a typesize neutral
     way.  */
  uuint u;
  strtol_error err = strtouuint (&u, str);
  mpz_t t;

  ಆಯ್ಕೆ (err)
    {
    ಸ್ಥಿತಿ LONGINT_OK:
      ಆದರೆ (hi (u) >> (W_TYPE_ಗಾತ್ರ - 1) == 0)
        {
          devmsg ("[using single-precision arithmetic] ");
          print_factors_single (u);
          ಹಿಂತಿರುಗಿ true;
        }
      FALLTHROUGH;
    ಸ್ಥಿತಿ LONGINT_ಅತಿಪ್ರವಾಹ:
      /* Try GMP.  */
      ಆದರೆ (0 <= mpz_init_ಗಣ_str (t, str, 10))
        ನಿಲ್ಲಿಸು;
      FALLTHROUGH;

    ಸ್ಥಿತಿ LONGINT_INVALID:
    ಸ್ಥಿತಿ LONGINT_INVALID_SUFFIX_CHAR:
    ಸ್ಥಿತಿ LONGINT_INVALID_SUFFIX_CHAR_WITH_ಅತಿಪ್ರವಾಹ:
    ಪೂರ್ವನಿಯೋಜಿತ:
      error (0, 0, _("%s is not a valid positive integer"), quote (ಒಳಹರಿವು));
      ಹಿಂತಿರುಗಿ false;
    }

  devmsg ("[using arbitrary-precision arithmetic] ");
  ಆದರೆ (MP_ಗುಣಕ_USING_POLLARD_RHO_N_MAX < mp_ಗಾತ್ರ (t))
    xalloc_die ();

  lbuf_putmpz (t);
  lbuf_putc (':');
  ರಚನೆ mp_factors factors = mp_ಗುಣಕ (t);

  ಗಾಗಿ (idx_t j = 0; j < factors.nfactors; j++)
    ಗಾಗಿ (mp_bitcnt_t k = 0; k < factors.f[j].e; k++)
      {
        lbuf_putc (' ');
        lbuf_putmpz (factors.f[j].p);
        ಆದರೆ (print_exponents && factors.f[j].e > 1)
          {
            lbuf_putc ('^');
            lbuf_putbitcnt (factors.f[j].e);
            ನಿಲ್ಲಿಸು;
          }
      }

  mp_ಗುಣಕ_clear (&factors);
  mpz_clear (t);
  lbuf_putnl ();
  ಹಿಂತಿರುಗಿ true;
}

/* Output a usage diagnostic and exit with STATUS.  */
ಶೂನ್ಯ
usage (ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2)
{
  ಆದರೆ (ಸ್ಥಿತಿ_2 != EXIT_SUCCESS)
    emit_try_help ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಬರೆ (_("\
Usage: %s [OPTION] [ಸಂಖ್ಯೆ_2]...\n\
"),
              ಕಾರ್ಯಕ್ರಮ_name);
      fputs (_("\
Print the ಅವಿಭಾಜ್ಯ factors ದ each specified integer ಸಂಖ್ಯೆ_2.  If none\n\
are specified on the command ಸಾಲು, ಪಠ them from standard ಒಳಹರಿವು.\n\
\n\
"), stdout);
      oputs (_("\
  -h, --exponents\n\
         print repeated factors ಒಳ form p^e unless e is 1\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (ಆವೃತ್ತಿ_OPTION_DESCRIPTION);
      emit_ancillary_info (ಕಾರ್ಯಕ್ರಮ_NAME);
    }
  ನಿರ್ಗಮಿಸು (ಸ್ಥಿತಿ_2);
}

/* Read numbers from stdin, one per line, and output their factors.  */
ಸ್ಥಿರ bool
do_stdin (ಶೂನ್ಯ)
{
  bool ok = true;
  token_ಬಫರ್ tokenbuffer;

  init_tokenbuffer (&tokenbuffer);

  ಆಗ (true)
    {
      size_t token_ಉದ್ದಳತೆ = readtoken (stdin, DELIM, ಗಾತ್ರ (DELIM) - 1,
                                       &tokenbuffer);
      ಆದರೆ (token_ಉದ್ದಳತೆ == (size_t) -1)
        {
          ಆದರೆ (ferror (stdin))
            error (EXIT_FAILURE, errno, _("error reading input"));
          ನಿಲ್ಲಿಸು;
        }

      ok &= print_factors (tokenbuffer.ಬಫರ್);
    }
  ಬಿಡುಗಡೆ (tokenbuffer.ಬಫರ್);

  ಹಿಂತಿರುಗಿ ok;
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
  initialize_main (&argc, &argv);
  ಗಣ_ಕಾರ್ಯಕ್ರಮ_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  ಪೂರ್ಣಾಂಕ c;
  ಆಗ ((c = getopt_long (argc, argv, "h", long_options, ಖಾಲಿ)) != -1)
    {
      ಆಯ್ಕೆ (c)
        {
        ಸ್ಥಿತಿ 'h':  /* NetBSD used -h for this functionality first.  */
          print_exponents = true;
          ನಿಲ್ಲಿಸು;

        ಸ್ಥಿತಿ DEV_DEBUG_OPTION:
          dev_debug = true;
          ನಿಲ್ಲಿಸು;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_ಆವೃತ್ತಿ_CHAR (ಕಾರ್ಯಕ್ರಮ_NAME, AUTHORS);

        ಪೂರ್ವನಿಯೋಜಿತ:
          usage (EXIT_FAILURE);
        }
    }

  atexit (lbuf_flush);

  bool ok;
  ಆದರೆ (argc <= optind)
    ok = do_stdin ();
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ok = true;
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = optind; i < argc; i++)
        ಆದರೆ (! print_factors (argv[i]))
          ok = false;
    }

  ಹಿಂತಿರುಗಿ ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
