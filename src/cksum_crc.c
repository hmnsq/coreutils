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
#جے_بنا_تعریف CRCTAB
# include <config.h>
#جے_انت

#جے_تعریف_شدہ CRCTAB

# include <stdio.h>

# define ثنائی_ہندسہ_2(x)	(1u << (x))
# define SBIT	ثنائی_ہندسہ_2 (31)

/* The generating polynomial is

          32   26   23   22   16   12   11   10   8   7   5   4   2   1
    G(X)=X  + X  + X  + X  + X  + X  + X  + X  + X + X + X + X + X + X + 1

  The i bit in GEN is set if X^i is a summand of G(X) except X^32.  */

# define GEN	(ثنائی_ہندسہ_2 (26) | ثنائی_ہندسہ_2 (23) | ثنائی_ہندسہ_2 (22) | ثنائی_ہندسہ_2 (16) | ثنائی_ہندسہ_2 (12) \
                 | ثنائی_ہندسہ_2 (11) | ثنائی_ہندسہ_2 (10) | ثنائی_ہندسہ_2 (8) | ثنائی_ہندسہ_2 (7) | ثنائی_ہندسہ_2 (5) \
                 | ثنائی_ہندسہ_2 (4) | ثنائی_ہندسہ_2 (2) | ثنائی_ہندسہ_2 (1) | ثنائی_ہندسہ_2 (0))

ساكن بنانشانی صحيح r[8];

ساكن فراغ
fill_r (فراغ)
{
  r[0] = GEN;
  لكل (صحيح i = 1; i < 8; i++)
    r[i] = (r[i - 1] << 1) ^ ((r[i - 1] & SBIT) ? GEN : 0);
}

ساكن بنانشانی صحيح
چکری_جانچ_remainder (صحيح m)
{
  بنانشانی صحيح rem = 0;

  لكل (صحيح i = 0; i < 8; i++)
    إذا (ثنائی_ہندسہ_2 (i) & m)
      rem ^= r[i];

  أرجع rem & 0xFFFFFFFF;	/* Make it run on 64-bit machine.  */
}

صحيح
مکھ (فراغ)
{
  ساكن بنانشانی صحيح crctab[8][256];

  fill_r ();

  لكل (صحيح i = 0; i < 256; i++)
    crctab[0][i] = چکری_جانچ_remainder (i);

  /* CRC(0x11 0x22 0x33 0x44)
     is equal to
     CRC(0x11 0x00 0x00 0x00) XOR CRC(0x22 0x00 0x00) XOR
     CRC(0x33 0x00) XOR CRC(0x44)
     We precompute the CRC values for the offset values into
     separate CRC tables. We can then use them to speed up
     CRC calculation by processing multiple bytes at the time. */
  لكل (صحيح i = 0; i < 256; i++)
    {
      بنانشانی صحيح چکری_جانچ = 0;

      چکری_جانچ = (چکری_جانچ << 8) ^ crctab[0][((چکری_جانچ >> 24) ^ i) & 0xFF];
      لكل (صحيح وتھ = 1; وتھ < 8; وتھ++)
        {
          چکری_جانچ = (چکری_جانچ << 8) ^ crctab[0][((چکری_جانچ >> 24) ^ 0) & 0xFF];
          crctab[وتھ][i] = چکری_جانچ & 0xFFFFFFFF;
        }
    }

  لکھو ("#include <config.h>\n");
  لکھو ("#include \"cksum_crc.h\"\n");
  لکھو ("\n");
  لکھو ("uint_fast32_t const crctab[8][256] = {\n");
  لكل (صحيح y = 0; y < 8; y++)
    {
      لکھو ("{\n  0x%08x", crctab[y][0]);
      لكل (صحيح i = 0; i < 51; i++)
        {
          لکھو (",\n  0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x",
                  crctab[y][i * 5 + 1], crctab[y][i * 5 + 2],
                  crctab[y][i * 5 + 3], crctab[y][i * 5 + 4],
                  crctab[y][i * 5 + 5]);
        }
        لکھو ("\n},\n");
    }
  لکھو ("};\n");
}

#وإلا /* !CRCTAB */

# include "cksum_crc.h"
# include <sys/types.h>
# include <endian.h>
# include "system.h"

# ifdef USE_VMULL_چکری_جانچ_32
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
# إذا USE_PCLMUL_چکری_جانچ_32 || GL_چکری_جانچ_X86_64_PCLMUL
  bool pclmul_enabled = (cpu_supports ("avx")
                         && cpu_supports ("pclmul"));
  إذا (cksum_debug)
    error (0, 0, "%s",
           (pclmul_enabled
            ? _("using pclmul hardware support")
            : _("pclmul support not detected")));
