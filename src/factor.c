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


#అన్తర్భావ <config.h>
#అన్తర్భావ <getopt.h>
#అన్తర్భావ <stdbit.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <gmp.h>

#అన్తర్భావ "system.h"
#అన్తర్భావ "assure.h"
#అన్తర్భావ "full-write.h"
#అన్తర్భావ "quote.h"
#అన్తర్భావ "readtokens.h"
#అన్తర్భావ "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#నిర్వచించు కార్యక్రమం_NAME "factor"

#నిర్వచించు AUTHORS \
  proper_name ("Paul Rubin"),                                           \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"),   \
  proper_name_lite ("Niels Moller", "Niels M\303\266ller")

/* Token delimiters when reading from a file.  */
స్థిర అక్షరం స్థిరరాశి DELIM[] = "\n\t ";

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
#నిర్వచితమైతే GMP_LIMB_బిట్లు
# define W_TYPE_కొలత GMP_LIMB_బిట్లు
#లేకపోతే
/* An older GMP, or mini-gmp; guess the usual value.  */
# define W_TYPE_కొలత ULONG_వెడల్పు
#అయితే_ముగింపు

/* The maximum value of a word.  */
#నిర్వచించు MP_LIMB_MAX ((mp_limb_t) -1)

/* Check W_TYPE_SIZE's value, as it might be a guess.  */
static_assert (MP_LIMB_MAX >> (W_TYPE_కొలత - 1) == 1);

/* Check that the builder didn't specify something perverse like
   "-DMINI_GMP_LIMB_TYPE=short -DW_TYPE_SIZE=USHRT_WIDTH".
   This could result in undefined behavior due to signed integer
   overflow if a word promotes to int.  */
static_assert (గరిష్ఠ_పూర్ణాంకం < MP_LIMB_MAX);

#నిర్వచితం_కాకపోతే USE_LONGLONG_H
# ఒకవేళ (defined INT32_MAX && defined UINT32_MAX \
      && defined INT64_MAX && defined UINT64_MAX)
#  define USE_LONGLONG_H true
# లేకపోతే
#  define USE_LONGLONG_H false
# endif
#అయితే_ముగింపు

#ఒకవేళ USE_LONGLONG_H

/* Make definitions for longlong.h to make it do what it can do for us */

రకనిర్వచనం mp_limb_t UWtype;
రకనిర్వచనం గుర్తులేని పూర్ణాంకం UHWtype;
# undef UDWtype
రకనిర్వచనం int32_t SItype;
రకనిర్వచనం uint32_t USItype;
రకనిర్వచనం int64_t DItype;
రకనిర్వచనం uint64_t UDItype;
# define LONGLONG_STANDALONE     /* Don't require GMP's longlong.h mdep files */

/* FIXME make longlong.h really standalone, so that ASSERT, __GMP_DECLSPEC
   and __GMP_GNUC_PREREQ need not be defined here.  */
# define ASSERT(x)
# define __GMP_DECLSPEC
# ifndef __GMP_GNUC_PREREQ
#  ఒకవేళ defined __GNUC__ && defined __GNUC_MINOR__
#   define __GMP_GNUC_PREREQ(a, b) ((a) < __GNUC__ + ((b) <= __GNUC_MINOR__))
#  లేకపోతే
#   define __GMP_GNUC_PREREQ(a, b) false
#  endif
# endif

/* longlong.h uses these macros only in certain system compiler combinations.
   Ensure usage to pacify -Wunused-macros.  */
# ఒకవేళ defined ASSERT || defined __GMP_DECLSPEC || defined __GMP_GNUC_PREREQ
# endif

/* longlong.h uses casts even when useless.  */
# ఒకవేళ 14 <= __GNUC__
#  pragma GCC diagnostic ignored "-Wuseless-cast"
# endif

# ఒకవేళ _ARCH_PPC
#  define HAVE_HOST_CPU_FAMILY_powerpc 1
# endif
# include "longlong.h"
#అయితే_ముగింపు

/* 2*3*5*7*11...*101 fits in 128 bits, and has 26 prime factors.
   This code can be extended in the future as needed; show as an example
   2*3*5*7*11...*193 which fits in 257 bits, and has 44 prime factors.  */
#ఒకవేళ 2 * W_TYPE_కొలత <= 128
గణన { MAX_NFACTS = 26 };
#లేదావేళ 2 * W_TYPE_కొలత <= 257
గణన { MAX_NFACTS = 44 };
#లేకపోతే
# error "configuration has a wide word; please update MAX_NFACTS definition"
#అయితే_ముగింపు

గణన
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1
};

స్థిర నిర్మాణం option స్థిరరాశి long_options[] =
{
  {"exponents", no_వాదం, ఖాళీ, 'h'},
  {"-debug", no_వాదం, ఖాళీ, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_సంస్కరణ_OPTION_DECL},
  {ఖాళీ, 0, ఖాళీ, 0}
};

/* If true, use p^e output format.  */
స్థిర bool print_exponents;

/* This represents an unsigned integer twice as wide as a word.  */
రకనిర్వచనం నిర్మాణం { mp_limb_t uu[2]; } uuint;

/* Accessors and constructors for the type.  Programs should not
   access the type's internals directly, in case some future version
   replaces the type with unsigned __int256 or whatever.  */
స్థిర mp_limb_t lo (uuint u) { తిరిగి u.uu[0]; }
స్థిర mp_limb_t hi (uuint u) { తిరిగి u.uu[1]; }
స్థిర శూన్య hiset (uuint *u, mp_limb_t hi) { u->uu[1] = hi; }
స్థిర bool hi_is_సమితి (uuint స్థిరరాశి *pu) { తిరిగి pu->uu[1] != 0; }
స్థిర శూన్య
uuset (mp_limb_t *phi, mp_limb_t *plo, uuint uu)
{
  *phi = hi (uu);
  *plo = lo (uu);
}
స్థిర uuint
make_uuint (mp_limb_t hi, mp_limb_t lo)
{
  తిరిగి (uuint) {{lo, hi}};
}

/* BIG_POWER_OF_10 is a positive power of 10 that fits in a word.
   The larger it is, the more efficient the code will likely be.
   LOG_BIG_POWER_OF_10 = log (BIG_POWER_OF_10).  */
#ఒకవేళ W_TYPE_కొలత < 4
# error "Configuration error; platform word is impossibly narrow"
#లేదావేళ W_TYPE_కొలత < 30
/* An unusually narrow word.  */
స్థిర mp_limb_t స్థిరరాశి BIG_POWER_యొక్క_10 = 10;
గణన { LOG_BIG_POWER_యొక్క_10 = 1 };
#లేదావేళ W_TYPE_కొలత < 64
/* Almost surely a 32-bit word.  */
స్థిర mp_limb_t స్థిరరాశి BIG_POWER_యొక్క_10 = 1000000000;
గణన { LOG_BIG_POWER_యొక్క_10 = 9 };
#లేదావేళ W_TYPE_కొలత < 128
/* Almost surely a 64-bit word.  */
స్థిర mp_limb_t స్థిరరాశి BIG_POWER_యొక్క_10 = 10000000000000000000llu;
గణన { LOG_BIG_POWER_యొక్క_10 = 19 };
#లేకపోతే
/* An unusually wide word.  */
స్థిర mp_limb_t స్థిరరాశి BIG_POWER_యొక్క_10 =
  (mp_limb_t) 10000000000000000000llu * 10000000000000000000llu;
గణన { LOG_BIG_POWER_యొక్క_10 = 38 };
#అయితే_ముగింపు

/* Check that struct factors can use unsigned char to record a uuint's
   prime factor's multiplicity, which is at most 2 * W_TYPE_SIZE - 1.  */
static_assert (2 * W_TYPE_కొలత - 1 <= UCHAR_MAX);

/* Likewise for recording the number of prime factors.  */
static_assert (MAX_NFACTS <= UCHAR_MAX);

/* Prime factors of a uuint.  At most one is a uuint.  */
నిర్మాణం factors
{
  /* If PLARGE.uu[1] is nonzero, PLARGE is a double-word prime factor
     with multiplicity 1; otherwise, PLARGE is not a factor and
     PLARGE.uu[0] might not be initialized.  */
  uuint plarge;

  /* Distinct single-word prime factors, their multiplicities,
     and their number.  */
  mp_limb_t     p[MAX_NFACTS];
  గుర్తులేని అక్షరం e[MAX_NFACTS];
  గుర్తులేని అక్షరం nfactors;
};

/* An mpz's prime factor and multiplicity.  */
నిర్మాణం mp_గుణకం
{
  mpz_t p;
  mp_bitcnt_t e;
};

/* Prime factors of an mpz_t.  */
నిర్మాణం mp_factors
{
  idx_t nfactors;
  idx_t nalloc;

  /* A vector of distinct prime factors, a count of the factors,
     and the number of allocated slots in the vector.  */
  నిర్మాణం mp_గుణకం *f COUNTED_BY (nfactors);
};

స్థిర శూన్య గుణకం (నిర్మాణం factors *, mp_limb_t, mp_limb_t);
స్థిర శూన్య గుణకం_పైకి (నిర్మాణం factors *, mp_limb_t, mp_limb_t, idx_t);

