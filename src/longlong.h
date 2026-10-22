/* longlong.h -- definitions for mixed size 32/64 bit arithmetic.

Copyright 1991-1994, 1996, 1997, 1999-2005, 2007-2009, 2011-2020 Free Software
Foundation, Inc.

This file is part of the GNU MP Library.

The GNU MP Library is free software; you can redistribute it and/or modify
it under the terms of either:

  * the GNU Lesser General Public License as published by the Free
    Software Foundation; either version 3 of the License, or (at your
    option) any later version.

or

  * the GNU General Public License as published by the Free Software
    Foundation; either version 2 of the License, or (at your option) any
    later version.

or both in parallel, as here.

The GNU MP Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received copies of the GNU General Public License and the
GNU Lesser General Public License along with the GNU MP Library.  If not,
see https://www.gnu.org/licenses/.  */

/* You have to define the following before including this file:

   UWtype -- An unsigned type, default type for operations (typically a "word")
   UHWtype -- An unsigned type, at least half the size of UWtype
   UDWtype -- An unsigned type, at least twice as large a UWtype
   W_TYPE_SIZE -- size in bits of UWtype

   SItype, USItype -- Signed and unsigned 32 bit types
   DItype, UDItype -- Signed and unsigned 64 bit types

   On a 32 bit machine UWtype should typically be USItype;
   on a 64 bit machine, UWtype should typically be UDItype.

   Optionally, define:

   LONGLONG_STANDALONE -- Avoid code that needs machine-dependent support files
   NO_ASM -- Disable inline asm


   CAUTION!  Using this version of longlong.h outside of GMP is not safe.  You
   need to include gmp.h and gmp-impl.h, or certain things might not work as
   expected.
*/

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __BITS4 (W_TYPE_ਮਾਪ / 4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ll_B ((UWtype) 1 << (W_TYPE_ਮਾਪ / 2))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ll_lowpart(t) ((UWtype) (t) & (__ll_B - 1))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __ll_highpart(t) ((UWtype) (t) >> (W_TYPE_ਮਾਪ / 2))

/* This is used to make sure no undesirable sharing between different libraries
   that use this file takes place.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __MPN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __MPN(x) __##x
#ਜੇ_ਅੰਤ

/* Define auxiliary asm macros.

   1) umul_ppmm(high_prod, low_prod, multiplier, multiplicand) multiplies two
   UWtype integers MULTIPLIER and MULTIPLICAND, and generates a two UWtype
   word product in HIGH_PROD and LOW_PROD.

   2) __umulsidi3(a,b) multiplies two UWtype integers A and B, and returns a
   UDWtype product.  This is just a variant of umul_ppmm.

   3) udiv_qrnnd(quotient, remainder, high_numerator, low_numerator,
   denominator) divides a UDWtype, composed by the UWtype integers
   HIGH_NUMERATOR and LOW_NUMERATOR, by DENOMINATOR and places the quotient
   in QUOTIENT and the remainder in REMAINDER.  HIGH_NUMERATOR must be less
   than DENOMINATOR for correct operation.  If, in addition, the most
   significant bit of DENOMINATOR must be 1, then the pre-processor symbol
   UDIV_NEEDS_NORMALIZATION is defined to 1.

   4) sdiv_qrnnd(quotient, remainder, high_numerator, low_numerator,
   denominator).  Like udiv_qrnnd but the numbers are signed.  The quotient
   is rounded towards 0.

   5) count_leading_zeros(count, x) counts the number of zero-bits from the
   msb to the first non-zero bit in the UWtype X.  This is the number of
   steps X needs to be shifted left to set the msb.  Undefined for X == 0,
   unless the symbol COUNT_LEADING_ZEROS_0 is defined to some value.

   6) count_trailing_zeros(count, x) like count_leading_zeros, but counts
   from the least significant end.

   7) add_ssaaaa(high_sum, low_sum, high_addend_1, low_addend_1,
   high_addend_2, low_addend_2) adds two UWtype integers, composed by
   HIGH_ADDEND_1 and LOW_ADDEND_1, and HIGH_ADDEND_2 and LOW_ADDEND_2
   respectively.  The result is placed in HIGH_SUM and LOW_SUM.  Overflow
   (i.e. carry out) is not stored anywhere, and is lost.

   8) sub_ddmmss(high_difference, low_difference, high_minuend, low_minuend,
   high_subtrahend, low_subtrahend) subtracts two two-word UWtype integers,
   composed by HIGH_MINUEND_1 and LOW_MINUEND_1, and HIGH_SUBTRAHEND_2 and
   LOW_SUBTRAHEND_2 respectively.  The result is placed in HIGH_DIFFERENCE
   and LOW_DIFFERENCE.  Overflow (i.e. carry out) is not stored anywhere,
   and is lost.

   If any of these macros are left undefined for a particular CPU,
   C macros are used.


   Notes:

   For add_ssaaaa the two high and two low addends can both commute, but
   unfortunately gcc only supports one "%" commutative in each asm block.
   This has always been so but is only documented in recent versions
   (eg. pre-release 3.3).  Having two or more "%"s can cause an internal
   compiler error in certain rare circumstances.

   Apparently it was only the last "%" that was ever actually respected, so
   the code has been updated to leave just that.  Clearly there's a free
   choice whether high or low should get it, if there's a reason to favour
   one over the other.  Also obviously when the constraints on the two
   operands are identical there's no benefit to the reloader in any "%" at
   all.

   */

/* The CPUs come in alphabetical order below.

   Please add support for more CPUs here, or improve the current support
   for the CPUs below!  */


/* count_leading_zeros_gcc_clz is count_leading_zeros implemented with gcc
   3.4 __builtin_clzl or __builtin_clzll, according to our limb size.
   Similarly count_trailing_zeros_gcc_ctz using __builtin_ctzl or
   __builtin_ctzll.

   These builtins are only used when we check what code comes out, on some
   chips they're merely libgcc calls, where we will instead want an inline
   in that case (either asm or generic C).

   These builtins are better than an asm block of the same insn, since an
   asm block doesn't give gcc any information about scheduling or resource
   usage.  We keep an asm block for use on prior versions of gcc though.

   For reference, __builtin_ffs existed in gcc prior to __builtin_clz, but
   it's not used (for count_leading_zeros) because it generally gives extra
   code to ensure the result is 0 when the input is 0, which we don't need
   or want.  */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _LONG_LONG_LIMB
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros_gcc_clz(ਗਿਣਤੀ_2,x)	\
  ਕਰੋ {						\
    ASSERT ((x) != 0);				\
    (ਗਿਣਤੀ_2) = __builtin_clzll (x);		\
  } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros_gcc_clz(ਗਿਣਤੀ_2,x)	\
  ਕਰੋ {						\
    ASSERT ((x) != 0);				\
    (ਗਿਣਤੀ_2) = __builtin_clzl (x);		\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _LONG_LONG_LIMB
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros_gcc_ctz(ਗਿਣਤੀ_2,x)	\
  ਕਰੋ {						\
    ASSERT ((x) != 0);				\
    (ਗਿਣਤੀ_2) = __builtin_ctzll (x);		\
  } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros_gcc_ctz(ਗਿਣਤੀ_2,x)	\
  ਕਰੋ {						\
    ASSERT ((x) != 0);				\
    (ਗਿਣਤੀ_2) = __builtin_ctzl (x);		\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ


/* FIXME: The macros using external routines like __MPN(count_leading_zeros)
   don't need to be under !NO_ASM */
#ਜੇ ! defined (NO_ASM)

#ਜੇ defined (__alpha) && W_TYPE_ਮਾਪ == 64
/* Most alpha-based machines, except Cray systems. */
#ਜੇ defined (__GNUC__)
#ਜੇ __GMP_GNUC_PREREQ (3,3)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1) \
  ਕਰੋ {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    (ph) = __builtin_alpha_umulh (__m0, __m1);				\
    (pl) = __m0 * __m1;							\
  } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1) \
  ਕਰੋ {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("umulh %r1,%2,%0"						\
	     : "=r" (ph)						\
	     : "%rJ" (__m0), "rI" (__m1));				\
    (pl) = __m0 * __m1;							\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ /* ! __GNUC__ */
#ਅਨ੍ਤਰ੍ਭਾਵ <machine/builtins.h>
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1) \
  ਕਰੋ {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    (ph) = __UMULH (__m0, __m1);					\
    (pl) = __m0 * __m1;							\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LONGLONG_STANDALONE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  ਕਰੋ { UWtype __di;							\
    __di = __MPN(invert_limb) (d);					\
    udiv_qrnnd_preinv (q, r, n1, n0, d, __di);				\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UDIV_PREINV_ALWAYS  1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UDIV_NEEDS_NORMALIZATION 1
#ਜੇ_ਅੰਤ /* LONGLONG_STANDALONE */

/* clz_tab is required in all configurations, since mpn/alpha/cntlz.asm
   always goes into libgmp.so, even when not actually used.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_NEED_CLZ_TAB

#ਜੇ defined (__GNUC__) && HAVE_HOST_CPU_alpha_CIX
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_3,X) \
  __asm__("ctlz %1,%0" : "=r"(ਗਿਣਤੀ_3) : "r"(X))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros(ਗਿਣਤੀ_3,X) \
  __asm__("cttz %1,%0" : "=r"(ਗਿਣਤੀ_3) : "r"(X))
#ਜੇ_ਅੰਤ /* clz/ctz using cix */

#ਜੇ ! defined (ਗਿਣਤੀ_leading_zeros)				\
  && defined (__GNUC__) && ! defined (LONGLONG_STANDALONE)
/* ALPHA_CMPBGE_0 gives "cmpbge $31,src,dst", ie. test src bytes == 0.
   "$31" is written explicitly in the asm, since an "r" constraint won't
   select reg 31.  There seems no need to worry about "r31" syntax for cray,
   since gcc itself (pre-release 3.4) emits just $31 in various places.	 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ALPHA_CMPBGE_0(dst, src)					\
  ਕਰੋ { asm ("cmpbge $31, %1, %0" : "=r" (dst) : "r" (src)); } ਜਦੋਂ (0)
/* Zero bytes are turned into bits with cmpbge, a __clz_tab lookup counts
   them, locating the highest non-zero byte.  A second __clz_tab lookup
   counts the leading zero bits in that byte, giving the result.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x)					\
  ਕਰੋ {									\
    UWtype  __clz__b, __clz__c, __clz__x = (x);				\
    ALPHA_CMPBGE_0 (__clz__b,  __clz__x);	    /* zero bytes */	\
    __clz__b = __clz_tab [(__clz__b >> 1) ^ 0x7F];  /* 8 to 1 byte */	\
    __clz__b = __clz__b * 8 - 7;		    /* 57 to 1 shift */ \
    __clz__x >>= __clz__b;						\
    __clz__c = __clz_tab [__clz__x];		    /* 8 to 1 bit */	\
    __clz__b = 65 - __clz__b;						\
    (ਗਿਣਤੀ_2) = __clz__b - __clz__c;					\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_NEED_CLZ_TAB
#ਜੇ_ਅੰਤ /* clz using cmpbge */

#ਜੇ ! defined (ਗਿਣਤੀ_leading_zeros) && ! defined (LONGLONG_STANDALONE)
#ਜੇ HAVE_ATTRIBUTE_CONST
ਲੰਮਾ __MPN(ਗਿਣਤੀ_leading_zeros) (UDItype) __attribute__ ((ਅਚਲ));
#ਨਹੀਂਤਾਂ
ਲੰਮਾ __MPN(ਗਿਣਤੀ_leading_zeros) (UDItype);
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  ((ਗਿਣਤੀ_2) = __MPN(ਗਿਣਤੀ_leading_zeros) (x))
#ਜੇ_ਅੰਤ /* clz using mpn */
#ਜੇ_ਅੰਤ /* __alpha */

#ਜੇ defined (__AVR) && W_TYPE_ਮਾਪ == 8
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1) \
  ਕਰੋ {									\
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ __p = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ) (m0) * (m1);			\
    (ph) = __p >> 8;							\
    (pl) = __p;								\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* AVR */

#ਜੇ defined (_CRAY) && W_TYPE_ਮਾਪ == 64
#ਅਨ੍ਤਰ੍ਭਾਵ <intrinsics.h>
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UDIV_PREINV_ALWAYS  1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UDIV_NEEDS_NORMALIZATION 1
ਲੰਮਾ __MPN(ਗਿਣਤੀ_leading_zeros) (UDItype);
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  ((ਗਿਣਤੀ_2) = _leadz ((UWtype) (x)))
#ਜੇ defined (_CRAYIEEE)		/* I.e., Cray T90/ieee, T3D, and T3E */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1) \
  ਕਰੋ {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    (ph) = int_mult_ਉੱਤਲਾ (__m0, __m1);				\
    (pl) = __m0 * __m1;							\
  } ਜਦੋਂ (0)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LONGLONG_STANDALONE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  ਕਰੋ { UWtype __di;							\
    __di = __MPN(invert_limb) (d);					\
    udiv_qrnnd_preinv (q, r, n1, n0, d, __di);				\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* LONGLONG_STANDALONE */
#ਜੇ_ਅੰਤ /* _CRAYIEEE */
#ਜੇ_ਅੰਤ /* _CRAY */

#ਜੇ defined (__ia64) && W_TYPE_ਮਾਪ == 64
/* This form encourages gcc (pre-release 3.4 at least) to emit predicated
   "sub r=r,r" and "sub r=r,r,1", giving a 2 cycle latency.  The generic
   code using "al<bl" arithmetically comes out making an actual 0 or 1 in a
   register, which takes an extra cycle.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl)      \
  ਕਰੋ {						\
    UWtype __x;					\
    __x = (al) - (bl);				\
    ਜੇ ((al) < (bl))				\
      (sh) = (ah) - (bh) - 1;			\
    ਨਹੀਂਤਾਂ					\
      (sh) = (ah) - (bh);			\
    (sl) = __x;					\
  } ਜਦੋਂ (0)
#ਜੇ defined (__GNUC__) && ! defined (__INTEL_COMPILER)
/* Do both product parts in assembly, since that gives better code with
   all gcc versions.  Some callers will just use the upper part, and in
   that situation we waste an instruction, but not any cycles.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1) \
    __asm__ ("xma.hu %0 = %2, %3, f0\n\txma.l %1 = %2, %3, f0"		\
	     : "=&f" (ph), "=f" (pl)					\
	     : "f" (m0), "f" (m1))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  ਕਰੋ {									\
    UWtype _x = (x), _y, _a, _c;					\
    __asm__ ("mux1 %0 = %1, @rev" : "=r" (_y) : "r" (_x));		\
    __asm__ ("czx1.l %0 = %1" : "=r" (_a) : "r" (-_y | _y));		\
    _c = (_a - 1) << 3;							\
    _x >>= _c;								\
    ਜੇ (_x >= 1 << 4)							\
      _x >>= 4, _c += 4;						\
    ਜੇ (_x >= 1 << 2)							\
      _x >>= 2, _c += 2;						\
    _c += _x >> 1;							\
    (ਗਿਣਤੀ_2) =  W_TYPE_ਮਾਪ - 1 - _c;					\
  } ਜਦੋਂ (0)
/* similar to what gcc does for __builtin_ffs, but 0 based rather than 1
   based, and we don't need a special case for x==0 here */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros(ਗਿਣਤੀ_2, x)					\
  ਕਰੋ {									\
    UWtype __ctz_x = (x);						\
    __asm__ ("popcnt %0 = %1"						\
	     : "=r" (ਗਿਣਤੀ_2)						\
	     : "r" ((__ctz_x-1) & ~__ctz_x));				\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ defined (__INTEL_COMPILER)
#ਅਨ੍ਤਰ੍ਭਾਵ <ia64intrin.h>
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1)					\
  ਕਰੋ {									\
    UWtype __m0 = (m0), __m1 = (m1);					\
    ph = _m64_xmahu (__m0, __m1, 0);					\
    pl = __m0 * __m1;							\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LONGLONG_STANDALONE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  ਕਰੋ { UWtype __di;							\
    __di = __MPN(invert_limb) (d);					\
    udiv_qrnnd_preinv (q, r, n1, n0, d, __di);				\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UDIV_PREINV_ALWAYS  1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UDIV_NEEDS_NORMALIZATION 1
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ


#ਜੇ defined (__GNUC__)

/* We sometimes need to clobber "cc" with gcc2, but that would not be
   understood by gcc1.  Use cpp to avoid major code duplication.  */
#ਜੇ __GNUC__ < 2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __CLOBBER_CC
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __AND_CLOBBER_CC
#ਨਹੀਂਤਾਂ /* __GNUC__ >= 2 */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __CLOBBER_CC : "cc"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __AND_CLOBBER_CC , "cc"
#ਜੇ_ਅੰਤ /* __GNUC__ < 2 */

#ਜੇ (defined (__a29k__) || defined (_AM29K)) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add %1,%4,%5\n\taddc %0,%2,%3"				\
	   : "=r" (sh), "=&r" (sl)					\
	   : "r" (ah), "rI" (bh), "%r" (al), "rI" (bl))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub %1,%4,%5\n\tsubc %0,%2,%3"				\
	   : "=r" (sh), "=&r" (sl)					\
	   : "r" (ah), "rI" (bh), "r" (al), "rI" (bl))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(xh, xl, m0, m1) \
  ਕਰੋ {									\
    USItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("multiplu %0,%1,%2"					\
	     : "=r" (xl)						\
	     : "r" (__m0), "r" (__m1));					\
    __asm__ ("multmu %0,%1,%2"						\
	     : "=r" (xh)						\
	     : "r" (__m0), "r" (__m1));					\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  __asm__ ("dividu %0,%3,%4"						\
	   : "=r" (q), "=q" (r)						\
	   : "1" (n1), "r" (n0), "r" (d))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
    __asm__ ("clz %0,%1"						\
	     : "=r" (ਗਿਣਤੀ_2)						\
	     : "r" (x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_0 32
#ਜੇ_ਅੰਤ /* __a29k__ */

#ਜੇ defined (__arc__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add.f\t%1, %4, %5\n\tadc\t%0, %2, %3"			\
	   : "=r" (sh),							\
	     "=&r" (sl)							\
	   : "r"  ((USItype) (ah)),					\
	     "rICal" ((USItype) (bh)),					\
	     "%r" ((USItype) (al)),					\
	     "rICal" ((USItype) (bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub.f\t%1, %4, %5\n\tsbc\t%0, %2, %3"			\
	   : "=r" (sh),							\
	     "=&r" (sl)							\
	   : "r" ((USItype) (ah)),					\
	     "rICal" ((USItype) (bh)),					\
	     "r" ((USItype) (al)),					\
	     "rICal" ((USItype) (bl)))
#ਜੇ_ਅੰਤ

#ਜੇ defined (__arm__) && (defined (__thumb2__) || !defined (__thumb__)) \
    && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  ਕਰੋ {									\
    ਜੇ (__builtin_constant_p (bl) && -(USItype)(bl) < (USItype)(bl))	\
      __asm__ ("subs\t%1, %4, %5\n\tadc\t%0, %2, %3"			\
	   : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "rI" (bh),					\
		 "%r" (al), "rI" (-(USItype)(bl)) __CLOBBER_CC);	\
    ਨਹੀਂਤਾਂ								\
      __asm__ ("adds\t%1, %4, %5\n\tadc\t%0, %2, %3"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "r" (ah), "rI" (bh), "%r" (al), "rI" (bl) __CLOBBER_CC);	\
  } ਜਦੋਂ (0)
/* FIXME: Extend the immediate range for the low word by using both ADDS and
   SUBS, since they set carry in the same way.  We need separate definitions
   for thumb and non-thumb since thumb lacks RSC.  */
#ਜੇ defined (__thumb__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  ਕਰੋ {									\
    ਜੇ (__builtin_constant_p (ah) && __builtin_constant_p (bh)		\
	&& (ah) == (bh))						\
      __asm__ ("subs\t%1, %2, %3\n\tsbc\t%0, %0, %0"			\
	       : "=r" (sh), "=r" (sl)					\
	       : "r" (al), "rI" (bl) __CLOBBER_CC);			\
    ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (al))					\
      __asm__ ("rsbs\t%1, %5, %4\n\tsbc\t%0, %2, %3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "rI" (bh), "rI" (al), "r" (bl) __CLOBBER_CC); \
    ਨਹੀਂਤਾਂ								\
      __asm__ ("subs\t%1, %4, %5\n\tsbc\t%0, %2, %3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "rI" (bh), "r" (al), "rI" (bl) __CLOBBER_CC); \
    } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  ਕਰੋ {									\
    ਜੇ (__builtin_constant_p (ah) && __builtin_constant_p (bh)		\
	&& (ah) == (bh))						\
      __asm__ ("subs\t%1, %2, %3\n\tsbc\t%0, %0, %0"			\
	       : "=r" (sh), "=r" (sl)					\
	       : "r" (al), "rI" (bl) __CLOBBER_CC);			\
    ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (al))					\
      {									\
	ਜੇ (__builtin_constant_p (ah))					\
	  __asm__ ("rsbs\t%1, %5, %4\n\trsc\t%0, %3, %2"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "rI" (ah), "r" (bh), "rI" (al), "r" (bl) __CLOBBER_CC); \
	ਨਹੀਂਤਾਂ								\
	  __asm__ ("rsbs\t%1, %5, %4\n\tsbc\t%0, %2, %3"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r" (ah), "rI" (bh), "rI" (al), "r" (bl) __CLOBBER_CC); \
      }									\
    ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (ah))					\
      {									\
	ਜੇ (__builtin_constant_p (bl))					\
	  __asm__ ("subs\t%1, %4, %5\n\trsc\t%0, %3, %2"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "rI" (ah), "r" (bh), "r" (al), "rI" (bl) __CLOBBER_CC); \
	ਨਹੀਂਤਾਂ								\
	  __asm__ ("rsbs\t%1, %5, %4\n\trsc\t%0, %3, %2"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "rI" (ah), "r" (bh), "rI" (al), "r" (bl) __CLOBBER_CC); \
      }									\
    ਨਹੀਂਤਾਂ								\
      __asm__ ("subs\t%1, %4, %5\n\tsbc\t%0, %2, %3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "rI" (bh), "r" (al), "rI" (bl) __CLOBBER_CC); \
    } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ defined (__ARM_ARCH_2__) || defined (__ARM_ARCH_2A__) \
    || defined (__ARM_ARCH_3__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(xh, xl, a, b)						\
  ਕਰੋ {									\
    ਰਜਿਸਟਰ USItype __t0, __t1, __t2;					\
    __asm__ ("%@ Inlined umul_ppmm\n"					\
	   "	mov	%2, %5, lsr #16\n"				\
	   "	mov	%0, %6, lsr #16\n"				\
	   "	bic	%3, %5, %2, lsl #16\n"				\
	   "	bic	%4, %6, %0, lsl #16\n"				\
	   "	mul	%1, %3, %4\n"					\
	   "	mul	%4, %2, %4\n"					\
	   "	mul	%3, %0, %3\n"					\
	   "	mul	%0, %2, %0\n"					\
	   "	adds	%3, %4, %3\n"					\
	   "	addcs	%0, %0, #65536\n"				\
	   "	adds	%1, %1, %3, lsl #16\n"				\
	   "	adc	%0, %0, %3, lsr #16"				\
	   : "=&r" ((USItype) (xh)), "=r" ((USItype) (xl)),		\
	     "=&r" (__t0), "=&r" (__t1), "=r" (__t2)			\
	   : "r" ((USItype) (a)), "r" ((USItype) (b)) __CLOBBER_CC);	\
  } ਜਦੋਂ (0)
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LONGLONG_STANDALONE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  ਕਰੋ { UWtype __r;							\
    (q) = __MPN(udiv_qrnnd) (&__r, (n1), (n0), (d));			\
    (r) = __r;								\
  } ਜਦੋਂ (0)
ਬਾਹਰੀ UWtype __MPN(udiv_qrnnd) (UWtype *, UWtype, UWtype, UWtype);
#ਜੇ_ਅੰਤ /* LONGLONG_STANDALONE */
#ਨਹੀਂਤਾਂ /* ARMv4 or newer */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(xh, xl, a, b) \
  __asm__ ("umull %0,%1,%2,%3" : "=&r" (xl), "=&r" (xh) : "r" (a), "r" (b))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ smul_ppmm(xh, xl, a, b) \
  __asm__ ("smull %0,%1,%2,%3" : "=&r" (xl), "=&r" (xh) : "r" (a), "r" (b))
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LONGLONG_STANDALONE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  ਕਰੋ { UWtype __di;							\
    __di = __MPN(invert_limb) (d);					\
    udiv_qrnnd_preinv (q, r, n1, n0, d, __di);				\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UDIV_PREINV_ALWAYS  1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UDIV_NEEDS_NORMALIZATION 1
#ਜੇ_ਅੰਤ /* LONGLONG_STANDALONE */
#ਜੇ_ਅੰਤ /* defined(__ARM_ARCH_2__) ... */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x)  ਗਿਣਤੀ_leading_zeros_gcc_clz(ਗਿਣਤੀ_2, x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros(ਗਿਣਤੀ_2, x)  ਗਿਣਤੀ_trailing_zeros_gcc_ctz(ਗਿਣਤੀ_2, x)
#ਜੇ_ਅੰਤ /* __arm__ */

#ਜੇ defined (__aarch64__) && W_TYPE_ਮਾਪ == 64
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  ਕਰੋ {									\
    ਜੇ (__builtin_constant_p (bl) && ~(UDItype)(bl) <= (UDItype)(bl))	\
      __asm__ ("subs\t%1, %x4, %5\n\tadc\t%0, %x2, %x3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "rZ" ((UDItype)(ah)), "rZ" ((UDItype)(bh)),		\
		 "%r" ((UDItype)(al)), "rI" (-(UDItype)(bl)) __CLOBBER_CC);\
    ਨਹੀਂਤਾਂ								\
      __asm__ ("adds\t%1, %x4, %5\n\tadc\t%0, %x2, %x3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "rZ" ((UDItype)(ah)), "rZ" ((UDItype)(bh)),		\
		 "%r" ((UDItype)(al)), "rI" ((UDItype)(bl)) __CLOBBER_CC);\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  ਕਰੋ {									\
    ਜੇ (__builtin_constant_p (bl) && ~(UDItype)(bl) <= (UDItype)(bl))	\
      __asm__ ("adds\t%1, %x4, %5\n\tsbc\t%0, %x2, %x3"			\
	       : "=r,r" (sh), "=&r,&r" (sl)				\
	       : "rZ,rZ" ((UDItype)(ah)), "rZ,rZ" ((UDItype)(bh)),	\
		 "r,Z"   ((UDItype)(al)), "rI,r" (-(UDItype)(bl)) __CLOBBER_CC);\
    ਨਹੀਂਤਾਂ								\
      __asm__ ("subs\t%1, %x4, %5\n\tsbc\t%0, %x2, %x3"			\
	       : "=r,r" (sh), "=&r,&r" (sl)				\
	       : "rZ,rZ" ((UDItype)(ah)), "rZ,rZ" ((UDItype)(bh)),	\
		 "r,Z"   ((UDItype)(al)), "rI,r"  ((UDItype)(bl)) __CLOBBER_CC);\
  } ਜਦੋਂ(0);
#ਜੇ __GMP_GNUC_PREREQ (4,9)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  ਕਰੋ {									\
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __ll_UTItype __attribute__((ਬਹੁਲਕ_2(TI)));	\
    __ll_UTItype __ll = (__ll_UTItype)(u) * (v);			\
    w1 = __ll >> 64;							\
    w0 = __ll;								\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ !defined (umul_ppmm)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1) \
  ਕਰੋ {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("umulh\t%0, %1, %2" : "=r" (ph) : "r" (__m0), "r" (__m1));	\
    (pl) = __m0 * __m1;							\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x)  ਗਿਣਤੀ_leading_zeros_gcc_clz(ਗਿਣਤੀ_2, x)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros(ਗਿਣਤੀ_2, x)  ਗਿਣਤੀ_trailing_zeros_gcc_ctz(ਗਿਣਤੀ_2, x)
#ਜੇ_ਅੰਤ /* __aarch64__ */

#ਜੇ defined (__clipper__) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  ({ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __l, __h;} __i;				\
	  } __x;							\
  __asm__ ("mulwux %2,%0"						\
	   : "=r" (__x.__ll)						\
	   : "%0" ((USItype)(u)), "r" ((USItype)(v)));			\
  (w1) = __x.__i.__h; (w0) = __x.__i.__l;})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ smul_ppmm(w1, w0, u, v) \
  ({ਸੰਘ {DItype __ll;							\
	   ਬਣਤਰ {SItype __l, __h;} __i;				\
	  } __x;							\
  __asm__ ("mulwx %2,%0"						\
	   : "=r" (__x.__ll)						\
	   : "%0" ((SItype)(u)), "r" ((SItype)(v)));			\
  (w1) = __x.__i.__h; (w0) = __x.__i.__l;})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __umulsidi3(u, v) \
  ({UDItype __w;							\
    __asm__ ("mulwux %2,%0"						\
	     : "=r" (__w) : "%0" ((USItype)(u)), "r" ((USItype)(v)));	\
    __w; })
#ਜੇ_ਅੰਤ /* __clipper__ */

/* Fujitsu vector computers.  */
#ਜੇ defined (__uxp__) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, u, v) \
  ਕਰੋ {									\
    ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("mult.lu %1,%2,%0"	: "=r" (__x.__ll) : "%r" (u), "rK" (v));\
    (ph) = __x.__i.__h;							\
    (pl) = __x.__i.__l;							\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ smul_ppmm(ph, pl, u, v) \
  ਕਰੋ {									\
    ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("mult.l %1,%2,%0" : "=r" (__x.__ll) : "%r" (u), "rK" (v));	\
    (ph) = __x.__i.__h;							\
    (pl) = __x.__i.__l;							\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਜੇ defined (__gmicro__) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add.w %5,%1\n\taddx %3,%0"					\
	   : "=g" (sh), "=&g" (sl)					\
	   : "0"  ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "g" ((USItype)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub.w %5,%1\n\tsubx %3,%0"					\
	   : "=g" (sh), "=&g" (sl)					\
	   : "0" ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "g" ((USItype)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1) \
  __asm__ ("mulx %3,%0,%1"						\
	   : "=g" (ph), "=r" (pl)					\
	   : "%0" ((USItype)(m0)), "g" ((USItype)(m1)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, nh, nl, d) \
  __asm__ ("divx %4,%0,%1"						\
	   : "=g" (q), "=r" (r)						\
	   : "1" ((USItype)(nh)), "0" ((USItype)(nl)), "g" ((USItype)(d)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  __asm__ ("bsch/1 %1,%0"						\
	   : "=g" (ਗਿਣਤੀ_2) : "g" ((USItype)(x)), "0" ((USItype)0))
#ਜੇ_ਅੰਤ

#ਜੇ defined (__hppa) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add%I5 %5,%r4,%1\n\taddc %r2,%r3,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rM" (ah), "rM" (bh), "%rM" (al), "rI" (bl))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub%I4 %4,%r5,%1\n\tsubb %r2,%r3,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rM" (ah), "rM" (bh), "rI" (al), "rM" (bl))
#ਜੇ defined (_PA_RISC1_1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(wh, wl, u, v) \
  ਕਰੋ {									\
    ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("xmpyu %1,%2,%0" : "=*f" (__x.__ll) : "*f" (u), "*f" (v));	\
    (wh) = __x.__i.__h;							\
    (wl) = __x.__i.__l;							\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  ਕਰੋ {									\
    USItype __tmp;							\
    __asm__ (								\
       "ldi		1,%0\n"						\
"	extru,=		%1,15,16,%%r0	; Bits 31..16 zero?\n"		\
"	extru,tr	%1,15,16,%1	; No.  Shift down, skip add.\n"	\
"	ldo		16(%0),%0	; Yes.  Perform add.\n"		\
"	extru,=		%1,23,8,%%r0	; Bits 15..8 zero?\n"		\
"	extru,tr	%1,23,8,%1	; No.  Shift down, skip add.\n"	\
"	ldo		8(%0),%0	; Yes.  Perform add.\n"		\
"	extru,=		%1,27,4,%%r0	; Bits 7..4 zero?\n"		\
"	extru,tr	%1,27,4,%1	; No.  Shift down, skip add.\n"	\
"	ldo		4(%0),%0	; Yes.  Perform add.\n"		\
"	extru,=		%1,29,2,%%r0	; Bits 3..2 zero?\n"		\
"	extru,tr	%1,29,2,%1	; No.  Shift down, skip add.\n"	\
"	ldo		2(%0),%0	; Yes.  Perform add.\n"		\
"	extru		%1,30,1,%1	; Extract bit 1.\n"		\
"	sub		%0,%1,%0	; Subtract it.\n"		\
	: "=r" (ਗਿਣਤੀ_2), "=r" (__tmp) : "1" (x));			\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* hppa */

/* These macros are for ABI=2.0w.  In ABI=2.0n they can't be used, since GCC
   (3.2) puts longlong into two adjacent 32-bit registers.  Presumably this
   is just a case of no direct support for 2.0n but treating it like 1.0. */
#ਜੇ defined (__hppa) && W_TYPE_ਮਾਪ == 64 && ! defined (_LONG_LONG_LIMB)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add%I5 %5,%r4,%1\n\tadd,dc %r2,%r3,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rM" (ah), "rM" (bh), "%rM" (al), "rI" (bl))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub%I4 %4,%r5,%1\n\tsub,db %r2,%r3,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rM" (ah), "rM" (bh), "rI" (al), "rM" (bl))
#ਜੇ_ਅੰਤ /* hppa */

#ਜੇ (defined (__i370__) || defined (__s390__) || defined (__mvs__)) && W_TYPE_ਮਾਪ == 32
#ਜੇ defined (__zarch__) || defined (HAVE_HOST_CPU_s390_zarch)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl)				\
  ਕਰੋ {									\
/*  if (__builtin_constant_p (bl))					\
      __asm__ ("alfi\t%1,%o5\n\talcr\t%0,%3"				\
	       : "=r" (sh), "=&r" (sl)					\
	       : "0"  (ah), "r" (bh), "%1" (al), "n" (bl) __CLOBBER_CC);\
    else								\
*/    __asm__ ("alr\t%1,%5\n\talcr\t%0,%3"				\
	       : "=r" (sh), "=&r" (sl)					\
	       : "0"  (ah), "r" (bh), "%1" (al), "r" (bl)__CLOBBER_CC);	\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl)				\
  ਕਰੋ {									\
/*  if (__builtin_constant_p (bl))					\
      __asm__ ("slfi\t%1,%o5\n\tslbr\t%0,%3"				\
	       : "=r" (sh), "=&r" (sl)					\
	       : "0" (ah), "r" (bh), "1" (al), "n" (bl) __CLOBBER_CC);	\
    else								\
*/    __asm__ ("slr\t%1,%5\n\tslbr\t%0,%3"				\
	       : "=r" (sh), "=&r" (sl)					\
	       : "0" (ah), "r" (bh), "1" (al), "r" (bl) __CLOBBER_CC);	\
  } ਜਦੋਂ (0)
#ਜੇ __GMP_GNUC_PREREQ (4,5)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(xh, xl, m0, m1)					\
  ਕਰੋ {									\
    ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __h, __l;} __i;				\
	  } __x;							\
    __x.__ll = (UDItype) (m0) * (UDItype) (m1);				\
    (xh) = __x.__i.__h; (xl) = __x.__i.__l;				\
  } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
#ਜੇ 0
/* FIXME: this fails if gcc knows about the 64-bit registers.  Use only
   with a new enough processor pretending we have 32-bit registers.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(xh, xl, m0, m1)					\
  ਕਰੋ {									\
    ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("mlr\t%0,%2"						\
	     : "=r" (__x.__ll)						\
	     : "%0" (m0), "r" (m1));					\
    (xh) = __x.__i.__h; (xl) = __x.__i.__l;				\
  } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(xh, xl, m0, m1)					\
  ਕਰੋ {									\
  /* When we have 64-bit regs and gcc is aware of that, we cannot simply use
     DImode for the product, since that would be allocated to a single 64-bit
     register, whereas mlr uses the low 32-bits of an even-odd register pair.
  */									\
    ਰਜਿਸਟਰ USItype __r0 __asm__ ("0");				\
    ਰਜਿਸਟਰ USItype __r1 __asm__ ("1") = (m0);				\
    __asm__ ("mlr\t%0,%3"						\
	     : "=r" (__r0), "=r" (__r1)					\
	     : "r" (__r1), "r" (m1));					\
    (xh) = __r0; (xl) = __r1;						\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* if 0 */
#ਜੇ_ਅੰਤ
#ਜੇ 0
/* FIXME: this fails if gcc knows about the 64-bit registers.  Use only
   with a new enough processor pretending we have 32-bit registers.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d)					\
  ਕਰੋ {									\
    ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __h, __l;} __i;				\
	  } __x;							\
    __x.__i.__h = n1; __x.__i.__l = n0;					\
    __asm__ ("dlr\t%0,%2"						\
	     : "=r" (__x.__ll)						\
	     : "0" (__x.__ll), "r" (d));				\
    (q) = __x.__i.__l; (r) = __x.__i.__h;				\
  } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d)					\
  ਕਰੋ {									\
    ਰਜਿਸਟਰ USItype __r0 __asm__ ("0") = (n1);				\
    ਰਜਿਸਟਰ USItype __r1 __asm__ ("1") = (n0);				\
    __asm__ ("dlr\t%0,%4"						\
	     : "=r" (__r0), "=r" (__r1)					\
	     : "r" (__r0), "r" (__r1), "r" (d));			\
    (q) = __r1; (r) = __r0;						\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* if 0 */
#ਨਹੀਂਤਾਂ /* if __zarch__ */
/* FIXME: this fails if gcc knows about the 64-bit registers.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ smul_ppmm(xh, xl, m0, m1)					\
  ਕਰੋ {									\
    ਸੰਘ {DItype __ll;							\
	   ਬਣਤਰ {USItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("mr\t%0,%2"						\
	     : "=r" (__x.__ll)						\
	     : "%0" (m0), "r" (m1));					\
    (xh) = __x.__i.__h; (xl) = __x.__i.__l;				\
  } ਜਦੋਂ (0)
/* FIXME: this fails if gcc knows about the 64-bit registers.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sdiv_qrnnd(q, r, n1, n0, d)					\
  ਕਰੋ {									\
    ਸੰਘ {DItype __ll;							\
	   ਬਣਤਰ {USItype __h, __l;} __i;				\
	  } __x;							\
    __x.__i.__h = n1; __x.__i.__l = n0;					\
    __asm__ ("dr\t%0,%2"						\
	     : "=r" (__x.__ll)						\
	     : "0" (__x.__ll), "r" (d));				\
    (q) = __x.__i.__l; (r) = __x.__i.__h;				\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* if __zarch__ */
#ਜੇ_ਅੰਤ

#ਜੇ defined (__s390x__) && W_TYPE_ਮਾਪ == 64
/* We need to cast operands with register constraints, otherwise their types
   will be assumed to be SImode by gcc.  For these machines, such operations
   will insert a value into the low 32 bits, and leave the high 32 bits with
   garbage.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl)				\
  ਕਰੋ {									\
    __asm__ ("algr\t%1,%5\n\talcgr\t%0,%3"				\
	       : "=r" (sh), "=&r" (sl)					\
	       : "0"  ((UDItype)(ah)), "r" ((UDItype)(bh)),		\
		 "%1" ((UDItype)(al)), "r" ((UDItype)(bl)) __CLOBBER_CC); \
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl)				\
  ਕਰੋ {									\
    __asm__ ("slgr\t%1,%5\n\tslbgr\t%0,%3"				\
	     : "=r" (sh), "=&r" (sl)					\
	     : "0" ((UDItype)(ah)), "r" ((UDItype)(bh)),		\
	       "1" ((UDItype)(al)), "r" ((UDItype)(bl)) __CLOBBER_CC);	\
  } ਜਦੋਂ (0)
#ਜੇ !defined (__clang__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(xh, xl, m0, m1)					\
  ਕਰੋ {									\
    ਸੰਘ {ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __attribute__ ((ਬਹੁਲਕ_2(TI))) __ll;		\
	   ਬਣਤਰ {UDItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("mlgr\t%0,%2"						\
	     : "=r" (__x.__ll)						\
	     : "%0" ((UDItype)(m0)), "r" ((UDItype)(m1)));		\
    (xh) = __x.__i.__h; (xl) = __x.__i.__l;				\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d)					\
  ਕਰੋ {									\
    ਸੰਘ {ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __attribute__ ((ਬਹੁਲਕ_2(TI))) __ll;		\
	   ਬਣਤਰ {UDItype __h, __l;} __i;				\
	  } __x;							\
    __x.__i.__h = n1; __x.__i.__l = n0;					\
    __asm__ ("dlgr\t%0,%2"						\
	     : "=r" (__x.__ll)						\
	     : "0" (__x.__ll), "r" ((UDItype)(d)));			\
    (q) = __x.__i.__l; (r) = __x.__i.__h;				\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ 0 /* FIXME: Enable for z10 (?) */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(cnt, x)					\
  ਕਰੋ {									\
    ਸੰਘ {ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __attribute__ ((ਬਹੁਲਕ_2(TI))) __ll;		\
	   ਬਣਤਰ {UDItype __h, __l;} __i;				\
	  } __clr_cnt;							\
    __asm__ ("flogr\t%0,%1"						\
	     : "=r" (__clr_cnt.__ll)					\
	     : "r" (x) __CLOBBER_CC);					\
    (cnt) = __clr_cnt.__i.__h;						\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

/* On x86 and x86_64, every asm implicitly clobbers "flags" and "fpsr",
   so we don't need __CLOBBER_CC.  */
#ਜੇ (defined (__i386__) || defined (__i486__)) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addl %5,%k1\n\tadcl %3,%k0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0"  ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "g" ((USItype)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subl %5,%k1\n\tsbbl %3,%k0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0" ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "g" ((USItype)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("mull %3"							\
	   : "=a" (w0), "=d" (w1)					\
	   : "%0" ((USItype)(u)), "rm" ((USItype)(v)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, dx) /* d renamed to dx avoiding "=d" */\
  __asm__ ("divl %4"		     /* stringification in K&R C */	\
	   : "=a" (q), "=d" (r)						\
	   : "0" ((USItype)(n0)), "1" ((USItype)(n1)), "rm" ((USItype)(dx)))

#ਜੇ HAVE_HOST_CPU_i586 || HAVE_HOST_CPU_pentium || HAVE_HOST_CPU_pentiummmx
/* Pentium bsrl takes between 10 and 72 cycles depending where the most
   significant 1 bit is, hence the use of the following alternatives.  bsfl
   is slow too, between 18 and 42 depending where the least significant 1
   bit is, so let the generic count_trailing_zeros below make use of the
   count_leading_zeros here too.  */

#ਜੇ HAVE_HOST_CPU_pentiummmx && ! defined (LONGLONG_STANDALONE)
/* The following should be a fixed 14 or 15 cycles, but possibly plus an L1
   cache miss reading from __clz_tab.  For P55 it's favoured over the float
   below so as to avoid mixing MMX and x87, since the penalty for switching
   between the two is about 100 cycles.

   The asm block sets __shift to -3 if the high 24 bits are clear, -2 for
   16, -1 for 8, or 0 otherwise.  This could be written equivalently as
   follows, but as of gcc 2.95.2 it results in conditional jumps.

       __shift = -(__n < 0x1000000);
       __shift -= (__n < 0x10000);
       __shift -= (__n < 0x100);

   The middle two sbbl and cmpl's pair, and with luck something gcc
   generates might pair with the first cmpl and the last sbbl.  The "32+1"
   constant could be folded into __clz_tab[], but it doesn't seem worth
   making a different table just for that.  */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(c,n)					\
  ਕਰੋ {									\
    USItype  __n = (n);							\
    USItype  __shift;							\
    __asm__ ("cmpl  $0x1000000, %1\n"					\
	     "sbbl  %0, %0\n"						\
	     "cmpl  $0x10000, %1\n"					\
	     "sbbl  $0, %0\n"						\
	     "cmpl  $0x100, %1\n"					\
	     "sbbl  $0, %0\n"						\
	     : "=&r" (__shift) : "r"  (__n));				\
    __shift = __shift*8 + 24 + 1;					\
    (c) = 32 + 1 - __shift - __clz_tab[__n >> __shift];			\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_NEED_CLZ_TAB
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_0   31   /* n==0 indistinguishable from n==1 */

#ਨਹੀਂਤਾਂ /* ! pentiummmx || LONGLONG_STANDALONE */
/* The following should be a fixed 14 cycles or so.  Some scheduling
   opportunities should be available between the float load/store too.  This
   sort of code is used in gcc 3 for __builtin_ffs (with "n&-n") and is
   apparently suggested by the Intel optimizing manual (don't know exactly
   where).  gcc 2.95 or up will be best for this, so the "double" is
   correctly aligned on the stack.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(c,n)					\
  ਕਰੋ {									\
    ਸੰਘ {								\
      ਦੁੱਗਣਾ    d;							\
      ਬਿਨਾਨਿਸ਼ਾਨੀ  a[2];							\
    } __u;								\
    __u.d = (UWtype) (n);						\
    (c) = 0x3FF + 31 - (__u.a[1] >> 20);				\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_0   (0x3FF + 31)
#ਜੇ_ਅੰਤ /* pentiummx */

#ਨਹੀਂਤਾਂ /* ! pentium */

#ਜੇ __GMP_GNUC_PREREQ (3,4)  /* using bsrl */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2,x)  ਗਿਣਤੀ_leading_zeros_gcc_clz(ਗਿਣਤੀ_2,x)
#ਜੇ_ਅੰਤ /* gcc clz */

/* On P6, gcc prior to 3.0 generates a partial register stall for
   __cbtmp^31, due to using "xorb $31" instead of "xorl $31", the former
   being 1 code byte smaller.  "31-__cbtmp" is a workaround, probably at the
   cost of one extra instruction.  Do this for "i386" too, since that means
   generic x86.  */
#ਜੇ ! defined (ਗਿਣਤੀ_leading_zeros) && __GNUC__ < 3			\
  && (HAVE_HOST_CPU_i386						\
      || HAVE_HOST_CPU_i686						\
      || HAVE_HOST_CPU_pentiumpro					\
      || HAVE_HOST_CPU_pentium2						\
      || HAVE_HOST_CPU_pentium3)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x)					\
  ਕਰੋ {									\
    USItype __cbtmp;							\
    ASSERT ((x) != 0);							\
    __asm__ ("bsrl %1,%0" : "=r" (__cbtmp) : "rm" ((USItype)(x)));	\
    (ਗਿਣਤੀ_2) = 31 - __cbtmp;						\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* gcc<3 asm bsrl */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗਿਣਤੀ_leading_zeros
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x)					\
  ਕਰੋ {									\
    USItype __cbtmp;							\
    ASSERT ((x) != 0);							\
    __asm__ ("bsrl %1,%0" : "=r" (__cbtmp) : "rm" ((USItype)(x)));	\
    (ਗਿਣਤੀ_2) = __cbtmp ^ 31;						\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* asm bsrl */

#ਜੇ __GMP_GNUC_PREREQ (3,4)  /* using bsfl */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros(ਗਿਣਤੀ_2,x)  ਗਿਣਤੀ_trailing_zeros_gcc_ctz(ਗਿਣਤੀ_2,x)
#ਜੇ_ਅੰਤ /* gcc ctz */

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਗਿਣਤੀ_trailing_zeros
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros(ਗਿਣਤੀ_2, x)					\
  ਕਰੋ {									\
    ASSERT ((x) != 0);							\
    __asm__ ("bsfl %1,%k0" : "=r" (ਗਿਣਤੀ_2) : "rm" ((USItype)(x)));	\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* asm bsfl */

#ਜੇ_ਅੰਤ /* ! pentium */

#ਜੇ_ਅੰਤ /* 80x86 */

#ਜੇ defined (__amd64__) && W_TYPE_ਮਾਪ == 64
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addq %5,%q1\n\tadcq %3,%q0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0"  ((UDItype)(ah)), "rme" ((UDItype)(bh)),		\
	     "%1" ((UDItype)(al)), "rme" ((UDItype)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subq %5,%q1\n\tsbbq %3,%q0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0" ((UDItype)(ah)), "rme" ((UDItype)(bh)),		\
	     "1" ((UDItype)(al)), "rme" ((UDItype)(bl)))
#ਜੇ X86_ASM_MULX \
   && (HAVE_HOST_CPU_haswell || HAVE_HOST_CPU_broadwell \
       || HAVE_HOST_CPU_skylake || HAVE_HOST_CPU_bd4 || HAVE_HOST_CPU_zen)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("mulx\t%3, %q0, %q1"						\
	   : "=r" (w0), "=r" (w1)					\
	   : "%d" ((UDItype)(u)), "rm" ((UDItype)(v)))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("mulq\t%3"							\
	   : "=a" (w0), "=d" (w1)					\
	   : "%0" ((UDItype)(u)), "rm" ((UDItype)(v)))
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, dx) /* d renamed to dx avoiding "=d" */\
  __asm__ ("divq %4"		     /* stringification in K&R C */	\
	   : "=a" (q), "=d" (r)						\
	   : "0" ((UDItype)(n0)), "1" ((UDItype)(n1)), "rm" ((UDItype)(dx)))

#ਜੇ HAVE_HOST_CPU_haswell || HAVE_HOST_CPU_broadwell || HAVE_HOST_CPU_skylake \
  || HAVE_HOST_CPU_k10 || HAVE_HOST_CPU_bd1 || HAVE_HOST_CPU_bd2	\
  || HAVE_HOST_CPU_bd3 || HAVE_HOST_CPU_bd4 || HAVE_HOST_CPU_zen	\
  || HAVE_HOST_CPU_bobcat || HAVE_HOST_CPU_jaguar
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x)					\
  ਕਰੋ {									\
    /* This is lzcnt, spelled for older assemblers.  Destination and */	\
    /* source must be a 64-bit registers, hence cast and %q.         */	\
    __asm__ ("rep;bsr\t%1, %q0" : "=r" (ਗਿਣਤੀ_2) : "rm" ((UDItype)(x)));	\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_0 64
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x)					\
  ਕਰੋ {									\
    UDItype __cbtmp;							\
    ASSERT ((x) != 0);							\
    __asm__ ("bsr\t%1,%0" : "=r" (__cbtmp) : "rm" ((UDItype)(x)));	\
    (ਗਿਣਤੀ_2) = __cbtmp ^ 63;						\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਜੇ HAVE_HOST_CPU_bd2 || HAVE_HOST_CPU_bd3 || HAVE_HOST_CPU_bd4 \
  || HAVE_HOST_CPU_zen || HAVE_HOST_CPU_jaguar
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros(ਗਿਣਤੀ_2, x)					\
  ਕਰੋ {									\
    /* This is tzcnt, spelled for older assemblers.  Destination and */	\
    /* source must be a 64-bit registers, hence cast and %q.         */	\
    __asm__ ("rep;bsf\t%1, %q0" : "=r" (ਗਿਣਤੀ_2) : "rm" ((UDItype)(x)));	\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_TRAILING_ZEROS_0 64
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros(ਗਿਣਤੀ_2, x)					\
  ਕਰੋ {									\
    ASSERT ((x) != 0);							\
    __asm__ ("bsf\t%1, %q0" : "=r" (ਗਿਣਤੀ_2) : "rm" ((UDItype)(x)));	\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* __amd64__ */

#ਜੇ defined (__i860__) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ rshift_rhlc(r,h,l,c) \
  __asm__ ("shr %3,r0,r0\;shrd %1,%2,%0"				\
	   "=r" (r) : "r" (h), "r" (l), "rn" (c))
#ਜੇ_ਅੰਤ /* i860 */

#ਜੇ defined (__i960__) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("cmpo 1,0\;addc %5,%4,%1\;addc %3,%2,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "dI" (ah), "dI" (bh), "%dI" (al), "dI" (bl))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("cmpo 0,0\;subc %5,%4,%1\;subc %3,%2,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "dI" (ah), "dI" (bh), "dI" (al), "dI" (bl))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  ({ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __l, __h;} __i;				\
	  } __x;							\
  __asm__ ("emul %2,%1,%0"						\
	   : "=d" (__x.__ll) : "%dI" (u), "dI" (v));			\
  (w1) = __x.__i.__h; (w0) = __x.__i.__l;})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __umulsidi3(u, v) \
  ({UDItype __w;							\
    __asm__ ("emul %2,%1,%0" : "=d" (__w) : "%dI" (u), "dI" (v));	\
    __w; })
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, nh, nl, d) \
  ਕਰੋ {									\
    ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __l, __h;} __i;				\
	  } __nn;							\
    __nn.__i.__h = (nh); __nn.__i.__l = (nl);				\
    __asm__ ("ediv %d,%n,%0"						\
	   : "=d" (__rq.__ll) : "dI" (__nn.__ll), "dI" (d));		\
    (r) = __rq.__i.__l; (q) = __rq.__i.__h;				\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  ਕਰੋ {									\
    USItype __cbtmp;							\
    __asm__ ("scanbit %1,%0" : "=r" (__cbtmp) : "r" (x));		\
    (ਗਿਣਤੀ_2) = __cbtmp ^ 31;						\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_0 (-32) /* sic */
#ਜੇ defined (__i960mx)		/* what is the proper symbol to test??? */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ rshift_rhlc(r,h,l,c) \
  ਕਰੋ {									\
    ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __l, __h;} __i;				\
	  } __nn;							\
    __nn.__i.__h = (h); __nn.__i.__l = (l);				\
    __asm__ ("shre %2,%1,%0" : "=d" (r) : "dI" (__nn.__ll), "dI" (c));	\
  }
#ਜੇ_ਅੰਤ /* i960mx */
#ਜੇ_ਅੰਤ /* i960 */


#ਜੇ defined (__loongarch64) && W_TYPE_ਮਾਪ == 64
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  ਕਰੋ {									\
    UDItype __u = (u), __v = (v);					\
    (w0) = __u * __v;							\
    (w1) = __extension__ (ਬਿਨਾਨਿਸ਼ਾਨੀ __int128) __u * __v >> 64;		\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ


#ਜੇ (defined (__mc68000__) || defined (__mc68020__) || defined(mc68020) \
     || defined (__m68k__) || defined (__mc5200__) || defined (__mc5206e__) \
     || defined (__mc5307__)) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add%.l %5,%1\n\taddx%.l %3,%0"				\
	   : "=d" (sh), "=&d" (sl)					\
	   : "0"  ((USItype)(ah)), "d" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "g" ((USItype)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub%.l %5,%1\n\tsubx%.l %3,%0"				\
	   : "=d" (sh), "=&d" (sl)					\
	   : "0" ((USItype)(ah)), "d" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "g" ((USItype)(bl)))
/* The '020, '030, '040 and CPU32 have 32x32->64 and 64/32->32q-32r.  */
#ਜੇ defined (__mc68020__) || defined(mc68020) \
     || defined (__mc68030__) || defined (mc68030) \
     || defined (__mc68040__) || defined (mc68040) \
     || defined (__mcpu32__) || defined (mcpu32) \
     || defined (__NeXT__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("mulu%.l %3,%1:%0"						\
	   : "=d" (w0), "=d" (w1)					\
	   : "%0" ((USItype)(u)), "dmi" ((USItype)(v)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  __asm__ ("divu%.l %4,%1:%0"						\
	   : "=d" (q), "=d" (r)						\
	   : "0" ((USItype)(n0)), "1" ((USItype)(n1)), "dmi" ((USItype)(d)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sdiv_qrnnd(q, r, n1, n0, d) \
  __asm__ ("divs%.l %4,%1:%0"						\
	   : "=d" (q), "=d" (r)						\
	   : "0" ((USItype)(n0)), "1" ((USItype)(n1)), "dmi" ((USItype)(d)))
#ਨਹੀਂਤਾਂ /* for other 68k family members use 16x16->32 multiplication */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(xh, xl, a, b) \
  ਕਰੋ { USItype __umul_tmp1, __umul_tmp2;				\
	__asm__ ("| Inlined umul_ppmm\n"				\
"	move%.l	%5,%3\n"						\
"	move%.l	%2,%0\n"						\
"	move%.w	%3,%1\n"						\
"	swap	%3\n"							\
"	swap	%0\n"							\
"	mulu%.w	%2,%1\n"						\
"	mulu%.w	%3,%0\n"						\
"	mulu%.w	%2,%3\n"						\
"	swap	%2\n"							\
"	mulu%.w	%5,%2\n"						\
"	add%.l	%3,%2\n"						\
"	jcc	1f\n"							\
"	add%.l	%#0x10000,%0\n"						\
"1:	move%.l	%2,%3\n"						\
"	clr%.w	%2\n"							\
"	swap	%2\n"							\
"	swap	%3\n"							\
"	clr%.w	%3\n"							\
"	add%.l	%3,%1\n"						\
"	addx%.l	%2,%0\n"						\
"	| End inlined umul_ppmm"					\
	      : "=&d" (xh), "=&d" (xl),					\
		"=&d" (__umul_tmp1), "=&d" (__umul_tmp2)			\
	      : "%2" ((USItype)(a)), "d" ((USItype)(b)));		\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* not mc68020 */
/* The '020, '030, '040 and '060 have bitfield insns.
   GCC 3.4 defines __mc68020__ when in CPU32 mode, check for __mcpu32__ to
   exclude bfffo on that chip (bitfield insns not available).  */
#ਜੇ (defined (__mc68020__) || defined (mc68020)    \
     || defined (__mc68030__) || defined (mc68030) \
     || defined (__mc68040__) || defined (mc68040) \
     || defined (__mc68060__) || defined (mc68060) \
     || defined (__NeXT__))			   \
  && ! defined (__mcpu32__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  __asm__ ("bfffo %1{%b2:%b2},%0"					\
	   : "=d" (ਗਿਣਤੀ_2)						\
	   : "od" ((USItype) (x)), "n" (0))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_0 32
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* mc68000 */

#ਜੇ defined (__m88000__) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addu.co %1,%r4,%r5\n\taddu.ci %0,%r2,%r3"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rJ" (ah), "rJ" (bh), "%rJ" (al), "rJ" (bl))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subu.co %1,%r4,%r5\n\tsubu.ci %0,%r2,%r3"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rJ" (ah), "rJ" (bh), "rJ" (al), "rJ" (bl))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  ਕਰੋ {									\
    USItype __cbtmp;							\
    __asm__ ("ff1 %0,%1" : "=r" (__cbtmp) : "r" (x));			\
    (ਗਿਣਤੀ_2) = __cbtmp ^ 31;						\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_0 63 /* sic */
#ਜੇ defined (__m88110__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(wh, wl, u, v) \
  ਕਰੋ {									\
    ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("mulu.d %0,%1,%2" : "=r" (__x.__ll) : "r" (u), "r" (v));	\
    (wh) = __x.__i.__h;							\
    (wl) = __x.__i.__l;							\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  ({ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __h, __l;} __i;				\
	  } __x, __q;							\
  __x.__i.__h = (n1); __x.__i.__l = (n0);				\
  __asm__ ("divu.d %0,%1,%2"						\
	   : "=r" (__q.__ll) : "r" (__x.__ll), "r" (d));		\
  (r) = (n0) - __q.__l * (d); (q) = __q.__l; })
#ਜੇ_ਅੰਤ /* __m88110__ */
#ਜੇ_ਅੰਤ /* __m88000__ */

#ਜੇ defined (__mips) && W_TYPE_ਮਾਪ == 32
#ਜੇ __GMP_GNUC_PREREQ (4,4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  ਕਰੋ {									\
    UDItype __ll = (UDItype)(u) * (v);					\
    w1 = __ll >> 32;							\
    w0 = __ll;								\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ !defined (umul_ppmm) && __GMP_GNUC_PREREQ (2,7) && !defined (__clang__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("multu %2,%3" : "=l" (w0), "=h" (w1) : "d" (u), "d" (v))
#ਜੇ_ਅੰਤ
#ਜੇ !defined (umul_ppmm)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("multu %2,%3\n\tmflo %0\n\tmfhi %1"				\
	   : "=d" (w0), "=d" (w1) : "d" (u), "d" (v))
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* __mips */

#ਜੇ (defined (__mips) && __mips >= 3) && W_TYPE_ਮਾਪ == 64
#ਜੇ defined (_MIPS_ARCH_MIPS64R6)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  ਕਰੋ {									\
    UDItype __m0 = (u), __m1 = (v);					\
    (w0) = __m0 * __m1;							\
    __asm__ ("dmuhu\t%0, %1, %2" : "=d" (w1) : "d" (__m0), "d" (__m1));	\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ !defined (umul_ppmm) && __GMP_GNUC_PREREQ (4,4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  ਕਰੋ {									\
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __ll_UTItype __attribute__((ਬਹੁਲਕ_2(TI)));	\
    __ll_UTItype __ll = (__ll_UTItype)(u) * (v);			\
    w1 = __ll >> 64;							\
    w0 = __ll;								\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ !defined (umul_ppmm) && __GMP_GNUC_PREREQ (2,7) && !defined (__clang__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("dmultu %2,%3"						\
	   : "=l" (w0), "=h" (w1)					\
	   : "d" ((UDItype)(u)), "d" ((UDItype)(v)))
#ਜੇ_ਅੰਤ
#ਜੇ !defined (umul_ppmm)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("dmultu %2,%3\n\tmflo %0\n\tmfhi %1"				\
	   : "=d" (w0), "=d" (w1)					\
	   : "d" ((UDItype)(u)), "d" ((UDItype)(v)))
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* __mips */

#ਜੇ defined (__mmix__) && W_TYPE_ਮਾਪ == 64
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("MULU %0,%2,%3" : "=r" (w0), "=z" (w1) : "r" (u), "r" (v))
#ਜੇ_ਅੰਤ

#ਜੇ defined (__ns32000__) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  ({ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __l, __h;} __i;				\
	  } __x;							\
  __asm__ ("meid %2,%0"							\
	   : "=g" (__x.__ll)						\
	   : "%0" ((USItype)(u)), "g" ((USItype)(v)));			\
  (w1) = __x.__i.__h; (w0) = __x.__i.__l;})
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __umulsidi3(u, v) \
  ({UDItype __w;							\
    __asm__ ("meid %2,%0"						\
	     : "=g" (__w)						\
	     : "%0" ((USItype)(u)), "g" ((USItype)(v)));		\
    __w; })
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  ({ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __l, __h;} __i;				\
	  } __x;							\
  __x.__i.__h = (n1); __x.__i.__l = (n0);				\
  __asm__ ("deid %2,%0"							\
	   : "=g" (__x.__ll)						\
	   : "0" (__x.__ll), "g" ((USItype)(d)));			\
  (r) = __x.__i.__l; (q) = __x.__i.__h; })
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros(ਗਿਣਤੀ_2,x) \
  ਕਰੋ {									\
    __asm__ ("ffsd	%2,%0"						\
	     : "=r" (ਗਿਣਤੀ_2)						\
	     : "0" ((USItype) 0), "r" ((USItype) (x)));			\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* __ns32000__ */

/* In the past we had a block of various #defines tested
       _ARCH_PPC    - AIX
       _ARCH_PWR    - AIX
       __powerpc__  - gcc
       __POWERPC__  - BEOS
       __ppc__      - Darwin
       PPC          - old gcc, GNU/Linux, SysV
   The plain PPC test was not good for vxWorks, since PPC is defined on all
   CPUs there (eg. m68k too), as a constant one is expected to compare
   CPU_FAMILY against.

   At any rate, this was pretty unattractive and a bit fragile.  The use of
   HAVE_HOST_CPU_FAMILY is designed to cut through it all and be sure of
   getting the desired effect.

   ENHANCE-ME: We should test _IBMR2 here when we add assembly support for
   the system vendor compilers.  (Is that vendor compilers with inline asm,
   or what?)  */

#ਜੇ (HAVE_HOST_CPU_FAMILY_power || HAVE_HOST_CPU_FAMILY_powerpc)	\
  && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  ਕਰੋ {									\
    ਜੇ (__builtin_constant_p (bh) && (bh) == 0)				\
      __asm__ ("add%I4c %1,%3,%4\n\taddze %0,%2"			\
	       : "=r" (sh), "=&r" (sl) : "r" (ah), "%r" (al), "rI" (bl)	\
		 __CLOBBER_CC);						\
    ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (bh) && (bh) == ~(USItype) 0)		\
      __asm__ ("add%I4c %1,%3,%4\n\taddme %0,%2"			\
	       : "=r" (sh), "=&r" (sl) : "r" (ah), "%r" (al), "rI" (bl)	\
		 __CLOBBER_CC);						\
    ਨਹੀਂਤਾਂ								\
      __asm__ ("add%I5c %1,%4,%5\n\tadde %0,%2,%3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "r" (bh), "%r" (al), "rI" (bl)		\
		 __CLOBBER_CC);						\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  ਕਰੋ {									\
    ਜੇ (__builtin_constant_p (ah) && (ah) == 0)				\
      __asm__ ("subf%I3c %1,%4,%3\n\tsubfze %0,%2"			\
	       : "=r" (sh), "=&r" (sl) : "r" (bh), "rI" (al), "r" (bl)	\
		 __CLOBBER_CC);						\
    ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (ah) && (ah) == ~(USItype) 0)		\
      __asm__ ("subf%I3c %1,%4,%3\n\tsubfme %0,%2"			\
	       : "=r" (sh), "=&r" (sl) : "r" (bh), "rI" (al), "r" (bl)	\
		 __CLOBBER_CC);						\
    ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (bh) && (bh) == 0)			\
      __asm__ ("subf%I3c %1,%4,%3\n\taddme %0,%2"			\
	       : "=r" (sh), "=&r" (sl) : "r" (ah), "rI" (al), "r" (bl)	\
		 __CLOBBER_CC);						\
    ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (bh) && (bh) == ~(USItype) 0)		\
      __asm__ ("subf%I3c %1,%4,%3\n\taddze %0,%2"			\
	       : "=r" (sh), "=&r" (sl) : "r" (ah), "rI" (al), "r" (bl)	\
		 __CLOBBER_CC);						\
    ਨਹੀਂਤਾਂ								\
      __asm__ ("subf%I4c %1,%5,%4\n\tsubfe %0,%3,%2"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "r" (bh), "rI" (al), "r" (bl)		\
		 __CLOBBER_CC);						\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  __asm__ ("cntlzw %0,%1" : "=r" (ਗਿਣਤੀ_2) : "r" (x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_0 32
#ਜੇ HAVE_HOST_CPU_FAMILY_powerpc
#ਜੇ __GMP_GNUC_PREREQ (4,4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  ਕਰੋ {									\
    UDItype __ll = (UDItype)(u) * (v);					\
    w1 = __ll >> 32;							\
    w0 = __ll;								\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ !defined (umul_ppmm)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1) \
  ਕਰੋ {									\
    USItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("mulhwu %0,%1,%2" : "=r" (ph) : "%r" (m0), "r" (m1));	\
    (pl) = __m0 * __m1;							\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ smul_ppmm(ph, pl, m0, m1) \
  ਕਰੋ {									\
    SItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("mulhw %0,%1,%2" : "=r" (ph) : "%r" (m0), "r" (m1));	\
    (pl) = __m0 * __m1;							\
  } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ smul_ppmm(xh, xl, m0, m1) \
  __asm__ ("mul %0,%2,%3" : "=r" (xh), "=q" (xl) : "r" (m0), "r" (m1))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sdiv_qrnnd(q, r, nh, nl, d) \
  __asm__ ("div %0,%2,%4" : "=r" (q), "=q" (r) : "r" (nh), "1" (nl), "r" (d))
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* 32-bit POWER architecture variants.  */

/* We should test _IBMR2 here when we add assembly support for the system
   vendor compilers.  */
#ਜੇ HAVE_HOST_CPU_FAMILY_powerpc && W_TYPE_ਮਾਪ == 64
#ਜੇ !defined (_LONG_LONG_LIMB)
/* _LONG_LONG_LIMB is ABI=mode32 where adde operates on 32-bit values.  So
   use adde etc only when not _LONG_LONG_LIMB.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  ਕਰੋ {									\
    ਜੇ (__builtin_constant_p (bh) && (bh) == 0)				\
      __asm__ ("add%I4c %1,%3,%4\n\taddze %0,%2"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r"  ((UDItype)(ah)),					\
		 "%r" ((UDItype)(al)), "rI" ((UDItype)(bl))		\
		 __CLOBBER_CC);						\
    ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (bh) && (bh) == ~(UDItype) 0)		\
      __asm__ ("add%I4c %1,%3,%4\n\taddme %0,%2"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r"  ((UDItype)(ah)),					\
		 "%r" ((UDItype)(al)), "rI" ((UDItype)(bl))		\
		 __CLOBBER_CC);						\
    ਨਹੀਂਤਾਂ								\
      __asm__ ("add%I5c %1,%4,%5\n\tadde %0,%2,%3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r"  ((UDItype)(ah)), "r"  ((UDItype)(bh)),		\
		 "%r" ((UDItype)(al)), "rI" ((UDItype)(bl))		\
		 __CLOBBER_CC);						\
  } ਜਦੋਂ (0)
/* We use "*rI" for the constant operand here, since with just "I", gcc barfs.
   This might seem strange, but gcc folds away the dead code late.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  ਕਰੋ {									\
    ਜੇ (__builtin_constant_p (bl)					\
	&& (bl) > -0x8000 && (bl) <= 0x8000 && (bl) != 0) {		\
	ਜੇ (__builtin_constant_p (ah) && (ah) == 0)			\
	  __asm__ ("addic %1,%3,%4\n\tsubfze %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   :                       "r" ((UDItype)(bh)),		\
		     "r" ((UDItype)(al)), "*rI" (-((UDItype)(bl)))	\
		     __CLOBBER_CC);					\
	ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (ah) && (ah) == ~(UDItype) 0)	\
	  __asm__ ("addic %1,%3,%4\n\tsubfme %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   :                       "r" ((UDItype)(bh)),		\
		     "r" ((UDItype)(al)), "*rI" (-((UDItype)(bl)))	\
		     __CLOBBER_CC);					\
	ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (bh) && (bh) == 0)		\
	  __asm__ ("addic %1,%3,%4\n\taddme %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r" ((UDItype)(ah)),				\
		     "r" ((UDItype)(al)), "*rI" (-((UDItype)(bl)))	\
		     __CLOBBER_CC);					\
	ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (bh) && (bh) == ~(UDItype) 0)	\
	  __asm__ ("addic %1,%3,%4\n\taddze %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r" ((UDItype)(ah)),				\
		     "r" ((UDItype)(al)), "*rI" (-((UDItype)(bl)))	\
		     __CLOBBER_CC);					\
	ਨਹੀਂਤਾਂ								\
	  __asm__ ("addic %1,%4,%5\n\tsubfe %0,%3,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r" ((UDItype)(ah)), "r" ((UDItype)(bh)),		\
		     "r" ((UDItype)(al)), "*rI" (-((UDItype)(bl)))	\
		     __CLOBBER_CC);					\
    } ਨਹੀਂਤਾਂ {								\
	ਜੇ (__builtin_constant_p (ah) && (ah) == 0)			\
	  __asm__ ("subf%I3c %1,%4,%3\n\tsubfze %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   :                       "r" ((UDItype)(bh)),		\
		     "rI" ((UDItype)(al)), "r" ((UDItype)(bl))		\
		     __CLOBBER_CC);					\
	ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (ah) && (ah) == ~(UDItype) 0)	\
	  __asm__ ("subf%I3c %1,%4,%3\n\tsubfme %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   :                       "r" ((UDItype)(bh)),		\
		     "rI" ((UDItype)(al)), "r" ((UDItype)(bl))		\
		     __CLOBBER_CC);					\
	ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (bh) && (bh) == 0)		\
	  __asm__ ("subf%I3c %1,%4,%3\n\taddme %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r"  ((UDItype)(ah)),				\
		     "rI" ((UDItype)(al)), "r" ((UDItype)(bl))		\
		     __CLOBBER_CC);					\
	ਨਹੀਂਤਾਂ ਜੇ (__builtin_constant_p (bh) && (bh) == ~(UDItype) 0)	\
	  __asm__ ("subf%I3c %1,%4,%3\n\taddze %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r"  ((UDItype)(ah)),				\
		     "rI" ((UDItype)(al)), "r" ((UDItype)(bl))		\
		     __CLOBBER_CC);					\
	ਨਹੀਂਤਾਂ								\
	  __asm__ ("subf%I4c %1,%5,%4\n\tsubfe %0,%3,%2"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r"  ((UDItype)(ah)), "r" ((UDItype)(bh)),		\
		     "rI" ((UDItype)(al)), "r" ((UDItype)(bl))		\
		     __CLOBBER_CC);					\
    }									\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* ! _LONG_LONG_LIMB */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  __asm__ ("cntlzd %0,%1" : "=r" (ਗਿਣਤੀ_2) : "r" (x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_0 64
#ਜੇ __GMP_GNUC_PREREQ (4,8)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  ਕਰੋ {									\
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __ll_UTItype __attribute__((ਬਹੁਲਕ_2(TI)));	\
    __ll_UTItype __ll = (__ll_UTItype)(u) * (v);			\
    w1 = __ll >> 64;							\
    w0 = __ll;								\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ !defined (umul_ppmm)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1) \
  ਕਰੋ {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("mulhdu %0,%1,%2" : "=r" (ph) : "%r" (__m0), "r" (__m1));	\
    (pl) = __m0 * __m1;							\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ smul_ppmm(ph, pl, m0, m1) \
  ਕਰੋ {									\
    DItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("mulhd %0,%1,%2" : "=r" (ph) : "%r" (__m0), "r" (__m1));	\
    (pl) = __m0 * __m1;							\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* 64-bit PowerPC.  */

#ਜੇ defined (__pyr__) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addw %5,%1\n\taddwc %3,%0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0"  ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "g" ((USItype)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subw %5,%1\n\tsubwb %3,%0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0" ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "g" ((USItype)(bl)))
/* This insn works on Pyramids with AP, XP, or MI CPUs, but not with SP.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  ({ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __h, __l;} __i;				\
	  } __x;							\
  __asm__ ("movw %1,%R0\n\tuemul %2,%0"					\
	   : "=&r" (__x.__ll)						\
	   : "g" ((USItype) (u)), "g" ((USItype)(v)));			\
  (w1) = __x.__i.__h; (w0) = __x.__i.__l;})
#ਜੇ_ਅੰਤ /* __pyr__ */

#ਜੇ defined (__ibm032__) /* RT/ROMP */  && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("a %1,%5\n\tae %0,%3"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0"  ((USItype)(ah)), "r" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "r" ((USItype)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("s %1,%5\n\tse %0,%3"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0" ((USItype)(ah)), "r" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "r" ((USItype)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ smul_ppmm(ph, pl, m0, m1) \
  __asm__ (								\
       "s	r2,r2\n"						\
"	mts r10,%2\n"							\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	m	r2,%3\n"						\
"	cas	%0,r2,r0\n"						\
"	mfs	r10,%1"							\
	   : "=r" (ph), "=r" (pl)					\
	   : "%r" ((USItype)(m0)), "r" ((USItype)(m1))			\
	   : "r2")
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  ਕਰੋ {									\
    ਜੇ ((x) >= 0x10000)							\
      __asm__ ("clz	%0,%1"						\
	       : "=r" (ਗਿਣਤੀ_2) : "r" ((USItype)(x) >> 16));		\
    ਨਹੀਂਤਾਂ								\
      {									\
	__asm__ ("clz	%0,%1"						\
		 : "=r" (ਗਿਣਤੀ_2) : "r" ((USItype)(x)));			\
	(ਗਿਣਤੀ_2) += 16;							\
      }									\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* RT/ROMP */

#ਜੇ defined (__riscv) && defined (__riscv_mul) && W_TYPE_ਮਾਪ == 64
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, u, v) \
  ਕਰੋ {									\
    UDItype __u = (u), __v = (v);					\
    (pl) = __u * __v;							\
    __asm__ ("mulhu\t%0, %1, %2" : "=r" (ph) : "%r" (__u), "r" (__v));	\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਜੇ (defined (__SH2__) || defined (__SH3__) || defined (__SH4__)) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("dmulu.l %2,%3\n\tsts macl,%1\n\tsts mach,%0"		\
	   : "=r" (w1), "=r" (w0) : "r" (u), "r" (v) : "macl", "mach")
#ਜੇ_ਅੰਤ

#ਜੇ defined (__sparc__) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addcc %r4,%5,%1\n\taddx %r2,%3,%0"				\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rJ" (ah), "rI" (bh),"%rJ" (al), "rI" (bl)			\
	   __CLOBBER_CC)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subcc %r4,%5,%1\n\tsubx %r2,%3,%0"				\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rJ" (ah), "rI" (bh), "rJ" (al), "rI" (bl)	\
	   __CLOBBER_CC)
/* FIXME: When gcc -mcpu=v9 is used on solaris, gcc/config/sol2-sld-64.h
   doesn't define anything to indicate that to us, it only sets __sparcv8. */
#ਜੇ defined (__sparc_v9__) || defined (__sparcv9)
/* Perhaps we should use floating-point operations here?  */
#ਜੇ 0
/* Triggers a bug making mpz/tests/t-gcd.c fail.
   Perhaps we simply need explicitly zero-extend the inputs?  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("mulx %2,%3,%%g1; srl %%g1,0,%1; srlx %%g1,32,%0" :		\
	   "=r" (w1), "=r" (w0) : "r" (u), "r" (v) : "g1")
#ਨਹੀਂਤਾਂ
/* Use v8 umul until above bug is fixed.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("umul %2,%3,%1;rd %%y,%0" : "=r" (w1), "=r" (w0) : "r" (u), "r" (v))
#ਜੇ_ਅੰਤ
/* Use a plain v8 divide for v9.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  ਕਰੋ {									\
    USItype __q;							\
    __asm__ ("mov %1,%%y;nop;nop;nop;udiv %2,%3,%0"			\
	     : "=r" (__q) : "r" (n1), "r" (n0), "r" (d));		\
    (r) = (n0) - __q * (d);						\
    (q) = __q;								\
  } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
#ਜੇ defined (__sparc_v8__)   /* gcc normal */				\
  || defined (__sparcv8)     /* gcc solaris */				\
  || HAVE_HOST_CPU_supersparc
/* Don't match immediate range because, 1) it is not often useful,
   2) the 'I' flag thinks of the range as a 13 bit signed interval,
   while we want to match a 13 bit interval, sign extended to 32 bits,
   but INTERPRETED AS UNSIGNED.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("umul %2,%3,%1;rd %%y,%0" : "=r" (w1), "=r" (w0) : "r" (u), "r" (v))

#ਜੇ HAVE_HOST_CPU_supersparc
#ਨਹੀਂਤਾਂ
/* Don't use this on SuperSPARC because its udiv only handles 53 bit
   dividends and will trap to the kernel for the rest. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  ਕਰੋ {									\
    USItype __q;							\
    __asm__ ("mov %1,%%y;nop;nop;nop;udiv %2,%3,%0"			\
	     : "=r" (__q) : "r" (n1), "r" (n0), "r" (d));		\
    (r) = (n0) - __q * (d);						\
    (q) = __q;								\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* HAVE_HOST_CPU_supersparc */

#ਨਹੀਂਤਾਂ /* ! __sparc_v8__ */
#ਜੇ defined (__sparclite__)
/* This has hardware multiply but not divide.  It also has two additional
   instructions scan (ffs from high bit) and divscc.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("umul %2,%3,%1;rd %%y,%0" : "=r" (w1), "=r" (w0) : "r" (u), "r" (v))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  __asm__ ("! Inlined udiv_qrnnd\n"					\
"	wr	%%g0,%2,%%y	! Not a delayed write for sparclite\n"	\
"	tst	%%g0\n"							\
"	divscc	%3,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%%g1\n"						\
"	divscc	%%g1,%4,%0\n"						\
"	rd	%%y,%1\n"						\
"	bl,a 1f\n"							\
"	add	%1,%4,%1\n"						\
"1:	! End of inline udiv_qrnnd"					\
	   : "=r" (q), "=r" (r) : "r" (n1), "r" (n0), "rI" (d)		\
	   : "%g1" __AND_CLOBBER_CC)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  __asm__ ("scan %1,1,%0" : "=r" (ਗਿਣਤੀ_2) : "r" (x))
/* Early sparclites return 63 for an argument of 0, but they warn that future
   implementations might change this.  Therefore, leave COUNT_LEADING_ZEROS_0
   undefined.  */
#ਜੇ_ਅੰਤ /* __sparclite__ */
#ਜੇ_ਅੰਤ /* __sparc_v8__ */
#ਜੇ_ਅੰਤ /* __sparc_v9__ */
/* Default to sparc v7 versions of umul_ppmm and udiv_qrnnd.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ umul_ppmm
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v) \
  __asm__ ("! Inlined umul_ppmm\n"					\
"	wr	%%g0,%2,%%y	! SPARC has 0-3 delay insn after a wr\n" \
"	sra	%3,31,%%g2	! Don't move this insn\n"		\
"	and	%2,%%g2,%%g2	! Don't move this insn\n"		\
"	andcc	%%g0,0,%%g1	! Don't move this insn\n"		\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,%3,%%g1\n"						\
"	mulscc	%%g1,0,%%g1\n"						\
"	add	%%g1,%%g2,%0\n"						\
"	rd	%%y,%1"							\
	   : "=r" (w1), "=r" (w0) : "%rI" (u), "r" (v)			\
	   : "%g1", "%g2" __AND_CLOBBER_CC)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ udiv_qrnnd
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LONGLONG_STANDALONE
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d) \
  ਕਰੋ { UWtype __r;							\
    (q) = __MPN(udiv_qrnnd) (&__r, (n1), (n0), (d));			\
    (r) = __r;								\
  } ਜਦੋਂ (0)
ਬਾਹਰੀ UWtype __MPN(udiv_qrnnd) (UWtype *, UWtype, UWtype, UWtype);
#ਜੇ_ਅੰਤ /* LONGLONG_STANDALONE */
#ਜੇ_ਅੰਤ /* udiv_qrnnd */
#ਜੇ_ਅੰਤ /* __sparc__ */

#ਜੇ defined (__sparc__) && W_TYPE_ਮਾਪ == 64
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ (								\
       "addcc	%r4,%5,%1\n"						\
      "	addccc	%r6,%7,%%g0\n"						\
      "	addc	%r2,%3,%0"						\
       : "=r" (sh), "=&r" (sl)						\
       : "rJ"  ((UDItype)(ah)), "rI" ((UDItype)(bh)),			\
	 "%rJ" ((UDItype)(al)), "rI" ((UDItype)(bl)),			\
	 "%rJ" ((UDItype)(al) >> 32), "rI" ((UDItype)(bl) >> 32)	\
	   __CLOBBER_CC)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ (								\
       "subcc	%r4,%5,%1\n"						\
      "	subccc	%r6,%7,%%g0\n"						\
      "	subc	%r2,%3,%0"						\
       : "=r" (sh), "=&r" (sl)						\
       : "rJ" ((UDItype)(ah)), "rI" ((UDItype)(bh)),			\
	 "rJ" ((UDItype)(al)), "rI" ((UDItype)(bl)),			\
	 "rJ" ((UDItype)(al) >> 32), "rI" ((UDItype)(bl) >> 32)		\
	   __CLOBBER_CC)
#ਜੇ __VIS__ >= 0x300
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਜੋੜੋ_ssaaaa
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ (								\
       "addcc	%r4, %5, %1\n"						\
      "	addxc	%r2, %r3, %0"						\
	  : "=r" (sh), "=&r" (sl)					\
       : "rJ"  ((UDItype)(ah)), "rJ" ((UDItype)(bh)),			\
	 "%rJ" ((UDItype)(al)), "rI" ((UDItype)(bl)) __CLOBBER_CC)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1) \
  ਕਰੋ {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    (pl) = __m0 * __m1;							\
    __asm__ ("umulxhi\t%2, %1, %0"					\
	     : "=r" (ph)						\
	     : "%r" (__m0), "r" (__m1));				\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  __asm__ ("lzd\t%1,%0" : "=r" (ਗਿਣਤੀ_2) : "r" (x))
/* Needed by count_leading_zeros_32 in sparc64.h.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_NEED_CLZ_TAB
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ (defined (__vax) || defined (__vax__)) && W_TYPE_ਮਾਪ == 32
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addl2 %5,%1\n\tadwc %3,%0"					\
	   : "=g" (sh), "=&g" (sl)					\
	   : "0"  ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "g" ((USItype)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subl2 %5,%1\n\tsbwc %3,%0"					\
	   : "=g" (sh), "=&g" (sl)					\
	   : "0" ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "g" ((USItype)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ smul_ppmm(xh, xl, m0, m1) \
  ਕਰੋ {									\
    ਸੰਘ {UDItype __ll;						\
	   ਬਣਤਰ {USItype __l, __h;} __i;				\
	  } __x;							\
    USItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("emul %1,%2,$0,%0"						\
	     : "=g" (__x.__ll) : "g" (__m0), "g" (__m1));		\
    (xh) = __x.__i.__h; (xl) = __x.__i.__l;				\
  } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sdiv_qrnnd(q, r, n1, n0, d) \
  ਕਰੋ {									\
    ਸੰਘ {DItype __ll;							\
	   ਬਣਤਰ {SItype __l, __h;} __i;				\
	  } __x;							\
    __x.__i.__h = n1; __x.__i.__l = n0;					\
    __asm__ ("ediv %3,%2,%0,%1"						\
	     : "=g" (q), "=g" (r) : "g" (__x.__ll), "g" (d));		\
  } ਜਦੋਂ (0)
#ਜੇ 0
/* FIXME: This instruction appears to be unimplemented on some systems (vax
   8800 maybe). */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros(ਗਿਣਤੀ_2,x)					\
  ਕਰੋ {									\
    __asm__ ("ffs 0, 31, %1, %0"					\
	     : "=g" (ਗਿਣਤੀ_2)						\
	     : "g" ((USItype) (x)));					\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ /* vax */

#ਜੇ defined (__z8000__) && W_TYPE_ਮਾਪ == 16
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add	%H1,%H5\n\tadc	%H0,%H3"				\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0"  ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(ah)), "r" ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(bh)),	\
	     "%1" ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(al)), "rQR" ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub	%H1,%H5\n\tsbc	%H0,%H3"				\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0" ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(ah)), "r" ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(bh)),	\
	     "1" ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(al)), "rQR" ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)(bl)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(xh, xl, m0, m1) \
  ਕਰੋ {									\
    ਸੰਘ {ਲੰਮਾ ਪੂਰਨ __ll;						\
	   ਬਣਤਰ {ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __h, __l;} __i;				\
	  } __x;							\
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ __m0 = (m0), __m1 = (m1);				\
    __asm__ ("mult	%S0,%H3"					\
	     : "=r" (__x.__i.__h), "=r" (__x.__i.__l)			\
	     : "%1" (m0), "rQR" (m1));					\
    (xh) = __x.__i.__h; (xl) = __x.__i.__l;				\
    (xh) += ((((ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ) __m0 >> 15) & __m1)				\
	     + (((ਨਿਸ਼ਾਨੀਵਾਲਾ ਪੂਰਨ) __m1 >> 15) & __m0));			\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ /* __z8000__ */

#ਜੇ_ਅੰਤ /* __GNUC__ */

#ਜੇ_ਅੰਤ /* NO_ASM */


/* FIXME: "sidi" here is highly doubtful, should sometimes be "diti".  */
#ਜੇ !defined (umul_ppmm) && defined (__umulsidi3)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(ph, pl, m0, m1) \
  ਕਰੋ {									\
    UDWtype __ll = __umulsidi3 (m0, m1);				\
    ph = (UWtype) (__ll >> W_TYPE_ਮਾਪ);				\
    pl = (UWtype) __ll;							\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਜੇ !defined (__umulsidi3)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __umulsidi3(u, v) \
  ({UWtype __hi, __lo;							\
    umul_ppmm (__hi, __lo, u, v);					\
    ((UDWtype) __hi << W_TYPE_ਮਾਪ) | __lo; })
#ਜੇ_ਅੰਤ


#ਜੇ defined (__cplusplus)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __longlong_h_C "C"
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __longlong_h_C
#ਜੇ_ਅੰਤ

/* Use mpn_umul_ppmm or mpn_udiv_qrnnd functions, if they exist.  The "_r"
   forms have "reversed" arguments, meaning the pointer is last, which
   sometimes allows better parameter passing, in particular on 64-bit
   hppa. */

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ mpn_umul_ppmm  __MPN(umul_ppmm)
ਬਾਹਰੀ __longlong_h_C UWtype mpn_umul_ppmm (UWtype *, UWtype, UWtype);

#ਜੇ ! defined (umul_ppmm) && HAVE_NATIVE_mpn_umul_ppmm  \
  && ! defined (LONGLONG_STANDALONE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(wh, wl, u, v)						\
  ਕਰੋ {									\
    UWtype __umul_ppmm__p0;						\
    (wh) = mpn_umul_ppmm (&__umul_ppmm__p0, (UWtype) (u), (UWtype) (v));\
    (wl) = __umul_ppmm__p0;						\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ mpn_umul_ppmm_r  __MPN(umul_ppmm_r)
ਬਾਹਰੀ __longlong_h_C UWtype mpn_umul_ppmm_r (UWtype, UWtype, UWtype *);

#ਜੇ ! defined (umul_ppmm) && HAVE_NATIVE_mpn_umul_ppmm_r	\
  && ! defined (LONGLONG_STANDALONE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(wh, wl, u, v)						\
  ਕਰੋ {									\
    UWtype __umul_p0;							\
    (wh) = mpn_umul_ppmm_r ((UWtype) (u), (UWtype) (v), &__umul_p0);	\
    (wl) = __umul_p0;							\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ mpn_udiv_qrnnd  __MPN(udiv_qrnnd)
ਬਾਹਰੀ __longlong_h_C UWtype mpn_udiv_qrnnd (UWtype *, UWtype, UWtype, UWtype);

#ਜੇ ! defined (udiv_qrnnd) && HAVE_NATIVE_mpn_udiv_qrnnd	\
  && ! defined (LONGLONG_STANDALONE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d)					\
  ਕਰੋ {									\
    UWtype __udiv_qrnnd_r;						\
    (q) = mpn_udiv_qrnnd (&__udiv_qrnnd_r,				\
			  (UWtype) (n1), (UWtype) (n0), (UWtype) d);	\
    (r) = __udiv_qrnnd_r;						\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ mpn_udiv_qrnnd_r  __MPN(udiv_qrnnd_r)
ਬਾਹਰੀ __longlong_h_C UWtype mpn_udiv_qrnnd_r (UWtype, UWtype, UWtype, UWtype *);

#ਜੇ ! defined (udiv_qrnnd) && HAVE_NATIVE_mpn_udiv_qrnnd_r	\
  && ! defined (LONGLONG_STANDALONE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, n1, n0, d)					\
  ਕਰੋ {									\
    UWtype __udiv_qrnnd_r;						\
    (q) = mpn_udiv_qrnnd_r ((UWtype) (n1), (UWtype) (n0), (UWtype) d,	\
			    &__udiv_qrnnd_r);				\
    (r) = __udiv_qrnnd_r;						\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ


/* If this machine has no inline assembler, use C macros.  */

#ਜੇ !defined (ਜੋੜੋ_ssaaaa)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਜੋੜੋ_ssaaaa(sh, sl, ah, al, bh, bl) \
  ਕਰੋ {									\
    UWtype __x;								\
    UWtype __al = (al);							\
    UWtype __bl = (bl);							\
    __x = __al + __bl;							\
    (sh) = (ah) + (bh) + (__x < __al);					\
    (sl) = __x;								\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

#ਜੇ !defined (sub_ddmmss)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ sub_ddmmss(sh, sl, ah, al, bh, bl) \
  ਕਰੋ {									\
    UWtype __x;								\
    UWtype __al = (al);							\
    UWtype __bl = (bl);							\
    __x = __al - __bl;							\
    (sh) = (ah) - (bh) - (__al < __bl);					\
    (sl) = __x;								\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

/* If we lack umul_ppmm but have smul_ppmm, define umul_ppmm in terms of
   smul_ppmm.  */
#ਜੇ !defined (umul_ppmm) && defined (smul_ppmm)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v)						\
  ਕਰੋ {									\
    UWtype __w1;							\
    UWtype __xm0 = (u), __xm1 = (v);					\
    smul_ppmm (__w1, w0, __xm0, __xm1);					\
    (w1) = __w1 + (-(__xm0 >> (W_TYPE_ਮਾਪ - 1)) & __xm1)		\
		+ (-(__xm1 >> (W_TYPE_ਮਾਪ - 1)) & __xm0);		\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

/* If we still don't have umul_ppmm, define it using plain C.

   For reference, when this code is used for squaring (ie. u and v identical
   expressions), gcc recognises __x1 and __x2 are the same and generates 3
   multiplies, not 4.  The subsequent additions could be optimized a bit,
   but the only place GMP currently uses such a square is mpn_sqr_basecase,
   and chips obliged to use this generic C umul will have plenty of worse
   performance problems than a couple of extra instructions on the diagonal
   of sqr_basecase.  */

#ਜੇ !defined (umul_ppmm)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ umul_ppmm(w1, w0, u, v)						\
  ਕਰੋ {									\
    UWtype __x0, __x1, __x2, __x3;					\
    UHWtype __ul, __vl, __uh, __vh;					\
    UWtype __u = (u), __v = (v);					\
									\
    __ul = __ll_lowpart (__u);						\
    __uh = __ll_highpart (__u);						\
    __vl = __ll_lowpart (__v);						\
    __vh = __ll_highpart (__v);						\
									\
    __x0 = (UWtype) __ul * __vl;					\
    __x1 = (UWtype) __ul * __vh;					\
    __x2 = (UWtype) __uh * __vl;					\
    __x3 = (UWtype) __uh * __vh;					\
									\
    __x1 += __ll_highpart (__x0);/* this can't give carry */		\
    __x1 += __x2;		/* but this indeed can */		\
    ਜੇ (__x1 < __x2)		/* did we get it? */			\
      __x3 += __ll_B;		/* yes, add it in the proper pos. */	\
									\
    (w1) = __x3 + __ll_highpart (__x1);					\
    (w0) = (__x1 << W_TYPE_ਮਾਪ/2) + __ll_lowpart (__x0);		\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

/* If we don't have smul_ppmm, define it using umul_ppmm (which surely will
   exist in one form or another.  */
#ਜੇ !defined (smul_ppmm)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ smul_ppmm(w1, w0, u, v)						\
  ਕਰੋ {									\
    UWtype __w1;							\
    UWtype __xm0 = (u), __xm1 = (v);					\
    umul_ppmm (__w1, w0, __xm0, __xm1);					\
    (w1) = __w1 - (-(__xm0 >> (W_TYPE_ਮਾਪ - 1)) & __xm1)		\
		- (-(__xm1 >> (W_TYPE_ਮਾਪ - 1)) & __xm0);		\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ

/* Define this unconditionally, so it can be used for debugging.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ __udiv_qrnnd_c(q, r, n1, n0, d) \
  ਕਰੋ {									\
    UWtype __d1, __d0, __q1, __q0, __r1, __r0, __m;			\
									\
    ASSERT ((d) != 0);							\
    ASSERT ((n1) < (d));						\
									\
    __d1 = __ll_highpart (d);						\
    __d0 = __ll_lowpart (d);						\
									\
    __q1 = (n1) / __d1;							\
    __r1 = (n1) - __q1 * __d1;						\
    __m = __q1 * __d0;							\
    __r1 = __r1 * __ll_B | __ll_highpart (n0);				\
    ਜੇ (__r1 < __m)							\
      {									\
	__q1--, __r1 += (d);						\
	ਜੇ (__r1 >= (d)) /* i.e. we didn't get carry when adding to __r1 */\
	  ਜੇ (__r1 < __m)						\
	    __q1--, __r1 += (d);					\
      }									\
    __r1 -= __m;							\
									\
    __q0 = __r1 / __d1;							\
    __r0 = __r1  - __q0 * __d1;						\
    __m = __q0 * __d0;							\
    __r0 = __r0 * __ll_B | __ll_lowpart (n0);				\
    ਜੇ (__r0 < __m)							\
      {									\
	__q0--, __r0 += (d);						\
	ਜੇ (__r0 >= (d))						\
	  ਜੇ (__r0 < __m)						\
	    __q0--, __r0 += (d);					\
      }									\
    __r0 -= __m;							\
									\
    (q) = __q1 * __ll_B | __q0;						\
    (r) = __r0;								\
  } ਜਦੋਂ (0)

/* If the processor has no udiv_qrnnd but sdiv_qrnnd, go through
   __udiv_w_sdiv (defined in libgcc or elsewhere).  */
#ਜੇ !defined (udiv_qrnnd) && defined (sdiv_qrnnd) \
  && ! defined (LONGLONG_STANDALONE)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd(q, r, nh, nl, d) \
  ਕਰੋ {									\
    UWtype __r;								\
    (q) = __MPN(udiv_w_sdiv) (&__r, nh, nl, d);				\
    (r) = __r;								\
  } ਜਦੋਂ (0)
__GMP_DECLSPEC UWtype __MPN(udiv_w_sdiv) (UWtype *, UWtype, UWtype, UWtype);
#ਜੇ_ਅੰਤ

/* If udiv_qrnnd was not defined for this processor, use __udiv_qrnnd_c.  */
#ਜੇ !defined (udiv_qrnnd)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UDIV_NEEDS_NORMALIZATION 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ udiv_qrnnd __udiv_qrnnd_c
#ਜੇ_ਅੰਤ

#ਜੇ !defined (ਗਿਣਤੀ_leading_zeros)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_leading_zeros(ਗਿਣਤੀ_2, x) \
  ਕਰੋ {									\
    UWtype __xr = (x);							\
    UWtype __a;								\
									\
    ਜੇ (W_TYPE_ਮਾਪ == 32)						\
      {									\
	__a = __xr < ((UWtype) 1 << 2*__BITS4)				\
	  ? (__xr < ((UWtype) 1 << __BITS4) ? 1 : __BITS4 + 1)		\
	  : (__xr < ((UWtype) 1 << 3*__BITS4) ? 2*__BITS4 + 1		\
	  : 3*__BITS4 + 1);						\
      }									\
    ਨਹੀਂਤਾਂ								\
      {									\
	ਲਈ (__a = W_TYPE_ਮਾਪ - 8; __a > 0; __a -= 8)			\
	  ਜੇ (((__xr >> __a) & 0xff) != 0)				\
	    ਰੋਕੋ;							\
	++__a;								\
      }									\
									\
    (ਗਿਣਤੀ_2) = W_TYPE_ਮਾਪ + 1 - __a - __clz_tab[__xr >> __a];		\
  } ਜਦੋਂ (0)
/* This version gives a well-defined value for zero. */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_0 (W_TYPE_ਮਾਪ - 1)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_NEED_CLZ_TAB
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_SLOW
#ਜੇ_ਅੰਤ

/* clz_tab needed by mpn/x86/pentium/mod_1.asm in a fat binary */
#ਜੇ HAVE_HOST_CPU_FAMILY_x86 && WANT_FAT_ਦੁਆਧਾਰੀ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_LEADING_ZEROS_NEED_CLZ_TAB
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਗਿਣਤੀ_LEADING_ZEROS_NEED_CLZ_TAB
ਬਾਹਰੀ ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ __GMP_DECLSPEC __clz_tab[129];
#ਜੇ_ਅੰਤ

#ਜੇ !defined (ਗਿਣਤੀ_trailing_zeros)
#ਜੇ !defined (ਗਿਣਤੀ_LEADING_ZEROS_SLOW)
/* Define count_trailing_zeros using an asm count_leading_zeros.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros(ਗਿਣਤੀ_2, x)					\
  ਕਰੋ {									\
    UWtype __ctz_x = (x);						\
    UWtype __ctz_c;							\
    ASSERT (__ctz_x != 0);						\
    ਗਿਣਤੀ_leading_zeros (__ctz_c, __ctz_x & -__ctz_x);			\
    (ਗਿਣਤੀ_2) = W_TYPE_ਮਾਪ - 1 - __ctz_c;				\
  } ਜਦੋਂ (0)
#ਨਹੀਂਤਾਂ
/* Define count_trailing_zeros in plain C, assuming small counts are common.
   We use clz_tab without ado, since the C count_leading_zeros above will have
   pulled it in.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਗਿਣਤੀ_trailing_zeros(ਗਿਣਤੀ_2, x)					\
  ਕਰੋ {									\
    UWtype __ctz_x = (x);						\
    ਪੂਰਨ __ctz_c;							\
									\
    ਜੇ (LIKELY ((__ctz_x & 0xff) != 0))					\
      (ਗਿਣਤੀ_2) = __clz_tab[__ctz_x & -__ctz_x] - 2;			\
    ਨਹੀਂਤਾਂ								\
      {									\
	ਲਈ (__ctz_c = 8 - 2; __ctz_c < W_TYPE_ਮਾਪ - 2; __ctz_c += 8)	\
	  {								\
	    __ctz_x >>= 8;						\
	    ਜੇ (LIKELY ((__ctz_x & 0xff) != 0))				\
	      ਰੋਕੋ;							\
	  }								\
									\
	(ਗਿਣਤੀ_2) = __ctz_c + __clz_tab[__ctz_x & -__ctz_x];		\
      }									\
  } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ UDIV_NEEDS_NORMALIZATION
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UDIV_NEEDS_NORMALIZATION 0
#ਜੇ_ਅੰਤ

/* Whether udiv_qrnnd is actually implemented with udiv_qrnnd_preinv, and
   that hence the latter should always be used.  */
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ UDIV_PREINV_ALWAYS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UDIV_PREINV_ALWAYS 0
#ਜੇ_ਅੰਤ
