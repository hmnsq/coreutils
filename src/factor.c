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


#شامل <config.h>
#شامل <getopt.h>
#شامل <stdbit.h>
#شامل <stdio.h>
#شامل <gmp.h>

#شامل "system.h"
#شامل "assure.h"
#شامل "full-write.h"
#شامل "quote.h"
#شامل "readtokens.h"
#شامل "xstrtol.h"

/* The official name of this program (e.g., no 'g' prefix).  */
#تعریف_کرو پروگرام_NAME "factor"

#تعریف_کرو AUTHORS \
  proper_name ("Paul Rubin"),                                           \
  proper_name_lite ("Torbjorn Granlund", "Torbj\303\266rn Granlund"),   \
  proper_name_lite ("Niels Moller", "Niels M\303\266ller")

/* Token delimiters when reading from a file.  */
ساکن حرف ثابت DELIM[] = "\n\t ";

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
#اگر_تعریف_شدہ GMP_LIMB_بٹ
# define W_TYPE_سائز GMP_LIMB_بٹ
#ورنہ
/* An older GMP, or mini-gmp; guess the usual value.  */
# define W_TYPE_سائز ULONG_چوڑائی
#اگر_ختم

/* The maximum value of a word.  */
#تعریف_کرو MP_LIMB_MAX ((mp_limb_t) -1)

/* Check W_TYPE_SIZE's value, as it might be a guess.  */
static_assert (MP_LIMB_MAX >> (W_TYPE_سائز - 1) == 1);

/* Check that the builder didn't specify something perverse like
   "-DMINI_GMP_LIMB_TYPE=short -DW_TYPE_SIZE=USHRT_WIDTH".
   This could result in undefined behavior due to signed integer
   overflow if a word promotes to int.  */
static_assert (زیادہ_سے_زیادہ_عدد < MP_LIMB_MAX);

#اگر_غیر_تعریف_شدہ USE_LONGLONG_H
# اگر (defined INT32_MAX && defined UINT32_MAX \
      && defined INT64_MAX && defined UINT64_MAX)
#  define USE_LONGLONG_H true
# ورنہ
#  define USE_LONGLONG_H false
# endif
#اگر_ختم

#اگر USE_LONGLONG_H

/* Make definitions for longlong.h to make it do what it can do for us */

قسمتعریف mp_limb_t UWtype;
قسمتعریف غیرعلامتی صحیح UHWtype;
# undef UDWtype
قسمتعریف int32_t SItype;
قسمتعریف uint32_t USItype;
قسمتعریف int64_t DItype;
قسمتعریف uint64_t UDItype;
# define LONGLONG_STANDALONE     /* Don't require GMP's longlong.h mdep files */

/* FIXME make longlong.h really standalone, so that ASSERT, __GMP_DECLSPEC
   and __GMP_GNUC_PREREQ need not be defined here.  */
# define ASSERT(x)
# define __GMP_DECLSPEC
# ifndef __GMP_GNUC_PREREQ
#  اگر defined __GNUC__ && defined __GNUC_MINOR__
#   define __GMP_GNUC_PREREQ(a, b) ((a) < __GNUC__ + ((b) <= __GNUC_MINOR__))
#  ورنہ
#   define __GMP_GNUC_PREREQ(a, b) false
#  endif
# endif

/* longlong.h uses these macros only in certain system compiler combinations.
   Ensure usage to pacify -Wunused-macros.  */
# اگر defined ASSERT || defined __GMP_DECLSPEC || defined __GMP_GNUC_PREREQ
# endif

/* longlong.h uses casts even when useless.  */
# اگر 14 <= __GNUC__
#  pragma GCC diagnostic ignored "-Wuseless-cast"
# endif

# اگر _ARCH_PPC
#  define HAVE_HOST_CPU_FAMILY_powerpc 1
# endif
# include "longlong.h"
#اگر_ختم

/* 2*3*5*7*11...*101 fits in 128 bits, and has 26 prime factors.
   This code can be extended in the future as needed; show as an example
   2*3*5*7*11...*193 which fits in 257 bits, and has 44 prime factors.  */
#اگر 2 * W_TYPE_سائز <= 128
شمار { MAX_NFACTS = 26 };
#وگرنہ_اگر 2 * W_TYPE_سائز <= 257
شمار { MAX_NFACTS = 44 };
#ورنہ
# error "configuration has a wide word; please update MAX_NFACTS definition"
#اگر_ختم

شمار
{
  DEV_DEBUG_OPTION = CHAR_MAX + 1
};

ساکن ساخت option ثابت long_options[] =
{
  {"exponents", no_دلیل, تہی, 'h'},
  {"-debug", no_دلیل, تہی, DEV_DEBUG_OPTION},
  {GETOPT_HELP_OPTION_DECL},
  {GETOPT_نسخہ_OPTION_DECL},
  {تہی, 0, تہی, 0}
};

/* If true, use p^e output format.  */
ساکن bool print_exponents;

/* This represents an unsigned integer twice as wide as a word.  */
قسمتعریف ساخت { mp_limb_t uu[2]; } uuint;

/* Accessors and constructors for the type.  Programs should not
   access the type's internals directly, in case some future version
   replaces the type with unsigned __int256 or whatever.  */
ساکن mp_limb_t lo (uuint u) { واپس u.uu[0]; }
ساکن mp_limb_t hi (uuint u) { واپس u.uu[1]; }
ساکن خالی hiset (uuint *u, mp_limb_t hi) { u->uu[1] = hi; }
ساکن bool hi_is_سیٹ (uuint ثابت *pu) { واپس pu->uu[1] != 0; }
ساکن خالی
uuset (mp_limb_t *phi, mp_limb_t *plo, uuint uu)
{
  *phi = hi (uu);
  *plo = lo (uu);
}
ساکن uuint
make_uuint (mp_limb_t hi, mp_limb_t lo)
{
  واپس (uuint) {{lo, hi}};
}

/* BIG_POWER_OF_10 is a positive power of 10 that fits in a word.
   The larger it is, the more efficient the code will likely be.
   LOG_BIG_POWER_OF_10 = log (BIG_POWER_OF_10).  */
#اگر W_TYPE_سائز < 4
# error "Configuration error; platform word is impossibly narrow"
#وگرنہ_اگر W_TYPE_سائز < 30
/* An unusually narrow word.  */
ساکن mp_limb_t ثابت BIG_POWER_کا_10 = 10;
شمار { LOG_BIG_POWER_کا_10 = 1 };
#وگرنہ_اگر W_TYPE_سائز < 64
/* Almost surely a 32-bit word.  */
ساکن mp_limb_t ثابت BIG_POWER_کا_10 = 1000000000;
شمار { LOG_BIG_POWER_کا_10 = 9 };
#وگرنہ_اگر W_TYPE_سائز < 128
/* Almost surely a 64-bit word.  */
ساکن mp_limb_t ثابت BIG_POWER_کا_10 = 10000000000000000000llu;
شمار { LOG_BIG_POWER_کا_10 = 19 };
#ورنہ
/* An unusually wide word.  */
ساکن mp_limb_t ثابت BIG_POWER_کا_10 =
  (mp_limb_t) 10000000000000000000llu * 10000000000000000000llu;
شمار { LOG_BIG_POWER_کا_10 = 38 };
#اگر_ختم

/* Check that struct factors can use unsigned char to record a uuint's
   prime factor's multiplicity, which is at most 2 * W_TYPE_SIZE - 1.  */
static_assert (2 * W_TYPE_سائز - 1 <= UCHAR_MAX);

/* Likewise for recording the number of prime factors.  */
static_assert (MAX_NFACTS <= UCHAR_MAX);

/* Prime factors of a uuint.  At most one is a uuint.  */
ساخت factors
{
  /* If PLARGE.uu[1] is nonzero, PLARGE is a double-word prime factor
     with multiplicity 1; otherwise, PLARGE is not a factor and
     PLARGE.uu[0] might not be initialized.  */
  uuint plarge;

  /* Distinct single-word prime factors, their multiplicities,
     and their number.  */
  mp_limb_t     p[MAX_NFACTS];
  غیرعلامتی حرف e[MAX_NFACTS];
  غیرعلامتی حرف nfactors;
};

/* An mpz's prime factor and multiplicity.  */
ساخت mp_ضربی_عدد
{
  mpz_t p;
  mp_bitcnt_t e;
};

/* Prime factors of an mpz_t.  */
ساخت mp_factors
{
  idx_t nfactors;
  idx_t nalloc;

  /* A vector of distinct prime factors, a count of the factors,
     and the number of allocated slots in the vector.  */
  ساخت mp_ضربی_عدد *f COUNTED_BY (nfactors);
};