/* Set (w1,w0) = u * v.  */
#నిర్వచితం_కాకపోతే umul_ppmm
/* Speed things up if there is an unsigned type uuroom_t that is wide
   enough to hold two words.  */
# ఒకవేళ W_TYPE_కొలత <= UINTMAX_వెడల్పు / 2
#  define uuroom_t uintmax_t
# elif W_TYPE_కొలత <= 64 && defined __SIZEOF_INT128__
#  define uuroom_t గుర్తులేని __int128
# endif
# ifdef uuroom_t
#  define umul_ppmm(w1, w0, u, v)		\
    చేయి {					\
      uuroom_t _u = u, _w = _u * (v);		\
      (w1) = _w >> W_TYPE_కొలత;			\
      (w0) = _w;				\
    } అయితే (false)
# endif
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే umul_ppmm
స్థిర mp_limb_t స్థిరరాశి __ll_B = (mp_limb_t) 1 << (W_TYPE_కొలత / 2);
స్థిర mp_limb_t __ll_lowpart (mp_limb_t t) { తిరిగి t & (__ll_B - 1); }
స్థిర mp_limb_t __ll_highpart (mp_limb_t t) { తిరిగి t >> (W_TYPE_కొలత / 2); }
# define umul_ppmm(w1, w0, u, v)                                        \
  చేయి {                                                                  \
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
    ఒకవేళ (ckd_కూడు (&__x1, __x1, __x2))	/* Did this give a carry?  */	\
      __x3 += __ll_B;		/* Yes, add it in the proper pos.  */	\
                                                                        \
    (w1) = __x3 + __ll_highpart (__x1);                                 \
    (w0) = (__x1 << W_TYPE_కొలత / 2) + __ll_lowpart (__x0);             \
  } అయితే (false)
#అయితే_ముగింపు

/* Set (q,r) to the quotient and remainder of dividing (n1,n0) by d.  */
#ఒకవేళ !defined udiv_qrnnd || defined UDIV_NEEDS_NORMALIZATION
/* Define our own, not needing normalization.  This function is
   currently not performance critical, so keep it simple.  Similar to
   the mod macro below.  */
# undef udiv_qrnnd
# define udiv_qrnnd(q, r, n1, n0, d)                                    \
  చేయి {                                                                  \
    mp_limb_t __d1 = d, __d0 = 0, __q = 0, __r1 = n1, __r0 = n0;	\
                                                                        \
    affirm (__r1 < __d1);                                               \
    కొరకు (పూర్ణాంకం __i = W_TYPE_కొలత; __i > 0; __i--)                         \
      {                                                                 \
        rsh2 (__d1, __d0, __d1, __d0, 1);                               \
        __q <<= 1;                                                      \
        ఒకవేళ (ge2 (__r1, __r0, __d1, __d0))                               \
          {                                                             \
            __q++;                                                      \
            sub_ddmmss (__r1, __r0, __r1, __r0, __d1, __d0);            \
          }                                                             \
      }                                                                 \
    (r) = __r0;                                                         \
    (q) = __q;                                                          \
  } అయితే (false)
#అయితే_ముగింపు

/* Set (sh,sl) = (ah,al) + (bh,bl).  Overflow wraps around.  */
#ఒకవేళ !defined కూడు_ssaaaa
# define కూడు_ssaaaa(sh, sl, ah, al, bh, bl)                             \
   ((sh) = (ah) + (bh) + ckd_కూడు (&(sl), al, bl))
#అయితే_ముగింపు

/* Set (rh,rl) = (ah,al) >> cnt, where 0 < cnt < W_TYPE_SIZE.  */
#నిర్వచించు rsh2(rh, rl, ah, al, cnt)                                       \
  చేయి {                                                                  \
    (rl) = ((ah) << (W_TYPE_కొలత - (cnt))) | ((al) >> (cnt));           \
    (rh) = (ah) >> (cnt);                                               \
  } అయితే (false)

/* Set (rh,rl) = (ah,al) << cnt, where 0 < cnt < W_TYPE_SIZE.
   Overflow wraps around.  */
#నిర్వచించు lsh2(rh, rl, ah, al, cnt)                                       \
  చేయి {                                                                  \
    (rh) = ((ah) << cnt) | ((al) >> (W_TYPE_కొలత - (cnt)));             \
    (rl) = (al) << (cnt);                                               \
  } అయితే (false)

/* (ah,hl) < (bh,bl)?  */
స్థిర bool
lt2 (mp_limb_t ah, mp_limb_t al, mp_limb_t bh, mp_limb_t bl)
{
  mp_limb_t dh, dl;
  bool vh = ckd_sub (&dh, ah, bh);
  mp_limb_t vl = ckd_sub (&dl, al, bl);
  తిరిగి vh | ckd_sub (&dh, dh, vl);
}

/* (ah,hl) >= (bh,bl)?  */
స్థిర bool
ge2 (mp_limb_t ah, mp_limb_t al, mp_limb_t bh, mp_limb_t bl)
{
  తిరిగి !lt2 (ah, al, bh, bl);
}

/* Set (rh,rl) = (ah,al) - (bh,bl).  Overflow wraps around.  */
#నిర్వచితం_కాకపోతే sub_ddmmss
# define sub_ddmmss(rh, rl, ah, al, bh, bl)                             \
   ((rh) = (ah) - (bh) - ckd_sub (&(rl), al, bl))
#అయితే_ముగింపు

/* Set r = (a - b) mod n, where a < n & b <= n.  */
#నిర్వచించు submod(r,a,b,n)                                                 \
  చేయి {                                                                  \
    mp_limb_t _s, _t = -ckd_sub (&_s, a, b);				\
    (r) = ((n) & _t) + _s;						\
  } అయితే (false)

/* Set r = (a + b) mod n, where a < n & b <= n.  */
#నిర్వచించు addmod(r,a,b,n)                                                 \
  submod (r, a, (n) - (b), n)

/* Modular two-word addition and subtraction.  For performance reasons, the
   most significant bit of n1 must be clear.  The destination variables must be
   distinct from the mod operand.  */
#నిర్వచించు addmod2(r1, r0, a1, a0, b1, b0, n1, n0)                         \
  చేయి {                                                                  \
    కూడు_ssaaaa (r1, r0, a1, a0, b1, b0);				\
    ఒకవేళ (ge2 (r1, r0, n1, n0))						\
      sub_ddmmss (r1, r0, r1, r0, n1, n0);				\
  } అయితే (false)
#నిర్వచించు submod2(r1, r0, a1, a0, b1, b0, n1, n0)                         \
  చేయి {                                                                  \
    bool _v1 = ckd_sub (&(r1), a1, b1);					\
    mp_limb_t _v0 = ckd_sub (&(r0), a0, b0);				\
    ఒకవేళ (_v1 | ckd_sub (&(r1), r1, _v0))					\
      కూడు_ssaaaa (r1, r0, r1, r0, n1, n0);				\
  } అయితే (false)

/* Return 0 if x < B/2, MP_LIMB_MAX otherwise.  */
స్థిర mp_limb_t
highbit_గమ్యం_ముసుగు (mp_limb_t x)
{
  తిరిగి - (x >> (W_TYPE_కొలత - 1));
}

/* Return (a1,a0) mod (d1,d0), where d1 != 0.  */
ATTRIBUTE_శుద్ధ స్థిర uuint
mod2 (mp_limb_t a1, mp_limb_t a0, mp_limb_t d1, mp_limb_t d0)
{
  affirm (d1 != 0);

  ఒకవేళ (a1)
    {
      పూర్ణాంకం cntd = stdc_leading_zeros (d1);
      పూర్ణాంకం cnta = stdc_leading_zeros (a1);
      పూర్ణాంకం cnt = cntd - cnta;
      ఒకవేళ (0 < cnt)
        {
          lsh2 (d1, d0, d1, d0, cnt);
          కొరకు (పూర్ణాంకం i = 0; i < cnt; i++)
            {
              ఒకవేళ (ge2 (a1, a0, d1, d0))
                sub_ddmmss (a1, a0, a1, a0, d1, d0);
              rsh2 (d1, d0, d1, d0, 1);
            }
        }
    }

  తిరిగి make_uuint (a1, a0);
}

/* Return the greatest common divisor of a and b,
   where b is odd.  */
ATTRIBUTE_CONST
స్థిర mp_limb_t
గరిష్ఠ_సామాన్య_భాజకం_odd (mp_limb_t a, mp_limb_t b)
{
  ఒకవేళ (a == 0)
    తిరిగి b;

  /* Take out least significant one bit, to make room for sign */
  b >>= 1;

  కొరకు (;;)
    {
      mp_limb_t t;
      mp_limb_t bgta;

      assume (a);
      mp_limb_t ao = a >> stdc_trailing_zeros (a);

      t = (ao >> 1) - b;
      ఒకవేళ (t == 0)
        తిరిగి ao;

      bgta = highbit_గమ్యం_ముసుగు (t);

      /* b <-- min (a, b) */
      b += (bgta & t);

      /* a <-- |a - b| */
      a = (t ^ bgta) - bgta;
    }
}

/* Return the greatest common divisor of (a1,a0) and (b1,b0),
   where (b1,b0) is odd.  */
