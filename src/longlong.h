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

#تعریف_کرو __BITS4 (W_TYPE_سائز / 4)
#تعریف_کرو __ll_B ((UWtype) 1 << (W_TYPE_سائز / 2))
#تعریف_کرو __ll_lowpart(t) ((UWtype) (t) & (__ll_B - 1))
#تعریف_کرو __ll_highpart(t) ((UWtype) (t) >> (W_TYPE_سائز / 2))

/* This is used to make sure no undesirable sharing between different libraries
   that use this file takes place.  */
#اگر_غیر_تعریف_شدہ __MPN
#تعریف_کرو __MPN(x) __##x
#اگر_ختم

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

#اگر_تعریف_شدہ _LONG_LONG_LIMB
#تعریف_کرو گنتی_leading_zeros_gcc_clz(گنتی,x)	\
  کرو {						\
    ASSERT ((x) != 0);				\
    (گنتی) = __builtin_clzll (x);		\
  } جب (0)
#ورنہ
#تعریف_کرو گنتی_leading_zeros_gcc_clz(گنتی,x)	\
  کرو {						\
    ASSERT ((x) != 0);				\
    (گنتی) = __builtin_clzl (x);		\
  } جب (0)
#اگر_ختم

#اگر_تعریف_شدہ _LONG_LONG_LIMB
#تعریف_کرو گنتی_trailing_zeros_gcc_ctz(گنتی,x)	\
  کرو {						\
    ASSERT ((x) != 0);				\
    (گنتی) = __builtin_ctzll (x);		\
  } جب (0)
#ورنہ
#تعریف_کرو گنتی_trailing_zeros_gcc_ctz(گنتی,x)	\
  کرو {						\
    ASSERT ((x) != 0);				\
    (گنتی) = __builtin_ctzl (x);		\
  } جب (0)
#اگر_ختم


/* FIXME: The macros using external routines like __MPN(count_leading_zeros)
   don't need to be under !NO_ASM */
#اگر ! defined (NO_ASM)

#اگر defined (__alpha) && W_TYPE_سائز == 64
/* Most alpha-based machines, except Cray systems. */
#اگر defined (__GNUC__)
#اگر __GMP_GNUC_PREREQ (3,3)
#تعریف_کرو umul_ppmm(ph, pl, m0, m1) \
  کرو {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    (ph) = __builtin_alpha_umulh (__m0, __m1);				\
    (pl) = __m0 * __m1;							\
  } جب (0)
#ورنہ
#تعریف_کرو umul_ppmm(ph, pl, m0, m1) \
  کرو {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("umulh %r1,%2,%0"						\
	     : "=r" (ph)						\
	     : "%rJ" (__m0), "rI" (__m1));				\
    (pl) = __m0 * __m1;							\
  } جب (0)
#اگر_ختم
#ورنہ /* ! __GNUC__ */
#شامل <machine/builtins.h>
#تعریف_کرو umul_ppmm(ph, pl, m0, m1) \
  کرو {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    (ph) = __UMULH (__m0, __m1);					\
    (pl) = __m0 * __m1;							\
  } جب (0)
#اگر_ختم
#اگر_غیر_تعریف_شدہ LONGLONG_STANDALONE
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
  کرو { UWtype __di;							\
    __di = __MPN(invert_limb) (d);					\
    udiv_qrnnd_preinv (q, r, n1, n0, d, __di);				\
  } جب (0)
#تعریف_کرو UDIV_PREINV_ALWAYS  1
#تعریف_کرو UDIV_NEEDS_NORMALIZATION 1
#اگر_ختم /* LONGLONG_STANDALONE */

/* clz_tab is required in all configurations, since mpn/alpha/cntlz.asm
   always goes into libgmp.so, even when not actually used.  */
#تعریف_کرو گنتی_LEADING_ZEROS_NEED_CLZ_TAB

#اگر defined (__GNUC__) && HAVE_HOST_CPU_alpha_CIX
#تعریف_کرو گنتی_leading_zeros(گنتی_2,X) \
  __asm__("ctlz %1,%0" : "=r"(گنتی_2) : "r"(X))
#تعریف_کرو گنتی_trailing_zeros(گنتی_2,X) \
  __asm__("cttz %1,%0" : "=r"(گنتی_2) : "r"(X))
#اگر_ختم /* clz/ctz using cix */

#اگر ! defined (گنتی_leading_zeros)				\
  && defined (__GNUC__) && ! defined (LONGLONG_STANDALONE)
/* ALPHA_CMPBGE_0 gives "cmpbge $31,src,dst", ie. test src bytes == 0.
   "$31" is written explicitly in the asm, since an "r" constraint won't
   select reg 31.  There seems no need to worry about "r31" syntax for cray,
   since gcc itself (pre-release 3.4) emits just $31 in various places.	 */
#تعریف_کرو ALPHA_CMPBGE_0(dst, src)					\
  کرو { asm ("cmpbge $31, %1, %0" : "=r" (dst) : "r" (src)); } جب (0)
/* Zero bytes are turned into bits with cmpbge, a __clz_tab lookup counts
   them, locating the highest non-zero byte.  A second __clz_tab lookup
   counts the leading zero bits in that byte, giving the result.  */
#تعریف_کرو گنتی_leading_zeros(گنتی, x)					\
  کرو {									\
    UWtype  __clz__b, __clz__c, __clz__x = (x);				\
    ALPHA_CMPBGE_0 (__clz__b,  __clz__x);	    /* zero bytes */	\
    __clz__b = __clz_tab [(__clz__b >> 1) ^ 0x7F];  /* 8 to 1 byte */	\
    __clz__b = __clz__b * 8 - 7;		    /* 57 to 1 shift */ \
    __clz__x >>= __clz__b;						\
    __clz__c = __clz_tab [__clz__x];		    /* 8 to 1 bit */	\
    __clz__b = 65 - __clz__b;						\
    (گنتی) = __clz__b - __clz__c;					\
  } جب (0)
#تعریف_کرو گنتی_LEADING_ZEROS_NEED_CLZ_TAB
#اگر_ختم /* clz using cmpbge */

#اگر ! defined (گنتی_leading_zeros) && ! defined (LONGLONG_STANDALONE)
#اگر HAVE_ATTRIBUTE_CONST
طویل __MPN(گنتی_leading_zeros) (UDItype) __attribute__ ((ثابت));
#ورنہ
طویل __MPN(گنتی_leading_zeros) (UDItype);
#اگر_ختم
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  ((گنتی) = __MPN(گنتی_leading_zeros) (x))
#اگر_ختم /* clz using mpn */
#اگر_ختم /* __alpha */

#اگر defined (__AVR) && W_TYPE_سائز == 8
#تعریف_کرو umul_ppmm(ph, pl, m0, m1) \
  کرو {									\
    غیرعلامتی مختصر __p = (غیرعلامتی مختصر) (m0) * (m1);			\
    (ph) = __p >> 8;							\
    (pl) = __p;								\
  } جب (0)
#اگر_ختم /* AVR */

#اگر defined (_CRAY) && W_TYPE_سائز == 64
#شامل <intrinsics.h>
#تعریف_کرو UDIV_PREINV_ALWAYS  1
#تعریف_کرو UDIV_NEEDS_NORMALIZATION 1
طویل __MPN(گنتی_leading_zeros) (UDItype);
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  ((گنتی) = _leadz ((UWtype) (x)))
#اگر defined (_CRAYIEEE)		/* I.e., Cray T90/ieee, T3D, and T3E */
#تعریف_کرو umul_ppmm(ph, pl, m0, m1) \
  کرو {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    (ph) = int_mult_بالائی (__m0, __m1);				\
    (pl) = __m0 * __m1;							\
  } جب (0)
#اگر_غیر_تعریف_شدہ LONGLONG_STANDALONE
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
  کرو { UWtype __di;							\
    __di = __MPN(invert_limb) (d);					\
    udiv_qrnnd_preinv (q, r, n1, n0, d, __di);				\
  } جب (0)
#اگر_ختم /* LONGLONG_STANDALONE */
#اگر_ختم /* _CRAYIEEE */
#اگر_ختم /* _CRAY */

#اگر defined (__ia64) && W_TYPE_سائز == 64
/* This form encourages gcc (pre-release 3.4 at least) to emit predicated
   "sub r=r,r" and "sub r=r,r,1", giving a 2 cycle latency.  The generic
   code using "al<bl" arithmetically comes out making an actual 0 or 1 in a
   register, which takes an extra cycle.  */
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl)      \
  کرو {						\
    UWtype __x;					\
    __x = (al) - (bl);				\
    اگر ((al) < (bl))				\
      (sh) = (ah) - (bh) - 1;			\
    ورنہ					\
      (sh) = (ah) - (bh);			\
    (sl) = __x;					\
  } جب (0)
#اگر defined (__GNUC__) && ! defined (__INTEL_COMPILER)
/* Do both product parts in assembly, since that gives better code with
   all gcc versions.  Some callers will just use the upper part, and in
   that situation we waste an instruction, but not any cycles.  */
#تعریف_کرو umul_ppmm(ph, pl, m0, m1) \
    __asm__ ("xma.hu %0 = %2, %3, f0\n\txma.l %1 = %2, %3, f0"		\
	     : "=&f" (ph), "=f" (pl)					\
	     : "f" (m0), "f" (m1))
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  کرو {									\
    UWtype _x = (x), _y, _a, _c;					\
    __asm__ ("mux1 %0 = %1, @rev" : "=r" (_y) : "r" (_x));		\
    __asm__ ("czx1.l %0 = %1" : "=r" (_a) : "r" (-_y | _y));		\
    _c = (_a - 1) << 3;							\
    _x >>= _c;								\
    اگر (_x >= 1 << 4)							\
      _x >>= 4, _c += 4;						\
    اگر (_x >= 1 << 2)							\
      _x >>= 2, _c += 2;						\
    _c += _x >> 1;							\
    (گنتی) =  W_TYPE_سائز - 1 - _c;					\
  } جب (0)
/* similar to what gcc does for __builtin_ffs, but 0 based rather than 1
   based, and we don't need a special case for x==0 here */
#تعریف_کرو گنتی_trailing_zeros(گنتی, x)					\
  کرو {									\
    UWtype __ctz_x = (x);						\
    __asm__ ("popcnt %0 = %1"						\
	     : "=r" (گنتی)						\
	     : "r" ((__ctz_x-1) & ~__ctz_x));				\
  } جب (0)
#اگر_ختم
#اگر defined (__INTEL_COMPILER)
#شامل <ia64intrin.h>
#تعریف_کرو umul_ppmm(ph, pl, m0, m1)					\
  کرو {									\
    UWtype __m0 = (m0), __m1 = (m1);					\
    ph = _m64_xmahu (__m0, __m1, 0);					\
    pl = __m0 * __m1;							\
  } جب (0)
#اگر_ختم
#اگر_غیر_تعریف_شدہ LONGLONG_STANDALONE
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
  کرو { UWtype __di;							\
    __di = __MPN(invert_limb) (d);					\
    udiv_qrnnd_preinv (q, r, n1, n0, d, __di);				\
  } جب (0)
#تعریف_کرو UDIV_PREINV_ALWAYS  1
#تعریف_کرو UDIV_NEEDS_NORMALIZATION 1
#اگر_ختم
#اگر_ختم


#اگر defined (__GNUC__)

/* We sometimes need to clobber "cc" with gcc2, but that would not be
   understood by gcc1.  Use cpp to avoid major code duplication.  */
#اگر __GNUC__ < 2
#تعریف_کرو __CLOBBER_CC
#تعریف_کرو __AND_CLOBBER_CC
#ورنہ /* __GNUC__ >= 2 */
#تعریف_کرو __CLOBBER_CC : "cc"
#تعریف_کرو __AND_CLOBBER_CC , "cc"
#اگر_ختم /* __GNUC__ < 2 */