ساکن خالی ضربی_عدد (ساخت factors *, mp_limb_t, mp_limb_t);
ساکن خالی ضربی_عدد_اوپر (ساخت factors *, mp_limb_t, mp_limb_t, idx_t);

/* Set (w1,w0) = u * v.  */
#اگر_غیر_تعریف_شدہ umul_ppmm
/* Speed things up if there is an unsigned type uuroom_t that is wide
   enough to hold two words.  */
# اگر W_TYPE_سائز <= UINTMAX_چوڑائی / 2
#  define uuroom_t uintmax_t
# elif W_TYPE_سائز <= 64 && defined __SIZEOF_INT128__
#  define uuroom_t غیرعلامتی __int128
# endif
# ifdef uuroom_t
#  define umul_ppmm(w1, w0, u, v)		\
    کرو {					\
      uuroom_t _u = u, _w = _u * (v);		\
      (w1) = _w >> W_TYPE_سائز;			\
      (w0) = _w;				\
    } جب (false)
# endif
#اگر_ختم
#اگر_غیر_تعریف_شدہ umul_ppmm
ساکن mp_limb_t ثابت __ll_B = (mp_limb_t) 1 << (W_TYPE_سائز / 2);
ساکن mp_limb_t __ll_lowpart (mp_limb_t t) { واپس t & (__ll_B - 1); }
ساکن mp_limb_t __ll_highpart (mp_limb_t t) { واپس t >> (W_TYPE_سائز / 2); }
# define umul_ppmm(w1, w0, u, v)                                        \
  کرو {                                                                  \
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
    اگر (ckd_جوڑو (&__x1, __x1, __x2))	/* Did this give a carry?  */	\
      __x3 += __ll_B;		/* Yes, add it in the proper pos.  */	\
                                                                        \
    (w1) = __x3 + __ll_highpart (__x1);                                 \
    (w0) = (__x1 << W_TYPE_سائز / 2) + __ll_lowpart (__x0);             \
  } جب (false)
#اگر_ختم

/* Set (q,r) to the quotient and remainder of dividing (n1,n0) by d.  */
#اگر !defined udiv_qrnnd || defined UDIV_NEEDS_NORMALIZATION
/* Define our own, not needing normalization.  This function is
   currently not performance critical, so keep it simple.  Similar to
   the mod macro below.  */
# undef udiv_qrnnd
# define udiv_qrnnd(q, r, n1, n0, d)                                    \
  کرو {                                                                  \
    mp_limb_t __d1 = d, __d0 = 0, __q = 0, __r1 = n1, __r0 = n0;	\
                                                                        \
    affirm (__r1 < __d1);                                               \
    برائے (صحیح __i = W_TYPE_سائز; __i > 0; __i--)                         \
      {                                                                 \
        rsh2 (__d1, __d0, __d1, __d0, 1);                               \
        __q <<= 1;                                                      \
        اگر (ge2 (__r1, __r0, __d1, __d0))                               \
          {                                                             \
            __q++;                                                      \
            sub_ddmmss (__r1, __r0, __r1, __r0, __d1, __d0);            \
          }                                                             \
      }                                                                 \
    (r) = __r0;                                                         \
    (q) = __q;                                                          \
  } جب (false)
#اگر_ختم

/* Set (sh,sl) = (ah,al) + (bh,bl).  Overflow wraps around.  */
#اگر !defined جوڑو_ssaaaa
# define جوڑو_ssaaaa(sh, sl, ah, al, bh, bl)                             \
   ((sh) = (ah) + (bh) + ckd_جوڑو (&(sl), al, bl))
#اگر_ختم

/* Set (rh,rl) = (ah,al) >> cnt, where 0 < cnt < W_TYPE_SIZE.  */
#تعریف_کرو rsh2(rh, rl, ah, al, cnt)                                       \
  کرو {                                                                  \
    (rl) = ((ah) << (W_TYPE_سائز - (cnt))) | ((al) >> (cnt));           \
    (rh) = (ah) >> (cnt);                                               \
  } جب (false)

/* Set (rh,rl) = (ah,al) << cnt, where 0 < cnt < W_TYPE_SIZE.
   Overflow wraps around.  */
#تعریف_کرو lsh2(rh, rl, ah, al, cnt)                                       \
  کرو {                                                                  \
    (rh) = ((ah) << cnt) | ((al) >> (W_TYPE_سائز - (cnt)));             \
    (rl) = (al) << (cnt);                                               \
  } جب (false)

/* (ah,hl) < (bh,bl)?  */
ساکن bool
lt2 (mp_limb_t ah, mp_limb_t al, mp_limb_t bh, mp_limb_t bl)
{
  mp_limb_t dh, dl;
  bool vh = ckd_sub (&dh, ah, bh);
  mp_limb_t vl = ckd_sub (&dl, al, bl);
  واپس vh | ckd_sub (&dh, dh, vl);
}

/* (ah,hl) >= (bh,bl)?  */
ساکن bool
ge2 (mp_limb_t ah, mp_limb_t al, mp_limb_t bh, mp_limb_t bl)
{
  واپس !lt2 (ah, al, bh, bl);
}

/* Set (rh,rl) = (ah,al) - (bh,bl).  Overflow wraps around.  */
#اگر_غیر_تعریف_شدہ sub_ddmmss
# define sub_ddmmss(rh, rl, ah, al, bh, bl)                             \
   ((rh) = (ah) - (bh) - ckd_sub (&(rl), al, bl))
#اگر_ختم

/* Set r = (a - b) mod n, where a < n & b <= n.  */
#تعریف_کرو submod(r,a,b,n)                                                 \
  کرو {                                                                  \
    mp_limb_t _s, _t = -ckd_sub (&_s, a, b);				\
    (r) = ((n) & _t) + _s;						\
  } جب (false)

/* Set r = (a + b) mod n, where a < n & b <= n.  */
#تعریف_کرو addmod(r,a,b,n)                                                 \
  submod (r, a, (n) - (b), n)

/* Modular two-word addition and subtraction.  For performance reasons, the
   most significant bit of n1 must be clear.  The destination variables must be
   distinct from the mod operand.  */
#تعریف_کرو addmod2(r1, r0, a1, a0, b1, b0, n1, n0)                         \
  کرو {                                                                  \
    جوڑو_ssaaaa (r1, r0, a1, a0, b1, b0);				\
    اگر (ge2 (r1, r0, n1, n0))						\
      sub_ddmmss (r1, r0, r1, r0, n1, n0);				\
  } جب (false)
#تعریف_کرو submod2(r1, r0, a1, a0, b1, b0, n1, n0)                         \
  کرو {                                                                  \
    bool _v1 = ckd_sub (&(r1), a1, b1);					\
    mp_limb_t _v0 = ckd_sub (&(r0), a0, b0);				\
    اگر (_v1 | ckd_sub (&(r1), r1, _v0))					\
      جوڑو_ssaaaa (r1, r0, r1, r0, n1, n0);				\
  } جب (false)

/* Return 0 if x < B/2, MP_LIMB_MAX otherwise.  */
ساکن mp_limb_t
highbit_مقصد_نقاب (mp_limb_t x)
{
  واپس - (x >> (W_TYPE_سائز - 1));
}

/* Return (a1,a0) mod (d1,d0), where d1 != 0.  */
ATTRIBUTE_خالص ساکن uuint
mod2 (mp_limb_t a1, mp_limb_t a0, mp_limb_t d1, mp_limb_t d0)
{
  affirm (d1 != 0);

  اگر (a1)
    {
      صحیح cntd = stdc_leading_zeros (d1);
      صحیح cnta = stdc_leading_zeros (a1);
      صحیح cnt = cntd - cnta;
      اگر (0 < cnt)
        {
          lsh2 (d1, d0, d1, d0, cnt);
          برائے (صحیح i = 0; i < cnt; i++)
            {
              اگر (ge2 (a1, a0, d1, d0))
                sub_ddmmss (a1, a0, a1, a0, d1, d0);
              rsh2 (d1, d0, d1, d0, 1);
            }
        }
    }

  واپس make_uuint (a1, a0);
}

/* Return the greatest common divisor of a and b,
   where b is odd.  */