ATTRIBUTE_శుద్ధ స్థిర uuint
gcd2_odd (mp_limb_t a1, mp_limb_t a0, mp_limb_t b1, mp_limb_t b0)
{
  affirm (b0 & 1);

  ఒకవేళ (!a0)
    {
      a0 = a1, a1 = 0;
      ఒకవేళ (!a0)
        తిరిగి make_uuint (b1, b0);
    }
  వెళ్ళు make_A_odd;

  కొరకు (;;)
    {
      ఒకవేళ ((b1 | a1) == 0)
        తిరిగి make_uuint (0, గరిష్ఠ_సామాన్య_భాజకం_odd (b0, a0));

      ఒకవేళ (lt2 (b1, b0, a1, a0))
        {
          sub_ddmmss (a1, a0, a1, a0, b1, b0);
          ఒకవేళ (!a0)
            a0 = a1, a1 = 0;
        make_A_odd:
          assume (a0);
          పూర్ణాంకం ctz = stdc_trailing_zeros (a0);
          ఒకవేళ (ctz)
            rsh2 (a1, a0, a1, a0, ctz);
        }
      లేకపోతే
        {
          sub_ddmmss (b1, b0, b1, b0, a1, a0);
          ఒకవేళ (!b0)
            {
              b0 = b1, b1 = 0;
              ఒకవేళ (!b0)
                ఆపు;
            }
          పూర్ణాంకం ctz = stdc_trailing_zeros (b0);
          ఒకవేళ (ctz)
            rsh2 (b1, b0, b1, b0, ctz);
        }
    }

  తిరిగి make_uuint (a1, a0);
}

/* Store into FACTORS a prime factor PRIME with multiplicity M.  */
స్థిర శూన్య
గుణకం_చొప్పించు_multiplicity (నిర్మాణం factors *factors,
                            mp_limb_t ప్రధానసంఖ్య, పూర్ణాంకం m)
{
  పూర్ణాంకం nfactors = factors->nfactors;
  mp_limb_t *p = factors->p;
  గుర్తులేని అక్షరం *e = factors->e;

  /* Locate position for insert new or increment e.  */
  పూర్ణాంకం i;
  కొరకు (i = nfactors; 0 < i; i--)
    {
      ఒకవేళ (p[i - 1] < ప్రధానసంఖ్య)
        ఆపు;
      ఒకవేళ (p[i - 1] == ప్రధానసంఖ్య)
        {
          e[i - 1] += m;
          తిరిగి;
        }
    }

  factors->nfactors = nfactors + 1;
  memmove (&p[i + 1], &p[i], (nfactors - i) * పరిమాణం *p);
  memmove (&e[i + 1], &e[i], (nfactors - i) * పరిమాణం *e);
  e[i] = m;
  p[i] = ప్రధానసంఖ్య;
}

/* Store into FACTORS a prime factor PRIME.  */
స్థిర శూన్య
గుణకం_చొప్పించు (నిర్మాణం factors *factors, mp_limb_t ప్రధానసంఖ్య)
{
  గుణకం_చొప్పించు_multiplicity (factors, ప్రధానసంఖ్య, 1);
}

/* Store into FACTORS a prime factor (P1,P0).  If P1 != 0,
   FACTORS must not already contain a large prime factor.  */
స్థిర శూన్య
గుణకం_చొప్పించు_large (నిర్మాణం factors *factors,
                     mp_limb_t p1, mp_limb_t p0)
{
  ఒకవేళ (p1 > 0)
    {
      affirm (!hi_is_సమితి (&factors->plarge));
      factors->plarge = make_uuint (p1, p0);
    }
  లేకపోతే
    గుణకం_చొప్పించు (factors, p0);
}

#నిర్వచితం_కాకపోతే mpz_inits

# include <stdarg.h>

# define mpz_inits(...) mpz_va_init (mpz_init, __VA_ARGS__)
# define mpz_clears(...) mpz_va_init (mpz_clear, __VA_ARGS__)

స్థిర శూన్య
mpz_va_init (శూన్య (*mpz_single_init)(mpz_t), ...)
{
  va_list ap;

  va_ప్రారంభం (ap, mpz_single_init);

  mpz_t *mpz;
  అయితే ((mpz = va_arg (ap, mpz_t *)))
    mpz_single_init (*mpz);

  va_చివర (ap);
}
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే mpn_tdiv_qr

స్థిర శూన్య
copy_mpn_from_mpz (mp_limb_t *p, mp_కొలత_t n, mpz_t z)
{
  mp_కొలత_t zsize = mpz_కొలత (z);
  mpn_copyi (p, mpz_limbs_read (z), zsize);
  mpn_zero (p + zsize, n - zsize);
}

స్థిర శూన్య
mpn_tdiv_qr (mp_limb_t *qp, mp_limb_t *rp, MAYBE_వాడని mp_కొలత_t qxn,
             mp_limb_t స్థిరరాశి *np, mp_కొలత_t nn,
             mp_limb_t స్థిరరాశి *dp, mp_కొలత_t dn)
{
  mpz_t q, r, n, d;
  mpz_inits (q, r, ఖాళీ);
  mpz_tdiv_qr (q, r, mpz_roinit_n (n, np, nn), mpz_roinit_n (d, dp, dn));
  copy_mpn_from_mpz (qp, nn - dn + 1, q);
  copy_mpn_from_mpz (rp, dn, r);
  mpz_clears (q, r, ఖాళీ);
}
#అయితే_ముగింపు

స్థిర నిర్మాణం mp_factors mp_గుణకం (mpz_t);

/* Return an empty set of factors.  */
స్థిర నిర్మాణం mp_factors
mp_no_factors (శూన్య)
{
  తిరిగి (నిర్మాణం mp_factors) {0};
}

/* Free storage allocated for FACTORS, making it uninitialized.  */
స్థిర శూన్య
mp_గుణకం_clear (నిర్మాణం mp_factors *factors)
{
  నిర్మాణం mp_గుణకం *f = factors->f;
  కొరకు (idx_t i = 0; i < factors->nfactors; i++)
    mpz_clear (f[i].p);
  విడుదల (f);
}

/* Store into FACTORS a prime factor PRIME with multiplicity M.  */
స్థిర శూన్య
mp_గుణకం_చొప్పించు (నిర్మాణం mp_factors *factors, mpz_t ప్రధానసంఖ్య, mp_bitcnt_t m)
{
  idx_t nfactors = factors->nfactors;
  నిర్మాణం mp_గుణకం *f = factors->f;
  idx_t i;

  /* Locate position for insert new or increment e.  */
  కొరకు (i = nfactors; 0 < i; i--)
    {
      పూర్ణాంకం sgn = mpz_cmp (f[i - 1].p, ప్రధానసంఖ్య);
      ఒకవేళ (sgn < 0)
        ఆపు;
      ఒకవేళ (sgn == 0)
        {
          f[i - 1].e += m;
          తిరిగి;
        }
    }

  ఒకవేళ (nfactors == factors->nalloc)
    factors->f = f = xpalloc (f, &factors->nalloc, 1, -1, పరిమాణం *f);

  factors->nfactors = nfactors + 1;
  memmove (&f[i + 1], &f[i], (nfactors - i) * పరిమాణం *f);
  f[i].e = m;
  mpz_init_సమితి (f[i].p, ప్రధానసంఖ్య);
}

/* Store into FACTORS a prime factor PRIME with multiplicity M.  */
స్థిర శూన్య
mp_గుణకం_insert1 (నిర్మాణం mp_factors *factors, mp_limb_t ప్రధానసంఖ్య, mp_bitcnt_t m)
{
  mpz_t pz = MPZ_ROINIT_N (&ప్రధానసంఖ్య, 1);
  mp_గుణకం_చొప్పించు (factors, pz, m);
}


/* primes_ptab[i] is prime i, where the zeroth prime is 3.
   However, the last 7 entries are sentinels.  */
#నిర్వచించు P(a,b,c) a,
స్థిర int_least16_t స్థిరరాశి ప్రధానసంఖ్యలు_ptab[] = {
#అన్తర్భావ "primes.h"
0,0,0,0,0,0,0                           /* 7 sentinels for 8-way loop */
};
#నిర్వచనం_రద్దు P

గణన { ప్రధానసంఖ్యలు_PTAB_ENTRIES = countof (ప్రధానసంఖ్యలు_ptab) - 8 + 1 };

నిర్మాణం ప్రధానసంఖ్యలు_dtab
{
  mp_limb_t binv, lim;
};

/* primes_dtab[i].binv is the multiplicative inverse of prime i
   modulo B, where the zeroth prime is 3.  That is,
   ((primes_dtab[i].binv) * (prime i)) mod B = 1.
   primes_dtab[i].lim is the maximum value that won't overflow in
   mp_limb_t arithmetic when multiplied by the ith prime.
   However, the last 7 entries of primes_dtab are sentinels.  */
#నిర్వచించు P(a,b,c) {b,c},
స్థిర స్థిరరాశి నిర్మాణం ప్రధానసంఖ్యలు_dtab ప్రధానసంఖ్యలు_dtab[] = {
#అన్తర్భావ "primes.h"
{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0} /* 7 sentinels for 8-way loop */
};
#నిర్వచనం_రద్దు P

