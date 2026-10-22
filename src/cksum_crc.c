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
#ifndef CRCTAB
# include <config.h>
#endif

#ifdef CRCTAB

# include <stdio.h>

# define BIT(x)	(1u << (x))
# define SBIT	BIT (31)

/* The generating polynomial is

          32   26   23   22   16   12   11   10   8   7   5   4   2   1
    G(X)=X  + X  + X  + X  + X  + X  + X  + X  + X + X + X + X + X + X + 1

  The i bit in GEN is set if X^i is a summand of G(X) except X^32.  */

# define GEN	(BIT (26) | BIT (23) | BIT (22) | BIT (16) | BIT (12) \
                 | BIT (11) | BIT (10) | BIT (8) | BIT (7) | BIT (5) \
                 | BIT (4) | BIT (2) | BIT (1) | BIT (0))

ساكن unsigned صحيح r[8];

ساكن فراغ
fill_r (فراغ)
{
  r[0] = GEN;
  لكل (صحيح i = 1; i < 8; i++)
    r[i] = (r[i - 1] << 1) ^ ((r[i - 1] & SBIT) ? GEN : 0);
}

ساكن unsigned صحيح
crc_remainder (صحيح m)
{
  unsigned صحيح rem = 0;

  لكل (صحيح i = 0; i < 8; i++)
    إذا (BIT (i) & m)
      rem ^= r[i];

  أرجع rem & 0xFFFFFFFF;	/* Make it run on 64-bit machine.  */
}

صحيح
main (فراغ)
{
  ساكن unsigned صحيح crctab[8][256];

  fill_r ();

  لكل (صحيح i = 0; i < 256; i++)
    crctab[0][i] = crc_remainder (i);

  /* CRC(0x11 0x22 0x33 0x44)
     is equal to
     CRC(0x11 0x00 0x00 0x00) XOR CRC(0x22 0x00 0x00) XOR
     CRC(0x33 0x00) XOR CRC(0x44)
     We precompute the CRC values for the offset values into
     separate CRC tables. We can then use them to speed up
     CRC calculation by processing multiple bytes at the time. */
  لكل (صحيح i = 0; i < 256; i++)
    {
      unsigned صحيح crc = 0;

      crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ i) & 0xFF];
      لكل (صحيح offset = 1; offset < 8; offset++)
        {
          crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ 0) & 0xFF];
          crctab[offset][i] = crc & 0xFFFFFFFF;
        }
    }

  printf ("#include <config.h>\n");
  printf ("#include \"cksum_crc.h\"\n");
  printf ("\n");
  printf ("uint_fast32_t const crctab[8][256] = {\n");
  لكل (صحيح y = 0; y < 8; y++)
    {
      printf ("{\n  0x%08x", crctab[y][0]);
      لكل (صحيح i = 0; i < 51; i++)
        {
          printf (",\n  0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x",
                  crctab[y][i * 5 + 1], crctab[y][i * 5 + 2],
                  crctab[y][i * 5 + 3], crctab[y][i * 5 + 4],
                  crctab[y][i * 5 + 5]);
        }
        printf ("\n},\n");
    }
  printf ("};\n");
}

#وإلا /* !CRCTAB */

# include "cksum_crc.h"
# include <sys/types.h>
# include <endian.h>
# include "system.h"

# ifdef USE_VMULL_CRC32
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
# إذا USE_PCLMUL_CRC32 || GL_CRC_X86_64_PCLMUL
  bool pclmul_enabled = (cpu_supports ("avx")
                         && cpu_supports ("pclmul"));
  إذا (cksum_debug)
    error (0, 0, "%s",
           (pclmul_enabled
            ? _("using pclmul hardware support")
            : _("pclmul support not detected")));