#اگر (defined (__a29k__) || defined (_AM29K)) && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add %1,%4,%5\n\taddc %0,%2,%3"				\
	   : "=r" (sh), "=&r" (sl)					\
	   : "r" (ah), "rI" (bh), "%r" (al), "rI" (bl))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub %1,%4,%5\n\tsubc %0,%2,%3"				\
	   : "=r" (sh), "=&r" (sl)					\
	   : "r" (ah), "rI" (bh), "r" (al), "rI" (bl))
#تعریف_کرو umul_ppmm(xh, xl, m0, m1) \
  کرو {									\
    USItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("multiplu %0,%1,%2"					\
	     : "=r" (xl)						\
	     : "r" (__m0), "r" (__m1));					\
    __asm__ ("multmu %0,%1,%2"						\
	     : "=r" (xh)						\
	     : "r" (__m0), "r" (__m1));					\
  } جب (0)
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
  __asm__ ("dividu %0,%3,%4"						\
	   : "=r" (q), "=q" (r)						\
	   : "1" (n1), "r" (n0), "r" (d))
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
    __asm__ ("clz %0,%1"						\
	     : "=r" (گنتی)						\
	     : "r" (x))
#تعریف_کرو گنتی_LEADING_ZEROS_0 32
#اگر_ختم /* __a29k__ */

#اگر defined (__arc__)
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add.f\t%1, %4, %5\n\tadc\t%0, %2, %3"			\
	   : "=r" (sh),							\
	     "=&r" (sl)							\
	   : "r"  ((USItype) (ah)),					\
	     "rICal" ((USItype) (bh)),					\
	     "%r" ((USItype) (al)),					\
	     "rICal" ((USItype) (bl)))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub.f\t%1, %4, %5\n\tsbc\t%0, %2, %3"			\
	   : "=r" (sh),							\
	     "=&r" (sl)							\
	   : "r" ((USItype) (ah)),					\
	     "rICal" ((USItype) (bh)),					\
	     "r" ((USItype) (al)),					\
	     "rICal" ((USItype) (bl)))
#اگر_ختم

#اگر defined (__arm__) && (defined (__thumb2__) || !defined (__thumb__)) \
    && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  کرو {									\
    اگر (__builtin_constant_p (bl) && -(USItype)(bl) < (USItype)(bl))	\
      __asm__ ("subs\t%1, %4, %5\n\tadc\t%0, %2, %3"			\
	   : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "rI" (bh),					\
		 "%r" (al), "rI" (-(USItype)(bl)) __CLOBBER_CC);	\
    ورنہ								\
      __asm__ ("adds\t%1, %4, %5\n\tadc\t%0, %2, %3"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "r" (ah), "rI" (bh), "%r" (al), "rI" (bl) __CLOBBER_CC);	\
  } جب (0)
/* FIXME: Extend the immediate range for the low word by using both ADDS and
   SUBS, since they set carry in the same way.  We need separate definitions
   for thumb and non-thumb since thumb lacks RSC.  */
#اگر defined (__thumb__)
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  کرو {									\
    اگر (__builtin_constant_p (ah) && __builtin_constant_p (bh)		\
	&& (ah) == (bh))						\
      __asm__ ("subs\t%1, %2, %3\n\tsbc\t%0, %0, %0"			\
	       : "=r" (sh), "=r" (sl)					\
	       : "r" (al), "rI" (bl) __CLOBBER_CC);			\
    ورنہ اگر (__builtin_constant_p (al))					\
      __asm__ ("rsbs\t%1, %5, %4\n\tsbc\t%0, %2, %3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "rI" (bh), "rI" (al), "r" (bl) __CLOBBER_CC); \
    ورنہ								\
      __asm__ ("subs\t%1, %4, %5\n\tsbc\t%0, %2, %3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "rI" (bh), "r" (al), "rI" (bl) __CLOBBER_CC); \
    } جب (0)