/* Verify that a word is not wider than
   the integers used to generate primes.h.  */
static_assert (W_TYPE_కొలత <= WIDE_UINT_బిట్లు);

/* debugging for developers.  Enables devmsg().
   This flag is used only in the GMP code.  */
స్థిర bool dev_debug = false;

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
#నిర్వచితం_కాకపోతే MR_REPS
# define MR_REPS 24
#అయితే_ముగింపు

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
స్థిర అంతఃవరుస mp_limb_t
divblock (నిర్మాణం factors *factors, mp_limb_t t0, నిర్మాణం ప్రధానసంఖ్యలు_dtab స్థిరరాశి *pd,
          idx_t i, పూర్ణాంకం ioff)
{
  కొరకు (;;)
    {
      mp_limb_t q = t0 * pd[ioff].binv;
      ఒకవేళ (LIKELY (pd[ioff].lim < q))
        తిరిగి t0;
      t0 = q;
      గుణకం_చొప్పించు (factors, ప్రధానసంఖ్యలు_ptab[i + ioff]);
    }
}

/* Insert into FACTORS the factors of (T1,T0) found via trial division.
   The candidate factors are 2 and the primes in the primes table.
   However, primes less than prime I have
   already been considered, and need not be looked at again.
   Return (T1,T0) divided by the factors found.  */
స్థిర uuint
గుణకం_using_division (నిర్మాణం factors *factors, mp_limb_t t1, mp_limb_t t0,
                       idx_t i)
{
  ఒకవేళ (t0 % 2 == 0)
    {
      పూర్ణాంకం cnt;

      ఒకవేళ (t0 == 0)
        {
          assume (t1);
          cnt = stdc_trailing_zeros (t1);
          t0 = t1 >> cnt;
          t1 = 0;
          cnt += W_TYPE_కొలత;
        }
      లేకపోతే
        {
          cnt = stdc_trailing_zeros (t0);
          rsh2 (t1, t0, t1, t0, cnt);
        }

      గుణకం_చొప్పించు_multiplicity (factors, 2, cnt);
    }

  కొరకు (; t1 > 0 && i < ప్రధానసంఖ్యలు_PTAB_ENTRIES; i++)
    {
      mp_limb_t p = ప్రధానసంఖ్యలు_ptab[i];
      కొరకు (;;)
        {
          mp_limb_t q1, q0, hi;
          MAYBE_వాడని mp_limb_t lo;

          q0 = t0 * ప్రధానసంఖ్యలు_dtab[i].binv;
          umul_ppmm (hi, lo, q0, p);
          ఒకవేళ (hi > t1)
            ఆపు;
          hi = t1 - hi;
          q1 = hi * ప్రధానసంఖ్యలు_dtab[i].binv;
          ఒకవేళ (LIKELY (q1 > ప్రధానసంఖ్యలు_dtab[i].lim))
            ఆపు;
          t1 = q1; t0 = q0;
          గుణకం_చొప్పించు (factors, p);
        }
    }

  కొరకు (; i < ప్రధానసంఖ్యలు_PTAB_ENTRIES; i += 8)
    {
      స్థిరరాశి నిర్మాణం ప్రధానసంఖ్యలు_dtab *pd = &ప్రధానసంఖ్యలు_dtab[i];
      t0 = divblock (factors, t0, pd, i, 0);
      t0 = divblock (factors, t0, pd, i, 1);
      t0 = divblock (factors, t0, pd, i, 2);
      t0 = divblock (factors, t0, pd, i, 3);
      t0 = divblock (factors, t0, pd, i, 4);
      t0 = divblock (factors, t0, pd, i, 5);
      t0 = divblock (factors, t0, pd, i, 6);
      t0 = divblock (factors, t0, pd, i, 7);

      ఒకవేళ (ప్రధానసంఖ్యలు_PTAB_ENTRIES <= i + 8)
        ఆపు;
      int_least32_t p = ప్రధానసంఖ్యలు_ptab[i + 8];
      ఒకవేళ (p * p > t0)
        ఆపు;
    }

  తిరిగి make_uuint (t1, t0);
}

/* Return the number of limbs in positive N.  */
స్థిర mp_కొలత_t
mp_కొలత (mpz_t n)
{
  /* Tell the compiler that N is positive; this can speed up access to N.  */
  assume (0 < mpz_sgn (n));

  తిరిగి mpz_కొలత (n);
}

/* Insert into MP_FACTORS the factors of N if N < B^2 / 2,
   and return true.  Otherwise, return false.
   Primes less than prime PRIME_IDX have
   already been considered, and need not be looked at again.  */
స్థిర bool
mp_ముగింపు_పైకి_లోపల_single (నిర్మాణం mp_factors *mp_factors, mpz_t n,
                        idx_t ప్రధానసంఖ్య_idx)
{
  ఒకవేళ (2 < mp_కొలత (n))
    తిరిగి false;
  mp_limb_t n1 = mpz_getlimbn (n, 1);
  ఒకవేళ (n1 >> (W_TYPE_కొలత - 1))
    తిరిగి false;
  mp_limb_t n0 = mpz_getlimbn (n, 0);
  mpz_సమితి_ui (n, 1);

  నిర్మాణం factors factors;
  గుణకం_పైకి (&factors, n1, n0, ప్రధానసంఖ్య_idx);

  ఒకవేళ (hi_is_సమితి (&factors.plarge))
    {
      mpz_t p = MPZ_ROINIT_N (factors.plarge.uu, 2);
      mp_గుణకం_చొప్పించు (mp_factors, p, 1);
    }

  కొరకు (పూర్ణాంకం i = factors.nfactors; 0 < i; i--)
    {
      mpz_t p = MPZ_ROINIT_N (&factors.p[i - 1], 1);
      mp_గుణకం_చొప్పించు (mp_factors, p, factors.e[i - 1]);
    }

  తిరిగి true;
}

/* Insert into MP_FACTORS the factors of N if N < B^2 / 2, and
   return true.  Otherwise, return false.  N must be odd.  */
స్థిర bool
mp_ముగింపు_లోపల_single (నిర్మాణం mp_factors *mp_factors, mpz_t n)
{
  తిరిగి mp_ముగింపు_పైకి_లోపల_single (mp_factors, n, 0);
}

/* Return some or all factors of T.
   Divide T by the factors found.  */
స్థిర నిర్మాణం mp_factors
mp_గుణకం_using_division (mpz_t t)
{
  devmsg ("[trial division] ");
  నిర్మాణం mp_factors factors = mp_no_factors ();

  mp_bitcnt_t m = mpz_scan1 (t, 0);
  ఒకవేళ (m)
    {
      mpz_fdiv_q_2exp (t, t, m);
      mp_గుణకం_insert1 (&factors, 2, m);
      ఒకవేళ (mp_ముగింపు_లోపల_single (&factors, t))
        తిరిగి factors;
    }

  కొరకు (idx_t i = 0; i < ప్రధానసంఖ్యలు_PTAB_ENTRIES; i++)
    {
      mp_limb_t d = ప్రధానసంఖ్యలు_ptab[i];
      కొరకు (m = 0; mpz_divisible_ui_p (t, d); m++)
        {
          mpz_divexact_ui (t, t, d);
          ఒకవేళ (mp_ముగింపు_పైకి_లోపల_single (&factors, t, i))
            {
              mp_గుణకం_insert1 (&factors, d, m + 1);
              తిరిగి factors;
            }
        }
      ఒకవేళ (m)
        mp_గుణకం_insert1 (&factors, d, m);
      static_assert (వర్గం_యొక్క_మొదటి_OMITTED_ప్రధానసంఖ్య
                     <= MIN (MP_LIMB_MAX, ULONG_MAX));
      ఒకవేళ (mpz_cmp_ui (t, d * d) < 0)
        ఆపు;
    }

  తిరిగి factors;
}

/* Entry i contains (2i+1)^(-1) mod 2^8.  */
స్థిర స్థిరరాశి గుర్తులేని అక్షరం binvert_పట్టిక[128] =
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
స్థిర mp_limb_t
binv_limb (mp_limb_t n)
{
  mp_limb_t inv = binvert_పట్టిక[(n / 2) & 0x7F];
  ఒకవేళ ( 8 < W_TYPE_కొలత) inv = 2 * inv - inv * inv * n;
  ఒకవేళ (16 < W_TYPE_కొలత) inv = 2 * inv - inv * inv * n;
  ఒకవేళ (32 < W_TYPE_కొలత) inv = 2 * inv - inv * inv * n;
  కొరకు (పూర్ణాంకం invbits = 64; invbits < W_TYPE_కొలత; invbits *= 2)
    inv = 2 * inv - inv * inv * n;
  తిరిగి inv;
}

/* q = u / d, assuming d|u.  */
#నిర్వచించు divexact_21(q1, q0, u1, u0, d)                                  \
  చేయి {                                                                  \
    mp_limb_t _di, _q0;							\
    _di = binv_limb (d);						\
    _q0 = (u0) * _di;                                                   \
    ఒకవేళ ((u1) >= (d))                                                    \
      {                                                                 \
        mp_limb_t _p1;							\
        MAYBE_వాడని mp_limb_t _p0;					\
        umul_ppmm (_p1, _p0, _q0, d);                                   \
        (q1) = ((u1) - _p1) * _di;                                      \
        (q0) = _q0;                                                     \
      }                                                                 \
    లేకపోతే                                                                \
      {                                                                 \
        (q0) = _q0;                                                     \
        (q1) = 0;                                                       \
      }                                                                 \
  } అయితే (false)