#  إذا USE_PCLMUL_CRC32
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
# إذا USE_AVX2_CRC32
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
# إذا USE_AVX512_CRC32
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
# إذا USE_VMULL_CRC32
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
cksum_slice8 (FILE *fp, uint_fast32_t *crc_out, intmax_t *length_out)
{
  uint32_t buf[BUFLEN / حجم (uint32_t)];
  uint_fast32_t crc = 0;
  intmax_t length = 0;
  size_t bytes_read;

  إذا (!fp || !crc_out || !length_out)
    أرجع false;

  طالما ((bytes_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      uint32_t *datap;

      إذا (ckd_add (&length, length, bytes_read))
        {
          errno = EOVERFLOW;
          أرجع false;
        }

      /* Process multiples of 8 bytes */
      datap = buf;
      طالما (bytes_read >= 8)
        {
          uint32_t first = *datap++, second = *datap++;
          crc ^= htobe32 (first);
          second = htobe32 (second);
          crc = (crctab[7][(crc >> 24) & 0xFF]
                 ^ crctab[6][(crc >> 16) & 0xFF]
                 ^ crctab[5][(crc >> 8) & 0xFF]
                 ^ crctab[4][(crc) & 0xFF]
                 ^ crctab[3][(second >> 24) & 0xFF]
                 ^ crctab[2][(second >> 16) & 0xFF]
                 ^ crctab[1][(second >> 8) & 0xFF]
                 ^ crctab[0][(second) & 0xFF]);
          bytes_read -= 8;
        }

      /* And finish up last 0-7 bytes in a byte by byte fashion */
      unsigned حرف *cp = (unsigned حرف *)datap;
      طالما (bytes_read--)
        crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ *cp++) & 0xFF];
      إذا (feof (fp))
        اكسر;
    }

  *crc_out = crc;
  *length_out = length;

  أرجع !ferror (fp);
}

/* Calculate the checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحيح
crc_sum_stream (FILE *stream, فراغ *resstream, intmax_t *length)
{
  intmax_t total_bytes = 0;
  uint_fast32_t crc = 0;

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

  إذا (! cksum_fp (stream, &crc, &total_bytes))
    أرجع -1;

  *length = total_bytes;

  لكل (; total_bytes; total_bytes >>= 8)
    crc = (crc << 8) ^ crctab[0][((crc >> 24) ^ total_bytes) & 0xFF];
  crc = ~crc & 0xFFFFFFFF;

  unsigned صحيح crc_out = crc;
  memcpy (resstream, &crc_out, حجم crc_out);

  أرجع 0;
}

/* Calculate the crc32b checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

صحيح
crc32b_sum_stream (FILE *stream, فراغ *resstream, intmax_t *reslen)
{
  uint32_t buf[BUFLEN / حجم (uint32_t)];
  uint32_t crc = 0;
  intmax_t len = 0;
  size_t bytes_read;

  إذا (!stream || !resstream || !reslen)
    أرجع -1;

# إذا GL_CRC_X86_64_PCLMUL
  إذا (cksum_debug)
    (فراغ) pclmul_supported ();
# endif

  طالما ((bytes_read = fread (buf, 1, BUFLEN, stream)) > 0)
    {
      إذا (ckd_add (&len, len, bytes_read))
        {
          errno = EOVERFLOW;
          أرجع -1;
        }

      crc = crc32_update (crc, (حرف ثابت *)buf, bytes_read);

      إذا (feof (stream))
        اكسر;
    }

  unsigned صحيح crc_out = crc;
  memcpy (resstream, &crc_out, حجم crc_out);

  *reslen = len;

  أرجع ferror (stream) ? -1 : 0;
}

/* Print the checksum and size to stdout.
   If ARGS is true, also print the FILE name.  */

فراغ
output_crc (حرف ثابت *file, MAYBE_UNUSED صحيح ثنائي_file,
            فراغ ثابت *digest, bool raw, MAYBE_UNUSED bool tagged,
            unsigned حرف delim, bool وسائط, intmax_t length)
{
  إذا (raw)
    {
      /* Output in network byte order (big endian).  */
      uint32_t out_int = htobe32 (*(uint32_t *)digest);
      fwrite (&out_int, 1, 32/8, stdout);
      أرجع;
    }

  printf ("%u %jd", *(unsigned صحيح *)digest, length);
  إذا (وسائط)
    printf (" %s", file);
  putchar (delim);
}

#endif /* !CRCTAB */
