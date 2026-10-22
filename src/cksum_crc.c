/* cksum_crc -- calculate and print POSIX checksums and sizes of files
   Copyright (C) 1992-2026 Free Software Foundation, Inc.

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

/* Written by Q. Frank Xia, qx@math.columbia.edu.
   Cosmetic changes and reorganization by David MacKenzie, djm@gnu.ai.mit.edu.

  The code segment between "#ifdef CRCTAB" and "#else" is the code
  which generates crctab.c

  This software is compatible with neither the System V nor the BSD
  'sum' program.  It is supposed to conform to POSIX, except perhaps
  for foreign language support.  Any inconsistency with the standard
  (other than foreign language support) is a bug.  */

/* This include must be at the top of the file to satisfy
   sc_require_config_h_first.  */
#اگر_تعریف_نشده CRCTAB
# include <config.h>
#پایان_اگر

#اگر_تعریف_شده CRCTAB

# include <stdio.h>

# define بیت_2(x)	(1u << (x))
# define SBIT	بیت_2 (31)

/* The generating polynomial is

          32   26   23   22   16   12   11   10   8   7   5   4   2   1
    G(X)=X  + X  + X  + X  + X  + X  + X  + X  + X + X + X + X + X + X + 1

  The i bit in GEN is set if X^i is a summand of G(X) except X^32.  */

# define GEN	(بیت_2 (26) | بیت_2 (23) | بیت_2 (22) | بیت_2 (16) | بیت_2 (12) \
                 | بیت_2 (11) | بیت_2 (10) | بیت_2 (8) | بیت_2 (7) | بیت_2 (5) \
                 | بیت_2 (4) | بیت_2 (2) | بیت_2 (1) | بیت_2 (0))

ساكن بیعلامت صحيح r[8];

ساكن فراغ
fill_r (فراغ)
{
  r[0] = GEN;
  لكل (صحيح i = 1; i < 8; i++)
    r[i] = (r[i - 1] << 1) ^ ((r[i - 1] & SBIT) ? GEN : 0);
}

ساكن بیعلامت صحيح
بررسی_چرخهای_remainder (صحيح m)
{
  بیعلامت صحيح rem = 0;

  لكل (صحيح i = 0; i < 8; i++)
    إذا (بیت_2 (i) & m)
      rem ^= r[i];

  أرجع rem & 0xFFFFFFFF;	/* Make it run on 64-bit machine.  */
}

صحيح
اصلی (فراغ)
{
  ساكن بیعلامت صحيح crctab[8][256];

  fill_r ();

  لكل (صحيح i = 0; i < 256; i++)
    crctab[0][i] = بررسی_چرخهای_remainder (i);

  /* CRC(0x11 0x22 0x33 0x44)
     is equal to
     CRC(0x11 0x00 0x00 0x00) XOR CRC(0x22 0x00 0x00) XOR
     CRC(0x33 0x00) XOR CRC(0x44)
     We precompute the CRC values for the offset values into
     separate CRC tables. We can then use them to speed up
     CRC calculation by processing multiple bytes at the time. */
  لكل (صحيح i = 0; i < 256; i++)
    {
      بیعلامت صحيح بررسی_چرخهای = 0;

      بررسی_چرخهای = (بررسی_چرخهای << 8) ^ crctab[0][((بررسی_چرخهای >> 24) ^ i) & 0xFF];
      لكل (صحيح جابه_جایی = 1; جابه_جایی < 8; جابه_جایی++)
        {
          بررسی_چرخهای = (بررسی_چرخهای << 8) ^ crctab[0][((بررسی_چرخهای >> 24) ^ 0) & 0xFF];
          crctab[جابه_جایی][i] = بررسی_چرخهای & 0xFFFFFFFF;
        }
    }

  چاپ_کن ("#include <config.h>\n");
  چاپ_کن ("#include \"cksum_crc.h\"\n");
  چاپ_کن ("\n");
  چاپ_کن ("uint_fast32_t const crctab[8][256] = {\n");
  لكل (صحيح y = 0; y < 8; y++)
    {
      چاپ_کن ("{\n  0x%08x", crctab[y][0]);
      لكل (صحيح i = 0; i < 51; i++)
        {
          چاپ_کن (",\n  0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x",
                  crctab[y][i * 5 + 1], crctab[y][i * 5 + 2],
                  crctab[y][i * 5 + 3], crctab[y][i * 5 + 4],
                  crctab[y][i * 5 + 5]);
        }
        چاپ_کن ("\n},\n");
    }
  چاپ_کن ("};\n");
}