/* x B (mod n).  */
#నిర్వచించు redcify(r_prim, r, n)                                           \
  చేయి {                                                                  \
    MAYBE_వాడని mp_limb_t _redcify_q;					\
    udiv_qrnnd (_redcify_q, r_prim, r, 0, n);                           \
  } అయితే (false)

/* x B^2 (mod n).  Requires x > 0, n1 < B/2.  */
#నిర్వచించు redcify2(r1, r0, x, n1, n0)                                     \
  చేయి {                                                                  \
    mp_limb_t _r1, _r0, _i;						\
    ఒకవేళ ((x) < (n1))                                                     \
      {                                                                 \
        _r1 = (x); _r0 = 0;                                             \
        _i = W_TYPE_కొలత;                                               \
      }                                                                 \
    లేకపోతే                                                                \
      {                                                                 \
        _r1 = 0; _r0 = (x);                                             \
        _i = 2 * W_TYPE_కొలత;                                           \
      }                                                                 \
    అయితే (_i-- > 0)                                                    \
      {                                                                 \
        lsh2 (_r1, _r0, _r1, _r0, 1);                                   \
        ఒకవేళ (ge2 (_r1, _r0, n1, n0))					\
          sub_ddmmss (_r1, _r0, _r1, _r0, n1, n0);			\
      }                                                                 \
    (r1) = _r1;                                                         \
    (r0) = _r0;                                                         \
  } అయితే (false)

/* Modular two-word multiplication, r = a * b mod m, with mi = m^(-1) mod B.
   Both a and b must be in redc form, the result will be in redc form too.  */
స్థిర అంతఃవరుస mp_limb_t
mulredc (mp_limb_t a, mp_limb_t b, mp_limb_t m, mp_limb_t mi)
{
  mp_limb_t rh, rl, q, th, xh;
  MAYBE_వాడని mp_limb_t tl;

  umul_ppmm (rh, rl, a, b);
  q = rl * mi;
  umul_ppmm (th, tl, q, m);
  ఒకవేళ (ckd_sub (&xh, rh, th))
    xh += m;

  తిరిగి xh;
}

/* Modular two-word multiplication, r = a * b mod m, with mi = m^(-1) mod B.
   Both a and b must be in redc form, the result will be in redc form too.
   For performance reasons, the most significant bit of m must be clear.  */
స్థిర mp_limb_t
mulredc2 (mp_limb_t *r1p,
          mp_limb_t a1, mp_limb_t a0, mp_limb_t b1, mp_limb_t b0,
          mp_limb_t m1, mp_limb_t m0, mp_limb_t mi)
{
  mp_limb_t r1, r0, q, p1, t1, t0, s1, s0;
  MAYBE_వాడని mp_limb_t p0;
  mi = -mi;
  affirm ((m1 >> (W_TYPE_కొలత - 1)) == 0);

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
  r0 += (t0 != 0); /*వాహకం*/
  కూడు_ssaaaa (r1, r0, r1, r0, 0, p1);
  కూడు_ssaaaa (r1, r0, r1, r0, 0, t1);
  కూడు_ssaaaa (r1, r0, r1, r0, s1, s0);

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
  కూడు_ssaaaa (t1, t0, t1, t0, 0, r0);
  q = mi * t0;
  కూడు_ssaaaa (r1, r0, s1, s0, 0, r1);
  umul_ppmm (p1, p0, q, m0);
  umul_ppmm (s1, s0, q, m1);
  r0 += (t0 != 0); /*వాహకం*/
  కూడు_ssaaaa (r1, r0, r1, r0, 0, p1);
  కూడు_ssaaaa (r1, r0, r1, r0, 0, t1);
  కూడు_ssaaaa (r1, r0, r1, r0, s1, s0);

  ఒకవేళ (ge2 (r1, r0, m1, m0))
    sub_ddmmss (r1, r0, r1, r0, m1, m0);

  *r1p = r1;
  తిరిగి r0;
}

స్థిర bool mp_ప్రధానసంఖ్య_p (mpz_t);

/* Is N prime?  N cannot be even or be a composite number less than
   SQUARE_OF_FIRST_OMITTED_PRIME.  */
స్థిర bool
ప్రధానసంఖ్య_p (mp_limb_t n)
{
  ఒకవేళ (n <= 1)
    తిరిగి false;

  /* We have already cast out small primes.  */
  ఒకవేళ (n < వర్గం_యొక్క_మొదటి_OMITTED_ప్రధానసంఖ్య)
    తిరిగి true;

  mpz_t mn = MPZ_ROINIT_N (&n, 1);
  తిరిగి mp_ప్రధానసంఖ్య_p (mn);
}

/* Is (n1,n0) prime?  (n1,n0) cannot be even or be a composite number
   less than SQUARE_OF_FIRST_OMITTED_PRIME.  */
స్థిర bool
prime2_p (mp_limb_t n1, mp_limb_t n0)
{
  ఒకవేళ (n1 == 0)
    తిరిగి ప్రధానసంఖ్య_p (n0);

  uuint n = make_uuint (n1, n0);
  mpz_t mn = MPZ_ROINIT_N (n.uu, 2);
  తిరిగి mp_ప్రధానసంఖ్య_p (mn);
}

/* Is N prime?  N cannot be even or be a composite number less than
   SQUARE_OF_FIRST_OMITTED_PRIME.  */
స్థిర bool
mp_ప్రధానసంఖ్య_p (mpz_t n)
{
  ఒకవేళ (mpz_cmp_ui (n, 1) <= 0)
    తిరిగి false;

  /* We have already cast out small primes.  */
  ఒకవేళ (mpz_cmp_ui (n, వర్గం_యొక్క_మొదటి_OMITTED_ప్రధానసంఖ్య) < 0)
    తిరిగి true;

  తిరిగి !!mpz_probab_ప్రధానసంఖ్య_p (n, MR_REPS);
}

/* Insert into FACTORS the result of factoring N,
   using Pollard-rho with starting value A.  N must be odd.  */
స్థిర శూన్య
గుణకం_using_pollard_rho (నిర్మాణం factors *factors, mp_limb_t n, mp_limb_t a)
{
  mp_limb_t x, z, y, P, t, ni, g;

  int_fast64_t k = 1;
  int_fast64_t l = 1;

  redcify (P, 1, n);
  addmod (x, P, P, n);          /* i.e., redcify(2) */
  y = z = x;

  అయితే (n != 1)
    {
      affirm (a < n);

      ni = binv_limb (n);	/* FIXME: when could we use old 'ni' value?  */

      కొరకు (;;)
        {
          చేయి
            {
              x = mulredc (x, x, n, ni);
              addmod (x, x, a, n);

              submod (t, z, x, n);
              P = mulredc (P, t, n, ni);

              ఒకవేళ ((k & 31) == 1)
                {
                  ఒకవేళ (గరిష్ఠ_సామాన్య_భాజకం_odd (P, n) != 1)
                    వెళ్ళు గుణకం_దొరికింది;
                  y = x;
                }
            }
          అయితే (--k != 0);

          z = x;
          k = l;
          l = 2 * l;
          కొరకు (int_fast64_t i = 0; i < k; i++)
            {
              x = mulredc (x, x, n, ni);
              addmod (x, x, a, n);
            }
          y = x;
        }

    గుణకం_దొరికింది:
      చేయి
        {
          y = mulredc (y, y, n, ni);
          addmod (y, y, a, n);

          submod (t, z, y, n);
          g = గరిష్ఠ_సామాన్య_భాజకం_odd (t, n);
        }
      అయితే (g == 1);

      ఒకవేళ (n == g)
        {
          /* Found n itself as factor.  Restart with different params.  */
          గుణకం_using_pollard_rho (factors, n, a + 1);
          తిరిగి;
        }

      n = n / g;

      ఒకవేళ (!ప్రధానసంఖ్య_p (g))
        గుణకం_using_pollard_rho (factors, g, a + 1);
      లేకపోతే
        గుణకం_చొప్పించు (factors, g);

      ఒకవేళ (ప్రధానసంఖ్య_p (n))
        {
          గుణకం_చొప్పించు (factors, n);
          ఆపు;
        }

      x = x % n;
      z = z % n;
      y = y % n;
    }
}