ATTRIBUTE_CONST
ساکن mp_limb_t
عاد_اعظم_odd (mp_limb_t a, mp_limb_t b)
{
  اگر (a == 0)
    واپس b;

  /* Take out least significant one bit, to make room for sign */
  b >>= 1;

  برائے (;;)
    {
      mp_limb_t t;
      mp_limb_t bgta;

      assume (a);
      mp_limb_t ao = a >> stdc_trailing_zeros (a);

      t = (ao >> 1) - b;
      اگر (t == 0)
        واپس ao;

      bgta = highbit_مقصد_نقاب (t);

      /* b <-- min (a, b) */
      b += (bgta & t);

      /* a <-- |a - b| */
      a = (t ^ bgta) - bgta;
    }
}

/* Return the greatest common divisor of (a1,a0) and (b1,b0),
   where (b1,b0) is odd.  */
ATTRIBUTE_خالص ساکن uuint
gcd2_odd (mp_limb_t a1, mp_limb_t a0, mp_limb_t b1, mp_limb_t b0)
{
  affirm (b0 & 1);

  اگر (!a0)
    {
      a0 = a1, a1 = 0;
      اگر (!a0)
        واپس make_uuint (b1, b0);
    }
  جاؤ make_A_odd;

  برائے (;;)
    {
      اگر ((b1 | a1) == 0)
        واپس make_uuint (0, عاد_اعظم_odd (b0, a0));

      اگر (lt2 (b1, b0, a1, a0))
        {
          sub_ddmmss (a1, a0, a1, a0, b1, b0);
          اگر (!a0)
            a0 = a1, a1 = 0;
        make_A_odd:
          assume (a0);
          صحیح ctz = stdc_trailing_zeros (a0);
          اگر (ctz)
            rsh2 (a1, a0, a1, a0, ctz);
        }
      ورنہ
        {
          sub_ddmmss (b1, b0, b1, b0, a1, a0);
          اگر (!b0)
            {
              b0 = b1, b1 = 0;
              اگر (!b0)
                توڑو;
            }
          صحیح ctz = stdc_trailing_zeros (b0);
          اگر (ctz)
            rsh2 (b1, b0, b1, b0, ctz);
        }
    }

  واپس make_uuint (a1, a0);
}

/* Store into FACTORS a prime factor PRIME with multiplicity M.  */
ساکن خالی
ضربی_عدد_داخل_کرو_multiplicity (ساخت factors *factors,
                            mp_limb_t مفرد, صحیح m)
{
  صحیح nfactors = factors->nfactors;
  mp_limb_t *p = factors->p;
  غیرعلامتی حرف *e = factors->e;

  /* Locate position for insert new or increment e.  */
  صحیح i;
  برائے (i = nfactors; 0 < i; i--)
    {
      اگر (p[i - 1] < مفرد)
        توڑو;
      اگر (p[i - 1] == مفرد)
        {
          e[i - 1] += m;
          واپس;
        }
    }

  factors->nfactors = nfactors + 1;
  memmove (&p[i + 1], &p[i], (nfactors - i) * حجم *p);
  memmove (&e[i + 1], &e[i], (nfactors - i) * حجم *e);
  e[i] = m;
  p[i] = مفرد;
}

/* Store into FACTORS a prime factor PRIME.  */
ساکن خالی
ضربی_عدد_داخل_کرو (ساخت factors *factors, mp_limb_t مفرد)
{
  ضربی_عدد_داخل_کرو_multiplicity (factors, مفرد, 1);
}

/* Store into FACTORS a prime factor (P1,P0).  If P1 != 0,
   FACTORS must not already contain a large prime factor.  */
ساکن خالی
ضربی_عدد_داخل_کرو_large (ساخت factors *factors,
                     mp_limb_t p1, mp_limb_t p0)
{
  اگر (p1 > 0)
    {
      affirm (!hi_is_سیٹ (&factors->plarge));
      factors->plarge = make_uuint (p1, p0);
    }
  ورنہ
    ضربی_عدد_داخل_کرو (factors, p0);
}

#اگر_غیر_تعریف_شدہ mpz_inits

# include <stdarg.h>

# define mpz_inits(...) mpz_va_init (mpz_init, __VA_ARGS__)
# define mpz_clears(...) mpz_va_init (mpz_clear, __VA_ARGS__)

ساکن خالی
mpz_va_init (خالی (*mpz_single_init)(mpz_t), ...)
{
  va_list ap;

  va_آغاز (ap, mpz_single_init);

  mpz_t *mpz;
  جب ((mpz = va_arg (ap, mpz_t *)))
    mpz_single_init (*mpz);

  va_انتہا (ap);
}
#اگر_ختم

#اگر_غیر_تعریف_شدہ mpn_tdiv_qr

ساکن خالی
copy_mpn_from_mpz (mp_limb_t *p, mp_سائز_t n, mpz_t z)
{
  mp_سائز_t zsize = mpz_سائز (z);
  mpn_copyi (p, mpz_limbs_read (z), zsize);
  mpn_zero (p + zsize, n - zsize);
}

ساکن خالی
mpn_tdiv_qr (mp_limb_t *qp, mp_limb_t *rp, MAYBE_غیر_مستعمل mp_سائز_t qxn,
             mp_limb_t ثابت *np, mp_سائز_t nn,
             mp_limb_t ثابت *dp, mp_سائز_t dn)
{
  mpz_t q, r, n, d;
  mpz_inits (q, r, تہی);
  mpz_tdiv_qr (q, r, mpz_roinit_n (n, np, nn), mpz_roinit_n (d, dp, dn));
  copy_mpn_from_mpz (qp, nn - dn + 1, q);
  copy_mpn_from_mpz (rp, dn, r);
  mpz_clears (q, r, تہی);
}
#اگر_ختم

ساکن ساخت mp_factors mp_ضربی_عدد (mpz_t);

/* Return an empty set of factors.  */
ساکن ساخت mp_factors
mp_no_factors (خالی)
{
  واپس (ساخت mp_factors) {0};
}

/* Free storage allocated for FACTORS, making it uninitialized.  */
ساکن خالی
mp_ضربی_عدد_clear (ساخت mp_factors *factors)
{
  ساخت mp_ضربی_عدد *f = factors->f;
  برائے (idx_t i = 0; i < factors->nfactors; i++)
    mpz_clear (f[i].p);
  آزاد (f);
}

/* Store into FACTORS a prime factor PRIME with multiplicity M.  */
ساکن خالی
mp_ضربی_عدد_داخل_کرو (ساخت mp_factors *factors, mpz_t مفرد, mp_bitcnt_t m)
{
  idx_t nfactors = factors->nfactors;
  ساخت mp_ضربی_عدد *f = factors->f;
  idx_t i;

  /* Locate position for insert new or increment e.  */
  برائے (i = nfactors; 0 < i; i--)
    {
      صحیح sgn = mpz_cmp (f[i - 1].p, مفرد);
      اگر (sgn < 0)
        توڑو;
      اگر (sgn == 0)
        {
          f[i - 1].e += m;
          واپس;
        }
    }

  اگر (nfactors == factors->nalloc)
    factors->f = f = xpalloc (f, &factors->nalloc, 1, -1, حجم *f);

  factors->nfactors = nfactors + 1;
  memmove (&f[i + 1], &f[i], (nfactors - i) * حجم *f);
  f[i].e = m;
  mpz_init_سیٹ (f[i].p, مفرد);
}

/* Store into FACTORS a prime factor PRIME with multiplicity M.  */
ساکن خالی
mp_ضربی_عدد_insert1 (ساخت mp_factors *factors, mp_limb_t مفرد, mp_bitcnt_t m)
{
  mpz_t pz = MPZ_ROINIT_N (&مفرد, 1);
  mp_ضربی_عدد_داخل_کرو (factors, pz, m);
}


/* primes_ptab[i] is prime i, where the zeroth prime is 3.
   However, the last 7 entries are sentinels.  */
#تعریف_کرو P(a,b,c) a,
ساکن int_least16_t ثابت مفرد_اعداد_ptab[] = {
#شامل "primes.h"
0,0,0,0,0,0,0                           /* 7 sentinels for 8-way loop */
};
#تعریف_منسوخ P

شمار { مفرد_اعداد_PTAB_ENTRIES = countof (مفرد_اعداد_ptab) - 8 + 1 };

ساخت مفرد_اعداد_dtab
{
  mp_limb_t binv, lim;
};

/* primes_dtab[i].binv is the multiplicative inverse of prime i
   modulo B, where the zeroth prime is 3.  That is,
   ((primes_dtab[i].binv) * (prime i)) mod B = 1.
   primes_dtab[i].lim is the maximum value that won't overflow in
   mp_limb_t arithmetic when multiplied by the ith prime.
   However, the last 7 entries of primes_dtab are sentinels.  */