#  إذا USE_PCLMUL_چکری_جانچ_32
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
# إذا USE_AVX2_چکری_جانچ_32
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
# إذا USE_AVX512_چکری_جانچ_32
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
# إذا USE_VMULL_چکری_جانچ_32
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
cksum_slice8 (FILE *fp, uint_fast32_t *چکری_جانچ_باہرلا, intmax_t *لمائی_باہرلا)
{
  uint32_t buf[BUFLEN / حجم (uint32_t)];
  uint_fast32_t چکری_جانچ = 0;
  intmax_t لمائی = 0;
  size_t بائٹاں_read;

  إذا (!fp || !چکری_جانچ_باہرلا || !لمائی_باہرلا)
    أرجع false;

  طالما ((بائٹاں_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      uint32_t *datap;

      إذا (ckd_جوڑو (&لمائی, لمائی, بائٹاں_read))
        {
          errno = EOVERFLOW;
          أرجع false;
        }

      /* Process multiples of 8 bytes */
      datap = buf;
      طالما (بائٹاں_read >= 8)
        {
          uint32_t پہلا = *datap++, دوجا = *datap++;
          چکری_جانچ ^= htobe32 (پہلا);
          دوجا = htobe32 (دوجا);
          چکری_جانچ = (crctab[7][(چکری_جانچ >> 24) & 0xFF]
                 ^ crctab[6][(چکری_جانچ >> 16) & 0xFF]
                 ^ crctab[5][(چکری_جانچ >> 8) & 0xFF]
                 ^ crctab[4][(چکری_جانچ) & 0xFF]
                 ^ crctab[3][(دوجا >> 24) & 0xFF]
                 ^ crctab[2][(دوجا >> 16) & 0xFF]
                 ^ crctab[1][(دوجا >> 8) & 0xFF]
                 ^ crctab[0][(دوجا) & 0xFF]);
          بائٹاں_read -= 8;
        }

      /* And finish up last 0-7 bytes in a byte by byte fashion */
      بنانشانی حرف *cp = (بنانشانی حرف *)datap;
      طالما (بائٹاں_read--)
        چکری_جانچ = (چکری_جانچ << 8) ^ crctab[0][((چکری_جانچ >> 24) ^ *cp++) & 0xFF];
      إذا (feof (fp))
        اكسر;
    }

  *چکری_جانچ_باہرلا = چکری_جانچ;
  *لمائی_باہرلا = لمائی;

  أرجع !ferror (fp);
}

/* Calculate the checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحيح
چکری_جانچ_جوڑ_دھارا (FILE *دھارا, فراغ *resstream, intmax_t *لمائی)
{
  intmax_t کل_بائٹاں = 0;
  uint_fast32_t چکری_جانچ = 0;

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

  إذا (! cksum_fp (دھارا, &چکری_جانچ, &کل_بائٹاں))
    أرجع -1;

  *لمائی = کل_بائٹاں;

  لكل (; کل_بائٹاں; کل_بائٹاں >>= 8)
    چکری_جانچ = (چکری_جانچ << 8) ^ crctab[0][((چکری_جانچ >> 24) ^ کل_بائٹاں) & 0xFF];
  چکری_جانچ = ~چکری_جانچ & 0xFFFFFFFF;

  بنانشانی صحيح چکری_جانچ_باہرلا = چکری_جانچ;
  حافظہ_نقل (resstream, &چکری_جانچ_باہرلا, حجم چکری_جانچ_باہرلا);

  أرجع 0;
}

/* Calculate the crc32b checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحيح
crc32b_جوڑ_دھارا (FILE *دھارا, فراغ *resstream, intmax_t *reslen)
{
  uint32_t buf[BUFLEN / حجم (uint32_t)];
  uint32_t چکری_جانچ = 0;
  intmax_t len = 0;
  size_t بائٹاں_read;

  إذا (!دھارا || !resstream || !reslen)
    أرجع -1;

# إذا GL_چکری_جانچ_X86_64_PCLMUL
  إذا (cksum_debug)
    (فراغ) pclmul_supported ();
# endif

  طالما ((بائٹاں_read = fread (buf, 1, BUFLEN, دھارا)) > 0)
    {
      إذا (ckd_جوڑو (&len, len, بائٹاں_read))
        {
          errno = EOVERFLOW;
          أرجع -1;
        }

      چکری_جانچ = چکری_جانچ_32_update (چکری_جانچ, (حرف ثابت *)buf, بائٹاں_read);

      إذا (feof (دھارا))
        اكسر;
    }

  بنانشانی صحيح چکری_جانچ_باہرلا = چکری_جانچ;
  حافظہ_نقل (resstream, &چکری_جانچ_باہرلا, حجم چکری_جانچ_باہرلا);

  *reslen = len;

  أرجع ferror (دھارا) ? -1 : 0;
}

/* Print the checksum and size to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_چکری_جانچ (حرف ثابت *file, MAYBE_بن_ورتیا صحيح ثنائی_file,
            فراغ ثابت *digest, bool raw, MAYBE_بن_ورتیا bool tagged,
            بنانشانی حرف delim, bool دلیلاں, intmax_t لمائی)
{
  إذا (raw)
    {
      /* Output in network byte order (big endian).  */
      uint32_t باہرلا_int = htobe32 (*(uint32_t *)digest);
      fwrite (&باہرلا_int, 1, 32/8, stdout);
      أرجع;
    }

  لکھو ("%u %jd", *(بنانشانی صحيح *)digest, لمائی);
  إذا (دلیلاں)
    لکھو (" %s", file);
  حرف_لکھو (delim);
}

#جے_انت /* !CRCTAB */