స్థిర శూన్య
గుణకం_using_pollard_rho2 (నిర్మాణం factors *factors,
                           mp_limb_t n1, mp_limb_t n0, mp_limb_t a)
{
  mp_limb_t x1, x0, z1, z0, y1, y0, P1, P0, t1, t0, g1, g0, r1m;

  int_fast64_t k = 1;
  int_fast64_t l = 1;

  redcify2 (P1, P0, 1, n1, n0);
  addmod2 (x1, x0, P1, P0, P1, P0, n1, n0); /* i.e., redcify(2) */
  y1 = z1 = x1;
  y0 = z0 = x0;

  అయితే (n1 != 0 || n0 != 1)
    {
      mp_limb_t ni = binv_limb (n0);

      కొరకు (;;)
        {
          చేయి
            {
              x0 = mulredc2 (&r1m, x1, x0, x1, x0, n1, n0, ni);
              x1 = r1m;
              addmod2 (x1, x0, x1, x0, 0, a, n1, n0);

              submod2 (t1, t0, z1, z0, x1, x0, n1, n0);
              P0 = mulredc2 (&r1m, P1, P0, t1, t0, n1, n0, ni);
              P1 = r1m;

              ఒకవేళ ((k & 31) == 1)
                {
                  uuset (&g1, &g0, gcd2_odd (P1, P0, n1, n0));
                  ఒకవేళ (g1 != 0 || g0 != 1)
                    వెళ్ళు గుణకం_దొరికింది;
                  y1 = x1; y0 = x0;
                }
            }
          అయితే (--k != 0);

          z1 = x1; z0 = x0;
          k = l;
          l = 2 * l;
          కొరకు (int_fast64_t i = 0; i < k; i++)
            {
              x0 = mulredc2 (&r1m, x1, x0, x1, x0, n1, n0, ni);
              x1 = r1m;
              addmod2 (x1, x0, x1, x0, 0, a, n1, n0);
            }
          y1 = x1; y0 = x0;
        }

    గుణకం_దొరికింది:
      చేయి
        {
          y0 = mulredc2 (&r1m, y1, y0, y1, y0, n1, n0, ni);
          y1 = r1m;
          addmod2 (y1, y0, y1, y0, 0, a, n1, n0);

          submod2 (t1, t0, z1, z0, y1, y0, n1, n0);
          uuset (&g1, &g0, gcd2_odd (t1, t0, n1, n0));
        }
      అయితే (g1 == 0 && g0 == 1);

      ఒకవేళ (g1 == 0)
        {
          /* The found factor is one word, and > 1.  */
          divexact_21 (n1, n0, n1, n0, g0);     /* n = n / g */

          ఒకవేళ (!ప్రధానసంఖ్య_p (g0))
            గుణకం_using_pollard_rho (factors, g0, a + 1);
          లేకపోతే
            గుణకం_చొప్పించు (factors, g0);
        }
      లేకపోతే
        {
          /* The found factor is two words.  This is highly unlikely, thus hard
             to trigger.  Please be careful before you change this code!  */

          ఒకవేళ (n1 == g1 && n0 == g0)
            {
              /* Found n itself as factor.  Restart with different params.  */
              గుణకం_using_pollard_rho2 (factors, n1, n0, a + 1);
              తిరిగి;
            }

          /* Compute n = n / g.  Since the result will fit one word,
             we can compute the quotient modulo B, ignoring the high
             divisor word.  */
          n0 = binv_limb (g0) * n0;
          n1 = 0;

          ఒకవేళ (!prime2_p (g1, g0))
            గుణకం_using_pollard_rho2 (factors, g1, g0, a + 1);
          లేకపోతే
            గుణకం_చొప్పించు_large (factors, g1, g0);
        }

      ఒకవేళ (n1 == 0)
        {
          ఒకవేళ (ప్రధానసంఖ్య_p (n0))
            {
              గుణకం_చొప్పించు (factors, n0);
              ఆపు;
            }

          గుణకం_using_pollard_rho (factors, n0, a);
          తిరిగి;
        }

      ఒకవేళ (prime2_p (n1, n0))
        {
          గుణకం_చొప్పించు_large (factors, n1, n0);
          ఆపు;
        }

      uuset (&x1, &x0, mod2 (x1, x0, n1, n0));
      uuset (&z1, &z0, mod2 (z1, z0, n1, n0));
      uuset (&y1, &y0, mod2 (y1, y0, n1, n0));
    }
}

/* Set RP = (AP + BP) mod MP.  All values are nonnegative and take up
   N>0 words, and AP and BP are both less than MP.  */
స్థిర శూన్య
mp_modadd (mp_limb_t *rp, mp_limb_t స్థిరరాశి *ap, mp_limb_t స్థిరరాశి *bp,
           mp_limb_t స్థిరరాశి *mp, mp_కొలత_t n)
{
  mp_limb_t cy = mpn_కూడు_n (rp, ap, bp, n);
  ఒకవేళ (cy || mpn_cmp (rp, mp, n) >= 0)
    mpn_sub_n (rp, rp, mp, n);
}

/* Set RP = (AP - BP) mod MP.  All values are nonnegative and take up
   N>0 words, and AP and BP are both less than MP.  */
స్థిర శూన్య
mp_modsub (mp_limb_t *rp, mp_limb_t స్థిరరాశి *ap, mp_limb_t స్థిరరాశి *bp,
           mp_limb_t స్థిరరాశి *mp, mp_కొలత_t n)
{
  mp_limb_t cy = mpn_sub_n (rp, ap, bp, n);
  ఒకవేళ (cy)
    mpn_కూడు_n (rp, rp, mp, n);
}

/* Set RP = (AP - B0) mod MP.  All values are nonnegative, AP and MP
   both take up N>0 words, and AP < MP.  */
స్థిర శూన్య
mp_modadd_1 (mp_limb_t *rp, mp_limb_t స్థిరరాశి *ap, mp_limb_t b0,
             mp_limb_t స్థిరరాశి *mp, mp_కొలత_t n)
{
  mp_limb_t cy = mpn_కూడు_1 (rp, ap, n, b0);
  ఒకవేళ (cy || mpn_cmp (rp, mp, n) >= 0)
    mpn_sub_n (rp, rp, mp, n);
}

స్థిర శూన్య
mp_mulredc (mp_limb_t *rp, mp_limb_t స్థిరరాశి *ap, mp_limb_t స్థిరరాశి *bp,
            mp_limb_t స్థిరరాశి *mp, mp_కొలత_t n, mp_limb_t m0inv, mp_limb_t *tp)
{
  tp[n] = mpn_mul_1 (tp, ap, n, bp[0]);
  tp[0] = mpn_addmul_1 (tp, mp, n, tp[0] * m0inv);

  కొరకు (mp_కొలత_t i = 1; i < n; i++)
    {
      tp[n + i] = mpn_addmul_1 (tp + i, ap, n, bp[i]);
      tp[i] = mpn_addmul_1 (tp + i, mp, n, tp[i] * m0inv);
    }
  mp_modadd (rp, tp, tp + n, mp, n);
}

/* Maximum value for N in mp_factor_using_pollard_rho,
   to avoid integer overflow when it calls xinmalloc.  */
#నిర్వచించు MP_గుణకం_USING_POLLARD_RHO_N_MAX \
  ((MIN (IDX_MAX, TYPE_గరిష్ఠ (mp_కొలత_t)) - 3) / 10)

/* Insert into FACTORS the result of factoring MP, of size N,
   using Pollard-rho with starting value A.  MP must be odd. */
స్థిర శూన్య
mp_గుణకం_using_pollard_rho (నిర్మాణం mp_factors *factors,
                             mp_limb_t స్థిరరాశి *mp, mp_కొలత_t n,
                             mp_limb_t a)
{
  devmsg ("[pollard-rho (%lu)] ", (గుర్తులేని దీర్ఘ పూర్ణాంకం) a);

  static_assert (10 * MP_గుణకం_USING_POLLARD_RHO_N_MAX + 3
                 <= MIN (IDX_MAX, TYPE_గరిష్ఠ (mp_కొలత_t)));
  mp_limb_t *scratch = xinmalloc (10 * n + 3, పరిమాణం *scratch);
  mp_limb_t *qp = scratch + 2 * n + 1, *pp = qp + n + 2,
    *xp = pp + n, *yp = xp + n, *zp = yp + n,
    *tp = zp + n, *sp = tp + n, *gp = sp + n;
  mp_కొలత_t gn;

  mpn_zero (scratch, n);
  scratch[n] = 1;
  mpn_tdiv_qr (qp, pp, 0, scratch, n + 1, mp, n);

  mp_modadd (xp, pp, pp, mp, n);
  mpn_copyi (yp, xp, n);
  mpn_copyi (zp, xp, n);

  mp_limb_t m0inv = binv_limb (-mp[0]);

  కొరకు (int_fast64_t k = 1; ; k *= 2)
    {
      కొరకు (int_fast64_t i = k; 0 < i; i--)
        {
          mp_mulredc (tp, xp, xp, mp, n, m0inv, scratch);
          mp_modadd_1 (xp, tp, a, mp, n);

          mp_modsub (tp, zp, xp, mp, n);
          mp_mulredc (pp, pp, tp, mp, n, m0inv, scratch);

          ఒకవేళ (i % 128 == 1)
            {
              ఒకవేళ (mpn_zero_p (pp, n))
                {
                  mp_గుణకం_using_pollard_rho (factors, mp, n, a + 1);
                  వెళ్ళు ముగింపు;
                }
              mpn_copyi (tp, pp, n);
              mpn_copyi (sp, mp, n);
              gn = mpn_గరిష్ఠ_సామాన్య_భాజకం (gp, tp, n, sp, n);
              ఒకవేళ (gn != 1 || gp[0] != 1)
                వెళ్ళు గుణకం_దొరికింది;
              mpn_copyi (yp, xp, n);
            }
        }

      mpn_copyi (zp, xp, n);
      కొరకు (int_fast64_t i = 2 * k; 0 < i; i--)
        {
          mp_mulredc (tp, xp, xp, mp, n, m0inv, scratch);
          mp_modadd_1 (xp, tp, a, mp, n);
        }
      mpn_copyi (yp, xp, n);
    }

 గుణకం_దొరికింది:
  చేయి
    {
      mp_mulredc (tp, yp, yp, mp, n, m0inv, scratch);
      mp_modadd_1 (yp, tp, a, mp, n);
      mp_modsub (tp, zp, yp, mp, n);
      mpn_copyi (sp, mp, n);
      gn = mpn_గరిష్ఠ_సామాన్య_భాజకం (gp, tp, n, sp, n);
    }
  అయితే (gn == 1 && gp[0] == 1);

  mpz_t g = MPZ_ROINIT_N (gp, gn);
  mpz_t m = MPZ_ROINIT_N ((mp_limb_t *) mp, n), q;
  mpz_init (q);
  mpz_divexact (q, m, g);

  ఒకవేళ (!mp_ముగింపు_లోపల_single (factors, g))
    {
      ఒకవేళ (mp_ప్రధానసంఖ్య_p (g))
        mp_గుణకం_చొప్పించు (factors, g, 1);
      లేకపోతే
        mp_గుణకం_using_pollard_rho (factors, gp, gn, a + 1);
    }

  ఒకవేళ (!mp_ముగింపు_లోపల_single (factors, q))
    {
      ఒకవేళ (mp_ప్రధానసంఖ్య_p (q))
        mp_గుణకం_చొప్పించు (factors, q, 1);
      లేకపోతే
        {
          devmsg ("[composite factor--restarting pollard-rho] ");
          mp_గుణకం_using_pollard_rho (factors, mpz_limbs_read (q),
                                       mp_కొలత (q), a + 1);
        }
    }

  mpz_clear (q);

 ముగింపు:
  విడుదల (scratch);
}