#تعریف_کرو P(a,b,c) {b,c},
ساکن ثابت ساخت مفرد_اعداد_dtab مفرد_اعداد_dtab[] = {
#شامل "primes.h"
{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0} /* 7 sentinels for 8-way loop */
};
#تعریف_منسوخ P

/* Verify that a word is not wider than
   the integers used to generate primes.h.  */
static_assert (W_TYPE_سائز <= WIDE_UINT_بٹ);

/* debugging for developers.  Enables devmsg().
   This flag is used only in the GMP code.  */
ساکن bool dev_debug = false;

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
#اگر_غیر_تعریف_شدہ MR_REPS
# define MR_REPS 24
#اگر_ختم

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
ساکن اندرونی mp_limb_t
divblock (ساخت factors *factors, mp_limb_t t0, ساخت مفرد_اعداد_dtab ثابت *pd,
          idx_t i, صحیح ioff)
{
  برائے (;;)
    {
      mp_limb_t q = t0 * pd[ioff].binv;
      اگر (LIKELY (pd[ioff].lim < q))
        واپس t0;
      t0 = q;
      ضربی_عدد_داخل_کرو (factors, مفرد_اعداد_ptab[i + ioff]);
    }
}

/* Insert into FACTORS the factors of (T1,T0) found via trial division.
   The candidate factors are 2 and the primes in the primes table.
   However, primes less than prime I have
   already been considered, and need not be looked at again.
   Return (T1,T0) divided by the factors found.  */
ساکن uuint
ضربی_عدد_using_division (ساخت factors *factors, mp_limb_t t1, mp_limb_t t0,
                       idx_t i)
{
  اگر (t0 % 2 == 0)
    {
      صحیح cnt;

      اگر (t0 == 0)
        {
          assume (t1);
          cnt = stdc_trailing_zeros (t1);
          t0 = t1 >> cnt;
          t1 = 0;
          cnt += W_TYPE_سائز;
        }
      ورنہ
        {
          cnt = stdc_trailing_zeros (t0);
          rsh2 (t1, t0, t1, t0, cnt);
        }

      ضربی_عدد_داخل_کرو_multiplicity (factors, 2, cnt);
    }

  برائے (; t1 > 0 && i < مفرد_اعداد_PTAB_ENTRIES; i++)
    {
      mp_limb_t p = مفرد_اعداد_ptab[i];
      برائے (;;)
        {
          mp_limb_t q1, q0, hi;
          MAYBE_غیر_مستعمل mp_limb_t lo;

          q0 = t0 * مفرد_اعداد_dtab[i].binv;
          umul_ppmm (hi, lo, q0, p);
          اگر (hi > t1)
            توڑو;
          hi = t1 - hi;
          q1 = hi * مفرد_اعداد_dtab[i].binv;
          اگر (LIKELY (q1 > مفرد_اعداد_dtab[i].lim))
            توڑو;
          t1 = q1; t0 = q0;
          ضربی_عدد_داخل_کرو (factors, p);
        }
    }

  برائے (; i < مفرد_اعداد_PTAB_ENTRIES; i += 8)
    {
      ثابت ساخت مفرد_اعداد_dtab *pd = &مفرد_اعداد_dtab[i];
      t0 = divblock (factors, t0, pd, i, 0);
      t0 = divblock (factors, t0, pd, i, 1);
      t0 = divblock (factors, t0, pd, i, 2);
      t0 = divblock (factors, t0, pd, i, 3);
      t0 = divblock (factors, t0, pd, i, 4);
      t0 = divblock (factors, t0, pd, i, 5);
      t0 = divblock (factors, t0, pd, i, 6);
      t0 = divblock (factors, t0, pd, i, 7);

      اگر (مفرد_اعداد_PTAB_ENTRIES <= i + 8)
        توڑو;
      int_least32_t p = مفرد_اعداد_ptab[i + 8];
      اگر (p * p > t0)
        توڑو;
    }

  واپس make_uuint (t1, t0);
}

/* Return the number of limbs in positive N.  */
ساکن mp_سائز_t
mp_سائز (mpz_t n)
{
  /* Tell the compiler that N is positive; this can speed up access to N.  */
  assume (0 < mpz_sgn (n));

  واپس mpz_سائز (n);
}

/* Insert into MP_FACTORS the factors of N if N < B^2 / 2,
   and return true.  Otherwise, return false.
   Primes less than prime PRIME_IDX have
   already been considered, and need not be looked at again.  */
ساکن bool
mp_اختتام_اوپر_اندر_single (ساخت mp_factors *mp_factors, mpz_t n,
                        idx_t مفرد_idx)
{
  اگر (2 < mp_سائز (n))
    واپس false;
  mp_limb_t n1 = mpz_getlimbn (n, 1);
  اگر (n1 >> (W_TYPE_سائز - 1))
    واپس false;
  mp_limb_t n0 = mpz_getlimbn (n, 0);
  mpz_سیٹ_ui (n, 1);

  ساخت factors factors;
  ضربی_عدد_اوپر (&factors, n1, n0, مفرد_idx);

  اگر (hi_is_سیٹ (&factors.plarge))
    {
      mpz_t p = MPZ_ROINIT_N (factors.plarge.uu, 2);
      mp_ضربی_عدد_داخل_کرو (mp_factors, p, 1);
    }

  برائے (صحیح i = factors.nfactors; 0 < i; i--)
    {
      mpz_t p = MPZ_ROINIT_N (&factors.p[i - 1], 1);
      mp_ضربی_عدد_داخل_کرو (mp_factors, p, factors.e[i - 1]);
    }

  واپس true;
}

/* Insert into MP_FACTORS the factors of N if N < B^2 / 2, and
   return true.  Otherwise, return false.  N must be odd.  */
ساکن bool
mp_اختتام_اندر_single (ساخت mp_factors *mp_factors, mpz_t n)
{
  واپس mp_اختتام_اوپر_اندر_single (mp_factors, n, 0);
}

/* Return some or all factors of T.
   Divide T by the factors found.  */
ساکن ساخت mp_factors
mp_ضربی_عدد_using_division (mpz_t t)
{
  devmsg ("[trial division] ");
  ساخت mp_factors factors = mp_no_factors ();

  mp_bitcnt_t m = mpz_scan1 (t, 0);
  اگر (m)
    {
      mpz_fdiv_q_2exp (t, t, m);
      mp_ضربی_عدد_insert1 (&factors, 2, m);
      اگر (mp_اختتام_اندر_single (&factors, t))
        واپس factors;
    }

  برائے (idx_t i = 0; i < مفرد_اعداد_PTAB_ENTRIES; i++)
    {
      mp_limb_t d = مفرد_اعداد_ptab[i];
      برائے (m = 0; mpz_divisible_ui_p (t, d); m++)
        {
          mpz_divexact_ui (t, t, d);
          اگر (mp_اختتام_اوپر_اندر_single (&factors, t, i))
            {
              mp_ضربی_عدد_insert1 (&factors, d, m + 1);
              واپس factors;
            }
        }
      اگر (m)
        mp_ضربی_عدد_insert1 (&factors, d, m);
      static_assert (مربع_کا_پہلا_OMITTED_مفرد
                     <= MIN (MP_LIMB_MAX, ULONG_MAX));
      اگر (mpz_cmp_ui (t, d * d) < 0)
        توڑو;
    }

  واپس factors;
}

/* Entry i contains (2i+1)^(-1) mod 2^8.  */
ساکن ثابت غیرعلامتی حرف binvert_جدول[128] =
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
ساکن mp_limb_t
binv_limb (mp_limb_t n)
{
  mp_limb_t inv = binvert_جدول[(n / 2) & 0x7F];
  اگر ( 8 < W_TYPE_سائز) inv = 2 * inv - inv * inv * n;
  اگر (16 < W_TYPE_سائز) inv = 2 * inv - inv * inv * n;
  اگر (32 < W_TYPE_سائز) inv = 2 * inv - inv * inv * n;
  برائے (صحیح invbits = 64; invbits < W_TYPE_سائز; invbits *= 2)
    inv = 2 * inv - inv * inv * n;
  واپس inv;
}

/* q = u / d, assuming d|u.  */
#تعریف_کرو divexact_21(q1, q0, u1, u0, d)                                  \
  کرو {                                                                  \
    mp_limb_t _di, _q0;							\
    _di = binv_limb (d);						\
    _q0 = (u0) * _di;                                                   \
    اگر ((u1) >= (d))                                                    \
      {                                                                 \
        mp_limb_t _p1;							\
        MAYBE_غیر_مستعمل mp_limb_t _p0;					\
        umul_ppmm (_p1, _p0, _q0, d);                                   \
        (q1) = ((u1) - _p1) * _di;                                      \
        (q0) = _q0;                                                     \
      }                                                                 \
    ورنہ                                                                \
      {                                                                 \
        (q0) = _q0;                                                     \
        (q1) = 0;                                                       \
      }                                                                 \
  } جب (false)