#ورنہ
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  کرو {									\
    اگر (__builtin_constant_p (ah) && __builtin_constant_p (bh)		\
	&& (ah) == (bh))						\
      __asm__ ("subs\t%1, %2, %3\n\tsbc\t%0, %0, %0"			\
	       : "=r" (sh), "=r" (sl)					\
	       : "r" (al), "rI" (bl) __CLOBBER_CC);			\
    ورنہ اگر (__builtin_constant_p (al))					\
      {									\
	اگر (__builtin_constant_p (ah))					\
	  __asm__ ("rsbs\t%1, %5, %4\n\trsc\t%0, %3, %2"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "rI" (ah), "r" (bh), "rI" (al), "r" (bl) __CLOBBER_CC); \
	ورنہ								\
	  __asm__ ("rsbs\t%1, %5, %4\n\tsbc\t%0, %2, %3"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r" (ah), "rI" (bh), "rI" (al), "r" (bl) __CLOBBER_CC); \
      }									\
    ورنہ اگر (__builtin_constant_p (ah))					\
      {									\
	اگر (__builtin_constant_p (bl))					\
	  __asm__ ("subs\t%1, %4, %5\n\trsc\t%0, %3, %2"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "rI" (ah), "r" (bh), "r" (al), "rI" (bl) __CLOBBER_CC); \
	ورنہ								\
	  __asm__ ("rsbs\t%1, %5, %4\n\trsc\t%0, %3, %2"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "rI" (ah), "r" (bh), "rI" (al), "r" (bl) __CLOBBER_CC); \
      }									\
    ورنہ								\
      __asm__ ("subs\t%1, %4, %5\n\tsbc\t%0, %2, %3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "rI" (bh), "r" (al), "rI" (bl) __CLOBBER_CC); \
    } جب (0)
#اگر_ختم
#اگر defined (__ARM_ARCH_2__) || defined (__ARM_ARCH_2A__) \
    || defined (__ARM_ARCH_3__)
#تعریف_کرو umul_ppmm(xh, xl, a, b)						\
  کرو {									\
    اندراج USItype __t0, __t1, __t2;					\
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
  } جب (0)
#اگر_غیر_تعریف_شدہ LONGLONG_STANDALONE
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
  کرو { UWtype __r;							\
    (q) = __MPN(udiv_qrnnd) (&__r, (n1), (n0), (d));			\
    (r) = __r;								\
  } جب (0)
خارجی UWtype __MPN(udiv_qrnnd) (UWtype *, UWtype, UWtype, UWtype);
#اگر_ختم /* LONGLONG_STANDALONE */
#ورنہ /* ARMv4 or newer */
#تعریف_کرو umul_ppmm(xh, xl, a, b) \
  __asm__ ("umull %0,%1,%2,%3" : "=&r" (xl), "=&r" (xh) : "r" (a), "r" (b))
#تعریف_کرو smul_ppmm(xh, xl, a, b) \
  __asm__ ("smull %0,%1,%2,%3" : "=&r" (xl), "=&r" (xh) : "r" (a), "r" (b))
#اگر_غیر_تعریف_شدہ LONGLONG_STANDALONE
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
  کرو { UWtype __di;							\
    __di = __MPN(invert_limb) (d);					\
    udiv_qrnnd_preinv (q, r, n1, n0, d, __di);				\
  } جب (0)
#تعریف_کرو UDIV_PREINV_ALWAYS  1
#تعریف_کرو UDIV_NEEDS_NORMALIZATION 1
#اگر_ختم /* LONGLONG_STANDALONE */
#اگر_ختم /* defined(__ARM_ARCH_2__) ... */
#تعریف_کرو گنتی_leading_zeros(گنتی, x)  گنتی_leading_zeros_gcc_clz(گنتی, x)
#تعریف_کرو گنتی_trailing_zeros(گنتی, x)  گنتی_trailing_zeros_gcc_ctz(گنتی, x)
#اگر_ختم /* __arm__ */

#اگر defined (__aarch64__) && W_TYPE_سائز == 64
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  کرو {									\
    اگر (__builtin_constant_p (bl) && ~(UDItype)(bl) <= (UDItype)(bl))	\
      __asm__ ("subs\t%1, %x4, %5\n\tadc\t%0, %x2, %x3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "rZ" ((UDItype)(ah)), "rZ" ((UDItype)(bh)),		\
		 "%r" ((UDItype)(al)), "rI" (-(UDItype)(bl)) __CLOBBER_CC);\
    ورنہ								\
      __asm__ ("adds\t%1, %x4, %5\n\tadc\t%0, %x2, %x3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "rZ" ((UDItype)(ah)), "rZ" ((UDItype)(bh)),		\
		 "%r" ((UDItype)(al)), "rI" ((UDItype)(bl)) __CLOBBER_CC);\
  } جب (0)
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  کرو {									\
    اگر (__builtin_constant_p (bl) && ~(UDItype)(bl) <= (UDItype)(bl))	\
      __asm__ ("adds\t%1, %x4, %5\n\tsbc\t%0, %x2, %x3"			\
	       : "=r,r" (sh), "=&r,&r" (sl)				\
	       : "rZ,rZ" ((UDItype)(ah)), "rZ,rZ" ((UDItype)(bh)),	\
		 "r,Z"   ((UDItype)(al)), "rI,r" (-(UDItype)(bl)) __CLOBBER_CC);\
    ورنہ								\
      __asm__ ("subs\t%1, %x4, %5\n\tsbc\t%0, %x2, %x3"			\
	       : "=r,r" (sh), "=&r,&r" (sl)				\
	       : "rZ,rZ" ((UDItype)(ah)), "rZ,rZ" ((UDItype)(bh)),	\
		 "r,Z"   ((UDItype)(al)), "rI,r"  ((UDItype)(bl)) __CLOBBER_CC);\
  } جب(0);
#اگر __GMP_GNUC_PREREQ (4,9)
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  کرو {									\
    قسمتعریف غیرعلامتی صحیح __ll_UTItype __attribute__((منوال_2(TI)));	\
    __ll_UTItype __ll = (__ll_UTItype)(u) * (v);			\
    w1 = __ll >> 64;							\
    w0 = __ll;								\
  } جب (0)
#اگر_ختم
#اگر !defined (umul_ppmm)
#تعریف_کرو umul_ppmm(ph, pl, m0, m1) \
  کرو {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("umulh\t%0, %1, %2" : "=r" (ph) : "r" (__m0), "r" (__m1));	\
    (pl) = __m0 * __m1;							\
  } جب (0)
#اگر_ختم
#تعریف_کرو گنتی_leading_zeros(گنتی, x)  گنتی_leading_zeros_gcc_clz(گنتی, x)
#تعریف_کرو گنتی_trailing_zeros(گنتی, x)  گنتی_trailing_zeros_gcc_ctz(گنتی, x)
#اگر_ختم /* __aarch64__ */

#اگر defined (__clipper__) && W_TYPE_سائز == 32
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  ({اتحاد {UDItype __ll;						\
	   ساخت {USItype __l, __h;} __i;				\
	  } __x;							\
  __asm__ ("mulwux %2,%0"						\
	   : "=r" (__x.__ll)						\
	   : "%0" ((USItype)(u)), "r" ((USItype)(v)));			\
  (w1) = __x.__i.__h; (w0) = __x.__i.__l;})
#تعریف_کرو smul_ppmm(w1, w0, u, v) \
  ({اتحاد {DItype __ll;							\
	   ساخت {SItype __l, __h;} __i;				\
	  } __x;							\
  __asm__ ("mulwx %2,%0"						\
	   : "=r" (__x.__ll)						\
	   : "%0" ((SItype)(u)), "r" ((SItype)(v)));			\
  (w1) = __x.__i.__h; (w0) = __x.__i.__l;})
#تعریف_کرو __umulsidi3(u, v) \
  ({UDItype __w;							\
    __asm__ ("mulwux %2,%0"						\
	     : "=r" (__w) : "%0" ((USItype)(u)), "r" ((USItype)(v)));	\
    __w; })
#اگر_ختم /* __clipper__ */

/* Fujitsu vector computers.  */
#اگر defined (__uxp__) && W_TYPE_سائز == 32
#تعریف_کرو umul_ppmm(ph, pl, u, v) \
  کرو {									\
    اتحاد {UDItype __ll;						\
	   ساخت {USItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("mult.lu %1,%2,%0"	: "=r" (__x.__ll) : "%r" (u), "rK" (v));\
    (ph) = __x.__i.__h;							\
    (pl) = __x.__i.__l;							\
  } جب (0)
#تعریف_کرو smul_ppmm(ph, pl, u, v) \
  کرو {									\
    اتحاد {UDItype __ll;						\
	   ساخت {USItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("mult.l %1,%2,%0" : "=r" (__x.__ll) : "%r" (u), "rK" (v));	\
    (ph) = __x.__i.__h;							\
    (pl) = __x.__i.__l;							\
  } جب (0)
#اگر_ختم

#اگر defined (__gmicro__) && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add.w %5,%1\n\taddx %3,%0"					\
	   : "=g" (sh), "=&g" (sl)					\
	   : "0"  ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "g" ((USItype)(bl)))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub.w %5,%1\n\tsubx %3,%0"					\
	   : "=g" (sh), "=&g" (sl)					\
	   : "0" ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "g" ((USItype)(bl)))
#تعریف_کرو umul_ppmm(ph, pl, m0, m1) \
  __asm__ ("mulx %3,%0,%1"						\
	   : "=g" (ph), "=r" (pl)					\
	   : "%0" ((USItype)(m0)), "g" ((USItype)(m1)))
#تعریف_کرو udiv_qrnnd(q, r, nh, nl, d) \
  __asm__ ("divx %4,%0,%1"						\
	   : "=g" (q), "=r" (r)						\
	   : "1" ((USItype)(nh)), "0" ((USItype)(nl)), "g" ((USItype)(d)))
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  __asm__ ("bsch/1 %1,%0"						\
	   : "=g" (گنتی) : "g" ((USItype)(x)), "0" ((USItype)0))
#اگر_ختم

#اگر defined (__hppa) && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add%I5 %5,%r4,%1\n\taddc %r2,%r3,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rM" (ah), "rM" (bh), "%rM" (al), "rI" (bl))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub%I4 %4,%r5,%1\n\tsubb %r2,%r3,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rM" (ah), "rM" (bh), "rI" (al), "rM" (bl))
#اگر defined (_PA_RISC1_1)
#تعریف_کرو umul_ppmm(wh, wl, u, v) \
  کرو {									\
    اتحاد {UDItype __ll;						\
	   ساخت {USItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("xmpyu %1,%2,%0" : "=*f" (__x.__ll) : "*f" (u), "*f" (v));	\
    (wh) = __x.__i.__h;							\
    (wl) = __x.__i.__l;							\
  } جب (0)
#اگر_ختم
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  کرو {									\
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
	: "=r" (گنتی), "=r" (__tmp) : "1" (x));			\
  } جب (0)
#اگر_ختم /* hppa */

/* These macros are for ABI=2.0w.  In ABI=2.0n they can't be used, since GCC
   (3.2) puts longlong into two adjacent 32-bit registers.  Presumably this
   is just a case of no direct support for 2.0n but treating it like 1.0. */
#اگر defined (__hppa) && W_TYPE_سائز == 64 && ! defined (_LONG_LONG_LIMB)
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add%I5 %5,%r4,%1\n\tadd,dc %r2,%r3,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rM" (ah), "rM" (bh), "%rM" (al), "rI" (bl))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub%I4 %4,%r5,%1\n\tsub,db %r2,%r3,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rM" (ah), "rM" (bh), "rI" (al), "rM" (bl))
#اگر_ختم /* hppa */

#اگر (defined (__i370__) || defined (__s390__) || defined (__mvs__)) && W_TYPE_سائز == 32
#اگر defined (__zarch__) || defined (HAVE_HOST_CPU_s390_zarch)
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl)				\
  کرو {									\
/*  if (__builtin_constant_p (bl))					\
      __asm__ ("alfi\t%1,%o5\n\talcr\t%0,%3"				\
	       : "=r" (sh), "=&r" (sl)					\
	       : "0"  (ah), "r" (bh), "%1" (al), "n" (bl) __CLOBBER_CC);\
    else								\
*/    __asm__ ("alr\t%1,%5\n\talcr\t%0,%3"				\
	       : "=r" (sh), "=&r" (sl)					\
	       : "0"  (ah), "r" (bh), "%1" (al), "r" (bl)__CLOBBER_CC);	\
  } جب (0)
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl)				\
  کرو {									\
/*  if (__builtin_constant_p (bl))					\
      __asm__ ("slfi\t%1,%o5\n\tslbr\t%0,%3"				\
	       : "=r" (sh), "=&r" (sl)					\
	       : "0" (ah), "r" (bh), "1" (al), "n" (bl) __CLOBBER_CC);	\
    else								\
*/    __asm__ ("slr\t%1,%5\n\tslbr\t%0,%3"				\
	       : "=r" (sh), "=&r" (sl)					\
	       : "0" (ah), "r" (bh), "1" (al), "r" (bl) __CLOBBER_CC);	\
  } جب (0)
#اگر __GMP_GNUC_PREREQ (4,5)
#تعریف_کرو umul_ppmm(xh, xl, m0, m1)					\
  کرو {									\
    اتحاد {UDItype __ll;						\
	   ساخت {USItype __h, __l;} __i;				\
	  } __x;							\
    __x.__ll = (UDItype) (m0) * (UDItype) (m1);				\
    (xh) = __x.__i.__h; (xl) = __x.__i.__l;				\
  } جب (0)
#ورنہ
#اگر 0
/* FIXME: this fails if gcc knows about the 64-bit registers.  Use only
   with a new enough processor pretending we have 32-bit registers.  */
#تعریف_کرو umul_ppmm(xh, xl, m0, m1)					\
  کرو {									\
    اتحاد {UDItype __ll;						\
	   ساخت {USItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("mlr\t%0,%2"						\
	     : "=r" (__x.__ll)						\
	     : "%0" (m0), "r" (m1));					\
    (xh) = __x.__i.__h; (xl) = __x.__i.__l;				\
  } جب (0)
#ورنہ
#تعریف_کرو umul_ppmm(xh, xl, m0, m1)					\
  کرو {									\
  /* When we have 64-bit regs and gcc is aware of that, we cannot simply use
     DImode for the product, since that would be allocated to a single 64-bit
     register, whereas mlr uses the low 32-bits of an even-odd register pair.
  */									\
    اندراج USItype __r0 __asm__ ("0");				\
    اندراج USItype __r1 __asm__ ("1") = (m0);				\
    __asm__ ("mlr\t%0,%3"						\
	     : "=r" (__r0), "=r" (__r1)					\
	     : "r" (__r1), "r" (m1));					\
    (xh) = __r0; (xl) = __r1;						\
  } جب (0)
#اگر_ختم /* if 0 */
#اگر_ختم
#اگر 0
/* FIXME: this fails if gcc knows about the 64-bit registers.  Use only
   with a new enough processor pretending we have 32-bit registers.  */
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d)					\
  کرو {									\
    اتحاد {UDItype __ll;						\
	   ساخت {USItype __h, __l;} __i;				\
	  } __x;							\
    __x.__i.__h = n1; __x.__i.__l = n0;					\
    __asm__ ("dlr\t%0,%2"						\
	     : "=r" (__x.__ll)						\
	     : "0" (__x.__ll), "r" (d));				\
    (q) = __x.__i.__l; (r) = __x.__i.__h;				\
  } جب (0)
#ورنہ
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d)					\
  کرو {									\
    اندراج USItype __r0 __asm__ ("0") = (n1);				\
    اندراج USItype __r1 __asm__ ("1") = (n0);				\
    __asm__ ("dlr\t%0,%4"						\
	     : "=r" (__r0), "=r" (__r1)					\
	     : "r" (__r0), "r" (__r1), "r" (d));			\
    (q) = __r1; (r) = __r0;						\
  } جب (0)
#اگر_ختم /* if 0 */
#ورنہ /* if __zarch__ */
/* FIXME: this fails if gcc knows about the 64-bit registers.  */
#تعریف_کرو smul_ppmm(xh, xl, m0, m1)					\
  کرو {									\
    اتحاد {DItype __ll;							\
	   ساخت {USItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("mr\t%0,%2"						\
	     : "=r" (__x.__ll)						\
	     : "%0" (m0), "r" (m1));					\
    (xh) = __x.__i.__h; (xl) = __x.__i.__l;				\
  } جب (0)
/* FIXME: this fails if gcc knows about the 64-bit registers.  */
#تعریف_کرو sdiv_qrnnd(q, r, n1, n0, d)					\
  کرو {									\
    اتحاد {DItype __ll;							\
	   ساخت {USItype __h, __l;} __i;				\
	  } __x;							\
    __x.__i.__h = n1; __x.__i.__l = n0;					\
    __asm__ ("dr\t%0,%2"						\
	     : "=r" (__x.__ll)						\
	     : "0" (__x.__ll), "r" (d));				\
    (q) = __x.__i.__l; (r) = __x.__i.__h;				\
  } جب (0)
#اگر_ختم /* if __zarch__ */
#اگر_ختم

#اگر defined (__s390x__) && W_TYPE_سائز == 64
/* We need to cast operands with register constraints, otherwise their types
   will be assumed to be SImode by gcc.  For these machines, such operations
   will insert a value into the low 32 bits, and leave the high 32 bits with
   garbage.  */
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl)				\
  کرو {									\
    __asm__ ("algr\t%1,%5\n\talcgr\t%0,%3"				\
	       : "=r" (sh), "=&r" (sl)					\
	       : "0"  ((UDItype)(ah)), "r" ((UDItype)(bh)),		\
		 "%1" ((UDItype)(al)), "r" ((UDItype)(bl)) __CLOBBER_CC); \
  } جب (0)
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl)				\
  کرو {									\
    __asm__ ("slgr\t%1,%5\n\tslbgr\t%0,%3"				\
	     : "=r" (sh), "=&r" (sl)					\
	     : "0" ((UDItype)(ah)), "r" ((UDItype)(bh)),		\
	       "1" ((UDItype)(al)), "r" ((UDItype)(bl)) __CLOBBER_CC);	\
  } جب (0)
#اگر !defined (__clang__)
#تعریف_کرو umul_ppmm(xh, xl, m0, m1)					\
  کرو {									\
    اتحاد {غیرعلامتی صحیح __attribute__ ((منوال_2(TI))) __ll;		\
	   ساخت {UDItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("mlgr\t%0,%2"						\
	     : "=r" (__x.__ll)						\
	     : "%0" ((UDItype)(m0)), "r" ((UDItype)(m1)));		\
    (xh) = __x.__i.__h; (xl) = __x.__i.__l;				\
  } جب (0)
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d)					\
  کرو {									\
    اتحاد {غیرعلامتی صحیح __attribute__ ((منوال_2(TI))) __ll;		\
	   ساخت {UDItype __h, __l;} __i;				\
	  } __x;							\
    __x.__i.__h = n1; __x.__i.__l = n0;					\
    __asm__ ("dlgr\t%0,%2"						\
	     : "=r" (__x.__ll)						\
	     : "0" (__x.__ll), "r" ((UDItype)(d)));			\
    (q) = __x.__i.__l; (r) = __x.__i.__h;				\
  } جب (0)
#اگر_ختم
#اگر 0 /* FIXME: Enable for z10 (?) */
#تعریف_کرو گنتی_leading_zeros(cnt, x)					\
  کرو {									\
    اتحاد {غیرعلامتی صحیح __attribute__ ((منوال_2(TI))) __ll;		\
	   ساخت {UDItype __h, __l;} __i;				\
	  } __clr_cnt;							\
    __asm__ ("flogr\t%0,%1"						\
	     : "=r" (__clr_cnt.__ll)					\
	     : "r" (x) __CLOBBER_CC);					\
    (cnt) = __clr_cnt.__i.__h;						\
  } جب (0)
#اگر_ختم
#اگر_ختم

/* On x86 and x86_64, every asm implicitly clobbers "flags" and "fpsr",
   so we don't need __CLOBBER_CC.  */
#اگر (defined (__i386__) || defined (__i486__)) && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addl %5,%k1\n\tadcl %3,%k0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0"  ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "g" ((USItype)(bl)))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subl %5,%k1\n\tsbbl %3,%k0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0" ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "g" ((USItype)(bl)))
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("mull %3"							\
	   : "=a" (w0), "=d" (w1)					\
	   : "%0" ((USItype)(u)), "rm" ((USItype)(v)))
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, dx) /* d renamed to dx avoiding "=d" */\
  __asm__ ("divl %4"		     /* stringification in K&R C */	\
	   : "=a" (q), "=d" (r)						\
	   : "0" ((USItype)(n0)), "1" ((USItype)(n1)), "rm" ((USItype)(dx)))

#اگر HAVE_HOST_CPU_i586 || HAVE_HOST_CPU_pentium || HAVE_HOST_CPU_pentiummmx
/* Pentium bsrl takes between 10 and 72 cycles depending where the most
   significant 1 bit is, hence the use of the following alternatives.  bsfl
   is slow too, between 18 and 42 depending where the least significant 1
   bit is, so let the generic count_trailing_zeros below make use of the
   count_leading_zeros here too.  */

#اگر HAVE_HOST_CPU_pentiummmx && ! defined (LONGLONG_STANDALONE)
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

#تعریف_کرو گنتی_leading_zeros(c,n)					\
  کرو {									\
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
  } جب (0)
#تعریف_کرو گنتی_LEADING_ZEROS_NEED_CLZ_TAB
#تعریف_کرو گنتی_LEADING_ZEROS_0   31   /* n==0 indistinguishable from n==1 */

#ورنہ /* ! pentiummmx || LONGLONG_STANDALONE */
/* The following should be a fixed 14 cycles or so.  Some scheduling
   opportunities should be available between the float load/store too.  This
   sort of code is used in gcc 3 for __builtin_ffs (with "n&-n") and is
   apparently suggested by the Intel optimizing manual (don't know exactly
   where).  gcc 2.95 or up will be best for this, so the "double" is
   correctly aligned on the stack.  */
#تعریف_کرو گنتی_leading_zeros(c,n)					\
  کرو {									\
    اتحاد {								\
      دوہرا    d;							\
      غیرعلامتی  a[2];							\
    } __u;								\
    __u.d = (UWtype) (n);						\
    (c) = 0x3FF + 31 - (__u.a[1] >> 20);				\
  } جب (0)
#تعریف_کرو گنتی_LEADING_ZEROS_0   (0x3FF + 31)
#اگر_ختم /* pentiummx */

#ورنہ /* ! pentium */

#اگر __GMP_GNUC_PREREQ (3,4)  /* using bsrl */
#تعریف_کرو گنتی_leading_zeros(گنتی,x)  گنتی_leading_zeros_gcc_clz(گنتی,x)
#اگر_ختم /* gcc clz */

/* On P6, gcc prior to 3.0 generates a partial register stall for
   __cbtmp^31, due to using "xorb $31" instead of "xorl $31", the former
   being 1 code byte smaller.  "31-__cbtmp" is a workaround, probably at the
   cost of one extra instruction.  Do this for "i386" too, since that means
   generic x86.  */
#اگر ! defined (گنتی_leading_zeros) && __GNUC__ < 3			\
  && (HAVE_HOST_CPU_i386						\
      || HAVE_HOST_CPU_i686						\
      || HAVE_HOST_CPU_pentiumpro					\
      || HAVE_HOST_CPU_pentium2						\
      || HAVE_HOST_CPU_pentium3)
#تعریف_کرو گنتی_leading_zeros(گنتی, x)					\
  کرو {									\
    USItype __cbtmp;							\
    ASSERT ((x) != 0);							\
    __asm__ ("bsrl %1,%0" : "=r" (__cbtmp) : "rm" ((USItype)(x)));	\
    (گنتی) = 31 - __cbtmp;						\
  } جب (0)
#اگر_ختم /* gcc<3 asm bsrl */

#اگر_غیر_تعریف_شدہ گنتی_leading_zeros
#تعریف_کرو گنتی_leading_zeros(گنتی, x)					\
  کرو {									\
    USItype __cbtmp;							\
    ASSERT ((x) != 0);							\
    __asm__ ("bsrl %1,%0" : "=r" (__cbtmp) : "rm" ((USItype)(x)));	\
    (گنتی) = __cbtmp ^ 31;						\
  } جب (0)
#اگر_ختم /* asm bsrl */

#اگر __GMP_GNUC_PREREQ (3,4)  /* using bsfl */
#تعریف_کرو گنتی_trailing_zeros(گنتی,x)  گنتی_trailing_zeros_gcc_ctz(گنتی,x)
#اگر_ختم /* gcc ctz */

#اگر_غیر_تعریف_شدہ گنتی_trailing_zeros
#تعریف_کرو گنتی_trailing_zeros(گنتی, x)					\
  کرو {									\
    ASSERT ((x) != 0);							\
    __asm__ ("bsfl %1,%k0" : "=r" (گنتی) : "rm" ((USItype)(x)));	\
  } جب (0)
#اگر_ختم /* asm bsfl */

#اگر_ختم /* ! pentium */

#اگر_ختم /* 80x86 */

#اگر defined (__amd64__) && W_TYPE_سائز == 64
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addq %5,%q1\n\tadcq %3,%q0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0"  ((UDItype)(ah)), "rme" ((UDItype)(bh)),		\
	     "%1" ((UDItype)(al)), "rme" ((UDItype)(bl)))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subq %5,%q1\n\tsbbq %3,%q0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0" ((UDItype)(ah)), "rme" ((UDItype)(bh)),		\
	     "1" ((UDItype)(al)), "rme" ((UDItype)(bl)))
#اگر X86_ASM_MULX \
   && (HAVE_HOST_CPU_haswell || HAVE_HOST_CPU_broadwell \
       || HAVE_HOST_CPU_skylake || HAVE_HOST_CPU_bd4 || HAVE_HOST_CPU_zen)
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("mulx\t%3, %q0, %q1"						\
	   : "=r" (w0), "=r" (w1)					\
	   : "%d" ((UDItype)(u)), "rm" ((UDItype)(v)))
#ورنہ
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("mulq\t%3"							\
	   : "=a" (w0), "=d" (w1)					\
	   : "%0" ((UDItype)(u)), "rm" ((UDItype)(v)))
#اگر_ختم
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, dx) /* d renamed to dx avoiding "=d" */\
  __asm__ ("divq %4"		     /* stringification in K&R C */	\
	   : "=a" (q), "=d" (r)						\
	   : "0" ((UDItype)(n0)), "1" ((UDItype)(n1)), "rm" ((UDItype)(dx)))