/* Insert into FACTORS the prime factors of the two-word number (T1,T0).
   Primes less than the prime with index PRIME_IDX
   have already been considered, and need not be looked at again.  */
స్థిర శూన్య
గుణకం_పైకి (నిర్మాణం factors *factors, mp_limb_t t1, mp_limb_t t0,
           idx_t ప్రధానసంఖ్య_idx)
{
  factors->nfactors = 0;
  hiset (&factors->plarge, 0);

  ఒకవేళ (t1 == 0 && t0 < 2)
    తిరిగి;

  uuset (&t1, &t0, గుణకం_using_division (factors, t1, t0, ప్రధానసంఖ్య_idx));

  ఒకవేళ (t1 == 0 && t0 < 2)
    తిరిగి;

  ఒకవేళ (prime2_p (t1, t0))
    గుణకం_చొప్పించు_large (factors, t1, t0);
  లేకపోతే
    {
      ఒకవేళ (t1 == 0)
        గుణకం_using_pollard_rho (factors, t0, 1);
      లేకపోతే
        గుణకం_using_pollard_rho2 (factors, t1, t0, 1);
    }
}

/* Compute the prime factors of the two-word number (T1,T0),
   and put the results in FACTORS.  */
స్థిర శూన్య
గుణకం (నిర్మాణం factors *factors, mp_limb_t t1, mp_limb_t t0)
{
  గుణకం_పైకి (factors, t1, t0, 0);
}

/* Return the prime factors of T.  */
స్థిర నిర్మాణం mp_factors
mp_గుణకం (mpz_t t)
{
  నిర్మాణం mp_factors factors = mp_no_factors ();

  ఒకవేళ (mpz_sgn (t) != 0)
    {
      factors = mp_గుణకం_using_division (t);

      ఒకవేళ (mpz_cmp_ui (t, 1) != 0)
        {
          devmsg ("[is number prime?] ");
          ఒకవేళ (mp_ప్రధానసంఖ్య_p (t))
            mp_గుణకం_చొప్పించు (&factors, t, 1);
          లేకపోతే
            mp_గుణకం_using_pollard_rho (&factors, mpz_limbs_read (t),
                                         mp_కొలత (t), 1);
        }
    }

  తిరిగి factors;
}

/* Convert to *U the value represented by S, and return LONGINT_OK.
   Return LONGINT_OVERFLOW if S is composed of all digits but is too
   large to fit in *U, otherwise LONGINT_INVALID if S is not a valid
   positive integer.  */
స్థిర strtol_error
strtouuint (uuint *u, అక్షరం స్థిరరాశి *s)
{
  mp_limb_t hi = 0, lo = *s++ - '0';

  ఒకవేళ (UNLIKELY (9 < lo))
    తిరిగి LONGINT_INVALID;

  కొరకు (; LIKELY (0 <= *s - '0' && *s - '0' <= 9); s++)
    {
      ఒకవేళ (UNLIKELY (ckd_mul (&hi, hi, 10)))
        వెళ్ళు అధికప్రవాహం;

      పూర్ణాంకం lo_వాహకం = (lo >> (W_TYPE_కొలత - 3)) + (lo >> (W_TYPE_కొలత - 1));
      lo_వాహకం += 10 * lo < 2 * lo;

      lo = 10 * lo;
      lo_వాహకం += ckd_కూడు (&lo, lo, *s - '0');
      ఒకవేళ (UNLIKELY (ckd_కూడు (&hi, hi, lo_వాహకం)))
        వెళ్ళు అధికప్రవాహం;
    }

  ఒకవేళ (UNLIKELY (*s))
    తిరిగి LONGINT_INVALID;

  *u = make_uuint (hi, lo);
  తిరిగి LONGINT_OK;

 అధికప్రవాహం:
  /* Since s[0] caused the overflow, we know it is a valid integer.  */
  s++;
  తిరిగి s[strspn (s, "0123456789")] ? LONGINT_INVALID : LONGINT_అధికప్రవాహం;
}

/* FACTOR_PIPE_BUF is chosen to give good performance,
   and also is the max guaranteed size that
   consumers can read atomically through pipes.
   Also it's big enough to cater for max line length
   even with 128 bit word.  */
#నిర్వచితం_కాకపోతే _POSIX_PIPE_BUF
# define _POSIX_PIPE_BUF 512
#అయితే_ముగింపు
#నిర్వచితమైతే PIPE_BUF
గణన { గుణకం_PIPE_BUF = PIPE_BUF };
#లేకపోతే
గణన { గుణకం_PIPE_BUF = _POSIX_PIPE_BUF };
#అయితే_ముగింపు

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
స్థిర అక్షరం lbuf_buf[2 * గుణకం_PIPE_BUF];
స్థిర idx_t lbuffered;

/* Write complete LBUF to standard output.  */
స్థిర శూన్య
lbuf_flush (శూన్య)
{
  idx_t కొలత = lbuffered;

  /* Update lbuffered now, to avoid infinite recursion on write error.  */
  lbuffered = 0;

  ఒకవేళ (నిండిన_write (STDOUT_FILENO, lbuf_buf, కొలత) != కొలత)
    write_error ();
}

/* Write LBUF to standard output.
   LBUF should contain at least FACTOR_PIPE_BUF bytes.
   If possible, write a prefix of LBUF that is newline terminated
   and contains <= FACTOR_PIPE_BUF bytes, so consumers can read atomically.
   But if the first FACTOR_PIPE_BUF bytes contain no newlines,
   give up on atomicity and just write the first FACTOR_PIPE_BUF bytes.  */
స్థిర శూన్య
lbuf_half_flush (శూన్య)
{
  అక్షరం *nl = memrchr (lbuf_buf, '\n', గుణకం_PIPE_BUF);
  అక్షరం *suffix = nl ? nl + 1 : lbuf_buf + గుణకం_PIPE_BUF;
  idx_t ఉపసర్గ_కొలత = suffix - lbuf_buf;
  idx_t suffix_కొలత = lbuffered - ఉపసర్గ_కొలత;
  lbuffered = ఉపసర్గ_కొలత;
  lbuf_flush ();
  lbuffered = suffix_కొలత;
  memmove (lbuf_buf, suffix, suffix_కొలత);
}

/* Add a character C to lbuf_buf.  */
స్థిర శూన్య
lbuf_putc (అక్షరం c)
{
  lbuf_buf[lbuffered++] = c;
}

/* Add a newline to lbuf_buf.  Then, if enough bytes are already
   buffered, write the buffer atomically to standard output.  */
స్థిర శూన్య
lbuf_putnl (శూన్య)
{
  lbuf_putc ('\n');

  /* Provide immediate output for interactive use.  */
  స్థిర పూర్ణాంకం పంక్తి_buffered = -1;
  ఒకవేళ (పంక్తి_buffered < 0)
    పంక్తి_buffered = isatty (STDOUT_FILENO);

  ఒకవేళ (పంక్తి_buffered)
    lbuf_flush ();
  లేకపోతే ఒకవేళ (గుణకం_PIPE_BUF <= lbuffered)
    lbuf_half_flush ();
}

