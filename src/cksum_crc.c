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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CRCTAB
# include <config.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CRCTAB

# include <stdio.h>

# define ಬಿಟ್_2(x)	(1u << (x))
# define SBIT	ಬಿಟ್_2 (31)

/* The generating polynomial is

          32   26   23   22   16   12   11   10   8   7   5   4   2   1
    G(X)=X  + X  + X  + X  + X  + X  + X  + X  + X + X + X + X + X + X + 1

  The i bit in GEN is set if X^i is a summand of G(X) except X^32.  */

# define GEN	(ಬಿಟ್_2 (26) | ಬಿಟ್_2 (23) | ಬಿಟ್_2 (22) | ಬಿಟ್_2 (16) | ಬಿಟ್_2 (12) \
                 | ಬಿಟ್_2 (11) | ಬಿಟ್_2 (10) | ಬಿಟ್_2 (8) | ಬಿಟ್_2 (7) | ಬಿಟ್_2 (5) \
                 | ಬಿಟ್_2 (4) | ಬಿಟ್_2 (2) | ಬಿಟ್_2 (1) | ಬಿಟ್_2 (0))

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ r[8];

ಸ್ಥಿರ ಶೂನ್ಯ
fill_r (ಶೂನ್ಯ)
{
  r[0] = GEN;
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < 8; i++)
    r[i] = (r[i - 1] << 1) ^ ((r[i - 1] & SBIT) ? GEN : 0);
}

ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ
ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_remainder (ಪೂರ್ಣಾಂಕ m)
{
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ rem = 0;

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 8; i++)
    ಆದರೆ (ಬಿಟ್_2 (i) & m)
      rem ^= r[i];

  ಹಿಂತಿರುಗಿ rem & 0xFFFFFFFF;	/* Make it run on 64-bit machine.  */
}

ಪೂರ್ಣಾಂಕ
ಮುಖ್ಯ (ಶೂನ್ಯ)
{
  ಸ್ಥಿರ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ crctab[8][256];

  fill_r ();

  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 256; i++)
    crctab[0][i] = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_remainder (i);

  /* CRC(0x11 0x22 0x33 0x44)
     is equal to
     CRC(0x11 0x00 0x00 0x00) XOR CRC(0x22 0x00 0x00) XOR
     CRC(0x33 0x00) XOR CRC(0x44)
     We precompute the CRC values for the offset values into
     separate CRC tables. We can then use them to speed up
     CRC calculation by processing multiple bytes at the time. */
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 256; i++)
    {
      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = 0;

      ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = (ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ << 8) ^ crctab[0][((ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 24) ^ i) & 0xFF];
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = 1; ಸ್ಥಾನಾಂತರ < 8; ಸ್ಥಾನಾಂತರ++)
        {
          ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = (ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ << 8) ^ crctab[0][((ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 24) ^ 0) & 0xFF];
          crctab[ಸ್ಥಾನಾಂತರ][i] = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ & 0xFFFFFFFF;
        }
    }

  ಬರೆ ("#include <config.h>\n");
  ಬರೆ ("#include \"cksum_crc.h\"\n");
  ಬರೆ ("\n");
  ಬರೆ ("uint_fast32_t const crctab[8][256] = {\n");
  ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < 8; y++)
    {
      ಬರೆ ("{\n  0x%08x", crctab[y][0]);
      ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 51; i++)
        {
          ಬರೆ (",\n  0x%08x, 0x%08x, 0x%08x, 0x%08x, 0x%08x",
                  crctab[y][i * 5 + 1], crctab[y][i * 5 + 2],
                  crctab[y][i * 5 + 3], crctab[y][i * 5 + 4],
                  crctab[y][i * 5 + 5]);
        }
        ಬರೆ ("\n},\n");
    }
  ಬರೆ ("};\n");
}

#ಇಲ್ಲದಿದ್ದರೆ /* !CRCTAB */

# include "cksum_crc.h"
# include <sys/types.h>
# include <endian.h>
# include "system.h"

# ifdef USE_VMULL_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32
#  include <sys/auxv.h>
#  include <asm/hwcap.h>
# endif

# include "crc.h"
# include "cpu-supports.h"

/* Number of bytes to read at once.  */
# define BUFLEN (1 << 16)

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ bool (*cksum_fp_t) (FILE *, uint_fast32_t *, intmax_t *);