/* x B (mod n).  */
#تعریف_کرو redcify(r_prim, r, n)                                           \
  کرو {                                                                  \
    MAYBE_غیر_مستعمل mp_limb_t _redcify_q;					\
    udiv_qrnnd (_redcify_q, r_prim, r, 0, n);                           \
  } جب (false)

/* x B^2 (mod n).  Requires x > 0, n1 < B/2.  */
#تعریف_کرو redcify2(r1, r0, x, n1, n0)                                     \
  کرو {                                                                  \
    mp_limb_t _r1, _r0, _i;						\
    اگر ((x) < (n1))                                                     \
      {                                                                 \
        _r1 = (x); _r0 = 0;                                             \
        _i = W_TYPE_سائز;                                               \
      }                                                                 \
    ورنہ                                                                \
      {                                                                 \
        _r1 = 0; _r0 = (x);                                             \
        _i = 2 * W_TYPE_سائز;                                           \
      }                                                                 \
    جب (_i-- > 0)                                                    \
      {                                                                 \
        lsh2 (_r1, _r0, _r1, _r0, 1);                                   \
        اگر (ge2 (_r1, _r0, n1, n0))					\
          sub_ddmmss (_r1, _r0, _r1, _r0, n1, n0);			\
      }                                                                 \
    (r1) = _r1;                                                         \
    (r0) = _r0;                                                         \
  } جب (false)

/* Modular two-word multiplication, r = a * b mod m, with mi = m^(-1) mod B.
   Both a and b must be in redc form, the result will be in redc form too.  */
ساکن اندرونی mp_limb_t
mulredc (mp_limb_t a, mp_limb_t b, mp_limb_t m, mp_limb_t mi)
{
  mp_limb_t rh, rl, q, th, xh;
  MAYBE_غیر_مستعمل mp_limb_t tl;

  umul_ppmm (rh, rl, a, b);
  q = rl * mi;
  umul_ppmm (th, tl, q, m);
  اگر (ckd_sub (&xh, rh, th))
    xh += m;

  واپس xh;
}

/* Modular two-word multiplication, r = a * b mod m, with mi = m^(-1) mod B.
   Both a and b must be in redc form, the result will be in redc form too.
   For performance reasons, the most significant bit of m must be clear.  */
ساکن mp_limb_t
mulredc2 (mp_limb_t *r1p,
          mp_limb_t a1, mp_limb_t a0, mp_limb_t b1, mp_limb_t b0,
          mp_limb_t m1, mp_limb_t m0, mp_limb_t mi)
{
  mp_limb_t r1, r0, q, p1, t1, t0, s1, s0;
  MAYBE_غیر_مستعمل mp_limb_t p0;
  mi = -mi;
  affirm ((m1 >> (W_TYPE_سائز - 1)) == 0);

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
  r0 += (t0 != 0); /*حاصل*/
  جوڑو_ssaaaa (r1, r0, r1, r0, 0, p1);
  جوڑو_ssaaaa (r1, r0, r1, r0, 0, t1);
  جوڑو_ssaaaa (r1, r0, r1, r0, s1, s0);

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
  جوڑو_ssaaaa (t1, t0, t1, t0, 0, r0);
  q = mi * t0;
  جوڑو_ssaaaa (r1, r0, s1, s0, 0, r1);
  umul_ppmm (p1, p0, q, m0);
  umul_ppmm (s1, s0, q, m1);
  r0 += (t0 != 0); /*حاصل*/
  جوڑو_ssaaaa (r1, r0, r1, r0, 0, p1);
  جوڑو_ssaaaa (r1, r0, r1, r0, 0, t1);
  جوڑو_ssaaaa (r1, r0, r1, r0, s1, s0);

  اگر (ge2 (r1, r0, m1, m0))
    sub_ddmmss (r1, r0, r1, r0, m1, m0);

  *r1p = r1;
  واپس r0;
}

ساکن bool mp_مفرد_p (mpz_t);

/* Is N prime?  N cannot be even or be a composite number less than
   SQUARE_OF_FIRST_OMITTED_PRIME.  */
ساکن bool
مفرد_p (mp_limb_t n)
{
  اگر (n <= 1)
    واپس false;

  /* We have already cast out small primes.  */
  اگر (n < مربع_کا_پہلا_OMITTED_مفرد)
    واپس true;

  mpz_t mn = MPZ_ROINIT_N (&n, 1);
  واپس mp_مفرد_p (mn);
}

/* Is (n1,n0) prime?  (n1,n0) cannot be even or be a composite number
   less than SQUARE_OF_FIRST_OMITTED_PRIME.  */
ساکن bool
prime2_p (mp_limb_t n1, mp_limb_t n0)
{
  اگر (n1 == 0)
    واپس مفرد_p (n0);

  uuint n = make_uuint (n1, n0);
  mpz_t mn = MPZ_ROINIT_N (n.uu, 2);
  واپس mp_مفرد_p (mn);
}

/* Is N prime?  N cannot be even or be a composite number less than
   SQUARE_OF_FIRST_OMITTED_PRIME.  */
ساکن bool
mp_مفرد_p (mpz_t n)
{
  اگر (mpz_cmp_ui (n, 1) <= 0)
    واپس false;

  /* We have already cast out small primes.  */
  اگر (mpz_cmp_ui (n, مربع_کا_پہلا_OMITTED_مفرد) < 0)
    واپس true;

  واپس !!mpz_probab_مفرد_p (n, MR_REPS);
}

/* Insert into FACTORS the result of factoring N,
   using Pollard-rho with starting value A.  N must be odd.  */
ساکن خالی
ضربی_عدد_using_pollard_rho (ساخت factors *factors, mp_limb_t n, mp_limb_t a)
{
  mp_limb_t x, z, y, P, t, ni, g;

  int_fast64_t k = 1;
  int_fast64_t l = 1;

  redcify (P, 1, n);
  addmod (x, P, P, n);          /* i.e., redcify(2) */
  y = z = x;

  جب (n != 1)
    {
      affirm (a < n);

      ni = binv_limb (n);	/* FIXME: when could we use old 'ni' value?  */

      برائے (;;)
        {
          کرو
            {
              x = mulredc (x, x, n, ni);
              addmod (x, x, a, n);

              submod (t, z, x, n);
              P = mulredc (P, t, n, ni);

              اگر ((k & 31) == 1)
                {
                  اگر (عاد_اعظم_odd (P, n) != 1)
                    جاؤ ضربی_عدد_ملا;
                  y = x;
                }
            }
          جب (--k != 0);

          z = x;
          k = l;
          l = 2 * l;
          برائے (int_fast64_t i = 0; i < k; i++)
            {
              x = mulredc (x, x, n, ni);
              addmod (x, x, a, n);
            }
          y = x;
        }

    ضربی_عدد_ملا:
      کرو
        {
          y = mulredc (y, y, n, ni);
          addmod (y, y, a, n);

          submod (t, z, y, n);
          g = عاد_اعظم_odd (t, n);
        }
      جب (g == 1);

      اگر (n == g)
        {
          /* Found n itself as factor.  Restart with different params.  */
          ضربی_عدد_using_pollard_rho (factors, n, a + 1);
          واپس;
        }

      n = n / g;

      اگر (!مفرد_p (g))
        ضربی_عدد_using_pollard_rho (factors, g, a + 1);
      ورنہ
        ضربی_عدد_داخل_کرو (factors, g);

      اگر (مفرد_p (n))
        {
          ضربی_عدد_داخل_کرو (factors, n);
          توڑو;
        }

      x = x % n;
      z = z % n;
      y = y % n;
    }
}