#اگر HAVE_HOST_CPU_haswell || HAVE_HOST_CPU_broadwell || HAVE_HOST_CPU_skylake \
  || HAVE_HOST_CPU_k10 || HAVE_HOST_CPU_bd1 || HAVE_HOST_CPU_bd2	\
  || HAVE_HOST_CPU_bd3 || HAVE_HOST_CPU_bd4 || HAVE_HOST_CPU_zen	\
  || HAVE_HOST_CPU_bobcat || HAVE_HOST_CPU_jaguar
#تعریف_کرو گنتی_leading_zeros(گنتی, x)					\
  کرو {									\
    /* This is lzcnt, spelled for older assemblers.  Destination and */	\
    /* source must be a 64-bit registers, hence cast and %q.         */	\
    __asm__ ("rep;bsr\t%1, %q0" : "=r" (گنتی) : "rm" ((UDItype)(x)));	\
  } جب (0)
#تعریف_کرو گنتی_LEADING_ZEROS_0 64
#ورنہ
#تعریف_کرو گنتی_leading_zeros(گنتی, x)					\
  کرو {									\
    UDItype __cbtmp;							\
    ASSERT ((x) != 0);							\
    __asm__ ("bsr\t%1,%0" : "=r" (__cbtmp) : "rm" ((UDItype)(x)));	\
    (گنتی) = __cbtmp ^ 63;						\
  } جب (0)
#اگر_ختم

#اگر HAVE_HOST_CPU_bd2 || HAVE_HOST_CPU_bd3 || HAVE_HOST_CPU_bd4 \
  || HAVE_HOST_CPU_zen || HAVE_HOST_CPU_jaguar
#تعریف_کرو گنتی_trailing_zeros(گنتی, x)					\
  کرو {									\
    /* This is tzcnt, spelled for older assemblers.  Destination and */	\
    /* source must be a 64-bit registers, hence cast and %q.         */	\
    __asm__ ("rep;bsf\t%1, %q0" : "=r" (گنتی) : "rm" ((UDItype)(x)));	\
  } جب (0)
#تعریف_کرو گنتی_TRAILING_ZEROS_0 64
#ورنہ
#تعریف_کرو گنتی_trailing_zeros(گنتی, x)					\
  کرو {									\
    ASSERT ((x) != 0);							\
    __asm__ ("bsf\t%1, %q0" : "=r" (گنتی) : "rm" ((UDItype)(x)));	\
  } جب (0)
#اگر_ختم
#اگر_ختم /* __amd64__ */

#اگر defined (__i860__) && W_TYPE_سائز == 32
#تعریف_کرو rshift_rhlc(r,h,l,c) \
  __asm__ ("shr %3,r0,r0\;shrd %1,%2,%0"				\
	   "=r" (r) : "r" (h), "r" (l), "rn" (c))
#اگر_ختم /* i860 */

#اگر defined (__i960__) && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("cmpo 1,0\;addc %5,%4,%1\;addc %3,%2,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "dI" (ah), "dI" (bh), "%dI" (al), "dI" (bl))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("cmpo 0,0\;subc %5,%4,%1\;subc %3,%2,%0"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "dI" (ah), "dI" (bh), "dI" (al), "dI" (bl))
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  ({اتحاد {UDItype __ll;						\
	   ساخت {USItype __l, __h;} __i;				\
	  } __x;							\
  __asm__ ("emul %2,%1,%0"						\
	   : "=d" (__x.__ll) : "%dI" (u), "dI" (v));			\
  (w1) = __x.__i.__h; (w0) = __x.__i.__l;})
#تعریف_کرو __umulsidi3(u, v) \
  ({UDItype __w;							\
    __asm__ ("emul %2,%1,%0" : "=d" (__w) : "%dI" (u), "dI" (v));	\
    __w; })
#تعریف_کرو udiv_qrnnd(q, r, nh, nl, d) \
  کرو {									\
    اتحاد {UDItype __ll;						\
	   ساخت {USItype __l, __h;} __i;				\
	  } __nn;							\
    __nn.__i.__h = (nh); __nn.__i.__l = (nl);				\
    __asm__ ("ediv %d,%n,%0"						\
	   : "=d" (__rq.__ll) : "dI" (__nn.__ll), "dI" (d));		\
    (r) = __rq.__i.__l; (q) = __rq.__i.__h;				\
  } جب (0)
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  کرو {									\
    USItype __cbtmp;							\
    __asm__ ("scanbit %1,%0" : "=r" (__cbtmp) : "r" (x));		\
    (گنتی) = __cbtmp ^ 31;						\
  } جب (0)