#وإلا /* !CRCTAB */

# include "cksum_crc.h"
# include <sys/types.h>
# include <endian.h>
# include "system.h"

# ifdef USE_VMULL_بررسی_چرخهای_32
#  include <sys/auxv.h>
#  include <asm/hwcap.h>
# endif

# include "crc.h"
# include "cpu-supports.h"

/* Number of bytes to read at once.  */
# define BUFLEN (1 << 16)

تعريفنوع bool (*cksum_fp_t) (FILE *, uint_fast32_t *, intmax_t *);

ساكن cksum_fp_t
pclmul_supported (فراغ)
{
# إذا USE_PCLMUL_بررسی_چرخهای_32 || GL_بررسی_چرخهای_X86_64_PCLMUL
  bool pclmul_enabled = (cpu_supports ("avx")
                         && cpu_supports ("pclmul"));
  إذا (cksum_debug)
    error (0, 0, "%s",
           (pclmul_enabled
            ? _("using pclmul hardware support")
            : _("pclmul support not detected")));
#  إذا USE_PCLMUL_بررسی_چرخهای_32
  إذا (pclmul_enabled)
    أرجع cksum_pclmul;
#  endif
# endif

  أرجع عدم;
}

ساكن cksum_fp_t
avx2_supported (فراغ)
{
  /* AVX512 processors will not set vpclmulqdq unless they support
     the avx512 version, but it implies that the avx2 version
     is supported  */
# إذا USE_AVX2_بررسی_چرخهای_32
  bool avx2_enabled = (cpu_supports ("avx2")
                       && cpu_supports ("vpclmulqdq"));
  إذا (cksum_debug)
    error (0, 0, "%s",
           (avx2_enabled
            ? _("using avx2 hardware support")
            : _("avx2 support not detected")));
  إذا (avx2_enabled)
    أرجع cksum_avx2;
# endif

  أرجع عدم;
}

ساكن cksum_fp_t
avx512_supported (فراغ)
{
  /* vpclmulqdq for multiplication
     mavx512f for most of the avx512 functions we're using
     mavx512bw for byte swapping  */
# إذا USE_AVX512_بررسی_چرخهای_32
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw")
                         && cpu_supports ("vpclmulqdq"));

  إذا (cksum_debug)
    error (0, 0, "%s",
           (avx512_enabled
            ? _("using avx512 hardware support")
            : _("avx512 support not detected")));
  إذا (avx512_enabled)
    أرجع cksum_avx512;
# endif

  أرجع عدم;
}

ساكن cksum_fp_t
vmull_supported (فراغ)
{
  /* vmull for multiplication  */
# إذا USE_VMULL_بررسی_چرخهای_32
  bool vmull_enabled = (cpu_may_support ("pmull")
                        && (getauxval (AT_HWCAP) & HWCAP_PMULL) > 0);
  إذا (cksum_debug)
    error (0, 0, "%s",
           (vmull_enabled
            ? _("using vmull hardware support")
            : _("vmull support not detected")));
  إذا (vmull_enabled)
    أرجع cksum_vmull;
# endif

  أرجع عدم;
}