ساکن خالی
ضربی_عدد_using_pollard_rho2 (ساخت factors *factors,
                           mp_limb_t n1, mp_limb_t n0, mp_limb_t a)
{
  mp_limb_t x1, x0, z1, z0, y1, y0, P1, P0, t1, t0, g1, g0, r1m;

  int_fast64_t k = 1;
  int_fast64_t l = 1;

  redcify2 (P1, P0, 1, n1, n0);
  addmod2 (x1, x0, P1, P0, P1, P0, n1, n0); /* i.e., redcify(2) */
  y1 = z1 = x1;
  y0 = z0 = x0;

  جب (n1 != 0 || n0 != 1)
    {
      mp_limb_t ni = binv_limb (n0);

      برائے (;;)
        {
          کرو
            {
              x0 = mulredc2 (&r1m, x1, x0, x1, x0, n1, n0, ni);
              x1 = r1m;
              addmod2 (x1, x0, x1, x0, 0, a, n1, n0);

              submod2 (t1, t0, z1, z0, x1, x0, n1, n0);
              P0 = mulredc2 (&r1m, P1, P0, t1, t0, n1, n0, ni);
              P1 = r1m;

              اگر ((k & 31) == 1)
                {
                  uuset (&g1, &g0, gcd2_odd (P1, P0, n1, n0));
                  اگر (g1 != 0 || g0 != 1)
                    جاؤ ضربی_عدد_ملا;
                  y1 = x1; y0 = x0;
                }
            }
          جب (--k != 0);

          z1 = x1; z0 = x0;
          k = l;
          l = 2 * l;
          برائے (int_fast64_t i = 0; i < k; i++)
            {
              x0 = mulredc2 (&r1m, x1, x0, x1, x0, n1, n0, ni);
              x1 = r1m;
              addmod2 (x1, x0, x1, x0, 0, a, n1, n0);
            }
          y1 = x1; y0 = x0;
        }

    ضربی_عدد_ملا:
      کرو
        {
          y0 = mulredc2 (&r1m, y1, y0, y1, y0, n1, n0, ni);
          y1 = r1m;
          addmod2 (y1, y0, y1, y0, 0, a, n1, n0);

          submod2 (t1, t0, z1, z0, y1, y0, n1, n0);
          uuset (&g1, &g0, gcd2_odd (t1, t0, n1, n0));
        }
      جب (g1 == 0 && g0 == 1);

      اگر (g1 == 0)
        {
          /* The found factor is one word, and > 1.  */
          divexact_21 (n1, n0, n1, n0, g0);     /* n = n / g */

          اگر (!مفرد_p (g0))
            ضربی_عدد_using_pollard_rho (factors, g0, a + 1);
          ورنہ
            ضربی_عدد_داخل_کرو (factors, g0);
        }
      ورنہ
        {
          /* The found factor is two words.  This is highly unlikely, thus hard
             to trigger.  Please be careful before you change this code!  */

          اگر (n1 == g1 && n0 == g0)
            {
              /* Found n itself as factor.  Restart with different params.  */
              ضربی_عدد_using_pollard_rho2 (factors, n1, n0, a + 1);
              واپس;
            }

          /* Compute n = n / g.  Since the result will fit one word,
             we can compute the quotient modulo B, ignoring the high
             divisor word.  */
          n0 = binv_limb (g0) * n0;
          n1 = 0;

          اگر (!prime2_p (g1, g0))
            ضربی_عدد_using_pollard_rho2 (factors, g1, g0, a + 1);
          ورنہ
            ضربی_عدد_داخل_کرو_large (factors, g1, g0);
        }

      اگر (n1 == 0)
        {
          اگر (مفرد_p (n0))
            {
              ضربی_عدد_داخل_کرو (factors, n0);
              توڑو;
            }

          ضربی_عدد_using_pollard_rho (factors, n0, a);
          واپس;
        }

      اگر (prime2_p (n1, n0))
        {
          ضربی_عدد_داخل_کرو_large (factors, n1, n0);
          توڑو;
        }

      uuset (&x1, &x0, mod2 (x1, x0, n1, n0));
      uuset (&z1, &z0, mod2 (z1, z0, n1, n0));
      uuset (&y1, &y0, mod2 (y1, y0, n1, n0));
    }
}

/* Set RP = (AP + BP) mod MP.  All values are nonnegative and take up
   N>0 words, and AP and BP are both less than MP.  */
ساکن خالی
mp_modadd (mp_limb_t *rp, mp_limb_t ثابت *ap, mp_limb_t ثابت *bp,
           mp_limb_t ثابت *mp, mp_سائز_t n)
{
  mp_limb_t cy = mpn_جوڑو_n (rp, ap, bp, n);
  اگر (cy || mpn_cmp (rp, mp, n) >= 0)
    mpn_sub_n (rp, rp, mp, n);
}

/* Set RP = (AP - BP) mod MP.  All values are nonnegative and take up
   N>0 words, and AP and BP are both less than MP.  */
ساکن خالی
mp_modsub (mp_limb_t *rp, mp_limb_t ثابت *ap, mp_limb_t ثابت *bp,
           mp_limb_t ثابت *mp, mp_سائز_t n)
{
  mp_limb_t cy = mpn_sub_n (rp, ap, bp, n);
  اگر (cy)
    mpn_جوڑو_n (rp, rp, mp, n);
}

/* Set RP = (AP - B0) mod MP.  All values are nonnegative, AP and MP
   both take up N>0 words, and AP < MP.  */
ساکن خالی
mp_modadd_1 (mp_limb_t *rp, mp_limb_t ثابت *ap, mp_limb_t b0,
             mp_limb_t ثابت *mp, mp_سائز_t n)
{
  mp_limb_t cy = mpn_جوڑو_1 (rp, ap, n, b0);
  اگر (cy || mpn_cmp (rp, mp, n) >= 0)
    mpn_sub_n (rp, rp, mp, n);
}

ساکن خالی
mp_mulredc (mp_limb_t *rp, mp_limb_t ثابت *ap, mp_limb_t ثابت *bp,
            mp_limb_t ثابت *mp, mp_سائز_t n, mp_limb_t m0inv, mp_limb_t *tp)
{
  tp[n] = mpn_mul_1 (tp, ap, n, bp[0]);
  tp[0] = mpn_addmul_1 (tp, mp, n, tp[0] * m0inv);

  برائے (mp_سائز_t i = 1; i < n; i++)
    {
      tp[n + i] = mpn_addmul_1 (tp + i, ap, n, bp[i]);
      tp[i] = mpn_addmul_1 (tp + i, mp, n, tp[i] * m0inv);
    }
  mp_modadd (rp, tp, tp + n, mp, n);
}

/* Maximum value for N in mp_factor_using_pollard_rho,
   to avoid integer overflow when it calls xinmalloc.  */
#تعریف_کرو MP_ضربی_عدد_USING_POLLARD_RHO_N_MAX \
  ((MIN (IDX_MAX, TYPE_زیادہ_سے_زیادہ (mp_سائز_t)) - 3) / 10)

/* Insert into FACTORS the result of factoring MP, of size N,
   using Pollard-rho with starting value A.  MP must be odd. */
ساکن خالی
mp_ضربی_عدد_using_pollard_rho (ساخت mp_factors *factors,
                             mp_limb_t ثابت *mp, mp_سائز_t n,
                             mp_limb_t a)
{
  devmsg ("[pollard-rho (%lu)] ", (غیرعلامتی طویل صحیح) a);

  static_assert (10 * MP_ضربی_عدد_USING_POLLARD_RHO_N_MAX + 3
                 <= MIN (IDX_MAX, TYPE_زیادہ_سے_زیادہ (mp_سائز_t)));
  mp_limb_t *scratch = xinmalloc (10 * n + 3, حجم *scratch);
  mp_limb_t *qp = scratch + 2 * n + 1, *pp = qp + n + 2,
    *xp = pp + n, *yp = xp + n, *zp = yp + n,
    *tp = zp + n, *sp = tp + n, *gp = sp + n;
  mp_سائز_t gn;

  mpn_zero (scratch, n);
  scratch[n] = 1;
  mpn_tdiv_qr (qp, pp, 0, scratch, n + 1, mp, n);

  mp_modadd (xp, pp, pp, mp, n);
  mpn_copyi (yp, xp, n);
  mpn_copyi (zp, xp, n);

  mp_limb_t m0inv = binv_limb (-mp[0]);

  برائے (int_fast64_t k = 1; ; k *= 2)
    {
      برائے (int_fast64_t i = k; 0 < i; i--)
        {
          mp_mulredc (tp, xp, xp, mp, n, m0inv, scratch);
          mp_modadd_1 (xp, tp, a, mp, n);

          mp_modsub (tp, zp, xp, mp, n);
          mp_mulredc (pp, pp, tp, mp, n, m0inv, scratch);

          اگر (i % 128 == 1)
            {
              اگر (mpn_zero_p (pp, n))
                {
                  mp_ضربی_عدد_using_pollard_rho (factors, mp, n, a + 1);
                  جاؤ اختتام;
                }
              mpn_copyi (tp, pp, n);
              mpn_copyi (sp, mp, n);
              gn = mpn_عاد_اعظم (gp, tp, n, sp, n);
              اگر (gn != 1 || gp[0] != 1)
                جاؤ ضربی_عدد_ملا;
              mpn_copyi (yp, xp, n);
            }
        }

      mpn_copyi (zp, xp, n);
      برائے (int_fast64_t i = 2 * k; 0 < i; i--)
        {
          mp_mulredc (tp, xp, xp, mp, n, m0inv, scratch);
          mp_modadd_1 (xp, tp, a, mp, n);
        }
      mpn_copyi (yp, xp, n);
    }

 ضربی_عدد_ملا:
  کرو
    {
      mp_mulredc (tp, yp, yp, mp, n, m0inv, scratch);
      mp_modadd_1 (yp, tp, a, mp, n);
      mp_modsub (tp, zp, yp, mp, n);
      mpn_copyi (sp, mp, n);
      gn = mpn_عاد_اعظم (gp, tp, n, sp, n);
    }
  جب (gn == 1 && gp[0] == 1);

  mpz_t g = MPZ_ROINIT_N (gp, gn);
  mpz_t m = MPZ_ROINIT_N ((mp_limb_t *) mp, n), q;
  mpz_init (q);
  mpz_divexact (q, m, g);

  اگر (!mp_اختتام_اندر_single (factors, g))
    {
      اگر (mp_مفرد_p (g))
        mp_ضربی_عدد_داخل_کرو (factors, g, 1);
      ورنہ
        mp_ضربی_عدد_using_pollard_rho (factors, gp, gn, a + 1);
    }

  اگر (!mp_اختتام_اندر_single (factors, q))
    {
      اگر (mp_مفرد_p (q))
        mp_ضربی_عدد_داخل_کرو (factors, q, 1);
      ورنہ
        {
          devmsg ("[composite factor--restarting pollard-rho] ");
          mp_ضربی_عدد_using_pollard_rho (factors, mpz_limbs_read (q),
                                       mp_سائز (q), a + 1);
        }
    }

  mpz_clear (q);

 اختتام:
  آزاد (scratch);
}