#تعریف_کرو گنتی_LEADING_ZEROS_0 (-32) /* sic */
#اگر defined (__i960mx)		/* what is the proper symbol to test??? */
#تعریف_کرو rshift_rhlc(r,h,l,c) \
  کرو {									\
    اتحاد {UDItype __ll;						\
	   ساخت {USItype __l, __h;} __i;				\
	  } __nn;							\
    __nn.__i.__h = (h); __nn.__i.__l = (l);				\
    __asm__ ("shre %2,%1,%0" : "=d" (r) : "dI" (__nn.__ll), "dI" (c));	\
  }
#اگر_ختم /* i960mx */
#اگر_ختم /* i960 */


#اگر defined (__loongarch64) && W_TYPE_سائز == 64
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  کرو {									\
    UDItype __u = (u), __v = (v);					\
    (w0) = __u * __v;							\
    (w1) = __extension__ (غیرعلامتی __int128) __u * __v >> 64;		\
  } جب (0)
#اگر_ختم


#اگر (defined (__mc68000__) || defined (__mc68020__) || defined(mc68020) \
     || defined (__m68k__) || defined (__mc5200__) || defined (__mc5206e__) \
     || defined (__mc5307__)) && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add%.l %5,%1\n\taddx%.l %3,%0"				\
	   : "=d" (sh), "=&d" (sl)					\
	   : "0"  ((USItype)(ah)), "d" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "g" ((USItype)(bl)))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub%.l %5,%1\n\tsubx%.l %3,%0"				\
	   : "=d" (sh), "=&d" (sl)					\
	   : "0" ((USItype)(ah)), "d" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "g" ((USItype)(bl)))
/* The '020, '030, '040 and CPU32 have 32x32->64 and 64/32->32q-32r.  */
#اگر defined (__mc68020__) || defined(mc68020) \
     || defined (__mc68030__) || defined (mc68030) \
     || defined (__mc68040__) || defined (mc68040) \
     || defined (__mcpu32__) || defined (mcpu32) \
     || defined (__NeXT__)
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("mulu%.l %3,%1:%0"						\
	   : "=d" (w0), "=d" (w1)					\
	   : "%0" ((USItype)(u)), "dmi" ((USItype)(v)))
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
  __asm__ ("divu%.l %4,%1:%0"						\
	   : "=d" (q), "=d" (r)						\
	   : "0" ((USItype)(n0)), "1" ((USItype)(n1)), "dmi" ((USItype)(d)))
#تعریف_کرو sdiv_qrnnd(q, r, n1, n0, d) \
  __asm__ ("divs%.l %4,%1:%0"						\
	   : "=d" (q), "=d" (r)						\
	   : "0" ((USItype)(n0)), "1" ((USItype)(n1)), "dmi" ((USItype)(d)))
#ورنہ /* for other 68k family members use 16x16->32 multiplication */
#تعریف_کرو umul_ppmm(xh, xl, a, b) \
  کرو { USItype __umul_tmp1, __umul_tmp2;				\
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
  } جب (0)
#اگر_ختم /* not mc68020 */
/* The '020, '030, '040 and '060 have bitfield insns.
   GCC 3.4 defines __mc68020__ when in CPU32 mode, check for __mcpu32__ to
   exclude bfffo on that chip (bitfield insns not available).  */
#اگر (defined (__mc68020__) || defined (mc68020)    \
     || defined (__mc68030__) || defined (mc68030) \
     || defined (__mc68040__) || defined (mc68040) \
     || defined (__mc68060__) || defined (mc68060) \
     || defined (__NeXT__))			   \
  && ! defined (__mcpu32__)
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  __asm__ ("bfffo %1{%b2:%b2},%0"					\
	   : "=d" (گنتی)						\
	   : "od" ((USItype) (x)), "n" (0))
#تعریف_کرو گنتی_LEADING_ZEROS_0 32
#اگر_ختم
#اگر_ختم /* mc68000 */

#اگر defined (__m88000__) && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addu.co %1,%r4,%r5\n\taddu.ci %0,%r2,%r3"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rJ" (ah), "rJ" (bh), "%rJ" (al), "rJ" (bl))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subu.co %1,%r4,%r5\n\tsubu.ci %0,%r2,%r3"			\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rJ" (ah), "rJ" (bh), "rJ" (al), "rJ" (bl))
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  کرو {									\
    USItype __cbtmp;							\
    __asm__ ("ff1 %0,%1" : "=r" (__cbtmp) : "r" (x));			\
    (گنتی) = __cbtmp ^ 31;						\
  } جب (0)
#تعریف_کرو گنتی_LEADING_ZEROS_0 63 /* sic */
#اگر defined (__m88110__)
#تعریف_کرو umul_ppmm(wh, wl, u, v) \
  کرو {									\
    اتحاد {UDItype __ll;						\
	   ساخت {USItype __h, __l;} __i;				\
	  } __x;							\
    __asm__ ("mulu.d %0,%1,%2" : "=r" (__x.__ll) : "r" (u), "r" (v));	\
    (wh) = __x.__i.__h;							\
    (wl) = __x.__i.__l;							\
  } جب (0)
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
  ({اتحاد {UDItype __ll;						\
	   ساخت {USItype __h, __l;} __i;				\
	  } __x, __q;							\
  __x.__i.__h = (n1); __x.__i.__l = (n0);				\
  __asm__ ("divu.d %0,%1,%2"						\
	   : "=r" (__q.__ll) : "r" (__x.__ll), "r" (d));		\
  (r) = (n0) - __q.__l * (d); (q) = __q.__l; })
#اگر_ختم /* __m88110__ */
#اگر_ختم /* __m88000__ */

#اگر defined (__mips) && W_TYPE_سائز == 32
#اگر __GMP_GNUC_PREREQ (4,4)
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  کرو {									\
    UDItype __ll = (UDItype)(u) * (v);					\
    w1 = __ll >> 32;							\
    w0 = __ll;								\
  } جب (0)
#اگر_ختم
#اگر !defined (umul_ppmm) && __GMP_GNUC_PREREQ (2,7) && !defined (__clang__)
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("multu %2,%3" : "=l" (w0), "=h" (w1) : "d" (u), "d" (v))
#اگر_ختم
#اگر !defined (umul_ppmm)
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("multu %2,%3\n\tmflo %0\n\tmfhi %1"				\
	   : "=d" (w0), "=d" (w1) : "d" (u), "d" (v))
#اگر_ختم
#اگر_ختم /* __mips */

#اگر (defined (__mips) && __mips >= 3) && W_TYPE_سائز == 64
#اگر defined (_MIPS_ARCH_MIPS64R6)
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  کرو {									\
    UDItype __m0 = (u), __m1 = (v);					\
    (w0) = __m0 * __m1;							\
    __asm__ ("dmuhu\t%0, %1, %2" : "=d" (w1) : "d" (__m0), "d" (__m1));	\
  } جب (0)
#اگر_ختم
#اگر !defined (umul_ppmm) && __GMP_GNUC_PREREQ (4,4)
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  کرو {									\
    قسمتعریف غیرعلامتی صحیح __ll_UTItype __attribute__((منوال_2(TI)));	\
    __ll_UTItype __ll = (__ll_UTItype)(u) * (v);			\
    w1 = __ll >> 64;							\
    w0 = __ll;								\
  } جب (0)
#اگر_ختم
#اگر !defined (umul_ppmm) && __GMP_GNUC_PREREQ (2,7) && !defined (__clang__)
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("dmultu %2,%3"						\
	   : "=l" (w0), "=h" (w1)					\
	   : "d" ((UDItype)(u)), "d" ((UDItype)(v)))
#اگر_ختم
#اگر !defined (umul_ppmm)
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("dmultu %2,%3\n\tmflo %0\n\tmfhi %1"				\
	   : "=d" (w0), "=d" (w1)					\
	   : "d" ((UDItype)(u)), "d" ((UDItype)(v)))
#اگر_ختم
#اگر_ختم /* __mips */

#اگر defined (__mmix__) && W_TYPE_سائز == 64
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("MULU %0,%2,%3" : "=r" (w0), "=z" (w1) : "r" (u), "r" (v))
#اگر_ختم

#اگر defined (__ns32000__) && W_TYPE_سائز == 32
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  ({اتحاد {UDItype __ll;						\
	   ساخت {USItype __l, __h;} __i;				\
	  } __x;							\
  __asm__ ("meid %2,%0"							\
	   : "=g" (__x.__ll)						\
	   : "%0" ((USItype)(u)), "g" ((USItype)(v)));			\
  (w1) = __x.__i.__h; (w0) = __x.__i.__l;})
#تعریف_کرو __umulsidi3(u, v) \
  ({UDItype __w;							\
    __asm__ ("meid %2,%0"						\
	     : "=g" (__w)						\
	     : "%0" ((USItype)(u)), "g" ((USItype)(v)));		\
    __w; })
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
  ({اتحاد {UDItype __ll;						\
	   ساخت {USItype __l, __h;} __i;				\
	  } __x;							\
  __x.__i.__h = (n1); __x.__i.__l = (n0);				\
  __asm__ ("deid %2,%0"							\
	   : "=g" (__x.__ll)						\
	   : "0" (__x.__ll), "g" ((USItype)(d)));			\
  (r) = __x.__i.__l; (q) = __x.__i.__h; })
#تعریف_کرو گنتی_trailing_zeros(گنتی,x) \
  کرو {									\
    __asm__ ("ffsd	%2,%0"						\
	     : "=r" (گنتی)						\
	     : "0" ((USItype) 0), "r" ((USItype) (x)));			\
  } جب (0)
#اگر_ختم /* __ns32000__ */

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