ساكن bool
cksum_slice8 (FILE *fp, uint_fast32_t *بررسی_چرخهای_بیرون, intmax_t *درازی_بیرون)
{
  uint32_t buf[BUFLEN / حجم (uint32_t)];
  uint_fast32_t بررسی_چرخهای = 0;
  intmax_t درازی = 0;
  size_t بایتها_read;

  إذا (!fp || !بررسی_چرخهای_بیرون || !درازی_بیرون)
    أرجع false;

  طالما ((بایتها_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      uint32_t *datap;

      إذا (ckd_جمع_کن (&درازی, درازی, بایتها_read))
        {
          errno = EOVERFLOW;
          أرجع false;
        }

      /* Process multiples of 8 bytes */
      datap = buf;
      طالما (بایتها_read >= 8)
        {
          uint32_t اولی = *datap++, دومی = *datap++;
          بررسی_چرخهای ^= htobe32 (اولی);
          دومی = htobe32 (دومی);
          بررسی_چرخهای = (crctab[7][(بررسی_چرخهای >> 24) & 0xFF]
                 ^ crctab[6][(بررسی_چرخهای >> 16) & 0xFF]
                 ^ crctab[5][(بررسی_چرخهای >> 8) & 0xFF]
                 ^ crctab[4][(بررسی_چرخهای) & 0xFF]
                 ^ crctab[3][(دومی >> 24) & 0xFF]
                 ^ crctab[2][(دومی >> 16) & 0xFF]
                 ^ crctab[1][(دومی >> 8) & 0xFF]
                 ^ crctab[0][(دومی) & 0xFF]);
          بایتها_read -= 8;
        }

      /* And finish up last 0-7 bytes in a byte by byte fashion */
      بیعلامت حرف *cp = (بیعلامت حرف *)datap;
      طالما (بایتها_read--)
        بررسی_چرخهای = (بررسی_چرخهای << 8) ^ crctab[0][((بررسی_چرخهای >> 24) ^ *cp++) & 0xFF];
      إذا (feof (fp))
        اكسر;
    }

  *بررسی_چرخهای_بیرون = بررسی_چرخهای;
  *درازی_بیرون = درازی;

  أرجع !ferror (fp);
}

/* Calculate the checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحيح
بررسی_چرخهای_جمع_جریان (FILE *جریان, فراغ *resstream, intmax_t *درازی)
{
  intmax_t مجموع_بایتها = 0;
  uint_fast32_t بررسی_چرخهای = 0;

  ساكن cksum_fp_t cksum_fp;
  إذا (! cksum_fp)
    cksum_fp = avx512_supported ();
  إذا (! cksum_fp)
    cksum_fp = avx2_supported ();
  إذا (! cksum_fp)
    cksum_fp = pclmul_supported ();
  إذا (! cksum_fp)
    cksum_fp = vmull_supported ();
  إذا (! cksum_fp)
    cksum_fp = cksum_slice8;

  إذا (! cksum_fp (جریان, &بررسی_چرخهای, &مجموع_بایتها))
    أرجع -1;

  *درازی = مجموع_بایتها;

  لكل (; مجموع_بایتها; مجموع_بایتها >>= 8)
    بررسی_چرخهای = (بررسی_چرخهای << 8) ^ crctab[0][((بررسی_چرخهای >> 24) ^ مجموع_بایتها) & 0xFF];
  بررسی_چرخهای = ~بررسی_چرخهای & 0xFFFFFFFF;

  بیعلامت صحيح بررسی_چرخهای_بیرون = بررسی_چرخهای;
  کپی_حافظه (resstream, &بررسی_چرخهای_بیرون, حجم بررسی_چرخهای_بیرون);

  أرجع 0;
}

/* Calculate the crc32b checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحيح
crc32b_جمع_جریان (FILE *جریان, فراغ *resstream, intmax_t *reslen)
{
  uint32_t buf[BUFLEN / حجم (uint32_t)];
  uint32_t بررسی_چرخهای = 0;
  intmax_t len = 0;
  size_t بایتها_read;

  إذا (!جریان || !resstream || !reslen)
    أرجع -1;

# إذا GL_بررسی_چرخهای_X86_64_PCLMUL
  إذا (cksum_debug)
    (فراغ) pclmul_supported ();
# endif

  طالما ((بایتها_read = fread (buf, 1, BUFLEN, جریان)) > 0)
    {
      إذا (ckd_جمع_کن (&len, len, بایتها_read))
        {
          errno = EOVERFLOW;
          أرجع -1;
        }

      بررسی_چرخهای = بررسی_چرخهای_32_update (بررسی_چرخهای, (حرف ثابت *)buf, بایتها_read);

      إذا (feof (جریان))
        اكسر;
    }

  بیعلامت صحيح بررسی_چرخهای_بیرون = بررسی_چرخهای;
  کپی_حافظه (resstream, &بررسی_چرخهای_بیرون, حجم بررسی_چرخهای_بیرون);

  *reslen = len;

  أرجع ferror (جریان) ? -1 : 0;
}

/* Print the checksum and size to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_بررسی_چرخهای (حرف ثابت *file, MAYBE_استفادهنشده صحيح دودویی_file,
            فراغ ثابت *digest, bool raw, MAYBE_استفادهنشده bool tagged,
            بیعلامت حرف delim, bool آرگومانها, intmax_t درازی)
{
  إذا (raw)
    {
      /* Output in network byte order (big endian).  */
      uint32_t بیرون_int = htobe32 (*(uint32_t *)digest);
      fwrite (&بیرون_int, 1, 32/8, stdout);
      أرجع;
    }

  چاپ_کن ("%u %jd", *(بیعلامت صحيح *)digest, درازی);
  إذا (آرگومانها)
    چاپ_کن (" %s", file);
  حرف_چاپ_کن (delim);
}

#پایان_اگر /* !CRCTAB */