/* Insert into FACTORS the prime factors of the two-word number (T1,T0).
   Primes less than the prime with index PRIME_IDX
   have already been considered, and need not be looked at again.  */
ساکن خالی
ضربی_عدد_اوپر (ساخت factors *factors, mp_limb_t t1, mp_limb_t t0,
           idx_t مفرد_idx)
{
  factors->nfactors = 0;
  hiset (&factors->plarge, 0);

  اگر (t1 == 0 && t0 < 2)
    واپس;

  uuset (&t1, &t0, ضربی_عدد_using_division (factors, t1, t0, مفرد_idx));

  اگر (t1 == 0 && t0 < 2)
    واپس;

  اگر (prime2_p (t1, t0))
    ضربی_عدد_داخل_کرو_large (factors, t1, t0);
  ورنہ
    {
      اگر (t1 == 0)
        ضربی_عدد_using_pollard_rho (factors, t0, 1);
      ورنہ
        ضربی_عدد_using_pollard_rho2 (factors, t1, t0, 1);
    }
}

/* Compute the prime factors of the two-word number (T1,T0),
   and put the results in FACTORS.  */
ساکن خالی
ضربی_عدد (ساخت factors *factors, mp_limb_t t1, mp_limb_t t0)
{
  ضربی_عدد_اوپر (factors, t1, t0, 0);
}

/* Return the prime factors of T.  */
ساکن ساخت mp_factors
mp_ضربی_عدد (mpz_t t)
{
  ساخت mp_factors factors = mp_no_factors ();

  اگر (mpz_sgn (t) != 0)
    {
      factors = mp_ضربی_عدد_using_division (t);

      اگر (mpz_cmp_ui (t, 1) != 0)
        {
          devmsg ("[is number prime?] ");
          اگر (mp_مفرد_p (t))
            mp_ضربی_عدد_داخل_کرو (&factors, t, 1);
          ورنہ
            mp_ضربی_عدد_using_pollard_rho (&factors, mpz_limbs_read (t),
                                         mp_سائز (t), 1);
        }
    }

  واپس factors;
}

/* Convert to *U the value represented by S, and return LONGINT_OK.
   Return LONGINT_OVERFLOW if S is composed of all digits but is too
   large to fit in *U, otherwise LONGINT_INVALID if S is not a valid
   positive integer.  */
ساکن strtol_error
strtouuint (uuint *u, حرف ثابت *s)
{
  mp_limb_t hi = 0, lo = *s++ - '0';

  اگر (UNLIKELY (9 < lo))
    واپس LONGINT_INVALID;

  برائے (; LIKELY (0 <= *s - '0' && *s - '0' <= 9); s++)
    {
      اگر (UNLIKELY (ckd_mul (&hi, hi, 10)))
        جاؤ حد_سے_تجاوز;

      صحیح lo_حاصل = (lo >> (W_TYPE_سائز - 3)) + (lo >> (W_TYPE_سائز - 1));
      lo_حاصل += 10 * lo < 2 * lo;

      lo = 10 * lo;
      lo_حاصل += ckd_جوڑو (&lo, lo, *s - '0');
      اگر (UNLIKELY (ckd_جوڑو (&hi, hi, lo_حاصل)))
        جاؤ حد_سے_تجاوز;
    }

  اگر (UNLIKELY (*s))
    واپس LONGINT_INVALID;

  *u = make_uuint (hi, lo);
  واپس LONGINT_OK;

 حد_سے_تجاوز:
  /* Since s[0] caused the overflow, we know it is a valid integer.  */
  s++;
  واپس s[strspn (s, "0123456789")] ? LONGINT_INVALID : LONGINT_حد_سے_تجاوز;
}

/* FACTOR_PIPE_BUF is chosen to give good performance,
   and also is the max guaranteed size that
   consumers can read atomically through pipes.
   Also it's big enough to cater for max line length
   even with 128 bit word.  */
#اگر_غیر_تعریف_شدہ _POSIX_PIPE_BUF
# define _POSIX_PIPE_BUF 512
#اگر_ختم
#اگر_تعریف_شدہ PIPE_BUF
شمار { ضربی_عدد_PIPE_BUF = PIPE_BUF };
#ورنہ
شمار { ضربی_عدد_PIPE_BUF = _POSIX_PIPE_BUF };
#اگر_ختم

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
ساکن حرف lbuf_buf[2 * ضربی_عدد_PIPE_BUF];
ساکن idx_t lbuffered;

/* Write complete LBUF to standard output.  */
ساکن خالی
lbuf_flush (خالی)
{
  idx_t سائز = lbuffered;

  /* Update lbuffered now, to avoid infinite recursion on write error.  */
  lbuffered = 0;

  اگر (بھرا_write (STDOUT_FILENO, lbuf_buf, سائز) != سائز)
    write_error ();
}

/* Write LBUF to standard output.
   LBUF should contain at least FACTOR_PIPE_BUF bytes.
   If possible, write a prefix of LBUF that is newline terminated
   and contains <= FACTOR_PIPE_BUF bytes, so consumers can read atomically.
   But if the first FACTOR_PIPE_BUF bytes contain no newlines,
   give up on atomicity and just write the first FACTOR_PIPE_BUF bytes.  */
ساکن خالی
lbuf_half_flush (خالی)
{
  حرف *nl = memrchr (lbuf_buf, '\n', ضربی_عدد_PIPE_BUF);
  حرف *suffix = nl ? nl + 1 : lbuf_buf + ضربی_عدد_PIPE_BUF;
  idx_t سابقہ_سائز = suffix - lbuf_buf;
  idx_t suffix_سائز = lbuffered - سابقہ_سائز;
  lbuffered = سابقہ_سائز;
  lbuf_flush ();
  lbuffered = suffix_سائز;
  memmove (lbuf_buf, suffix, suffix_سائز);
}

/* Add a character C to lbuf_buf.  */
ساکن خالی
lbuf_putc (حرف c)
{
  lbuf_buf[lbuffered++] = c;
}

/* Add a newline to lbuf_buf.  Then, if enough bytes are already
   buffered, write the buffer atomically to standard output.  */
ساکن خالی
lbuf_putnl (خالی)
{
  lbuf_putc ('\n');

  /* Provide immediate output for interactive use.  */
  ساکن صحیح سطر_buffered = -1;
  اگر (سطر_buffered < 0)
    سطر_buffered = isatty (STDOUT_FILENO);

  اگر (سطر_buffered)
    lbuf_flush ();
  ورنہ اگر (ضربی_عدد_PIPE_BUF <= lbuffered)
    lbuf_half_flush ();
}