ಸ್ಥಿರ cksum_fp_t
pclmul_supported (ಶೂನ್ಯ)
{
# ಆದರೆ USE_PCLMUL_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32 || GL_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_X86_64_PCLMUL
  bool pclmul_enabled = (cpu_supports ("avx")
                         && cpu_supports ("pclmul"));
  ಆದರೆ (cksum_debug)
    error (0, 0, "%s",
           (pclmul_enabled
            ? _("using pclmul hardware support")
            : _("pclmul support not detected")));
#  ಆದರೆ USE_PCLMUL_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32
  ಆದರೆ (pclmul_enabled)
    ಹಿಂತಿರುಗಿ cksum_pclmul;
#  endif
# endif

  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ cksum_fp_t
avx2_supported (ಶೂನ್ಯ)
{
  /* AVX512 processors will not set vpclmulqdq unless they support
     the avx512 version, but it implies that the avx2 version
     is supported  */
# ಆದರೆ USE_AVX2_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32
  bool avx2_enabled = (cpu_supports ("avx2")
                       && cpu_supports ("vpclmulqdq"));
  ಆದರೆ (cksum_debug)
    error (0, 0, "%s",
           (avx2_enabled
            ? _("using avx2 hardware support")
            : _("avx2 support not detected")));
  ಆದರೆ (avx2_enabled)
    ಹಿಂತಿರುಗಿ cksum_avx2;
# endif

  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ cksum_fp_t
avx512_supported (ಶೂನ್ಯ)
{
  /* vpclmulqdq for multiplication
     mavx512f for most of the avx512 functions we're using
     mavx512bw for byte swapping  */
# ಆದರೆ USE_AVX512_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32
  bool avx512_enabled = (cpu_supports ("avx512f")
                         && cpu_supports ("avx512bw")
                         && cpu_supports ("vpclmulqdq"));

  ಆದರೆ (cksum_debug)
    error (0, 0, "%s",
           (avx512_enabled
            ? _("using avx512 hardware support")
            : _("avx512 support not detected")));
  ಆದರೆ (avx512_enabled)
    ಹಿಂತಿರುಗಿ cksum_avx512;
# endif

  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ cksum_fp_t
vmull_supported (ಶೂನ್ಯ)
{
  /* vmull for multiplication  */
# ಆದರೆ USE_VMULL_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32
  bool vmull_enabled = (cpu_may_support ("pmull")
                        && (getauxval (AT_HWCAP) & HWCAP_PMULL) > 0);
  ಆದರೆ (cksum_debug)
    error (0, 0, "%s",
           (vmull_enabled
            ? _("using vmull hardware support")
            : _("vmull support not detected")));
  ಆದರೆ (vmull_enabled)
    ಹಿಂತಿರುಗಿ cksum_vmull;
# endif

  ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

ಸ್ಥಿರ bool
cksum_slice8 (FILE *fp, uint_fast32_t *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು, intmax_t *ಉದ್ದಳತೆ_ಹೊರಹೋಗು)
{
  uint32_t buf[BUFLEN / ಗಾತ್ರ (uint32_t)];
  uint_fast32_t ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = 0;
  intmax_t ಉದ್ದಳತೆ = 0;
  size_t ಬೈಟುಗಳು_read;

  ಆದರೆ (!fp || !ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು || !ಉದ್ದಳತೆ_ಹೊರಹೋಗು)
    ಹಿಂತಿರುಗಿ false;

  ಆಗ ((ಬೈಟುಗಳು_read = fread (buf, 1, BUFLEN, fp)) > 0)
    {
      uint32_t *datap;

      ಆದರೆ (ckd_ಕೂಡಿಸು (&ಉದ್ದಳತೆ, ಉದ್ದಳತೆ, ಬೈಟುಗಳು_read))
        {
          errno = EOVERFLOW;
          ಹಿಂತಿರುಗಿ false;
        }

      /* Process multiples of 8 bytes */
      datap = buf;
      ಆಗ (ಬೈಟುಗಳು_read >= 8)
        {
          uint32_t ಮೊದಲ = *datap++, ಎರಡನೇ = *datap++;
          ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ ^= htobe32 (ಮೊದಲ);
          ಎರಡನೇ = htobe32 (ಎರಡನೇ);
          ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = (crctab[7][(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 24) & 0xFF]
                 ^ crctab[6][(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 16) & 0xFF]
                 ^ crctab[5][(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 8) & 0xFF]
                 ^ crctab[4][(ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ) & 0xFF]
                 ^ crctab[3][(ಎರಡನೇ >> 24) & 0xFF]
                 ^ crctab[2][(ಎರಡನೇ >> 16) & 0xFF]
                 ^ crctab[1][(ಎರಡನೇ >> 8) & 0xFF]
                 ^ crctab[0][(ಎರಡನೇ) & 0xFF]);
          ಬೈಟುಗಳು_read -= 8;
        }

      /* And finish up last 0-7 bytes in a byte by byte fashion */
      ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *cp = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)datap;
      ಆಗ (ಬೈಟುಗಳು_read--)
        ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = (ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ << 8) ^ crctab[0][((ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 24) ^ *cp++) & 0xFF];
      ಆದರೆ (feof (fp))
        ನಿಲ್ಲಿಸು;
    }

  *ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
  *ಉದ್ದಳತೆ_ಹೊರಹೋಗು = ಉದ್ದಳತೆ;

  ಹಿಂತಿರುಗಿ !ferror (fp);
}

/* Calculate the checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ಪೂರ್ಣಾಂಕ
ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, intmax_t *ಉದ್ದಳತೆ)
{
  intmax_t ಒಟ್ಟು_ಬೈಟುಗಳು = 0;
  uint_fast32_t ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = 0;

  ಸ್ಥಿರ cksum_fp_t cksum_fp;
  ಆದರೆ (! cksum_fp)
    cksum_fp = avx512_supported ();
  ಆದರೆ (! cksum_fp)
    cksum_fp = avx2_supported ();
  ಆದರೆ (! cksum_fp)
    cksum_fp = pclmul_supported ();
  ಆದರೆ (! cksum_fp)
    cksum_fp = vmull_supported ();
  ಆದರೆ (! cksum_fp)
    cksum_fp = cksum_slice8;

  ಆದರೆ (! cksum_fp (ಹರಿವು, &ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, &ಒಟ್ಟು_ಬೈಟುಗಳು))
    ಹಿಂತಿರುಗಿ -1;

  *ಉದ್ದಳತೆ = ಒಟ್ಟು_ಬೈಟುಗಳು;

  ಗಾಗಿ (; ಒಟ್ಟು_ಬೈಟುಗಳು; ಒಟ್ಟು_ಬೈಟುಗಳು >>= 8)
    ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = (ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ << 8) ^ crctab[0][((ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ >> 24) ^ ಒಟ್ಟು_ಬೈಟುಗಳು) & 0xFF];
  ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ~ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ & 0xFFFFFFFF;

  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
  ಸ್ಮರಣೆ_ನಕಲು (resstream, &ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು, ಗಾತ್ರ ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು);

  ಹಿಂತಿರುಗಿ 0;
}

/* Calculate the crc32b checksum and length in bytes of stream STREAM.
   Return -1 on error, 0 on success.  */

ಪೂರ್ಣಾಂಕ
crc32b_ಮೊತ್ತ_ಹರಿವು (FILE *ಹರಿವು, ಶೂನ್ಯ *resstream, intmax_t *reslen)
{
  uint32_t buf[BUFLEN / ಗಾತ್ರ (uint32_t)];
  uint32_t ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = 0;
  intmax_t len = 0;
  size_t ಬೈಟುಗಳು_read;

  ಆದರೆ (!ಹರಿವು || !resstream || !reslen)
    ಹಿಂತಿರುಗಿ -1;

# ಆದರೆ GL_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_X86_64_PCLMUL
  ಆದರೆ (cksum_debug)
    (ಶೂನ್ಯ) pclmul_supported ();
# endif

  ಆಗ ((ಬೈಟುಗಳು_read = fread (buf, 1, BUFLEN, ಹರಿವು)) > 0)
    {
      ಆದರೆ (ckd_ಕೂಡಿಸು (&len, len, ಬೈಟುಗಳು_read))
        {
          errno = EOVERFLOW;
          ಹಿಂತಿರುಗಿ -1;
        }

      ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_32_update (ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ, (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *)buf, ಬೈಟುಗಳು_read);

      ಆದರೆ (feof (ಹರಿವು))
        ನಿಲ್ಲಿಸು;
    }

  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು = ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ;
  ಸ್ಮರಣೆ_ನಕಲು (resstream, &ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು, ಗಾತ್ರ ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ_ಹೊರಹೋಗು);

  *reslen = len;

  ಹಿಂತಿರುಗಿ ferror (ಹರಿವು) ? -1 : 0;
}

/* Print the checksum and size to stdout.
   If ARGS is true, also print the FILE name.  */

ಶೂನ್ಯ
output_ಚಕ್ರೀಯ_ಪರೀಕ್ಷೆ (ಅಕ್ಷರ ಸ್ಥಿರಾಂಕ *file, MAYBE_ಬಳಸದ ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ_file,
            ಶೂನ್ಯ ಸ್ಥಿರಾಂಕ *digest, bool raw, MAYBE_ಬಳಸದ bool tagged,
            ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ delim, bool ವಾದಗಳು, intmax_t ಉದ್ದಳತೆ)
{
  ಆದರೆ (raw)
    {
      /* Output in network byte order (big endian).  */
      uint32_t ಹೊರಹೋಗು_int = htobe32 (*(uint32_t *)digest);
      fwrite (&ಹೊರಹೋಗು_int, 1, 32/8, stdout);
      ಹಿಂತಿರುಗಿ;
    }

  ಬರೆ ("%u %jd", *(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ *)digest, ಉದ್ದಳತೆ);
  ಆದರೆ (ವಾದಗಳು)
    ಬರೆ (" %s", file);
  ಅಕ್ಷರ_ಬರೆ (delim);
}

#ಆದರೆ_ಅಂತ್ಯ /* !CRCTAB */
