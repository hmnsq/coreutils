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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CRCTAB
# include <config.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CRCTAB

# include <stdio.h>

# define ਦੋਆਧਾਰੀ_ਅੰਕ_2(x)	(1u << (x))
# define SBIT	ਦੋਆਧਾਰੀ_ਅੰਕ_2 (31)

/* The generating polynomial is

          32   26   23   22   16   12   11   10   8   7   5   4   2   1
    G(X)=X  + X  + X  + X  + X  + X  + X  + X  + X + X + X + X + X + X + 1

  The i bit in GEN is set if X^i is a summand of G(X) except X^32.  */

# define GEN	(ਦੋਆਧਾਰੀ_ਅੰਕ_2 (26) | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (23) | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (22) | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (16) | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (12) \
                 | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (11) | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (10) | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (8) | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (7) | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (5) \
                 | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (4) | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (2) | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (1) | ਦੋਆਧਾਰੀ_ਅੰਕ_2 (0))

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ r[8];

ਸਥਿਰ ਖਾਲੀ
fill_r (ਖਾਲੀ)
{
  r[0] = GEN;
  ਲਈ (ਪੂਰਨ i = 1; i < 8; i++)
    r[i] = (r[i - 1] << 1) ^ ((r[i - 1] & SBIT) ? GEN : 0);
}

ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ
ਚੱਕਰੀ_ਜਾਂਚ_remainder (ਪੂਰਨ m)
{
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ rem = 0;

  ਲਈ (ਪੂਰਨ i = 0; i < 8; i++)
    ਜੇ (ਦੋਆਧਾਰੀ_ਅੰਕ_2 (i) & m)
      rem ^= r[i];

  ਵਾਪਸ rem & 0xFFFFFFFF;	/* Make it run on 64-bit machine.  */
}

ਪੂਰਨ
ਮੁੱਖ (ਖਾਲੀ)
{
  ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ crctab[8][256];

  fill_r ();

  ਲਈ (ਪੂਰਨ i = 0; i < 256; i++)
    crctab[0][i] = ਚੱਕਰੀ_ਜਾਂਚ_remainder (i);

  /* CRC(0x11 0x22 0x33 0x44)
     is equal to
     CRC(0x11 0x00 0x00 0x00) XOR CRC(0x22 0x00 0x00) XOR
     CRC(0x33 0x00) XOR CRC(0x44)
     We precompute the CRC values for the offset values into
     separate CRC tables. We can then use them to speed up
     CRC calculation by processing multiple bytes at the time. */
  ਲਈ (ਪੂਰਨ i = 0; i < 256; i++)
    {
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੱਕਰੀ_ਜਾਂਚ = 0;

      ਚੱਕਰੀ_ਜਾਂਚ = (ਚੱਕਰੀ_ਜਾਂਚ << 8) ^ crctab[0][((ਚੱਕਰੀ_ਜਾਂਚ >> 24) ^ i) & 0xFF];
      ਲਈ (ਪੂਰਨ ਵਿੱਥ = 1; ਵਿੱਥ < 8; ਵਿੱਥ++)
        {
          ਚੱਕਰੀ_ਜਾਂਚ = (ਚੱਕਰੀ_ਜਾਂਚ << 8) ^ crctab[0][((ਚੱਕਰੀ_ਜਾਂਚ >> 24) ^ 0) & 0xFF];
          crctab[ਵਿੱਥ][i] = ਚੱਕਰੀ_ਜਾਂਚ & 0xFFFFFFFF;
        }
    }

  ਲਿਖੋ ("#include <config.h>\n");
  ਲਿਖੋ ("#include \"cksum_crc.h\"\n");
  ਲਿਖੋ ("\n");
  ਲਿਖੋ ("uint_fast32_t const crctab[8][256] = {\n");
  ਲਈ (ਪੂਰਨ y = 0; y < 8; y++)
    {
      ਲਿਖੋ ("{\n  0x%08x", crctab[y][0]);
      ਲਈ (ਪੂਰਨ i = 0; i < 51; i++)
        {
          ਲਿਖੋ (",\n  0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x",
                  crctab[y][i * 5 + 1], crctab[y][i * 5 + 2],
                  crctab[y][i * 5 + 3], crctab[y][i * 5 + 4],
                  crctab[y][i * 5 + 5]);
        }
        ਲਿਖੋ ("\n},\n");
    }
  ਲਿਖੋ ("};\n");
}