/* Append the string representation of I to lbuf_buf, followed by
   everything from BUFEND to lbuf_buf's end.  Use the area just before
   BUFEND temporarily.  */
ساکن خالی
lbuf_putint_append (mp_limb_t i, حرف *bufend)
{
  حرف *istr = bufend;
  کرو
    {
      *--istr = '0' + i % 10;
      i /= 10;
    }
  جب (i);

  حرف *p = lbuf_buf + lbuffered;
  کرو
    *p++ = *istr++;
  جب (istr < lbuf_buf + حجم lbuf_buf);

  lbuffered = p - lbuf_buf;
}

/* Append the string representation of I to lbuf_buf.  */
ساکن خالی
lbuf_putint (mp_limb_t i)
{
  lbuf_putint_append (i, lbuf_buf + حجم lbuf_buf);
}
ساکن خالی
lbuf_putbitcnt (mp_bitcnt_t i)
{
  حرف *bufend = lbuf_buf + حجم lbuf_buf;
  برائے (; MP_LIMB_MAX < i; i /= 10)
    *--bufend = '0' + i % 10;
  lbuf_putint_append (i, bufend);
}

/* Append the string representation of T to lbuf_buf.  */
ساکن خالی
print_uuint (uuint t)
{
  mp_limb_t t1 = hi (t), t0 = lo (t);
  حرف *bufend = lbuf_buf + حجم lbuf_buf;

  جب (t1)
    {
      mp_limb_t r = t1 % BIG_POWER_کا_10;
      t1 /= BIG_POWER_کا_10;
      udiv_qrnnd (t0, r, r, t0, BIG_POWER_کا_10);
      برائے (صحیح i = 0; i < LOG_BIG_POWER_کا_10; i++)
        {
          *--bufend = '0' + r % 10;
          r /= 10;
        }
    }

  lbuf_putint_append (t0, bufend);
}

/* Buffer the mpz I to lbuf_buf, possibly writing if it is long.  */
ساکن خالی
lbuf_putmpz (mpz_t ثابت i)
{
  idx_t sizeinbase = mpz_sizeinbase (i, 10);
  حرف *lbuf_bufend = lbuf_buf + حجم lbuf_buf;
  حرف *p = lbuf_buf + lbuffered;
  اگر (sizeinbase < lbuf_bufend - p)
    {
      mpz_get_str (p, 10, i);
      p += sizeinbase;
      lbuffered = p - !p[-1] - lbuf_buf;
      جب (ضربی_عدد_PIPE_BUF <= lbuffered)
        lbuf_half_flush ();
    }
  ورنہ
    {
      lbuf_flush ();
      حرف *istr = ximalloc (sizeinbase + 1);
      mpz_get_str (istr, 10, i);
      idx_t istrlen = sizeinbase - !istr[sizeinbase - 1];
      اگر (بھرا_write (STDOUT_FILENO, istr, istrlen) != istrlen)
        write_error ();
      آزاد (istr);
    }
}

/* Emit the factors of T, which is less than B^2 / 2.  */
ساکن خالی
print_factors_single (uuint t)
{
  ساخت factors factors;

  print_uuint (t);
  lbuf_putc (':');

  ضربی_عدد (&factors, hi (t), lo (t));

  برائے (صحیح j = 0; j < factors.nfactors; j++)
    برائے (صحیح k = 0; k < factors.e[j]; k++)
      {
        lbuf_putc (' ');
        print_uuint (make_uuint (0, factors.p[j]));
        اگر (print_exponents && factors.e[j] > 1)
          {
            lbuf_putc ('^');
            lbuf_putint (factors.e[j]);
            توڑو;
          }
      }

  اگر (hi_is_سیٹ (&factors.plarge))
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
ساکن bool
print_factors (حرف ثابت *داخلہ)
{
  /* Skip initial spaces and '+'.  */
  حرف ثابت *str = داخلہ;
  جب (*str == ' ')
    str++;
  str += *str == '+';

  /* Try converting the number to one or two words.  If it fails, use GMP or
     print an error message.  The 2nd condition checks that the most
     significant bit of the two-word number is clear, in a typesize neutral
     way.  */
  uuint u;
  strtol_error err = strtouuint (&u, str);
  mpz_t t;

  انتخاب (err)
    {
    حالت LONGINT_OK:
      اگر (hi (u) >> (W_TYPE_سائز - 1) == 0)
        {
          devmsg ("[using single-precision arithmetic] ");
          print_factors_single (u);
          واپس true;
        }
      FALLTHROUGH;
    حالت LONGINT_حد_سے_تجاوز:
      /* Try GMP.  */
      اگر (0 <= mpz_init_سیٹ_str (t, str, 10))
        توڑو;
      FALLTHROUGH;

    حالت LONGINT_INVALID:
    حالت LONGINT_INVALID_SUFFIX_CHAR:
    حالت LONGINT_INVALID_SUFFIX_CHAR_WITH_حد_سے_تجاوز:
    طےشدہ:
      error (0, 0, _("%s is not a valid positive integer"), quote (داخلہ));
      واپس false;
    }

  devmsg ("[using arbitrary-precision arithmetic] ");
  اگر (MP_ضربی_عدد_USING_POLLARD_RHO_N_MAX < mp_سائز (t))
    xalloc_die ();

  lbuf_putmpz (t);
  lbuf_putc (':');
  ساخت mp_factors factors = mp_ضربی_عدد (t);

  برائے (idx_t j = 0; j < factors.nfactors; j++)
    برائے (mp_bitcnt_t k = 0; k < factors.f[j].e; k++)
      {
        lbuf_putc (' ');
        lbuf_putmpz (factors.f[j].p);
        اگر (print_exponents && factors.f[j].e > 1)
          {
            lbuf_putc ('^');
            lbuf_putbitcnt (factors.f[j].e);
            توڑو;
          }
      }

  mp_ضربی_عدد_clear (&factors);
  mpz_clear (t);
  lbuf_putnl ();
  واپس true;
}

/* Output a usage diagnostic and exit with STATUS.  */
خالی
usage (صحیح حالت_2)
{
  اگر (حالت_2 != EXIT_SUCCESS)
    emit_try_help ();
  ورنہ
    {
      لکھو (_("\
Usage: %s [OPTION] [نمبر_2]...\n\
"),
              پروگرام_name);
      fputs (_("\
Print the مفرد factors کا each specified integer نمبر_2.  If none\n\
are specified on the command سطر, اقرأ them from standard داخلہ.\n\
\n\
"), stdout);
      oputs (_("\
  -h, --exponents\n\
         print repeated factors اندر form p^e unless e is 1\n\
"));
      oputs (HELP_OPTION_DESCRIPTION);
      oputs (نسخہ_OPTION_DESCRIPTION);
      emit_ancillary_info (پروگرام_NAME);
    }
  باہر_نکلو (حالت_2);
}

/* Read numbers from stdin, one per line, and output their factors.  */
ساکن bool
do_stdin (خالی)
{
  bool ok = true;
  token_بفر tokenbuffer;

  init_tokenbuffer (&tokenbuffer);

  جب (true)
    {
      size_t token_لمبائی = readtoken (stdin, DELIM, حجم (DELIM) - 1,
                                       &tokenbuffer);
      اگر (token_لمبائی == (size_t) -1)
        {
          اگر (ferror (stdin))
            error (EXIT_FAILURE, errno, _("error reading input"));
          توڑو;
        }

      ok &= print_factors (tokenbuffer.بفر);
    }
  آزاد (tokenbuffer.بفر);

  واپس ok;
}

صحیح
مین (صحیح argc, حرف **argv)
{
  initialize_main (&argc, &argv);
  سیٹ_پروگرام_name (argv[0]);
  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);

  atexit (close_stdout);

  صحیح c;
  جب ((c = getopt_long (argc, argv, "h", long_options, تہی)) != -1)
    {
      انتخاب (c)
        {
        حالت 'h':  /* NetBSD used -h for this functionality first.  */
          print_exponents = true;
          توڑو;

        حالت DEV_DEBUG_OPTION:
          dev_debug = true;
          توڑو;

        case_GETOPT_HELP_CHAR;

        case_GETOPT_نسخہ_CHAR (پروگرام_NAME, AUTHORS);

        طےشدہ:
          usage (EXIT_FAILURE);
        }
    }

  atexit (lbuf_flush);

  bool ok;
  اگر (argc <= optind)
    ok = do_stdin ();
  ورنہ
    {
      ok = true;
      برائے (صحیح i = optind; i < argc; i++)
        اگر (! print_factors (argv[i]))
          ok = false;
    }

  واپس ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
