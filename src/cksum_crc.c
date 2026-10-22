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
#اگر_غیر_تعریف_شدہ CRCTAB
# include <config.h>
#اگر_ختم

#اگر_تعریف_شدہ CRCTAB

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

ساکن غیرعلامتی صحیح r[8];

ساکن خالی
fill_r (خالی)
{
  r[0] = GEN;
  برائے (صحیح i = 1; i < 8; i++)
    r[i] = (r[i - 1] << 1) ^ ((r[i - 1] & SBIT) ? GEN : 0);
}

ساکن غیرعلامتی صحیح
چکری_جانچ_remainder (صحیح m)
{
  غیرعلامتی صحیح rem = 0;

  برائے (صحیح i = 0; i < 8; i++)
    اگر (ثنائی_ہندسہ_2 (i) & m)
      rem ^= r[i];

  واپس rem & 0xFFFFFFFF;	/* Make it run on 64-bit machine.  */
}

صحیح
مین (خالی)
{
  ساکن غیرعلامتی صحیح crctab[8][256];

  fill_r ();

  برائے (صحیح i = 0; i < 256; i++)
    crctab[0][i] = چکری_جانچ_remainder (i);

  /* CRC(0x11 0x22 0x33 0x44)
     is equal to
     CRC(0x11 0x00 0x00 0x00) XOR CRC(0x22 0x00 0x00) XOR
     CRC(0x33 0x00) XOR CRC(0x44)
     We precompute the CRC values for the offset values into
     separate CRC tables. We can then use them to speed up
     CRC calculation by processing multiple bytes at the time. */
  برائے (صحیح i = 0; i < 256; i++)
    {
      غیرعلامتی صحیح چکری_جانچ = 0;

      چکری_جانچ = (چکری_جانچ << 8) ^ crctab[0][((چکری_جانچ >> 24) ^ i) & 0xFF];
      برائے (صحیح انحراف = 1; انحراف < 8; انحراف++)
        {
          چکری_جانچ = (چکری_جانچ << 8) ^ crctab[0][((چکری_جانچ >> 24) ^ 0) & 0xFF];
          crctab[انحراف][i] = چکری_جانچ & 0xFFFFFFFF;
        }
    }

  لکھو ("#include <config.h>\n");
  لکھو ("#include \"cksum_crc.h\"\n");
  لکھو ("\n");
  لکھو ("uint_fast32_t const crctab[8][256] = {\n");
  برائے (صحیح y = 0; y < 8; y++)
    {
      لکھو ("{\n  0x%08x", crctab[y][0]);
      برائے (صحیح i = 0; i < 51; i++)
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

#ورنہ /* !CRCTAB */

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

قسمتعریف bool (*cksum_fp_t) (FILE *, uint_fast32_t *, intmax_t *);

ساکن cksum_fp_t
pclmul_supported (خالی)
{
# اگر USE_PCLMUL_چکری_جانچ_32 || GL_چکری_جانچ_X86_64_PCLMUL
  bool pclmul_enabled = (cpu_supports ("avx")
                         && cpu_supports ("pclmul"));
  اگر (cksum_debug)
    error (0, 0, "%s",
           (pclmul_enabled
            ? _("using pclmul hardware support")
            : _("pclmul support not detected")));
#  اگر USE_PCLMUL_چکری_جانچ_32
  اگر (pclmul_enabled)
    واپس cksum_pclmul;
#  endif
# endif

  واپس تہی;
}

ساکن cksum_fp_t
avx2_supported (خالی)
{
  /* AVX512 processors will not set vpclmulqdq unless they support
     the avx512 version, but it implies that the avx2 version
     is supported  */
# اگر USE_AVX2_چکری_جانچ_32
  bool avx2_enabled = (cpu_supports ("avx2")
                       && cpu_supports ("vpclmulqdq"));
  اگر (cksum_debug)
    error (0, 0, "%s",
           (avx2_enabled
            ? _("using avx2 hardware support")
            : _("avx2 support not detected")));
  اگر (avx2_enabled)
    واپس cksum_avx2;
# endif

  واپس تہی;
}

ساکن cksum_fp_t
avx512_supported (خالی)
{
  /* vpclmulqdq for multiplication
     mavx512f for most of the avx512 functions we're using
     mavx512bw for byte swapping  */
# اگر USE_AVX512_چکری_جانچ_32
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw")
                         && cpu_supports ("vpclmulqdq"));

  اگر (cksum_debug)
    error (0, 0, "%s",
           (avx512_enabled
            ? _("using avx512 hardware support")
            : _("avx512 support not detected")));
  اگر (avx512_enabled)
    واپس cksum_avx512;
# endif

  واپس تہی;
}

ساکن cksum_fp_t
vmull_supported (خالی)
{
  /* vmull for multiplication  */
# اگر USE_VMULL_چکری_جانچ_32
  bool vmull_enabled = (cpu_may_support ("pmull")
                        && (getauxval (AT_HWCAP) & HWCAP_PMULL) > 0);
  اگر (cksum_debug)
    error (0, 0, "%s",
           (vmull_enabled
            ? _("using vmull hardware support")
            : _("vmull support not detected")));
  اگر (vmull_enabled)
    واپس cksum_vmull;
# endif

  واپس تہی;
}

ساکن bool
cksum_slice8 (FILE *fp, uint_fast32_t *چکری_جانچ_برآمد, intmax_t *لمبائی_برآمد)
{
  uint32_t buf[BUFLEN / حجم (uint32_t)];
  uint_fast32_t چکری_جانچ = 0;
  intmax_t لمبائی = 0;
  size_t بائٹیں_read;

  اگر (!fp || !چکری_جانچ_برآمد || !لمبائی_برآمد)
    واپس false;

  جب ((بائٹیں_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      uint32_t *datap;

      اگر (ckd_جوڑو (&لمبائی, لمبائی, بائٹیں_read))
        {
          errno = EOVERFLOW;
          واپس false;
        }

      /* Process multiples of 8 bytes */
      datap = buf;
      جب (بائٹیں_read >= 8)
        {
          uint32_t پہلا = *datap++, دوسرا = *datap++;
          چکری_جانچ ^= htobe32 (پہلا);
          دوسرا = htobe32 (دوسرا);
          چکری_جانچ = (crctab[7][(چکری_جانچ >> 24) & 0xFF]
                 ^ crctab[6][(چکری_جانچ >> 16) & 0xFF]
                 ^ crctab[5][(چکری_جانچ >> 8) & 0xFF]
                 ^ crctab[4][(چکری_جانچ) & 0xFF]
                 ^ crctab[3][(دوسرا >> 24) & 0xFF]
                 ^ crctab[2][(دوسرا >> 16) & 0xFF]
                 ^ crctab[1][(دوسرا >> 8) & 0xFF]
                 ^ crctab[0][(دوسرا) & 0xFF]);
          بائٹیں_read -= 8;
        }

      /* And finish up last 0-7 bytes in a byte by byte fashion */
      غیرعلامتی حرف *cp = (غیرعلامتی حرف *)datap;
      جب (بائٹیں_read--)
        چکری_جانچ = (چکری_جانچ << 8) ^ crctab[0][((چکری_جانچ >> 24) ^ *cp++) & 0xFF];
      اگر (feof (fp))
        توڑو;
    }

  *چکری_جانچ_برآمد = چکری_جانچ;
  *لمبائی_برآمد = لمبائی;

  واپس !ferror (fp);
}

/* Calculate the checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحیح
چکری_جانچ_مجموع_دھارا (FILE *دھارا, خالی *resstream, intmax_t *لمبائی)
{
  intmax_t کل_بائٹیں = 0;
  uint_fast32_t چکری_جانچ = 0;

  ساکن cksum_fp_t cksum_fp;
  اگر (! cksum_fp)
    cksum_fp = avx512_supported ();
  اگر (! cksum_fp)
    cksum_fp = avx2_supported ();
  اگر (! cksum_fp)
    cksum_fp = pclmul_supported ();
  اگر (! cksum_fp)
    cksum_fp = vmull_supported ();
  اگر (! cksum_fp)
    cksum_fp = cksum_slice8;

  اگر (! cksum_fp (دھارا, &چکری_جانچ, &کل_بائٹیں))
    واپس -1;

  *لمبائی = کل_بائٹیں;

  برائے (; کل_بائٹیں; کل_بائٹیں >>= 8)
    چکری_جانچ = (چکری_جانچ << 8) ^ crctab[0][((چکری_جانچ >> 24) ^ کل_بائٹیں) & 0xFF];
  چکری_جانچ = ~چکری_جانچ & 0xFFFFFFFF;

  غیرعلامتی صحیح چکری_جانچ_برآمد = چکری_جانچ;
  حافظہ_نقل (resstream, &چکری_جانچ_برآمد, حجم چکری_جانچ_برآمد);

  واپس 0;
}

/* Calculate the crc32b checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحیح
crc32b_مجموع_دھارا (FILE *دھارا, خالی *resstream, intmax_t *reslen)
{
  uint32_t buf[BUFLEN / حجم (uint32_t)];
  uint32_t چکری_جانچ = 0;
  intmax_t len = 0;
  size_t بائٹیں_read;

  اگر (!دھارا || !resstream || !reslen)
    واپس -1;

# اگر GL_چکری_جانچ_X86_64_PCLMUL
  اگر (cksum_debug)
    (خالی) pclmul_supported ();
# endif

  جب ((بائٹیں_read = fread (buf, 1, BUFLEN, دھارا)) > 0)
    {
      اگر (ckd_جوڑو (&len, len, بائٹیں_read))
        {
          errno = EOVERFLOW;
          واپس -1;
        }

      چکری_جانچ = چکری_جانچ_32_update (چکری_جانچ, (حرف ثابت *)buf, بائٹیں_read);

      اگر (feof (دھارا))
        توڑو;
    }

  غیرعلامتی صحیح چکری_جانچ_برآمد = چکری_جانچ;
  حافظہ_نقل (resstream, &چکری_جانچ_برآمد, حجم چکری_جانچ_برآمد);

  *reslen = len;

  واپس ferror (دھارا) ? -1 : 0;
}

/* Print the checksum and size to stdout.
   If ARGS is true, also print the FILE name.  */

خالی
output_چکری_جانچ (حرف ثابت *file, MAYBE_غیر_مستعمل صحیح ثنائی_file,
            خالی ثابت *digest, bool raw, MAYBE_غیر_مستعمل bool tagged,
            غیرعلامتی حرف delim, bool دلائل, intmax_t لمبائی)
{
  اگر (raw)
    {
      /* Output in network byte order (big endian).  */
      uint32_t برآمد_int = htobe32 (*(uint32_t *)digest);
      fwrite (&برآمد_int, 1, 32/8, stdout);
      واپس;
    }

  لکھو ("%u %jd", *(غیرعلامتی صحیح *)digest, لمبائی);
  اگر (دلائل)
    لکھو (" %s", file);
  حرف_لکھو (delim);
}

#اگر_ختم /* !CRCTAB */