#ਨਹੀਂਤਾਂ /* !CRCTAB */

# include "cksum_crc.h"
# include <sys/types.h>
# include <endian.h>
# include "system.h"

# ifdef USE_VMULL_ਚੱਕਰੀ_ਜਾਂਚ_32
#  include <sys/auxv.h>
#  include <asm/hwcap.h>
# endif

# include "crc.h"
# include "cpu-supports.h"

/* Number of bytes to read at once.  */
# define BUFLEN (1 << 16)

ਕਿਸਮਪਰਿਭਾਸ਼ਾ bool (*cksum_fp_t) (FILE *, uint_fast32_t *, intmax_t *);

ਸਥਿਰ cksum_fp_t
pclmul_supported (ਖਾਲੀ)
{
# ਜੇ USE_PCLMUL_ਚੱਕਰੀ_ਜਾਂਚ_32 || GL_ਚੱਕਰੀ_ਜਾਂਚ_X86_64_PCLMUL
  bool pclmul_enabled = (cpu_supports ("avx")
                         && cpu_supports ("pclmul"));
  ਜੇ (cksum_debug)
    error (0, 0, "%s",
           (pclmul_enabled
            ? _("using pclmul hardware support")
            : _("pclmul support not detected")));
#  ਜੇ USE_PCLMUL_ਚੱਕਰੀ_ਜਾਂਚ_32
  ਜੇ (pclmul_enabled)
    ਵਾਪਸ cksum_pclmul;
#  endif
# endif

  ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ cksum_fp_t
avx2_supported (ਖਾਲੀ)
{
  /* AVX512 processors will not set vpclmulqdq unless they support
     the avx512 version, but it implies that the avx2 version
     is supported  */
# ਜੇ USE_AVX2_ਚੱਕਰੀ_ਜਾਂਚ_32
  bool avx2_enabled = (cpu_supports ("avx2")
                       && cpu_supports ("vpclmulqdq"));
  ਜੇ (cksum_debug)
    error (0, 0, "%s",
           (avx2_enabled
            ? _("using avx2 hardware support")
            : _("avx2 support not detected")));
  ਜੇ (avx2_enabled)
    ਵਾਪਸ cksum_avx2;
# endif

  ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ cksum_fp_t
avx512_supported (ਖਾਲੀ)
{
  /* vpclmulqdq for multiplication
     mavx512f for most of the avx512 functions we're using
     mavx512bw for byte swapping  */
# ਜੇ USE_AVX512_ਚੱਕਰੀ_ਜਾਂਚ_32
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw")
                         && cpu_supports ("vpclmulqdq"));

  ਜੇ (cksum_debug)
    error (0, 0, "%s",
           (avx512_enabled
            ? _("using avx512 hardware support")
            : _("avx512 support not detected")));
  ਜੇ (avx512_enabled)
    ਵਾਪਸ cksum_avx512;
# endif

  ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ cksum_fp_t
vmull_supported (ਖਾਲੀ)
{
  /* vmull for multiplication  */
# ਜੇ USE_VMULL_ਚੱਕਰੀ_ਜਾਂਚ_32
  bool vmull_enabled = (cpu_may_support ("pmull")
                        && (getauxval (AT_HWCAP) & HWCAP_PMULL) > 0);
  ਜੇ (cksum_debug)
    error (0, 0, "%s",
           (vmull_enabled
            ? _("using vmull hardware support")
            : _("vmull support not detected")));
  ਜੇ (vmull_enabled)
    ਵਾਪਸ cksum_vmull;
# endif

  ਵਾਪਸ ਨਿਰਾ;
}

ਸਥਿਰ bool
cksum_slice8 (FILE *fp, uint_fast32_t *ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ, intmax_t *ਲੰਮਾਈ_ਬਾਹਰਲਾ)
{
  uint32_t buf[BUFLEN / ਆਕਾਰ (uint32_t)];
  uint_fast32_t ਚੱਕਰੀ_ਜਾਂਚ = 0;
  intmax_t ਲੰਮਾਈ = 0;
  size_t ਬਾਈਟਾਂ_read;

  ਜੇ (!fp || !ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ || !ਲੰਮਾਈ_ਬਾਹਰਲਾ)
    ਵਾਪਸ false;

  ਜਦੋਂ ((ਬਾਈਟਾਂ_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      uint32_t *datap;

      ਜੇ (ckd_ਜੋੜੋ (&ਲੰਮਾਈ, ਲੰਮਾਈ, ਬਾਈਟਾਂ_read))
        {
          errno = EOVERFLOW;
          ਵਾਪਸ false;
        }

      /* Process multiples of 8 bytes */
      datap = buf;
      ਜਦੋਂ (ਬਾਈਟਾਂ_read >= 8)
        {
          uint32_t ਪਹਿਲਾ = *datap++, ਦੂਜਾ = *datap++;
          ਚੱਕਰੀ_ਜਾਂਚ ^= htobe32 (ਪਹਿਲਾ);
          ਦੂਜਾ = htobe32 (ਦੂਜਾ);
          ਚੱਕਰੀ_ਜਾਂਚ = (crctab[7][(ਚੱਕਰੀ_ਜਾਂਚ >> 24) & 0xFF]
                 ^ crctab[6][(ਚੱਕਰੀ_ਜਾਂਚ >> 16) & 0xFF]
                 ^ crctab[5][(ਚੱਕਰੀ_ਜਾਂਚ >> 8) & 0xFF]
                 ^ crctab[4][(ਚੱਕਰੀ_ਜਾਂਚ) & 0xFF]
                 ^ crctab[3][(ਦੂਜਾ >> 24) & 0xFF]
                 ^ crctab[2][(ਦੂਜਾ >> 16) & 0xFF]
                 ^ crctab[1][(ਦੂਜਾ >> 8) & 0xFF]
                 ^ crctab[0][(ਦੂਜਾ) & 0xFF]);
          ਬਾਈਟਾਂ_read -= 8;
        }

      /* And finish up last 0-7 bytes in a byte by byte fashion */
      ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *cp = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)datap;
      ਜਦੋਂ (ਬਾਈਟਾਂ_read--)
        ਚੱਕਰੀ_ਜਾਂਚ = (ਚੱਕਰੀ_ਜਾਂਚ << 8) ^ crctab[0][((ਚੱਕਰੀ_ਜਾਂਚ >> 24) ^ *cp++) & 0xFF];
      ਜੇ (feof (fp))
        ਰੋਕੋ;
    }

  *ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ = ਚੱਕਰੀ_ਜਾਂਚ;
  *ਲੰਮਾਈ_ਬਾਹਰਲਾ = ਲੰਮਾਈ;

  ਵਾਪਸ !ferror (fp);
}

/* Calculate the checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ਪੂਰਨ
ਚੱਕਰੀ_ਜਾਂਚ_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, intmax_t *ਲੰਮਾਈ)
{
  intmax_t ਕੁੱਲ_ਬਾਈਟਾਂ = 0;
  uint_fast32_t ਚੱਕਰੀ_ਜਾਂਚ = 0;

  ਸਥਿਰ cksum_fp_t cksum_fp;
  ਜੇ (! cksum_fp)
    cksum_fp = avx512_supported ();
  ਜੇ (! cksum_fp)
    cksum_fp = avx2_supported ();
  ਜੇ (! cksum_fp)
    cksum_fp = pclmul_supported ();
  ਜੇ (! cksum_fp)
    cksum_fp = vmull_supported ();
  ਜੇ (! cksum_fp)
    cksum_fp = cksum_slice8;

  ਜੇ (! cksum_fp (ਧਾਰਾ, &ਚੱਕਰੀ_ਜਾਂਚ, &ਕੁੱਲ_ਬਾਈਟਾਂ))
    ਵਾਪਸ -1;

  *ਲੰਮਾਈ = ਕੁੱਲ_ਬਾਈਟਾਂ;

  ਲਈ (; ਕੁੱਲ_ਬਾਈਟਾਂ; ਕੁੱਲ_ਬਾਈਟਾਂ >>= 8)
    ਚੱਕਰੀ_ਜਾਂਚ = (ਚੱਕਰੀ_ਜਾਂਚ << 8) ^ crctab[0][((ਚੱਕਰੀ_ਜਾਂਚ >> 24) ^ ਕੁੱਲ_ਬਾਈਟਾਂ) & 0xFF];
  ਚੱਕਰੀ_ਜਾਂਚ = ~ਚੱਕਰੀ_ਜਾਂਚ & 0xFFFFFFFF;

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ = ਚੱਕਰੀ_ਜਾਂਚ;
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (resstream, &ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ, ਆਕਾਰ ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ);

  ਵਾਪਸ 0;
}

/* Calculate the crc32b checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ਪੂਰਨ
crc32b_ਜੋੜ_ਧਾਰਾ (FILE *ਧਾਰਾ, ਖਾਲੀ *resstream, intmax_t *reslen)
{
  uint32_t buf[BUFLEN / ਆਕਾਰ (uint32_t)];
  uint32_t ਚੱਕਰੀ_ਜਾਂਚ = 0;
  intmax_t len = 0;
  size_t ਬਾਈਟਾਂ_read;

  ਜੇ (!ਧਾਰਾ || !resstream || !reslen)
    ਵਾਪਸ -1;

# ਜੇ GL_ਚੱਕਰੀ_ਜਾਂਚ_X86_64_PCLMUL
  ਜੇ (cksum_debug)
    (ਖਾਲੀ) pclmul_supported ();
# endif

  ਜਦੋਂ ((ਬਾਈਟਾਂ_read = fread (buf, 1, BUFLEN, ਧਾਰਾ)) > 0)
    {
      ਜੇ (ckd_ਜੋੜੋ (&len, len, ਬਾਈਟਾਂ_read))
        {
          errno = EOVERFLOW;
          ਵਾਪਸ -1;
        }

      ਚੱਕਰੀ_ਜਾਂਚ = ਚੱਕਰੀ_ਜਾਂਚ_32_update (ਚੱਕਰੀ_ਜਾਂਚ, (ਅੱਖਰ ਅਚਲ *)buf, ਬਾਈਟਾਂ_read);

      ਜੇ (feof (ਧਾਰਾ))
        ਰੋਕੋ;
    }

  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ = ਚੱਕਰੀ_ਜਾਂਚ;
  ਸਮ੍ਰਿਤੀ_ਨਕਲ (resstream, &ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ, ਆਕਾਰ ਚੱਕਰੀ_ਜਾਂਚ_ਬਾਹਰਲਾ);

  *reslen = len;

  ਵਾਪਸ ferror (ਧਾਰਾ) ? -1 : 0;
}

/* Print the checksum and size to stdout.
   If ARGS is true, also print the FILE name.  */

ਖਾਲੀ
output_ਚੱਕਰੀ_ਜਾਂਚ (ਅੱਖਰ ਅਚਲ *file, MAYBE_ਅਣਵਰਤਿਆ ਪੂਰਨ ਦੁਆਧਾਰੀ_file,
            ਖਾਲੀ ਅਚਲ *digest, bool raw, MAYBE_ਅਣਵਰਤਿਆ bool tagged,
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ delim, bool ਦਲੀਲਾਂ, intmax_t ਲੰਮਾਈ)
{
  ਜੇ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint32_t ਬਾਹਰਲਾ_int = htobe32 (*(uint32_t *)digest);
      fwrite (&ਬਾਹਰਲਾ_int, 1, 32/8, stdout);
      ਵਾਪਸ;
    }

  ਲਿਖੋ ("%u %jd", *(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ *)digest, ਲੰਮਾਈ);
  ਜੇ (ਦਲੀਲਾਂ)
    ਲਿਖੋ (" %s", file);
  ਅੱਖਰ_ਲਿਖੋ (delim);
}

#ਜੇ_ਅੰਤ /* !CRCTAB */