/* Append the string representation of I to lbuf_buf, followed by
   everything from BUFEND to lbuf_buf's end.  Use the area just before
   BUFEND temporarily.  */
స్థిర శూన్య
lbuf_putint_append (mp_limb_t i, అక్షరం *bufend)
{
  అక్షరం *istr = bufend;
  చేయి
    {
      *--istr = '0' + i % 10;
      i /= 10;
    }
  అయితే (i);

  అక్షరం *p = lbuf_buf + lbuffered;
  చేయి
    *p++ = *istr++;
  అయితే (istr < lbuf_buf + పరిమాణం lbuf_buf);

  lbuffered = p - lbuf_buf;
}

/* Append the string representation of I to lbuf_buf.  */
స్థిర శూన్య
lbuf_putint (mp_limb_t i)
{
  lbuf_putint_append (i, lbuf_buf + పరిమాణం lbuf_buf);
}
స్థిర శూన్య
lbuf_putbitcnt (mp_bitcnt_t i)
{
  అక్షరం *bufend = lbuf_buf + పరిమాణం lbuf_buf;
  కొరకు (; MP_LIMB_MAX < i; i /= 10)
    *--bufend = '0' + i % 10;
  lbuf_putint_append (i, bufend);
}

/* Append the string representation of T to lbuf_buf.  */
స్థిర శూన్య
print_uuint (uuint t)
{
  mp_limb_t t1 = hi (t), t0 = lo (t);
  అక్షరం *bufend = lbuf_buf + పరిమాణం lbuf_buf;

  అయితే (t1)
    {
      mp_limb_t r = t1 % BIG_POWER_యొక్క_10;
      t1 /= BIG_POWER_యొక్క_10;
      udiv_qrnnd (t0, r, r, t0, BIG_POWER_యొక్క_10);
      కొరకు (పూర్ణాంకం i = 0; i < LOG_BIG_POWER_యొక్క_10; i++)
        {
          *--bufend = '0' + r % 10;
          r /= 10;
        }
    }

  lbuf_putint_append (t0, bufend);
}

/* Buffer the mpz I to lbuf_buf, possibly writing if it is long.  */
స్థిర శూన్య
lbuf_putmpz (mpz_t స్థిరరాశి i)
{
  idx_t sizeinbase = mpz_sizeinbase (i, 10);
  అక్షరం *lbuf_bufend = lbuf_buf + పరిమాణం lbuf_buf;
  అక్షరం *p = lbuf_buf + lbuffered;
  ఒకవేళ (sizeinbase < lbuf_bufend - p)
    {
      mpz_get_str (p, 10, i);
      p += sizeinbase;
      lbuffered = p - !p[-1] - lbuf_buf;
      అయితే (గుణకం_PIPE_BUF <= lbuffered)
        lbuf_half_flush ();
    }
  లేకపోతే
    {
      lbuf_flush ();
      అక్షరం *istr = ximalloc (sizeinbase + 1);
      mpz_get_str (istr, 10, i);
      idx_t istrlen = sizeinbase - !istr[sizeinbase - 1];
      ఒకవేళ (నిండిన_write (STDOUT_FILENO, istr, istrlen) != istrlen)
        write_error ();
      విడుదల (istr);
    }
}

/* Emit the factors of T, which is less than B^2 / 2.  */
స్థిర శూన్య
print_factors_single (uuint t)
{
  నిర్మాణం factors factors;

  print_uuint (t);
  lbuf_putc (':');

  గుణకం (&factors, hi (t), lo (t));

  కొరకు (పూర్ణాంకం j = 0; j < factors.nfactors; j++)
    కొరకు (పూర్ణాంకం k = 0; k < factors.e[j]; k++)
      {
        lbuf_putc (' ');
        print_uuint (make_uuint (0, factors.p[j]));
        ఒకవేళ (print_exponents && factors.e[j] > 1)
          {
            lbuf_putc ('^');
            lbuf_putint (factors.e[j]);
            ఆపు;
          }
      }

  ఒకవేళ (hi_is_సమితి (&factors.plarge))
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
స్థిర bool
print_factors (అక్షరం స్థిరరాశి *ఆదానం)
{
  /* Skip initial spaces and '+'.  */
  అక్షరం స్థిరరాశి *str = ఆదానం;
  అయితే (*str == ' ')
    str++;
  str += *str == '+';

  /* Try converting the number to one or two words.  If it fails, use GMP or
     print an error message.  The 2nd condition checks that the most
     significant bit of the two-word number is clear, in a typesize neutral
     way.  */
  uuint u;
  strtol_error err = strtouuint (&u, str);
  mpz_t t;

  ఎంపిక (err)
    {
    స్థితి LONGINT_OK:
      ఒకవేళ (hi (u) >> (W_TYPE_కొలత - 1) == 0)
        {
          devmsg ("[using single-precision arithmetic] ");
          print_factors_single (u);
          తిరిగి true;
        }
      FALLTHROUGH;
    స్థితి LONGINT_అధికప్రవాహం:
      /* Try GMP.  */
      ఒకవేళ (0 <= mpz_init_సమితి_str (t, str, 10))
        ఆపు;
      FALLTHROUGH;

    స్థితి LONGINT_INVALID:
    స్థితి LONGINT_INVALID_SUFFIX_CHAR:
    స్థితి LONGINT_INVALID_SUFFIX_CHAR_WITH_అధికప్రవాహం:
    అప్రమేయ:
      error (0, 0, _("%s is not a valid positive integer"), quote (ఆదానం));
      తిరిగి false;
    }

  devmsg ("[using arbitrary-precision arithmetic] ");
  ఒకవేళ (MP_గుణకం_USING_POLLARD_RHO_N_MAX < mp_కొలత (t))
    xalloc_die ();

  lbuf_putmpz (t);
  lbuf_putc (':');
  నిర్మాణం mp_factors factors = mp_గుణకం (t);

  కొరకు (idx_t j = 0; j < factors.nfactors; j++)
    కొరకు (mp_bitcnt_t k = 0; k < factors.f[j].e; k++)
      {
        lbuf_putc (' ');
        lbuf_putmpz (factors.f[j].p);
        ఒకవేళ (print_exponents && factors.f[j].e > 1)
          {
            lbuf_putc ('^');
            lbuf_putbitcnt (factors.f[j].e);
            ఆపు;
          }
      }

  mp_గుణకం_clear (&factors);
  mpz_clear (t);
  lbuf_putnl ();
  తిరిగి true;
}

/* Output a usage diagnostic and exit with STATUS.  */
శూన్య
usage (పూర్ణాంకం స్థితి_2)
{
  ఒకవేళ (స్థితి_2 != EXIT_SUCCESS)
    emit_try_help ();
  లేకపోతే
    {
      రాయి (_("\
Usage: %s [OPTION] [సంఖ్య_2]...\n\
"),
              కార్యక్రమం_name);
      fputs (_("\
Print the ప్రధానసంఖ్య factors యొక్క each specified integer సంఖ్య_2.  If none\n\
are specified on the command పంక్తి, పఠ them from standard ఆదానం.\n\
\n\
"), stdout);
      oputs (_("\
  -h, --exponents\n\
         print repeated factors లోపల form p^e unless e is 1\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (సంస్కరణ_OPTION_DESCRIPTION);
      emit_ancillary_info (కార్యక్రమం_NAME);
    }
  నిష్క్రమించు (స్థితి_2);
}

/* Read numbers from stdin, one per line, and output their factors.  */
స్థిర bool
do_stdin (శూన్య)
{
  bool ok = true;
  token_బఫరు tokenbuffer;

  init_tokenbuffer (&tokenbuffer);

  అయితే (true)
    {
      size_t token_నిడివి = readtoken (stdin, DELIM, పరిమాణం (DELIM) - 1,
                                       &tokenbuffer);
      ఒకవేళ (token_నిడివి == (size_t) -1)
        {
          ఒకవేళ (ferror (stdin))
            error (EXIT_FAILURE, errno, _("error reading input"));
          ఆపు;
        }

      ok &= print_factors (tokenbuffer.బఫరు);
    }
  విడుదల (tokenbuffer.బఫరు);

  తిరిగి ok;
}

పూర్ణాంకం
ప్రధాన (పూర్ణాంకం argc, అక్షరం **argv)
{
  initialize_main (&argc, &argv);
  సమితి_కార్యక్రమం_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  పూర్ణాంకం c;
  అయితే ((c = getopt_long (argc, argv, "h", long_options, ఖాళీ)) != -1)
    {
      ఎంపిక (c)
        {
        స్థితి 'h':  /* NetBSD used -h for this functionality first.  */
          print_exponents = true;
          ఆపు;

        స్థితి DEV_DEBUG_OPTION:
          dev_debug = true;
          ఆపు;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_సంస్కరణ_CHAR (కార్యక్రమం_NAME, AUTHORS);

        అప్రమేయ:
          usage (EXIT_FAILURE);
        }
    }

  atexit (lbuf_flush);

  bool ok;
  ఒకవేళ (argc <= optind)
    ok = do_stdin ();
  లేకపోతే
    {
      ok = true;
      కొరకు (పూర్ణాంకం i = optind; i < argc; i++)
        ఒకవేళ (! print_factors (argv[i]))
          ok = false;
    }

  తిరిగి ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