#اگر (HAVE_HOST_CPU_FAMILY_power || HAVE_HOST_CPU_FAMILY_powerpc)	\
  && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  کرو {									\
    اگر (__builtin_constant_p (bh) && (bh) == 0)				\
      __asm__ ("add%I4c %1,%3,%4\n\taddze %0,%2"			\
	       : "=r" (sh), "=&r" (sl) : "r" (ah), "%r" (al), "rI" (bl)	\
		 __CLOBBER_CC);						\
    ورنہ اگر (__builtin_constant_p (bh) && (bh) == ~(USItype) 0)		\
      __asm__ ("add%I4c %1,%3,%4\n\taddme %0,%2"			\
	       : "=r" (sh), "=&r" (sl) : "r" (ah), "%r" (al), "rI" (bl)	\
		 __CLOBBER_CC);						\
    ورنہ								\
      __asm__ ("add%I5c %1,%4,%5\n\tadde %0,%2,%3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "r" (bh), "%r" (al), "rI" (bl)		\
		 __CLOBBER_CC);						\
  } جب (0)
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  کرو {									\
    اگر (__builtin_constant_p (ah) && (ah) == 0)				\
      __asm__ ("subf%I3c %1,%4,%3\n\tsubfze %0,%2"			\
	       : "=r" (sh), "=&r" (sl) : "r" (bh), "rI" (al), "r" (bl)	\
		 __CLOBBER_CC);						\
    ورنہ اگر (__builtin_constant_p (ah) && (ah) == ~(USItype) 0)		\
      __asm__ ("subf%I3c %1,%4,%3\n\tsubfme %0,%2"			\
	       : "=r" (sh), "=&r" (sl) : "r" (bh), "rI" (al), "r" (bl)	\
		 __CLOBBER_CC);						\
    ورنہ اگر (__builtin_constant_p (bh) && (bh) == 0)			\
      __asm__ ("subf%I3c %1,%4,%3\n\taddme %0,%2"			\
	       : "=r" (sh), "=&r" (sl) : "r" (ah), "rI" (al), "r" (bl)	\
		 __CLOBBER_CC);						\
    ورنہ اگر (__builtin_constant_p (bh) && (bh) == ~(USItype) 0)		\
      __asm__ ("subf%I3c %1,%4,%3\n\taddze %0,%2"			\
	       : "=r" (sh), "=&r" (sl) : "r" (ah), "rI" (al), "r" (bl)	\
		 __CLOBBER_CC);						\
    ورنہ								\
      __asm__ ("subf%I4c %1,%5,%4\n\tsubfe %0,%3,%2"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r" (ah), "r" (bh), "rI" (al), "r" (bl)		\
		 __CLOBBER_CC);						\
  } جب (0)
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  __asm__ ("cntlzw %0,%1" : "=r" (گنتی) : "r" (x))
#تعریف_کرو گنتی_LEADING_ZEROS_0 32
#اگر HAVE_HOST_CPU_FAMILY_powerpc
#اگر __GMP_GNUC_PREREQ (4,4)
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  کرو {									\
    UDItype __ll = (UDItype)(u) * (v);					\
    w1 = __ll >> 32;							\
    w0 = __ll;								\
  } جب (0)
#اگر_ختم
#اگر !defined (umul_ppmm)
#تعریف_کرو umul_ppmm(ph, pl, m0, m1) \
  کرو {									\
    USItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("mulhwu %0,%1,%2" : "=r" (ph) : "%r" (m0), "r" (m1));	\
    (pl) = __m0 * __m1;							\
  } جب (0)
#اگر_ختم
#تعریف_کرو smul_ppmm(ph, pl, m0, m1) \
  کرو {									\
    SItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("mulhw %0,%1,%2" : "=r" (ph) : "%r" (m0), "r" (m1));	\
    (pl) = __m0 * __m1;							\
  } جب (0)
#ورنہ
#تعریف_کرو smul_ppmm(xh, xl, m0, m1) \
  __asm__ ("mul %0,%2,%3" : "=r" (xh), "=q" (xl) : "r" (m0), "r" (m1))
#تعریف_کرو sdiv_qrnnd(q, r, nh, nl, d) \
  __asm__ ("div %0,%2,%4" : "=r" (q), "=q" (r) : "r" (nh), "1" (nl), "r" (d))
#اگر_ختم
#اگر_ختم /* 32-bit POWER architecture variants.  */

/* We should test _IBMR2 here when we add assembly support for the system
   vendor compilers.  */
#اگر HAVE_HOST_CPU_FAMILY_powerpc && W_TYPE_سائز == 64
#اگر !defined (_LONG_LONG_LIMB)
/* _LONG_LONG_LIMB is ABI=mode32 where adde operates on 32-bit values.  So
   use adde etc only when not _LONG_LONG_LIMB.  */
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  کرو {									\
    اگر (__builtin_constant_p (bh) && (bh) == 0)				\
      __asm__ ("add%I4c %1,%3,%4\n\taddze %0,%2"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r"  ((UDItype)(ah)),					\
		 "%r" ((UDItype)(al)), "rI" ((UDItype)(bl))		\
		 __CLOBBER_CC);						\
    ورنہ اگر (__builtin_constant_p (bh) && (bh) == ~(UDItype) 0)		\
      __asm__ ("add%I4c %1,%3,%4\n\taddme %0,%2"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r"  ((UDItype)(ah)),					\
		 "%r" ((UDItype)(al)), "rI" ((UDItype)(bl))		\
		 __CLOBBER_CC);						\
    ورنہ								\
      __asm__ ("add%I5c %1,%4,%5\n\tadde %0,%2,%3"			\
	       : "=r" (sh), "=&r" (sl)					\
	       : "r"  ((UDItype)(ah)), "r"  ((UDItype)(bh)),		\
		 "%r" ((UDItype)(al)), "rI" ((UDItype)(bl))		\
		 __CLOBBER_CC);						\
  } جب (0)
/* We use "*rI" for the constant operand here, since with just "I", gcc barfs.
   This might seem strange, but gcc folds away the dead code late.  */
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  کرو {									\
    اگر (__builtin_constant_p (bl)					\
	&& (bl) > -0x8000 && (bl) <= 0x8000 && (bl) != 0) {		\
	اگر (__builtin_constant_p (ah) && (ah) == 0)			\
	  __asm__ ("addic %1,%3,%4\n\tsubfze %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   :                       "r" ((UDItype)(bh)),		\
		     "r" ((UDItype)(al)), "*rI" (-((UDItype)(bl)))	\
		     __CLOBBER_CC);					\
	ورنہ اگر (__builtin_constant_p (ah) && (ah) == ~(UDItype) 0)	\
	  __asm__ ("addic %1,%3,%4\n\tsubfme %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   :                       "r" ((UDItype)(bh)),		\
		     "r" ((UDItype)(al)), "*rI" (-((UDItype)(bl)))	\
		     __CLOBBER_CC);					\
	ورنہ اگر (__builtin_constant_p (bh) && (bh) == 0)		\
	  __asm__ ("addic %1,%3,%4\n\taddme %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r" ((UDItype)(ah)),				\
		     "r" ((UDItype)(al)), "*rI" (-((UDItype)(bl)))	\
		     __CLOBBER_CC);					\
	ورنہ اگر (__builtin_constant_p (bh) && (bh) == ~(UDItype) 0)	\
	  __asm__ ("addic %1,%3,%4\n\taddze %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r" ((UDItype)(ah)),				\
		     "r" ((UDItype)(al)), "*rI" (-((UDItype)(bl)))	\
		     __CLOBBER_CC);					\
	ورنہ								\
	  __asm__ ("addic %1,%4,%5\n\tsubfe %0,%3,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r" ((UDItype)(ah)), "r" ((UDItype)(bh)),		\
		     "r" ((UDItype)(al)), "*rI" (-((UDItype)(bl)))	\
		     __CLOBBER_CC);					\
    } ورنہ {								\
	اگر (__builtin_constant_p (ah) && (ah) == 0)			\
	  __asm__ ("subf%I3c %1,%4,%3\n\tsubfze %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   :                       "r" ((UDItype)(bh)),		\
		     "rI" ((UDItype)(al)), "r" ((UDItype)(bl))		\
		     __CLOBBER_CC);					\
	ورنہ اگر (__builtin_constant_p (ah) && (ah) == ~(UDItype) 0)	\
	  __asm__ ("subf%I3c %1,%4,%3\n\tsubfme %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   :                       "r" ((UDItype)(bh)),		\
		     "rI" ((UDItype)(al)), "r" ((UDItype)(bl))		\
		     __CLOBBER_CC);					\
	ورنہ اگر (__builtin_constant_p (bh) && (bh) == 0)		\
	  __asm__ ("subf%I3c %1,%4,%3\n\taddme %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r"  ((UDItype)(ah)),				\
		     "rI" ((UDItype)(al)), "r" ((UDItype)(bl))		\
		     __CLOBBER_CC);					\
	ورنہ اگر (__builtin_constant_p (bh) && (bh) == ~(UDItype) 0)	\
	  __asm__ ("subf%I3c %1,%4,%3\n\taddze %0,%2"			\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r"  ((UDItype)(ah)),				\
		     "rI" ((UDItype)(al)), "r" ((UDItype)(bl))		\
		     __CLOBBER_CC);					\
	ورنہ								\
	  __asm__ ("subf%I4c %1,%5,%4\n\tsubfe %0,%3,%2"		\
		   : "=r" (sh), "=&r" (sl)				\
		   : "r"  ((UDItype)(ah)), "r" ((UDItype)(bh)),		\
		     "rI" ((UDItype)(al)), "r" ((UDItype)(bl))		\
		     __CLOBBER_CC);					\
    }									\
  } جب (0)
#اگر_ختم /* ! _LONG_LONG_LIMB */
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  __asm__ ("cntlzd %0,%1" : "=r" (گنتی) : "r" (x))
#تعریف_کرو گنتی_LEADING_ZEROS_0 64
#اگر __GMP_GNUC_PREREQ (4,8)
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  کرو {									\
    قسمتعریف غیرعلامتی صحیح __ll_UTItype __attribute__((منوال_2(TI)));	\
    __ll_UTItype __ll = (__ll_UTItype)(u) * (v);			\
    w1 = __ll >> 64;							\
    w0 = __ll;								\
  } جب (0)
#اگر_ختم
#اگر !defined (umul_ppmm)
#تعریف_کرو umul_ppmm(ph, pl, m0, m1) \
  کرو {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("mulhdu %0,%1,%2" : "=r" (ph) : "%r" (__m0), "r" (__m1));	\
    (pl) = __m0 * __m1;							\
  } جب (0)
#اگر_ختم
#تعریف_کرو smul_ppmm(ph, pl, m0, m1) \
  کرو {									\
    DItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("mulhd %0,%1,%2" : "=r" (ph) : "%r" (__m0), "r" (__m1));	\
    (pl) = __m0 * __m1;							\
  } جب (0)
#اگر_ختم /* 64-bit PowerPC.  */

#اگر defined (__pyr__) && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addw %5,%1\n\taddwc %3,%0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0"  ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "g" ((USItype)(bl)))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subw %5,%1\n\tsubwb %3,%0"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0" ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "g" ((USItype)(bl)))
/* This insn works on Pyramids with AP, XP, or MI CPUs, but not with SP.  */
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  ({اتحاد {UDItype __ll;						\
	   ساخت {USItype __h, __l;} __i;				\
	  } __x;							\
  __asm__ ("movw %1,%R0\n\tuemul %2,%0"					\
	   : "=&r" (__x.__ll)						\
	   : "g" ((USItype) (u)), "g" ((USItype)(v)));			\
  (w1) = __x.__i.__h; (w0) = __x.__i.__l;})
#اگر_ختم /* __pyr__ */

#اگر defined (__ibm032__) /* RT/ROMP */  && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("a %1,%5\n\tae %0,%3"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0"  ((USItype)(ah)), "r" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "r" ((USItype)(bl)))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("s %1,%5\n\tse %0,%3"					\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0" ((USItype)(ah)), "r" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "r" ((USItype)(bl)))
#تعریف_کرو smul_ppmm(ph, pl, m0, m1) \
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
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  کرو {									\
    اگر ((x) >= 0x10000)							\
      __asm__ ("clz	%0,%1"						\
	       : "=r" (گنتی) : "r" ((USItype)(x) >> 16));		\
    ورنہ								\
      {									\
	__asm__ ("clz	%0,%1"						\
		 : "=r" (گنتی) : "r" ((USItype)(x)));			\
	(گنتی) += 16;							\
      }									\
  } جب (0)
#اگر_ختم /* RT/ROMP */

#اگر defined (__riscv) && defined (__riscv_mul) && W_TYPE_سائز == 64
#تعریف_کرو umul_ppmm(ph, pl, u, v) \
  کرو {									\
    UDItype __u = (u), __v = (v);					\
    (pl) = __u * __v;							\
    __asm__ ("mulhu\t%0, %1, %2" : "=r" (ph) : "%r" (__u), "r" (__v));	\
  } جب (0)
#اگر_ختم

#اگر (defined (__SH2__) || defined (__SH3__) || defined (__SH4__)) && W_TYPE_سائز == 32
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("dmulu.l %2,%3\n\tsts macl,%1\n\tsts mach,%0"		\
	   : "=r" (w1), "=r" (w0) : "r" (u), "r" (v) : "macl", "mach")
#اگر_ختم

#اگر defined (__sparc__) && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addcc %r4,%5,%1\n\taddx %r2,%3,%0"				\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rJ" (ah), "rI" (bh),"%rJ" (al), "rI" (bl)			\
	   __CLOBBER_CC)
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subcc %r4,%5,%1\n\tsubx %r2,%3,%0"				\
	   : "=r" (sh), "=&r" (sl)					\
	   : "rJ" (ah), "rI" (bh), "rJ" (al), "rI" (bl)	\
	   __CLOBBER_CC)
/* FIXME: When gcc -mcpu=v9 is used on solaris, gcc/config/sol2-sld-64.h
   doesn't define anything to indicate that to us, it only sets __sparcv8. */
#اگر defined (__sparc_v9__) || defined (__sparcv9)
/* Perhaps we should use floating-point operations here?  */
#اگر 0
/* Triggers a bug making mpz/tests/t-gcd.c fail.
   Perhaps we simply need explicitly zero-extend the inputs?  */
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("mulx %2,%3,%%g1; srl %%g1,0,%1; srlx %%g1,32,%0" :		\
	   "=r" (w1), "=r" (w0) : "r" (u), "r" (v) : "g1")
#ورنہ
/* Use v8 umul until above bug is fixed.  */
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("umul %2,%3,%1;rd %%y,%0" : "=r" (w1), "=r" (w0) : "r" (u), "r" (v))
#اگر_ختم
/* Use a plain v8 divide for v9.  */
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
  کرو {									\
    USItype __q;							\
    __asm__ ("mov %1,%%y;nop;nop;nop;udiv %2,%3,%0"			\
	     : "=r" (__q) : "r" (n1), "r" (n0), "r" (d));		\
    (r) = (n0) - __q * (d);						\
    (q) = __q;								\
  } جب (0)
#ورنہ
#اگر defined (__sparc_v8__)   /* gcc normal */				\
  || defined (__sparcv8)     /* gcc solaris */				\
  || HAVE_HOST_CPU_supersparc
/* Don't match immediate range because, 1) it is not often useful,
   2) the 'I' flag thinks of the range as a 13 bit signed interval,
   while we want to match a 13 bit interval, sign extended to 32 bits,
   but INTERPRETED AS UNSIGNED.  */
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("umul %2,%3,%1;rd %%y,%0" : "=r" (w1), "=r" (w0) : "r" (u), "r" (v))

#اگر HAVE_HOST_CPU_supersparc
#ورنہ
/* Don't use this on SuperSPARC because its udiv only handles 53 bit
   dividends and will trap to the kernel for the rest. */
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
  کرو {									\
    USItype __q;							\
    __asm__ ("mov %1,%%y;nop;nop;nop;udiv %2,%3,%0"			\
	     : "=r" (__q) : "r" (n1), "r" (n0), "r" (d));		\
    (r) = (n0) - __q * (d);						\
    (q) = __q;								\
  } جب (0)
#اگر_ختم /* HAVE_HOST_CPU_supersparc */

#ورنہ /* ! __sparc_v8__ */
#اگر defined (__sparclite__)
/* This has hardware multiply but not divide.  It also has two additional
   instructions scan (ffs from high bit) and divscc.  */
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
  __asm__ ("umul %2,%3,%1;rd %%y,%0" : "=r" (w1), "=r" (w0) : "r" (u), "r" (v))
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
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
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  __asm__ ("scan %1,1,%0" : "=r" (گنتی) : "r" (x))
/* Early sparclites return 63 for an argument of 0, but they warn that future
   implementations might change this.  Therefore, leave COUNT_LEADING_ZEROS_0
   undefined.  */
#اگر_ختم /* __sparclite__ */
#اگر_ختم /* __sparc_v8__ */
#اگر_ختم /* __sparc_v9__ */
/* Default to sparc v7 versions of umul_ppmm and udiv_qrnnd.  */
#اگر_غیر_تعریف_شدہ umul_ppmm
#تعریف_کرو umul_ppmm(w1, w0, u, v) \
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
#اگر_ختم
#اگر_غیر_تعریف_شدہ udiv_qrnnd
#اگر_غیر_تعریف_شدہ LONGLONG_STANDALONE
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d) \
  کرو { UWtype __r;							\
    (q) = __MPN(udiv_qrnnd) (&__r, (n1), (n0), (d));			\
    (r) = __r;								\
  } جب (0)
خارجی UWtype __MPN(udiv_qrnnd) (UWtype *, UWtype, UWtype, UWtype);
#اگر_ختم /* LONGLONG_STANDALONE */
#اگر_ختم /* udiv_qrnnd */
#اگر_ختم /* __sparc__ */

#اگر defined (__sparc__) && W_TYPE_سائز == 64
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ (								\
       "addcc	%r4,%5,%1\n"						\
      "	addccc	%r6,%7,%%g0\n"						\
      "	addc	%r2,%3,%0"						\
       : "=r" (sh), "=&r" (sl)						\
       : "rJ"  ((UDItype)(ah)), "rI" ((UDItype)(bh)),			\
	 "%rJ" ((UDItype)(al)), "rI" ((UDItype)(bl)),			\
	 "%rJ" ((UDItype)(al) >> 32), "rI" ((UDItype)(bl) >> 32)	\
	   __CLOBBER_CC)
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ (								\
       "subcc	%r4,%5,%1\n"						\
      "	subccc	%r6,%7,%%g0\n"						\
      "	subc	%r2,%3,%0"						\
       : "=r" (sh), "=&r" (sl)						\
       : "rJ" ((UDItype)(ah)), "rI" ((UDItype)(bh)),			\
	 "rJ" ((UDItype)(al)), "rI" ((UDItype)(bl)),			\
	 "rJ" ((UDItype)(al) >> 32), "rI" ((UDItype)(bl) >> 32)		\
	   __CLOBBER_CC)
#اگر __VIS__ >= 0x300
#تعریف_منسوخ جوڑو_ssaaaa
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ (								\
       "addcc	%r4, %5, %1\n"						\
      "	addxc	%r2, %r3, %0"						\
	  : "=r" (sh), "=&r" (sl)					\
       : "rJ"  ((UDItype)(ah)), "rJ" ((UDItype)(bh)),			\
	 "%rJ" ((UDItype)(al)), "rI" ((UDItype)(bl)) __CLOBBER_CC)
#تعریف_کرو umul_ppmm(ph, pl, m0, m1) \
  کرو {									\
    UDItype __m0 = (m0), __m1 = (m1);					\
    (pl) = __m0 * __m1;							\
    __asm__ ("umulxhi\t%2, %1, %0"					\
	     : "=r" (ph)						\
	     : "%r" (__m0), "r" (__m1));				\
  } جب (0)
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  __asm__ ("lzd\t%1,%0" : "=r" (گنتی) : "r" (x))
/* Needed by count_leading_zeros_32 in sparc64.h.  */
#تعریف_کرو گنتی_LEADING_ZEROS_NEED_CLZ_TAB
#اگر_ختم
#اگر_ختم

#اگر (defined (__vax) || defined (__vax__)) && W_TYPE_سائز == 32
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("addl2 %5,%1\n\tadwc %3,%0"					\
	   : "=g" (sh), "=&g" (sl)					\
	   : "0"  ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "%1" ((USItype)(al)), "g" ((USItype)(bl)))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("subl2 %5,%1\n\tsbwc %3,%0"					\
	   : "=g" (sh), "=&g" (sl)					\
	   : "0" ((USItype)(ah)), "g" ((USItype)(bh)),			\
	     "1" ((USItype)(al)), "g" ((USItype)(bl)))
#تعریف_کرو smul_ppmm(xh, xl, m0, m1) \
  کرو {									\
    اتحاد {UDItype __ll;						\
	   ساخت {USItype __l, __h;} __i;				\
	  } __x;							\
    USItype __m0 = (m0), __m1 = (m1);					\
    __asm__ ("emul %1,%2,$0,%0"						\
	     : "=g" (__x.__ll) : "g" (__m0), "g" (__m1));		\
    (xh) = __x.__i.__h; (xl) = __x.__i.__l;				\
  } جب (0)
#تعریف_کرو sdiv_qrnnd(q, r, n1, n0, d) \
  کرو {									\
    اتحاد {DItype __ll;							\
	   ساخت {SItype __l, __h;} __i;				\
	  } __x;							\
    __x.__i.__h = n1; __x.__i.__l = n0;					\
    __asm__ ("ediv %3,%2,%0,%1"						\
	     : "=g" (q), "=g" (r) : "g" (__x.__ll), "g" (d));		\
  } جب (0)
#اگر 0
/* FIXME: This instruction appears to be unimplemented on some systems (vax
   8800 maybe). */
#تعریف_کرو گنتی_trailing_zeros(گنتی,x)					\
  کرو {									\
    __asm__ ("ffs 0, 31, %1, %0"					\
	     : "=g" (گنتی)						\
	     : "g" ((USItype) (x)));					\
  } جب (0)
#اگر_ختم
#اگر_ختم /* vax */

#اگر defined (__z8000__) && W_TYPE_سائز == 16
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  __asm__ ("add	%H1,%H5\n\tadc	%H0,%H3"				\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0"  ((غیرعلامتی صحیح)(ah)), "r" ((غیرعلامتی صحیح)(bh)),	\
	     "%1" ((غیرعلامتی صحیح)(al)), "rQR" ((غیرعلامتی صحیح)(bl)))
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  __asm__ ("sub	%H1,%H5\n\tsbc	%H0,%H3"				\
	   : "=r" (sh), "=&r" (sl)					\
	   : "0" ((غیرعلامتی صحیح)(ah)), "r" ((غیرعلامتی صحیح)(bh)),	\
	     "1" ((غیرعلامتی صحیح)(al)), "rQR" ((غیرعلامتی صحیح)(bl)))
#تعریف_کرو umul_ppmm(xh, xl, m0, m1) \
  کرو {									\
    اتحاد {طویل صحیح __ll;						\
	   ساخت {غیرعلامتی صحیح __h, __l;} __i;				\
	  } __x;							\
    غیرعلامتی صحیح __m0 = (m0), __m1 = (m1);				\
    __asm__ ("mult	%S0,%H3"					\
	     : "=r" (__x.__i.__h), "=r" (__x.__i.__l)			\
	     : "%1" (m0), "rQR" (m1));					\
    (xh) = __x.__i.__h; (xl) = __x.__i.__l;				\
    (xh) += ((((علامتی صحیح) __m0 >> 15) & __m1)				\
	     + (((علامتی صحیح) __m1 >> 15) & __m0));			\
  } جب (0)
#اگر_ختم /* __z8000__ */

#اگر_ختم /* __GNUC__ */

#اگر_ختم /* NO_ASM */


/* FIXME: "sidi" here is highly doubtful, should sometimes be "diti".  */
#اگر !defined (umul_ppmm) && defined (__umulsidi3)
#تعریف_کرو umul_ppmm(ph, pl, m0, m1) \
  کرو {									\
    UDWtype __ll = __umulsidi3 (m0, m1);				\
    ph = (UWtype) (__ll >> W_TYPE_سائز);				\
    pl = (UWtype) __ll;							\
  } جب (0)
#اگر_ختم

#اگر !defined (__umulsidi3)
#تعریف_کرو __umulsidi3(u, v) \
  ({UWtype __hi, __lo;							\
    umul_ppmm (__hi, __lo, u, v);					\
    ((UDWtype) __hi << W_TYPE_سائز) | __lo; })
#اگر_ختم


#اگر defined (__cplusplus)
#تعریف_کرو __longlong_h_C "C"
#ورنہ
#تعریف_کرو __longlong_h_C
#اگر_ختم

/* Use mpn_umul_ppmm or mpn_udiv_qrnnd functions, if they exist.  The "_r"
   forms have "reversed" arguments, meaning the pointer is last, which
   sometimes allows better parameter passing, in particular on 64-bit
   hppa. */

#تعریف_کرو mpn_umul_ppmm  __MPN(umul_ppmm)
خارجی __longlong_h_C UWtype mpn_umul_ppmm (UWtype *, UWtype, UWtype);

#اگر ! defined (umul_ppmm) && HAVE_NATIVE_mpn_umul_ppmm  \
  && ! defined (LONGLONG_STANDALONE)
#تعریف_کرو umul_ppmm(wh, wl, u, v)						\
  کرو {									\
    UWtype __umul_ppmm__p0;						\
    (wh) = mpn_umul_ppmm (&__umul_ppmm__p0, (UWtype) (u), (UWtype) (v));\
    (wl) = __umul_ppmm__p0;						\
  } جب (0)
#اگر_ختم

#تعریف_کرو mpn_umul_ppmm_r  __MPN(umul_ppmm_r)
خارجی __longlong_h_C UWtype mpn_umul_ppmm_r (UWtype, UWtype, UWtype *);

#اگر ! defined (umul_ppmm) && HAVE_NATIVE_mpn_umul_ppmm_r	\
  && ! defined (LONGLONG_STANDALONE)
#تعریف_کرو umul_ppmm(wh, wl, u, v)						\
  کرو {									\
    UWtype __umul_p0;							\
    (wh) = mpn_umul_ppmm_r ((UWtype) (u), (UWtype) (v), &__umul_p0);	\
    (wl) = __umul_p0;							\
  } جب (0)
#اگر_ختم

#تعریف_کرو mpn_udiv_qrnnd  __MPN(udiv_qrnnd)
خارجی __longlong_h_C UWtype mpn_udiv_qrnnd (UWtype *, UWtype, UWtype, UWtype);

#اگر ! defined (udiv_qrnnd) && HAVE_NATIVE_mpn_udiv_qrnnd	\
  && ! defined (LONGLONG_STANDALONE)
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d)					\
  کرو {									\
    UWtype __udiv_qrnnd_r;						\
    (q) = mpn_udiv_qrnnd (&__udiv_qrnnd_r,				\
			  (UWtype) (n1), (UWtype) (n0), (UWtype) d);	\
    (r) = __udiv_qrnnd_r;						\
  } جب (0)
#اگر_ختم

#تعریف_کرو mpn_udiv_qrnnd_r  __MPN(udiv_qrnnd_r)
خارجی __longlong_h_C UWtype mpn_udiv_qrnnd_r (UWtype, UWtype, UWtype, UWtype *);

#اگر ! defined (udiv_qrnnd) && HAVE_NATIVE_mpn_udiv_qrnnd_r	\
  && ! defined (LONGLONG_STANDALONE)
#تعریف_کرو udiv_qrnnd(q, r, n1, n0, d)					\
  کرو {									\
    UWtype __udiv_qrnnd_r;						\
    (q) = mpn_udiv_qrnnd_r ((UWtype) (n1), (UWtype) (n0), (UWtype) d,	\
			    &__udiv_qrnnd_r);				\
    (r) = __udiv_qrnnd_r;						\
  } جب (0)
#اگر_ختم


/* If this machine has no inline assembler, use C macros.  */

#اگر !defined (جوڑو_ssaaaa)
#تعریف_کرو جوڑو_ssaaaa(sh, sl, ah, al, bh, bl) \
  کرو {									\
    UWtype __x;								\
    UWtype __al = (al);							\
    UWtype __bl = (bl);							\
    __x = __al + __bl;							\
    (sh) = (ah) + (bh) + (__x < __al);					\
    (sl) = __x;								\
  } جب (0)
#اگر_ختم

#اگر !defined (sub_ddmmss)
#تعریف_کرو sub_ddmmss(sh, sl, ah, al, bh, bl) \
  کرو {									\
    UWtype __x;								\
    UWtype __al = (al);							\
    UWtype __bl = (bl);							\
    __x = __al - __bl;							\
    (sh) = (ah) - (bh) - (__al < __bl);					\
    (sl) = __x;								\
  } جب (0)
#اگر_ختم

/* If we lack umul_ppmm but have smul_ppmm, define umul_ppmm in terms of
   smul_ppmm.  */
#اگر !defined (umul_ppmm) && defined (smul_ppmm)
#تعریف_کرو umul_ppmm(w1, w0, u, v)						\
  کرو {									\
    UWtype __w1;							\
    UWtype __xm0 = (u), __xm1 = (v);					\
    smul_ppmm (__w1, w0, __xm0, __xm1);					\
    (w1) = __w1 + (-(__xm0 >> (W_TYPE_سائز - 1)) & __xm1)		\
		+ (-(__xm1 >> (W_TYPE_سائز - 1)) & __xm0);		\
  } جب (0)
#اگر_ختم

/* If we still don't have umul_ppmm, define it using plain C.

   For reference, when this code is used for squaring (ie. u and v identical
   expressions), gcc recognises __x1 and __x2 are the same and generates 3
   multiplies, not 4.  The subsequent additions could be optimized a bit,
   but the only place GMP currently uses such a square is mpn_sqr_basecase,
   and chips obliged to use this generic C umul will have plenty of worse
   performance problems than a couple of extra instructions on the diagonal
   of sqr_basecase.  */

#اگر !defined (umul_ppmm)
#تعریف_کرو umul_ppmm(w1, w0, u, v)						\
  کرو {									\
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
    اگر (__x1 < __x2)		/* did we get it? */			\
      __x3 += __ll_B;		/* yes, add it in the proper pos. */	\
									\
    (w1) = __x3 + __ll_highpart (__x1);					\
    (w0) = (__x1 << W_TYPE_سائز/2) + __ll_lowpart (__x0);		\
  } جب (0)
#اگر_ختم

/* If we don't have smul_ppmm, define it using umul_ppmm (which surely will
   exist in one form or another.  */
#اگر !defined (smul_ppmm)
#تعریف_کرو smul_ppmm(w1, w0, u, v)						\
  کرو {									\
    UWtype __w1;							\
    UWtype __xm0 = (u), __xm1 = (v);					\
    umul_ppmm (__w1, w0, __xm0, __xm1);					\
    (w1) = __w1 - (-(__xm0 >> (W_TYPE_سائز - 1)) & __xm1)		\
		- (-(__xm1 >> (W_TYPE_سائز - 1)) & __xm0);		\
  } جب (0)
#اگر_ختم

/* Define this unconditionally, so it can be used for debugging.  */
#تعریف_کرو __udiv_qrnnd_c(q, r, n1, n0, d) \
  کرو {									\
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
    اگر (__r1 < __m)							\
      {									\
	__q1--, __r1 += (d);						\
	اگر (__r1 >= (d)) /* i.e. we didn't get carry when adding to __r1 */\
	  اگر (__r1 < __m)						\
	    __q1--, __r1 += (d);					\
      }									\
    __r1 -= __m;							\
									\
    __q0 = __r1 / __d1;							\
    __r0 = __r1  - __q0 * __d1;						\
    __m = __q0 * __d0;							\
    __r0 = __r0 * __ll_B | __ll_lowpart (n0);				\
    اگر (__r0 < __m)							\
      {									\
	__q0--, __r0 += (d);						\
	اگر (__r0 >= (d))						\
	  اگر (__r0 < __m)						\
	    __q0--, __r0 += (d);					\
      }									\
    __r0 -= __m;							\
									\
    (q) = __q1 * __ll_B | __q0;						\
    (r) = __r0;								\
  } جب (0)

/* If the processor has no udiv_qrnnd but sdiv_qrnnd, go through
   __udiv_w_sdiv (defined in libgcc or elsewhere).  */
#اگر !defined (udiv_qrnnd) && defined (sdiv_qrnnd) \
  && ! defined (LONGLONG_STANDALONE)
#تعریف_کرو udiv_qrnnd(q, r, nh, nl, d) \
  کرو {									\
    UWtype __r;								\
    (q) = __MPN(udiv_w_sdiv) (&__r, nh, nl, d);				\
    (r) = __r;								\
  } جب (0)
__GMP_DECLSPEC UWtype __MPN(udiv_w_sdiv) (UWtype *, UWtype, UWtype, UWtype);
#اگر_ختم

/* If udiv_qrnnd was not defined for this processor, use __udiv_qrnnd_c.  */
#اگر !defined (udiv_qrnnd)
#تعریف_کرو UDIV_NEEDS_NORMALIZATION 1
#تعریف_کرو udiv_qrnnd __udiv_qrnnd_c
#اگر_ختم

#اگر !defined (گنتی_leading_zeros)
#تعریف_کرو گنتی_leading_zeros(گنتی, x) \
  کرو {									\
    UWtype __xr = (x);							\
    UWtype __a;								\
									\
    اگر (W_TYPE_سائز == 32)						\
      {									\
	__a = __xr < ((UWtype) 1 << 2*__BITS4)				\
	  ? (__xr < ((UWtype) 1 << __BITS4) ? 1 : __BITS4 + 1)		\
	  : (__xr < ((UWtype) 1 << 3*__BITS4) ? 2*__BITS4 + 1		\
	  : 3*__BITS4 + 1);						\
      }									\
    ورنہ								\
      {									\
	برائے (__a = W_TYPE_سائز - 8; __a > 0; __a -= 8)			\
	  اگر (((__xr >> __a) & 0xff) != 0)				\
	    توڑو;							\
	++__a;								\
      }									\
									\
    (گنتی) = W_TYPE_سائز + 1 - __a - __clz_tab[__xr >> __a];		\
  } جب (0)
/* This version gives a well-defined value for zero. */
#تعریف_کرو گنتی_LEADING_ZEROS_0 (W_TYPE_سائز - 1)
#تعریف_کرو گنتی_LEADING_ZEROS_NEED_CLZ_TAB
#تعریف_کرو گنتی_LEADING_ZEROS_SLOW
#اگر_ختم

/* clz_tab needed by mpn/x86/pentium/mod_1.asm in a fat binary */
#اگر HAVE_HOST_CPU_FAMILY_x86 && WANT_FAT_ثنائی
#تعریف_کرو گنتی_LEADING_ZEROS_NEED_CLZ_TAB
#اگر_ختم

#اگر_تعریف_شدہ گنتی_LEADING_ZEROS_NEED_CLZ_TAB
خارجی ثابت غیرعلامتی حرف __GMP_DECLSPEC __clz_tab[129];
#اگر_ختم

#اگر !defined (گنتی_trailing_zeros)
#اگر !defined (گنتی_LEADING_ZEROS_SLOW)
/* Define count_trailing_zeros using an asm count_leading_zeros.  */
#تعریف_کرو گنتی_trailing_zeros(گنتی, x)					\
  کرو {									\
    UWtype __ctz_x = (x);						\
    UWtype __ctz_c;							\
    ASSERT (__ctz_x != 0);						\
    گنتی_leading_zeros (__ctz_c, __ctz_x & -__ctz_x);			\
    (گنتی) = W_TYPE_سائز - 1 - __ctz_c;				\
  } جب (0)
#ورنہ
/* Define count_trailing_zeros in plain C, assuming small counts are common.
   We use clz_tab without ado, since the C count_leading_zeros above will have
   pulled it in.  */
#تعریف_کرو گنتی_trailing_zeros(گنتی, x)					\
  کرو {									\
    UWtype __ctz_x = (x);						\
    صحیح __ctz_c;							\
									\
    اگر (LIKELY ((__ctz_x & 0xff) != 0))					\
      (گنتی) = __clz_tab[__ctz_x & -__ctz_x] - 2;			\
    ورنہ								\
      {									\
	برائے (__ctz_c = 8 - 2; __ctz_c < W_TYPE_سائز - 2; __ctz_c += 8)	\
	  {								\
	    __ctz_x >>= 8;						\
	    اگر (LIKELY ((__ctz_x & 0xff) != 0))				\
	      توڑو;							\
	  }								\
									\
	(گنتی) = __ctz_c + __clz_tab[__ctz_x & -__ctz_x];		\
      }									\
  } جب (0)
#اگر_ختم
#اگر_ختم

#اگر_غیر_تعریف_شدہ UDIV_NEEDS_NORMALIZATION
#تعریف_کرو UDIV_NEEDS_NORMALIZATION 0
#اگر_ختم

/* Whether udiv_qrnnd is actually implemented with udiv_qrnnd_preinv, and
   that hence the latter should always be used.  */
#اگر_غیر_تعریف_شدہ UDIV_PREINV_ALWAYS
#تعریف_کرو UDIV_PREINV_ALWAYS 0
#اگر_ختم
